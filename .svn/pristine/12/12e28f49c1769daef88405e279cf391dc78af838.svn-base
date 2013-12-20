package JX_Mod::Tourney;

sub ICS_init {
	return(
	[ #commands
	#name           sub          exec see args vis
	['register',           \&addlist,   1,   1,  2,   0  ],
	['create',           \&sublist,   1,   1,  2,   0  ],
	['unregister',           \&showlist,  1,   1,  1,   0  ],

	],[ #lists
	#name see view add sub pri verify     addsub  subsub  max type def
	['Tourney',
	      1,  1,   50, 60, 0,  \&::handle,\&newtu,\&remtu,0,  'names',[$::operator]],

	],[ #vars
	#name have def vis verify      set
	['Idiots',1,24,0,  \&var_height,undef],

	],[ #avars
	#name     see set verify      def                setsub
	['TD',90,100,\&::handle,$::operator,\&mkcoo],
	],
	undef	
	);
}

sub ICS_module_info {
	return ('Module for organize Tourney');
}

1;
