// Fang-Morris example
// BAB 11-3-2012
#include "fangmorris.hpp"
#include <vector>

void solveModel (double signalAccuracy);

int main(int argc, char ** argv)
{
  vector<double> accuracies (8,0.0);
  accuracies[0] = 0.5;
  accuracies[1] = 0.65;
  accuracies[2] = 0.8;
  accuracies[3] = 0.9;
  accuracies[4] = 0.95;
  accuracies[5] = 0.98;
  accuracies[6] = 0.995;
  accuracies[7] = 0.9999;

  for (double signalAccuracy: accuracies)
    solveModel(signalAccuracy);

  return 0;
}

void solveModel (double signalAccuracy) 
{
  int t, a, ahat;
  double entryCost=0.00;
  double minAngleIncrement = 0.05;
  int nbids=35;

  cout << "Signal strength = " << signalAccuracy << endl;

  cout << "Starting main" << endl;

  FangMorris fpa(nbids,entryCost,signalAccuracy);
  BCEGurobiSolver solver(fpa);

  solver.setParameter(BCESolver::MinAngleIncrement,minAngleIncrement);

  cout << "Constructor finished" << endl;
  try
    {
      solver.populate();
      cout << "Done populating" << endl;

      char filename[100];
      sprintf(filename,"fangmorris_nb=%d_sig=%1.2f.bce",nbids,signalAccuracy);
      // sprintf(filename,"fangmorris_-p1+p0_nb=%d_entrycost_%1.2f_signalacc_%1.2f.bce",
      // 	      nbids,entryCost,signalAccuracy);

      model.setObjective(-solver.getObjectiveFunction(1)
			 +solver.getObjectiveFunction(0),GRB_MAXIMIZE);
      cout << "Objective function set" << endl;
      
      // solver.solve();
      // cout << "Solved" << endl;

      // cout << "Objective = " << setprecision(16) << cplex.getObjValue() << endl;

      solver.mapBoundary();

      BCESolution data;
      solver.getSolution(data);
      // data.setNumValues(vector<int>(2,2));

      // vector<int> sortObj(2,0);
      // sortObj[1]=1;
      // data.sortEquilibria(sortObj);

      BCESolution::save(data,filename);

      // vector< vector<int> > actionConditions(2,vector<int>(0));
      // vector< vector<int> > typeConditions(2,vector<int>(0));
      // vector<int> stateConditions(0);

      // // actionConditions[0] = vector<int>(3,0);
      // // actionConditions[0][1] = 1;
      // // actionConditions[0][2] = 2;
      // // stateConditions = vector<int>(1,0);

      // vector<double> distr;

      // data.getConditionalMarginal(stateConditions,actionConditions,typeConditions,
      // 				  false, vector<bool>(2,false),vector<bool>(2,true),
      // 				  distr);
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
  catch (...)
    {
      cout << "Unknown exception caught." << endl;
    }
}
