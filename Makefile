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

libDPDFset.so: DPDFset.cc DPDFset.h
	g++ -shared -O2 -g $(CFLAGS)  -I$(LHAINC)  DPDFset.cc   -o $@ -fPIC

testDPDF: testDPDF.cc DPDFset.o 
	g++ -g $(CFLAGS)  -I$(LHAINC) -IstandAlone/zeuspdf/lib  $^  standAlone/dpdf2006.o standAlone/dpdf2007.o   -L$(LHALIBS) -lLHAPDF -Wl,-rpath=$(LHALIBS)   -LstandAlone/zeuspdf/lib  -lzdpdf09    -o $@  -lgfortran

