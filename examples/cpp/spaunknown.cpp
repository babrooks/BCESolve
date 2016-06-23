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

// First price auction with common but not perfectly correlated values
// BAB 11-3-2012

#include "spaunknown.hpp"

int main(int argc, char ** argv)
{
  double entryCost=0.0;
  double minAngleIncrement = 0.025;
  int nb = 20;
  int nv = 5;

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

      vector< vector<double> > bndryWeights(2,vector<double>(4,0));
      bndryWeights[0][0] = 1; // b1 surplus
      bndryWeights[0][1] = 1; // b2 surplus
      bndryWeights[0][2] = 1; // revenue
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
  catch (GRBException & e)
    {
      cout << "GRBException caught: " << endl
	   << e.getMessage() << endl;
    }
  catch (...)
    {
      cout << "Unknown exception caught." << endl;
    }

  return 0;
}

