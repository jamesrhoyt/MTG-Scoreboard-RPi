<footer>
	<div class="Uploader">
		<h3> FILE UPLOADING </h3>
		<hr>
		<form action="mtg_lobby.php" method="post" enctype="multipart/form-data">
			<label for="file">Filename:</label>
			<input type="file" name="file" id="file" />
			<br />
			<input type="submit" name="submit" value="Submit" />
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
			  
				$uploads_dir = '../htdocs/player_images/';
				$tmp_name = $_FILES['file']['tmp_name'];
				$pic_name = $_FILES['file']['name'];
				
				$ext = pathinfo($pic_name, PATHINFO_EXTENSION);
				move_uploaded_file($tmp_name, $uploads_dir."0".".".$ext);
			}
		?> 
	</div>
	</footer>