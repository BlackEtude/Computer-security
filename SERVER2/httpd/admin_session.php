<?php
   include('config.php');
   session_start();
   $user_check = $_SESSION['login_user'];
   $ses_sql = mysqli_query($db,"SELECT admin.user FROM admin where admin.user = '$user_check'");
   $row = mysqli_fetch_array($ses_sql, MYSQLI_ASSOC);
   $ses_count = mysqli_num_rows($ses_sql);
   $login_session = $row['user'];
   if(!isset($_SESSION['login_user']) || $ses_count == 0){
      header("location:index.php");
   }
?>