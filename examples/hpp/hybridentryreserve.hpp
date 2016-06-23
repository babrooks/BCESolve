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

/* Header for hybrid auctions with 
   reserve and entree fee example.
   RJM 9-18-2015
*/
//! @example

#ifndef HYBRIDENTRYRESERVE_HPP
#define HYBRIDENTRYRESERVE_HPP

#include "bce.hpp"
#include <math.h>

class HybridEntryReserve : public BCEAbstractGame {

private:
  double weightOnOwnBid;
  double reservePrice;
  double entryFee;
  double highBid;
  
public:
  HybridEntryReserve(int nv,
		     int na,
		     double _weightOnOwnBid,
		     double _reservePrice,
		     double _entryFee,
		     double _highBid):
    BCEAbstractGame(nv,na,1,3),
    weightOnOwnBid(_weightOnOwnBid),
    reservePrice(_reservePrice),
    entryFee(_entryFee),
    highBid(_highBid)
  { }

  double prior (int state, const vector<int> & types) const {
    return 1.0/numStates;    
  } // prior

  double objective (int state, 
		    const vector<int> & actions, 
		    int obj) const {

    if (obj<2)
      {
	double val = static_cast<double>(state)/(numStates-1);
	double ownBid = highBid*static_cast<double>(actions[obj])/(numActions[obj]-1);
	double otherBid = highBid*static_cast<double>(actions[1-obj])/(numActions[obj]-1);

	if (ownBid == 0)
	  return 0.0;

	if (ownBid < reservePrice || ownBid < otherBid)
	  return -entryFee;

	if (ownBid > otherBid)
	  return val-ownBid*weightOnOwnBid-otherBid*(1.0-weightOnOwnBid)-entryFee;

	else if (ownBid == otherBid)
	  return 0.5*(val-ownBid*weightOnOwnBid-otherBid*(1.0-weightOnOwnBid))-entryFee;

      } // players' payoffs
    else if (obj==2)
      {
	// revenue
	
	double b0 = highBid*static_cast<double>(actions[0])/(numActions[0]-1);
	double b1 = highBid*static_cast<double>(actions[1])/(numActions[1]-1);

	double entryFeeRevenue = ceil(b0)*entryFee + ceil(b1)*entryFee;

	double winningBid = b0;
	if (b1 > b0)
	  winningBid = b1;

	if (winningBid < reservePrice)
	  return entryFeeRevenue;

	if (b0 >= b1) 
	  return weightOnOwnBid*b0+(1.0-weightOnOwnBid)*b1+entryFeeRevenue;

	else 
	  return weightOnOwnBid*b1+(1.0-weightOnOwnBid)*b0+entryFeeRevenue;	  
      }

    return 0;
  } // objective


};

#endif
