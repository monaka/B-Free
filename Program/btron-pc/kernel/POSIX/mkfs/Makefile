
CFLAGS=-Wall

all: mksfs statfs sfsck

user_inst:
	cp mksfs mksfs.sh  /usr/local/bin/
	rm -f /usr/local/bin/mkfs.sfs
	ln mksfs.sh /usr/local/bin/mkfs.sfs
	cp statfs sfsck /usr/local/bin

mksfs: mkfs.c
	cc $(CFLAGS) -o mksfs mkfs.c

#statfs: statfs.c
#	cc -o statfs statfs.c

clean:
	rm -f statfs mksfs sfsck
