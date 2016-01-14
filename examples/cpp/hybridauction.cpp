#include "hybridauction.hpp"

int main()
{
  int numBids = 20;
  int numVals = 20;
  double weightOnOwnBid = 1;
  
  HybridAuction ha(numVals,numBids,weightOnOwnBid);

  BCESolver solver(ha);
  
  solver.populate();
  
  IloCplex cplex = solver.getCplex();
  cplex.getObjective().setSense(IloObjective::Minimize);
  cplex.getObjective().setExpr(solver.getObjectiveFunction(2));
  
  solver.solve();

  BCESolution soln;
  solver.getSolution(soln);
  
  stringstream fName;
  fName << "hybridauction_nv=" << numVals
	<< "_nb=" << numBids
	<< "_w=" << weightOnOwnBid << ".bce";
  string fNameStr = fName.str();
  const char * fNameC = fNameStr.c_str();
  BCESolution::save(soln,fNameC);

  BCEGame game = soln.getGame();
  BCESolver solver2(game);
  solver2.populate();
  
  cplex = solver2.getCplex();
  cplex.getObjective().setSense(IloObjective::Minimize);
  cplex.getObjective().setExpr(solver2.getObjectiveFunction(2));
  
  solver2.solve();


} // main
