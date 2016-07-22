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
	$(RMCMD) *.o $(BCEMAINS)

