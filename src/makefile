# makefile for bce library
# BAB 11-3-2012

CXX=g++

# CPLEX
SYSTEM = x86-64_linux
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
# CPLEXDIR=/Users/benjaminbrooks/Applications/IBM/ILOG/CPLEX_Studio1262/cplex
# CONCERTDIR=/Users/benjaminbrooks/Applications/IBM/ILOG/CPLEX_Studio1262/concert
# CPLEXDIR=/opt/ibm/ILOG/CPLEX_Enterprise_Server1262/CPLEX_Studio/cplex
# CONCERTDIR=/opt/ibm/ILOG/CPLEX_Enterprise_Server1262/CPLEX_Studio/concert
# CPLEXNAME=cplex1262	

CPLEXDIR=/opt/ibm/ILOG/CPLEX_Studio1251/cplex
CONCERTDIR=/opt/ibm/ILOG/CPLEX_Studio1251/concert
CPLEXNAME=cplex1251

CPPDIR=./cpp
HPPDIR=./hpp
LIBDIR=../lib

# Boost directories
BOOSTINCLDIR=/usr/include/boost
BOOSTDIR=/usr/include/boost


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
	$(LDOPTIONS) \
	-ldl

VPATH=../lib

all: libbce.a

# BCE
BCEOBJ=bceabstractgame.o bcegame.o bceutilities.o bcesolver.o \
	 bcedata.o bcecounter.o bcesolution.o \
	 rca.o

$(BCEOBJ): %.o : $(CPPDIR)/%.cpp $(HPPDIR)/%.hpp $(HPPDIR)/bce.hpp
	$(CXX) $(CFLAGS) $< -c

libbce.a: $(BCEOBJ)
	ar r $(LIBDIR)/libbce.a $(BCEOBJ)
	ranlib $(LIBDIR)/libbce.a

clean:
	rm -rf *.o $(BCEMAINS)

