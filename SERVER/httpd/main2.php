<?php
include('config.php');
$login_number = "1234567891012345";

$target_recipient = "9876543211012345";
$target_value = 98;

$sql = "INSERT INTO history(recipient, sender, value, execution_date) VALUES ($target_recipient, $login_number, $target_value, now())";
$result = mysqli_query($db, $sql);
if (!$result) {
	printf("Error: %s\n", mysqli_error($db));
	exit();
}
?>
