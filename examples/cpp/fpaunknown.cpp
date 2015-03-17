// First price auction with common but not perfectly correlated values
// BAB 11-3-2012

#include "fpagame.hpp"

int main(int argc, char ** argv)
{
  double entryCost=0.0;
  double reservePrice=0;
  double minAngleIncrement = 0.025;
  int nb = 50;
  int nv = 2;

  double highbid = 1.0;

  int t, a, ahat;

  char filename[100];
  sprintf(filename,"noinfo_maxp0_nv=%d_nb=%d_entrycost_%1.2f.bce",
	  nv,nb,entryCost);

  cout << "Starting main" << endl;

  FPAGame fpa(nb,nv,entryCost,reservePrice,highbid);
  
  BCESolver solver(fpa);

  solver.setParameter(BCESolver::MinAngleIncrement,minAngleIncrement);

  cout << "Constructor finished" << endl;
  try
    {
      solver.populate();
      cout << "Done populating" << endl;

      IloCplex cplex=solver.getCplex();
      cplex.setParam(IloCplex::Threads,4);

      // // First add efficiency constraint
      // cplex.getObjective().setExpr(solver.getObjectiveFunction(3));
      // solver.solve();
      // cplex.getModel().add(solver.getObjectiveFunction(3) == cplex.getObjValue());

      // Sum of bidders' surplus
      // cplex.getObjective().setExpr(solver.getObjectiveFunction(1)
      // 				   +solver.getObjectiveFunction(0));

      cplex.getObjective().setExpr(-1.0*solver.getObjectiveFunction(2));
      cout << "Objective function set" << endl;
      
      cplex.setParam(IloCplex::BarDisplay,1);
      cplex.setParam(IloCplex::SimDisplay,1);
      // cplex.setParam(IloCplex::RootAlg,IloCplex::Dual);

      solver.solve();
      cout << "Solved" << endl;
      
      cout << "ItLim: " << cplex.getParam(IloCplex::ItLim) << endl;

      cout << "Objective = " << setprecision(16) << cplex.getObjValue() << endl;

      cout << "Bidder 1's surplus: " 
      	   << cplex.getValue(solver.getObjectiveFunction(0)) << endl;
      cout << "Bidder 2's surplus: "
      	   << cplex.getValue(solver.getObjectiveFunction(1)) << endl;
      cout << "Revenue: " 
      	   << cplex.getValue(solver.getObjectiveFunction(2)) << endl;
      cout << "Total surplus: " 
      	   << cplex.getValue(solver.getObjectiveFunction(3)) << endl;

      // BCEData data;
      // solver.getData(data);
      // data.setNumValues(vector<int>(2,fpa.getNumValues()));

      // BCEData::save(data,filename);

      // solver.setParameter(BCESolver::BoundaryObjective1,4);
      // solver.setParameter(BCESolver::BoundaryObjective2,2);
      
      // solver.mapBoundary("fpaunknownbndry.dat");
      // cout << "Mapped boundary" << endl;

      // solver.setParameter(BCESolver::BoundaryObjective1,0);
      // solver.setParameter(BCESolver::BoundaryObjective2,1);
      
      // solver.mapBoundary("fpaunknownbndry_bidder.dat");
      // cout << "Mapped boundary" << endl;


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

