<?php
	$conn=mysqli_connect("localhost","root","","chemistbot");
	if(!$conn){
		die("No s'ha pogut conectar al server:" .mysql_error());
	}
?>
