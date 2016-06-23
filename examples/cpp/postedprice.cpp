#include "postedprice.hpp"
#include "bcesolver.hpp"

int main() {

  int numVals = 100;
  double price = .05;
  double prPlayer0Wins = .50;

  PostedPrice pp(numVals,price,prPlayer0Wins);
  pp.setHasProductStructureData(false);

  BCESolver solver(pp);
  
  solver.populate();
  
  vector<double> objWeights(3,0);
  objWeights[2]=-1;
  solver.solve(objWeights);

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

