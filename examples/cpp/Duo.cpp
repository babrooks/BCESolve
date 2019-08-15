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

// Duopoly Game
// AH 7-17-2019

#include "Duo.hpp"

int main(int argc, char ** argv)
{
  double entryCost=0.0;
  double reservePrice=0;
  int nprices = 60;
  int nt = 2;
  double alpha = 1.0;
  double highBid = 1.0;
  double minAngleIncrement = 0.05;

  Duo cva(nprices,nt,alpha,highBid,
			 reservePrice,entryCost, minAngleIncrement);

  BCESolver solver(cva);

  solver.populate();

  vector<double> objWeights(3);
  objWeights[2]=1;

  solver.solve(objWeights);

      // Weights are set, by default, to 0.
      vector<vector<double> > mapBoundaryWeights(2,vector<double>(3,0));
      // Sets objective 0 weights for the x-axis
      mapBoundaryWeights[0][0]=1;
      // Sets objective 1 weights for the x-axis
      mapBoundaryWeights[0][1]=-1;
      // Sets objective 2 weights on the y-axis
      mapBoundaryWeights[1][2]=1;

      solver.mapBoundary("Duo.dat",mapBoundaryWeights);

  BCESolution soln;
  solver.getSolution(soln);

  stringstream fName;
  fName << "Duo_nstates=" << nt
	<< "_nprices=" << nprices
	<< "_ec=" << entryCost
	<< "_rp=" << reservePrice
	<< "_hb=" << highBid
	<< "_alpha=" << alpha << ".bce";
  string fNameStr = fName.str();
  const char * fNameC = fNameStr.c_str();

  BCESolution::save(soln,fNameC);

  return 0;
}
