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
//fpa perfect correlation, send dates

#ifndef FPAPERFCOR_HPP
#define FPAPERFCOR_HPP

#include "bce.hpp"
#include "bceabstractgame.hpp"

class FPAPERFCOR : public BCEAbstractGame {

private:
  double alpha;
  double highBid;
  double reservePrice;
  double entryCost;

public:
  FPAPERFCOR(int _na, int _nv, double _alpha,
		     double _highBid, double _rp,
		     double _entryCost):
    BCEAbstractGame(_nv,_na+1,1,3),alpha(_alpha),
    highBid(_highBid),reservePrice(_rp),entryCost(_entryCost)
  { }

  double prior (int state,const vector<int> & types) const {

    double val = static_cast<double>(state)/static_cast<double>(numStates);
    double incr = 1.0/static_cast<double>(numStates);

    return pow(val,alpha)-pow(val-incr,alpha);

  }

  double objective (int state,
		    const vector<int> & actions,
		    int obj) const {

    if (obj<2)
      {
	// obj is the player whose payoff we are returning

	double val = static_cast<double>(state)/(numStates-1.0);
    if (obj==0)
    {
        val += 1.5;
    }
	double ownBid = highBid*static_cast<double>(actions[obj]-1)/(numActions[obj]-2.0);

	  if (actions[obj]>actions[1-obj])
	    return val-ownBid;
	  else if (actions[obj]==actions[1-obj])
	    return 0.5*(val-ownBid);
    return 0.0;
	// if (ownBid < reservePrice)
	//   return ceil(ownBid)*entryCost;
	// else {
	//   if (actions[obj]>actions[1-obj] && obj==0)
    //   {
    //       return val-ownBid-entryCost+1.0;
    //   }
    //   else if (actions[obj]>actions[1-obj])
    //   {
    //       return val-ownBid-entryCost;
    //   }
	//   else if (actions[obj]==actions[1-obj] && obj==0)
    //   {
    //       return 0.5*(val+1.0-ownBid)-entryCost;
    //   }
    //   else if (actions[obj]==actions[1-obj])
    //   {
    //       return 0.5*(val-ownBid)-entryCost;
    //   }
	//   else if (actions[obj] < actions[1-obj])
	//     return -entryCost;
	// }


      } // players' payoffs
    else if (obj==2)
      {
	int maxBid = actions[0];
	if (actions[1] > actions[0]) {
	  maxBid = actions[1];
	}

	double winningBid = (highBid*maxBid)/(numActions[0]-1.0);
    return winningBid;
	// if (winningBid >= reservePrice)
	//   return winningBid + entryCost;
	// else
	//   return ceil(winningBid)*entryCost;

      } // seller's revenue

    return 0.0;
  } // objective

};

#endif
