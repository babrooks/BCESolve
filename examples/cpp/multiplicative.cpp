// Multiplicative first price auction model
// BAB 6-11-2013

#include "multiplicative.hpp"

void solve(double cV, int numRatios, 
	   double cS, int numShades);

int main(int argc, char ** argv)
{
  int numRatios=40;
  double cV = 1.02;
  double cS = 1.01;
  int numShades = 140;

  solve(cV,numRatios,cS,numShades);
  return 0;

}

void solve(double cV, int numRatios, 
	   double cS, int numShades)
{
  double minAngleIncrement = 0.05;
  double prob;
  bool verbose = true;
  
  if (verbose)
    cout << "Starting main" << endl;

  try
    {
      Multiplicative model(cV,numRatios,cS,numShades);

      BCESolver solver(model);

      solver.setParameter(BCESolver::MinAngleIncrement,minAngleIncrement);
      solver.setParameter(BCESolver::DisplayLevel,1);

      if (verbose)
	cout << "Constructor finished" << endl;

      solver.populate();
      if (verbose)
	cout << "Done populating" << endl;

      char filename[100];
      sprintf(filename,"multiplicative_nR=%d_cV=%1.2f_cS=%1.2f_minrev_barrier.bce",
	      numRatios,cV,cS);

      IloCplex cplex = solver.getCplex();

      cplex.setParam(IloCplex::SimDisplay,0);
      solver.setParameter(BCESolver::DisplayLevel,1);

      cplex.getObjective().setExpr(solver.getObjectiveFunction(1)
      				   +solver.getObjectiveFunction(0)); // revenue
      if (verbose)
	cout << "Objective function set" << endl;
      solver.solve();

      cout << "Player 0's payoff = " << cplex.getValue(solver.getObjectiveFunction(0)) << endl;
      cout << "Player 1's payoff = " << cplex.getValue(solver.getObjectiveFunction(1)) << endl;


      // solver.mapBoundary();

      BCEData data;
      solver.getData(data);

      if (verbose)
	cout << "Saving data..." << endl;

      BCEData::save(data,filename);

      
      BCEData data2;
      BCEData::load(data2,filename);
      cout << "Data2 loaded" << endl;
    }
  catch (IloException & e)
    {
      cerr << "Concert exception caught: " << e << endl
	   << e.getMessage() << endl;
    }
  catch (BCEException & err)
    {
      cerr << "BCEException caught." << endl;
    }
  catch (exception & err)
    {
      cerr << "Standard exception caught: " << err.what() << endl;
    }
  catch (...)
    {
      cout << "Unknown exception caught." << endl;
    }

}
