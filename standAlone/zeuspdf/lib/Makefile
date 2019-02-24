ifeq ($(OS),)
OS := $(shell uname)
endif

ifeq ($(OS),Linux)
CP = cp -p -f
MYLIBDIR := $(HOME)/data/lib
ifeq ($(HOST),ztcat1)
FC = gfortran
endif
else
CC = gcc
FC = g77
CP = cp.bat
MYLIBDIR := C:/lib
endif
RM = rm -f
MKDIR = mkdir -p -v
SAY = @echo

PRJ = zdpdf09
CSRC = qgrid_base.cc xgrid_base.cc zdpdf_io.cc zdpdf.cc gZDPDF.cc \
  pion/grvpi.c emsg.cc tblipol.cc GrdHeader.cc xdr2cc.cc
FSRC = pion/grvpilo.f pion/grvpiho.f
OBJ := $(CSRC:.c=.o)
OBJ := $(OBJ:.cc=.o)
OBJ += $(FSRC:.f=.o)

MYINCDIR := $(MYLIBDIR)/inc

#--- place debug info
#DEBINFO = -D__DEBUG__
DEBINFO =

BIN  := lib$(PRJ).a
CXXFLAGS = -fexceptions -g3 $(DEBINFO) $(CXXINCS) $(DEFS)
CFLAGS = $(CXXFLAGS)
FFLAGS = -g3 $(DEBINFO)
LINKOBJ  = $(OBJ)
LIBS += -lstdc++ $(CERNLIBS)
.PHONY: all all-before all-after clean cleandep test

all: all-before $(BIN) all-after

test:
	$(SAY) "CC = $(CC)"
	$(SAY) "CXX = $(CXX)"

all-before:
	$(SAY) "------------------------------------------------------"
	$(SAY) "In order to rebuild dependencies run"
	$(SAY) "'make cleandep' first."
	$(SAY) "------------------------------------------------------"

cleandep:
	$(RM) $(OBJ:.o=.d)

%.d: %.c
	$(CC) -MM $(CFLAGS) $< > $@

%.d: %.cc
	$(CC) -MM $(CFLAGS) $< > $@

clean: cleandep
	${RM} $(OBJ) $(BIN)

DEPS := $(CSRC:.c=.d)
DEPS := $(DEPS:.cc=.d)
-include $(DEPS)

$(BIN): $(OBJ)
	$(AR) -r $(BIN) $(OBJ)
	ranlib $(BIN)

