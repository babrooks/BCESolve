ifneq "$(wildcard /Users/benjaminbrooks/Applications/IBM/ILOG/CPLEX_Studio1262/cplex/bin/x86-64_osx/cplex )" "" 
 $(info Compiling on Bens macbook pro)
 SYSTEM=x86-64_osx

 GRBDIR=/Library/gurobi650/mac64
 GRBINCLDIR=$(GRBDIR)/include
 GRBLIBDIR=$(GRBDIR)/lib
 GRBNAME=gurobi65

 STATIC=
 DYNAMIC=

 CXX=clang++ -O0

endif
ifneq "$(wildcard /opt/ibm/ILOG/CPLEX_Studio1251/cplex/bin/x86-64_sles10_4.1/cplex )" "" 
 $(info Compiling on Bens desktop)
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
 SYSTEM = x86-64_linux

 GRBDIR=/opt/gurobi650/linux64
 GRBNAME=gurobi65

 STATIC=-Wl,-Bstatic
 DYNAMIC=-Wl,-Bdynamic

 CXX=g++
endif
