// First price auction with private values
// BAB 11-3-2012

#include "fpaknown.hpp"

void solveFPA(int nvals, int nbids, 
	      double entryCost, double reservePrice);

int main(int argc, char ** argv)
{
  double entryCost=0.0;
  double reservePrice=0.0;
  int nvals=3;
  int nbids=50;

  solveFPA(nvals,nbids,entryCost,reservePrice);
  return 0;

  for (nvals=10; nvals < 11; nvals+=2)
    solveFPA(nvals,nbids,entryCost,reservePrice);

  return 0;
}

void solveFPA(int nvals, int nbids, 
	      double entryCost, double reservePrice)
{
  int t, a, ahat;
  double minAngleIncrement = 0.05;
  vector< vector<double> > devObjectives;

  bool verbose = true;
  double highbid = 0.35;
  
  if (verbose)
    cout << "Starting main" << endl;

  try
    {
      FPAKnown fpa(nbids,nvals,entryCost,reservePrice,highbid);
      fpa.distribution.clear();
      // fpa.distribution.push_back(uniformWithMassPnt(0.3),1.0); // uniform with mass 
      fpa.distribution.push_back(vToTheAlpha(1.0),1.0); // uniform
      // fpa.distribution.push_back(independent(1.0/3.0),1.0);
      // fpa.distribution.push_back(uniformNoTie(nvals),1.0);
      // fpa.distribution.push_back(independent(1.0/3.0),1.0);

      BCESolver solver(fpa);

      solver.setParameter(BCESolver::MinAngleIncrement,minAngleIncrement);
      solver.setParameter(BCESolver::OnlyICDown,false);
      solver.setParameter(BCESolver::OnlyICUp,true);
      solver.setParameter(BCESolver::DisplayLevel,1);

      if (verbose)
	cout << "Constructor finished" << endl;

      solver.populate();
      if (verbose)
	cout << "Done populating" << endl;

      char filename[100];
      // sprintf(filename,"fpaknown_nv=%d_nb=%d_entrycost_%1.2f_reserve_%1.2f_highbid=%1.2f_corner_barrier.bce",
      // 	      nvals,nbids,entryCost,reservePrice,fpa.highbid);

      // sprintf(filename,"fpaknown_revenue_nv=%d_nb=%d.bce",
      // 	      nvals,nbids);
      // solver.setParameter(BCESolver::BoundaryObjective1,2);
      // solver.setParameter(BCESolver::BoundaryObjective2,3);

      sprintf(filename,"fpaknown_nv=%d_nb=%d.bce",
      	      nvals,nbids);
      // sprintf(filename,"fpaknown_efficient_nv=%d_nb=%d.bce",
      // 	      nvals,nbids);

      // sprintf(filename,"fpaknown_nv=%d_nb=%d.bce",
      // 	      nvals,nbids);

      IloCplex cplex = solver.getCplex();

      cplex.setParam(IloCplex::RootAlg,IloCplex::Barrier);
      cplex.setParam(IloCplex::SimDisplay,0);
      solver.setParameter(BCESolver::DisplayLevel,1);

      cplex.getObjective().setSense(IloObjective::Maximize);
      cplex.getObjective().setExpr(solver.getObjectiveFunction(4)
      				   +solver.getObjectiveFunction(5)); // bidder
								     // surplus
      // cplex.getObjective().setExpr(solver.getObjectiveFunction(3)); // total surplus
      
      if (verbose)
      	cout << "Objective function set" << endl;
      solver.solve();

      cout << cplex.getValue(solver.getObjectiveFunction(4)) << endl;
      cout << cplex.getValue(solver.getObjectiveFunction(5)) << endl;

      cout << "Total surplus: " << cplex.getValue(solver.getObjectiveFunction(3)) << endl;

      // cout << "Adding constraint" << endl;
      // cplex.getModel().add(solver.getObjectiveFunction(0)+solver.getObjectiveFunction(1) == cplex.getObjValue());


      // cplex.getObjective().setExpr(-1*solver.getObjectiveFunction(3));
      // solver.solve();


      // cplex.getObjective().setExpr(solver.getObjectiveFunction(1));
      
      // cout << "Resolving with bound on player 1's payoff." << endl;
      // solver.solve();
      // cout << "Solved" << endl;

      // cout << "Objective = " << setprecision(16) << cplex.getObjValue() << endl;
      // cout << "Player 0's payoff = " << cplex.getValue(solver.getObjectiveFunction(0)) << endl;
      // cout << "Player 1's payoff = " << cplex.getValue(solver.getObjectiveFunction(1)) << endl;
      // cout << "Revenue = " << cplex.getValue(solver.getObjectiveFunction(2)) << endl;

      // cout << cplex.getValue(solver.getObjectiveFunction(0)) << ";..." << endl;


      // cout << "Sorting data..." << endl;

      // vector<int> sortObj(2,0);
      // sortObj[1]=1;
      // data.sortEquilibria(sortObj);

      // solver.mapBoundary();

      BCEData data;
      solver.getData(data);
      data.setNumValues(vector<int>(2,nvals));

      if (verbose)
      	cout << "Saving data..." << endl;

      BCEData::save(data,filename);
      

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
