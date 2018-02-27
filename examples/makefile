# This file is part of the BCESolve library for games of incomplete
# information
# Copyright (C) 2016 Benjamin A. Brooks and Robert J. Minton
#
# BCESolve free software: you can redistribute it and/or modify it
# under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# BCESolve is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see
# <http://www.gnu.org/licenses/>.
#
# Benjamin A. Brooks
# ben@benjaminbrooks.net
# Chicago, IL

# makefile for bce examples
# BAB 11-3-2012

# Directories
include ../localsettings.mk

MAINS=allpayauction commonvalueauction duopoly \
	fangmorris fpaknown fpaunknown \
	hybridentryreserve postedprice spaunknown \
	smoothauction maxminconj maxminconj2 maxminconj3 maxminmech resale songzi

all: $(MAINS)

$(MAINS): % : $(EXAMPLEDIR)/%.cpp
	$(CXX) $(CFLAGS) -I$(GRBDIR)/include \
	$< -L. $(STATIC) -lbce $(STATIC) -L/usr/lib/x86_64-linux-gnu/ \
	-L$(BOOSTDIR) -lboost_serialization -L$(GRBDIR)/lib \
	-lgurobi_c++ \
	$(DYNAMIC) -L$(GRBDIR)/lib \
	-l$(GRBNAME) \
	 $(LDFLAGS) -L./cpp -L../lib -o $@ 

clean:
	$(RMCMD) *.o $(MAINS)
	make -C ../lib clean

