# makefile for bce examples
# BAB 11-3-2012

# CPLEX

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
include ../localsettings.mk

# Directories

INCLDIR=../src/hpp
HPPDIR=./hpp
CPPDIR=./cpp
LIBDIR=../lib

# Boost directories
BOOSTINCLDIR=/usr/include/boost
BOOSTDIR=/usr/include/boost

CFLAGS =-std=gnu++11 -m64 -O $(CSYSFLAGS) $(DEBUG) \
	-I$(BOOSTDIR) \
	-I$(INCLDIR) \
	-I$(HPPDIR) \
	$(CMTFLAGS) \
	$(OPTIONS)  
LDFLAGS = $(LDSYSFLAGS) \
	-L/usr/local/lib/ $(LDMTFLAGS) \
	$(LDOPTIONS) -ldl 

# CODE REMOVED: -Wl,--no-as-needed

# MAINS=fpaknown fpaunknown fangmorris spa spaknown survey
MAINS=exampleforrobbie \
	exantefee fpaunknown spaunknown \
	fpaunknown_revbounds fpacommon fpaknown_revbounds \
	stationary stationary2 \
	  publicinfo commonfpa \
	 multiplicative survey onesided \
	loadingExample hybridentryreserve \
	hybridauction hybridreserve 

MAINSGRB=revenuelb onesidedrestricted relaxed rcacommon randomrprelaxed optauction
MAINSBCEGRB = surplusbnd allpayauction gurobihybridentryreserve postedprice \
		fpagurobiunknown fparandomrp duopoly fangmorris fpaknown \
		randomwinner commonvalueauction mvsopt \
		guitester

RCAMAINS=rcatest2

all: $(MAINS)

# BCE
libbce.a: 
	make -C ../src

pricediscr: % : $(CPPDIR)/%.cpp
	$(CXX) $(CFLAGS) $< -L. $(LDFLAGS) -L./cpp -o $@

$(MAINS): % : $(CPPDIR)/%.cpp $(INCLDIR)/bce.hpp libbce.a
	$(CXX) $(CFLAGS) $< -L. $(STATIC) -lbce $(STATIC) -L/usr/lib/x86_64-linux-gnu/ \
	 -lboost_serialization \
	 $(DYNAMIC)  $(LDFLAGS) -L./cpp -L../lib -o $@

$(MAINSBCEGRB): % : $(CPPDIR)/%.cpp $(INCLDIR)/bce.hpp libbce.a
	$(CXX) $(CFLAGS) -I$(GRBDIR)/include \
	$< -L. $(STATIC) -lbce $(STATIC) -L/usr/lib/x86_64-linux-gnu/ \
	-lboost_serialization -L$(GRBDIR)/lib \
	-lgurobi_c++ \
	$(DYNAMIC) -L$(GRBDIR)/lib \
	-l$(GRBNAME) \
	 $(LDFLAGS) -L./cpp -L../lib -o $@ 

$(RCAMAINS): % : $(CPPDIR)/%.cpp $(INCLDIR)/bce.hpp  $(INCLDIR)/rca.hpp libbce.a
	$(CXX) $(CFLAGS) $< -L. $(STATIC) -lbce -L/usr/lib/x86_64-linux-gnu/ \
	 -lboost_serialization \
	 $(DYNAMIC) $(LDFLAGS) -L./cpp -L../lib -o $@

rcatest.o: $(CPPDIR)/rcatest.cpp $(INCLDIR)/rca.hpp $(INCLDIR)/bce.hpp
	$(CXX) $(CFLAGS) -I./cpp $(CPPDIR)/rcatest.cpp -c

rcatest: rcatest.o rca.o bce.o
	$(CXX) $(CFLAGS) -L./cpp -I./cpp rcatest.o -lbce -o rcatest $(LDFLAGS)

exampleforpappy: ./cpp/exampleforpappy.cpp
	$(CXX) -I../src/hpp ./cpp/exampleforpappy.cpp -o exampleforpappy

$(MAINSGRB): % : $(CPPDIR)/%.cpp
	$(CXX) -I$(GRBINCLDIR) $(CFLAGS) $< -L. -L./cpp -L$(GRBLIBDIR) -lgurobi_c++ -l$(GRBNAME) -o $@

clean:
	rm -rf *.o $(MAINS) $(LIBDIR)/libbce.a $(MAINSGRB)

