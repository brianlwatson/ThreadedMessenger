# Makefile for echo client and server

CXX=			g++ $(CCFLAGS)

ECHO-SERVER=		echo-server.o inet-server.o server.o
ECHO-CLIENT=		echo-client.o inet-client.o client.o
OBJS =			$(ECHO-SERVER) $(ECHO-CLIENT)

LIBS=

CCFLAGS= -g

all:	echo-server echo-client

echo-server:$(ECHO-SERVER)
	$(CXX) -std=c++11 -o msgd $(ECHO-SERVER) $(LIBS) -pthread 

echo-client:$(ECHO-CLIENT)
	$(CXX) -o msg $(ECHO-CLIENT) $(LIBS)

clean:
	rm -f $(OBJS) $(OBJS:.o=.d)

realclean:
	rm -f $(OBJS) $(OBJS:.o=.d) msgd msg


# These lines ensure that dependencies are handled automatically.
%.d:	%.cc
	$(SHELL) -ec '$(CC) -M $(CPPFLAGS) $< \
		| sed '\''s/\($*\)\.o[ :]*/\1.o $@ : /g'\'' > $@; \
		[ -s $@ ] || rm -f $@'

include	$(OBJS:.o=.d)
