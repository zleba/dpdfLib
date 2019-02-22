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

DPDFset.o: DPDFset.cc
	g++ -c $(CFLAGS)  -I$(LHAINC)  $^   -o $@ 

testDPDF: testDPDF.cc DPDFset.o
	g++  $(CFLAGS)  -I$(LHAINC)  $^   -L$(LHALIBS) -lLHAPDF -Wl,-rpath=$(LHALIBS)   -o $@ 

