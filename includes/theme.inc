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

  $exclude = array(
    'settings' => FALSE,
    'misc/jquery.js' => FALSE,
    'misc/drupal.js' => FALSE,
    'misc/jquery.once.js' => FALSE,
  );

  $js = array_diff_key($js, $exclude);
}
