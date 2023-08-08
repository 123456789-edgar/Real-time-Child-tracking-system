<?php 
session_start(); 
include "db_conn.php";

if (isset($_POST['username']) && isset($_POST['password'])) {

	function validate($data){
       $data = trim($data);
	   $data = stripslashes($data);
	   $data = htmlspecialchars($data);
	   return $data;
	}

	$username = validate($_POST['username']);
	$password = validate($_POST['password']);

	if (empty($username)) {
		header("Location: index.php?error=Parent/Guardian name is required");
	    exit();
	}else if(empty($password)){
        header("Location: index.php?error=Child's name is required");
	    exit();
	}else{
		$sql = "SELECT * FROM users WHERE user_name='$username' AND password='$password'";

		$result = mysqli_query($conn, $sql);

		if (mysqli_num_rows($result) === 1) {
			$row = mysqli_fetch_assoc($result);
            if ($row['user_name'] === $username && $row['password'] === $password) {
            	$_SESSION['user_name'] = $row['user_name'];
            	$_SESSION['name'] = $row['name'];
            	$_SESSION['id'] = $row['id'];
            	header("Location: home.php");
		        exit();
            }else{
				header("Location: index.php?error=Incorrect Parent/Guardian name or Child's name");
		        exit();
			}
		}else{
			header("Location: index.php?error=Incorrect Parent/Guardian name or Child's name");
	        exit();
		}
	}
	
}else{
	header("Location: index.php");
	exit();
}
