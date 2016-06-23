#include "hybridentryreserve.hpp"

int main() {

  int numBids = 30;
  int numVals = 30;
  double weightOnOwnBid = 1;
  double reservePrice = .25;
  double entryFee = 0.0;
  double highBid = 1.0;

  HybridEntryReserve hera(numVals,
			  numBids,
			  weightOnOwnBid,
			  reservePrice,
			  entryFee,
			  highBid);

  BCESolver solver(hera);
  
  solver.populate();
  
  // Minimize Revenue
  vector<double> objWeights(3,0);
  objWeights[2]=-1;
  solver.solve(objWeights);

  BCESolution data;
  solver.getSolution(data);
  
  cout << "Objective 0: " << solver.getObjectiveFunction(0).getValue() << endl;
  cout << "Objective 1: " << solver.getObjectiveFunction(1).getValue() << endl;
  cout << "Objective 2: " << solver.getObjectiveFunction(2).getValue() << endl;

  stringstream fName;
  fName << "hybridauction_nv=" << numVals
	<< "_nb=" << numBids
	<< "_w=" << weightOnOwnBid
	<< "_rp=" << reservePrice 
	<< "_ef=" << entryFee << ".bce";
  string fNameStr = fName.str();
  const char * fNameC = fNameStr.c_str();
  BCESolution::save(data,fNameC);

}
