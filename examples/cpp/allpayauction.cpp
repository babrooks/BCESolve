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

#include "allpayauction.hpp"

int main() {

  int numBids = 20;
  int numVals = 20;

  AllPayAuction apa(numVals,numBids);

  try {
    BCESolver solver(apa);

    solver.populate();
  
    // Minimize Revenue by default.
    // Objective function 0 is player 0 payoff.
    // Objective function 1 is player 1 payoff.
    // Objective function 2 is seller revenue.
    vector<double> objWeights(3);
    objWeights[2]=-1;

    solver.solve(objWeights);
    
    BCESolution soln;

    solver.getSolution(soln);

    stringstream fName;
    fName << "allpayauction_nv=" << numVals
	  << "_nb=" << numBids << ".bce";
    string fNameStr = fName.str();
    const char * fNameC = fNameStr.c_str();
    BCESolution::save(soln,fNameC);

  }
  catch(BCEException &e) {
    cout << e.getMessage() << endl;
  }
  catch(GRBException &e) {
    cout << e.getMessage() << endl;
  }

  return 0;

}
