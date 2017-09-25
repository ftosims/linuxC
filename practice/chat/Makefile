all:
	gcc serv.c -g -o tserver -pthread -lmysqlclient
	gcc cli.c -g -o tclient -pthread -lcurses -DNCURSES_INTERNALS -DNCURSES_OPAQUE=0 #-lsocket -lnsl 
clean:
	rm -f server client
