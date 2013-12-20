#------------------------
# tbot.MAK make file
#------------------------

tbot.exe : main.obj arb.obj command.obj font.obj group.obj gpplayer.obj help.obj history.obj idle.obj ini.obj parse.obj player.obj queue.obj script.obj state.obj sys.obj telnet.obj timeseal.obj toolbox.obj main.res
     $(LINKER) $(GUIFLAGS) -OUT:tbot.exe main.obj arb.obj command.obj font.obj group.obj gpplayer.obj help.obj history.obj idle.obj ini.obj parse.obj player.obj queue.obj script.obj state.obj sys.obj telnet.obj timeseal.obj toolbox.obj main.res $(GUILIBS)

main.obj : main.c global.h
     $(CC) $(CFLAGS) main.c

arb.obj : arb.c arb.h global.h
     $(CC) $(CFLAGS) arb.c

command.obj : command.c command.h global.h
     $(CC) $(CFLAGS) command.c

font.obj : font.c font.h global.h
     $(CC) $(CFLAGS) font.c

gpplayer.obj : gpplayer.c gpplayer.h global.h
     $(CC) $(CFLAGS) gpplayer.c

group.obj : group.c group.h global.h
     $(CC) $(CFLAGS) group.c

help.obj : help.c help.h global.h
     $(CC) $(CFLAGS) help.c

history.obj : history.c history.h global.h
     $(CC) $(CFLAGS) history.c

ini.obj : ini.c ini.h global.h
     $(CC) $(CFLAGS) ini.c

idle.obj : idle.c idle.h global.h
     $(CC) $(CFLAGS) idle.c

parse.obj : parse.c parse.h global.h
     $(CC) $(CFLAGS) parse.c

player.obj : player.c player.h global.h
     $(CC) $(CFLAGS) player.c

queue.obj : queue.c queue.h global.h
     $(CC) $(CFLAGS) queue.c

script.obj : script.c script.h global.h
     $(CC) $(CFLAGS) script.c

state.obj : state.c state.h global.h
     $(CC) $(CFLAGS) state.c

sys.obj : sys.c sys.h global.h
     $(CC) $(CFLAGS) sys.c

telnet.obj : telnet.c telnet.h global.h
     $(CC) $(CFLAGS) telnet.c

timeseal.obj : timeseal.c timeseal.h global.h
     $(CC) $(CFLAGS) timeseal.c

toolbox.obj : toolbox.c toolbox.h global.h
     $(CC) $(CFLAGS) toolbox.c

main.res : main.rc main.ico global.h
     $(RC) $(RCVARS) main.rc
