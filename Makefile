#LHAPDF includes + libraries
isLHA := $(shell which lhapdf-config > /dev/null; echo $$?)
ifeq ($(isLHA),0)
LHALIBS = $(shell lhapdf-config --libdir)
LHAINC  = $(shell lhapdf-config --incdir)
else
LHAdir=lhapdf/install
LHALIBS = $(LHAdir)/lib
LHAINC  = $(LHAdir)/include
endif

DPDFset.o: DPDFset.cc DPDFset.h
	g++ -c -O2 -g $(CFLAGS)  -I$(LHAINC)  DPDFset.cc   -o $@ 

testDPDF: testDPDF.cc DPDFset.o standAlone/dpdf2006.o standAlone/dpdf2007.o
	g++ -g $(CFLAGS)  -I$(LHAINC) -IstandAlone/zeuspdf/lib  $^   -L$(LHALIBS) -lLHAPDF -Wl,-rpath=$(LHALIBS)   -LstandAlone/zeuspdf/lib  -lzdpdf09    -o $@  -lgfortran

