

all: itron.ps

itron.ps: itron.dvi
	dvi2ps itron.dvi > itron.ps

itron.nup: itron.ps
	nup -2 < itron.ps > itron.nup

	