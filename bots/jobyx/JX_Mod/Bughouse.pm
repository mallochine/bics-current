package JX_Mod::Bughouse;

sub ICS_init {
	return ([['!',           \&atell,   1,   1,  2,   0  ]],[],[],[],undef);
}

sub atell {
my($handle,$what)=@_;
 &::send ("$what\n");

}

sub ICS_input {
	my $line=shift;
# <12> r-bqkb-r ppp--pp- --n-pn-p ---pN--- ---P---B --N----- PPP-PPPP R--QKB- R B -1 1 1 1 1 1 1 fuckbot botone -1 3 0 42 42 180000 162233 6 B/g5-h4 (-27:-36. -60) Bh4 0 1 0
	$i="^<12>*";
	if($line=~/$i/i)
	{
		$line=~/\s(\S+)\s\(.*/;
		$move=$1;
		$move=~s#^(.*)/##;
		$fig=$1;
		$move=~s#@@#$fig@#;
		$move=~s#-##;
		&::send ("$move\n");
		return 0;
	}
	$i="Challenge:";
	if($line=~/$i/i)
	{
		&::send ("accept\n");
		return 0;
	}
	$i="Your partner is playing game";
	if($line=~/$i/i)
	{
		&::send ("pobserve\n");
		return 0;
	}
	$i="partner";
	if($line=~/$i/i)
	{
		&::send ("accept\n");
		return 0;
	}
	return 0;
}

sub ICS_module_info {
	return ('Module for play bug');
}

1;
