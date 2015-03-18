// First price auction revenue bounds with reserve prices and entry fees.

#include "fpagame.hpp"

void solveModel(int nb, int nv, double entryCost, double reservePrice,
		bool exAnteFee, ofstream & datafile);

int main(int argc, char ** argv)
{
  double entryCost=0.0;
  double reservePrice=0.0;
  int nb = 20;
  int nv = nb;

  stringstream ss; 
  ss.str("");
  ss << "fpaunknown_revbounds_nb="
     << nb << "_nv=" << nv << ".dat";

  cout << "Writing to file: " << ss.str() << endl;
  
  ofstream datafile; 
  char * fname = new char [ss.str().length()+1];
  strcpy(fname,ss.str().c_str());
  datafile.open(fname,ofstream::out);
  
  for (int k = 0; k < nv; k++)
    {
      reservePrice = (1.0*k)/(nv-1.0)-1e-6;
      solveModel(nb, nv, entryCost, reservePrice,false, datafile);
    }
  reservePrice = 0.0;
  for (int k = 0; k < nv; k++)
    {
      entryCost = (1.0*k)/(nv-1.0)-1e-6;
      solveModel(nb, nv, entryCost, reservePrice,false, datafile);
    }
  for (int k = 0; k < nv; k++)
    {
      entryCost = (1.0*k)/(nv-1.0)-1e-6;
      solveModel(nb, nv, entryCost, reservePrice,true, datafile);
    }


  return 0;
}

void solveModel(int nb, int nv, double entryCost, double reservePrice,
		bool exAnteFee, ofstream & datafile)
{
  double minAngleIncrement = 0.025;
  double highbid = 1.0;

  FPAGame fpa(nb,nv,entryCost,reservePrice,highbid,exAnteFee);
  
  BCESolver solver(fpa);

  solver.setParameter(BCESolver::DisplayLevel,0);
  solver.setParameter(BCESolver::MinAngleIncrement,minAngleIncrement);

  try
    {
      solver.populate();

      IloCplex cplex=solver.getCplex();
      cplex.setParam(IloCplex::Threads,4);

      for (int player = 0; player < 2; player++)
	{
	  for (int val = 0; val < nv; val++)
	    {
	      cplex.getModel()
		.add(solver.getObjectiveFunction(5+val+player*nv)>=0.0);
	    } // val
	}

      cplex.setParam(IloCplex::BarDisplay,0);
      cplex.setParam(IloCplex::SimDisplay,0);
      // cplex.setParam(IloCplex::RootAlg,IloCplex::Dual);

      cplex.getObjective().setExpr(-1.0*solver.getObjectiveFunction(2));
      solver.solve();
      // cout << "Solved" << endl;
      
      // cout << "ItLim: " << cplex.getParam(IloCplex::ItLim) << endl;

      // cout << "Objective = " << setprecision(16) << cplex.getObjValue() << endl;

      // cout << "Bidder 1's surplus: " 
      // 	   << cplex.getValue(solver.getObjectiveFunction(0)) << endl;
      // cout << "Bidder 2's surplus: "
      // 	   << cplex.getValue(solver.getObjectiveFunction(1)) << endl;
      // cout << "Revenue: " 
      // 	   << cplex.getValue(solver.getObjectiveFunction(2)) << endl;
      // cout << "Total surplus: " 
      // 	   << cplex.getValue(solver.getObjectiveFunction(3)) << endl;
      double minrev = cplex.getValue(solver.getObjectiveFunction(2));


      cplex.getObjective().setExpr(solver.getObjectiveFunction(2));
      solver.solve();

      double maxrev = cplex.getValue(solver.getObjectiveFunction(2));
      cout << "nb: " << nb << ", nv: " << nv 
	   << ", entry cost: " << entryCost
	   << ", ex-ante fee: " << exAnteFee
	   << ", reserve price: " << reservePrice
	   << ", minimum revenue: " << minrev
	   << ", maximum revenue: " << maxrev << endl;

      datafile << nb << " " << nv
      	       << " " << entryCost
	       << " " << exAnteFee
      	       << " " << reservePrice
      	       << " " << minrev << " " << maxrev << endl;
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
}
