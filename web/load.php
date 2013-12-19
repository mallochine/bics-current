<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1" />
<title>Untitled Document</title>
</head>

<body>
<FORM method="GET" action="view.php" target="_self">
<select size="8" NAME="filename">
<?php
$dir    = 'bpgn';
$files = scandir($dir);

foreach ($files as $file_num => $file) {
  if ($file != ".." && $file != ".") {
  echo "<option value='".$file."'>".$file."</option>";
  }
}

?> 

</select>
<input type="submit" value="Load">
</FORM>
</body>
</html>
