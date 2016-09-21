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

#include "commonvalueauction.hpp"

int main() {

  double entryCost = 0.0;
  double reservePrice = 0.125;
  int nv = 50;
  int nbids = 50;
  double highBid = 1;
  double alpha = 1;

  CommonValueAuction cva(nbids,nv,alpha,highBid,
			 reservePrice,entryCost);

  BCESolver solver(cva);

  solver.populate();

  vector<double> objWeights(3);
  objWeights[2]=-1;

  solver.solve(objWeights);

  BCESolution soln;
  solver.getSolution(soln);

  stringstream fName;
  fName << "cvauction_nv=" << nv
	<< "_nb=" << nbids
	<< "_ec=" << entryCost
	<< "_rp=" << reservePrice
	<< "_hb=" << highBid 
	<< "_alpha=" << alpha << ".bce";
  string fNameStr = fName.str();
  const char * fNameC = fNameStr.c_str();

  BCESolution::save(soln,fNameC);

  return 0;
}
