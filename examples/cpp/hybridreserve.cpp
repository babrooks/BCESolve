#include "hybridreserve.hpp"

int main()
{
  int numBids = 75;
  int numVals = 75;
  double weightOnOwnBid = 1;
  double reservePrice = .3;

  HybridReserveAuction hra(numVals,numBids,weightOnOwnBid,reservePrice);

  BCESolver solver(hra);
  
  solver.populate();
  
  // Minimize Revenue
  IloCplex cplex = solver.getCplex();
  cplex.getObjective().setSense(IloObjective::Minimize);
  cplex.getObjective().setExpr(solver.getObjectiveFunction(2));
  
  solver.solve();

  BCEData data;
  solver.getData(data);
  
  stringstream fName;
  fName << "hybridauction_nv=" << numVals
	<< "_nb=" << numBids
	<< "_w=" << weightOnOwnBid
	<< "_rp=" << reservePrice << ".bce";
  string fNameStr = fName.str();
  const char * fNameC = fNameStr.c_str();
  BCEData::save(data,fNameC);
  
} // main
