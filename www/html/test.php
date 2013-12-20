<?php

ini_set('display_errors',1); 
error_reporting(E_ALL);

require('lib/bicsdb.php');
$bicsdb = new BICSDB();
$bicsdb->addplayer();

#phpinfo();

#$conn=pg_connect("host=92.168.75.3 port=5432 user=bics password=superpass");
#$result=pg_exec($conn,"select * from v_all_games");
#$num=pg_numrows($result);
#for($i=0;$i<$num;$i++)
#{
# $r=pg_fetch_row($result,$i);
# for($j=0;$j<count($r);$j++)
#	echo "$r[$j]&nbsp;";
#echo "<BR>";
#}
