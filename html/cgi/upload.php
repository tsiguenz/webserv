<?php
$str = "";
error_reporting(0);
$filename = $_FILES;
$a =  $_FILES['file'];
$img = $a['full_path'];

echo "<img src='../upload/$img'/>";
?>