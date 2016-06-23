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

// Header file for fpaunknown example.
// BAB 11-3-2012
//! @example

#ifndef FPAGAME_HPP
#define FPAGAME_HPP

#include "bce.hpp"
#include "bceabstractgame.hpp"

class FPAGame : public BCEAbstractGame
{
protected:
  int numValues;
  double fee;
  double reservePrice;
  double highbid;
  double lowbid;
  bool exAnteFee;

public:
  BCEDistrArray distribution;

  FPAGame() {distribution.push_back(new vToTheAlpha(1.0),1.0);}
  
  FPAGame(int na, int nv, double _fee, 
	  double _reservePrice,double _highbid,
	  bool _exAnteFee)
    : BCEAbstractGame(nv*nv,na+1,1,3), numValues(nv), 
      fee(_fee), highbid(_highbid),
      lowbid(0.0), reservePrice(_reservePrice),
      exAnteFee(_exAnteFee)
  {
    setNumPrivateStates(vector<int>(2,nv));
    
    distribution.push_back(new vToTheAlpha(1.0),1.0);
    // distribution.push_back(new additiveCorrelated(),1.0);
    // distribution.push_back(new uniform(),1.0);
    // distribution.push_back(new bernoulli(0.25),1.0);
  }

  int getNumValues() const {return numValues;}

  void stateToPrivateValues(int state, vector<int> &values) const
  {
    values.resize(2);

    values[0]=state%numValues;
    state-=values[0]; 
    values[1]=state/numValues;
  }

  double prior (int state, const vector<int> &types) const
  {
    vector<int> values;
    stateToPrivateValues(state,values);
    double v0 = static_cast<double>(values[0]+1)/numValues;
    double v1 = static_cast<double>(values[1]+1)/numValues;
    double incr = 1.0/numValues;

    return distribution.PDF(v0,v1,incr);
  }

  double objective(int state, const vector<int> &actions, 
		   int objectiveIndex) const
  {
    // Convert the state into a pair of valuations
    vector<int> values(2,0);
    stateToPrivateValues(state,values);

    double obj = 0; 

    double winbid = lowbid + ((actions[0]>actions[1]? actions[0]: actions[1])-1.0)
      * (highbid-lowbid) / (numActions[0]-2.0);
    // If winning bid is less than reserve price, all objectives are
    // zero.
    if (winbid < reservePrice)
      return 0.0;

    if (objectiveIndex < 2)
      {
	int player = objectiveIndex;

	// Player 1's payoff
	if (actions[player]>actions[1-player]
	    || (actions[player]==actions[1-player] 
		&& values[player]>values[1-player]))
	  obj = ((1.0*values[player])/(numValues-1.0)-winbid);
	else if (actions[player]==actions[1-player] 
		 && values[player]==values[1-player])
	  obj = ((1.0*values[player])/(numValues-1.0)-winbid)/2.0;
	
	obj -= (actions[player]>0? fee : 0.0);
      }
    else if (objectiveIndex==2)
      {
	// Revenue
	obj = winbid;
	obj += (actions[0]>0? fee : 0.0);
	obj += (actions[1]>0? fee : 0.0);
      }
    return obj;
  } // objective

  bool feasibleDeviation(int action, int dev, 
  			 int type, int player) const
  {
    return !(exAnteFee
  	     && action>0 
  	     && dev == 0); // Just exclude deviations from positive to
			   // zero actions.
  }
};

#endif
