<?php
   include('config.php');
   session_start();
   $user_check = $_SESSION['login_user'];
   $ses_sql = mysqli_query($db,"SELECT clients.name, clients.number, clients.balance FROM clients where clients.user = '$user_check'");
   $row = mysqli_fetch_array($ses_sql, MYSQLI_ASSOC);
   $ses_count = mysqli_num_rows($ses_sql);
   $balance = $row['balance'];
   $login_session = $row['name'];
   $login_number = $row['number'];
   if(!isset($_SESSION['login_user']) || $ses_count == 0){
      header("location:index.php");
   }
?>