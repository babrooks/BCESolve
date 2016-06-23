# makefile for bce library
# BAB 11-3-2012

include ../localsettings.mk

all: libbce.a

# BCE
BCEOBJ=bceabstractgame.o bcegame.o bceutilities.o \
	 bcecounter.o bcesolution.o bcesolver.o

$(BCEOBJ): %.o : $(CPPDIR)/%.cpp $(HPPDIR)/%.hpp $(HPPDIR)/bce.hpp
	$(CXX) $(CFLAGS) $< -c

libbce.a: $(BCEOBJ)
	ar r $(LIBDIR)/libbce.a $(BCEOBJ)
	ranlib $(LIBDIR)/libbce.a

clean:
	rm -rf *.o $(BCEMAINS)

