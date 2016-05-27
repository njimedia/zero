<?php
/**
 * @file
 * theme.inc
 */

/**
 * Implements hook_css_alter().
 */
function zero_css_alter(&$css) {
  $exclude = array(
    'modules/system/system.base.css' => FALSE,
    'modules/system/system.messages.css' => FALSE,
    'modules/system/system.theme.css' => FALSE,
    'modules/system/system.menus.css' => FALSE,
    'modules/comment/comment.css' => FALSE,
    'modules/field/theme/field.css' => FALSE,
    'modules/node/node.css' => FALSE,
    'modules/search/search.css' => FALSE,
    'modules/user/user.css' => FALSE,
  );
  $css = array_diff_key($css, $exclude);
}

/**
 * Implements hook_js_alter().
 */
function zero_js_alter(&$js) {
  // $jquery_path = '/' . drupal_get_path('theme', 'my') . '/assets/js/jquery.min.js';
  $jquery_path = 'https://ajax.googleapis.com/ajax/libs/jquery/1.11.1/jquery.min.js';
  $js['misc/jquery.js']['data'] = $jquery_path;
  $js['misc/jquery.js']['version'] = '1.11.1';
  $js['misc/jquery.js']['type'] = 'external';

  // $exclude = array(
  //   'settings' => FALSE,
  //   'misc/jquery.js' => FALSE,
  //   'misc/drupal.js' => FALSE,
  //   'misc/jquery.once.js' => FALSE,
  // );
  //
  // $js = array_diff_key($js, $exclude);
}

/**
 * Implements hook_menu_link().
 */
function zero_menu_link($vars) {
  $element = $vars["element"];
  $link = l($element["#title"], $element["#href"], $element["#localized_options"]);
  $submenu = _zero_nested_menus($element);
  return "<li" . drupal_attributes($element["#attributes"]) . ">" . $link . implode("\n", $submenu) . "</li>\n";
}

/**
 * Implements hook_menu_tree().
 */
function zero_menu_tree(&$vars) {
  return "<ul>" . $vars["tree"] . "</ul>";
}

/**
 * Find all nested menus for a menu.
 *
 * @param array $element
 *   Element array.
 *
 * @return string
 *   Submenu with nested submenus.
 */
function _zero_nested_menus($element) {

  $submenu = array();

  // If below add below and recurse.
  if (!empty($element["#below"])) {
    unset($element["#below"]["#sorted"]);
    unset($element["#below"]["#theme_wrappers"]);
    $submenu[] = "<ul>";
    foreach ($element["#below"] as $key => $value) {
      $subsubmenu = _zero_nested_menus($value);
      if (!empty($subsubmenu)) {
        $submenu[] = "<li>" . l($value["#title"], $value["#href"], $value["#localized_options"]) . implode("\n", $subsubmenu) . "</li>";
      }
      else {
        $submenu[] = "<li>" . l($value["#title"], $value["#href"], $value["#localized_options"]) . "</li>";
      }
    }
    $submenu[] = "</ul>";
  }

  return $submenu;

}
