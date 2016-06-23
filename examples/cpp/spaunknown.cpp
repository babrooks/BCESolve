// First price auction with common but not perfectly correlated values
// BAB 11-3-2012

#include "spaunknown.hpp"

int main(int argc, char ** argv)
{
  double entryCost=0.0;
  double minAngleIncrement = 0.025;
  int nb = 50;
  int nv = 10;

  double highbid = 1.0;

  int t, a, ahat;

  char filename[100];
  sprintf(filename,"spa_noinfo_maxp0_nv=%d_nb=%d_entrycost_%1.2f.bce",
	  nv,nb,entryCost);

  cout << "Starting main" << endl;

  SPAUnknown spa(nb,nv,entryCost,highbid);
  
  BCESolver solver(spa);

  solver.setParameter(BCESolver::MinAngleIncrement,minAngleIncrement);

  cout << "Constructor finished" << endl;
  try
    {
      solver.populate();
      cout << "Done populating" << endl;

      solver.populate();
      cout << "Done populating" << endl;

      vector<double> objWeights(4,0);
      objWeights[2]=-1;
      solver.solve(objWeights);
      cout << "Solved" << endl;
      
      cout << "Bidder 1's surplus: " 
      	   << solver.getObjectiveFunction(0).getValue() << endl;
      cout << "Bidder 2's surplus: "
      	   << solver.getObjectiveFunction(1).getValue() << endl;
      cout << "Revenue: " 
      	   << solver.getObjectiveFunction(2).getValue() << endl;
      cout << "Total surplus: " 
      	   << solver.getObjectiveFunction(3).getValue() << endl;

      vector< vector<double> > bndryWeights(2,vector<double>(3));
      bndryWeights[0] = vector<double>(3,1); // total surplus
      bndryWeights[1][2] = 1; // revenue
      
      solver.mapBoundary("fpaunknownbndry_correlated.dat",
			 bndryWeights);
      cout << "Mapped boundary" << endl;

      BCESolution soln;
      solver.getSolution(soln);
      BCESolution::save(soln,filename);
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

