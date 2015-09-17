#include "allpayauction.hpp"

int main() {

  int numBids = 75;
  int numVals = 75;

  AllPayAuction apa(numVals,numBids);

  BCESolver solver(apa);
  
  solver.populate();
  
  IloCplex cplex = solver.getCplex();

  // // Minimize revenue
  // string objective = "minrev";
  // cplex.getObjective().setSense(IloObjective::Minimize);
  // cplex.getObjective().setExpr(solver.getObjectiveFunction(2));
  
  // Maximize revenue
  string objective = "maxrev";
  cplex.getObjective().setSense(IloObjective::Maximize);
  cplex.getObjective().setExpr(solver.getObjectiveFunction(2));

  solver.solve();

  BCEData data;
  solver.getData(data);
  
  stringstream fName;
  fName << "allpayauction_nv=" << numVals
	<< "_nb=" << numBids
	<< "_objective=" << objective << ".bce";
  string fNameStr = fName.str();
  const char * fNameC = fNameStr.c_str();
  BCEData::save(data,fNameC);

}
