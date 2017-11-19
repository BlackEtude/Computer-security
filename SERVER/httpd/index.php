<?php
include("config.php");
session_start();

if($_SERVER["REQUEST_METHOD"] == "POST") {
   $myusername = mysqli_real_escape_string($db, $_POST['username']);
   $sql = "SELECT salts.salt FROM salts WHERE user = '$myusername'";
   $result = mysqli_query($db, $sql);
   $row = mysqli_fetch_array($result, MYSQLI_ASSOC);
   $count = mysqli_num_rows($result);
   $salt = "0";
   if($count != 1) {
      echo '<p id="errormessage"><font color="red">error message</font></p>';
      // exit();
   }
   $salt = $row['salt'];
   $mypassword = sha1($salt . $_POST['password']);
   
   $sql = "SELECT clients.password FROM clients WHERE user = '$myusername'";
   $result = mysqli_query($db, $sql);
   $row = mysqli_fetch_array($result, MYSQLI_ASSOC);    
   $count = mysqli_num_rows($result);
	
   if($count == 1) {
      $_SESSION['login_user'] = $myusername;
      header("location:main.php");
   }
   else {
      $error = "Nieprawidłowe dane logowania!";
   }
}
?>

<!-- html>
<head>
   <title>Logging</title>
   <style type = "text/css">
    body {
      font-family:Arial, Helvetica, sans-serif;
      font-size:30px;
   }
   label {
      font-weight:bold;
      width:100px;
      font-size:17px;
   }
   .box {
      border:#666666 solid 1px;
      color: black;
   }
   </style>
</head>

<body bgcolor = "#FFFFFF">
   <div align = "center">
      <div style = "width:370px; height:250px; border: solid 1px #333333; " align = "left">
         <div style = "background-color:green; color:#FFFFFF; padding:6px;"><b>Log in</b></div>
         <div style = "margin:40px">
            <form action = "" method = "post">
               <label>Username:  </label><input type = "text" name = "username" class = "box"/><br/><br/>
               <label>Password:  </label><input type = "password" name = "password" class = "box" /><br/><br />
               <input type = "submit" value = " Log in "/><br />
            </form>
            <div style = "font-size:11px; color:#cc0000; margin-top:10px"></div>
         </div>
      </div>
   </div>
</body>
</html> -->

<!DOCTYPE html>
<html>
<style>


form {
    border: 4px solid #f1f1f1;
}

input[type=text], input[type=password] {
    width: 100%;
    padding: 12px 20px;
    margin: 8px 0;
    display: inline-block;
    border: 1px solid #ccc;
    box-sizing: border-box;
}

button {
    background-color: #4CAF50;
    color: white;
    padding: 14px 20px;
    margin: 8px 0;
    border: none;
    cursor: pointer;
    width: 100%;
}

.imgcontainer {
    text-align: center;
    margin: 20px 0 10px 0;
}

img.avatar {
    width: 30%;
    border-radius: 30%;
}

.container {
    padding: 16px;
}



</style>
<body>

<h2>Your Safe Bank</h2>
<div class="imgcontainer">
  <img src="avatar.png" alt="Avatar" class="avatar">
</div>
<div class="container">
  <form action = "" method = "post">
    <label>Username:  </label><input type = "text" name = "username" required class = "box"/><br/><br/>
    <label>Password:  </label><input type = "password" name = "password" required class = "box" /><br/><br />
    <button type="submit">Log in</button><br />
  </form>
</div>
</body>
</html>