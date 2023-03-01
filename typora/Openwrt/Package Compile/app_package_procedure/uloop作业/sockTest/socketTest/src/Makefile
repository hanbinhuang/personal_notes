LINKLIB:= -lubox
STAG_INCL_PATH   := $(STAGING_DIR)/usr/include
STAG_LIB_PATH    := $(STAGING_DIR)/usr/lib
export INFLAGS   := -I $(STAG_INCL_PATH)
export LDFLAGS   := -L $(STAG_LIB_PATH)
export DBGFLAGS  := -Wall -fPIC -DBUILD_TIME="\"`date`\"" -DDEBUG


all:client server 

client:tcp_client.o
	$(CC) -o client  tcp_client.o  -D_GNU_SOURCE  $(LINKLIB) $(DBGFLAGS) $(INFLAGS) $(LDFLAGS)

server: tcp_server.o								
	$(CC) -o server  tcp_server.o  -D_GNU_SOURCE  $(LINKLIB) $(DBGFLAGS) $(INFLAGS) $(LDFLAGS)
clean:															
	rm -f client server *.o
