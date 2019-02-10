<html>
 <head>
  <title>Magic Lobby</title>
  <link rel="stylesheet" type="text/css" href="mtgstyle.css">
  <?php
		// Set Session variables
		session_start();
		if(isset($_SESSION["players"])){ 
			$_SESSION["players"] = array();
		}
	?>
 </head>
 <body>
 
	<header>
			<span>Active Magic Games</span>
	</header>
		
	<nav>
	</nav>
	
	<div class="Container">
		
		<section class="main_section">
			 <?php
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

			$sql = "SELECT id, name FROM games WHERE active = 1";
			$result = $conn->query($sql);

			if ($result->num_rows > 0) {
				$select= '<form action="/game.php" target="/game.php" method="post">';
				// $select.= '<p>Active Magic Games:</p>';
				$select.= '<select name="game" id="game" multiple>';
				// output data of each row
				while($row = $result->fetch_assoc()) {
					$select.='<option value="'.$row['id'].'">'.$row['name'].'</option>';
				}
				
				$select.='</select>';
				$select.='<p><input type="submit" id="join" value="Join Game"></p>';
				$select.='</form>';
				
				echo $select;
			} else {
				echo "No Active Magic Games!";
			}

			$conn->close();
			?>
		</section>
		<aside class="side_section">
			<input type="button" id="create" value="Create Game" onclick="location='create_game.php'" />
		</aside>
	</div>
 </body>
</html>




