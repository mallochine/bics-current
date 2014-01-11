<?php

class BICSDB
{
    private static $conn;

    function BICSDB() {
        #$this->connect();
    }

    function connect() {
        $this->conn = mysql_connect("localhost", "root", "", "bics");
        if (mysql_errno()) {
            echo "MySQL connect failed";
        }
    }

    function getSQLResource() {
        return $this->conn;
    }

    // Copied from src/crypt.c
    function chessd_crypt ($passwd, $salt) {
        $salt = '$1$'.$salt;
        return crypt($passwd, $salt);
    }

    function generatePassword() {
        $salt = "aa";
        $password = "bics";
        echo "Your password is '$password' <br />";
        $res = $this->chessd_crypt($password, $salt);
        return $res;
    }

    function checkUsername($username) {
        if (!preg_match("/^[a-zA-Z]+$/", $username))
            return False;
        if (strlen($username) < 3)
            return False;
        $tmp = strtolower($username);
        $firstchar = $tmp[0];
        if (file_exists("/var/bics/players/$firstchar/$tmp"))
            return False;
        return True;
    }

    function addplayer($username, $fullname, $email) {
        if (!$this->checkUsername($username)) {
            echo "Registration failed. <br />";
            echo "Your username can only have letters, ";
            echo "must have more than 3 characters, ";
            echo "and must not yet exist.";
            return;
        }
        echo "Your username is '$username' <br />";
        $newuser = file_get_contents('./lib/newuser.template');
        $newuser = str_replace('%u',$username, $newuser);
        $newuser = str_replace('%p',$this->generatePassword(), $newuser);
        $newuser = str_replace('%f',$fullname, $newuser);
        $newuser = str_replace('%e',$email, $newuser);
        $username = strtolower($username);
        $firstchar = $username[0];
        file_put_contents("/var/bics/players/$firstchar/$username", $newuser);
    }
}
