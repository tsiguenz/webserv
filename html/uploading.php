<?php
echo "yo";
error_reporting(0);
$filename = $_FILES["uploadfile"]["name"];
$tempname = $_FILES["uploadfile"]["tmp_name"];
$folder = "upload/".$filename;
move_uploaded_file($tempname,$folder);
echo "<img src='$folder' height=100 width=100 />";
?>
