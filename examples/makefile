# makefile for bce examples
# BAB 11-3-2012

CXX=g++

# CPLEX
# SYSTEM = x86-64_linux
SYSTEM=x86-64_sles10_4.1

LIBFORMAT = static_pic
CSYSFLAGS  = -fPIC -DIL_STD 
LDSYSFLAGS = 
# Compilation and linking flags enabling Multi-threading
CMTFLAGS = -DILOUSEMT -D_REENTRANT
LDMTFLAGS = -lpthread
# Code optimization/debugging options
#DEBUG = 
# DEBUG = -O1 -DNDEBUG
DEBUG = -g

# Directories
# CPLEXDIR=/opt/ibm/ILOG/CPLEX_Enterprise_Server1262/CPLEX_Studio/cplex
# CONCERTDIR=/opt/ibm/ILOG/CPLEX_Enterprise_Server1262/CPLEX_Studio/concert
# CPLEXNAME=cplex1262	

CPLEXDIR=/opt/ibm/ILOG/CPLEX_Studio1251/cplex
CONCERTDIR=/opt/ibm/ILOG/CPLEX_Studio1251/concert
CPLEXNAME=cplex1251

# Directories
GRBDIR=/opt/gurobi563/linux64
GRBINCLDIR=$(GRBDIR)/include
GRBLIBDIR=$(GRBDIR)/lib

INCLDIR=../src/hpp
HPPDIR=./hpp
CPPDIR=./cpp
LIBDIR=../lib

# Boost directories
BOOSTINCLDIR=/usr/include/boost
BOOSTDIR=/usr/include/boost

CFLAGS = -std=gnu++11 -I$(CONCERTDIR)/include $(CSYSFLAGS) $(DEBUG) \
	-I$(CPLEXDIR)/include \
	-I$(CONCERTDIR)/include \
	-I$(BOOSTDIR) \
	-I$(INCLDIR) \
	-I$(HPPDIR) \
	$(CMTFLAGS) \
	$(OPTIONS)  
LDFLAGS = -L$(CPLEXDIR)/lib/$(SYSTEM)/$(LIBFORMAT) \
	-L$(CONCERTDIR)/lib/$(SYSTEM)/$(LIBFORMAT) \
	-lilocplex -lconcert \
	-L$(CPLEXDIR)/bin/$(SYSTEM) \
	-l$(CPLEXNAME)  \
	$(LDSYSFLAGS) \
	-L/usr/local/lib/	$(LDMTFLAGS) \
	$(LDOPTIONS) \
	-ldl \

# MAINS=fpaknown fpaunknown fangmorris spa spaknown survey
MAINS=exampleforrobbie \
	exantefee fpaunknown spaunknown \
	fpaunknown_revbounds fpacommon fpaknown_revbounds \
	stationary stationary2 fpaknown \
	 fangmorris publicinfo commonfpa \
	 multiplicative survey duopoly onesided \
	loadingExample \
	hybridauction

MAINSGRB=revenuelb onesidedrestricted surplusbnd relaxed

all: $(MAINS)

# BCE
libbce.a: 
	make -C ../src

pricediscr: % : $(CPPDIR)/%.cpp
	$(CXX) $(CFLAGS) $< -L. $(LDFLAGS) -L./cpp -o $@

$(MAINS): % : $(CPPDIR)/%.cpp $(INCLDIR)/bce.hpp libbce.a
	$(CXX) $(CFLAGS) $< -L. -Bstatic -lbce -Wl,-Bstatic -L/usr/lib/x86_64-linux-gnu/ \
	 -lboost_serialization \
	 -Wl,-Bdynamic  $(LDFLAGS) -L./cpp -L../lib -o $@

rcatest.o: $(CPPDIR)/rcatest.cpp $(CPPDIR)/rca.hpp $(CPPDIR)/bce.hpp
	$(CXX) $(CFLAGS) -I./cpp $(CPPDIR)/rcatest.cpp -c

rcatest: rcatest.o rca.o bce.o
	$(CXX) $(CFLAGS) -L./cpp -I./cpp rcatest.o -lbce -o rcatest $(LDFLAGS)

exampleforpappy: ./cpp/exampleforpappy.cpp
	$(CXX) -I../src/hpp ./cpp/exampleforpappy.cpp -o exampleforpappy

$(MAINSGRB): % : $(CPPDIR)/%.cpp
	$(CXX) -I$(GRBINCLDIR) $(CFLAGS) $< -L. -L./cpp -L$(GRBLIBDIR) -lgurobi_c++ -lgurobi56 -o $@

clean:
	rm -rf *.o $(MAINS) $(LIBDIR)/libbce.a $(MAINSGRB)

