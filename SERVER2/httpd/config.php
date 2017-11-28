<?php
define('DB_SERVER', 'localhost');
define('DB_USERNAME', 'root');
define('DB_PASSWORD', '');
define('DB_DATABASE', 'bankBase');
$db = mysqli_connect(DB_SERVER, DB_USERNAME, DB_PASSWORD, DB_DATABASE);

if (!$db) {
    die("Database connection failed: " . mysqli_error($db));
}

$db_select = mysqli_select_db($db, DB_DATABASE);
if (!$db_select) {
    die("Database selection failed: " . mysqli_error($db_select));
}
?>