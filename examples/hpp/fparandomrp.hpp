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

#ifndef FPARANDOMRP_HPP
#define FPARANDOMRP_HPP

#include "bce.hpp"
#include <math.h>

class FPARandomRP : public BCEAbstractGame
{
private:
  // vector<vector<double> > cdf;
  // int rpPrecision;
  double ub;
  double highBid;
  double postedPrice;
  double postedPriceProb;
  
  double rpcdf(double x) const
  {
    double lb = 0.0;
    double ub2 = 0.175;
    if (x<lb)
      return 0;
    if (x<ub2)
      return pow((x-lb)/(ub2-lb),2);
    return 1;

    if (x<0.125)
      return 0;
    return 1;
  }

public:
  FPARandomRP(int nv,
	      int na,
	      double _ub):
    BCEAbstractGame(nv,na,1,3),
    highBid(0.5),
    ub(_ub),
    postedPrice(0.2),
    postedPriceProb(0.0)
  {}

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
	
	double payoff = 0;

	if (ownBid > otherBid)
	  payoff += (1.0-postedPriceProb)*(val-ownBid)*rpcdf(ownBid);
	else if (ownBid == otherBid)
	  return 0.5*(1.0-postedPriceProb)*(val-ownBid)*rpcdf(ownBid);

	if (ownBid >= postedPrice && otherBid >= postedPrice)
	  payoff += 0.5*postedPriceProb*(val-postedPrice);
	else if (ownBid > postedPrice)
	  payoff += postedPriceProb*(val-postedPrice);
	  
	return payoff;
      } // players' payoffs
    else if (obj==2)
      {
	// revenue
	
	double b0 = highBid*static_cast<double>(actions[0])/(numActions[0]-1);
	double b1 = highBid*static_cast<double>(actions[1])/(numActions[1]-1);

	double winningBid = b0;
	if (b1 > b0)
	  winningBid = b1;

	double revenue=(1.0-postedPriceProb) * winningBid*rpcdf(winningBid);
	
	if (winningBid>=postedPrice)
	  revenue += postedPriceProb * postedPrice;
	return revenue;

      }
    return 0;
  } // objective


};

#endif
