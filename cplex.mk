ifneq "$(wildcard /Users/benjaminbrooks/Applications/IBM/ILOG/CPLEX_Studio1262/cplex/bin/x86-64_osx/cplex )" "" 
 $(info Compiling on Bens macbook pro)
 CPLEXDIR=/Users/benjaminbrooks/Applications/IBM/ILOG/CPLEX_Studio1262/cplex
 CONCERTDIR=/Users/benjaminbrooks/Applications/IBM/ILOG/CPLEX_Studio1262/concert
 CPLEXNAME=cplex1262	
 SYSTEM=x86-64_osx
endif
ifneq "$(wildcard /opt/ibm/ILOG/CPLEX_Studio1251/cplex/bin/x86-64_sles10_4.1/cplex )" "" 
 $(info Compiling on Bens desktop)
 CPLEXDIR=/opt/ibm/ILOG/CPLEX_Studio1251/cplex
 CONCERTDIR=/opt/ibm/ILOG/CPLEX_Studio1251/concert
 CPLEXNAME=cplex1251
 SYSTEM = x86-64_sles10_4.1
endif
ifneq "$(wildcard /opt/ibm/ILOG/CPLEX_Enterprise_Server1262/CPLEX_Studio/cplex/bin/x86-64_linux/cplex )" "" 
 $(info Compiling on Robbie''s laptop)
 CPLEXDIR=/opt/ibm/ILOG/CPLEX_Enterprise_Server1262/CPLEX_Studio/cplex
 CONCERTDIR=/opt/ibm/ILOG/CPLEX_Enterprise_Server1262/CPLEX_Studio/concert
 CPLEXNAME=cplex1262	
 SYSTEM = x86-64_linux
endif
