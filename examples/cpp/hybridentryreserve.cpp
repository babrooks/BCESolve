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

#include "hybridentryreserve.hpp"

int main() {

  int numBids = 15;
  int numVals = 15;
  double weightOnOwnBid = 1;
  double reservePrice = .25;
  double entryFee = 0.0;
  double highBid = 1.0;

  HybridEntryReserve hera(numVals,
			  numBids,
			  weightOnOwnBid,
			  reservePrice,
			  entryFee,
			  highBid);

  BCESolver solver(hera);
  
  solver.populate();
  
  // Minimize Revenue
  vector<double> objWeights(3,0);
  objWeights[2]=-1;
  solver.solve(objWeights);

  BCESolution data;
  solver.getSolution(data);
  
  stringstream fName;
  fName << "hybridauction_nv=" << numVals
	<< "_nb=" << numBids
	<< "_w=" << weightOnOwnBid
	<< "_rp=" << reservePrice 
	<< "_ef=" << entryFee << ".bce";
  string fNameStr = fName.str();
  const char * fNameC = fNameStr.c_str();
  BCESolution::save(data,fNameC);

}
