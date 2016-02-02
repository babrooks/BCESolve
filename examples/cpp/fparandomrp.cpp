// FPA with random reserve price

#include "fparandomrp.hpp"
#include "bcegurobisolver.hpp"

int main() {

  int numBids = 50;
  int numVals = 50;
  // if bid < reservePrice, bidder still wins the item with pr reserveProb
  double reservePrice = .5;
  // SOLVER SWITCH
  // CPLEX, set to 0; GUROBI, set to 1;
  int solverSwitch = 1;

  FPARandomRP fparrp(numVals,
		     numBids,
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
  	  << "_rp=" << reservePrice << ".bce";
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

    solver.setParameter(BCEGurobiSolver::BoundaryObjective1,0);
    solver.setParameter(BCEGurobiSolver::BoundaryObjective2,1);

    solver.mapBoundary("fparandomrp_bndry.dat");

    BCESolution data;
    solver.getSolution(data);
  
    stringstream fName;
    fName << "fparandomrp_nv=" << numVals
  	  << "_nb=" << numBids
  	  << "_rp=" << reservePrice << ".bce";
    string fNameStr = fName.str();
    const char * fNameC = fNameStr.c_str();
    BCESolution::save(data,fNameC);


  }

}
