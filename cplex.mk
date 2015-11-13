ifneq "$(wildcard /Users/benjaminbrooks/Applications/IBM/ILOG/CPLEX_Studio1262/cplex/bin/x86-64_osx/cplex )" "" 
 $(info Compiling on Bens macbook pro)
 CPLEXDIR=/Users/benjaminbrooks/Applications/IBM/ILOG/CPLEX_Studio1262/cplex
 CONCERTDIR=/Users/benjaminbrooks/Applications/IBM/ILOG/CPLEX_Studio1262/concert
 # CPLEXLIB=-lcplex1262	
 CPLEXLIB=-lcplex
 SYSTEM=x86-64_osx

 GRBDIR=/Library/gurobi604/mac64
 GRBINCLDIR=$(GRBDIR)/include
 GRBLIBDIR=$(GRBDIR)/lib
 GRBNAME=gurobi60

 STATIC=
 DYNAMIC=

 CXX=clang++ -O0

endif
ifneq "$(wildcard /opt/ibm/ILOG/CPLEX_Studio1251/cplex/bin/x86-64_sles10_4.1/cplex )" "" 
 $(info Compiling on Bens desktop)
 CPLEXDIR=/opt/ibm/ILOG/CPLEX_Studio1251/cplex
 CONCERTDIR=/opt/ibm/ILOG/CPLEX_Studio1251/concert
 CPLEXLIB=-lcplex1251
 SYSTEM = x86-64_sles10_4.1

 GRBDIR=/opt/gurobi563/linux64
 GRBINCLDIR=$(GRBDIR)/include
 GRBLIBDIR=$(GRBDIR)/lib
 GRBNAME=gurobi56

 STATIC=-Wl,-Bstatic
 DYNAMIC=-Wl,-Bdynamic

 CXX=g++
endif
ifneq "$(wildcard /opt/ibm/ILOG/CPLEX_Enterprise_Server1262/CPLEX_Studio/cplex/bin/x86-64_linux/cplex )" "" 
 $(info Compiling on Robbies laptop)
 CPLEXDIR=/opt/ibm/ILOG/CPLEX_Enterprise_Server1262/CPLEX_Studio/cplex
 CONCERTDIR=/opt/ibm/ILOG/CPLEX_Enterprise_Server1262/CPLEX_Studio/concert
 CPLEXLIB=-lcplex1262	
 SYSTEM = x86-64_linux

 STATIC=-Wl,-Bstatic
 DYNAMIC=-Wl,-Bdynamic

 CXX=g++
endif
