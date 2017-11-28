<?php
   include('session.php');
   session_start();
   if($_SERVER["REQUEST_METHOD"] == "POST") {
   		$_SESSION['transfer_value'] = $_POST['value'];
		$_SESSION['recipient_number'] = $_POST['recipient'];
		header("location:return_page.php");
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

		<h2 id="points_title">Potwierdź zgodność danych</h2>

		<table id="points_table">
		<tr>
				<th>Na rachunek:</th>
				<th>Kwota:</th>
				
		</tr>
		<tr>
				<th id="recipient_number"></th>
				<th id="transfer_value"></th>
				
		</tr>
		</table>

		<form name = "from1" action = "" onsubmit="post()" method = "post">
                  <input class="hidden" id="recipient" type = "number" name = "recipient"/>
                  <input class="hidden" id="value" type = "number" name = "value"/>
                  <input id = "submit" name = "submit" type = "submit" value = " Wykonaj "/><br />
        </form>
        <p id="note"><a href = "main.php">Anuluj</a></p>
		</div>
	</div>
	<footer>
		Kontakt z bankiem jedynie za pośrednictwem gołębia pocztowego!
	</footer>
	<script src="script2.js"></script>
	</body>
</html>