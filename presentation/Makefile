DOCBASE=visigoth-presentation
GRAPHICS=
LATEX?=pdflatex

all: $(GRAPHICS) $(DOCBASE).pdf pdfclean

%-crop.pdf: %.pdf
	pdfcrop $<

%.pdf: %.eps
	epstopdf --outfile=$@ $@

%.pdf: %.ps
	ps2pdf $< $@

%.eps: %.dia
	dia -t eps -e $@ $<

%.eps: %.dot
	dot $< -Teps -o $@

%.ps: %.dot
	dot $< -Tps2 -o $@

%.ps: %.dvi
	dvips -o $@ $<

#$(DOCBASE).dvi: $(DOCBASE).tex $(GRAPHICS)
#	latex $(DOCBASE).tex
#	latex $(DOCBASE).tex
#	dvips $(DOCBASE).dvi

$(DOCBASE).pdf: $(DOCBASE).tex $(GRAPHICS)
	$(LATEX) $(DOCBASE).tex
	$(LATEX) $(DOCBASE).tex

.PHONY: pdfclean
pdfclean:
	rm -f $(DOCBASE).ps
	rm -f $(DOCBASE).dvi
	rm -f $(DOCBASE).aux
	rm -f $(DOCBASE).out
	rm -f $(DOCBASE).toc
	rm -f $(DOCBASE).nav
	rm -f $(DOCBASE).snm
	rm -f $(DOCBASE).log
	rm -f texput.log
	rm -f *~

.PHONY: clean
clean: pdfclean
	rm -f $(DOCBASE).pdf
