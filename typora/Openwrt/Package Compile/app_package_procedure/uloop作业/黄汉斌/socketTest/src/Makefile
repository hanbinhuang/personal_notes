LINKLIB:= -lubox
STAG_INCL_PATH   := $(STAGING_DIR)/usr/include
STAG_LIB_PATH    := $(STAGING_DIR)/usr/lib
export INFLAGS   := -I $(STAG_INCL_PATH)
export LDFLAGS   := -L $(STAG_LIB_PATH)
export DBGFLAGS  := -Wall 


all:client server 

client:tcp_client.o
	$(CC) -o client  tcp_client.o  $(LINKLIB) $(DBGFLAGS) $(INFLAGS) $(LDFLAGS)

server: tcp_server.o								
	$(CC) -o server  tcp_server.o  $(LINKLIB) $(DBGFLAGS) $(INFLAGS) $(LDFLAGS)
clean:														
	rm -f client server *.o
