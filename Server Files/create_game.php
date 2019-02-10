<html>
 <head>
  <title>Create Game</title>
  <link rel="stylesheet" type="text/css" href="mtgstyle.css">
  
	<?php
		// Set Session variables
		session_start();
		if(isset($_SESSION["players"])){ 
			if(isset($_POST["player"])){ 
				if(in_array($_POST["player"], $_SESSION["players"])){
				}
				else{
					array_push($_SESSION["players"], $_POST["player"]);
				}
			}
		}
		else{
			$_SESSION["players"] = array();
		}
		
		// Setup mysql connection
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
	?>
 </head>
 <body>
 
	<header>
			<span>Game Builder</span>
	</header>
		
	<nav>
	</nav>
	
	<div class="Container">
		
		<section class="main_section">
			<?php
				$sql = "SELECT id, name FROM players";
				$result = $conn->query($sql);

				if ($result->num_rows > 0) {
					$select= '<form action="/create_game.php" target="/create_game.php" method="post">';
					$select.= '<p>Choose Players:</p>';
					$select.= '<select name="player" id="game" multiple>';
					// output data of each row
					while($row = $result->fetch_assoc()) {
						$select.='<option value="'.$row['id'].'">'.$row['name'].'</option>';
					}
					
					$select.='</select>';
					$select.='<p><input type="submit" id="join" value="Add Player"></p>';
					$select.='</form>';
					
					echo $select;
				} else {
					echo "No Active Magic Games!";
				}

				$conn->close();
			?>
		</section>
		<aside class="side_section">
			<p>Active Players:</p>
			
			<?php
				$players_selected= '';
				foreach ($_SESSION["players"] as &$value) {
					$players_selected.= '<p>'.$value.'</p>';
				}
				unset($value); // break the reference with the last element
				
				echo $players_selected;
			?>
			<form action="create_game.php" method="post">
				<input type="submit" name="clearCommand" value="Clear Players" />
			</form>
			<input type="button" id="createP" value="Create Player" onclick="location='mtg_lobby.php'" />
		</aside>
	</div>
	
	<input type="button" id="create" value="Create Game" onclick="location='mtg_lobby.php'" />
	
	<p>Game Type:</p>
	<form>
	  <input type="radio" name="gtype" value="s" checked> Standard<br>
	  <input type="radio" name="gtype" value="c"> Commander (EDH)<br>
	  <input type="radio" name="gtype" value="e"> Emperor
	</form>
	
	<p>Team Size:</p>
	<form>
	  <input type="radio" name="tsize" value="1" checked> Single<br>
	  <input type="radio" name="tsize" value="2"> Two-Headed Giant<br>
	  <input type="radio" name="tsize" value="3"> Three-Headed Giant
	</form>
	
	
	<?php
		if($_SERVER['REQUEST_METHOD'] == "POST" and isset($_POST['clearCommand']))
		{
			$_SESSION["players"] = array();
			$_POST['clearCommand'] = null;
			echo '<meta http-equiv="refresh" content="0">';
		}
	?>
 </body>
</html>




