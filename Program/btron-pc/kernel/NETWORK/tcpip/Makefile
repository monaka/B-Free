#
#

OBJS=ip.o tcp.o udp.o tcpip.o link.o
CC = gcc

all: $(OBJS)

ip.o: ip.c ip.h
tcp.o : tcp.c tcp.h ip.h
udp.o : udp.c udp.h ip.h
tcpip.o : tcpip.c tcpip.h
link.o : link.c tcpip.h
lowlib.o : lowlib.c tcpip.h



clean:
	rm -f $(OBJS)
