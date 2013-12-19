package JX_Mod::CORE;

use strict 'vars';
use Text::Wrap 2001.0929 ();
$Text::Wrap::unexpand=0;
my $last_read=0;
my $revival=0;

#JobyX core commands, lists, variables, and avariables.

#A significant amount hacking into JobyX's namespace is done here.  Be careful
#if you want to do the same!

#By hacking the namespace, you can override CORE module commands, modify
#command/list/variable/avariable definitions on the fly, and a lot more.
#Eventually, there will be some docs on what to do and what not to do.

#Note that the "insmod" and "rmmod" commands are the only commands whose
#handlers do _not_ reside in this file.  Otherwise, how would we insert this
#one?

our(@helpindex,@usageindex);

sub ICS_init {
	$Lang::CORE::loaded or return(0,
	'CORE language pack was not found.  Please insert it into '.
	'jobyxconfig.pm');
	return(
	[ #commands
	#name           sub          exec see args vis
	['+',           \&addlist,   1,   1,  2,   0  ],
	['-',           \&sublist,   1,   1,  2,   0  ],
	['=',           \&showlist,  1,   1,  1,   0  ],
	['?',           \&help,      1,   1,  1,   0  ],
	['f',           \&finger,    1,   1,  1,   0  ],
	['ac',          \&addcomment,20,  20, 2,   0  ],
	['sc',          \&showcomment,20, 20, 1,   0  ],
	[$Lang::CORE::List_TU_Alias,
			\&listtus,   1,   1,  1,   0  ],

	['AddList',     \&addlist,   1,   1,  2,   1  ],
	['AddComment',  \&addcomment,20,  20, 2,   1  ],
	['ASet',        \&aset,      50,  50, 2,   1  ],
	['AVariables',  \&avariables,50,  50, 1,   1  ],
	['DeleteComment',\&delcomment,95, 20, 2,   1  ],
	['EvalCode',    \&eval,      99,  99, 1,   1  ],
	['Finger',      \&finger,    1,   1,  1,   1  ],
	['Help',        \&help,      1,   1,  1,   1  ],
	['insmod',      \&::insmod,  90,  90, 1,   1  ],
	['ListCommands',\&commands,  1,   1,  1,   1  ],
	[$Lang::CORE::List_TU_Command,
	                \&listtus,   1,   1,  1,   1  ],
	['LoadAll',     \&loadall,   99,  99, 1,   1  ],
	['lsmod',       \&lsmod,     90,  90, 1,   1  ],
	['MessageCOO',  \&mcoo,      10,  1,  1,   1  ],
	['MessageProgrammer',\&mprg, 10,  1,  1,   1  ],
	['modinfo',     \&modinfo,   90,  90, 1,   1  ],
	['More',        \&more,      1,   1,  1,   0  ],
	['Next',        \&more,      1,   1,  1,   1  ],
	['RebuildIndex',\&helpindex, 99,  99, 1,   1  ],
	['reloadmod',   \&::reloadmod,90, 90, 1,   1  ],
	['Restart',     \&restart,   99,  99, 1,   1  ],
	['ResetNotes',  \&resetnotes,99,  99, 1,   1  ],
	['rmmod',       \&::rmmod,   95,  90, 1,   1  ],
	['SaveAll',     \&saveall,   90,  90, 1,   1  ],
	['Send',        \&pose,      90,  90, 1,   1  ],
	['Set',         \&set,       1,   1,  2,   1  ],
	['SetLevel',    \&setlevel,  60,  60, 2,   1  ],
	['ShowCommandInfo',\&commandinfo,1,1, 1,   1  ],
	['ShowComment', \&showcomment,20, 20, 1,   1  ],
	['ShowList',    \&showlist,  1,   1,  1,   1  ],
	['ShowListInfo',\&listinfo,  1,   1,  1,   1  ],
	['ShowVariableInfo',\&varinfo,1,  1,  1,   1  ],
	['sk',          \&commands,  1,   1,  1,   0  ],
	['Shutdown',    \&softquit,  99,  99, 1,   1  ],
	['SubList',     \&sublist,   1,   1,  2,   1  ],
	['ToggleList',  \&togglelist,1,   1,  2,   1  ],
	['Usage',       \&usage,     1,   1,  1,   1  ],
	['Variables',   \&variables, 1,   1,  1,   1  ],

	],[ #lists
	#name see view add sub pri verify     addsub  subsub  max type def
	[$Lang::CORE::TU_list,
	      1,  1,   50, 60, 0,  \&::handle,\&newtu,\&remtu,0,  'names',[$::operator]],

	],[ #vars
	#name have def vis verify      set
	['Height',1,24,0,  \&var_height,undef],

	],[ #avars
	#name     see set verify      def                setsub
	['COO',   90, 99, \&::handle,$::operator,\&mkcoo],
	['Programmer',90,100,\&::handle,$::operator,\&mkcoo],
	['TellMe',90, 90, \&::notnull,"tell $::username",undef],
	],
	\&detect_drop
	);
}

sub mkcoo {
	my($handle,$type,$who,$last)=@_;
	&addcomment($handle,$who,"Made $type.");
	&addcomment($handle,$last,"Relieved as $type.");
	&setlevel($handle,$who,($type eq 'COO')?96:99,1);
}

sub detect_drop {
	if ($::warnspam>0) {
		if (--$::warnspam==0) {
			$::spammed and &::send(
			"xtell $::operator Warnings reactivated.\n");
			$::spammed=0;
		}
	}
	if ($last_read+60<time and not $revival) {
		print " DD> Checking connection...\n";
		&::send("notacommand\n");
		$revival=1;
	} elsif ($last_read+90<time and $revival) {
		$revival=0;
		undef $::net;
		print  " DD> Connection was dropped.\n";
		&::log(" DD> Connection was dropped.\n");
		&restart($::username);
	}
}

sub ICS_term { 1 }

sub ICS_lsmod { $::version }

sub ICS_module_info { "Distributed as part of JobyX $::version." }

sub ICS_begin {
	$last_read=time;
	$revival=0;
	&helpindex('*DEBUG*');
	&resetnotes;
}

sub ICS_input {
	$last_read=time;
	if ($revival) {
		print " DD> It's still up!\n";
		$revival=0;
	}
	my $line=shift;
	foreach my $i (@Lang::CORE::connection_fail) {
		if ($line=~/$i/i) {
			undef $::net;
			print  " DD> Connection was closed. ($i)\n";
			&::log(" DD> Connection was closed. ($i)\n");
			&restart($::username,undef,undef,1);
		}
	}
	#it is oh so important to return 0 here
	return 0;
}

sub ICS_finger {
	my($handle,$who,$parray)=@_;
	my $lv=&::fulllevelname($parray->[4]);
	my @res=(sprintf("$Lang::CORE::finger_level%s",$::username,$lv));
	my @ui=&::userinfo($handle);
	if ($ui[0] and $ui[1]->[4]>=20) {
		push(@res,sprintf($Lang::CORE::finger_comments,
		$parray->[6]->{'comments'}));
	}
	return @res;
}

### VARIABLES

sub var_height {
	my $setting=$_[2] || '';
	($setting=~/[^\d]/) and return 0;
	($setting<7 or $setting>9999) and return 0;
	return 1;
}

sub newtu {
	my($handle,undef,$item)=@_;
	my @ui=&::userinfo($item);
	$ui[0] or return 1;
	&::list_comment(@_);
	$ui[1]->[4]<20 and &setlevel($handle,$item,20);
	return 1;
}

sub remtu {
	my($handle,undef,$item)=@_;
	my @ui=&::userinfo($item);
	$ui[0] or return 1;
	&::list_comment(@_);
	$ui[1]->[4]>19 and &setlevel($handle,$item,10);
	return 1;
}

### COMMANDS

sub mprg {
	my($handle,$message)=@_;
	return 255 if ($message=~/^ *$/);
	$message=~s/^ *(.+?) *$/$1/;
	my $prg=&::getavar('Programmer');
	&::send("message $prg MessageProgrammer from $handle: $message\n");
	$message=~s/\\n/[\\ n]/g;
	&::send("qtell $handle The following message was sent to $prg:\\n".
	"$message\n");
}

sub mcoo {
	my($handle,$message)=@_;
	return 255 if ($message=~/^ *$/);
	$message=~s/^ *(.+?) *$/$1/;
	my $coo=&::getavar('COO');
	&::send("message $coo MessageCOO from $handle: $message\n");
	$message=~s/\\n/[\\ n]/g;
	&::send("qtell $handle The following message was sent to $coo:\\n".
	"$message\n");
}

sub listtus {
	my($handle,$on)=@_;
	$on=($on eq '')?'all':$on;
	if ($on=~/^ *(on|all) *$/i) {
		$on=(lc($1) eq 'on')?1:0;
	} else {
		return 255;
	}
	my $items=&::getlist($Lang::CORE::TU_list);
	my @list=(['Manager','Level'],[]);
	foreach my $i (@{$items}) {
		my @ui=&::userinfo($i);
		$ui[0] or next;
		$on and ($ui[2] or next);
		push(@list,[sprintf("%s%s",($ui[2]?'+':'-'),$ui[1]->[0]),
		&::fulllevelname($ui[1]->[4])]);
	}
	if ($#list==1) {
		&::error($handle,$Lang::CORE::no_TUs);
		return;
	}
	&::qtell($handle,$Lang::CORE::TU_list_head,'',
	&::table(1,@list),'',sprintf($Lang::CORE::TU_list_footer,$#list+1));
}

sub addcomment {
	my($handle,$player,$comment)=@_;
	return 255 if ($player=~/^ *$/ or $comment=~/^ *$/);
	my @ui=&::getpi($player);
	if (not $ui[0]) {
		&::error($handle,$Lang::no_handle);
		return;
	}
	$player=$ui[1];
	my $online=$ui[2];
	@ui=&::userinfo($player);
	$ui[0] or return;
	my $fn=sprintf("data/players/%s.comments",lc($player));
	my @lines;
	if (open(COMMENT,$fn)) {
		chomp(@lines=<COMMENT>);
		close COMMENT;
	}
	$comment=sprintf("%s at %s: %s",$handle,scalar localtime,$comment);
	push(@lines,$comment);
	if (not open(COMMENT,">$fn")) {
		&::error($handle,"Couldn't clobber comment file.");
		return;
	}
	print COMMENT join("\n",@lines);
	$ui[1]->[6]->{'comments'}=$#lines+1;
	&::saveparray($ui[1]) unless ($online);
	&::send("qtell $handle Comment added for $player.\n");
}

sub delcomment {
	my($handle,$player,$comment)=@_;
	return 255 if ($player=~/^ *$/ or $comment=~/^ *$/);
	$comment=int($comment);
	return 255 if ($comment<1);
	my @ui=&::getpi($player);
	if (not $ui[0]) {
		&::error($handle,$Lang::no_handle);
		return;
	}
	$player=$ui[1];
	my $online=$ui[2];
	@ui=&::userinfo($player);
	$ui[0] or return;
	my $fn=sprintf("data/players/%s.comments",lc($player));
	my @lines;
	if (open(COMMENT,$fn)) {
		chomp(@lines=<COMMENT>);
		close COMMENT;
	}
	if ($#lines+1<$comment) {
		&::error($handle,$Lang::CORE::no_comment);
		return;
	}
	splice(@lines,$comment-1,1);
	if (not open(COMMENT,">$fn")) {
		&::error($handle,"Couldn't clobber comment file.");
		return;
	}
	print COMMENT join("\n",@lines);
	$ui[1]->[6]->{'comments'}=$#lines+1;
	&::saveparray($ui[1]) unless ($online);
	&::send("qtell $handle Comment $comment for $player deleted.\n");
}

sub showcomment {
	my($handle,$player)=@_;
	return 255 if ($player=~/^ *$/);
	my @ui=&::getpi($player);
	if (not $ui[0]) {
		&::error($handle,$Lang::no_handle);
		return;
	}
	$player=$ui[1];
	my $fn=sprintf("data/players/%s.comments",lc($player));
	if (not open(COMMENT,$fn)) {
		&::error($handle,sprintf($Lang::CORE::no_comments,$player));
		return;
	}
	chomp(my @lines=<COMMENT>);
	if ($#lines==-1 or ($#lines==0 and $lines[0] eq '')) {
		&::error($handle,sprintf($Lang::CORE::no_comments,$player));
		return;
	}
	my $line=0;
	@lines=map { $line++;"$line. $_" } @lines;
	&::qtell($handle,sprintf($Lang::CORE::comments,$player),'',@lines);
}

sub more {
	my $handle=lc(shift);
	if (not open(MORE,"data/$handle.more")) {
		&::error($handle,$Lang::no_more_pages);
		return;
	}
	chomp(my @lines=<MORE>);
	close MORE;
	&::qtell($handle,@lines);
}

sub index {
	my($dir,$help)=@_;
	my(@list,%seen);
	foreach my $m (@::modules) {
		my $i="$dir/$m->[0]";
		-d $i or next;
		foreach my $j (glob "$i/*") {
			-d $j and next;
			$j=~m#/([^/]+)$# or next;
			my $title=$1;
			$seen{lc($title)} and next;
			my $ha=[$title,$j];
			push(@{$ha},1);
			my $usage=not $help;
			if ($help) {
				if (open(INDEX,$j)) {
					chomp(my $l=<INDEX>);
					if ($l=~/^#SEE +(.+)$/) {
						$ha->[2]=$1;
					} elsif ($l eq '#COMMAND') {
						$usage=1;
					}
					close INDEX;
				}
			} if ($usage) {
				foreach my $i (@::commands) {
					next unless ($i->[-1] eq $m->[0]);
					if (lc($i->[0]) eq lc($title)) {
						$ha->[2]=$i->[3];
						last;
					}
				}
			}
			push(@list,$ha);
			$seen{lc($title)}=1;
		}
	}
	return @list;
}

sub helpindex {
	my $handle=shift;
	@helpindex=&index('help',1);
	@usageindex=&index('usage');
	if ($handle eq '*DEBUG*') {
		printf "COR> %d helpfiles and %s usage files.\n",
		$#helpindex+1,$#usageindex+1;
		return;
	}
	&::send(sprintf("qtell %s Index rebuilt. %d helpfiles and %s usage ".
	"files.\n",$handle,$#helpindex+1,$#usageindex+1));
}

sub help {
	my($handle,$file)=@_;
	$file='index' if ($file=~/^ *$/);
	my @res=&::closestmatch(\@helpindex,$file,$handle,2);
	if ($res[0]==0) {
		shift @res;
		@res=sort {lc($a) cmp lc($b)} map {$_->[0]} @res;
		&::error($handle,$Lang::help_ambig.join(', ',@res));
		return;
	} elsif ($res[0]==2 or not -e $res[1]->[1]) {
		&::error($handle,"$file: $Lang::help_not_found");
		return;
	}
	my $title;
	($title,$file)=@{$res[1]};
	@res=(sprintf($Lang::help_head,$title),'');
	if (not open(HELP,$file)) {
		&::error($handle,"Couldn't open the helpfile.");
		return;
	}
	my $able=1;
	my $line;
	my $tellme=&::getavar('TellMe');
	while ($line=<HELP>) {
		chomp($line);
		study $line;
		if ($line=~/^#(!?)QUALIFIED +(.+)$/) {
			$able=(&::qualified($handle,$2) xor ($1 eq '!'));
			next;
		} elsif ($line eq '#BREAK') {
			$able=1;
			next;
		} elsif ($line eq '#COMMAND' or $line=~/^#SEE /) {
			next;
		}
		$able or next;
		if ($line eq '#STOP') {
			last;
		} elsif ($line eq '#USAGE') {
			my $f;
			foreach my $i (@usageindex) {
				if (lc($i->[0]) eq lc($title)) {
					$f=$i;
					last;
				}
			}
			if (not defined $f) {
				push(@res,'** Usage file missing!');
				next;
			} elsif (not open(USAGE,$f->[1])) {
				push(@res,'** Can\'t open usage file!');
				next;
			}
			chomp(my @usage=<USAGE>);
			push(@res,@usage);
			next;
		}
		$line=~s/%me/$::username/g;
		$line=~s/%tell/$tellme/g;
		$line=~s/%var'([^']+)'/eval("$1")/eg;
		push(@res,&Text::Wrap::wrap('','',$line));
	}
	close HELP;
	&::qtell($handle,@res);
}

sub usage {
	my($handle,$file)=@_;
	$file='usage' if ($file=~/^ *$/);
	my @res=&::closestmatch(\@usageindex,$file,$handle,2);
	if ($res[0]==0) {
		shift @res;
		@res=sort {lc($a) cmp lc($b)} map {$_->[0]} @res;
		&::error($handle,$Lang::usage_ambig.join(', ',@res));
		return;
	} elsif ($res[0]==2 or not -e $res[1]->[1]) {
		&::error($handle,"$file: $Lang::usage_not_found");
		return;
	}
	$file=$res[1]->[1];
	if (not open(USAGE,$file)) {
		&::error($handle,"Couldn't open the usage file.");
		return;
	}
	chomp(my @usage=<USAGE>);
	close USAGE;
	&::qtell($handle,@usage);
}

sub loadall {
	my $handle=shift;
	my $res=&::loadall;
	my @res=();
	if ($res & 1) {
		push(@res,'Failed to load lists!');
	} if ($res & 2) {
		push(@res,'Failed to load avariables!');
	}
	if ($#res==-1) {
		&::send("qtell $handle Files loaded.\n");
	} else {
		&::qtell($handle,@res);
	}
}

sub saveall {
	my $handle=shift;
	my $res=&::saveall;
	my @res=();
	if ($res & 1) {
		push(@res,'Failed to save lists!');
	} if ($res & 2) {
		push(@res,'Failed to save avariables!');
	}
	if ($#res==-1) {
		&::send("qtell $handle Files saved.\n");
	} else {
		&::qtell($handle,@res);
	}
}

sub lsmod {
	my $handle=shift;
	my @res=('Loaded modules:','');
	my $number=1;
	foreach my $i (@::modules) {
		my $inf=sprintf("%d. %s",$number,$i->[0]);
		my $sub=sprintf("JX_Mod::%s::ICS_lsmod",$i->[0]);
		if (defined &{$sub}) {
			$inf.=sprintf(" (%s)",
			&{$sub}(79-length($inf)-3));
		}
		push(@res,$inf);
		$number++;
	}
	&::qtell($handle,@res);
}

sub gather_info {
	my($mod,$list,$plural)=@_;
	my @items;
	foreach my $i (@{$list}) {
		if ($i->[-1] eq $mod) { push(@items,$i->[0]) }
	}
	@items=sort {lc($a) cmp lc($b)} @items;
	return('',sprintf("-- %d %s --",$#items+1,$plural),
	&::columns(@items));
}

sub modinfo {
	my($handle,$mod)=@_;
	return 255 if ($mod=~/^ *$/);
	my $f=0;
	foreach my $i (@::modules) {
		if ($i->[0] eq $mod) {
			$f=1;
			last;
		}
	}
	if (not $f) {
		&::error($handle,"Module $mod is not loaded.");
		return;
	}
	my @res=("Module information for $mod:");
	my $sub=sprintf("JX_Mod::%s::ICS_module_info",$mod);
	if (exists &{$sub}) {
		push(@res,'',&{$sub});
	}
	my @lists=(
		[\@::commands,  'commands'],
		[\@::lists,     'lists'],
		[\@::variables, 'variables'],
		[\@::avariables,'avariables'],
	);
	foreach my $i (@lists) {
		push(@res,&gather_info($mod,@{$i}));
	}
	&::qtell($handle,@res);
}

sub pose {
	my($handle,$stuff)=@_;
	return 255 if ($stuff=~/^ *$/);
	&::send("$stuff\n");
	&::send("qtell $handle $Lang::CORE::send_ack\n");
}

sub finger {
	my($handle,$user)=@_;
	(not defined($user) or $user=~/^ *$/) and $user=$handle;
	my @res=&::getpi($user);
	if ($res[0]==0) {
		&::error($handle,$Lang::no_handle);
		return;
	}
	my $online=$res[2];
	$user=$res[1];
	my $parray;
	foreach my $i (@::users) {
		if (lc($i->[0]) eq lc($user)) {
			$parray=$i;
			last;
		}
	}
	if (not defined $parray) {
		my @pr=&::playerfile($user);
		$parray=[
			$res[1],	#handle
			0,		#guest
			$res[6],	#email
			$res[7],	#ip
			@pr,
		];
	}
	@res=(sprintf($Lang::CORE::finger_header,$parray->[0],
	$online?'online':'offline'));
	foreach my $i (@::modules) {
		my $sub=sprintf("JX_Mod::%s::ICS_finger",$i->[0]);
		if (exists &{$sub}) {
			my @fs=&{$sub}($handle,$parray->[0],$parray);
			$fs[0] eq '' and shift @fs;
			$fs[-1] eq '' and pop @fs;
			$#fs==-1 and next;
			push(@res,'');
			foreach my $j (@fs) { push(@res,split(/\n/,$j)) }
		}
	}
	$parray->[6]->{'used'} or
	push(@res,'',"#$parray->[0] has not used $::username yet.");
	&::qtell($handle,@res);
}

sub eval {
	my($handle,$code)=@_;
	return 255 if ($code=~/^ *$/);
	my @res;
	{
		local $SIG{__DIE__}='DEFAULT';
		@res=eval($code);
	}
	chomp(@res);
	if ($@) {
		chomp($@);
		&::error($handle,"Execution failed: $@");
		return;
	}
	&::qtell($handle,'Code returned the following:','',@res);
}

sub resetnotes {
	my $handle=shift || '';
	my @fnotes;
	if (-e 'jobyx.notes') {
		open(NOTES,'jobyx.notes') and chomp(@fnotes=<NOTES>);
		close NOTES;
	}
	$#fnotes=7;
	push(@fnotes,"Powered by JobyX $::version, written by Chris Howie.",
	"JobyX is open-source.  http://jobyx.sf.net");
	for (my $i=9;$i>=0;$i--) {
		if ($i==0 and $fnotes[$i] eq '') {
			$fnotes[$i]='Put finger notes in jobyx.notes';
		}
		#&::send(sprintf("set 0 %s\n",$fnotes[$i] || ''));
	}
	&::send("qtell $handle Notes reset.\n") if ($handle);
}

sub commands {
	my $handle=shift;
	my(%seen,@coms);
	my $usable=0;
	foreach my $i (@::commands) {
		$seen{lc($i->[0])} and next;
		$seen{lc($i->[0])}=1;
		&::qualified($handle,$i->[3]) or next;
		$i->[5] or next;
		my $execlev='Unknown';
		if ($i->[2]=~/(\d+)/) { $execlev=&::fulllevelname($1) }
		my $seelev='Unknown';
		if ($i->[3]=~/(\d+)/) { $seelev =&::fulllevelname($1) }
		&::qualified($handle,$i->[2]) and $usable++;
		push(@coms,[$i->[0],$execlev,$seelev]);
	}
	@coms=sort {lc($a->[0]) cmp lc($b->[0])} @coms;
	unshift(@coms,['Command','Use level','See level'],[]);
	&::qtell($handle,$Lang::CORE::command_list,'',&::table(1,@coms),
	'',sprintf($Lang::CORE::command_list_footer,$#coms-1,$usable));
}

sub restart {
	my($handle,undef,$die)=@_;
	&::saveall;
	&::send(sprintf("qtell %s %s...\n",
	$handle,($die?'Shutting down':'Restarting')));
	while ($#::users>=0) {
		&::parse(sprintf("[%s has disconnected.]",$::users[0]->[0]));
	}
	foreach my $i (@::modules) {
		my $sub=sprintf("JX_Mod::%s::ICS_term",$i->[0]);
		defined(&{$sub}) and &{$sub}('*DEBUG*');
	}
	&::send("exit\n");
	&::log(sprintf("====== %s has ordered a %s at %s ======\n",$handle,
	($die?'shutdown':'restart'),scalar localtime));
	$die and exit;
	print "\n\nRestarting in two seconds...\n\n";
	sleep 2;
	exec($0);
	exit;
}

sub softquit { $#_=1;&restart(@_,1) }

sub togglelist {
	$#_=2;
	&edlist(@_,2);
}

sub addlist {
	$#_=2;
	&edlist(@_,1);
}

sub sublist {
	$#_=2;
	&edlist(@_,0);
}

sub edlist {
	my($handle,$list,$item,$add)=@_;
	return 255 if ($list=~/^ *$/ or $item=~/^ *$/);
	if (not ref($list)) {
		my @res=&::closestmatch(\@::lists,$list,$handle,1);
		if ($res[0]==2) {
			&::error($handle,"$list: $Lang::CORE::no_list");
			return;
		} elsif ($res[0]==0) {
			shift @res;
			@res=sort {lc($a) cmp lc($b)} map {$_->[0]} @res;
			&::error($handle,$Lang::list_ambig.join(', ',@res));
			return;
		}
		$list=$res[1];
	}
	my $owner=$list->[5] ? $handle : $::global_list;
	my $checked=0;
	if ($add==2) {
		if (defined $list->[6]) {
			my @res=&{$list->[6]}($handle,$list->[0],$item);
			if ($res[0]==0) {
				&::error($handle,$res[1] ? $res[1] :
				sprintf($Lang::CORE::bad_list_item,$list->[0]));
				return;
			} elsif ($res[0]==2) {
				$item=$res[1] or die
				"Null item from verifysub - list $list->[0]";
			}
			$checked=1;
		}
		$add=not &::inlist($list->[0],$item,$owner);
	}
	my $yourthe=$list->[5] ? $Lang::CORE::your : $Lang::CORE::the;
	my $addsub=$add ? $Lang::CORE::add : $Lang::CORE::sub;
	if ($add and not &::qualified($handle,$list->[3])) {
		&::error($handle,sprintf(
		$Lang::CORE::list_notqual_add,$yourthe,$list->[0]));
		return;
	} elsif (not $add and not &::qualified($handle,$list->[4])) {
		&::error($handle,sprintf(
		$Lang::CORE::list_notqual_sub,$yourthe,$list->[0]));
		return;
	}
	if (defined $list->[6] and not $checked) {
		my @res=&{$list->[6]}($handle,$list->[0],$item);
		if ($res[0]==0 and $add) {
			&::error($handle,$res[1] ? $res[1] :
			sprintf($Lang::CORE::bad_list_item,$list->[0]));
			return;
		} elsif ($res[0]==2) {
			$item=$res[1] or die
			"Null item from verifysub - list $list->[0]";
		}
	}
	my $contents=&::getlist($list->[0],$owner);
	my $in=0;
	foreach my $i (@{$contents}) {
		if (lc($item) eq lc($i)) {
			$in=1;
			last;
		}
	}
	if ($add and $in) {
		&::send("qtell $handle ".sprintf(
		"$Lang::CORE::already_in\n",$item,$yourthe,$list->[0]));
		return;
	} elsif (not $add and not $in) {
		&::send("qtell $handle ".sprintf(
		"$Lang::CORE::not_in\n",$item,$yourthe,$list->[0]));
		return;
	}
	my $notify=0;
	if ($add) {
		push(@{$contents},$item);
		defined($list->[7]) and
		$notify=&{$list->[7]}($handle,$list->[0],$item,1);
	} else {
		for (my $i=0;$i<=$#{$contents};$i++) {
			if (lc($item) eq lc($contents->[$i])) {
				splice(@{$contents},$i,1);
				last;
			}
		}
		defined($list->[8]) and
		$notify=&{$list->[8]}($handle,$list->[0],$item,0);
	}
	&::send(sprintf("qtell %s $Lang::CORE::modify_ack\n",
	$handle,$item,$addsub,$yourthe,$list->[0]));
	if (not $list->[5] and $notify) {
		&::send(sprintf(
		"qtell %s $Lang::CORE::modify_not\n",
		$item,$addsub,$::username,$list->[0],$handle));
	}
}

sub varinfo {
	my($handle,$var)=@_;
	return 255 if ($var=~/^ *$/);
	my @res=&::closestmatch(\@::variables,$var,$handle,1);
	if ($res[0]==2) {
		&::error($handle,"$var: $Lang::CORE::no_var");
		return;
	} elsif ($res[0]==0) {
		shift @res;
		@res=sort {lc($a) cmp lc($b)} map {$_->[0]} @res;
		&::error($handle,$Lang::variable_ambig.join(', ',@res));
		return;
	}
	$var=$res[1];
	@res=(
		[$Lang::CORE::variable,$var->[0]],
		[],
		[$Lang::CORE::uselv,&::fulllevelname($var->[1])],
		[$Lang::CORE::default,$var->[2]],
	);
	&::qtell($handle,&::table(1,@res));
}

sub commandinfo {
	my($handle,$command)=@_;
	return 255 if ($command=~/^ *$/);
	my @res=&::closestmatch(\@::commands,$command,$handle,3);
	if ($res[0]==2) {
		&::error($handle,"$command: $Lang::CORE::no_command");
		return;
	} elsif ($res[0]==0) {
		shift @res;
		@res=sort {lc($a) cmp lc($b)} map {$_->[0]} @res;
		&::error($handle,$Lang::command_ambig.join(', ',@res));
		return;
	}
	$command=$res[1];
	@res=(
		[$Lang::CORE::command,$command->[0]],
		[],
		[$Lang::CORE::seelv,&::fulllevelname($command->[3])],
		[$Lang::CORE::uselv,&::fulllevelname($command->[2])],
		[$Lang::CORE::visible,$command->[5]?$Lang::yes:$Lang::no],
	);
	&::qtell($handle,&::table(1,@res));
}

sub listinfo {
	my($handle,$list)=@_;
	return 255 if ($list=~/^ *$/);
	my @res=&::closestmatch(\@::lists,$list,$handle,1);
	if ($res[0]==2) {
		&::error($handle,"$list: $Lang::CORE::no_list");
		return;
	} elsif ($res[0]==0) {
		shift @res;
		@res=sort {lc($a) cmp lc($b)} map {$_->[0]} @res;
		&::error($handle,$Lang::list_ambig.join(', ',@res));
		return;
	}
	$list=$res[1];
	@res=(
		[$Lang::CORE::list,$list->[0]],
		[],
		[$Lang::CORE::seelv,&::fulllevelname($list->[1])],
		[$Lang::CORE::viewlv,&::fulllevelname($list->[2])],
		[$Lang::CORE::addlv,&::fulllevelname($list->[3])],
		[$Lang::CORE::sublv,&::fulllevelname($list->[4])],
		[$Lang::CORE::list_public,$list->[5]?$Lang::no:$Lang::yes],
		[$Lang::CORE::list_max,$list->[9]?$list->[9]:$Lang::no],
	);
	&::qtell($handle,&::table(1,@res));
}

sub showlist {
	my($handle,$list)=@_;
	if ($list=~/^ *$/) {
		my(%lists,@ls);
		foreach my $i (@::lists) {
			exists($lists{lc($i->[0])}) and next;
			$lists{lc($i->[0])}=$i;
			&::qualified($handle,$i->[1]) or next;
			my $seelev= 'Unknown';
			my $viewlev='Unknown';
			if ($i->[1]=~/(\d+)/) { $seelev=&::fulllevelname($1)}
			if ($i->[2]=~/(\d+)/) {$viewlev=&::fulllevelname($1)}
			push(@ls,[$i->[0],$i->[5]?$Lang::no:$Lang::yes,
			$seelev,$viewlev]);
		}
		@ls=sort {lc($a->[0]) cmp lc($b->[0])} @ls;
		unshift(@ls,['List','Public','See level','View level'],[]);
		&::qtell($handle,&::table(1,@ls));
	} else {
		my @res=&::closestmatch(\@::lists,$list,$handle,1);
		if ($res[0]==2) {
			&::error($handle,"$list: $Lang::CORE::no_list");
			return;
		} elsif ($res[0]==0) {
			shift @res;
			@res=sort {lc($a) cmp lc($b)} map {$_->[0]} @res;
			&::error($handle,$Lang::list_ambig.join(', ',@res));
			return;
		}
		my $list=$res[1];
		my $yourthe=$list->[5] ? $Lang::CORE::your : $Lang::CORE::the;
		if (not &::qualified($handle,$list->[2])) {
			&::error($handle,sprintf(
			$Lang::CORE::list_notqual_view,$yourthe,$list->[0]));
			return;
		}
		my $owner=$list->[5] ? $handle : $::global_list;
		my @content=sort {lc($a) cmp lc($b)}
		@{&::getlist($list->[0],$owner)};
		@res=(sprintf("-- $Lang::CORE::list_out: %d %s --",
		$list->[0],$#content+1,$list->[10]));
		if ($#content>-1) {
			push(@res,'',&::columns(@content));
		}
		&::qtell($handle,@res);
	}
}

sub variables {
	my($handle,$who)=@_;
	if (lc($who) eq lc($::username)) {
		&::send(sprintf("qtell %s $Lang::CORE::no_player_vars\n",
		$handle,$::username));
		return;
	}
	$who=$handle if ($who=~/^ *$/);
	my @res=&::getpi($who);
	if ($res[0]==0) {
		&::error($handle,$Lang::no_handle);
		return;
	}
	$who=$res[1];
	my $p;
	foreach my $i (@::users) {
		if (lc($i->[0]) eq lc($who)) {
			$p=$i;
			last;
		}
	}
	if (not defined($p)) {
		my @res=&::playerfile($who);
		$p=[$handle,0,'','',@res];
	}
	my(@vars,%seen);
	foreach my $i (@::variables) {
		next if $seen{lc($i->[0])};
		$seen{lc($i->[0])}=1;
		next if ($i->[3]==2);
		&::qualified($p,$i->[1]) or next;
		($i->[3]==1) and (lc($handle) ne lc($who)) and next;
		push(@vars,$i->[0]);
	}
	@vars=sort {lc($a) cmp lc($b)} @vars;
	if ($#vars==-1) {
		&::send(sprintf("qtell %s $Lang::CORE::no_player_vars\n",
		$handle,$who));
		return;
	}
	my @pv;
	foreach my $i (@vars) {
		push(@pv,sprintf("%s=%s",$i,&::getvar($who,$i)));
	}
	&::qtell($handle,sprintf($Lang::CORE::variable_settings,$who),'',
	&::columns(@pv));
}

sub avariables {
	my $handle=shift;
	my(%seen,@res);
	foreach my $i (@::avariables) {
		next if $seen{lc($i)};
		$seen{lc($i)}=1;
		push(@res,sprintf("%s=%s",$i->[0],&::getavar($i)));
	}
	if ($#res==-1) {
		&::send(sprintf("qtell %s $Lang::CORE::no_avars\n",
		$handle,$::username));
		return;
	}
	&::qtell($handle,sprintf($Lang::CORE::avariable_settings,$::username),
	'',&::columns(@res));
}

sub set {
	my($handle,$var,$new)=@_;
	return 255 if ($var=~/^ *$/);
	my @res=&::closestmatch(\@::variables,$var,$handle,1);
	if ($res[0]==2) {
		&::error($handle,$Lang::CORE::no_var);
		return;
	} elsif ($res[0]==0) {
		shift @res;
		@res=sort {lc($a) cmp lc($b)} map {$_->[0]} @res;
		&error($handle,$Lang::variable_ambig.join(', ',@res));
		return;
	}
	my $dvar=$res[1];
	$var=$dvar->[0];
	my @ver=&{$dvar->[4]}($handle,$var,$new,&::getvar($handle,$dvar));
	if ($ver[0]==0) {
		&::error($handle,defined($ver[1])?$ver[1]:
		sprintf($Lang::CORE::bad_var,$var));
		return;
	} elsif ($ver[0]==2) {
		$new=$ver[1];
	}
	my $p;
	foreach my $i (@::users) {
		if (lc($i->[0]) eq lc($handle)) {
			$p=$i;
			last;
		}
	}
	if (not defined($p)) {
		&::error($handle,$Lang::CORE::no_var);
		return;
	}
	$p->[5]->{lc($var)}=$new;
	defined($dvar->[5]) and
	&{$dvar->[5]}($handle,$var,$new,&::getvar($handle,$dvar));
	&::send("qtell $handle $var==$new\n");
}

sub aset {
	my($handle,$var,$new)=@_;
	return 255 if ($var=~/^ *$/);
	my @res=&::closestmatch(\@::avariables,$var,$handle,1);
	if ($res[0]==2) {
		&::error($handle,$Lang::CORE::no_avar);
		return;
	} elsif ($res[0]==0) {
		shift @res;
		@res=sort {lc($a) cmp lc($b)} map {$_->[0]} @res;
		&error($handle,$Lang::avariable_ambig.join(', ',@res));
		return;
	}
	my $avar=$res[1];
	$var=$avar->[0];
	my $last=&::getavar($avar);
	my @ver=&{$avar->[3]}($handle,$var,$new,$last);
	if ($ver[0]==0) {
		&::error($handle,defined($ver[1])?$ver[1]:
		sprintf($handle,$Lang::CORE::bad_avar,$var));
		return;
	} elsif ($ver[0]==2) {
		$new=$ver[1];
	}
	$::avariables{lc($var)}=$new;
	defined($avar->[5]) and &{$avar->[5]}($handle,$var,$new,$last);
	&::send("qtell $handle $var==$new\n");
}

sub setlevel {
	my($handle,$who,$level,$force)=@_;
	return 255 if ($who=~/^ *$/ or $level=~/^ *$/);
	$level=int($level);
	return 255 if ($level<0 or $level>100);
	my @ui=&::getpi($who);
	if ($ui[0]==0) {
		&::error($handle,$Lang::no_handle);
		return;
	}
	$who=$ui[1];
	@ui=&::userinfo($handle);
	$ui[0] or return;
	my $hlv=$ui[1]->[4];
	if (not $force and $level>=$hlv) {
		&::error($handle,sprintf(
		$Lang::CORE::level_too_high,$::username));
		return;
	}
	@ui=&::userinfo($who);
	$ui[0] or return;
	my $wholv=$ui[1]->[4];
	if ($wholv>=$hlv) {
		&::error($handle,sprintf(
		$Lang::CORE::level_higher,$who,$::username));
		return;
	}
	$ui[1]->[4]=$level;
	my $lv=&::fulllevelname($level);
	&::send(sprintf("qtell %s $Lang::CORE::level_changed\n",
	$handle,$who,$::username,$lv));
	&addcomment($handle,$who,sprintf(
	$Lang::CORE::level_changed_comment,$::username,$lv));
	&::send(sprintf("qtell %s $Lang::CORE::level_changed_ack\n",
	$who,$handle,$::username,$lv));
	&::saveparray($ui[1]) unless ($ui[2]);
}

1;
