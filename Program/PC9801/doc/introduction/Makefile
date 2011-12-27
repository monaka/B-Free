

all: B-Free-1st.ps B-Free-1st.nup

B-Free-1st.ps: B-Free-1st.dvi
	dvi2ps B-Free-1st.dvi > B-Free-1st.ps

B-Free-1st.nup: B-Free-1st.ps
	nup -2 < B-Free-1st.ps > B-Free-1st.nup
	
B-Free-1st.dvi:
	jlatex B-Free-1st.tex
	jlatex B-Free-1st.tex

make-index:
	makeindex B-Free-1st.tex
