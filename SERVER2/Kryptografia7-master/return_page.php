<?php
   include('session.php');
   include('config.php');
   session_start();
   if($_SERVER["REQUEST_METHOD"] == "POST") {
		header("location:main.php");
   $target_recipient = $_SESSION['recipient_number'];
   $target_value = $_SESSION['transfer_value'];
   $sql = "INSERT INTO pending(recipient, sender, value, execution_date) VALUES ($target_recipient, $login_number, $target_value, now())";
   $result = mysqli_query($db, $sql);
	}
?>
<html>
<head>
<title>Wykonaj przelew</title>
<meta charset="utf-8" />
<meta name="viewport" content="width=device-width, initial-scale=1">
<link rel="stylesheet" href="style.css">
</head>
	<body>
	 <div id="main_container">

		<header id="main_title">
			<h1> Witaj <?php echo $login_session; ?> w swoim banku! Nr konta: <?php echo $login_number; ?></h1>>
			<p id="note"><a href = "logout.php">Wyloguj się</a></p>
		</header>
		<div id="points_table_container">

		<h2 id="points_title">Zlecono przelew!</h2>

		<table id="points_table">
		<tr>
				<th>Na rachunek:</th>
				<th>Kwota:</th>
				
		</tr>
		<tr>
				<th id="recipient_number"><?php echo $_SESSION['recipient_number']; ?></th>
				<th id="transfer_value"><?php echo $_SESSION['transfer_value']; ?></th>
				
		</tr>
		</table>

		<form action = "" method = "post">
                  <input type = "submit" value = " Powrót "/><br />
        </form>
		</div>
	</div>
	<footer>
		Kontakt z bankiem jedynie za pośrednictwem gołębia pocztowego!
	</footer>
	</body>
</html>