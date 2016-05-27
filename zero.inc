<?php
/**
 * @file
 * Zero base class.
 */

namespace Zero;

/**
 * Theme Class.
 */
class Theme {

  /**
   * CMP Sorting function.
   */
  private static function cmp($a, $b) {
    if ($a['weight'] === $b['weight']) {
      return 0;
    }
    return ($a['weight'] < $b['weight']) ? -1 : 1;
  }


  /**
   * Get menu.
   */
  public static function getMenu($name) {
    $menu = menu_load_links($name);
    $links = array();
    $children = array();

    foreach ($menu as $link) {

      $path = $link['link_path'];
      $id = $link['mlid'];
      $pid = $link['plid'];
      $title = $link['link_title'];
      $depth = $link['depth'];
      $has_children = $link['has_children'];
      $weight = $link['weight'];
      $nid = '';

      if ($path === '<front>' || $path === '') {
        $front_url = variable_get('site_frontpage', 'node');
        $front_url = drupal_get_normal_path($front_url);
        $nid = $front_url;
        $path = '/';
      }
      else {
        $alias = drupal_get_path_alias($path);
        $nid = $path;
        // Use $path if no $alias.
        if ($path === $alias) {
          $path = $path;
        }
        else {
          $path = '/' . $alias;
        }
      }

      if ($title === '') {
        $title = 'UNTITLED';
      }

      $nid_split = explode('/', $nid);
      if ($nid_split[0] === 'node' && isset($nid_split[1])) {
        $nid = $nid_split[1];
      }
      else {
        $nid = NULL;
      }

      // Create array.
      $links[$id] = array(
        'key' => $id,
        'title' => $title,
        'path' => $path,
        'weight' => $weight,
        'nid' => $nid,
      );

      // Add second level link information.
      if ($depth === '1' && $has_children == '1') {
        $links[$id]['has_children'] = '1';
        $links[$id]['children'] = array();
        $links[$id]['below'] = array();
      }
      if ($depth !== '1') {
        $links[$id]['depth'] = $depth;
        $children[$id] = $pid;
      }
    }

    foreach ($children as $id => $pid) {
      $links[$pid]['children'][] = array(
        'id' => (string) $id,
        'pid' => (string) $pid,
      );
    }

    foreach ($links as $key => $value) {
      if (isset($links[$key]['children'])) {
        $child_ids = $links[$key]['children'];
        foreach ($child_ids as $child_id) {
          $child_id = $child_id['id'];
          $links[$key]['below'][] = $links[$child_id];
        }
      }
    }

    foreach ($links as $key => $value) {
      if (isset($links[$key]['depth'])) {
        unset($links[$key]);
      }
    }

    usort($links, array("Zero\Theme", "cmp"));

    $output = array(
      'links' => $links,
    );

    return $output;
  }

  /**
   * Get menus.
   */
  public static function getMenus($names) {

    $menu = array();
    foreach ($names as $key => $name) {
      if ($name === "main-menu") {
        $menu[$name] = Theme::getMenu($name);
      }
      else {
        $menu[$name] = Theme::getMenu("menu-" . $name);
      }
      $menu[$name]["title"] = ucwords($name);
    }

    return $menu;

  }

}

/**
 * Get more out of entities.
 */
class Entity {

  /**
   * Get all field values for an entity.
   *
   * @param object $entity
   *   Entity Object (eg. node).
   * @param string $bundle
   *   Bundle (eg. content type).
   * @param string $entity_type
   *   Type of entity (eg. "node").
   */
  public function __construct($entity, $bundle, $entity_type = "node") {
    $this->wrapper = entity_metadata_wrapper($entity_type, $entity, array('bundle' => $bundle));
    $this->info = $this->wrapper->getPropertyInfo();
  }

  /**
   * Generate a fields array.
   */
  public function getFieldsArray($wrapper = NULL, $info = NULL) {

    $info = !is_null($info) ? $info : $this->info;
    $wrapper = !is_null($wrapper) ? $wrapper : $this->wrapper;

    $out = array();

    foreach ($info as $key => $value) {
      $info = $wrapper->$key->info();
      $type = $info["type"];
      if ($type === "list<field_collection_item>") {
        $out[$key] = $this->processLists($wrapper->$key);
      }
      elseif ($type === "node") {
        // Return a EntityDrupalWrapper to do whatever with.
        $out[$key] = $wrapper->$key;
      }
      else {
        $out[$key] = $wrapper->$key->value();
      }

    }

    return $out;

  }

  /**
   * Return the wrapper.
   */
  public function getWrapper() {
    return $this->wrapper;
  }


  /**
   * Process fields with multiple values.
   */
  private function processLists($entity_list_wrapper) {

    $out = array();

    // Get how many values we are dealing with here.
    $count = $entity_list_wrapper->count();

    // Look through all items to get each item.
    for ($i = 0; $i < $count; $i++) {
      // Wrapper is an EntityDrupalWrapper.
      $wrapper = $entity_list_wrapper->get($i);
      $info = $wrapper->getPropertyInfo();
      $out[$i] = $this->getFieldsArray($wrapper, $info);
    }

    return $out;

  }

}
