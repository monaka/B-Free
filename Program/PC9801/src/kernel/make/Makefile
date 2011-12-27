#
#
#

all: clean btron

btron: build
	build btron

build: config.c build.c
	cc -o build build.c config.c

config.c: config.tab getentry
	config.pl < config.tab > config.c

getentry: getentry.c
	cc -o getentry getentry.c

clean:
	rm -f config.c
