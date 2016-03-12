#include "commonvalueauction.hpp"

int main() {

  double entryCost = 0.0;
  double reservePrice = 0.0;
  int nv = 30;
  int nbids = 30;
  double highBid = 1;
  double alpha = 1;

  CommonValueAuction cva(nbids,nv,alpha,highBid,
			 reservePrice,entryCost);

  BCESolver solver(cva);

  solver.populate();

  solver.model.setObjective(solver.getObjectiveFunction(2),
			    GRB_MINIMIZE);

  solver.solve();

  BCESolution soln;
  solver.getSolution(soln);

  stringstream fName;
  fName << "cvauction_nv=" << nv
	<< "_nb=" << nbids
	<< "_ec=" << entryCost
	<< "_rp=" << reservePrice
	<< "_hb=" << highBid 
	<< "_alpha=" << alpha << ".bce";
  string fNameStr = fName.str();
  const char * fNameC = fNameStr.c_str();

  BCESolution::save(soln,fNameC);

  return 0;
}
