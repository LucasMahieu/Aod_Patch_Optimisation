CC=gcc
LATEXC=pdflatex
DOCC=doxygen
CFLAGS=-g -Wall 

REFDIR=.
SRCDIR=$(REFDIR)/src
BINDIR=$(REFDIR)/bin
DOCDIR=$(REFDIR)/doc
REPORTDIR=$(REFDIR)/rapport

LATEXSOURCE=$(wildcard $(REPORTDIR)/*.tex)
#CSOURCE=$(wildcard $(SRCDIR)/applyPatch.c)
CSOURCE=$(wildcard $(SRCDIR)/computePatchOpt.c)
#PDF=$(LATEXSOURCE:.tex=.pdf)


#all: binary report doc 
all: binary

#$(BINDIR)/applyPatch: $(CSOURCE)
#	$(CC) $(CFLAGS)  $^ -o $@
$(BINDIR)/computePatchOpt: $(CSOURCE)
	$(CC) $(CFLAGS)  $^ -o $@

#%.pdf: $(LATEXSOURCE)
#	$(LATEXC) -output-directory $(REPORTDIR) $^ 

#$(DOCDIR)/index.html: $(SRCDIR)/Doxyfile $(CSOURCE) 
#	$(DOCC) $(SRCDIR)/Doxyfile

binary: $(BINDIR)/applyPatch
binary: $(BINDIR)/computePatchOpt

#report: $(PDF) 

#doc: $(DOCDIR)/index.html

clean:
	rm -rf $(DOCDIR) $(BINDIR)/* $(REPORTDIR)/*.aux $(REPORTDIR)/*.log  $(REPORTDIR)/rapport.pdf 


.PHONY: all doc binary report 
