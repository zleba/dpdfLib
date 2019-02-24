ifeq ($(OS),)
OS := $(shell uname)
endif

ifeq ($(OS),Linux)
CP = cp -p -f
else
FC = g77
CP = cp.bat
endif
CC = gcc
RM = rm -f
MKDIR = mkdir -p -v
SAY = @echo

PRJ = gfrac
CSRC = main.cc oFDstream.cc
FSRC =
OBJ := $(CSRC:.c=.o)
OBJ := $(OBJ:.cc=.o)
OBJ += $(FSRC:.f=.o)

#--- place debug info
#DEBINFO = -D__DEBUG__
DEBINFO =

CXXINCS += -I . -I ../lib
BIN  := $(PRJ).exe
CXXFLAGS = -fexceptions -g3 $(DEBINFO) $(CXXINCS)  
CFLAGS = $(CXXFLAGS)
FFLAGS = -g3 $(DEBINFO)
LINKOBJ  = $(OBJ)
LIBS =  -L ../lib -lzdpdf09
ifneq ($(strip $(filter Windows%,$(OS))),)
LIBS +=  -Lc:/lib -lxdr
endif
.PHONY: all all-before all-after clean cleandep

all: all-before $(BIN) all-after

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
	$(CXX) $(LINKOBJ) -o $(BIN) $(LIBS)

