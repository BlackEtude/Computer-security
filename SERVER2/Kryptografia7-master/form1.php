<?php
   include('session.php');
   include('config.php');
   session_start();
if($_SERVER["REQUEST_METHOD"] == "POST") {

		if (($_POST['value'] > 0) && ($_POST['value'] <= $balance)){
			$error = "";
			header("location:summary.php");
		}
		else
			$error = "Niepoprawna wartość!";
		
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
			<h1> Witaj <?php echo $login_session; ?> w swoim banku! Nr konta: <?php echo $login_number; ?></h1>
			<p id="note"><a href = "logout.php">Wyloguj się</a></p>
		</header>
		<div id="points_table_container">

		<h2 id="points_title">Stan konta: <?php echo $balance ?> BTC </h2>

		<form name = "from1" action = "" onsubmit="storeData()" method = "post">
                  <label>Nr rachunku odbiorcy:</label><input type = "number" name = "recipient"/><br /><br />
                  <label>Kwota:</label><input type = "number" name = "value"/><br/><br />
                  <input name = "submit" type = "submit" value = " Podsumowanie "/><br />
        </form>
        <div id="error"><?php echo $error . "\n"; ?></div>
		</div>
	</div>
	<footer>
		Kontakt z bankiem jedynie za pośrednictwem gołębia pocztowego!
	</footer>
	<script src="script1.js"></script>
	</body>
</html>