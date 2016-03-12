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

// Header file for common value auction example.
// RJM 03-12-2016
//! @example

#ifndef COMMONVALUEAUCTION_HPP
#define COMMONVALUEAUCTION_HPP

#include "bce.hpp"
#include "bceabstractgame.hpp"

class CommonValueAuction : public BCEAbstractGame {

private:
  double alpha;
  double highBid;
  double reservePrice;
  double entryCost;

public:
  CommonValueAuction(int _na, int _nv, double _alpha,
		     double _highBid, double _rp,
		     double _entryCost):
    BCEAbstractGame(_nv,_na,1,3),alpha(_alpha),
    highBid(_highBid),reservePrice(_rp),entryCost(_entryCost) 
  { }

  double prior (int state,const vector<int> & types) const {

    double val = static_cast<double>(state)/(numStates-1.0);
    double incr = 1.0/(numStates-1);

    if (state == 0)
      return pow(val,alpha);
    else
      return pow(val,alpha)-pow(val-incr,alpha);

  }

  double objective (int state,
		    const vector<int> & actions,
		    int obj) const {
  
    if (obj<2)
      {
	// obj is the player whose payoff we are returning
	
	double val = static_cast<double>(state)/(numStates-1.0);
	double ownBid = static_cast<double>(actions[obj])/(numActions[obj]-1.0);
	double otherBid = static_cast<double>(actions[1-obj])/(numActions[obj]-1.0);

	if (actions[obj]>actions[1-obj])
	  return val-ownBid;
	else if (actions[obj]==actions[1-obj])
	  return 0.5*(val-ownBid);
	else if (actions[obj] < actions[1-obj])
	  return 0;

      } // players' payoffs
    else if (obj==2)
      {	
	double b0 = static_cast<double>(actions[0])/(numActions[0]-1.0);
	double b1 = static_cast<double>(actions[1])/(numActions[1]-1.0);
	
	double winningBid = b0;
	if (b1 > b0) {
	  winningBid = b1;
	}

	return winningBid;
      } // seller's revenue
    
    return 0.0;
  } // objective

};

#endif
