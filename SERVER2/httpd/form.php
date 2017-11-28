<?php
include('session.php');
session_start();
if($_SERVER["REQUEST_METHOD"] == "POST") {
	$target_value = $_POST['value'];
	$target_recipient = $_POST['recipient'];
	$sql = "SELECT clients.number FROM clients WHERE clients.number = $target_recipient";
	$result = mysqli_query($db, $sql);
	$row = mysqli_fetch_array($result, MYSQLI_ASSOC);    
	$count = mysqli_num_rows($result);

	if($count == 1 and $target_value <= $balance and $target_value > 0) {
		$_SESSION['recipient_number'] = $target_recipient;
		$_SESSION['transfer_value'] = $target_value;
		header("location:summary.php");
	}
	else {
    	$error = "Nieprawidłowe dane!";
	}
}
?>
<html>
<head>
<title>Make a transfer</title>
<meta charset="utf-8" />
<meta name="viewport" content="width=device-width, initial-scale=1">
<link rel="stylesheet" href="style.css">
</head>
<body>
	<div id="main_container">
		<header id="main_title">
			<h1> Welcome to Your Bank, <?php echo $login_session; ?>! <br>Account: <?php echo $login_number; ?></h1>
			<p id="note" ><a href="logout.php"><button>LOGOUT</button></a></p>
		</header>
		<div id="points_table_container">
		<h2 id="points_title">Balance: <?php echo $balance ?> PLN </h2>
		<form action = "" method = "post">
			<label>Receiver:</label><input type = "text" name = "recipient"/><br /><br />
			<label>Cash:</label><input type = "number" name = "value"/><br/><br />
			<input type = "submit" value = " Accept "/><br />
        </form>
        <div id="error"></div>
    </div>
</div>
<footer>
	Corporate Customer Service Center lines are active Monday to Friday 8 am - 6 pm
</footer>
</body>
</html>