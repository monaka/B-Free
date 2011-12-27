#
#
#

JTEX=/usr/local/bin/jtex
JLATEX=/usr/local/bin/jlatex
DVI2TTY=/usr/local/bin/dvi2tty
DVI2PS=/usr/local/bin/dvi2ps

all: btron386.ps
	
btron386.ps: btron386.dvi
	${DVI2PS} btron386.dvi > btron386.ps

btron386.dvi: btron386.tex
	${JLATEX} btron386.tex
	${JLATEX} btron386.tex


clean:
	rm -f *.log *.aux *.dvi *.tty *.ps *.toc
