<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<?php

$conn=pg_connect("host=192.168.75.3 port=5432 user=bics password=superpass");

  	  $host  = $_SERVER['HTTP_HOST'];
	  $uri  = rtrim(dirname($_SERVER['PHP_SELF']), '/\\');
	  $extra = 'load04.php';
	  // $filename=$_SERVER['REQUEST_URI']; and we have full url
  
  if (count($_GET) == 0) {
  }  
  
  else if(array_key_exists("page",$_GET))  {
     if (preg_match('/[0-9]/', $_GET["page"])==0)  {
	   header("Location: http://$host$uri/$extra");
	   exit;
     }
  }
    
  else {

	  header("Location: http://$host$uri/$extra");
	  exit;
  }
?>
<style type="text/css">
<!--
.black {
	color: #FFFFFF;
	background-color: #666666;
}
body,td,th {
	font-family: Courier New, Courier, monospace;
}
table {
	table-layout: fixed;
	white-space: nowrap;
}
td {
	font-size: 14px;
	white-space: nowrap;
}
.nonWrapped {
	white-space:normal;
}
-->
</style>
<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1" />
<title>Untitled Document</title>
</head>

<body>
<h2 align="center" >Welcome to BICS bughouse games database!</h2>
<p align="center" >&nbsp;</p>


<?php
    //-----------VARS----------
	
$GText=''; //var containing parsed game data
$dir    = 'bpgn';
$GTable = ''; //var containing parsed game data and HTML table
$MatchPatternGame = "[Event %[^]]] [Site %[^]]] [Date %[^]]] [WhiteA %[^]]] [WhiteAElo %[^]]] [BlackA %[^]]] [BlackAElo %[^]]] [WhiteB %[^]]] [WhiteBElo %[^]]] [BlackB %[^]]] [BlackBElo %[^]]] [TimeControl %[^]]] [Result %[^]]]"; // var containig the match pattern of the file 
     
$files = scandir('bpgn',1);

$pagesTotal = ceil(count ($files)/10);
$pageStart = 1;
$pageEnd = $pagesTotal;
if (count($_GET) != 0) {
$page = $_GET["page"];
}
else
{$page=1;
}
if (!$page){$page=1;}
if ($page < 1){$page=1;}
if ($page > $pagesTotal) {$page = $pagesTotal;}
if ($pagesTotal > 5) {
	$pageStart = $page - 5;
	$pageEnd = $page + 5;
	if ($pageStart < 1) {$pageStart = 1;}
	if ($pageEnd > $pagesTotal) {$pageEnd = $pagesTotal;}
}

$files = array_chunk($files, 10);
$files = $files[$page-1];

?>
<!--Top pagination-->
<table width="600" border="0" cellspacing="0" cellpadding="4" align="left">
  <tr>
    <?php
	if ($page != 1) {
	echo '<td width="20%" align="center"><a href="load04.php?page='.($page-1).'">Previous</a></td>';
	}
	else {
	  echo '<td width="20%" align="center">&nbsp</td>';
	}
	?>
	<td width="60%" align="center">
    <?php
	$counter=$pageStart;
	while ($counter <= $pageEnd) {
	  if ($counter == $page) {
	    echo $counter;
	  }
	  else{
	    echo ' <a href="load04.php?page='.$counter.'">'.$counter.'</a> ';
	  }
	  $counter++;
	}
	echo '</td>';
	if ($page != $pagesTotal){
      echo '<td width="20%" align="center"><a href="load04.php?page='.($page+1).'">Next</a></td>';
	}
	else {
	  echo '<td width="20%" align="center">&nbsp</td>';
	}
	?>
  </tr>
</table>

<p align="center" >&nbsp;</p>
<?php

     //----FUNCTIONS-------
			  
	//-------Generate HTML function------
function generateHTML($file, $GDate, $WhiteA, $WhiteAElo, $BlackA, $BlackAElo, $WhiteB, $WhiteBElo, $BlackB, $BlackBElo, $TimeControl, $Result) {
			
		$GTable = '';
		$GTable .='<table width="680" border="0" cellspacing = "2" cellpadding="2">';
		$GTable .='<tr>';
		$GTable .='<td class="nonWrapped" width="11%" rowspan="2" >'.$GDate.'</td>';
		$GTable .='<td width="15%" >'.$WhiteA.'('.$WhiteAElo.')</td>';
		$GTable .='<td width="15%" class="black">'.$BlackB.'('.$BlackBElo.')</td>';
		$GTable .='<td width="4%" rowspan="2" >'.$Result.'</td>';
		$GTable .='<td width="5%" rowspan="2" ><a href="view.php?filename='.$file.'">View</td>';
		$GTable .='</tr>';
		$GTable .='<tr>';
		$GTable .='<td width="15%" class="black">'.$BlackA.'('.$BlackAElo.')</td>';
		$GTable .='<td width="15%" >'.$WhiteB.'('.$WhiteBElo.')</td>';
		$GTable .='</tr>';
		$GTable .='<tr>';
		$GTable .='<td colspan="5" ><hr/></td>';
		$GTable .='</tr>';
		$GTable .='</table>';
		return $GTable;
	 
} 
	 //------Extract all data from files and echo result-------
//set start numbers for pagination
$start_row=20;
//set count of numbers
$count_row=10;
$result=pg_exec($conn,"
select * from bpgn_games order by \"Date\" desc offset $start_row limit $count_row
	");
$i=0;
while ( $i<$count_row)
{
	$r=pg_fetch_row($result,$i);
	$GDATE=$r['Date'];
	$WhiteA=$r['WhiteA']; //TODO: etc, all others fields
		$GTable = generateHTML($file, $GDate, $WhiteA, $WhiteAElo, $BlackA, $BlackAElo, $WhiteB, $WhiteBElo, $BlackB, $BlackBElo, $TimeControl, $Result);
		$Result ="Result";
		echo $GTable;
	$i++;
}
/*
  $Result ="Result";
  foreach ($files as $file_num => $file) {
    if ($file != ".." && $file != ".") {
		$GText = "";
		$lines = file($dir."/".$file);
		$GText = implode("",$lines);
		$GText = str_replace('"', "", $GText);
		$GText = str_replace("'", "&#39;", $GText);	

		sscanf($GText, $MatchPatternGame, $GEvent, $GSite, $GDate, $WhiteA, $WhiteAElo, $BlackA, $BlackAElo, $WhiteB, $WhiteBElo, $BlackB, $BlackBElo, $TimeControl, $Result);
		$GTable = generateHTML($file, $GDate, $WhiteA, $WhiteAElo, $BlackA, $BlackAElo, $WhiteB, $WhiteBElo, $BlackB, $BlackBElo, $TimeControl, $Result);
		$Result ="Result";
		echo $GTable;
	}
  }
*/
?>


<!--Bottom pagination-->


<table width="600" border="0" cellspacing="0" cellpadding="4" align="left">
  <tr>
    <?php
	if ($page != 1) {
	echo '<td width="20%" align="center"><a href="load04.php?page='.($page-1).'">Previous</a></td>';
	}
	else {
	  echo '<td width="20%" align="center">&nbsp</td>';
	}
	?>
	<td width="60%" align="center">
    <?php
	$counter=$pageStart;
	while ($counter <= $pageEnd) {
	  if ($counter == $page) {
	    echo $counter;
	  }
	  else{
	    echo ' <a href="load04.php?page='.$counter.'">'.$counter.'</a> ';
	  }
	  $counter++;
	}
	echo '</td>';
	if ($page != $pagesTotal){
      echo '<td width="20%" align="center"><a href="load04.php?page='.($page+1).'">Next</a></td>';
	}
	else {
	  echo '<td width="20%" align="center">&nbsp</td>';
	}
	?>
  </tr>
</table>

<p align="center" >&nbsp;</p>
<p align="center" >&nbsp;</p>


</body>
</html>
