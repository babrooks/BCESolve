// First price auction with common but not perfectly correlated values
// BAB 11-3-2012

#include "fpagame.hpp"
#include "bcegurobisolver.hpp"

int main(int argc, char ** argv)
{
  double entryCost=0.0;
  double reservePrice=0;
  double minAngleIncrement = 0.025;
  int nb = 10;
  int nv = 5;

  double highbid = 1.0;

  int t, a, ahat;

  cout << "Starting main" << endl;

  FPAGame fpa(nb,nv,entryCost,reservePrice,highbid,entryCost);
  
  BCEGurobiSolver solver(fpa);

  solver.setParameter(BCEGurobiSolver::MinAngleIncrement,minAngleIncrement);

  cout << "Constructor finished" << endl;
  try
    {
      solver.populate();
      cout << "Done populating" << endl;

      // // First add efficiency constraint
      // cplex.getObjective().setExpr(solver.getObjectiveFunction(3));
      // solver.solve();

      solver.model.setObjective(.5*solver.getObjectiveFunction(0) +
      			 .5*solver.getObjectiveFunction(1),GRB_MAXIMIZE);
      solver.model.update();

      solver.solve();
      cout << "Solved" << endl;

      cout << "Bidder 1's surplus: " 
      	   << solver.getObjectiveFunction(0).getValue() << endl;
      cout << "Bidder 2's surplus: "
      	   << solver.getObjectiveFunction(1).getValue() << endl;
      cout << "Revenue: " 
      	   << solver.getObjectiveFunction(2).getValue() << endl;
      cout << "Total surplus: " 
      	   << solver.getObjectiveFunction(3).getValue() << endl;

      // solver.setParameter(BCEGurobiSolver::BoundaryObjective1,4);
      // solver.setParameter(BCEGurobiSolver::BoundaryObjective2,2);
      
      // solver.mapBoundary("fpaunknownbndry_correlated.dat");
      // cout << "Mapped boundary" << endl;

      // solver.setParameter(BCEGurobiSolver::BoundaryObjective1,0);
      // solver.setParameter(BCEGurobiSolver::BoundaryObjective2,1);
      
      // solver.mapBoundary("fpaunknownbndry_bidder.dat");
      // cout << "Mapped boundary" << endl;

      BCESolution soln;

      solver.getSolution(soln);

      stringstream fName;
      fName << "fpaunknown_nv=" << nv
	    << "_nb=" << nb
	    << "_rp=" << reservePrice
	    << "_ec=" << entryCost
	    << "SolnRev=" << solver.getObjectiveFunction(2).getValue() << ".bce";
      string fNameStr = fName.str();
      const char * fNameC = fNameStr.c_str();
      BCESolution::save(soln,fNameC);

    }
  catch (IloException & e)
    {
      cerr << "Concert exception caught: " << e << endl
	   << e.getMessage() << endl;
    }
  catch (BCEException & bcee)
    {
      cerr << "BCEException caught: " << endl;
      if (bcee.errorType == BCEException::NotProbDistr)
	cerr << "Not a probability distribution" << endl;
    }
  catch (...)
    {
      cout << "Unknown exception caught." << endl;
    }

  return 0;
}

