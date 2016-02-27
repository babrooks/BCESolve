#include "hybridentryreserve.hpp"

int main() {

  int numBids = 30;
  int numVals = 30;
  double weightOnOwnBid = 1;
  double reservePrice = .25;
  double entryFee = 0;

  HybridEntryReserve hera(numVals,
			  numBids,
			  weightOnOwnBid,
			  reservePrice,
			  entryFee);

  BCESolver solver(hera);
  
  solver.populate();
  
  solver.model.setObjective(solver.getObjectiveFunction(2),GRB_MINIMIZE);  

  solver.solve();

  BCESolution data;
  solver.getSolution(data);
  
  stringstream fName;
  fName << "hybridauction_nv=" << numVals
	<< "_nb=" << numBids
	<< "_w=" << weightOnOwnBid
	<< "_rp=" << reservePrice 
	<< "_ef=" << entryFee << ".bce";
  string fNameStr = fName.str();
  const char * fNameC = fNameStr.c_str();
  BCESolution::save(data,fNameC);

  return 0;

}
