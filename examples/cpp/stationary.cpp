// Stationary first price auction model
// BAB 6-11-2013

#include "stationary.hpp"

void solve(int k);

int main(int argc, char ** argv)
{
  int k=5;

  solve(k);
  return 0;

}

void solve(int k)
{
  double minAngleIncrement = 0.05;

  bool verbose = true;
  
  if (verbose)
    cout << "Starting main" << endl;

  try
    {
      Stationary model(k);

      BCESolver solver(model);

      solver.setParameter(BCESolver::MinAngleIncrement,minAngleIncrement);
      solver.setParameter(BCESolver::DisplayLevel,1);

      double prob = 0.0;
      for (int state = 0; state < 2*k+1; state++)
	{
	  prob += model.prior(state,vector<int>(2,0));
	}
      cout << "Total prob = " << prob << endl;

      if (verbose)
	cout << "Constructor finished" << endl;

      solver.populate();
      if (verbose)
	cout << "Done populating" << endl;

      char filename[100];
      sprintf(filename,"stationary_normal_k=%d_minrev_barrier.bce",k);
      // sprintf(filename,"stationary_k=%d_maxrev_barrier.bce",k);

      IloCplex cplex = solver.getCplex();

      cplex.setParam(IloCplex::RootAlg,IloCplex::Barrier);
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

      // Retrieve marginal distribution over winning recommendations. 
      vector<double> distr;
      vector<int> stateConditions(k,0); 
      int state;
      for (state=0; state < k; state++)
	stateConditions[state]=k+1+state;
      vector<bool> actionMarginal(2,false); actionMarginal[0] = true;

      data.getConditionalMarginal(stateConditions,vector< vector<int> >(2),
				  vector< vector<int> >(2),
				  false,actionMarginal,vector<bool>(2,false),
				  distr);
      
      double expectation = 0.0;
      prob = 0.0;
      for (state = 0; state < distr.size(); state++)
	{
	  cout << state << " " << distr[state] << endl;
	  expectation += distr[state]*state;
	  prob += distr[state];
	}
      cout << "Expectation = " << expectation << ", prob sum = " << prob << endl;

      BCEData data2;
      BCEData::load(data2,filename);
      cout << "Data2 loaded successfully" << endl;
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
      cerr << "Standard exception caught" << endl;
    }
  catch (...)
    {
      cout << "Unknown exception caught." << endl;
    }

}
