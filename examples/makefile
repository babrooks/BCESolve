# makefile for bce examples
# BAB 11-3-2012

# Directories
include ../localsettings.mk

MAINS=allpayauction commonvalueauction duopoly \
	fangmorris fpaknown fpaunknown \
	hybridentryreserve postedprice spaunknown

all: $(MAINS)

# BCE
$(MAINS): % : $(EXAMPLEDIR)/%.cpp
	$(CXX) $(CFLAGS) -I$(GRBDIR)/include \
	$< -L. $(STATIC) -lbce $(STATIC) -L/usr/lib/x86_64-linux-gnu/ \
	-lboost_serialization -L$(GRBDIR)/lib \
	-lgurobi_c++ \
	$(DYNAMIC) -L$(GRBDIR)/lib \
	-l$(GRBNAME) \
	 $(LDFLAGS) -L./cpp -L../lib -o $@ 

clean:
	rm -rf *.o $(MAINS) $(LIBDIR)/libbce.a 

