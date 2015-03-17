// Krasnokutskaya motivated example with public signals
// BAB 6-10-2013
#include "publicinfo.hpp"

void solveModel (double signalAccuracy);

int main(int argc, char ** argv)
{
  double signalAccuracy = 0.5;

  for (signalAccuracy=0.5; signalAccuracy<1.0; signalAccuracy+=0.1)
    solveModel(signalAccuracy);

  return 0;
}

void solveModel (double signalAccuracy) 
{
  int t, a, ahat;
  double entryCost=0.01;
  double minAngleIncrement = 0.05;
  int nbids=30;

  cout << "Starting main" << endl;

  PublicInfo fpa(nbids,entryCost,signalAccuracy);
  BCESolver solver(fpa);

  solver.setParameter(BCESolver::MinAngleIncrement,minAngleIncrement);

  cout << "Constructor finished" << endl;
  try
    {
      solver.populate();
      cout << "Done populating" << endl;

      char filename[100];
      sprintf(filename,"publicinfo_nb=%d_sig=%1.2f.bce",nbids,signalAccuracy);
      // sprintf(filename,"fangmorris_-p1+p0_nb=%d_entrycost_%1.2f_signalacc_%1.2f.bce",
      // 	      nbids,entryCost,signalAccuracy);

      IloCplex cplex=solver.getCplex();
      cplex.getObjective().setExpr(-solver.getObjectiveFunction(1)+solver.getObjectiveFunction(0));
      cout << "Objective function set" << endl;
      
      // solver.solve();
      // cout << "Solved" << endl;

      // cout << "Objective = " << setprecision(16) << cplex.getObjValue() << endl;

      solver.mapBoundary();

      BCEData data;
      solver.getData(data);
      data.setNumValues(vector<int>(2,2));

      // vector<int> sortObj(2,0);
      // sortObj[1]=1;
      // data.sortEquilibria(sortObj);

      BCEData::save(data,filename);

      vector< vector<int> > actionConditions(2,vector<int>(0));
      vector< vector<int> > typeConditions(2,vector<int>(0));
      vector<int> stateConditions(0);

      // actionConditions[0] = vector<int>(3,0);
      // actionConditions[0][1] = 1;
      // actionConditions[0][2] = 2;
      // stateConditions = vector<int>(1,0);

      vector<double> distr;

      data.getConditionalMarginal(stateConditions,actionConditions,typeConditions,
				  false, vector<bool>(2,false),vector<bool>(2,true),
				  distr);
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
