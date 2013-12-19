#!/usr/bin/perl 

####################
# CLOBAL OPTIONS

use jobyxconfig;

# END GLOBAL OPTIONS
####################

#####################################################################
# Don't edit the following lines unless you know what you're doing! #
#####################################################################

our $version="0.99.7 beta";

use IO::Socket::INET;
use Carp;
sub _die ($);
$|=1;
our $logopen=0;

print "Loading JobyX $version...\n";
print "This version of JobyX was coded by Chris Howie.\n\n";

$Lang::loaded or _die "No language pack was found.  Please insert one into ".
"jobyxconfig.pm.";

#possible config omissions
if (not $username) {
	_die "No username.";
} elsif (not $password) {
	_die "No password.";
} elsif (not $server) {
	_die "No server.";
} elsif (not $port) {
	_die "No port.";
} elsif ($port<1 or $port>65535) {
	_die "Invalid port.";
} elsif (not $operator) {
	_die "No operator.";
}

mkdir 'data';
mkdir 'data/players';
mkdir 'data/lists';

#open global log
print "Opening logfile...\n";
open(LOG,">>$logfile") or _die "Can't open logfile $logfile for append.";
$logopen=1;
select LOG;$|=1;select STDOUT;
print LOG "\n\n====== Initializing JobyX... ======\n";

#global variables
our(@modules,@commands,@lists,@variables,@avariables,@users,@wlines,
$finger,$who,$last_flags,@whol,%lists,%avariables);
print "Inserting modules...";
foreach my $i (@default_modules) {
	&insmod('*DEBUG*',$i) or _die "Can't load module \"$i\"!";
}
print " done.\n";
print "Loading saved files...";
{
	my $res=&loadall;
	if ($res & 1) {
		_die "Failed to load stored lists!";
	} elsif ($res & 2) {
		_die "Failed to load stored avariables!";
	}
}
print " done.\n";
print LOG "====== JobyX core initialized at ".(scalar localtime)." ======\n";
print "Login options:\n";
print "\tusername:\t$username\n";
print "\tpassword:\t(hidden)\n";
print "\tserver:\t\t$server:$port\n\n";

#connect
our $net;
{
	my $retries=0;
	print "Each dot below represents one try.\n";
	print "Attempting to connect to $server:$port.";
	while (not defined $net) {
		$net=new IO::Socket::INET(Proto => 'tcp',PeerAddr => $server,PeerPort => $port,Timeout => 5);
		if (not defined $net) {
			$retries++;
			if ($retries==10) {
				print "\nIs the server up?\n";
			}
			sleep 10;
			print '.';
		}
	}
}

#log in
print "\nConnected.  Logging in...\n";
my $loggedin=0;
#we expect to be logged in within 15 seconds
$SIG{ALRM}=sub{_die "Login timeout."};
alarm 15;
#send everything in one chunk
print $net "$username\n$password\nset prompt JobyX%\nwho\nset open 0\n";
print $net "iset nowrap 1\nset wordwrap 0\nset width 999\nset pin 1\n";
$who=1;
&log("OUT> [username/password]\n");
while (<$net>) {
	chomp;
	s/\r//g;
	#oops, bad password
	if (/$Lang::invalid_password/i) {
		_die "Password was rejected.";
	#prompt is here; we're logged in
	} elsif (/^JobyX% /i) {
		$loggedin=1;
		last;
	}
}
#cancel login timeout
alarm 0;
_die "Connection closed." unless $loggedin;
print "Login successful!\n";

our $warnspam=0;
our $spammed=0;
$SIG{__WARN__}=sub {
	chomp(my $w=shift);
	return if $w=~/redefined/i;
	$spammed and return;
	if ($warnspam>=200) {
		&send("xtell $operator Warnings temporarily deactivated.\n");
		$spammed=1;
		return;
	}
	$warnspam+=50;
	&send("xtell $operator Perl warning: $w\n");
};

{
	my $f=0;
	foreach my $i (@levels) {
		if ($i->[0]==100) {
			$i->[1]=$username;
			$f=1;
			last;
		}
	}
	$f or push(@levels,[100,$username]);
}

#nonblocking mode
$net->blocking(0);

foreach my $i (@default_modules) {
	my $sub=sprintf("JX_Mod::%s::ICS_begin",$i);
	exists(&{$sub}) and &{$sub};
}

#begin input loop
while (1) {
	my $in=&getnextline(1);
	$in=~s/^(?:JobyX% )+//i;
	next if ($in eq '');
	&parse($in);
}
#we ran out of input!
_die "Connection closed.";




### HELPER SUBS

sub getnextline {	#overloadable
	my $loop=int(shift);
	#return any waiting lines if we're looping
	$loop and ($#wlines>-1) and return shift(@wlines);
	my $buffer='';
	while (1) {
		my $in;
		read($net,$in,1);
		while ($in eq '') {
			#execute idle subs
			foreach my $i (@modules) {
				$i->[1] and &{$i->[1]};
			}
			#pause to keep from draining the CPU
			select undef,undef,undef,0.1;
			read($net,$in,1);
		}
		if ($in eq "\n") {
			last;
		} elsif ($in ne "\r") {
			$buffer.=$in;
			undef $in;
		}
	}
	#push line to the wait list if not called from the input loop
	$loop or push(@wlines,$buffer);
	return $buffer;
}

sub parse {
	my $line=shift;
	print " IN> $line\n";
	{
		my $cont=1;
		my $i;
		foreach $i (@modules) {
			my $sub="JX_Mod::$i->[0]::ICS_input";
			if (exists &{$sub}) {
				my $res=&{$sub}($line);
				if ($res==1) {
					$cont=0;
				} elsif ($res==2) {
					$cont=-1;
					last;
				}
			}
		}
		if ($cont!=1) {
			&log(sprintf("BLK> %s - %s",$i->[0],
			($cont==-1)?'allmods':'cont'));
			return;
		}
	}
	return if ($line=~/^notacommand/);
	&log(" IN> $line\n");
	study $line;
	#server is reporting a connection change
	if ($line=~/^\[([^ ]+) (.*)has (dis)?connected.\]$/) {
		#get items
		my($handle,$admin,$dis)=($1,$2,($3 || ''));
		#turn on $in if the user connected
		my $in=not ($dis eq 'dis');
		#set $dis to 'dis' if the user disconnected, else clear
		#this is just a precaution in case ($dis eq $something_weird)
		$dis=($in?'':'dis');
		#declare variables
		my($guest,$ip)=(0,'');
		#if we have access to IP/guest...
		if ($admin=~/^\(([RU]): (.*)\)/) {
			#user is guest?
			$guest=$1 eq 'U';
			#user's IP
			$ip=$2;
		}
		#user has arrived
		if ($in) {
			#get the player's email address
			my @userstat;
			if ($needemail or ($needguest and not $who)) {
				@userstat=&getpi($handle,1);
			}
			my $dl=10;
			   if (lc($operator) eq lc($handle)) { $dl=99  }
			elsif (lc($username) eq lc($handle)) { $dl=100 }
			elsif ($guest)			     { $dl=1   }
			#get user's data file
			my @userf=&playerfile($handle,$dl);
			#set user parray
			$ip=$ip||$userstat[7];
			my $user=[
				$handle,	#handle
				$guest,		#$guest if user is a guest
				$userstat[6],	#email
				$ip || $userstat[7],	#ip
				@userf,		#userlevel, vars, misc
			];
			$user->[6]->{'rch'}=$handle;
			#append to the global user array
			push(@users,$user);
		#user has departed
		} else {
			my $player;
			#remove user from the array
			for (my $i=0 ; $i<=$#users ; $i++) {
				my $j=$users[$i];
				if (lc($j->[0]) eq lc($handle)) {
					$player=splice(@users,$i,1);
					last;
				}
			}
			defined($player) and not $player->[1]
				and &saveparray($player);
			unlink sprintf("data/%s.more",lc($player->[0]));
		}
		#call ICS_(dis_?)?connect hooks
		foreach my $i (@modules) {
			my $sub=sprintf("JX_Mod::%s::ICS_%sconnect",
			$i->[0],$dis);
			my @params=($handle);
			if ($dis ne 'dis') {
				push(@params,$guest,$ip);
			}
			if (exists &{$sub}) {&{$sub}(@params)}
			$sub=sprintf("JX_Mod::%s::ICS_dis_connect",
			$i->[0]);
			if (exists &{$sub}) {&{$sub}($handle,$in)}
		}
	#someone is executing a command
	} elsif ($line=~/^([^( ]+)([^ ]*) $Lang::tell (.+)$/) {
		my($handle,$flags,$message)=($1,$2,$3);
		#keep admins from getting full access by posing JobyX to send
		#tells to itself
		(lc($handle) ne lc($username)) or return;
		my @ui=&userinfo($handle);
		$ui[0] and $ui[1]->[6]->{'used'}=1;
		#remove leading and trailing spaces from message
		$message=~s/^ +//;
		$message=~s/ +$//;
		my $stop=0;
		foreach my $i (@modules) {
			my $sub="JX_Mod::$i->[0]::ICS_tell";
			if (defined &{$sub}) {
				my $res=&{$sub}($handle,$flags,$message);
				if ($res==1) {
					$stop=1;
				} elsif ($res==2) {
					$stop=1;
					last;
				}
			}
		}
		$stop and return;
		$last_flags=$flags || '';
		#this is a command like "+censor"
		if ($message=~/^([^a-zA-Z])(?: ?(.*))?$/) {
			&execcommand($handle,$1,$2);
		#regular command
		} elsif ($message=~/^([^ ]+)(?: (.*))?$/) {
			&execcommand($handle,$1,$2);
		#this should never happen, and never has in my experience
		} else {
			&send("tell $handle I couldn't parse that tell.  ".
			"Please notify the programmer.\n");
		}
	#this is a 'who' line
	} elsif ($who and ($line=~/^ *\d+ $Lang::who_end/i)) {
		while (my $i=shift(@whol)) {
			&parse(sprintf("[%s (%s: ) has connected.]",
			$i->[1],($i->[0] eq '++++')?'U':'R'));
		}
		$who=0;
	} elsif ($who and ($line=~/^ *[0-9+-]{1,4}.[^ (]+/)) {
		&process_who($line);
	}
}

sub process_who {
	my $line=shift;
	while ($line=~/^ *([0-9+-]{1,4}).([^ (]+)[^ ]*(.*)$/) {
		my($rat,$hand)=($1,$2);
		$line=$3;
		push(@whol,[$rat,$hand]);
	}
}

sub getpi {
	my($handle,$expect_complete)=@_;
	&log("GPI> Getting player info for $handle...\n");
	print("GPI> Getting player info for $handle...\n");
	my($HANDLE,$ONLINE,$GUEST,$FLAGS,$EMAIL,$IP,%RATINGS,$rate,$hdr,$skip);
	$GUEST=0;
	$ONLINE=0;
	$skip=0;
	&send(sprintf("$finger_command\n",$handle));
	&send("notacommand\n");
	FLOOP: while (1) {
		my $in=&getnextline(0);
		$in=~s/^(?:JobyX% )+// unless ($skip);
		study $in;
		my $else;
		if ($in=~/^ *$Lang::finger_header +([^(: ]+)([^: ]*)(.*)$/i) {
			($HANDLE,$FLAGS)=($1,$2);
			$expect_complete and (lc($HANDLE) ne lc($handle))
			and return(0,$handle);
			my $extra=$3;
			$ONLINE=($extra=~/$Lang::finger_online/i);
			$hdr=$skip=1;
		} elsif ($in=~/^$Lang::finger_online/i) {	#FICS hack
			$ONLINE=1;
		} elsif ($in=~/^ *$Lang::finger_rating/) {
			$rate=1;
		} elsif ($rate and ($in=~/^ *([^0-9]+?) +(\d+)/)) {
			$RATINGS{lc($1)}=int($2);
		} elsif ($in=~/^ *$Lang::finger_email *: *(.*?) *$/i) {
			$EMAIL=$1;
		} elsif ($in=~/^ *$Lang::finger_ip *: *(.*?) *$/i) {
			$IP=$1;
		} elsif ($hdr and ($in=~/$Lang::finger_guest/i)) {
			$GUEST=1;
		} elsif ($in=~/^ *$/) {
			$rate=$hdr=0;
		} elsif ($skip and ($in=~/^JobyX% /)) {
			last;
		} else {
			$else=1;
		}
		$skip and pop(@wlines);
		if ($else) {
			foreach my $i (@Lang::finger_fail) {
				($in=~/$i/i) and last FLOOP;
			}
		}
	}
	$GUEST=($GUEST or $FLAGS=~/\(U\)/i);
	if (not defined $HANDLE) {
		return(0,$handle);
	}
	return(1,$HANDLE,$ONLINE,$GUEST,$FLAGS,\%RATINGS,$EMAIL,$IP);
}

sub inlist {
	my($slist,$item,$owner)=@_;
	my $list;
	foreach my $i (@lists) {
		if (lc($slist) eq lc($i->[0])) {
			$list=$i;
			last;
		}
	}
	defined $list or return 0;
	$owner=$owner || $global_list;
	my $people=$lists{lc($slist)} or return 0;
	my $person=$people->{lc($owner)} or return 0;
	foreach my $i (@{$person}) {
		if (lc($item) eq lc($i)) { return 1 }
	}
	return 0;
}

sub getlist {
	my($slist,$owner)=@_;
	foreach my $i (@lists) {
		if (lc($slist) eq lc($i->[0])) {
			$list=$i;
			last;
		}
	}
	defined $list or return 0;
	$owner=$owner || $global_list;
	my($people,$person);
	if (not defined($people=$lists{lc($slist)})) {
		$people=$lists{lc($slist)}={};
	}
	if (not defined($person=$people->{lc($owner)})) {
		$person=$people->{lc($owner)}=[@{$list->[11]}];
	}
	return $person;
}

sub playerfile {
	my $handle=lc(shift);
	my $fn="data/players/$handle";
	my %res=(
		userlevel	=> shift || 10,
		comments	=> 0,
	);
	if (-e $fn) {
		open(PFILE,$fn);
		while (<PFILE>) {
			if (/^(.+?)=(.+)$/) {
				$res{lc($1)}=$2;
			}
		}
		close PFILE;
	}
	my %vars;
	foreach my $i (keys %res) {
		if ($i=~/^var-(.+)$/) {
			$vars{lc($1)}=delete $res{$i};
		}
	}
	return (
		delete $res{'userlevel'},
		\%vars,
		\%res,
	);
}

sub levelname {
	my $level=shift;
	($level=~/[0-9]/) or return undef;
	foreach my $i (@levels) {
		($i->[0]==$level) and return $i->[1];
	}
	return undef;
}

sub fulllevelname {
	my $level=shift;
	my $name=&::levelname($level);
	if (defined $name) {
		$level=sprintf("%s(%d)",$name,$level);
	}
	return $level;
}

sub userinfo {
	my $handle=lc(shift);
	foreach my $i (@users) {
		if (lc($i->[0]) eq $handle) {
			return (1,$i,1);
		}
	}
	if (-e "data/players/$handle") {
		my @res=&playerfile($handle);
		$handle=$res[2]->{'rch'} || $handle;
		return (1,[$handle,0,'','',@res],0);
	}
	my @res=&getpi($handle);
	if ($res[0]) {
		return (1,[$handle,@res[3,6,7],10,{},{'comments' => 0}]);
	}
	return (0,undef);
}

sub saveall {
	my $fails=0;
	&savelists and $fails+=1;
	&saveavars or $fails+=2;
	return $fails;
}

sub loadall {
	my $fails=0;
	&loadlists or $fails+=1;
	&loadavars or $fails+=2;
	return $fails;
}

sub saveavars {
	open(AVARS,">data/avariables") or return 0;
	foreach my $i (keys %avariables) {
		next if ($i=~/=/);
		printf AVARS "%s=%s\n",$i,$avariables{$i};
	}
	close AVARS;
	return 1;
}

sub loadavars {
	open(AVARS,"<data/avariables") or return 1;
	while(<AVARS>) {
		chomp;
		if (/^([^=]+)=(.+)$/) {
			$avariables{$1}=$2;
		}
	}
	close AVARS;
	return 1;
}

sub savelists {
	my $fails=0;
	foreach my $i (keys %lists) {
		-d "data/lists" or mkdir "data/lists" or return -1;
		my $path="data/lists/$i";
		-d $path or mkdir $path or return -2;
		unlink glob "$path/*";
		foreach my $s (keys %{$lists{$i}}) {
			if (not open(SAVEL,">$path/$s")) {
				$fails++;
				&log("Couldn't save list $i-$s!\n");
				print "Couldn't save list $i-$s!\n";
				next;
			}
			print SAVEL join("\n",@{$lists{$i}->{$s}});
			print SAVEL "\n";
			close SAVEL;
		}
	}
	return $fails;
}

sub loadlists {
	foreach my $i (glob('data/lists/*')) {
		-d $i or next;
		my $listname=substr($i,11);
		my $list={};
		foreach my $j (glob("$i/*")) {
			open(LOADL,$j) or next;
			my @items=<LOADL>;
			chomp(@items);
			$items[-1] eq '' and pop(@items);
			my $itemname=substr($j,length($i)+1);
			$list->{$itemname}=[@items];
		}
		$lists{$listname}=$list;
	}
	return 1;
}

sub saveparray {
	my $parray=shift;
	return if ($parray->[1]);
	return unless ($parray->[6]->{'used'});
	my $handle=$parray->[0];
	my %vars=%{$parray->[5]};
	my %misc=%{$parray->[6]};
	foreach my $i (keys %vars) {
		$misc{"var-$i"}=$vars{$i};
	}
	&saveplayerfile($handle,
		'userlevel' => $parray->[4],
		%misc,
	);
}

sub saveplayerfile {
	my $handle=lc(shift);
	#get filename
	my $fn="data/players/$handle";
	#get params
	my %data=@_;
	#open the file
	open(PFILE,">$fn") or return 0;
	foreach my $i (sort keys %data) {
		($i=~/=/) and next;
		print PFILE "$i=$data{$i}\n";
	}
	close PFILE;
	return 1;
}

sub getvar {
	my($handle,$var)=@_;
	my $vdef;
	if (ref($var)) {
		$vdef=$var;
	} else {
		foreach my $i (@variables) {
			if (lc($i->[0]) eq lc($var)) {
				$vdef=$i;
				last;
			}
		}
	}
	defined($vdef) or return undef;
	$var=lc($vdef->[0]);
	my @res=&userinfo($handle);
	$res[0] or return undef;
	my $p=$res[1];
	foreach my $i (keys %{$p->[5]}) {
		if (lc($i) eq $var) {
			return $p->[5]->{$var};
		}
	}
	return $vdef->[2];
}

sub getavar {
	my $avar=shift;
	if (not ref($avar)) {
		$avar=lc($avar);
		foreach my $i (@avariables) {
			if (lc($i->[0]) eq $avar) {
				$avar=$i;
				last;
			}
		}
	}
	ref($avar) or return undef;
	return $avariables{lc($avar->[0])} || $avar->[4];
}

sub execcommand {
	my($handle,$command,$params)=@_;
	#get the possible commands
	my @res=&closestmatch(\@commands,$command,$handle,3);
	#no match was found
	if ($res[0]==2) {
		&error($handle,"$command: $Lang::command_not_found");
		return;
	#it is ambiguous
	} elsif ($res[0]==0) {
		shift @res;
		@res=sort {lc($a) cmp lc($b)} map {$_->[0]} @res;
		&error($handle,$Lang::command_ambig.join(', ',@res));
		return;
	}
	#ah, just one match
	my $com=$res[1];
	#make sure the user has permissions to run this command
	if (not &qualified($handle,$com->[2])) {
		&error($handle,sprintf($Lang::command_notqual,$com->[0]));
		return;
	}
	#parse out "" '' params
	my @ps=&parseparams($params,$com->[4]);
	#set die trap
	local $SIG{__DIE__}=sub {
		&error($handle,sprintf($Lang::command_fatal,$com->[0]));
		chomp(my $e=shift);
		&send(sprintf(
		"message %s Command handler \"%s\" of module \"%s\"".
		" die()d: %s\n",$operator,$com->[0],$com->[-1],$e));
		&send("tell $operator I may be in an inconsistent state due".
		" to the last die()!\n");
		&log("FTL> $e\n");
	};
	#execute the command
	&JX_Mod::CORE::usage($handle,$com->[0]) if ((
		eval('&{$com->[1]}($handle,@ps)') || 0
		)==255);
}

sub qualified {	#overloadable
	my($handle,$string)=@_;
	#we need a reference to the user's file...
	#look for it if we don't have a reference
	#the if() is because we recurse to this sub later _with_ a reference
	if (not ref($handle)) {
		foreach my $i (@users) {
			if (lc($handle) eq lc($i->[0])) {
				$handle=$i;
				last;
			}
		}
		#couldn't find a reference... silently fail
		ref($handle) or return 0;
	}
	#remove spaces
	$string=~s/ //g;
	#try to find logical OR "," or AND "+"
	my @pms=split(/([,+])/,$string,2);
	#yes, we have an OR or AND
	if ($#pms==2) {
		#recurse to parse each "conjunct"
		if ($pms[1] eq ',') {
			return (&qualified($pms[0]) or &qualified($pws[2]));
		}
		return (&qualified($pms[0]) and &qualified($pws[2]));
	}
	#is the level a numerical userlevel?
	if ($pms[0]=~/^\d+$/) {
		#return true when the user's level >= the required level
		return ($handle->[4] >= $pms[0]);
	#is the level a list assertion?
	} elsif ($pms[0]=~/^l([yn]):(.+)$/) {
		my($bit,$list)=($1,$2);
		#turn $bit on when we _don't_ want the user in the list
		$bit=($bit eq 'n');
		#note the clever xor'ing of $bit to produce the correct return
		#value without a cumbersome if...then
		return (&inlist($list,$handle->[0]) xor $bit);
	}
	#this string is not a valid userlevel... fail
	&send("message $operator Warning: invalid userlevel \"$pms[0]\" ".
	"parsed.  Check your modules!\n");
	return 0;
}

sub closestmatch {
	my($type,$name,$handle,$see)=@_;
	my(@name_part,%seen);
	#lowercase the name
	$name=lc($name);
	foreach my $i (@{$type}) {
		#skip if we've seen an item with the same name
		$seen{lc($i->[0])} and next;
		#also skip if the user can't see it
		defined($see) and (&qualified($handle,$i->[$see]) or next);
		#note that we've seen this item
		$seen{lc($i->[0])}=1;
		#this is an exact match!
		if (lc($i->[0]) eq $name) {
			return (1,$i);
		#partial match--append to array and keep looking
		} elsif (lc(substr($i->[0],0,length($name))) eq $name) {
			push(@name_part,$i);
		}
	}
	#good, there's only one item that starts with $name
	if ($#name_part==0) {
		return (1,$name_part[0]);
	#no matches... item not found
	} elsif ($#name_part==-1) {
		return (2);
	#we have more than one possible match
	} else {
		return (0,@name_part);
	}
}

sub parseparams {
	my($plist,$limit)=@_;
	my @ps;
	my $count=0;
	#try to avoid a perl warning
	defined($plist) or $plist='';
	#continue as long as there's more to parse
	while ($plist !~ /^ *$/) {
		#increase the number of parameters seen
		$count++;
		#remove leading spaces
		$plist=~s/^ +//;
		#if it's in "" or '' quotes...
		if ($plist=~/^(["'])(.*?)\1(?: (.*)|)$/) {
			my($item,$rest)=($2,$3);
			#the limit has been reached, and there's no more
			if ($count==$limit and $rest=~/^ *$/) {
				push(@ps,$item);
				last;
			#oops, there is more--i guess we'll append the rest of
			#the params as one string
			} elsif ($count==$limit) {
				push(@ps,$plist);
				last;
			}
			#append the item
			push(@ps,$item);
			#change $plist to the unparsed section
			$plist=defined($rest)?$rest:'';
		} else {
			#the limit has been reached, append the remainder
			if ($count==$limit) {
				push(@ps,$plist);
				last;
			}
			#split at spaces
			my($item,$rest)=split(' ',$plist,2);
			#append the item
			push(@ps,$item);
			#change $plist to the unparsed section
			$plist=defined($rest)?$rest:'';
		}
	}
	while ($#ps+1<$limit) { push(@ps,'') }
	#return the parameters
	return @ps;
}

sub _die ($) {
	my $err=shift;
	chomp($err);
	print STDERR "$0: $err\n";
	&log("DIE> $err\n");
	exit;
}

sub error {	#overloadable
	my($handle,$mes)=@_;
	chomp($mes);
	if ($handle eq '*DEBUG*') {
		print "*DEBUG* \&::error called: $mes\n";
		return;
	}
	&send("qtell $handle $Lang::generic_error $mes\n");
}

sub send {	#overloadable
	defined($net) or return;
	print $net @_;
	&log("OUT> ",@_);
	print "OUT> ",@_;
}

sub qtell {	#overloadable
	my $handle=lc(shift);
	unlink "data/$handle.more";
	my $height=&getvar($handle,'height')-2;
	my $linecount=0;
	my @qt=(&newline_to_qtell(shift));
	while ($#_ > -1) {
		my $i=shift(@_);
		my $s=&newline_to_qtell($i);
		if (length(join("\\n",@qt).$s) > $max_communication-50) {
			&send("qtell $handle ".join("\\n",@qt)."\n");
			$linecount+=$#qt+1;
			@qt=();
		}
		push(@qt,$s);
		if ($#qt+1+$linecount>=$height) {
			push(@qt,'',sprintf($Lang::more_pages,
			&getavar('TellMe'))) if ($#_ > -1);
			last;
		}
	}
	if ($#_ > -1) {
		open(MORE,">data/$handle.more") and print MORE join("\n",@_);
		close MORE;
	} else {
		unlink "data/$handle.more";
	}
	&send("qtell $handle ".join("\\n",@qt)."\n");
}

sub newline_to_qtell {
	my $i=shift;
	$i=~s/\\n/\\\\n/g;
	$i=~s/\n/\\n/g;
	return $i;
}

sub log {	#overloadable
	$logopen and print LOG @_;
}

sub insert_mod_data {
	my($dest_array,$src_array,$num_of_elements,$mod_name,$vfy)=@_;
	if ($vfy) {
		foreach my $i (@{$src_array}) {
			if ($#{$i} != ($num_of_elements-1)) {return 1,$i->[0]}
		}
	} else {
		foreach my $i (@{$src_array}) {
			push(@{$i},$mod_name);
			push(@{$dest_array},$i);
		}
	}
	return 0;
}

sub columns {	#overloadable
	my @items=@_;
	my(@tr,@res);
	my $len=0;
	foreach my $i (@items) {
		$len=(length($i)+2 > $len) ? length($i)+2 : $len;
	}
	while ($#items>=0) {
		push(@tr,shift(@items));
		if ($len * ($#tr+1) > 79) {
			if ($#tr>0) { unshift(@items,pop(@tr)) }
			$t=&space_out($len,@tr);
			push(@res,$t);
			@tr=();
		}
	}
	if ($#tr>-1) { push(@res,&space_out($len,@tr)) }
	return @res;
}

sub table {	#overloadable
	my($padding,@items)=@_;
	my $borders=1;
	if ($padding==-1) {
		$borders=0;
		$padding=0;
	} else {
		$padding=int($padding!=0)*2;
	}
	#find out how many columns we've got and make sure there is at least
	#one non-black row.
	my $columns=0;
	my $seencontent=0;
	foreach my $i (@items) {
		my $count=0;
		foreach my $j (@{$i}) {
			defined($j) and $count++;
		}
		($count>0) or next;
		$seencontent=1;
		($count>$columns) and $columns=$count;
	}
	$seencontent or carp 'no content found, can\'t draw up table';
	($columns>0) or carp 'no columns found, can\'t draw up table';
	#determine how wide each column is.
	my(@cwidths,@wide);
	for (1..$columns) { push(@cwidths,0) }
	for (my $h=0;$h<=$#items;$h++) {
		my $i=$items[$h];
		my $count=$#{$i};
		($count>-1) or next;
		for ($count..$columns-1) { push(@{$i},'') }
		for (my $j=0;$j<$columns;$j++) {
			if (not defined $i->[$j]) {
				($j>0) or carp 'can\'t continue column in the'.
				' first column';
				push(@wide,[$h,$j]);
				next;
			} elsif ($j+1<$columns and not defined $i->[$j+1]) {
				next;
			}
			my $len=length($i->[$j])+$padding;
			(substr($i->[$j],0,1) eq "\0") and $len--;
			($len>$cwidths[$j]) and $cwidths[$j]=$len;
		}
	}
	while ($#wide>-1) {
		my $i=shift(@wide);
		my($row,$col)=(@{$i});
		my $text=$items[$row]->[$col-1];
		my $len=length($text)+$padding;
		my @cols=($col-1,$col);
		while ($#wide>-1) {
			my $j=shift(@wide);
			if ($j->[0]==$row and $j->[1]==$col+1) {
				push(@cols,$j->[1]);
				$col++;
			} else {
				unshift(@wide,$j);
				last;
			}
		}
		my $tw=0;
		foreach my $j (@cols) {
			$tw+=$cwidths[$j]+1;
		}
		$tw--;
		if ($len>$tw) {
			my $last=$cols[-1];
			my $lv=$cwidths[$last];
			$len-=$tw-$lv;
			$cwidths[$last]=$len;
		}
	}
	my $tablew=1;
	foreach my $i (@cwidths) {
		$tablew+=$i+1;
	}
	my @res;
	@res=('+'.('-'x($tablew-2)).'+') if ($borders);
	for (my $ix=0;$ix<=$#items;$ix++) {
		my $i=$items[$ix];
		my $item;
		$item='|' if ($borders);
		if ($#{$i}==-1) {
			#blank row
			if (not $borders) {
				push(@res,'');
				next;
			}
			for (my $h=0;$h<=$#cwidths;$h++) {
				my $bar='|';
				if ($smartbar and $h<$#cwidths) {
					if ($ix>0 and
					not defined $items[$ix-1]->[$h+1]) {
						$bar='-';
					} elsif ($ix<$#items and
					not defined $items[$ix+1]->[$h+1]) {
						$bar='-';
					}
				}
				my $j=$cwidths[$h];
				$item.=('-'x$j).$bar;
			}
		} else {
			for (my $j=0;$j<$columns;$j++) {
				my $span=1;
				my $wide=$cwidths[$j];
				my $sp=$j+1;
				while ($sp<$columns and not defined $i->[$sp])
				{
					$span++;
					$sp++;
					$wide+=$cwidths[$sp-1]+1;
				}
				$padding and $wide-=2;
				my $align='-';
				if (substr($i->[$j],0,1) eq "\0") {
					$align='';
					$i->[$j]=substr($i->[$j],1);
				}
				$item.=sprintf("%s%${align}*s%s%s",
				($padding?' ':''),$wide,$i->[$j],
				($padding?' ':''),($borders?'|':''));
				$j+=$span-1;
			}
		}
		$item=~s/ +$// unless ($borders);
		push(@res,$item);
	}
	push(@res,$res[0]) if ($borders);
	return @res;
}

sub space_out {
	my($len,@items)=@_;
	my $t=sprintf("%-${len}s"x($#items+1),@items);
	$t=~s/ +$//;
	return $t;
}

### COMMANDS

sub ipc {
	my($mod,@args)=@_;
	&::ismoduleloaded($mod) or return undef;
	my $sub=sprintf("JX_Mod::%s::ICS_ipc",$mod);
	defined(&{$sub}) or return undef;
	return &{$sub}(@args);
}

sub ismoduleloaded {
	my $mod=shift;
	foreach my $i (@modules) {
		($i->[0] eq $mod) and return 1;
	}
	return 0;
}

sub reloadmod {
	my($handle,$mod)=@_;
	&rmmod($handle,$mod,1);
	&insmod($handle,$mod);
}

sub insmod {
	my($handle,$mod)=@_;
	return 255 if ($mod=~/^ *$/);
	foreach my $j (@modules) {
		my $i=$j->[0];
		if ($i eq $mod) {
			&error($handle,"Module $mod already loaded.");
			return;
		}
		my $sn=sprintf('JX_Mod::%s::ICS_pre_module_load',$i);
		if (exists &{$sn}) {
			my @res=&{$sn}($mod);
			if ($res[0]) {
				&error($handle,"Module $i refused the load".
				" of module $mod: $res[1]");
				return;
			}
		}
	}
	my $ins_err=0;
	{
		if (not -e "JX_Mod/$mod.pm") {
			$ins_err="Module file not found";
			last;
		}
		local $SIG{__DIE__} = sub {$ins_err=$!};
		do "JX_Mod/$mod.pm" or $ins_err="Didn't return a true value";
	}
	if ($ins_err) {
		&error($handle,"Couldn't insert module $mod: $ins_err.");
		return;
	}
	my $init=sprintf('JX_Mod::%s::ICS_init',$mod);
	if (not exists(&{$init})) {
		&error($handle,"Module $mod does not define ICS_init.");
		return;
	}
	my @stuff=&$init($username,$version,$handle);
	if ($#stuff==1) {
		&error($handle,"Module $mod refused to load: $stuff[1]");
		return;
	} elsif ($#stuff!=4) {
		&error($handle,"Incorrect return data from module ${mod}'s".
		" ICS_init.");
		return;
	}

	my @lists=(
		[\@commands,6],
		[\@lists,12],
		[\@variables,6],
		[\@avariables,6],
	);
	my @sources=@stuff[0..3];
	foreach my $list (@lists) {
		my $src=shift(@sources);
		my @res=&insert_mod_data($list->[0],$src,$list->[1],$mod,1);
		if (shift @res) {
			&error($handle,"Invalid return data from module ".
			"${mod}'s ICS_init.  [$res[0]]");
			return;
		}
	}
	foreach my $list (@lists) {
		my $src=shift(@stuff);
		&insert_mod_data($list->[0],$src,$list->[1],$mod,0);
	}
	push(@modules,[$mod,shift(@stuff)]);
	my $sub=sprintf('JX_Mod::%s::ICS_begin',$mod);
	defined(&{$sub}) and &{$sub};
	$handle eq '*DEBUG*' and return 1;
	my @res=("Module $mod successfully inserted.");
	my %seen;
	@lists=(
		[\@commands,'command'],
		[\@lists,'list'],
		[\@variables,'variable'],
		[\@avariables,'avariable'],
	);
	foreach my $i (@lists) {
		%seen=();
		foreach my $j (@{$i->[0]}) {
			my $emod;
			if ($emod=$seen{lc($j->[0])}) {
				next if ($emod ne $mod and $j->[-1] ne $mod);
				push(@res,sprintf(
				"  Modules \"%s\" and \"%s\" both define the %s \"%s\"!",
				$emod,$j->[-1],$i->[1],$j->[0]));
			} else {
				$seen{lc($j->[0])}=$j->[-1];
			}
		}
	}
	push(@res,"$#res conflicts.");
	&qtell($handle,@res);
	foreach my $i (@modules) {
		my $sub=sprintf("JX_Mod::%s::ICS_module_loaded",$i->[0]);
		exists(&{$sub}) and &{$sub}($mod);
	}
	&JX_Mod::CORE::helpindex($handle);
}

sub rmmod {
	my($handle,$mod,$rel)=@_;
	return 255 if ($mod=~/^ *$/);
	my $f=-1;
	for (my $i=0;$i<=$#modules;$i++) {
		if ($modules[$i]->[0] eq $mod) {
			$f=$i;
			last;
		}
	}
	if ($f==-1) {
		&error($handle,"Module $mod is not loaded.");
		return;
	} else {
		foreach my $i (@modules) {
			my $sub=sprintf("JX_Mod::%s::ICS_pre_module_unload",
			$i->[0]);
			if (exists &{$sub}) {
				my @res=&{$sub}($mod);
				if ($res[0]==1) {
					&error($handle,sprintf(
					"Module %s refused the removal of ".
					"module %s. %s",$i->[0],$mod,
					$res[1] || ''));
				}
			}
		}
		my $sub=sprintf("JX_Mod::%s::ICS_term",$mod);
		my @res=(0);
		if (defined &{$sub}) {
			@res=&{$sub}($handle);
		}
		if ($res[0]) {
			&::error($handle,defined($res[1]) ?
			$res[1] : "Module $mod is not unloadable.");
			return;
		}
	}
	my @lists=(
		\@commands,
		\@lists,
		\@variables,
		\@avariables,
	);
	foreach my $i (@lists) {
		for (my $j=0;$j<=$#{$i};) {
			if ($i->[$j]->[-1] eq $mod) {
				splice(@{$i},$j,1);
			} else {
				$j++;
			}
		}
	}
	splice(@modules,$f,1);
	&send("qtell $handle Module $mod successfully unloaded.\n");
	foreach my $i (@modules) {
		my $sub=sprintf("JX_Mod::%s::ICS_module_unloaded",$i->[0]);
		exists(&{$sub}) and &{$sub}($mod);
	}
	&JX_Mod::CORE::helpindex($handle) unless ($rel);
}

### VERIFICATION SUBS

sub true {1}

sub notnull {$_[2] !~ /^ *$/}

sub boolean {
	my($handle,$var,$setting,$old)=@_;
	$setting=$setting || '';
	$setting=~s/ //g;
	if    ($setting=~/^(?: no?      | 0 | off? )$/ix) {return (2,0)}
	elsif ($setting=~/^(?: y(?:es)? | 1 | on   )$/ix) {return (2,1)}
	elsif ($setting eq '') { return(2,int(not $old)) }
	return 0;
}

sub handle {
	my $handle=$_[2];
	my @res=&getpi($handle,$_[3]);
	if ($res[0]==0) {
		return(0,$Lang::no_handle);
	}
	return(2,$res[1]);
}

sub handle_no_complete {&handle(@_,1)}

sub handle_lower {
	my @res=&handle(@_,$_[3]);
	$res[0] or return(@res);
	my @adder=&userinfo($_[0]);
	$adder[0] or return (0,sprintf($Lang::no_mutiny,$res[1]));
	my @addee=&userinfo($res[1]);
	$addee[0] or return(@res);
	($adder[1]->[4] > $addee[1]->[4]) and return(@res);
	return(0,sprintf($Lang::no_mutiny,$res[1]));
}

sub handle_lower_no_complete {&handle_lower(@_,1)}

sub list_comment {
	my($handle,$list,$item,$added)=@_;
	&JX_Mod::CORE::addcomment($handle,$item,sprintf(
	"%s the %s list.",$added?'Added to':'Removed from',$list));
	return 1;
}
