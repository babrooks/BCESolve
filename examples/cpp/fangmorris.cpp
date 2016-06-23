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

  FangMorris fma(nbids,entryCost,signalAccuracy);
  BCESolver solver(fma);

  solver.setParameter(BCESolver::MinAngleIncrement,minAngleIncrement);

  cout << "Constructor finished" << endl;
  try
    {
      solver.populate();
      cout << "Done populating" << endl;

      char filename[100];
      sprintf(filename,"fangmorris_nb=%d_sig=%1.2f.bce",nbids,signalAccuracy);

      solver.mapBoundary();

      BCESolution data;
      solver.getSolution(data);

      BCESolution::save(data,filename);

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
