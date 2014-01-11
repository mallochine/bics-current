<?php

ini_set('display_errors',1); 
error_reporting(E_ALL);

$username = $_POST['username'];
$realname = $_POST['realname'];
$email = $_POST['email'];

require('lib/bicsdb.php');
$bicsdb = new BICSDB();
$bicsdb->addplayer($username, $realname, $email);

?>

<br />
<br />

<a href=''>
    Get Thief
</a>

<br />

<a href='http://bughousechess.wz.cz/BICS/HowToConnectToNewICSWithClient.html'>
    Connect with Thief
</a>
