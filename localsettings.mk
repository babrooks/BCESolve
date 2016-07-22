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

BOOSTINCLDIR=/usr/include/boost
BOOSTDIR=/usr/include/boost

ifneq "$(wildcard /Users/benjaminbrooks/Applications/IBM/ILOG/CPLEX_Studio1262/cplex/bin/x86-64_osx/cplex )" "" 
 $(info Compiling on Bens macbook pro)

 GRBDIR=/Library/gurobi650/mac64
 GRBINCLDIR=$(GRBDIR)/include
 GRBLIBDIR=$(GRBDIR)/lib
 GRBNAME=gurobi65

 STATIC=
 DYNAMIC=

 CXX=clang++ -O0

 RMCMD=rm -rf 
endif
ifneq "$(wildcard /opt/ibm/ILOG/CPLEX_Studio1251/cplex/bin/x86-64_sles10_4.1/cplex )" "" 
 $(info Compiling on Bens desktop)

 GRBDIR=/opt/gurobi563/linux64
 GRBINCLDIR=$(GRBDIR)/include
 GRBLIBDIR=$(GRBDIR)/lib
 GRBNAME=gurobi56

 STATIC=-Wl,-Bstatic
 DYNAMIC=-Wl,-Bdynamic

 CXX=g++
 RMCMD=rm -rf 
endif
ifneq "$(wildcard C:\Users\babrooks\ )" "" 
 $(info Compiling on Bens windows laptop)

 GRBDIR=C:\gurobi652\win64
 GRBINCLDIR=$(GRBDIR)\include
 GRBLIBDIR=$(GRBDIR)\lib
 GRBNAME=gurobi65

 BOOSTINCLDIR="C:/Program Files/boost/boost_1_60_0/boost/serialization/"
 BOOSTDIR="C:/Program Files/boost/boost_1_60_0/"

 STATIC=-Wl,-Bstatic
 DYNAMIC=-Wl,-Bdynamic

 CXX=g++
 RMCMD=del 

endif
ifneq "$(wildcard /opt/ibm/ILOG/CPLEX_Enterprise_Server1262/CPLEX_Studio/cplex/bin/x86-64_linux/cplex )" ""
 $(info Compiling on Robbies laptop)

 GRBDIR=/opt/gurobi650/linux64
 GRBNAME=gurobi65

 STATIC=-Wl,-Bstatic
 DYNAMIC=-Wl,-Bdynamic

 CXX=g++
 RMCMD=rm -rf 
endif

# General settings
# DEBUG=-g
DEBUG=-DNDEBUG -O

CSYSFLAGS = -fPIC -std=gnu++11
LDMTFLAGS = -lpthread -lm

CPPDIR=../src/cpp
HPPDIR=../src/hpp
EXAMPLEDIR=./cpp
LIBDIR=../lib

# Boost directories
BOOSTINCLDIR=/usr/include/boost
BOOSTDIR=/usr/include/boost

CFLAGS = $(CSYSFLAGS) $(DEBUG) \
	-I$(BOOSTDIR) \
	-I$(HPPDIR) \
	-I./hpp/ \
	-I$(GRBDIR)/include \
	-I$(BOOSTINCLDIR)

LDFLAGS = -L$(GRBDIR)/lib \
	-lgurobi_c++ -l$(GRBNAME) \
	$(LDSYSFLAGS) \
	$(LDMTFLAGS) \
	$(LDOPTIONS) \
	-ldl
