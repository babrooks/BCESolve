exists(/Users/benjaminbrooks/Applications/IBM/ILOG/CPLEX_Studio1262/cplex/bin/x86-64_osx/cplex) {
 message("QMAKE run on Ben's macbook pro...")
LIBS += -L/opt/gurobi563/linux64/lib -lgurobi_c++ -lgurobi56
LIBS += -L/opt/ibm/ILOG/CPLEX_Studio1251/cplex/lib/x86-64_sles10_4.1/static_pic \
-L/opt/ibm/ILOG/CPLEX_Studio1251/concert/lib/x86-64_sles10_4.1/static_pic \
-lilocplex -lconcert -lcplex -L/opt/ibm/ILOG/CPLEX_Studio1251/cplex/bin/x86-64_sles10_4.1/ -lcplex1251
}

exists(/opt/ibm/ILOG/CPLEX_Studio1251/cplex/bin/x86-64_sles10_4.1/cplex) {
 message("QMAKE run on Ben's desktop...")
LIBS += -L/opt/gurobi563/linux64/lib -lgurobi_c++ -lgurobi56
LIBS += -L/opt/ibm/ILOG/CPLEX_Studio1251/cplex/lib/x86-64_sles10_4.1/static_pic \
-L/opt/ibm/ILOG/CPLEX_Studio1251/concert/lib/x86-64_sles10_4.1/static_pic \
-lilocplex -lconcert -L/opt/ibm/ILOG/CPLEX_Studio1251/cplex/bin/x86-64_sles10_4.1/ -lcplex1251
}

exists(/opt/ibm/ILOG/CPLEX_Enterprise_Server1262/CPLEX_Studio/cplex/bin/x86-64_linux/cplex) {
 message("QMAKE run on Robbie's laptop...")
LIBS += -L/opt/ibm/ILOG/CPLEX_Enterprise_Server1262/CPLEX_Studio/cplex/lib/x86-64_linux/static_pic -L/opt/ibm/ILOG/CPLEX_Enterprise_Server1262/CPLEX_Studio/concert/lib/x86-64_linux/static_pic -lilocplex -lconcert -L/opt/ibm/ILOG/CPLEX_Enterprise_Server1262/CPLEX_Studio/cplex/bin/x86-64_linux -lcplex1262
LIBS += -L/opt/gurobi650/linux64/lib -lgurobi_c++ -lgurobi65
}
