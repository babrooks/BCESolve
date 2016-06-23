// Duopoly game with correlated valuations
// BAB 2-12-2013

#include "duopoly.hpp"
#include "bcesolver.hpp"

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

  BCESolver solver(duop);

  solver.setParameter(BCESolver::MinAngleIncrement,minAngleIncrement);

  vector<double> objWeights(4,0);

  cout << "Constructor finished" << endl;
  try
    {
      solver.populate();
      cout << "Done populating" << endl;

      objWeights[0]=1;
      solver.solve(objWeights);

      objWeights[0]=0; objWeights[1]=1;
      solver.solve(objWeights);

      solver.mapBoundary();
      cout << "Mapped boundary" << endl;

      BCESolution data;
      solver.getSolution(data);

      BCESolution::save(data,filename);
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

