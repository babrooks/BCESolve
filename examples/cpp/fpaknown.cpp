// First price auction with private values
// BAB 11-3-2012

#include "fpaknown.hpp"

void solveFPA(int nvals, int nbids, 
	      double entryCost, double reservePrice,
	      bool exAnteFee);

int main(int argc, char ** argv)
{
  double entryCost=0.0;
  double reservePrice=0.0;
  int nvals=3;
  int nbids=25;
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
  filename << "fpaknown_nv=" << nvals << "_nb=" << nbids << ".bce";

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

      if (verbose)
	cout << "Constructor finished" << endl;

      solver.populate();
      if (verbose)
	cout << "Done populating" << endl;

      solver.model.setObjective(solver.getObjectiveFunction(2),GRB_MINIMIZE);
      if (verbose)
      	cout << "Objective function set" << endl;

      solver.solve();
      solver.mapBoundary("fpaknown.dat");

      BCESolution soln;
      solver.getSolution(soln);

      vector< vector<double> > expObj;
      // soln.getExpectedObjectives(expObj);

      string fileNameStr = filename.str();
      const char * fnameC = fileNameStr.c_str();
      BCESolution::save(soln,fnameC);
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
