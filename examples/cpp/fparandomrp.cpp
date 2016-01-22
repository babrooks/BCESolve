// FPA with random reserve price

#include "fparandomrp.hpp"
#include "bcegurobisolver.hpp"

int main() {

  int numBids = 75;
  int numVals = 75;
  // if bid < reservePrice, bidder still wins the item with pr reserveProb
  double reserveProb = .5;
  double reservePrice = .25;
  // SOLVER SWITCH
  // CPLEX, set to 0; GUROBI, set to 1;
  int solverSwitch = 0;

  FPARandomRP fparrp(numVals,
		     numBids,
		     reserveProb,
		     reservePrice);
  
  if (solverSwitch == 0) {

    BCESolver solver(fparrp);
    solver.populate();

    IloCplex cplex = solver.getCplex();
    cplex.getObjective().setSense(IloObjective::Minimize);
    cplex.getObjective().setExpr(solver.getObjectiveFunction(2));

    solver.solve();

    BCESolution data;
    solver.getSolution(data);
  
    stringstream fName;
    fName << "fparandomrp_nv=" << numVals
  	  << "_nb=" << numBids
  	  << "_rp=" << reservePrice
  	  << "_rpPr=" << reserveProb << ".bce";
    string fNameStr = fName.str();
    const char * fNameC = fNameStr.c_str();
    BCESolution::save(data,fNameC);

  }

  if (solverSwitch == 1) {

    BCEGurobiSolver solver(fparrp);
    solver.populate();

    // GRBLinExpr objective = 0;
    // objective += .5 * solver.getObjectiveFunction(0);
    // objective += .5 * solver.getObjectiveFunction(1);
    // solver.model.setObjective(objective,GRB_MAXIMIZE);
    solver.model.setObjective(solver.getObjectiveFunction(2),GRB_MINIMIZE);  
    solver.model.update();

    solver.solve();

    BCESolution data;
    solver.getSolution(data);
  
    stringstream fName;
    fName << "fparandomrp_nv=" << numVals
  	  << "_nb=" << numBids
  	  << "_rp=" << reservePrice
  	  << "_rpPr=" << reserveProb << ".bce";
    string fNameStr = fName.str();
    const char * fNameC = fNameStr.c_str();
    BCESolution::save(data,fNameC);

  }

}
