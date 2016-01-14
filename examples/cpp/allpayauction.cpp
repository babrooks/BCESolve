#include "allpayauction.hpp"
#include "bcegurobisolver.hpp"

int main() {

  int numBids = 50;
  int numVals = 50;

  AllPayAuction apa(numVals,numBids);
  apa.setHasProductStructureData(false);

  BCEGurobiSolver solver(apa);
  
  solver.populate();
  
  GRBModel model = solver.getModel();

  // // Minimize revenue
  // string objective = "minrev";
  // cplex.getObjective().setSense(IloObjective::Minimize);
  // cplex.getObjective().setExpr(solver.getObjectiveFunction(2));
  
  // // Maximize revenue
  // string objective = "maxrev";
  // cplex.getObjective().setSense(IloObjective::Minimize);
  // cplex.getObjective().setExpr(solver.getObjectiveFunction(2));

  BCESolution soln;

  model.setObjective(solver.getObjectiveFunction(2),GRB_MINIMIZE);

  solver.solve();

  solver.getSolution(soln);

  stringstream fName;
  fName << "allpayauction_nv=" << numVals
	<< "_nb=" << numBids
	<< "_objective=" << "Filler, need to get objective" << ".bce";
  string fNameStr = fName.str();
  const char * fNameC = fNameStr.c_str();
  BCESolution::save(soln,fNameC);

  return 0;

}
