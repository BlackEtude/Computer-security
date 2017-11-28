<?php
   include('session.php');
   include('config.php');
   session_start();

   $number = '';
   if($_SERVER["REQUEST_METHOD"] == "POST") {
    	// echo "POST sended";
    	$number = $_POST['number'];
    }
?>
<html>
<head>
<script src="https://ajax.googleapis.com/ajax/libs/jquery/3.1.1/jquery.min.js"></script>
<title>Bank - Panel Główny</title>
<meta charset="utf-8" />
<meta name="viewport" content="width=device-width, initial-scale=1">
<link rel="stylesheet" href="style.css">
</head>
   <body>
      <div id="main_container">
   
		<header id="main_title">
			<h1> Witaj <?php echo $login_session; ?> w swoim banku! Nr konta: <?php echo $login_number; ?></h1>
			<p id="note"><a href = "logout.php">Wyloguj się</a></p>
		</header>

		<div id="points_table_container">

		<h2 id="points_title">Stan konta: <?php echo $balance ?> BTC </h2>

		<!-- <h2 id="points_title">Otrzymane przelewy</h2> -->

		<?php
		
		// $sql = "SELECT history.sender, history.value, history.execution_date FROM history WHERE history.recipient = $login_number";

		// $result = mysqli_query($db, $sql);

		
		// echo '<table id="points_table">
		// 	<tr>
		// 		<th>Z rachunku:</th>
		// 		<th>Data:</th>
		// 		<th>Kwota:</th>
				
		// 	</tr>';

		
		// while($row = mysqli_fetch_assoc($result)) {
		// echo '<tr>' .
  //       '<th>' . 
  //       $row["sender"].
  //       '</th>' .
  //       '<th>' . 
  //       $row["execution_date"].
  //       '</th>' .
  //       '<th>' . 
  //       $row["value"].
  //       '</th>' .
  //       '</tr>';
  //   	}

  //   	echo '</table>';

		?>

		<h2 id="points_title">Wysłane przelewy</h2>

		<form name = "from1" action = "" method = "post">
                  <label>Nr rachunku:</label><input  type="text" name = "number"/><br /><br />
                  <input name = "submit" type = "submit" value = " Filter "/><br />
        </form>

		<?php

		$sql = "SELECT history.sender, history.recipient, history.value, history.execution_date FROM history 
		WHERE history.sender=$login_number && history.recipient = $number";

		$result = mysqli_multi_query($db, $sql);

		echo '<table id="points_table">
			<tr>
				<th>Z rachunku:</th>
				<th>Na rachunek:</th>
				<th>Data:</th>
				<th>Kwota:</th>
				
			</tr>';
		
		
		while($row = mysqli_fetch_assoc($result)) {
		echo '<tr>' .
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
        '</tr>';
		}
        echo '</table>';

		?>

		<div id="communicates">
			<h2 id="communicates_title"><a href = "form1.php">Wykonaj przelew</a></h2>		
		</div>

		</div>

	</div>
	<footer>
		Kontakt z bankiem jedynie za pośrednictwem gołębia pocztowego!
	</footer>
   </body>
   
</html>