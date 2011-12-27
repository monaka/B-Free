#
#
#
#

.dvi.ps:
	dvi2ps $*.dvi > $*.ps

.tex.dvi:
	jlatex $*.tex
	jlatex $*.tex

JTEX=/usr/local/bin/jtex
JLATEX=/usr/local/bin/jlatex
DVI2TTY=/usr/local/bin/dvi2tty
DVI2PS=/usr/local/bin/dvi2ps

all: library.ps


library.ps: library.dvi
	dvi2ps library.dvi > library.ps

library.dvi: library.tex
	jlatex $*.tex
	jlatex $*.tex
