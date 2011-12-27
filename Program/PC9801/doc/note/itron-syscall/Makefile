

all: syscall.ps

syscall.ps: syscall.dvi
	dvi2ps syscall.dvi > syscall.ps

syscall.dvi: syscall.tex
	jlatex syscall.tex
