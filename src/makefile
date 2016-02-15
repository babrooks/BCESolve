# makefile for bce library
# BAB 11-3-2012

CXX=g++

# CPLEX
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
include ../cplex.mk

CPPDIR=./cpp
HPPDIR=./hpp
LIBDIR=../lib

# Boost directories
BOOSTINCLDIR=/usr/include/boost
BOOSTDIR=/usr/include/boost

# GRBDIR=/opt/gurobi650/linux64
# GRBNAME=gurobi65

CFLAGS = -std=gnu++11 $(CSYSFLAGS) $(DEBUG) \
	-I$(GRBDIR)/include \
	-I$(BOOSTDIR) \
	-I$(HPPDIR) \
	$(CMTFLAGS) \
	$(OPTIONS)  
LDFLAGS = -L$(GRBDIR)/lib \
	-lgurobi_c++ -l$(GRBNAME) \
	$(LDSYSFLAGS) \
	$(LDMTFLAGS) \
	$(LDOPTIONS) \
	-ldl

VPATH=../lib

all: libbce.a

# BCE
BCEOBJ=bceabstractgame.o bcegame.o bceutilities.o \
	 bcecounter.o bcesolution.o bcegurobisolver.o #rca.o

$(BCEOBJ): %.o : $(CPPDIR)/%.cpp $(HPPDIR)/%.hpp $(HPPDIR)/bce.hpp
	$(CXX) $(CFLAGS) $< -c

libbce.a: $(BCEOBJ)
	ar r $(LIBDIR)/libbce.a $(BCEOBJ)
	ranlib $(LIBDIR)/libbce.a

clean:
	rm -rf *.o $(BCEMAINS)

