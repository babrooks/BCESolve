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

// Header file for fpaunknown example.
// BAB 11-3-2012
#ifndef FPAUNKNOWN_HPP
#define FPAUNKNOWN_HPP

#include "bce.hpp"

class FPAUnknown : public BCEGame
{
private:
  int numValues;
  double entryCost;
  double reservePrice;
  double highbid;
  double lowbid;

public:
  distributionArray distribution;

  FPAUnknown() {distribution.push_back(independent(),1.0);}
  
  FPAUnknown(int na, int nv, double _entryCost, 
	     double _reservePrice,double _highbid) 
    : BCEGame(2,nv*nv,na,1,5), numValues(nv), 
      entryCost(_entryCost), highbid(_highbid),
      lowbid(0.0), reservePrice(_reservePrice)
  {
    distribution.push_back(vToTheAlpha(1.0),1.0);
    // distribution.push_back(uniform(),1.0);
  }

  int getNumValues(){return numValues;}

  int stateToPrivateValues(int state, vector<int> &values)
  {
    values.resize(2);

    values[0]=state%numValues;
    state-=values[0]; 
    values[1]=state/numValues;
  }

  double prior (int state, const vector<int> &types)
  {
    vector<int> values;
    stateToPrivateValues(state,values);
    double v0 = static_cast<double>(values[0]+1)/numValues;
    double v1 = static_cast<double>(values[1]+1)/numValues;
    double incr = 1.0/numValues;

    if (numValues == 2)
      {
    	double p = 0.25;
    	if (v0 == 1 && v1 == 0
    	    || v0 == 0 && v1 == 1)
    	  return p;
    	return (1.0-2*p)/2.0;
      }

    // return PDF(distribution,v0,v1,incr);
    return distribution::PDF(distribution,v0,v1,incr);
  }

  double objective(int state, const vector<int> &actions, int objectiveIndex)
  {
    // Convert the state into a pair of valuations
    vector<int> values(2,0);
    stateToPrivateValues(state,values);

    double obj = 0; 

    double winbid = (actions[0]>actions[1]? actions[0]: actions[1])
      * (highbid-lowbid) / (numActions[0]-1.0) + lowbid;
    // If winning bid is less than reserve price, all objectives are
    // zero.
    if (winbid < reservePrice)
      return 0.0;

    if (objectiveIndex < 2)
      {
	int player = objectiveIndex;

	double mybid = lowbid
	  + (highbid-lowbid)*actions[player]/(numActions[player]-1.0);

	// Player 1's payoff
	if (actions[player]>actions[1-player]
	    || (actions[player]==actions[1-player] 
		&& values[player]>values[1-player]))
	  obj = (1.0*values[player]/(numValues-1.0)-mybid);
	else if (actions[player]==actions[1-player] 
		 && values[player]==values[1-player])
	  obj = (1.0*values[player]/(numValues-1.0)-mybid)/2.0;
		     
	obj -= (actions[player]>0? entryCost : 0.0);
      }
    else if (objectiveIndex==2)
      {
	// Revenue
	obj = winbid;
	obj += (actions[0]>0? entryCost : 0.0);
	obj += (actions[1]>0? entryCost : 0.0);
      }
    else if (objectiveIndex==3)
      {
	// Surplus
	if (actions[0]>actions[1] 
	    || (actions[0]==actions[1] && values[0]>values[1]))
	  obj = (1.0*values[0]/(numValues-1.0)); // Player 1 won
	else
	  obj = (1.0*values[1]/(numValues-1.0)); // Either player 2 won or they tied with same val.
      }
    else if (objectiveIndex==4)
      {
	return objective(state,actions,0)+objective(state,actions,1);
      }
    
    return obj;
  } // objective

  bool dominated(int a, int t, int player)
  {
    return false;
  }
};

#endif
