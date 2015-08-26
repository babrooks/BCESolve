// First price auction with private values
// BAB 11-3-2012

#include "fpaknown.hpp"

void solveFPA(int nvals, int nbids, 
	      double entryCost, double reservePrice,
	      bool exAnteFee);

int main(int argc, char ** argv)
{
  double entryCost=0;
  double reservePrice=0.0;
  int nvals=2;
  int nbids=50;
  double lowbid = 0.0;

  solveFPA(nvals,nbids,entryCost,reservePrice,false);
  return 0;

}

void solveFPA(int nvals, int nbids, 
	      double entryCost, double reservePrice,
	      bool exAnteFee)
{
  int t, a, ahat;
  double minAngleIncrement = 0.05;
  vector< vector<double> > devObjectives;

  stringstream filename;
  filename << "fpaknown_nv=" << nvals << "_nb=" << nbids;

  bool verbose = true;
  double highbid = 1.0;
  
  if (verbose)
    cout << "Starting main" << endl;

  try
    {
      FPAKnown fpa(nbids,nvals,entryCost,reservePrice,highbid,
		   exAnteFee);
      fpa.distribution.clear();
      // fpa.distribution.push_back(uniformWithMassPnt(0.3),1.0); // uniform with mass 
      fpa.distribution.push_back(new vToTheAlpha(1.0),1.0); // uniform


      BCESolver solver(fpa);

      solver.setParameter(BCESolver::MinAngleIncrement,minAngleIncrement);
      solver.setParameter(BCESolver::DisplayLevel,1);

      if (verbose)
	cout << "Constructor finished" << endl;

      solver.populate();
      if (verbose)
	cout << "Done populating" << endl;


      solver.setParameter(BCESolver::BoundaryObjective1,2);
      solver.setParameter(BCESolver::BoundaryObjective2,3);

      // sprintf(filename,"fpaknown_nv=%d_nb=%d_middlebid=%1.2f_minrev.bce",
      // 	      nvals,nbids,fpa.valueGrid[1]);

      // sprintf(filename,"fpaknown_efficient_nv=%d_nb=%d.bce",
      // 	      nvals,nbids);

      // sprintf(filename,"fpaknown_nv=%d_nb=%d_onlyup.bce",
      // 	      nvals,nbids);

      IloCplex cplex = solver.getCplex();

      for (int player = 0; player < 2; player++)
	{
	  for (int val = 0; val < nvals; val++)
	    {
	      cplex.getModel()
		.add(solver.getObjectiveFunction(5+val+player*nvals)>=0.0);
	    } // val
	}

      cplex.setParam(IloCplex::RootAlg,IloCplex::Barrier);
      cplex.setParam(IloCplex::SimDisplay,0);
      solver.setParameter(BCESolver::DisplayLevel,1);

      cplex.getObjective().setSense(IloObjective::Maximize);
      cplex.getObjective().setExpr(-1.0*(solver.getObjectiveFunction(2)));
      cplex.setParam(IloCplex::Threads,4);
      
      // filename << "_maxbidderssurplus";
      
      // cplex.getObjective().setSense(IloObjective::Minimize);
      // cplex.getObjective().setExpr(solver.getObjectiveFunction(2));
      // filename << "_minrevenue";

      if (verbose)
      	cout << "Objective function set" << endl;
      solver.solve();

      cout << "Bidder 1's surplus: " 
      	   << cplex.getValue(solver.getObjectiveFunction(0)) << endl;
      cout << "Bidder 2's surplus: "
      	   << cplex.getValue(solver.getObjectiveFunction(1)) << endl;
      cout << "Revenue: " 
      	   << cplex.getValue(solver.getObjectiveFunction(2)) << endl;
      cout << "Total surplus: " 
      	   << cplex.getValue(solver.getObjectiveFunction(3)) << endl;

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


      // if (verbose)
      // 	cout << "Saving data..." << endl;

      solver.setParameter(BCESolver::BoundaryObjective1,0);
      solver.setParameter(BCESolver::BoundaryObjective2,1);

      solver.setBndryObjective(1,solver.getObjectiveFunction(0)
      			       +solver.getObjectiveFunction(1));
      solver.setBndryObjective(2,solver.getObjectiveFunction(2));

      solver.mapBoundary("fpaknownbndry_bidder.dat");

      BCEData data;
      solver.getData(data);
      data.setNumValues(vector<int>(2,nvals));

      filename << ".bce";
      const string & filenamestring = filename.str();
      const char* filenamecstring = filenamestring.c_str();
      BCEData::save(data,filenamecstring);
      
      // solver.setParameter(BCESolver::BoundaryObjective1,4);
      // solver.setParameter(BCESolver::BoundaryObjective2,2);

      // solver.mapBoundary("fpaknownbndry.dat");


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
