<?php
include('session.php');
session_start();
if($_SERVER["REQUEST_METHOD"] == "POST") {
	header("location:main.php");
	$target_recipient = $_SESSION['recipient_number'];
   $target_value = $_SESSION['transfer_value'];
   $sql = "INSERT INTO pending(recipient, sender, value, execution_date) VALUES ($target_recipient, $login_number, $target_value, now())";
   $result = mysqli_query($db, $sql);
	if (!$result) {
		printf("Error: %s\n", mysqli_error($db));
		exit();
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
			<h1> Welcome to Your Bank, <?php echo $login_session;?>! <br>Account: <?php echo $login_number;?></h1>>
			<p id="note" ><a href="logout.php"><button>LOGOUT</button></a></p>
		</header>
		<div id="points_table_container">
		<h2 id="points_title">Transfer was made!</h2>
		<table id="points_table">
		<tr>
			<th>To:</th>
			<th>Cash:</th>
		</tr>
		<tr>
			<th><?php echo $_SESSION['recipient_number']; ?></th>
			<th><?php echo $_SESSION['transfer_value']; ?></th>	
		</tr>
		</table>
		<form action = "" method = "post">
			<input type = "submit" value = " Return "/><br />
        </form>
    </div>
</div>
<footer>
	Corporate Customer Service Center lines are active Monday to Friday 8 am - 6 pm
</footer>
</body>
</html>