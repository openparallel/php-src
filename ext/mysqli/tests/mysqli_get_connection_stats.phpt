--TEST--
mysqli_get_connection_stats()
--SKIPIF--
<?PHP
require_once('skipif.inc');
require_once('skipifemb.inc');
require_once('skipifconnectfailure.inc');
if (!function_exists('mysqli_get_connection_stats')) {
	die("skip only available with mysqlnd");
}
?>
--FILE--
<?php
	$tmp = $link = null;
	if (!is_null($tmp = @mysqli_get_connection_stats()))
		printf("[001] Expecting NULL, got %s/%s\n", gettype($tmp), $tmp);

	if (!is_null($tmp = @mysqli_get_connection_stats($link)))
		printf("[002] Expecting NULL, got %s/%s\n", gettype($tmp), $tmp);

	include "connect.inc";
	include "table.inc";

	if (!is_array($info = mysqli_get_connection_stats($link)) || empty($info))
		printf("[003] Expecting array/any_non_empty, got %s/%s\n", gettype($info), $info);

	if (!is_array($info2 = mysqli_get_client_stats()) || empty($info2))
		printf("[004] Expecting array/any_non_empty, got %s/%s\n", gettype($info2), $info2);

	if ($info !== $info2) {
		printf("[005] The hashes should be identical\n");
		var_dump($info);
		var_dump($info2);
	}

	mysqli_close($link);
	include "table.inc";

	if (!is_array($info = mysqli_get_connection_stats($link)) || empty($info))
		printf("[006] Expecting array/any_non_empty, got %s/%s\n", gettype($info), $info);

	if (!is_array($info2 = mysqli_get_client_stats()) || empty($info2))
		printf("[007] Expecting array/any_non_empty, got %s/%s\n", gettype($info2), $info2);

	// assuming the test is run in a plain-vanilla CLI environment
	if ($info === $info2) {
		printf("[008] The hashes should not be identical\n");
		var_dump($info);
		var_dump($info2);
	}

	print "done!";
?>
--EXPECTF--
done!