<?php
/*** set the content type header ***/
	header("Content-type: text/css");

	session_start();
	
	$servername = "localhost";
	$username = "root";
	$password = "mtgscoreboard";
	$dbname = "mtg";
	
	// Create connection
	$conn = new mysqli($servername, $username, $password, $dbname);
	// Check connection
	if ($conn->connect_error) {
		die("Connection failed: " . $conn->connect_error);
	} 

	
	$sql = "SELECT COUNT(player_id) as total FROM teams Where game_id =".$_SESSION['game'];
	$result = $conn->query($sql);

	if ($result->num_rows == 1) {
		while($row = $result->fetch_assoc()) {
			$numPlayers = $row['total'];
		}
	} else {
		echo "Players Couldn't be found!";
	}
	
	
	$sql = "SELECT id, life, player_id FROM teams Where game_id =".$_SESSION['game'];
	$result = $conn->query($sql);

	if ($result->num_rows > 0) {
		// output data of each row
		$posID = 0;
		while($row = $result->fetch_assoc()) {
			// echo "Welcome to ".$row['id'];
			$sq2 = "SELECT name, background_color, text_color FROM players Where id =".$row['player_id'];
			$playerInfo = $conn->query($sq2);
			
			/*
			.player_0 {
				position: fixed;
				top: 0px;
				left: 0px;
				height: 416px;
				width: 1600px;
				background-color: coral;
				border: 10px solid #313131;
			}  
			*/
			
			
			if ($playerInfo->num_rows == 1) {
				while($info = $playerInfo->fetch_assoc()) {
					// echo "Player Name ".$info['name'];
					$display = '#player_'.$posID++.' {';
					$display .= 'position: fixed;';
					$display .= 'border: 10px solid #313131;';
					$display .= 'background-color: #'.$info['background_color'].';';
					$display .= 'color: #'.$info['text_color'].';';
					$display .= '}';
					echo $display;
				}
			} else {
				echo "Bad Player Info!";
			}
		}
	} else {
		echo "Couldn't Find Players!";
	}
	
	$conn->close();
	
	/*
	$containerSize = 100;
	if ( $numPlayers > 0){
		$containerSize = $containerSize/$numPlayers;
	}
	*/
?>

.player_life {
	font-size: 23.5vh;
	line-height: 1.0;
	white-space: nowrap; 
}  

.player_buttons {
	display: block;
}

input[type=submit] {
    width: 50%;
	height: 21.5vh;
	font-size: 10vh;
}

body, html {
    margin: 0px;
    padding: 0px;
	overflow: hidden;
	background-color: #313131;
    font-family: 'Trebuchet MS';
}