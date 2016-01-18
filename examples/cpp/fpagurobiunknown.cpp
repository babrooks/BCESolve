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

      GRBModel cplex=solver.getModel();

      // // First add efficiency constraint
      // cplex.getObjective().setExpr(solver.getObjectiveFunction(3));
      // solver.solve();
      // cplex.getModel().add(solver.getObjectiveFunction(3) == cplex.getObjValue());

      // Sum of bidders' surplus
      // cplex.getObjective().setExpr(solver.getObjectiveFunction(1)
      // 				   +solver.getObjectiveFunction(0));

      // cplex.getObjective().setExpr(-1.0*solver.getObjectiveFunction(2));
      cout << "Objective function set to min revenue "
	   << "in BCEGurobiSolver source code." << endl;

      // cplex.setParam(IloCplex::RootAlg,IloCplex::Dual);

      solver.solve();
      cout << "Solved" << endl;
      
      // cout << "ItLim: " << cplex.getParam(IloCplex::ItLim) << endl;

      // cout << "Objective = " << setprecision(16) << cplex.getObjValue() << endl;

      cout << "Bidder 1's surplus: " 
      	   << solver.getObjectiveFunction(0).getValue() << endl;
      cout << "Bidder 2's surplus: "
      	   << solver.getObjectiveFunction(1).getValue() << endl;
      cout << "Revenue: " 
      	   << solver.getObjectiveFunction(2).getValue() << endl;
      cout << "Total surplus: " 
      	   << solver.getObjectiveFunction(3).getValue() << endl;

      // solver.setParameter(BCESolver::BoundaryObjective1,4);
      // solver.setParameter(BCESolver::BoundaryObjective2,2);
      
      // solver.mapBoundary("fpaunknownbndry_correlated.dat");
      // cout << "Mapped boundary" << endl;

      // BCEData data;
      // solver.getData(data);
      // data.setNumValues(vector<int>(2,fpa.getNumValues()));

      // BCEData::save(data,filename);

      // Solver.setParameter(BCESolver::BoundaryObjective1,0);
      // solver.setParameter(BCESolver::BoundaryObjective2,1);
      
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

