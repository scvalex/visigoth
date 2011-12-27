DOCBASE=report
GRAPHICS=

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

$(DOCBASE).pdf: $(DOCBASE).tex $(GRAPHICS)
	pdflatex $(DOCBASE).tex
	pdflatex $(DOCBASE).tex

#$(DOCBASE).ps: $(DOCBASE).tex $(GRAPHICS)
#	latex $(DOCBASE).tex
#	latex $(DOCBASE).tex
#	dvips $(DOCBASE).dvi
#	ps2pdf $(DOCBASE).ps

.PHONY: pdfclean
pdfclean:
	rm -f $(DOCBASE).ps
	rm -f $(DOCBASE).dvi
	rm -f $(DOCBASE).aux
	rm -f $(DOCBASE).out
	rm -f $(DOCBASE).toc
	rm -f $(DOCBASE).log
	rm -f texput.log
	rm -f *~

.PHONY: clean
clean: pdfclean
	rm -f $(DOCBASE).pdf
