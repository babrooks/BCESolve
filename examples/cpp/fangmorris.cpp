// This file is part of the BCESolve library for games of incomplete
// information
// Copyright (C) 2016 Benjamin A. Brooks and Robert J. Minton
// 
// BCESolve free software: you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// BCESolve is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see
// <http://www.gnu.org/licenses/>.
// 
// Benjamin A. Brooks
// ben@benjaminbrooks.net
// Chicago, IL

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
