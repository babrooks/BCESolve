// This file is part of the BCESolve library for games of incomplete
// information
// Copyright (C) 2022 Benjamin A. Brooks
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

// First price auction with private values
// BAB 11-3-2012

#include "fpaknown.hpp"

void solveFPA(int nvals, int nbids, 
	      double entryCost, double reservePrice,
	      bool exAnteFee);

int main(int argc, char ** argv)
{
  double entryCost=0.0;
  double reservePrice=0.125;
  int nvals=20;
  int nbids=20;
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

      // Denote that the game has a product structure (private values)
      fpa.setHasProductStructureData(true);

      // Creating the solver object
      BCESolver solver(fpa);

      if (verbose)
	cout << "Constructor finished" << endl;

      // Populating Constraints
      solver.populate();
      if (verbose)
	cout << "Done populating" << endl;

      // Set the main objective
      int numObjs = fpa.getNumObjectives();
      vector<double> solverWeights(numObjs,0);
      // Minimize Revenue
      solverWeights[2]=-1;

      // Minimize social surplus
      solverWeights[0]=-1;
      solverWeights[1]=-1;
      solverWeights[2]=-1;

      if (verbose)
      	cout << "Objective function set" << endl;

      // Solve the model
      solver.solve(solverWeights);


      // // Weights are set, by default, to 0.
      // vector<vector<double> > mapBoundaryWeights(2,vector<double>(numObjs,0));
      // // Sets objective 0 weights for the x-axis
      // mapBoundaryWeights[0][0]=1;
      // // Sets objective 1 weights for the x-axis
      // mapBoundaryWeights[0][1]=0;
      // // Sets objective 2 weights on the y-axis
      // mapBoundaryWeights[1][2]=1;      

      // solver.mapBoundary("fpaknown.dat",mapBoundaryWeights);

      BCESolution soln;
      solver.getSolution(soln);

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
