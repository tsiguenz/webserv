<?php
	session_start();
?>
<!DOCTYPE html>
<html>
	<style>
		body {
                background-image: url(https://wallpaperaccess.com/full/427004.jpg);
                background-position: center center;
                background-repeat: no-repeat;
                background-attachment: fixed;
                background-size: cover;
            }
		.gap-50 { 
            		width:100%; 
            		height:50px; 
			    }  
	</style>
	<head>
		<title>compteur de cookies</title>
	</head>
	<body style= "text-align:center">
		<div class="gap-50"></div> 
		<h1>compteur de cookies</h1> 
		<?php
			//TODO faire une page un peu styley
			if (!isset($_SESSION["compt"]))
				$_SESSION["compt"] = 1;
			else
				$_SESSION["compt"] += 1;
			$compt = $_SESSION["compt"];
			echo "<h1>$compt</h1>";
		?>
	</body>
</html>
