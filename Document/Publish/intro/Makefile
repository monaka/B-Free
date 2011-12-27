

all: B-Free-1st.ps


intro.html:
	/usr/local/bin/sgml2html intro.sgml

B-Free-1st.ps: B-Free-1st.dvi
	dvips -o B-Free-1st.ps B-Free-1st.dvi

B-Free-1st.nup: B-Free-1st.ps
	nup -2 < B-Free-1st.ps > B-Free-1st.nup

B-Free-1st.dvi:
	platex B-Free-1st.tex
	platex B-Free-1st.tex

make-index:
	makeindex B-Free-1st.tex

clean: 
	/bin/rm -f *~
