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

#include "postedprice.hpp"
#include "bcesolver.hpp"

int main() {

  int numVals = 100;
  double price = .05;
  double prPlayer0Wins = .50;

  PostedPrice pp(numVals,price,prPlayer0Wins);
  pp.setHasProductStructureData(false);

  BCESolver solver(pp);
  
  solver.populate();
  
  vector<double> objWeights(3,0);
  objWeights[2]=-1;
  solver.solve(objWeights);

  BCESolution soln;

  solver.getSolution(soln);

  stringstream fName;
  fName << "postedprice_nv=" << numVals
	<< "_price=" << price
	<< "_pr0Wins=" << prPlayer0Wins << ".bce";
  string fNameStr = fName.str();
  const char * fNameC = fNameStr.c_str();
  BCESolution::save(soln,fNameC);

  return 0;

}

