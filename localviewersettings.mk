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

exists(/opt/ibm/ILOG/CPLEX_Studio1251/cplex/bin/x86-64_sles10_4.1/cplex) {
message("QMAKE run on Ben's desktop...")

GRBDIR=/opt/gurobi563/linux64
GRBNAME=gurobi56
}

# Robbie's computer
exists(/opt/ibm/ILOG/CPLEX_Enterprise_Server1262/CPLEX_Studio/cplex/bin/x86-64_linux/cplex) {
message("QMAKE run on Robbie's laptop...")

GRBDIR=/opt/gurobi650/linux64
GRBNAME=gurobi65
} # Robbie's computer

exists( /Users/benjaminbrooks/Applications/IBM/ILOG/CPLEX_Studio1262/cplex/bin/x86-64_osx/cplex ) {
message("QMAKE run on Ben's macbook pro...")

GRBNAME=gurobi65
GRBDIR=/Library/gurobi650/mac64

} # Ben's macbook pro

LIBS += -L$${GRBDIR}/lib -lgurobi_c++ -l$${GRBNAME}
INCLUDEPATH += $${GRBDIR}/include
VPATH += $${GRBDIR}/include
