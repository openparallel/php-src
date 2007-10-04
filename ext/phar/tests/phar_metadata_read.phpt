--TEST--
Phar with phar-level meta-data (read)
--SKIPIF--
<?php if (!extension_loaded("phar")) print "skip";?>
--INI--
phar.require_hash=0
--FILE--
<?php
$fname = dirname(__FILE__) . '/' . basename(__FILE__, '.php') . '.phar.php';
$pname = 'phar://' . $fname;
$file = "<?php __HALT_COMPILER(); ?>";

$files = array();
$pmeta = 'hi there';
$files['a'] = array('cont' => 'a');
$files['b'] = array('cont' => 'b');
$files['c'] = array('cont' => 'c', 'meta' => array('hi', 'there'));
$files['d'] = array('cont' => 'd', 'meta' => array('hi'=>'there','foo'=>'bar'));
include 'phar_test.inc';

foreach($files as $name => $cont) {
	var_dump(file_get_contents($pname.'/'.$name));
}

$phar = new Phar($fname);
var_dump($phar->getMetaData());
foreach($files as $name => $cont) {
	var_dump($phar[$name]->getMetadata());
}

unset($phar);

foreach($files as $name => $cont) {
	var_dump(file_get_contents($pname.'/'.$name));
}
?>
===DONE===
--CLEAN--
<?php unlink(dirname(__FILE__) . '/' . basename(__FILE__, '.clean.php') . '.phar.php'); ?>
--EXPECT--
string(1) "a"
string(1) "b"
string(1) "c"
string(1) "d"
string(8) "hi there"
NULL
NULL
array(2) {
  [0]=>
  string(2) "hi"
  [1]=>
  string(5) "there"
}
array(2) {
  ["hi"]=>
  string(5) "there"
  ["foo"]=>
  string(3) "bar"
}
string(1) "a"
string(1) "b"
string(1) "c"
string(1) "d"
===DONE===
