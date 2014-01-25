
#ifndef _LISTS_H
#define _LISTS_H

/* yes, it's all cheesy..  there is no significance to the order, but make
   sure it matches the order in lists.c */

GENSTRUCT enum ListWhich {L_ADMIN = 0, L_REMOVEDCOM, L_FILTER, L_BAN, L_RATEBAN, L_NOTEBAN, L_ABUSER,
			  L_MUZZLE, L_CMUZZLE, L_C1MUZZLE, L_C24MUZZLE, L_C46MUZZLE, L_C49MUZZLE,
			  L_C50MUZZLE, L_C51MUZZLE, L_FM, L_IM, L_GM, L_SR, L_TM, L_WFM, L_WIM, L_WGM, L_BLIND, L_TEAMS,
			  L_COMPUTER, L_TD,
			  L_CENSOR, L_GNOTIFY, L_NOPLAY, L_NOTIFY, L_CHANNEL, L_NOPARTNER, /* L_FOLLOW,*/
			  L_REMOTE,
              L_IGNORE,
              L_ALLOWTELL,
              L_ALLOWPLAY,
              L_ALLOWALL,
              L_HEARCH,
			  L_LASTLIST /* this MUST be the last list, add all lists before it */
};

GENSTRUCT enum ListPerm {P_HEAD = 0, P_GOD, P_ADMIN, P_PUBLIC, P_PERSONAL};

typedef struct {enum ListPerm rights; char *name;} ListTable;

GENSTRUCT struct List {
	enum ListWhich which;
	int numMembers;
	char **m_member; _LEN(numMembers)
	struct List *next;
};



#endif   /* _LISTS_H */
