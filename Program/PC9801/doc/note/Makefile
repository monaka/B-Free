
TEXINPUTS=$TEXINPUTS:../style

.suffixes:	.ps.dvi.tex

.ps.dvi:
	dvi2ps $*.dvi > $*.ps

.dvi.tex:
	jlatex $*.tex



all:


clean:
	rm -f  *.dvi *.ps *.aux *.log *.toc *~
