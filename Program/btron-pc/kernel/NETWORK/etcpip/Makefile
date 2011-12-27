OBJS=ip.o tcp.o udp.o

all: $(OBJS)

ip.o: ip.c ip.h
tcp.o : tcp.c tcp.h ip.h
udp.o : udp.c udp.h ip.h

clean:
	rm -f $(OBJS)
