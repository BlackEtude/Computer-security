<?php
   include('admin_session.php');
   include('config2.php');
   session_start();
    if($_SERVER["REQUEST_METHOD"] == "POST") {
    	$target_id = $_POST['target_id'];
    	$sql = "DELETE FROM pending WHERE pending.id = '$target_id'";
      	$result = mysqli_query($db, $sql);
    }
?>
<html>
<head>
<script src="https://ajax.googleapis.com/ajax/libs/jquery/3.1.1/jquery.min.js"></script>
<title>Bank - Administrator Panel</title>
<meta charset="utf-8" />
<meta name="viewport" content="width=device-width, initial-scale=1">
<link rel="stylesheet" href="style.css">
</head>
   <body>
      <div id="main_container">
   
		<header id="main_title">
			<h1> Welcome <?php echo $login_session; ?> to Admin Panel!</h1>
			<p id="note"><a href = "logout.php">Logout</a></p>
		</header>

		<div id="points_table_container">

		<h2 id="points_title">Pending transfers:</h2>

		<?php
		$sql = "SELECT pending.id, pending.title, pending.sender, pending.recipient, pending.value, pending.execution_date FROM pending WHERE 1 = 1";
		$result = mysqli_query($db, $sql);

		echo '<table id="points_table">
			<tr>
				<th>Id:</th>
        <th>Title:</th>
				<th>Sender:</th>
				<th>Recipient:</th>
				<th>Date:</th>
				<th>Amount:</th>
				<th>Confirm:</th>
			</tr>';

		
		while($row = mysqli_fetch_assoc($result)) {
		echo '<tr>'
     .	'<th>' . $row["id"]. '</th>' .
        '<th>' . $row["title"]. '</th>' .
        '<th>' . $row["sender"]. '</th>' .
        '<th>' . $row["recipient"]. '</th>' .
        '<th>' . $row["execution_date"].'</th>' .
        '<th>' . $row["value"].'</th>' .
        '<th>' . 
        '<form name = "f" action = "" method = "post">
                  <input class="hidden" type = "text" name = "target_id" value =' .
                  (int)$row["id"] .
                  '/>
                  <input id = "submit" name = "submit" type = "submit" value = " Confirm "/><br />
        </form>' .
        '</th>' .
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