<html>
    <head>
        <?php
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

            function createPlayer()
            {
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

                $playername = $_POST['playername'];
                $textcolor = substr($_POST['textcolor'], 1, 6);
                $backgroundcolor = substr($_POST['bgcolor'], 1, 6);

                //Set this player's "id" value.
                $query = "SELECT COUNT(*) AS c FROM players";
                $result = $conn->query($query);
                $row = mysqli_fetch_assoc($result);
                $playerid = intval($row['c']);

                $query = "INSERT INTO players (`id`, `name`, `wins`, `ties`, `losses`, `text_color`, `background_color`, `avatar`) VALUES ('$playerid', '$playername', '0', '0', '0', '$textcolor', '$backgroundcolor', '$uploads_dir'.'$pic_name')";
                $conn->query($query);
                $conn->close();
            }
        ?>
    </head>
    <footer>
	    <div class="Uploader">
		    <h3> FILE UPLOADING </h3>
		    <hr>
		    <form action="mtg_lobby.php" method="post" enctype="multipart/form-data">
			    <label for="file">Avatar:</label>
			    <input type="file" name="file" id="file" />
			    <br />
                <label for="playername">Player Name:</label>
                <input type="text" name="playername" id="playername" />
                <br />
                <label for="textcolor">Text Color:</label>
                <input type="color" name="textcolor" id="textcolor" />
                <br />
                <label for="bgcolor">Background Color:</label>
                <input type="color" name="bgcolor" id="bgcolor" />
                <br />
			    <input type="submit" name="submit" value="Create Player" />
		    </form>
		
		    <?php
			    if ($_FILES["file"]["error"] > 0)
			    {
			        echo "Error: " . $_FILES["file"]["error"] . "<br />";
			    }
			    else
			    {
			        echo "Upload: " . $_FILES["file"]["name"] . "<br />";
			        echo "Type: " . $_FILES["file"]["type"] . "<br />";
			        echo "Size: " . ($_FILES["file"]["size"] / 1024) . " Kb<br />";
			        echo "Stored in: " . $_FILES["file"]["tmp_name"];
			     
			        $uploads_dir = '/player_images/';
			        $tmp_name = $_FILES['file']['tmp_name'];
			        $pic_name = $_FILES['file']['name'];
			    
			        $ext = pathinfo($pic_name, PATHINFO_EXTENSION);
			        move_uploaded_file($tmp_name, $uploads_dir."0".".".$ext);
			    }

                if(isset($_SERVER['REQUEST_METHOD']) and $_SERVER['REQUEST_METHOD'] == "POST" and isset($_POST['submit']))
                {
                    createPlayer();
                    $_POST['submit'] = null;
                    echo '<meta http-equiv="refresh" content="1;url=create_game.php">';
                }
		    ?> 
	    </div>
	</footer>
</html>
