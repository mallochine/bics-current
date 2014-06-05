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

    function genRandomPassword()
    {
        $acceptablePasswordChars ="abcdefghijkmnopqrstuvwxyz";
        $randomPassword = "";

        for($i = 0; $i < 6; $i++)
        {
         $randomPassword .= substr($acceptablePasswordChars, rand(0, strlen($acceptablePasswordChars) - 1), 1);
        }
        return $randomPassword;
    }

    function generatePassword() {
        $salt = "aa";
        $password = $this->genRandomPassword();

        echo "Your password is '$password' <br />";
        $res = $this->chessd_crypt($password, $salt);
        return $res;
    }

    function checkUsername($username) {
        if (!preg_match("/^[a-zA-Z]+$/", $username))
            return False;
        if (strlen($username) < 3)
            return False;
        if (strlen($username) > 20)
            return False;
        if (strpos(strtolower($username), "simul") !== FALSE)
            return False;
        $tmp = strtolower($username);
        $firstchar = $tmp[0];
        if (file_exists("/var/bics/players/$firstchar/$tmp"))
            return False;
        return True;
    }

    function checkEmail($email) {
        return filter_var($email, FILTER_VALIDATE_EMAIL);
    }

    function checkFullName($fullname) {
        if (!preg_match("/^[a-zA-Z ]+$/", $fullname))
            return False;
        if (strlen($fullname) < 3)
            return False;
        if (strlen($fullname) > 40)
            return False;
        return True;
    }

    function logNewPlayer($username, $file) {
        file_put_contents("/var/bics/newplayers/$username", $file);
    }

    // TODO: Alex Guo: this is not good design. The output must be
    // separated from the model. But it is here for now because it is
    // convenient, and switching costs to a new design is very small.
    function addplayer($username, $fullname, $email) {
        if (!$this->checkUsername($username)) {
            echo "Registration failed. <br />";
            echo "Your username can only have letters, ";
            echo "must have more than 3 characters, ";
            echo "less than 20 characters, ";
            echo "must not contain the word 'simul, ";
            echo "and must not yet exist.";
            return;
        }
        if (!$this->checkFullName($fullname)) {
            echo "Registration failed. <br />";
            echo "Your fullname can only have letters, ";
            echo "must have more than 3 characters, ";
            echo "and less than 40 characters.";
            return;
        }
        if (!$this->checkEmail($email)) {
            echo "Registration failed. <br />";
            echo "Your email was invalid.";
            return;
        }
        echo "Your username is '$username' <br />";
        $newuser = file_get_contents('./lib/newuser.template');
        $newuser = str_replace('%p',$this->generatePassword(), $newuser);
        $newuser = str_replace('%f',$fullname, $newuser);
        $newuser = str_replace('%e',$email, $newuser);
        $simulacc = str_replace('%u',$username."simul", $newuser);
        $newuser = str_replace('%u',$username, $newuser);
        $username = strtolower($username);
        $firstchar = $username[0];
        file_put_contents("/var/bics/players/$firstchar/$username", $newuser);
        file_put_contents("/var/bics/players/$firstchar/$username"."simul", $simulacc);
        $this->logNewPlayer($username, $newuser);
    }
}
