package Lang::CORE;

$loaded=1;

# name of TUs (make sure to edit it in jobyxconfig.pm!)
# for a tourney manager, for example, add these lines to jobyxconfig.pm:

# $Lang::CORE::TU_name = 'Manager';
# $Lang::CORE::TU_plural = 'Managers';
# &Lang::CORE::do_tu_names;

# If you're defining your userlevels depending on these constants (see the
# example config file), you'll need to do so _after_ you set these.

# what do we call our TUs?
$TU_name = 'TU';
$TU_plural = 'TUs';

sub do_tu_names {
# These all derive from $TU_name and $TU_plural.
# list name
$TU_list = $TU_name;
# TU list commants
$List_TU_Command = "List$TU_plural";
$List_TU_Alias = 'l'.lc(substr($TU_plural,0,1));	# shot in the dark
# no TUs
$no_TUs = "No $TU_plural to show.";
# TU list header
$TU_list_head = "$TU_plural:";
# TU list footer
$TU_list_footer = "%d $TU_plural displayed.";
}
&do_tu_names;

# a few signs that the connection has been dropped
@connection_fail = (
	'^ *Logging you out.',
	'^ *Thank you for using',
	'^ *\*+ *Server shutting down immediately.',
);
# ack for Send command
$send_ack = "Sent to server.";
# finger header
$finger_header = "Information on %s, who is %s:";
# userlevel
$finger_level = "%s level: ";
# usage not written
$no_usage = "Usage for %s not written.";
# no list
$no_list = "No such list.";
# bad item
$bad_list_item = "Bad item for list %s.";

# list acks
$list_out = "%s list";
$list_all = "Lists:";
$your = 'your';
$the = 'the';
$add = 'added to';
$sub = 'removed from';
$list_notqual_add  = "You are not qualified to add items to %s %s list.";
$list_notqual_sub  = "You are not qualified to remove items from %s %s list.";
$list_notqual_view = "You are not qualified to view %s %s list.";
$already_in = "[%s] is already in %s %s list.";
$not_in = "[%s] is not in %s %s list.";
$modify_ack = "[%s] has been %s %s %s list.";
$modify_not = "You have been %s %s's %s list by %s.";
$list_public = 'PUBLIC';
$list_private = 'PRIVATE';
$no_lists = 'No lists available.';

$no_command = 'No such command.';
$list = 'List:';
$command = 'Command:';
$variable = 'Variable:';
$default = 'Default:';
$seelv = 'See level:';
$uselv = 'Use level:';
$viewlv = 'View level:';
$addlv = 'Add level:';
$sublv = 'Remove level:';
$visible = 'Visible:';
$list_public = 'Public:';
$list_max = 'Item limit:';

# variable settings
$variable_settings = "Variable settings of %s:";
# no variables
$no_player_vars = "No variables to show for %s.";
# no variable
$no_var = 'No such variable.';
# bad variable setting
$bad_var = "Bad setting for variable %s.";
# command list header
$command_list = 'Commands:';
# command list footer
$command_list_footer = "%d commands displayed, %d usable.";

# avariable
$avariable_settings = "Avariable settings of %s:";
# no avariables
$no_avars = "No avariables to show for %s.";
# no avariable
$no_avar = 'No such avariable.';
# bad avariable setting
$bad_avar = "Bad setting for avariable %s.";

# level too high
$level_too_high = "You can't put someone on the same or higher %s level than yourself.";
# can't change level
$level_higher = "You are not qualified to change %s's %s level.";
# level changed
$level_changed = "%s's %s level set to %s.";
$level_changed_ack = "%s has set your %s level to %s.";
$level_changed_comment = "%s level set to %s.";

# no comments
$no_comments = "No comments to show for %s.";
# comments
$comments = "Comments for %s:";
# finger comments
$finger_comments = "Comments: %d";
# no comment
$no_comment = "No such comment.";
