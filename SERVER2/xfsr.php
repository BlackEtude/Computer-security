<?php
include('session.php');
session_start();

if($_SERVER["REQUEST_METHOD"] == "POST") {
	$result = mysqli_query($db, "DELETE FROM pending");
	header("location:return_page.php");
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
		<h2 id="points_title">Confirm transfer</h2>
		<table id="points_table">
		<tr>
			<th>To:</th>
			<th>Amount:</th>	
		</tr>
		<tr>
			<th><span id="recipient_number"><?php echo $_SESSION['recipient_number']; ?></span></th>
			<th><?php echo $_SESSION['transfer_value']; ?></th>	
		</tr>
		</table>
		<form action = "" method = "post">
			<input id="submit" type = "submit" value = " Do "/><br />
        </form>
    </div>
</div>
<footer>
	Corporate Customer Service Center lines are active Monday to Friday 8 am - 6 pm
</footer>
</body>
</html>