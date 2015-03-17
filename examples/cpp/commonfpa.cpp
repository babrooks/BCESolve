// Uses the BCE package to solve a first price auction with pure
// common values.
// BAB 11-3-2012

#include "commonfpa.hpp"

int main(int argc, char ** argv)
{
  int objectiveCounter, actionCounter;

  cout << "Starting main" << endl;

  CommonFPA fpa(2,30);
  BCESolver solver(fpa);
  cout << "Constructor finished" << endl;
  
  try
    {
      solver.populate();

      IloCplex cplex=solver.getCplex();
      // cplex.getObjective().setExpr(-solver.getObjectiveFunction(2));
      cplex.getObjective().setExpr(solver.getObjectiveFunction(0)+solver.getObjectiveFunction(1));
      cout << "Objective function set" << endl;
      
      solver.solve();

      BCEData data;
      solver.getData(data);
      vector< vector<double> > deviationValues;
      data.setCurrentEquilibrium(0);
      data.getDeviationObjectives(0,0,0,deviationValues);

      for (actionCounter=0; 
	   actionCounter<deviationValues[0].size();
	   actionCounter++)
	{
	  cout << deviationValues[0][actionCounter] << endl;
	}

      cout << "Done populating" << endl;
    }
  catch (IloException &e)
    {
      cerr << "Concert exception caught: " << e << endl
	   << e.getMessage() << endl;
    }
  catch (BCEException &bceerror)
    {
      cerr << "BCEException caught: " << bceerror.what() << endl;
    }
  catch (...)
    {
      cout << "Unknown exception caught." << endl;
    }
  
} // main
