

all: region.ps region.nup

region.ps: region.dvi
	dvi2ps region.dvi > region.ps

region.nup: region.ps
	nup < region.ps > region.nup

clean:
	rm -f region.ps region.nup