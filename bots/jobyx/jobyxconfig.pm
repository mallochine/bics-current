# Language packs.  You'll need to put each module's respective pack here, or
# they will complain that the language pack isn't loaded.  Yes, the modules
# _could_ insert them by themselves, but then you couldn't define custom
# constants in this file, and that wouldn't be very flexible, now would it?
use Lang::English;
use JX_Mod::CORE::English;

# Custom language constants.  You should put them here to keep a JobyX update
# from clobbering your customized file, while also allowing updates to supply
# new constants.

# Sample:
# Delete it (or at least comment it out) if you're not going to use it!

# Parse shouts instead of tells.
$Lang::tell = "tells you:";	# Probably not a good idea...

# We don't really expect nukes, and servers usually write "Logging you out."
# upon nuke, so we're safe.  This line for demonstration purposes only.
push(@Lang::CORE::connection_fail,'^ *\*+ You have been kicked out');

# End custom language constants.

# Login information.
our $username='tourneybot';
our $password='234';
our $server='fw';
our $port=5001;
our $operator='Aramen';

# If we need to know each user's email address, which we don't if the bot's
# account isn't an administrator.  Turning this on will "finger" every user
# when they log on, and will significantly delay logon time for the bot,
# especially over a slow connection.  If your bot is an admin, but doesn't need
# an email address for each user, be sure to leave it off.
our $needemail=0;

# Turn this on if you need to know if a user is a guest.  Similar effect as
# turning on needemail, except users are only fingered when they log on, not
# when the "who" list is retrieved.
our $needguest=1;

# Command to retrieve finger info.  Try to find one that omits user's custom
# notes, as this parses faster.  This would probably be better implemented as
# a language constant, but hey, it works.
our $finger_command="finger %s r";
# Change this to "rfinger %s" on DNCS, for example.

# Maximum communication size.
our $max_communication=1000;

# Name of log file.
our $logfile='jobyx.log';

# _INVALID_ server handle to be used for global lists.  Note that the
# filesystem must support the name! (No "*"s on Windows, for example.)
our $global_list='~global~';

# User levels to display on "Finger" output.  DO NOT remove levels for 0, 1,
# 10, 99, or 100!  You can change the name, so long as it retains the meaning.
our @levels=(
	[0,  'Abuser'],
	[1,  'Guest'],
	[10, 'Player'],
	[20, $Lang::CORE::TU_name],
	[30, $Lang::CORE::TU_name],
	[50, 'VP'],
	[60, 'XO'],
	[90, 'Assistant'],
	[95, 'Acting COO'],
	[96, 'COO'],
	[99, 'Operator'],
	[100,'[TBA]'],
);

# Default modules to load.
our @default_modules=(
	# Don't remove/change this line or JobyX will be unusable!
	'CORE',
	'Tourney',
	#'Bughouse',
	# Allows you to put custom helpfiles in help/CustomHelp/.  This module
	# should always be the last one in the list.
	'CustomHelp',
);

# Whether or not to try to intelligently place bars ("|") on separators fed to
# &::table.  Turn it off if you don't like the results.
our $smartbar=1;

1;
