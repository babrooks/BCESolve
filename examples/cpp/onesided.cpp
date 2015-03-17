// Calculates the lower bound on revenue and upper bound on bidders'
// surplus over all BCE of the first price auction.
// 
// BAB 5-30-2013

#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <vector>
#include "onesided.hpp"

using namespace std;

int main ()
{
  int numVals = 70;
  int numBids = numVals;
  double vstar = 0.6;
  double p = 0.5;
  // onesided(numVals,numBids);

  vector<int> _numTypes(2,0);
  _numTypes[0] = numVals; _numTypes[1]=1;

  try
    {
      OneSided game(numBids,numVals,_numTypes,0.0,0.0,vstar,p);
  
      BCESolver solver(game);

      solver.setParameter(BCESolver::MinAngleIncrement,0.05);
      solver.setParameter(BCESolver::OnlyICDown,false);
      solver.setParameter(BCESolver::OnlyICUp,false);
      solver.setParameter(BCESolver::DisplayLevel,1);

      solver.populate();

      IloCplex cplex = solver.getCplex();

      cplex.setParam(IloCplex::RootAlg,IloCplex::Barrier);
      cplex.setParam(IloCplex::SimDisplay,0);
      solver.setParameter(BCESolver::DisplayLevel,1);

      cplex.getObjective().setSense(IloObjective::Maximize);
      cplex.getObjective().setExpr(solver.getObjectiveFunction(3)); // efficiency
      solver.solve();

      cplex.getModel().add(solver.getObjectiveFunction(3)==cplex.getObjValue());
      
      // cout << cplex.getValue(solver.getObjectiveFunction(0)) << endl
      // 	   << cplex.getValue(solver.getObjectiveFunction(1)) << endl;

      cplex.getObjective().setExpr(solver.getObjectiveFunction(1));
      solver.solve();

      cplex.getModel().add(solver.getObjectiveFunction(1)==cplex.getObjValue());


      cplex.getObjective().setExpr(solver.getObjectiveFunction(0));
      solver.solve();

      // solver.mapBoundary();

      char filename[100];
      sprintf(filename,"onesided_nv=%d_vstar=%1.2f_p=%1.2f_efficiency_maxs2_cross.bce",
	      numVals,vstar,p);
      BCEData data;
      solver.getData(data);


      cout << "Saving data..." << endl;

      BCEData::save(data,filename);

      cout << "Saved" << endl;
      // vector<double> distribution;
      // vector<int> stateConditions;
      // vector<vector<int> > actionConditions(2), typeConditions(2);
      // bool stateMarginal = true;
      // vector<bool> actionMarginal(2,false), typeMarginal(2,false);
      
      // double prob = data.getConditionalMarginal(stateConditions,actionConditions,typeConditions,
      // 						stateMarginal,actionMarginal,typeMarginal,
      // 						distribution);
      //  cout << "prob = " << prob << endl;



      BCEData data2;
      BCEData::load(data2,filename);
      // prob = data2.getConditionalMarginal(stateConditions,actionConditions,typeConditions,
      // 					  stateMarginal,actionMarginal,typeMarginal,
      // 					  distribution);

      // cout << "prob = " << prob << endl;

      cout << "Reloaded " << endl;
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


  return 0;

} // main
