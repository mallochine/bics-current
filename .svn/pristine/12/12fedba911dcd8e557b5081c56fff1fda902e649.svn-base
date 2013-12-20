<?php 

$obid = $_GET["obid"];
$reqid= $_GET["reqid"];
$fen= $_GET["fen"];


//         --------request---------
$conn=pg_connect("host=192.168.75.3 port=5432 user=bics password=superpass");


$result=pg_exec($conn,"
select algstring,count(*) from bpgn_moves where fen='$fen'
	group by algstring 
	order by count(*) desc");
$num=pg_numrows($result);

/*         --------send to viewer ---------*/
//TODO instead draw count may be
print "reqid|".$reqid."|hook|null|obid|".$obid."|value|Draw \n"; //number of games in the Draw field;
$pieces=array('P'=>6,'N'=>5,'B'=>4,'R'=>3,'Q'=>2,'K'=>1); //TODO check this
$coords=array('a'=>0,'b'=>1,'c'=>2,'d'=>3,'e'=>4,'f'=>5,'g'=>6,'h'=>7);
for($i=0;$i<$num;$i++)
{
 $r=pg_fetch_row($result,$i);
 $algstring=$r[0];  //  X/YY-ZZ  //TODO: parser algstring -> wierd 0-5 
 //TODO:promotion
 if((strtoupper($algstring)=='O-O') or (strtoupper($algstring)=='O-O-O'))
 {// castle
	$answer=$algstring;
 }
 else
 {//recode
 if($algstring[3]=='@')
 {//DROP
	$p='-'.$pieces[$algstring[0]];
	$x='65';
 }
 else
 {//regular move
	$p='';
	$x=$coords[$algstring[2]]+($algstring[3]-1)*8;
 }
	$y=$coords[ $algstring[5] ]+($algstring[6]-1)*8;	
	$answer=$x . '-' . $y . $p;
	}

 echo $answer;
 echo ":";
 echo $r[1] .  "\n";
}
/* for list of games 'select g.bgid,"WhiteA","BlackB","WhiteB","BlackA" from bpgn_games g inner join bpgn_moves m on g.bgid=m.bgid where m.fen='rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w' order by "WhiteAElo" desc, "WhiteBElo" desc, "BlackAElo" desc, "BlackBElo" desc limit 10'
/*         ---------parse result-------
parse moves and number of games goes here, assign each to $moves and $numberGames array

parse fen if needed
*/
/* ----------sample request-------------

http://localhost:8080/bpgn_viewer/fetch.php?obid=et30.9737026394117703&reqid=req0.1131288766618942&hook=null&action=bics&fen=rnbqkb1r/pppp1p1p/4p1pB/8/3PP3/6P1/PPP2P1P/RN1QKBNR%20b%20-%20-%200%201
*/

?>
