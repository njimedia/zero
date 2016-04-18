<?php
/**
 * @file
 * template.php
 */

// Provide < PHP 5.3 support for the __DIR__ constant.
if (!defined('__DIR__')) {
  define('__DIR__', dirname(__FILE__));
}

require_once __DIR__ . '/includes/theme.inc';
require_once __DIR__ . '/includes/preprocess.inc';
