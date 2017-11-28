<?php
   include("config.php");
   session_start();
   
   if($_SERVER["REQUEST_METHOD"] == "POST") {      
      $myusername = mysqli_real_escape_string($db,$_POST['username']);

      $sql = "SELECT salts.salt FROM salts WHERE user = '$myusername'";
      $result = mysqli_query($db, $sql);
      $row = mysqli_fetch_array($result, MYSQLI_ASSOC);
      $count = mysqli_num_rows($result);
      $salt = "0";
      if($count == 1) {
      	$salt = $row['salt'];
      }

      $mypassword = sha1($salt . $_POST['password']);
      
      $sql = "SELECT clients.number FROM clients WHERE user = '$myusername' and password = '$mypassword'";
      $result = mysqli_query($db, $sql);
      $row = mysqli_fetch_array($result, MYSQLI_ASSOC);    
      $count = mysqli_num_rows($result);

      if($count == 1) {
         $_SESSION['login_user'] = $myusername;
         header("location:main.php");
      }
      else {
      	$sql = "SELECT admin.user FROM admin WHERE user = '$myusername' and password = '$mypassword'";
      	$result = mysqli_query($db, $sql);
      	$row = mysqli_fetch_array($result,MYSQLI_ASSOC);    
      	$count = mysqli_num_rows($result);

      	if($count == 1) {
        	   $_SESSION['login_user'] = $myusername;
        	   header("location:admin.php");
        }
        else{
        	$error = "Nieprawidłowe dane logowania!";	
        }	
      }
}
?>
<html>
   
   <head>
      <title>Logowanie do Banku</title>
      
      <style type = "text/css">
         body {
            font-family:Arial, Helvetica, sans-serif;
            font-size:14px;
         }
         
         label {
            font-weight:bold;
            width:100px;
            font-size:14px;
         }
         
         .box {
            border:#666666 solid 1px;
         }
      </style>
      <script src='https://www.google.com/recaptcha/api.js'></script>
   </head>

   
   <body bgcolor = "#FFFFFF">
	
      <div align = "center">
         <div style = "width:500px; border: solid 1px #333333; " align = "left">
            <div style = "background-color:#333333; color:#FFFFFF; padding:3px;"><b>Login</b></div>
				
            <div style = "margin:30px">
               
               <form action = "" method = "post">
                  <label>Username  :</label><input type = "text" name = "username" class = "box"/><br /><br />
                  <label>Password  :</label><input type = "password" name = "password" class = "box" /><br/><br />
                  <div class="g-recaptcha" data-sitekey="6Lfw5xMUAAAAAFNvGH1cstg6oK6_zr1GbCj-0mcB"></div>
                  <input type = "submit" value = " Submit "/><br />
               </form>
               
               <div style = "font-size:11px; color:#cc0000; margin-top:10px"><?php echo $error . "\n"; ?></div>
					
            </div>
				
         </div>
      </div>

   </body>

</html>