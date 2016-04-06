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