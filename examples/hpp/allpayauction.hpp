// This file is part of the BCESolve library for games of incomplete
// information
// Copyright (C) 2016 Benjamin A. Brooks, Robert J. Minton
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

// Header for all pay auction example.
// RJM 9-17-2015
//! @example

#ifndef ALLPAYAUCTION_HPP
#define ALLPAYAUCTION_HPP

#include "bce.hpp"
#include "bceabstractgame.hpp"
#include "bcegame.hpp"

class AllPayAuction : public BCEAbstractGame {

public:

  AllPayAuction(int nv,int na):
    BCEAbstractGame(nv,na,1,3) 
  {}

  double prior (int state,const vector<int> & types) const {
    return 1.0/numStates;
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
	  return -ownBid;

      } // players' payoffs
    else if (obj==2)
      {	
	double b0 = static_cast<double>(actions[0])/(numActions[0]-1.0);
	double b1 = static_cast<double>(actions[1])/(numActions[1]-1.0);
	return b0 + b1;
      } // seller's revenue
    
    return 0.0;
  } // objective

};

#endif
