#!/usr/bin/perl -w

####################################################################
# FicsTeamBot.pl
# A teamgame organizer bot for a chess server.
#
# Copyright (C) 2001 Tomohiko Tanabe
# Authors: Tomohiko Tanabe, Adam Stallard
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Library General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
#

####################################################################
# Global Variables
#
# If any of these need to be configured with the configuration file,
# put them in the config hash (next section.)
#
# More global variables are include in the "initialize" function.
#
# These are variables that don't get reset by "initialize".
#

#versions with an odd number of numbers are considered under development;
#versions with an even number of numbers are considered stable.
$version = '1.10';

use IO::Socket;

$SIG{CHLD} = 'IGNORE';

#the connection to the server
$net		= undef;

$slot    = A;
$color_A = 0;
$color_B = 0;
$game_number = -1;

$mute			= 0;
$last_activity = 0;
$logged_in= 0;

%ratings= ();
%rating_slot= ();

#these are for getting info back from commands
$pose_timer          = 0;
$pose_admin          = "";

# these are used to stop spam
$previous_command_name	= "";
$previous_command	= "";
$previous_command_count	= 0;
$previous_command_time	= 0;
$previous_joined_player	= "";
$previous_joined_time	= 0;

#config variables that can take multiple values
%config_hash_names = (
		  'ignore' => 1,
		  'admins' => 1,
		      );

#constants associated with the ratings file
$max_hash = 83521;

%hash_symbol = (
		 'a' => 1,
		 'c' => 2,
		 'd' => 3,
		 'e' =>	4,
		 'h' => 5,
		 'i' => 6,
		 'l' => 7,
		 'm' => 8,
		 'n' => 9,
		 'o' => 10,
		 'p' => 11,
		 'r' => 12,
		 's' => 13,
		 't' => 14,
		 'u' => 15,
		 ' ' => 16
		 );

####################################################################
#Default Configuration
#

$config{'user'}	= 'madrooks';
$config{'password'}	= undef;
$config{'server'}      = 'fics';
$config{'host'}	= 'freechess.org';
$config{'port'}	= '5000';
$config{'spam_interval'}	= 560;	# seconds
$config{'needed_to_appoint'}    = 3;
$config{'needed_to_kick'}       = 3;
$config{'needed_to_start'}	= 4;
$config{'default_time_controls'}       = '2 5';
$config{'captain_drop_time_out'}	= 900;  # seconds
$config{'player_idle_time_out'}	= 600; # seconds
$config{'activity_time_out'}		= 600; # stop spam if no activity for this long
$config{'cshout_interval'}		= 8000; 
$config{'pose_time_out'}                  = 1; 
$config{'teams_channel'} = 20;
$config{'team_A_channel'} = 21;
$config{'team_B_channel'} = 22;
$config{'logfile_prefix'} = "";
$config{'log_games'} = 0;
$config{'admins'} = {};
$config{'ignore'} = {};

####################################################################
# Main Routine
#

if (! &configure_from_file("ftb.config")){
    print "[Error configuring from \"ftb.config\".]\n";
}

if (not open (RATINGS, '+<', "ftb.ratings")){
    print "Creating empty ratings file, \"ftb.ratings\".\n";
    open (RATINGS, '+>', "ftb.ratings");
    for(my $i=0; $i<1586899; $i++){
	syswrite(RATINGS,"\0",1);
    }
}

#keep trying to connect until killed by operator
while(1){

    # gets "broken pipe" and quits if server dies
    if ($net = IO::Socket::INET->new (Proto => "tcp", PeerAddr => $config{'host'}, PeerPort => $config{'port'})){
	
	print "[Connected to $config{'host'}:$config{'port'}]\n";

	$net->autoflush (1);
    
	&initialize;	      	
	&login;

	#The ICS should send frequent messages that can be ignored, but
	#serve to keep the bot awake.  We use these to prompt checks on
	#our timers.  Another option would be to multithread/process.
	while (defined (my $line = <$net>)) {
	    
	    my $current_time = time;
	    &check_timers($current_time);
	    
	    #strip prompts and leading white space.  This makes output less 
	    #human readable, but parsing easier.
	    $line =~ s/^.{0,3}ics%//mg while $line =~ s/^\s+//mg;

	    if ($line ne ""
		and $line !~ /^<p/
		and $line !~ /^\*xtell/
		and $line !~ /^\(told/){

		#some output is returned from a pose command; this gets
		#handled differently.
		if ($current_time - $pose_timer <= $config{'pose_time_out'}){
		    &net_write("xtell $pose_admin $line\n",1) 
		}
		else{
		    print $line;	
		    parse_ics_output ($line);
		}
	    }
	}
	print "[Disconnecting from ICS.]\n";
    }
    $logged_in=0;
    print "[Trouble connecting to port $config{'port'} on $config{'host'}: $!]\n";
    print "[Retrying in 10 seconds.]\n";
    sleep (10);
}

####################################################################
#configure_from_file(filename)
#
#return 1 if configuration was successful.
#

sub configure_from_file {

    open (CONFIG , $_[0]) or return 0;
    while (<CONFIG>) {

# match : "variable = value (can be multi-word)"
#         allows comments beginning with '#'
	/^([^#]*)/;
	$_ = $1;
	if (/(\w+)\s*=\s*(.+?)\s*$/){
	    my $setting=lc($1);

# if value is expected to be multi-word
	    if (exists $config_hash_names{$setting}){
		foreach my $item (split(' ', $2)){
		    $item=lc($item);
		    print "[Added $item to $setting]\n";
		    $config{$setting}{$item} = 1;
		}	
	    }
	    else{
		my $value = lc($2);
		$config{$setting}= $value;
		print "[Set $setting to $value]\n";
	    }
	}
	}
    return 1;
}

####################################################################
#initialize()
#
#Get the bot ready to start forming a new game from scratch.
#

sub initialize {
    $game_going			= 0;
    $team_forming		= 1;  #in the team forming stage before the game
#Players are assigned values in this hash based on their user type:
# 1 is player, 2 is captain, 3 is team account, 4 is computer account.
    %joined = ();

    $captain_A			= 0;
    $captain_B			= 0;
    $teamname_A		= "";
    $teamname_B		= "";
    %team_A	= ();
    %team_B	= ();
    
    %voted_A = ();
    %voted_B = ();
    %voted_this_move = ();
    $num_half_moves = 1;
 
    $spam_timer		= time;
    $cshout_timer	= $spam_timer;
    $captain_drop_timer  = 0;

    %appoint_votes=();
    %kick_votes=();
	
    if($logged_in){
	&store_game_info;
	&store_captain_drop_time;
	&store_players;
	&net_write ("unobserve\n");
    }

    $logfile=0;
}

####################################################################
# net_write(line,[quiet])
#
# Writes line to the host.  Echos line to screen unless quiet is true.
#

sub net_write {
    my ($line , $quiet) = @_;
    print $net $line;
    $line = "[ToICS] $line";
    print $line unless defined $quiet;
}

####################################################################
#ch_write(line)
#
# Write line to the teams forming channel.
#

sub ch_write {
    my ($line) = @_;
    &net_write ("tell $config{'teams_channel'} $line");
}

####################################################################
# login()
#

sub login {

    &net_write ("$config{'user'}\n");
    &net_write ("$config{'password'}\n");
    &net_write ("\n\n");

    $logged_in=1;
    
    #This gets the bot's last known status which it stores in its formula variables
    &net_write("variables\n");

    my $admins = &admin_names;
    
    &net_write (<<"__SEND_TO_ICS_AT_LOGIN__");
iset pinginfo 1
iset lock 1
-ch 1
-ch 2
-ch 50
+ch $config{'team_B_channel'}
+ch $config{'team_A_channel'}
+ch $config{'teams_channel'}
set seek 0
set open 0
set shout 0
set cshout 0
set width 240
set 1 FicsTeamBot version $version
set 2 My job is to help organize and keep track of team games on $config{'server'}.
set 5 Help system: type 'tg help'
set 6 Basic commands: 'info' 'join' 'withdraw' 'captain'.  Each command has its own help file, e.g. 'help join'.  'help commands' lists all the commands.
set 7 +notify me and I\'ll let you know if a team game is going on when you log in.
set 8 I keep recent team games in my journal.  I support games between team accounts (T), just have the owner of the account type 'tg captain'  Read Finger note 5 of CaptainCheck and CaptainMate to learn how to use autocaptains in the games.
set 9 *********** Channel $config{'teams_channel'} is the team game channel '+ch $config{'teams_channel'}' ***********
set 10 Please contact $admins with any concerns.
set style 12
set bell 0
set notifiedby 1
set kibitz 0
tell mamer set tshout 0
tell mamer set censor 1
__SEND_TO_ICS_AT_LOGIN__
}

####################################################################
# check_players()
#
# Check all the players in the joined list and remove the ones that
# aren't logged in anymore.
#

sub check_players {
    foreach my $player (keys %joined){
	&net_write ("in $player\n");
    }
}

####################################################################
# check_timers(time)
#
# Respond to any timers which were set to go off before time.
#
sub check_timers {
    my $now = $_[0];
    
    if ($now - $spam_timer > $config{'spam_interval'} and $now - $last_activity < $config{'activity_time_out'} and !$mute) {
	&spam;
    }
    
    if (!$game_going && !$team_forming && $now - $captain_drop_timer > $config{'captain_drop_time_out'}){
	&initialize;
    }
    
    if ($game_going){
	foreach my $player (keys %team_A){
	    if($now - $team_A{$player} > $config{'player_idle_time_out'}){
		$team_A{$player}=$now;
		&player_remove($player);
	    }
	}
	foreach my $player (keys %team_B){
	    if($now - $team_B{$player} > $config{'player_idle_time_out'}){
		$team_B{$player}=$now;
		&player_remove($player);
	    }
	}
    }
    
    if ($now - $cshout_timer > $config{'cshout_interval'}){
	&net_write("cshout Consider playing a team game here on $config{'server'}.  They're fun and a good learning experience!  Type 'tg help' to find out more!\n");
	$cshout_timer = $now;
    }	    
}

####################################################################
# parse_ics_output(line)
#
# Parse the different kinds of output we get from the server.
#

sub parse_ics_output {
    my $line = $_[0];

####################################################################
# If requested, get last known status from formula variables 
#

#f1 stores the names and status (1=player,2=captain) of all prospective
#players.
    if($line =~ /^f1:\s+\#(.+?)\s*$/){
	foreach my $player (split(' ', $1)){
	    (my $name, my $status)=split(/:/,$player);
	    $joined{$name}=$status;
	    print "[Added $name to list with status $status]\n";
	}
	&check_players;
	return;
    }

#f2 is for storing which slot we are using
    if($line =~ /^f2:\s+\#(.+?)\s*$/){
	$slot = $1;
	return;
    }

#f3 is for storing info on a game in progress
    if($line =~ /^f3:\s+\#(.+?)\s*$/){
	($game_number , $color_A , $captain_A , $color_B , $captain_B) = split(' ',$1);
	&net_write("observe $game_number\n");
	return;
    }

#f4 is for storing at what time a captain dropped
    if($line =~ /^f4:\s+\#(.+?)\s*$/){
	$captain_drop_timer=$1;
	$game_going =0;
	$team_forming =0;
	return;
    }

####################################################################
# The "in" command can return "<user> is not logged in.", in which case
# they will be removed from the player list.
#

    if ($line =~ /^(\w+) is not logged in./){
	&player_remove (lc($1));
	return;
    }

####################################################################
# If the bot suspects there is a game in progress, it tries to observe
# it.  It should then verify whether the captains and board colors it
# has stored match those from the game it is observing.
#
    
    if ($line =~ /^You are now observing game (\d+)/){
	my $comeback = 0;
      CHECKOB: while(<$net>){

	  #Look for game info in the next three lines from server.
	  last CHECKOB if $comeback++ >= 3;
	  if (/^\s{0,2}<12>\s+(?:\S+\s+){8}([BW])\s+(?:\S+\s+){6}(\d+)\s+(\w+)\s+(\w+)\s+(?:\S+\s+){7}(\d+)/) { 
	      my $turn_color = $1;
	      $game_number = $2;
	      my $player_1 = lc($3);
	      my $player_2 = lc($4);
	      
	      print("[Checking whether $player_1 vs. $player_2 is a team game.]\n");
	      if (($player_1 eq $captain_A && $player_2 eq $captain_B and $color_A="white" and $color_B="black")
		  or ($player_2 eq $captain_A && $player_1 eq $captain_B and $color_A="black" and $color_B="white")){
		  $num_half_moves = $5 * 2;
		  
		  if($turn_color eq 'W'){
		      $num_half_moves--;
		      $team_turn = $color_A eq "white" ? 'A' : 'B';
		  }
		  else{
		      $team_turn = $color_A eq "black" ? 'A' : 'B';
		  }

		  $game_going= 1;
		  $team_forming =0;

		  $logfile = &logfile_name if ($config{'log_games'} && !$logfile);
		  last CHECKOB;
	      }
	  }
      }

	&net_write("unobserve\n") if ! $game_going;

	if ($team_forming){
	    $captain_A=0 if !exists $joined{$captain_A} or $joined{$captain_A}<2;
	    $captain_B=0 if !exists $joined{$captain_B} or $joined{$captain_B}<2;
	}

	&store_game_info;
	&store_captain_drop_time;
	return;
    }

####################################################################
# Direct tells to the bot are parsed here.
#

    if ($line =~ /^(\w+)(\(\S+\))? tells you:\s+(.+?)\s*$/) {
	my $name = lc($1);

	#title should contain all modifications to the user found
	#between two parentheses after the user's name.
	#e.g:  Bob(CA)(*)(TM)
	my $title = $2;
	my $cmd  = $3;

	return if ($name eq $config{'user'}); # this avoids hang up
	&parse_tell ($name, $cmd, $title);
	return;
    }

####################################################################
# Parse channel tells.
#

    if ($line =~ /^(\w+)(?:\(\S*\))?\((\d+)\):\s+(.+?)\s*$/) {
	&parse_channel (lc($1), $2, $3);
    }

####################################################################
# Game notifications.
#
# Remove people who are playing games from the team game.  Also determine
# if a team game has started.
#

    #                 Game notification: Qualm (1471) vs. register (++++) unrated blitz 5 0: Game 67
    elsif ($line =~ /^Game notification:\s+(\w+)\s*\(.*\)\s+vs\.\s+(\w+)\s*\(.*\)\s+(\w+)\s+\w+\s+(\d+)\s+(\d+):\s+Game\s+(\d+)/){
	my $a = lc($1);
	my $b = lc($2);
	my $etime = $4 + $5;
	my $rated = $3;
	my $tmp_game_number = $6;
	if (($a eq $captain_A && $b eq $captain_B and $color_A="white" and $color_B="black") or ($b eq $captain_A && $a eq $captain_B and $color_A="black" and $color_B="white")) {

	    # don't check ratedness for (T) against (T).
	    if ($joined{$captain_A} == 3 and $joined{$captain_B} == 3) {
		$teamname_A = $captain_A;
		$teamname_B = $captain_B;
	    }
	    elsif ($rated eq "rated") {
		&net_write ("tell $captain_A You are playing a rated game.  Team games should be unrated.  Please abort and create an unrated game.\n");
		&net_write ("tell $captain_B You are playing a rated game.  Team games should be unrated.  Please abort and create an unrated game.\n");
		return;
	    }

	    if($team_forming){
		&ch_write (">>> TEAM GAME STARTED <<<  Observe $tmp_game_number! +ch $config{'team_A_channel'} to play $color_A with $captain_A, +ch $config{'team_B_channel'} to play $color_B with $captain_B!  Type 'tg info' to get more info on this game.\n");

		&net_write ("cshout >>> TEAM GAME STARTED <<<  Type 'tg info' and join channel $config{'teams_channel'} to get info about this game.\n");

		my ($suggest_A,$suggest_B) = &suggestions;
		if ($suggest_A){
		    &ch_write ("Suggested teams (by rating):\n");
		    &ch_write("Channel $config{'team_A_channel'} with ${captain_A}:  $suggest_A.\n") if $suggest_A ne "";
		    &ch_write("Channel $config{'team_B_channel'} with ${captain_B}:  $suggest_B.\n") if $suggest_B ne "";
		}
		&net_write ("observe $tmp_game_number\n");
		return;
	    }

	    if(!$game_going){
		&ch_write (">>> TEAM GAME RESTARTED <<<  Observe $tmp_game_number! +ch $config{'team_A_channel'} to play $color_A with $captain_A, +ch $config{'team_B_channel'} to play $color_B with $captain_B!  Type 'tg info' to get more info on this game.\n");
		&net_write ("observe $tmp_game_number\n");
	    }
	    return;
	}

	if ($etime >= 15) {
	    if (exists $joined{$a}) {
		&net_write ("tell $a I'm removing you from the list because you seem to be playing a long game.  ".
			    "Tell me 'join' again if you want to participate in a team game.\n");
		&player_remove ($a);
	    }
	    if (exists $joined{$b}) {
		&net_write ("tell $b I'm removing you from the list because you seem to be playing a long game.  ".
			    "Tell me 'join' again if you want to participate in a team game.\n");
		&player_remove ($b);
	    }
	}	
	return;
    }

####################################################################
# Departure notifications.
#
# Players in the joined hash should be on the bot's notification list,
# so it should receive a message when they leave.
#

    if ($line =~ /^Notification:\s+(\w+) has departed\./) {
	my $player = lc($1);
	&player_remove ($player);
	return;
    }

####################################################################
# Arrival notifications.
#
# Users who add the bot to their notify lists can receive info about
# a game in progress when they arrive.
#

    if ($line =~ /^Notification:\s(\w+) has arrived and isn\'t on your notify list\./) {
	my $player = $1;
	if (!$team_forming) {
	    &net_write ("tell $player Welcome, $player!  We are playing a team game!\n");
	}
	return;
    }

####################################################################
# Game end notifications.
#

    if ($line =~ /^{Game.*\)(.*)}/) {
	my $result = $1;


	if ($result =~ /\baborted/
	    or ($result =~ /adjourned/ and $result =~ /agreement/)){
	    &ch_write ("=== Team Game aborted ===\n");

	    if($logfile){
		if(open (LOGFILE, '>>', $logfile)){ 
		    print LOGFILE "<br><br>=== Team Game aborted ===<br>\n";
		    close (LOGFILE);
		}
	    }

	    $team_forming=1;
	    $game_going=0;
	    $logfile=0;
	    return;
	}

	if ($result =~ /(\w+) lost connection/) {
	    $dropped_captain=$1;
	    &ch_write ("Captain: $dropped_captain lost connection.  Team game adjourned.\n");
	    $game_going=0;
	    $captain_drop_timer=time;
	    &store_captain_drop_time;
	    return;
	}

	my $result_A = .5;
	my $result_B = .5;
	    
	#parse the result to figure out who won.
	if($result =~ /\b(.+)\s+checkmated/ ||
	   $result =~ /\b(.+)\s+forfeits/ ||
	   $result =~ /\b(.+)\s+resigns/){
	    if(lc($1) eq $captain_A){
		$result_A = 0;
		$result_B = 1;
	    }
	    else{
		$result_A = 1;
		$result_B = 0;
	    }
	}

	my ($team_A_name , $team_B_name) = &get_teamnames();

	$result =~ s/$captain_A/$team_A_name/;
	$result =~ s/$captain_B/$team_B_name/;

	&ch_write ("+++ Team Game has ended +++  Result: $result\n");
	    
	my $active_A = $captain_A;
	my $active_B = $captain_B;
	my $highest_A = 1600;
	my $highest_B = 1600;
	my %all_voted = ();

	foreach my $player (keys %voted_A){
	    $all_voted{$player}=1;
	}
	foreach my $player (keys %voted_B){
	    $all_voted{$player}=1;
	}

	foreach my $player (keys %all_voted){
	    if(participation_level($player,'A')>=.5){
		$active_A .= ", $player" unless $player eq $captain_A;
		if (&get_rating($player)>$highest_A){
		    $highest_A=&get_rating($player);
		}
	    }
	    elsif(participation_level($player,'B')>=.5){
		$active_B .= ", $player" unless $player eq $captain_B;
		if (&get_rating($player)>$highest_B){
		    $highest_B=&get_rating($player);
		}
	    }
	}
	    
	foreach my $player (keys %all_voted){
	    &adjust_rating($player,'A',$highest_B,$result_A);
	    &adjust_rating($player,'B',$highest_A,$result_B);
	}
	
	&ch_write ("$team_A_name was:  $active_A.  $team_B_name was:  $active_B.  Thanks to all who participated!\n");
	# save the result of the game into the journal
	&net_write ("jsave $slot $captain_A -1\n");
	$slot++;
	$slot=substr($slot,0,1); #wrap around to 'A' after 'Z'

	if($logfile){
	    if(open (LOGFILE, '>>', $logfile)){ 
		print LOGFILE "<br><br>+++ Team Game has ended +++  Result: $result<br>\n";
		print LOGFILE "<br><br>$team_A_name was:  $active_A.  $team_B_name was:  $active_B.  Thanks to all who participated!<br>\n";
		close (LOGFILE);
	    }
	}
	&store_slot;	
	&initialize;
	return;
    }


####################################################################
# A move is made.
#

    if ($line =~ /^<12>/) {
	
	if($logfile){
	    if(open (LOGFILE, '>>', $logfile)){
		my $board = &html_board($');
		print LOGFILE "$board\n";
		close (LOGFILE);
	    }
	}

	$team_turn = $team_turn eq 'A' ? 'B' : 'A';
	%voted_this_move = ();
	$num_half_moves++;
	&net_write ("jsave $slot\n");
	return;
    }
}

#################################################################
# Helper functions for parse_ics_output.
#
#################################################################
# parse_tell(name,command,title)
#
# The commands must be the first word told to Fics Team Bot otherwise
# commands which take arguments get screwed up.  E.g:  tg check CaptainCool
# would match the captain command.
#

sub parse_tell {
    my ($name, $cmd, $title) = @_;
    my $now = time;

    if (exists $config{'ignore'}{$name}) {
	print ("[Ignoring: $name]\n");
	return;
    }

    # spam guard
    if ( !(exists $config{'admins'}{$name}) and $name eq $previous_command_name) {
	if ($now - $previous_command_time < 2) {
	    $previous_command_count++;
	    print"[spam detected] count = $previous_command_count\n";
	    if ($previous_command_count >= 5) {
		&net_write ("+censor $name\n");
		&player_remove ($name);
		return;
	    }
	    elsif ($previous_command_count >= 2) {
		&net_write ("xtell $name You tried to send $config{'user'} too many commands at once.  Please wait 3 seconds before sending the next command.\n");
		return;
	    }

	    if ($cmd eq $previous_command) {
		return;
	    }
	}
	else {
	    $previous_command_count = 0;
	}
    }
    else {
	$previous_command_count = 0;
    }

    $previous_command_name	= $name;
    $previous_command		= $cmd;
    $previous_command_time	= $now;

    #See if command is admin or captain command first because normal user commands may match anywhere in the phrase, even inside arguments to admin or cap commands
    &send_help ($name,$cmd) unless 
	((exists ($config{'admins'}{$name}) and &parse_admin_command ($name, $cmd)) 
	 or
	 (($name eq $captain_A or $name eq $captain_B)
	  and &parse_captain_command ($name, $cmd))
	 or
	 (&parse_user_command ($name, $cmd, $title)))
}

#################################################################
# parse_user_command(name,command,[title])
#
# return 1 if the command was matched.
#

sub parse_user_command {
    my ($name, $cmd, $title) = @_;

    # help
    if ($cmd =~/\s+\?/ or
	$cmd =~/^\s*help/i){
	&send_help ($name,$');
	return 1;
    }

    # unregistered users can only ask for help
    if (defined $title and $title =~ /\(U\)/){
	&net_write ("xtell $name *** Only registered users can participate in team games. ***\n");
	return 0;
    }

    if ($mute && !exists($config{'admins'}{$name})) {
	&net_write ("xtell $name $config{'user'} is muted.\\n\\nPlease tell " . &admin_names . " to unmute me.  In the mean-time check channel $config{'teams_channel'} for any team games.\n");
	return 1;
    }

    # commands below here cannot be used when the bot is muted

    # join
    if ($cmd =~ /^\s*joi/i ||
	$cmd =~ /^\s*play/i ||
	$cmd =~ /^\s*regi/i)
    {
	$last_activity = time;
	if ($team_forming) {
	    my $old_status = defined $joined{$name} ? $joined{$name} : 0;
	    if (defined $title and $title =~ /\(T\)/){
		if(! &not_kicked($name)){
		    &net_write ("xtell $name The other players have voted against your being a captain.  Sorry.\n");
		    return 1;
		}
		&player_add($name,3) and
		    &net_write("xtell $name You are now listed as a potential captain.\n");
	    }
	    elsif (&player_add($name,1)){
		&net_write ("xtell $name You are now listed as a potential player.\n");
	    }
	    &spam if $old_status != (defined $joined{$name} ? $joined{$name} : 0);
	}
	else {
	    &recommend_team ($name);
	}
	return 1;
    }

    #captain
    if ($cmd =~ /^\s*cap/i) {
	$last_activity = time;
	if ($team_forming) {
	    if(! &not_kicked($name)){
		&net_write ("xtell $name The other players have voted against your being a captain.  Sorry.\n");
		return 1;
	    }
	    my $old_status = defined $joined{$name} ? $joined{$name} : 0;
	    if (defined $title and $title =~ /(T)/){
		&player_add($name,3) and
		    &net_write("xtell $name You are now listed as a potential captain.\n");
	    }
	    else{
		my $type = &player_add($name,2);
		if($type==1){
		    &net_write ("xtell $name You are now listed as a potential player.\n");
		}
		elsif($type==2){
		    &net_write("xtell $name You are now listed as a potential captain.\n");
		}
	    }
	    &spam if $old_status != (defined $joined{$name} ? $joined{$name} : 0);
	}
	else {
	    &recommend_team ($name);
	}
	return 1;
    }

    #kick
    if ($cmd =~ /^\s*kick\s+(\w+)/i ||
	$cmd =~ /^\s*oust\s+(\w+)/i){
	if($game_going){
	    &net_write ("xtell $name You cannot vote to kick a captain while a game is going.\n");
	    return 1;
	}

	my $kickee = lc($1);

	# only registered players can make a kick vote
	$kick_votes{$kickee}{$name}=1 if exists $joined{$name};
	my $votes_needed=&not_kicked($1);
	if(! $votes_needed){
	    if($team_forming){
		&net_write ("xtell $name $kickee will not be allowed to captain the next game.\n");
		&player_add($kickee,1,1) if exists $joined{$kickee};
	    }
	    else{
		&net_write ("xtell $name We will begin forming a new team game now.\n");
		&initialize;
	    }
	}
	else{
	    &net_write ("xtell $name $votes_needed more votes are needed to kick $kickee.\n");
	}
	return 1;
    }

    #appoint
    if ($cmd =~ /^\s*app(?:oint)*\s+(\w+)/i ||
	$cmd =~ /^\s*nom(?:inate)*\s+(\w+)/i ||
	$cmd =~ /^\s*suggest\s+(\w)/i){
	if (! $team_forming){
	    &net_write ("xtell $name You cannot vote to appoint a captain unless a team game is forming.\n");
	    return 1;
	}
	if (! exists $joined{$name}){
	    &net_write ("xtell $name Please sign up for the team game (by telling me 'join') before you vote to appoint a captain.\n");
	    return 1;
	}

	my $nominee = lc($1);

	$appoint_votes{$nominee}{$name}=1;
	my $appoint_list = $appoint_votes{$nominee};
	my $votes_needed = $config{'needed_to_appoint'} - keys %$appoint_list;
	if($votes_needed <=0){
	    &net_write ("xtell $name $nominee has been appointed as a possible captain.\n");
	    if(!exists $joined{$nominee}){
		&player_add($nominee,4,1);
		&spam;
	    }
	}
	else{
	    if($votes_needed>1){
		&net_write ("xtell $name $votes_needed more votes are needed to appoint $nominee as a captain.\n");
		&ch_write ("$votes_needed more votes are needed to appoint $nominee as a captain.\n");
	    }
	    else{
		&net_write ("xtell $name one more vote is needed to appoint $nominee as a captain.\n");
		&ch_write ("1 more vote is needed to appoint $nominee as a captain.\n");
	    }
	}
	return 1;
    }
	
    #rating
    if ($cmd =~ /^\s*check\s*/i) {
	my $checkee=lc($');
	my $wanted="your";
	if (! $checkee){
	    $checkee=$name;
	}
	else{
	    $wanted="${checkee}'s";
	}
	my $check=&get_rating($checkee);
	&net_write("xtell $name $wanted rating is $check.\n");
	return 1;
    }

    # participation
    if ($cmd =~ /^\s*part(?:icipat)*(?:e)*(?:ion)*\s+(\w+)/i) {
	my $wanted = lc($1);
	my $part_pct_A = sprintf("%.3f%%",&participation_level($wanted,'A')*100);
	my $part_pct_B = sprintf("%.3f%%",&participation_level($wanted,'B')*100);
	my ($team_A_name , $team_B_name) = &get_teamnames(); 
	&net_write("tell $name ${wanted}'s participation level is $part_pct_A for $team_A_name and $part_pct_B for $team_B_name.\n");
	return 1;
    }

    # channel
    if ($cmd =~ /^\s*chan/i) {
	if(exists $team_A{$name} or $name eq $captain_A){
	    &net_write("tell $name You are in channel $config{'team_A_channel'}.\n");
	    return 1;
	}
	if(exists $team_B{$name} or $name eq $captain_B){
	    &net_write("tell $name You are in channel $config{'team_B_channel'}.\n");
	    return 1;
	}
	&net_write("tell $name You are not in any channel.\n");
	return 1;
    }
	
    #withdraw
    if ($cmd =~ /^\s*with/i
	or $cmd =~ /^\s*leav/i
	or $cmd =~ /^\s*cancel/i
	or $cmd =~ /^\s*quit/i
	or $cmd =~ /^\s*unjoi/i){
	if ($team_forming){
	    if (&player_remove ($name)) {
		&net_write ("xtell $name OK.  You have been withdrawn from the team game.\n");
	    }
	    else {
		&net_write ("xtell $name OK.\n");
	    }
	}
	else{
	    &net_write ("xtell $name OK.\n");
	}
	return 1;
    }

    # info
    if ($cmd =~ /^\s*inf/i
	or $cmd =~ /^\s*list/i) {
	$last_activity = time;
	&send_info ($name);
	return 1;
    }

    #command not recognized
    return 0;
}

#######################################################################
# parse_admin_command(name,command)
#
# (returns 1 on successful parsing)
#

sub parse_admin_command {
    my ($name, $cmd) = @_;

    if ($cmd =~ /^\s*add\s+(\w+)/i) {
	my $player = lc($1);	
	&player_add($player,1,1);
	&net_write ("xtell $name [$player] is now listed as a regular player.\n");
	return 1;
    }

    if ($cmd =~ /^\s*addcap\s+(\w+)/i) {
	my $player = lc($1);
	my $options = $';
	if (exists $joined{$player} and $joined{$player} >= 2) {
	    &net_write ("xtell $name [$player] is already a captain.\n");  
	}
	else {
	    &net_write ("xtell $name [$player] is now listed as a potential captain.\n");
	    if ($options =~ /t/i) {
		&player_add($player,3,1);
	    }
	    elsif ($options =~ /c/i) {
		&player_add($player,4,1);
	    }
	    else {
		&player_add($player,2,1);
	    }
	}
	return 1;
    }

    if ($cmd =~ /^\s*remove\s+(\w+)/i) {
	my $player = lc($1);
	if($player eq "all"){
	    foreach my $gone (keys %joined){
		&player_remove ($gone);
	    }
	    &net_write ("xtell $name Player list cleared.\n");
	}
	elsif (&player_remove ($player)) {
	    &net_write ("xtell $name [$player] removed from the list.\n");
	}
	else {
	    &net_write ("xtell $name [$player] isn't in the list or is a captain for whom we are waiting to return.\n");
	}
	return 1;
    }
    
    if ($cmd =~/^\s*spam/i) {
	&spam;
	return 1;
    }

    if ($cmd =~ /^\s*valentine\s+(\w+)\s+/i){
	&send_valentine($name,$1,$');
	return 1;
    }
    
    if ($cmd =~/^\s*act/i) {
	my $now = time;
	if ($last_activity) {
	    my $minutes = sprintf ("%d", ($now - $last_activity) / 60);
	    &net_write ("xtell $name Last activity detected $minutes minutes ago.\n");
	}
	else {
	    &net_write ("xtell $name No activity detected.\n");
	}
	return 1;
    }

    if ($cmd =~ /^\s*reset/i) {
	&net_write ("xtell $name Reset successful.\n");
	&initialize;
	return 1;
    }

    if ($cmd =~ /^\s*rate\s+(\w+)\s+(\w+)/i) {
	my $player = lc($1);
	&set_rating($player);
	$ratings{$player}=$2;
	&set_hash_rating($rating_slot{$player},$2);
	&net_write ("xtell $name ${player}'s rating is now $2.\n"); 
	return 1;
    }

    if ($cmd =~ /^\s*mute/i ||
	   $cmd =~ /^\s*unmute/i) {
	if ($mute){
	    &net_write ("xtell $name $config{'user'} Unmuted.\n");
	    $mute=0;
	}
	else {
	    &net_write ("xtell $name $config{'user'} Muted.\n");
	    $mute=1;
	    &initialize;
	}
	return 1;
    }

    if ($cmd =~ /^\s*setup\s+(\w+)\s+(\w+)/i){
	$captain_A = lc($1);
	$captain_B = lc($2);
	&net_write ("observe $captain_A\n");
	return 1;
    }

    if ($cmd =~ /^\s*pose\s+/i) {
        my $what = $';
	&net_write ("$what\n");
	$pose_timer=time;
	$pose_admin=$name;
	return 1;
    }

    if ($cmd =~ /^\s*die/i or
	$cmd =~ /^\s*bye/i) {
	&net_write ("xtell $name Shutting down $config{'user'}...\n");
	exit(0);
    }

    #command not matched
    return 0;
}

####################################################################
# parse_captain_command(name, command)
#
# return 1 if command was matched
#

sub parse_captain_command {
    my ($name, $cmd) = @_;
    
    if ($cmd =~ /^\s*(?:nameteam|name team|name)\s+/i) {
	if ($name eq $captain_A){
	    $teamname_A=$';
	}
	else{
	    $teamname_B=$';
	}
	return 1;
    }
    return 0;
}

####################################################################
# parse_channel(name,channel,line)
#

sub parse_channel {
    my ($name, $channel, $line) = @_;
    
    return 0 if $name eq $config{'user'};

    if (!$team_forming){
	if ($channel eq $config{'team_A_channel'}) {

	    if($logfile){
		if (open (LOGFILE, '>>', $logfile)){ 
		    print LOGFILE "<b>(TEAM A) $name :  $line<br></b>\n";
		    close (LOGFILE);
		}
	    }

	    $team_A{$name} = time;
	    &player_add($name) if !(exists $joined{$name});
	    delete $team_B{$name};
	    
	    if($team_turn eq 'A' && ! exists $voted_this_move{$name}){
		$voted_this_move{$name}=1;
		$voted_A{$name}++;
	    }
	}
	elsif ($channel eq $config{'team_B_channel'}) {

	    if($logfile){
		if (open (LOGFILE, '>>', $logfile)){ 
		    print LOGFILE "(TEAM B) $name :  $line<br>\n";
		    close (LOGFILE);
		}
	    }

	    $team_B{$name} = time;
	    &player_add($name) if !(exists $joined{$name});
	    delete $team_A{$name};

	    if($team_turn eq 'B' && ! exists $voted_this_move{$name}){
		$voted_this_move{$name}=1;
		$voted_B{$name}++;
	    }
	}
    }

    if ($channel eq $config{'teams_channel'}) {
	if($line=~ /\bteam/i
	   or $line=~ /\banother/i
	   or $line=~ /start/i
	   or $line=~ /\bplay/i
	   or $line=~ /\bjoin/i
	   or $line=~ /\bcap/i
	   or $line=~ /info/i
	   or $line=~ /game/i){
	    $last_activity = time;
	    print "[Activity detected in channel $config{'teams_channel'}]\n";
	}
	
	if($logfile){
	    if(open (LOGFILE, '>>', $logfile)){ 
		print LOGFILE "&nbsp&nbsp&nbsp&nbsp $name :  $line<br>\n";
		close (LOGFILE);
	    }
	}

	if (($line=~ /\btg/i or $line =~ /\bteamgame/i) and ! exists $joined{$name}){
	    $last_activity = time and print "[Activity detected in channel $config{'teams_channel'}]\n";
	    if ($team_forming) {
		&player_add ($name);
		&spam;
	    }
	    else {
		&recommend_team ($name);
	    }
	}
    }
}

########################################################################
# player_add (name,[status],[admin])
#
# Set admin to 1 if the player was added by issuing an admin command.
#
# Add a player to the list of those playing or waiting to play, or change
# that player's status.
#
# Status defaults to 1.  If the status given is 2 (captain), and the player's
# status already is 2, the status will be switched to 1.
#
# It assumes that a team game is forming; do not call this function unless
# that is the case.
#
# returns the player's status.
#

sub player_add {
    my ($name , $status , $admin) = @_;

    # spam guard against consecutive join/withdraw , join/cap
 
    if (! $admin){
	my $now = time;
	if ($previous_joined_player eq $name and $now - $previous_joined_time < 30){
	    &net_write("xtell $name Please wait 30 seconds and tell $config{'user'} this command again.\n");
	    return 0;
	}
	$previous_joined_time	= $now;
	$previous_joined_player	= $name;
    }

    if(exists $joined{$name}){
	if($joined{$name}==1 && defined $status && $status==2){
	    $joined{$name}=2;
	}
	elsif(! defined $status || $status<=2){
	    $captain_A=0 if $captain_A eq $name;
	    $captain_B=0 if $captain_B eq $name;
	    $joined{$name}=1;
	}
	else{
	    $joined{$name}=$status;
	}
    }
    else{
	if(defined $status){
	    $joined{$name}=$status;
	}
	else{
	    $joined{$name}=1;
	}
	&add_notify($name);
    }

    &store_players;
    return $joined{$name};
}

sub add_notify {
    my $name = $_[0];
    &net_write ("+notify $name\n");
    &net_write ("+gnotify $name\n");
}

####################################################################
# player_remove(name)
#
# Removed name player from games played or forming.
#

sub player_remove {
    my $name = $_[0];

   #don't remove a captain unless the teams are still forming
    return 0 if !$team_forming and ($name eq $captain_A or $name eq $captain_B);
    &net_write ("-notify $name\n");
    &net_write ("-gnotify $name\n");
    return 0 if !exists($joined{$name});
	
    delete $joined{$name};
    print "[Removing: $name]\n";
    delete $team_A{$name} if exists $team_A{$name};
    delete $team_B{$name} if exists $team_B{$name};
    
    if($team_forming){
	$captain_A=0 if $captain_A eq $name;
	$captain_B=0 if $captain_B eq $name;
    }

    &store_players;
    
    return 1;
}

sub assign_captains {
    return 1 if ($captain_A && $captain_B);
    foreach my $player (keys %joined) {
	if (exists $joined{$player} and $joined{$player} >= 2) {
	    if (!$captain_A and $player ne $captain_B) {
		$captain_A = $player;
	    }
	    elsif (!$captain_B and $player ne $captain_A) {
		$captain_B = $player;
	    }
	}
    }
    return ($captain_A && $captain_B);
}

sub not_kicked{
    my $name=$_[0];
    my $kick_list = $kick_votes{$name};
    my $votes_needed = $config{'needed_to_kick'} - keys %$kick_list;
    $votes_needed = 0 if $votes_needed < 0;
    return $votes_needed;
}

sub admin_names{
    my $admins_ref = $config{'admins'};
    join(' ',keys %$admins_ref);
}

sub logfile_name{
    (my $sec,my $min,my $hour,my $mday,my $mon,my $year) = localtime(time);
    $year += 1900;
    $mon ++;
    my $filename = "$config{'logfile_prefix'}${mon}.${mday}.${year}_at_${hour}.${min}.log.html";
    return $filename;
}

sub html_board {
    my $move_input = $_[0];
    my $board = $move_input . "<br>";
    return $board;
}

sub get_teamnames{
    return ("Team $teamname_A" . ($teamname_A eq "" ? "" : " ") . "in channel $config{'team_A_channel'}" ,
	    "Team $teamname_B" . ($teamname_B eq "" ? "" : " ") . "in channel $config{'team_B_channel'}");
}

sub send_valentine{
    my ($from,$to,$message) = @_;
    my $pad = "";
    my $tildes = "";
    my $tilde_pad = "";

    for (my $i=1;$i<length($from);$i+=2){
	$pad .= " ";
    }
    for (my $i=2;$i<=length($from);$i+=1){
    	$tildes .= "~";
    }
    for (my $i=0;$i<=(length($from)-length($tildes))/2;$i++){
	$tilde_pad .= " ";
    }

    $message=" $message " while length($message) < 10;
    $message = substr($message,0,10) if length($message) > 10;
    my $valentine =
	"$pad   .~**~.  .~**~.\\n" .
	"$pad  |      \\/      |\\n" .
	"$pad   \\_${message}_/\\n" .
	"$pad     \\_      _/\\n" .
	"$pad       \\_  _/\\n" . 
	"$pad         \\/\\n" .
	"$tilde_pad    .%*o0${tildes}0o*%.\\n" .
	".%*o0~ From $from ~0o*%.\\n";
    &net_write("xtell $to $valentine\n");
}

sub send_info {
    my $name = $_[0];
    if ($team_forming){
	&net_write ("xtell $name " . &get_player_list . "\\n\\n" .
		    &why_not_start . "\\n\\n" .
		    "Type 'tg captain' to become a captain or 'tg join' to join the game.\n");
    }    
    else {
	(&recommend_team($name));
    }
}

sub get_player_list {
    my $line;
    my $players = "";
    my $captains = "";
    my $total_count = 0;

    unless (keys %joined) {

	# use this opportunity to clear the ratings hash
	%ratings=();
	%rating_slot=();
	return "No one is signed up for the team game yet.";
    }

    foreach my $player (keys %joined) {
	if ($joined{$player} >= 2){
	    $captains .= " $player(" . &get_rating($player) . ")";
	    $total_count ++;
	}
	else{
	    $players .= " $player(" . &get_rating($player) . ")";
	    $total_count ++;
	}
    }

    $line = "Team game forming!  Registered players: " . ($players eq "" ? " none" : $players);
    $line .= ".  Registered captains: " . ($captains eq "" ? " none" : $captains);
    $line .= ".  $total_count total.";
}

sub why_not_start {
    if (&ready_to_start) {
	return "Our team game will begin when $captain_A and $captain_B match each other.";
    }
    if (&assign_captains){
	my $nmore = $config{'needed_to_start'} - keys %joined;
	return "We need $nmore more player" . ($nmore > 1 ? "s" : "") . " to join before we can start.\n";
    }
    return "Two players must register to be captains before we can start.";
}

sub ready_to_start {   
    &assign_captains and keys %joined >= $config{'needed_to_start'};
}

sub spam {
    $spam_timer = time;
    if ($team_forming) {
	&ch_write(&get_player_list . "\n");
	&ch_write(&why_not_start . "\n");
	return;
    }
    if ($game_going) {
        my $channel = &small_channel;
	&ch_write ("*** TEAM GAME IN PROGRESS!! *** Observe $game_number!  ".
		   "Channel $channel needs more players.  Type 'tg info' for more info on this game.\n");
	return;
    }
    &ch_write ("We are waiting " . &captain_time_left . " more minutes for $dropped_captain, who was a team captain, to return so we can restart the team game.\n");
}

#################################################################
# Help system.
#

sub send_help {
    my ($name , $topic) = @_;

    return if !defined $name;

    if($topic =~ /\bteam/i){
	&net_write("xtell $name General procedure for team games:\\n\\n  Potential players gather in channel $config{'teams_channel'} until at least $config{'needed_to_start'} players and at least two captains have registered.  (See 'join' and 'cap'.)\\n\\n  Each team has its own channel, either $config{'team_A_channel'} or $config{'team_B_channel'}, where they discuss the next move to be played.  Each team captain has the responsibility of entering the move agreed on by their team.\\n\\n  Captains match each other at the start of the game using $config{'default_time_controls'} or some other time control agreed upon by all.  The game should be unrated unless both Captains have team accounts (T) and wish to play for rating points.\\n\\n  Once the game has started, picking a side to join is as simple as typing '+ch $config{'team_A_channel'}' or '+ch $config{'team_B_channel'}' to join channel $config{'team_A_channel'} or $config{'team_B_channel'} and then letting the captain in that channel know you are present with 'tell $config{'team_A_channel'} (or $config{'team_B_channel'})'.\n");
    }
    elsif ($topic =~ /\brati/i){
	&net_write ("xtell $name Team game ratings are calculated using the ELO system.  Your rating after a game is adjusted as if you had played directly against the strongest player from the opposing team who voted on 50% or more of the moves.  The rating adjustment is multiplied by the percentage of moves in which you voted.  If you switched teams, you are counted as only having played for the team for which you voted the most.  Leaving the game early or disconnecting has no bearing on rating adjustment, except to possibly lower the percentage of moves in which you voted.\n");
    }
    elsif ($topic =~ /\die/i && exists($config{'admins'}{$name})) {
	&net_write ("xtell $name Command: 'DIE' also 'BYE'\\n\\n  Cause $config{'user'} to shut down.  It will exit the server and not try to come back.\n");
    }
    elsif ($topic =~ /\badmin/i && exists($config{'admins'}{$name})) {
	&net_write ("xtell $name Admin commands:\\n\\nADD <player>\\nADDCAP <player>\\nDIE\\nREMOVE <player | all>\\nSETUP <capA> <capB>\\nSPAM\\nACTIVITY\\nRESET\\nMUTE\\nPOSE <action>.\\nRATE <player>\\n\\nType 'tg help <command name>' for a detailed explanation about a command.\n");
    }
    elsif ($topic =~ /\binfo/i){
	&net_write ("xtell $name Command: 'INFO'\\n\\n  If there is a team game in progress, this command will tell you information such as the teams' players and captains, colors, and which channels they are using.  If a game is still forming, it will list all interested players, along with their team game ratings.  (See also 'help ratings'.)\n");
    }
    elsif ($topic =~ /\bcap/i){
	&net_write ("xtell $name Command: 'CAPTAIN' also 'CAP'\\n\\n  Offer to be a captain in the next team game.\\n\\n  If more than two captains apply, only the first two will be captains, but if one drops before the game starts, the next in line is chosen, so more than two potential captains can be stored.\\n\\n  If you change your mind about being captain, just type 'tg withdraw' to leave the game forming, or 'join' to change your status to a regular player.\\n\\n\n");
    }
    elsif ($topic =~ /\bapp/i){
	&net_write ("xtell $name Command: 'APPOINT <computer>' also 'APP'\\n\\n Vote to have a (C) or (TD) account participate in the team game as a captain.  $config{'needed_to_appoint'} people must vote for a computer, before it can be appointed.  See also the 'kick' command.\n");
    }
    elsif ($topic =~ /\bkick/i){
	&net_write ("xtell $name Command: 'KICK <captain>'\\n\\n Vote to have a captain removed.  This only works if the game is still forming, or if a game was in progress and that captain has dropped.  $config{'needed_to_kick'} people must vote to kick a captain before it can take effect.\n");
    }
    elsif ($topic =~ /\bpart/i){
	&net_write ("xtell $name Command: 'PARTICIPATION <player>' also 'PART'\\n\\n  Returns the participation level of a player for both teams as a percentage of moves voted on.\n");
    }
    elsif ($topic =~ /\bjoi/i ||
	   $topic =~ /\bplay/i ||
	   $topic =~ /\bregi/i){
	&net_write ("xtell $name Command: 'JOIN' also 'PLAY'\\n\\n Register to play in the next team game.  You may withdraw by telling me 'WITHDRAW'.  You may later offer to be a captain, with the 'CAPTAIN' command.\n");
    }
    elsif ($topic =~ /\bwith/i){
	&net_write ("xtell $name Command: 'WITHDRAW' also 'LEAVE'\\n\\n  Withdraw from game forming.\\n  $config{'user'} will also remove you if you disconnect from $config{'server'}.\n");
    }
    elsif ($topic =~ /\bcomm/i){
	&net_write ("xtell $name Commands are told to me in the form 'tell $config{'user'} command'. Server aliases 'tb' and 'tg' exist so that you only have to type 'tg (or tb) command' to run a command (tg stands for team game.)\\n\\n  The commands are:\\n\\n'INFO' - get information on current team game.\\n'JOIN' - join the game forming.\\n'WITHDRAW' - leave the game forming.\\n'CAPTAIN' - offer to be a captain in the game forming.\\n'APPOINT' - Appoint a computer captain.\\n'KICK' - Vote to remove a captain.\\n'NAMETEAM' - A command usable by team captains to name their team\\n'CHECK' - Check someone's rating\\n'HELP' or '?' - access the help system.\\n\\nType 'tg help <command>' for additional information on a specific command.\n");
	&net_write ("xtell $name See 'help admin' for admin commands.\n") if(exists $config{'admins'}{$name});
    }
    elsif ($topic =~ /\bname/i){
	&net_write ("xtell $name Command: 'NAMETEAM <name>' also 'NAME <name>'\\n\\n  Used by team captains to designate <name> as the name of their team.\n");
    }
    elsif ($topic =~ /\bcheck/i){
	&net_write ("xtell $name Command: 'CHECK [name]'\\n\\n  Return [name]'s team game rating.  If no name is given, your own rating will be returned.\n");
    }
    elsif ($topic =~ /\bact/i && exists $config{'admins'}{$name}){
	&net_write ("xtell $name Admin command: 'ACTIVITY'\\n\\n  Get back message on how much team game interest there is.\n");
    }
    elsif ($topic =~ /\baddcap/i && exists $config{'admins'}{$name}){
	&net_write ("xtell $name Admin command: 'ADDCAP <name> [t|c]'\\n\\n  Designate <name> as potential captain.  If the 't' or 'c' option is added, the player is designated as a team account or computer account respectively.\n");
    }
    elsif ($topic =~ /\brate/i && exists $config{'admins'}{$name}){
	&net_write ("xtell $name Admin command: 'RATE <name> <rating>'\\n\\n  Change <name>'s team game rating to <rating>.  Please use sparingly.\n");
    }
    elsif ($topic =~ /\badd/i && exists $config{'admins'}{$name}){
	&net_write ("xtell $name Admin command: 'ADD <name>'\\n\\n  Add <name> to list of players.\n");
    }
    elsif ($topic =~ /\bmute/i && exists $config{'admins'}{$name}){
	&net_write ("xtell $name Admin command: 'MUTE' also 'UNMUTE'\\n\\n  Toggle $config{'user'}\'s ability to send game information.\\n\\n  When muted, $config{'user'} will not respond to normal user commands, except 'help', and will not make any announcements other than a general cshout about team games.  Receiving another 'MUTE' command cancels this effect.\n");
    }
    elsif ($topic =~ /\bpose/i && exists $config{'admins'}{$name}){
	&net_write ("xtell $name Admin command: 'POSE <command>'\\n\\n  Have $config{'user'} execute the $config{'server'} command, <command> and tell you the results.\\n\\n  Don't abuse this command by having $config{'user'} interact with $config{'server'} excessively in a way which it wasn't intended.\n");
    }
    elsif ($topic =~ /\bremove/i && exists $config{'admins'}{$name}){
	&net_write ("xtell $name Admin command: 'REMOVE <name | all>'\\n\\n  Remove <name> from the player list for the current game forming.  'REMOVE all' removes all names.\n");
    }
    elsif ($topic =~ /\breset/i && exists $config{'admins'}{$name}){
	&net_write ("xtell $name Admin command: 'RESET'\\n\\n  Clear $config{'user'}\'s internal team list and put it in team formation mode.\\n\\n  Also removes some names from the gnotify and notify lists, so they don't fill up.\n");
    }
    elsif ($topic =~ /\bsetup/i && exists $config{'admins'}{$name}){
	&net_write ("xtell $name Admin command: 'SETUP <captainA> <captainB>'\\n\\n  $config{'user'} will follow and give information on a teamgame that was started without $config{'user'}.\\n\\n  <captainA> and <captainB> are the captains of the teams using channels $config{'team_A_channel'} and $config{'team_B_channel'} respectively.\n");
    }
    elsif ($topic =~ /\bspam/i && exists $config{'admins'}{$name}){
	&net_write ("xtell $name Admin command: 'SPAM'\\n\\n  Make me advertise the game forming or in progress in the team game channel ($config{'teams_channel'}).\n");
    }
    else{
	
	#No match, generic help
	&net_write ("xtell $name Help topics:\\n\\nType 'tg help commands' for a list of my commands.\\nType 'tg help teamgame' to learn how team games work.\\nType 'tg help ratings' to learn how I calculate player ratings.\\n\\nAdd channel $config{'teams_channel'} (team game channel) to your channel list ('+ch $config{'teams_channel'}') to talk about team games.\\n\\nType 'tg info' to find out if there is a team game underway right now.\n");
	&net_write ("xtell $name Type 'tg help admin' for a list of admin commands.\n") if(exists $config{'admins'}{$name});
    }
}

#################################################################
# Commands for helping a player find a team.
#

sub recommend_team {
    my $name = $_[0];
    if ($game_going) {
	my %players_A	= %team_A;
	my %players_B	= %team_B;
	my $team_A_list	= "";
	my $team_B_list	= "";

	delete $players_A{$captain_A} if exists $players_A{$captain_A};
	delete $players_B{$captain_B} if exists $players_B{$captain_B};

	$team_A = "$captain_A(" . &get_rating($captain_A) . ")(captain)";
	$team_B = "$captain_B(" . &get_rating($captain_B) . ")(captain)";

	foreach my $player (keys %players_A) {
	    $team_A .= " $player(" . &get_rating($player) . ")";
	}

	foreach my $player (keys %players_B) {
	    $team_B .= " $player(" . &get_rating($player) . ")";
	}

	my ($team_A_name , $team_B_name) = &get_teamnames();

	&net_write ("xtell $name Team game in progress:  'observe $game_number'\\n\\n$team_A_name ($color_A) is:\\n  $team_A.\\n\\n$team_B_name ($color_B) is:\\n  $team_B.\\n\\nPlease join the channel you feel you would help the most using '+ch $config{'team_A_channel'}' or '+ch $config{'team_B_channel'}'\n");
    }
    else {
	&net_write ("xtell $name We are waiting " . &captain_time_left . " more minutes for $dropped_captain, who was a team captain, to return so we can restart the team game.\n");
    }
}

sub captain_time_left {
    my $time_left = $config{'captain_drop_time_out'} - (time - $captain_drop_timer);
    sprintf ("%d", $time_left / 60);
}

sub suggestions {
    my %players_hash = %joined;
    delete $players_hash{$captain_A};
    delete $players_hash{$captain_B};
    my @suggest_A = ();
    my @suggest_B = ();
    my @best_players = sort {
	&get_rating($a) <=> &get_rating($b)
    } keys %players_hash;
    my $turn_B = 0;
    $turn_B = 1 if &get_rating($captain_A) > &get_rating($captain_B);
 
    while (@best_players) {
	my $gift = pop @best_players;
	$turn_B ? push (@suggest_B, $gift) : push (@suggest_A, $gift);
	$turn_B = (! $turn_B);
    }
       
    return (join (' ', @suggest_A) , join (' ', @suggest_B));
}

sub small_channel {
    scalar keys %team_A < scalar keys %team_B ? $config{'team_A_channel'} : $config{'team_B_channel'};
}

#######################################################################
# Functions for storing state information in formula variables, so that
# the bot can try to recover from a crash.
#

sub store_players {
    my $player_string="set f1 \#";
    foreach my $player (keys %joined){
	$player_string .= "$player:$joined{$player} ";
    }
    &net_write("$player_string\n");
}

sub store_game_info {
    if($team_forming){
	&net_write("set f3\n");
    }
    else{
	&net_write("set f3 \#$game_number $color_A $captain_A $color_B $captain_B\n");
    }
}

sub store_captain_drop_time {
    if(!$game_going && !$team_forming){
	&net_write("set f4 \#$captain_drop_timer\n");
    }
    else{
	&net_write("set f4\n");
    }
}

sub store_slot {
    &net_write("set f2 \#$slot\n");
}

#######################################################################
# Ratings and Participation.
#
# The ratings file is a hash table.
#
# Name lengths are fixed at 17 bytes.  Ratings are 2 bytes.
#
# If 6 people are added to the rating system a day, it will take 9.5 years
# for the ratings file to get 1/4 full.  There are 17^4 = 83521 slots in
# the file.  Each slot takes up 19 bytes, so the file is a constant 1586899
# bytes or 1.51 MB.  I do not compress the file.
#
    
# participation_level(name,team);
#
# Returns the percentage of moves voted on by name for team.  If
# name is more active on the opposite team, the level returned is 0.
#

sub participation_level {
    (my $name , my $team) = @_;
    
    if(exists $voted_A{$name}){
	if(exists $voted_B{$name}){
	    if($voted_A{$name} > $voted_B{$name}){
		if($team eq "A"){
		    return $voted_A{$name} / ($num_half_moves / 2);
		}
		return 0;
	    }
	    if($voted_B{$name} > $voted_A{$name}){
		if($team eq "B"){
		    return $voted_B{$name} / ($num_half_moves / 2);
		}
	    }
	    return 0;
	}
	if($team eq "A"){
	    return $voted_A{$name} / ($num_half_moves / 2);
	}
	return 0;
    }
    if($team eq "B" and exists $voted_B{$name}){
	return $voted_B{$name} / ($num_half_moves / 2);
    }
    return 0;
}

# get_rating(name)
#

sub get_rating{
    my $name = $_[0];
    return $ratings{$name} if exists $ratings{$name};
    my $current_slot = &ratings_hash($name);
    for (my $current_name="";($current_name=&get_hash_name($current_slot)) ne "";){
	if($current_name eq $name){
	    $ratings{$name}=&get_hash_rating($current_slot);
	    $rating_slot{$name}=$current_slot;
	    return $ratings{$name};
	}
	$current_slot++;
	$current_slot=0 if $current_slot == $max_hash;
    }
    $ratings{$name}=1600;
}

# adjust_rating(name,team,opp_rating,outcome)
#
# outcome = 1, .5, or 0
#

sub adjust_rating{
    my($name, $team, $opp_rating, $outcome) = @_;
    &set_rating($name);

    my $participated = &participation_level($name,$team);
    return 0 if not $participated;

    my $rating_change = 32 * ($outcome - $ratings{$name} / ($opp_rating + $ratings{$name})) * $participated;
    $ratings{$name} = sprintf ("%d", ($ratings{$name} + $rating_change));

    &set_hash_rating($rating_slot{$name},$ratings{$name});
}

####################################################################
# Low level helper functions for ratings file functions.
#

# set_rating(name)
#
# This makes sure %rating_slot and %ratings are valid for the player.
#

sub set_rating{
    my $name = $_[0];

    return if (exists $ratings{$name} and exists $rating_slot{$name});

    my $current_slot = &ratings_hash($name);
    for (my $current_name="";($current_name=&get_hash_name($current_slot)) ne "";){
	if($current_name eq $name){
	    $ratings{$name}=&get_hash_rating($current_slot);
	    $rating_slot{$name}=$current_slot;
	    return;
	}
	$current_slot++;
	$current_slot=0 if $current_slot == $max_hash;
    }
    $ratings{$name}=1600;
    $rating_slot{$name}=$current_slot;
    &set_hash_name($current_slot,$name);
}

# set_hash_rating(slot,rating)
#

sub set_hash_rating {
    (my $this_slot,my $this_rating) = @_;
    my $bytes = pack("n",$this_rating);
    sysseek RATINGS,$this_slot * 19 + 17,0;
    syswrite (RATINGS,$bytes,2);
}

# get_hash_name(slot)
#

sub get_hash_name {
    my $this_slot = $_[0];
    my $bytes = "";
    sysseek RATINGS, $this_slot * 19 , 0;
    sysread (RATINGS,$bytes,17);
    my $name = unpack("a*",$bytes);
    $name =~ s/\0//g;
    return $name;
}

# get_hash_rating(slot)
#

sub get_hash_rating {
    my $this_slot = $_[0];
    my $bytes= 0;
    sysseek RATINGS,$this_slot * 19 + 17,0;
    sysread (RATINGS,$bytes,2);
    unpack("n",$bytes);
}

# ratings_hash(name)
#
# Return the probable slot in the ratings file for the name.
#

sub ratings_hash{
    my $name = lc $_[0];
    my @letter = split(//,$name);
    $hash_val=0;
    for (my $i=0; $i < 4; $i++){
	$letter[$i]=' ' if not defined $letter[$i];
	$hash_val += (&seventeen_to_the($i) * $hash_symbol{$letter[$i]}) if(exists $hash_symbol{$letter[$i]});
    }
    return $hash_val;
}

# set_hash_name(slot,name)
#

sub set_hash_name {
    (my $this_slot,my $this_name) = @_;
    my $bytes = pack("a17",$this_name);
    sysseek RATINGS, $this_slot * 19 , 0;
    syswrite (RATINGS,$bytes,17);
}

# seventeen_to_the(y)
# 
# Return 17^y.
#

sub seventeen_to_the{
    my $power = $_[0];
    return 1 if not $power;
    17 * seventeen_to_the($power-1);
}
