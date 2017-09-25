all:
	gcc server.c -g -o server -pthread -lmysqlclient
	gcc client.c -g -o client -pthread -lcurses -DNCURSES_INTERNALS -DNCURSES_OPAQUE=0 #-lsocket -lnsl 
clean:
	rm -f server client
