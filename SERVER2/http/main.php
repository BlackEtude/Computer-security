<?php
include('session.php');
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
<title>Bank - Main Panel</title>
<meta charset="utf-8" />
<meta name="viewport" content="width=device-width, initial-scale=1">
<link rel="stylesheet" href="style.css">
</head>
<body>
	<div id="main_container">
		<header id="main_title">
			<h1> Welcome to Your Bank, <?php echo $login_session;?>! <br> Account: <?php echo $login_number;?></h1>
			<p id="note" ><a href="logout.php"><button>LOGOUT</button></a></p>
		</header>

		<div id="points_table_container">
		<h2 id="points_title">Balance: <?php echo $balance;?> PLN </h2>

		<div id="communicates">
			<h2 id="communicates_title"><a href = "form.php"><button>Make a transfer</button></a></h2>
		</div>

		<br><br>
		<h2 id="points_title">History:</h2>

		<form name = "from1" action = "" method = "post">
                  <label>Receiver:</label><input  type="text" name = "number"/><br /><br />
                  <input name = "submit" type = "submit" value = " Filter "/><br />
        </form>

		<?php
		$sql = "SELECT history.recipient, history.value, history.execution_date FROM history 
		WHERE history.sender = $login_number && history.recipient = $number; ";
		// $result = mysqli_query($db, $sql);
		$result = mysqli_multi_query($db, $sql);

		echo '<table id="points_table" cellpadding="6">
			<tr>
				<th>To</th>
				<th>Amount</th>
				<th>Date</th>
				
			</tr>';
		
		while($row = mysqli_fetch_assoc($result)) {
			echo '<tr>' .
        	'<th>' . $row["recipient"].'</th>' .
	        '<th>' . $row["value"]. ' PLN </th>' .
	        '<th>' . $row["execution_date"].'</th>' .
	        '</tr>';
		}
        echo '</table>';
		?>
	</div>
</div>
<footer>
	Corporate Customer Service Center lines are active Monday to Friday 8 am - 6 pm
</footer>
</body>
</html>