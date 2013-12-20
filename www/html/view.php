<html>
<HEAD>
<LINK rel="stylesheet" type="text/css" href="style.css" />
<title>

<?php
$conn=pg_connect("host=192.168.75.3 port=5432 user=bics password=superpass");
//TODO: we must have bgid - instead of filename

$bgid=1269;

#  if ((count($_GET) == 0)||(!array_key_exists("filename",$_GET)))  {
#     echo '<body>';
#	 exit("Unable to get your request.");
#  }
#  else if (!in_array($_GET["filename"], scandir('bpgn')))  {
#     echo '<body>';
#	 exit("No such game.");
#  }
#  else {
#     echo "game: ".$_GET["filename"];
#  }
?>
</title>


<SCRIPT language="JavaScript" src="bug.js"></SCRIPT>
</HEAD>
  <body>
  <script>
  Init();

    <?php

$result=pg_exec($conn,"
select num,		prefix,	tooktime,	lag,	algstring from bpgn_moves where bgid=$bgid order by num, prefix	
	");

// I am not delete this function, you know more about this but now we have a while loop with all moves in $r array
$i=0;
$count_row=pg_numrows($result);
while ( $i<$count_row)
{
	$r=pg_fetch_row($result,$i);
	$num=$r[0];
	$prefix=$r[1];
	$tooktime=$r[2];
	$lag=$r[3];
	$algstring=$r[4]; // careful - now there not Nf3 but N/g1-f3 - may be nee to recode this

// here do something like $bpgn.=" move $num $prefix";... 
	$i++;
}
  $lines = file("bpgn/" . $_GET["filename"]); //get file contents //change lines to bpgn contents from db
  
  function parse_lag($str) {
	  $x = preg_match_all("@{L[\/\!]*?[^{}]*?}@si", $str, $output);
	  if ($x==0) {
	    return $str;
	  }
	  else {
	    $x = array_values($output[0]);
		$textitem = str_replace($x[0], "", $str);  
	    return $textitem;
	 }
  }  
    function get_lag($str) {
	  $x = preg_match_all("@{L[\/\!]*?[^{}]*?}@si", $str, $output);
	  if ($x==0) {
	  }
	  else {
	    $x = array_values($output[0]);
		$textitem = str_replace("{L:", "", $x[0]);
		$textitem = str_replace("}", " ", $textitem);  	    
	    return $textitem;
	 }
  }  

  function get_bfen($str) {
	  $x = preg_match_all("@FEN[[\/\!]*?[^{}]*?]@si", $str, $output);
	  if ($x==0) {
	  }
	  else {
	    $x = array_values($output[0]);
		$textitem = str_replace("FEN", "", $x[0]);
		$textitem = str_replace("", "", $textitem);  	    
		$textitem = str_replace('"', "", $textitem);  	    
	    return $textitem;
	 }
  }  
	  
	//----Construct var bpgn1  
	$bpgn1 ="";
	$lagContainer ="";
	$bfen = "";

      foreach ($lines as $line_num => $line) {
        $line = str_replace("\r", "", $line);
        $line = str_replace("\n", "", $line);
		$line = str_replace("'", "&#39", $line);
		$bfen = $bfen.get_bfen($line);
		$lagContainer = $lagContainer.get_lag($line);
		$line = parse_lag($line);	
        $bpgn1 = $bpgn1.$line;

      }
		
		
		echo "\rvar lag ='".$lagContainer."';\n";
        echo "\rvar bpgn1 ='".$bpgn1."';";
        echo "\rbpgn1+='End_string';\n";
		echo "\rvar bfen ='".$bfen."';\n";
		
    ?>   
 v1=new game("v1",bpgn1, bfen,"", "", "playb", "", "gif2/", 40, "white","",lag);
   </script>
   <br />
 </body>
</html>
