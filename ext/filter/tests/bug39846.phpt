--TEST--
Bug #39846 ipv4 trailing data validation
--FILE--
<?php
var_dump(filter_var('192.168.1.100random-text-here', FILTER_VALIDATE_IP));
var_dump(filter_var("192.168.1.155\0foo", FILTER_VALIDATE_IP));
?>
--EXPECT--	
bool(false)
bool(false)
