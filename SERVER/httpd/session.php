<?php
include('config.php');
session_start();
$user_check = $_SESSION['login_user'];
$ses_sql = mysqli_query($db,"SELECT clients.name, clients.number, clients.balance FROM clients where clients.user = '$user_check'");
$row = mysqli_fetch_array($ses_sql, MYSQLI_ASSOC);
$balance = $row['balance'];
$login_session = $row['name'];
$login_number = $row['number'];
if(!isset($_SESSION['login_user'])) {
   header("location:index.php");
}
?>