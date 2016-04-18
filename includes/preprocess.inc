<?php
/**
 * @file
 * Preprocessors.
 */

/**
 * Implements hook_preprocess_page().
 */
function zero_preprocess_page(&$vars) {
  // Preprocess page template.
}

/**
 * Implements hook_preprocess_node().
 */
function zero_preprocess_node(&$vars) {
  // Replace classes array with our own classes.
  $vars["classes_array"] = array("node");
}
