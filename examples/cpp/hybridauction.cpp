#include "hybridauction.hpp"

int main()
{
  int numBids = 20;
  int numVals = 20;
  double weightOnOwnBid = 0.1;
  
  HybridAuction ha(numVals,numBids,weightOnOwnBid);

  BCESolver solver(ha);
  
  solver.populate();
  
  IloCplex cplex = solver.getCplex();
  cplex.getObjective().setSense(IloObjective::Minimize);
  cplex.getObjective().setExpr(solver.getObjectiveFunction(2));
  
  solver.solve();

  BCEData data;
  solver.getData(data);
  
  stringstream fName;
  fName << "hybridauction_nv=" << numVals
	<< "_nb=" << numBids
	<< "_w=" << weightOnOwnBid << ".bce";
  string fNameStr = fName.str();
  const char * fNameC = fNameStr.c_str();
  BCEData::save(data,fNameC);
  
} // main
