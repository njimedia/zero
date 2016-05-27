<?php
/**
 * @file
 * Preprocessors.
 */

require_once drupal_get_path('theme', 'zero') . "/zero.inc";

/**
 * Implements hook_preprocess_html().
 */
function zero_preprocess_html(&$vars) {

  $menus = array(
    "main-menu",
  );
  // Retrieve Menus.
  $main_menu = Zero\Theme::getMenus($menus);

  $vars["main_menu"] = $main_menu;

}

/**
 * Implements hook_preprocess_page().
 */
function zero_preprocess_page(&$vars) {
  // Preprocess page template.
  if (isset($vars["node"])) {
    $node = $vars["node"];
    $node_type = $node->type;
    if (!$vars['is_front']) {
      $vars['theme_hook_suggestions'][] = 'page__' . $node_type;
    }
    $entity = $vars["entity"] = new Zero\Entity($node, $node_type);
    $fields = $vars["fields"] = $entity->getFieldsArray();
    $wrapper = $vars["wrapper"] = $entity->getWrapper();

  }
}

/**
 * Implements hook_preprocess_node().
 */
function zero_preprocess_node(&$vars) {
  // Replace classes array with our own classes.
  $vars["classes_array"] = array("node");
}
