<?php 
session_start();

if (isset($_SESSION['id']) && isset($_SESSION['user_name'])) {

 ?>
<!DOCTYPE html>
<html>
<head>
	<title>Home</title>
	<link rel="stylesheet" type="text/css" href="style.css">
	<script type="text/javascript" src="http://maps.googleapis.com/maps/api/
js?AIzaSyDRbnGahOxW-y6yjMRpHO7Bzvan_-NqlL0sensor=false"></script> 

<script>
function init()
{
    <?php
    include('util-dbconn.php');
    
    $sql = "SELECT * FROM `GPS_TRACKER_DATA`";
    $result = $mysqli->query($sql);
    $resultCount = $result->num_rows;
    
    $zoomLatitude = "";
    $zoomLongitude = "";
    
    echo "var locations = [";
    
    if ($resultCount > 0)
    {
        $currentRow = 0;
        
        while($row = $result->fetch_assoc())
        {
            $currentRow = $currentRow + 1;
            $clientID=$row["CLIENT_ID"];
            $latitude=$row["LATITUDE"];
            $longitude=$row["LONGITUDE"];
            
            if($currentRow == 1)
            {
                $zoomLatitude = $latitude;
                $zoomLongitude = $longitude;
            }
            
            echo "['".$clientID."',".$latitude.",".$longitude."]";
            
            if($currentRow < $resultCount)
            {
                echo ",";
            }
        }
    }
				
    echo "];";
    
    echo "var latitude = '$zoomLatitude';";
    echo "var longitude = '$zoomLongitude';";
				
    $mysqli->close();
    ?>
				
    map = new google.maps.Map(document.getElementById('map'),
                              {
                              zoom: 10,
                              center: new google.maps.LatLng(latitude, longitude),
                              mapTypeId: google.maps.MapTypeId.ROADMAP
                              });
    
    var infowindow = new google.maps.InfoWindow();
    
    var marker, i;
    
    for (i = 0; i < locations.length; i++)
    {
        marker = new google.maps.Marker({position: new google.maps.LatLng(locations[i][1], locations[i][2]),map: map});
        
        google.maps.event.addListener(marker, 'click', (function(marker, i)
                                                        {
                                                        return function()
                                                        {
                                                        infowindow.setContent(locations[i][0]);
                                                        infowindow.open(map, marker);
                                                        }
                                                        })(marker, i));
    }
}
</script>

</head>
<body style="background-image: url('images/1.jpg');background-repeat: no-repeat;
  background-attachment: fixed;  
  background-size: cover;">
     <h1>Hello, <?php echo $_SESSION['name']; ?></h1>
     <div id="map" style="width: 100%; height: 65%; margin-top: 50px;"></div>

<script type="text/javascript">
init();
</script>
<br>
     <a href="logout.php">Logout</a>
</body>
</html>

<?php 
}else{
     header("Location: index.php");
     exit();
}
 ?>