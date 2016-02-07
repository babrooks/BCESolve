// Duopoly game with correlated valuations
// BAB 2-12-2013

#include "duopoly.hpp"
#include "bcegurobisolver.hpp"

int main(int argc, char ** argv)
{
  double entryCost=0.00;
  double margCost = 0.00;
  double minAngleIncrement = 0.025;
  int numBids = 75, numVals = 4;

  int t, a, ahat;

  char filename[100];
  sprintf(filename,"duopoly_nb=%d_entrycost_%1.2f.bce",numBids,entryCost);

  cout << "Starting main" << endl;

  Duopoly duop(numBids,numVals,4,entryCost,margCost);

  BCEGurobiSolver solver(duop);

  solver.setParameter(BCEGurobiSolver::MinAngleIncrement,minAngleIncrement);

  cout << "Constructor finished" << endl;
  try
    {
      solver.populate();
      cout << "Done populating" << endl;

      // // Sum of bidders' surplus
      // cplex.getObjective().setExpr(solver.getObjectiveFunction(1)+solver.getObjectiveFunction(0));
      
      // cplex.setParam(IloCplex::RootAlg,IloCplex::Dual);

      solver.model.setObjective(1.0*solver.getObjectiveFunction(0),GRB_MAXIMIZE);
      cout << "Objective function set" << endl;
      solver.solve();
      // cout << "Solved" << endl;
      // cout << "Objective = " << setprecision(16) << cplex.getObjValue() << endl;

      solver.model.setObjective(1.0*solver.getObjectiveFunction(1),GRB_MINIMIZE);
      cout << "Objective function set" << endl;
      solver.solve();
      // cout << "Solved" << endl;
      // cout << "Objective = " << setprecision(16) << cplex.getObjValue() << endl;

      solver.mapBoundary();
      cout << "Mapped boundary" << endl;

      BCESolution data;
      solver.getSolution(data);
      // data.setNumValues(vector<int>(2,duop.getNumValues()));

      // cout << "Sorting data." << endl;
      // vector<int> sortObj(2,0);
      // sortObj[1]=1;
      // data.sortEquilibria(sortObj);

      BCESolution::save(data,filename);
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

