#!/usr/local/bin/perl

##
#  mamer - Tournament Director for the Free Internet Chess Server
#  
#  Copyright (C) 1995 Fred Baumgarten
#  Copyright (C) 1996-2001 Michael A. Long
#  Copyright (C) 1996-2001 Matthew E. Moses
#  Copyright (C) 2002 Richard Archer
#  
#  $Id: report.perl,v 1.2 2002/08/08 02:53:35 rha Exp $
#  
#  mamer is free software; you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published 
#  by the Free Software Foundation; either version 2 of the License, 
#  or (at your option) any later version.
#  
#  mamer is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty
#  of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
#  See the GNU General Public License for more details.
#  
#  You should have received a copy of the GNU General Public License
#  along with mamer; if not, write to the Free Software Foundation, 
#  Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
##

if($ARGV[0] eq "") {
    die "Usage: report.perl <file>";
}

$num_of_days=0;
$inputfile = $ARGV[0];
$outputfile = "stats";
$managersfile = "/home/mlong/mamer/data/managers";

unlink("$outputfile");

open(FILE, $managersfile) || die "Can't open $managersfile\n";
while($lines = <FILE>) {
    chop($lines);
    @line = split(/\s/, $lines);
    push(@managers, $line[0]);
}

foreach $a (@managers) {
    $managertotals{$a}=0;
}

open(FILE, $inputfile) || die "2. can't open $inputfile\n";
$number_of_tourneys = 0;
while($lines = <FILE>) {
    if($lines =~ /\*\*Tourney Started/) {
	chop($lines);
	$number_of_tourneys++;
	($junk, $junk, $day, $month, $day_month, $t, $year, $manager, $style, $timed, , $inc, $mode, $variant, $ratingLow, $ratingHigh, $players, $rounds) = split(/\s+/, $lines);

	if($timed > 15){
	    printf "$lines\n";
	    print "day = $day\nmonth = $month\nday_month = $day_month\ntime = $t\nyear = $year\nmanager = $manager\nstyle = $style\ntime = $timed\ninc = $inc\n";	
	}
##	$b = <stdin>;

	$dates{"$year $month $day_month"}++;                      ## Wed Oct  7 17:21:23 1998
	$managertotals{$manager}++;
	if($style eq "s") {
	    $swisstotal++;
	} elsif($style eq "r") {
	    $rrtotal++;
	} elsif($style eq "d") {
	    $drrtotal++;
	} elsif($style eq "k") {
	    $kototal++;
	} else {
	    $unknowntotal++;
	}
	$roundstotal += $rounds;
	$playerstotal += $players;
	$timetotal += $timed;
	$inctotal += $inc;
	$tourneys{"$timed $inc"}++;
    }
}

open(OUT, ">$outputfile") || die "can't open $outputfile\n";
$temp = sprintf "Total number of tourneys: %2d\n\n", $number_of_tourneys; print OUT $temp;
$temp = sprintf "Total number of Round Robin tourneys: %2d\n", $rrtotal;  print OUT $temp;
$temp = sprintf "Total number of DRR tourneys: %2d\n", $drrtotal;       print OUT $temp;
$temp = sprintf "Total number of Swiss tourneys: %2d\n", $swisstotal;   print OUT $temp;
$temp = sprintf "Total number of KO tourneys: %2d\n", $kototal;         print OUT $temp;
$temp = sprintf "Total number of UNK tourneys: %2d\n\n", $unknowntotal;   print OUT $temp;

print OUT "   Tourneys by manager:        Tourneys by speed:\n";
print OUT "   ----------------------      ----------------------\n";
$i=0;
foreach $key (sort by_name (keys %managertotals)) {
    $text[$i] = sprintf ("   %-17s %4d", $key, $managertotals{$key});
    $i++;
}
$i=0;
foreach $key (sort by_name (keys %tourneys)) {
    $text[$i] = $text[$i] . sprintf("      %-8s %4d", $key, $tourneys{$key});
    $i++;
}
foreach $a (@text) {
    print OUT "$a\n";
}
foreach $a (keys(%dates)) {
    $num_of_days++;
}
$avgtourns = $number_of_tourneys / $num_of_days;
$temp = sprintf "\n Number of days counted: %2d\n", $num_of_days;               print OUT $temp;
$temp = sprintf " Average number of tourneys per day = %5.2f\n", $avgtourns;    print OUT $temp;

$avgplayers = $playerstotal / $number_of_tourneys;
$temp = sprintf " Average number of players in tourney = %5.2f\n", $avgplayers; print OUT $temp;
$avgrounds = $roundstotal / $number_of_tourneys;
$temp = sprintf " Average number of rounds in tourney = %5.2f\n", $avgrounds;   print OUT $temp;
$avgtime = $timetotal / $number_of_tourneys;
$temp = sprintf " Average time in tourneys = %5.2f\n", $avgtime;                print OUT $temp;
$avginc = $inctotal / $number_of_tourneys;
$temp = sprintf " Average inc in tourneys = %5.2f\n", $avginc;                  print OUT $temp;

sub by_number {
    $a <=> $b;
}

sub by_name {
    $c = $a;
    $d = $b;
    $c =~ tr/a-z/A-Z/;
    $d =~ tr/a-z/A-z/;
    $c cmp $d;
}


