<?php
    $servername = "localhost";
    $dbname = "project";
    $username = "root";
    $password = "";

    //Open a new connection to MySQL server
    $mysqli = new mysqli($servername, $username, $password, $dbname);

    //Output connection errors
    if ($mysqli->connect_error)
    {
        die("[ERROR] Connection Failed: " . $mysqli->connect_error);
    }
?>