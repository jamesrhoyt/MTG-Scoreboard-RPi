<?php
   $url=$_SERVER['REQUEST_URI'];
   header("Refresh: 5; URL=$url");
?>

<html>
<head>
	<link rel="stylesheet" href="game_style.php" media="screen">
	<?php
		// Set Session variables
		session_start();
		if(isset($_POST["game"])){ //first time
			$_SESSION['game'] = $_POST["game"];
		}
		
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

		$sql = "SELECT name FROM games Where id =".$_SESSION['game'];
		$result = $conn->query($sql);

		if ($result->num_rows == 1) {
			// output data of each row
			while($row = $result->fetch_assoc()) {
				// echo "Welcome to ".$row['name'];
				$gameName = $row['name'];
			}
		} else {
			echo "Couldn't Find Magic Game!";
		}
		$conn->close();
	?>
	<title><?php echo $gameName; ?></title>
</head>

<body>
<!-- Welcome <?php echo $_POST["game"]; ?><br> 
-->

<?php
/*
$width = "<script type=\"text/javascript\"> document.write(window.innerWidth);; </script>";
echo "width = ".$width;
$_SESSION['width'] = $width;
 
$height = "<script type=\"text/javascript\">document.write(window.innerHeight);;</script>";
$_SESSION['height'] = $height;
echo  "height = ".$height;
*/
?>

<?php	
	$servername = "localhost";
	$username = "root";
	$password = "";
	$dbname = "mtg";

	// Create connection
	$conn = new mysqli($servername, $username, $password, $dbname);
	// Check connection
	if ($conn->connect_error) {
		die("Connection failed: " . $conn->connect_error);
	} 

	$sql = "SELECT id, life, player_id FROM teams Where game_id =".$_SESSION['game'];
	$result = $conn->query($sql);

	if ($result->num_rows > 0) {
		// output data of each row
		$posID = 0;
		while($row = $result->fetch_assoc()) {
			// echo "Welcome to ".$row['id'];
			$sq2 = "SELECT name, avatar FROM players Where id =".$row['player_id'];
			$playerInfo = $conn->query($sq2);
			
			if ($playerInfo->num_rows == 1) {
				while($info = $playerInfo->fetch_assoc()) {
					// echo "Player Name ".$info['name'];
					$display = '<div id="player_'.$posID.'">';
					$display .= '<form action="game.php" method="post">';
					$display .= '<div class="player_life" id="name_'.$posID.'">';
					$display .= '<img src="'.$info['avatar'].'" id="image_'.$posID.'" alt="Picture Missing!" style="width:100px;height:100px;">';
					$display .= ' '.$info['name'].'';
					$display .= '</div>';
					$display .= '<div class="life_new" id="life_'.$posID.'">';
					$display .= $row['life'];
					$display .= '</div>';
					$display .='<input type="hidden" name="playerId" value="'.$row['id'].'">';
					$display .= '<div class="player_buttons">';
					$display .='<input type="submit" name="AddValue" value="+" />';
					$display .='<input type="submit" name="SubValue" value="-" />';
					$display .= '</div>';
					$display .='</form>';
					$display .= '</div>';
					echo $display;
					$posID++;
				}
			} else {
				echo "Bad Player Info!";
			}
		}
	} else {
		echo "Couldn't Find Players!";
	}
	$conn->close();
	?>

<?php
    if($_SERVER['REQUEST_METHOD'] == "POST" and isset($_POST['AddValue']))
    {
        addLife();
		$_POST['AddValue'] = null;
		echo '<meta http-equiv="refresh" content="0">';
    }
    function addLife()
    {
        echo "+ Button Pressed for player ".$_POST['playerId'];   
		
		$servername = "localhost";
		$username = "root";
		$password = "";
		$dbname = "mtg";

		// Create connection
		$conn = new mysqli($servername, $username, $password, $dbname);
		// Check connection
		if ($conn->connect_error) {
			die("Connection failed: " . $conn->connect_error);
		} 
		
		$sql = "UPDATE teams SET life = life + 1 Where id =".$_POST['playerId'];
		$result = $conn->query($sql);
		
		$conn->close();
    }
	if($_SERVER['REQUEST_METHOD'] == "POST" and isset($_POST['SubValue']))
    {
        subLife();
		$_POST['SubValue'] = null;
		echo '<meta http-equiv="refresh" content="0">';
    }
    function subLife()
    {
        echo "- Button Pressed for player ".$_POST['playerId'];  

		$servername = "localhost";
		$username = "root";
		$password = "";
		$dbname = "mtg";

		// Create connection
		$conn = new mysqli($servername, $username, $password, $dbname);
		// Check connection
		if ($conn->connect_error) {
			die("Connection failed: " . $conn->connect_error);
		} 
		
		$sql = "UPDATE teams SET life = life - 1 Where id =".$_POST['playerId'];
		$result = $conn->query($sql);
		
		$conn->close();
    }
?>
<script>
	var borderSize = 10;
	var life_width = ((window.innerHeight/2)-borderSize)*1.25;
	var player = document.getElementById("player_0");
	player.style.top = "0px";
	player.style.left = "0px";
	player.style.height = (window.innerHeight/2)-borderSize;
	player.style.width = window.innerWidth-(2*borderSize);
	
	player = document.getElementById("player_1");
	player.style.top = (window.innerHeight/2)-borderSize;
	player.style.left = "0px";
	player.style.height = (window.innerHeight/2)-borderSize;
	player.style.width = window.innerWidth-(2*borderSize);
	
	var image = document.getElementById("image_0");
	image.style.height  = ((window.innerHeight/2)-borderSize)/2;
	image.style.width  = ((window.innerHeight/2)-borderSize)/2;
	
	image = document.getElementById("image_1");
	image.style.height  = ((window.innerHeight/2)-borderSize)/2;
	image.style.width  = ((window.innerHeight/2)-borderSize)/2;
	
	var elem = document.getElementById("name_0");
	elem.style.fontSize = ((window.innerHeight/2)-borderSize)/2;
	elem.style.width = window.innerWidth-(2*borderSize)-life_width;	
	elem.style.cssFloat = 'left';
	
	
	elem = document.getElementById("name_1");
	elem.style.fontSize = ((window.innerHeight/2)-borderSize)/2;
	
	
	var life = document.getElementById("life_0");
	life.style.fontSize = 3*((window.innerHeight/2)-borderSize)/4;
	life.style.top = "0px";
	life.style.left =  window.innerWidth-(2*borderSize)-life_width;
	
	life.style.height = (window.innerHeight/2)-borderSize;
	life.style.width = life_width;
	
	/*
	life.style.borderColor = "#000";
	life.style.borderStyle = "solid";
	life.style.borderWidth = "10px";
	*/
	life.style.cssFloat = 'right';
	// alert( window.innerWidth-(2*borderSize)-life_width);
</script>
</body>
</html>

