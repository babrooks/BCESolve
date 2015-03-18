# makefile for bce library
# BAB 11-3-2012

CXX=g++

# CPLEX
SYSTEM = x86-64_sles10_4.1
LIBFORMAT = static_pic
CSYSFLAGS  = -fPIC -DIL_STD 
LDSYSFLAGS = 
# Compilation and linking flags enabling Multi-threading
CMTFLAGS = -DILOUSEMT -D_REENTRANT
LDMTFLAGS = -lpthread
# Code optimization/debugging options
# DEBUG = -O1 -DNDEBUG
DEBUG = -g
# Directories
CPLEXDIR=/opt/ibm/ILOG/CPLEX_Studio1251/cplex
CONCERTDIR=/opt/ibm/ILOG/CPLEX_Studio1251/concert
CPLEXNAME=cplex124	

CPPDIR=./cpp
HPPDIR=./hpp
LIBDIR=../lib

# Boost directories
BOOSTDIR=/usr/local/include/boost/


CFLAGS = -std=gnu++11 -I$(CONCERTDIR)/include $(CSYSFLAGS) $(DEBUG) \
	-I$(CPLEXDIR)/include \
	-I$(CONCERTDIR)/include \
	-I$(BOOSTDIR) \
	-I$(HPPDIR) \
	$(CMTFLAGS) \
	$(OPTIONS)  
LDFLAGS = -L$(CPLEXDIR)/lib/$(SYSTEM)/$(LIBFORMAT) \
	-L$(CONCERTDIR)/lib/$(SYSTEM)/$(LIBFORMAT) \
	-lilocplex -lconcert \
	-L$(CPLEXDIR)/bin/$(SYSTEM) \
	-l$(CPLEXNAME)  \
	$(LDSYSFLAGS) \
	$(LDMTFLAGS) \
	$(LDOPTIONS)

VPATH=../lib

all: libbce.a

# BCE
BCEOBJ=bcegame.o bceutilities.o bcesolver.o \
	 bcedata.o bcecounter.o \
	 rca.o

$(BCEOBJ): %.o : $(CPPDIR)/%.cpp $(HPPDIR)/%.hpp $(HPPDIR)/bce.hpp
	$(CXX) $(CFLAGS) $< -c

libbce.a: $(BCEOBJ)
	ar r $(LIBDIR)/libbce.a $(BCEOBJ)
	ranlib $(LIBDIR)/libbce.a

clean:
	rm -rf *.o $(BCEMAINS)

