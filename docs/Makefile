DOCBASE=report
PRESENTATION=visigoth-presentation
GRAPHICS=$(wildcard *.tex)
LATEX?=pdflatex

all: $(GRAPHICS) $(DOCBASE).pdf $(PRESENTATION).pdf pdfclean

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

$(DOCBASE).pdf: $(DOCBASE).tex $(GRAPHICS)
	$(LATEX) $(DOCBASE).tex
	$(LATEX) $(DOCBASE).tex

$(PRESENTATION).pdf: $(PRESENTATION).tex $(GRAPHICS)
	$(LATEX) $(PRESENTATION).tex
	$(LATEX) $(PRESENTATION).tex

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
