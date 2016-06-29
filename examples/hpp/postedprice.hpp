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

//! @example
#ifndef POSTEDPRICE_HPP
#define POSTEDPRICE_HPP

#include "bce.hpp"

class PostedPrice : public BCEAbstractGame {

private:
  double price;
  vector<double> prPlayerWins;

public:
  PostedPrice(int nv,
	      double _price,
	      double _prPlayer0Wins):
    // 2 actions ("buy","don't buy"), 1 type, 3 objectives
    BCEAbstractGame(nv,2,1,3),
    price(_price)
  {
    prPlayerWins = vector<double>(2,0);
    prPlayerWins[0] = _prPlayer0Wins;
    prPlayerWins[1] = 1-_prPlayer0Wins;
  }

  double prior (int state, const vector<int> & types) const {
    return 1.0/numStates; 
  }

  double objective (int state, 
		    const vector<int> & actions, 
		    int obj) const {

    if (obj<2)
      {
	double val = static_cast<double>(state)/(numStates-1);
	int ownChoice = actions[obj];
	int otherChoice = actions[1-obj];

	if (ownChoice == 0)
	  return 0.0;

	if (ownChoice == 1 && otherChoice ==1)
	  return prPlayerWins[obj]*(val-price);
	
	if (ownChoice ==1)
	  return val-price;

      } // players' payoffs
    else if (obj==2)
      {
	// revenue
	
	if (actions[0] == 1 || actions[1] == 1)
	  return price;
	  
      }
    return 0;
  } // objective 

};

#endif
