// FPA with random reserve price

#include "fparandomrp.hpp"
#include "bcegurobisolver.hpp"

int main() {

  int numBids = 75;
  int numVals = numBids;
  double ub = 0.33;

  FPARandomRP fparrp(numVals,
		     numBids,
		     ub);

  BCEGurobiSolver solver(fparrp);
  solver.populate();

  solver.model.setObjective(solver.getObjectiveFunction(2),GRB_MINIMIZE);  
  solver.model.update();
  
  solver.solve();

  BCESolution data;
  solver.getSolution(data);
  
  stringstream fName;
  fName << "fparandomrp_nv=" << numVals
	<< "_nb=" << numBids
	<< "_ub=" << ub << ".bce";
  string fNameStr = fName.str();
  const char * fNameC = fNameStr.c_str();
  BCESolution::save(data,fNameC);

}
