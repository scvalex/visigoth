DOCBASE=report
PRESENTATION=presentation
EXECSUMMARY=execsummary
GRAPHICS=
LATEX?=pdflatex

all: $(GRAPHICS) $(DOCBASE).pdf $(PRESENTATION).pdf $(EXECSUMMARY).pdf pdfclean

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

$(EXECSUMMARY).pdf: $(EXECSUMMARY).tex $(GRAPHICS)
	$(LATEX) $(EXECSUMMARY).tex
	$(LATEX) $(EXECSUMMARY).tex

.PHONY: pdfclean
pdfclean:
	$(foreach f,$(DOCBASE) $(PRESENTATION) $(EXECSUMMARY),\
	rm -f $(f).ps &&\
	rm -f $(f).dvi &&\
	rm -f $(f).aux &&\
	rm -f $(f).out &&\
	rm -f $(f).toc &&\
	rm -f $(f).nav &&\
	rm -f $(f).snm &&\
	rm -f $(f).log)
	rm -f texput.log
	rm -f *~

.PHONY: clean
clean: pdfclean
	rm -f $(DOCBASE).pdf
