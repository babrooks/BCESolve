#include "postedprice.hpp"
#include "bcegurobisolver.hpp"

int main() {

  int numVals = 100;
  double price = .05;
  double prPlayer0Wins = .50;

  PostedPrice pp(numVals,price,prPlayer0Wins);
  pp.setHasProductStructureData(false);

  BCEGurobiSolver solver(pp);
  // BCESolver solver(apa);
  
  solver.populate();
  
  GRBModel model = solver.getModel();
  // IloCplex model = solver.getCplex();

  // // CPLEX
  // // Minimize revenue
  // string objective = "minrev";
  // model.getObjective().setSense(IloObjective::Minimize);
  // model.getObjective().setExpr(solver.getObjectiveFunction(2));
  
  // // Maximize revenue
  // string objective = "maxrev";
  // cplex.getObjective().setSense(IloObjective::Minimize);
  // cplex.getObjective().setExpr(solver.getObjectiveFunction(2));


  // GUROBI
  model.setObjective(solver.getObjectiveFunction(2),GRB_MINIMIZE);

  solver.solve();

  BCESolution soln;

  solver.getSolution(soln);

  stringstream fName;
  fName << "postedprice_nv=" << numVals
	<< "_price=" << price
	<< "_pr0Wins=" << prPlayer0Wins << ".bce";
  string fNameStr = fName.str();
  const char * fNameC = fNameStr.c_str();
  BCESolution::save(soln,fNameC);

  return 0;

}

