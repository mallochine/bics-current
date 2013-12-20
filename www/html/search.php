<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.0 Transitional//EN">
<HTML>
<HEAD>
<SCRIPT language="JavaScript" src="search.js"></SCRIPT>
<TITLE> New Document </TITLE>
<style type="text/css">

.button {
    width: 100px;
    height: 17px;
    font-family: verdana, arial, helvetica, sans-serif;
    text-align: center;
    vertical-align: middle;
    font-size: 13px;
    font-weight: bold;
    color: #000000;
    border: 1px solid #CCD2DC;
    background: #E7F1FF;
    cursor: pointer;
	/* didomir*/
	background-color: #E7CEA5;
	border-top-color: #cce3ff;
	border-left-color: #cce3ff;
	border-bottom-color: #31557f;
	border-right-color: #31557f;
}

.info {
    font-family: verdana, arial, helvetica, sans-serif;
    text-align: left;
    vertical-align: middle;
    font-size: 11px;
    color: #455D7F;
    cursor: default;
}

.choiceOn {
    width: 100px;
    height: 17px;
    font-family: verdana, arial, helvetica, sans-serif;
    text-align: center;
    vertical-align: middle;
    font-size: 11px;
    font-weight: bold;
    /* color: #E7F1FF; */
    color: #aaaaaa;
    border: 1px solid #CCD2DC;
    /*background: #455D7F;*/
    background: #996600;
    cursor: pointer;
}

.choiceOff {
    width: 100px;
    height: 17px;
    font-family: verdana, arial, helvetica, sans-serif;
    text-align: center;
    vertical-align: middle;
    font-size: 11px;
    font-weight: bold;
    /*color: #455D7F;*/
    color: #ffffff;
    border: 1px solid #CCD2DC;
    /*background: #E7F1FF;*/
    background: #aaaaaa;
    cursor: pointer;
}


</style>

	<script language="JavaScript">
	
	self.focus();
	
	function fx() {return false;}

function enmouse(b) {
	window.onmousedown = b ? null : fx;
	window.onmouseup = b ? null : fx;
	window.onclick = b ? null : fx;
}	
enmouse(false);

	</script>



</HEAD>

<BODY onload="javascript:setTimeout('buildIt()',5);" onkeyup="keypress(event);">

<SCRIPT LANGUAGE="JavaScript">
var initialFEN="";
<?php
  if (array_key_exists("fen",$_GET)) {
	echo 'initialFEN = "'.$_GET["fen"].'";';
  }
?>



</SCRIPT>
<div class="info" style="position:absolute; left:0; top:362; display:none;" >FEN: </div>
<div id="info" class="info" onmouseover="enmouse(true);" onmouseout="enmouse(false);" style="display:none; position:absolute; left:30; top:362;" ></input></div>


<div class="button" style="position:absolute; left:400; top:389; " onmouseover="over(this);" onmouseout="leave(this);" onclick="takeback();" >Back</div>

<div class="button" style="position:absolute; left:520; top:389; " onmouseover="over(this);" onmouseout="leave(this);" onclick="BestMove();" >Most Played</div>

<div class="button" style="position:absolute; left:400; top:443;" onmouseover="over(this);" onmouseout="leave(this);" onclick="flip();" >Flip Board</div>

<div class="button" style="display:none; position:absolute; left:400; top:416;" onmouseover="over(this);" onmouseout="leave(this);" onclick="inputfen();" >Input FEN</div>


<div class="button" style="position:absolute; left:400; top:470;" onmouseover="over(this);" onmouseout="leave(this);" onclick="st.SetVisible(this);" >Hide Setup</div>
</BODY>
</HTML>
