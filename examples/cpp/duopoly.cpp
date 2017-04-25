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

// Duopoly game with correlated valuations
// BAB 2-12-2013

#include "duopoly.hpp"
#include "bcesolver.hpp"

int main(int argc, char ** argv)
{
  double entryCost=0.00;
  double margCost = 0.00;
  double minAngleIncrement = 0.025;
  int numPrices = 15, numVals = numPrices;

  int t, a, ahat;

  char filename[100];
  sprintf(filename,"duopoly_np=%d_nv=%d_entrycost_%1.2f.bce",numPrices,numVals,entryCost);

  cout << "Starting main" << endl;

  Duopoly duop(numPrices,numVals,4,entryCost,margCost);

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

      vector< vector<double> > bndryWeights(2,vector<double>(4,0));
      bndryWeights[1][0]=1; 
      bndryWeights[1][1]=1; 
      bndryWeights[0][2]=1; 
      solver.mapBoundary(bndryWeights);
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
      cerr << bcee.what() << endl;
    }
  catch (...)
    {
      cout << "Unknown exception caught." << endl;
    }

  return 0;
}

