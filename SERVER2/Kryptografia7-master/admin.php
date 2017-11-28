<?php
   include('admin_session.php');
   include('config2.php');
   session_start();
    if($_SERVER["REQUEST_METHOD"] == "POST") {
    	$target_id = $_POST['target_id'];

      $sql = "DELETE FROM pending WHERE pending.id = '$target_id'";
        $result = mysqli_query($db, $sql);
        if (!$result) {
            printf("Error: %s\n", mysqli_error($db));
            exit();
        }
    }
?>
<html>
<head>
<script src="https://ajax.googleapis.com/ajax/libs/jquery/3.1.1/jquery.min.js"></script>
<title>Bank - Panel Administratora</title>
<meta charset="utf-8" />
<meta name="viewport" content="width=device-width, initial-scale=1">
<link rel="stylesheet" href="style.css">
</head>
   <body>
      <div id="main_container">
   
		<header id="main_title">
			<h1> Witaj <?php echo $login_session; ?> w panelu administratora!</h1>
			<p id="note"><a href = "logout.php">Wyloguj się</a></p>
		</header>

		<div id="points_table_container">

		<h2 id="points_title">Przelewy do zatwierdzenia:</h2>

		<?php
		
		$sql = "SELECT pending.id, pending.sender, pending.recipient, pending.value, pending.execution_date FROM pending WHERE 1 = 1";
		$result = mysqli_query($db, $sql);
		
		echo '<table id="points_table">
			<tr>
				<th>id operacji:</th>
				<th>Z rachunku:</th>
				<th>Na rachunek:</th>
				<th>Data:</th>
				<th>Kwota:</th>
				<th>Zatwierdź:</th>
			</tr>';

		
		while($row = mysqli_fetch_assoc($result)) {
		echo '<tr>' .
		'<th>' . 
        $row["id"].
        '</th>' .
        '<th>' . 
        $row["sender"].
        '</th>' .
        '<th>' . 
        $row["recipient"].
        '</th>' .
        '<th>' . 
        $row["execution_date"].
        '</th>' .
        '<th>' . 
        $row["value"].
        '</th>' .
        '<th>' . 
        '<form name = "f" action = "" method = "post">
                  <input class="hidden" type = "text" name = "target_id" value =' .
                  (int)$row["id"] .
                  '/>
                  <input id = "submit" name = "submit" type = "submit" value = " Zatwierdź "/><br />
        </form>' .
        '</th>' .
        '</tr>';
    	}

    	echo '</table>';

		?>

		</div>

	</div>
	<footer>
		Kontakt z bankiem jedynie za pośrednictwem gołębia pocztowego!
	</footer>
   </body>
   
</html>