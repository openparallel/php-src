--TEST--
Phar::getSupportedSignatures()
--SKIPIF--
<?php if (!extension_loaded("phar")) die("skip"); ?>
<?php if (extension_loaded("hash")) die("skip extension hash conflicts"); ?>
<?php if (!extension_loaded("openssl")) die("skip extension openssl loaded"); ?>
--INI--
phar.require_hash=0
phar.readonly=0
--FILE--
<?php
var_dump(Phar::getSupportedSignatures());
?>
===DONE===
?>
--EXPECT--
array(3) {
  [0]=>
  string(3) "MD5"
  [1]=>
  string(5) "SHA-1"
  [2]=>
  string(7) "OpenSSL"
}
===DONE===
