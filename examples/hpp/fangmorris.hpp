// This file is part of the BCESolve library for games of incomplete
// information
// Copyright (C) 2022 Benjamin A. Brooks
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

// Implements the game in Fang and Morris (2006)
//! @example
#include "bce.hpp"

class FangMorris : public BCEAbstractGame
{
private:
  int numValues;
  double entryCost;
  double signalAccuracy;

public:
  BCEDistrArray distribution;

  FangMorris()
  { distribution.push_back(new bernoulli(1.0/3.0),1.0);};
  
  FangMorris(int nb, double _entryCost, double _signalAccuracy) 
    : BCEAbstractGame(4,nb,4,4), numValues(2), entryCost(_entryCost),
      signalAccuracy(_signalAccuracy)
  { distribution.push_back(new bernoulli(1.0/3.0),1.0);}

  FangMorris(int nvalues, int nb, double _entryCost, double _signalAccuracy) 
    : BCEAbstractGame(4,nb,4,4), numValues(2), entryCost(_entryCost),
      signalAccuracy(_signalAccuracy)
  { distribution.push_back(new bernoulli(1.0/3.0),1.0);}

  void stateToPrivateValues(int state, vector<int> &values) const
  {
    values.resize(2);

    values[0]=state%numValues;
    state-=values[0]; 
    values[1]=state/numValues;
  }

  double prior (int state, const vector<int> &types) const
  {
    // Use a uniform prior
    vector<int> values(2,0);
    int localType;
    double prob=0.0;
    stateToPrivateValues(state,values);

    if ((values[0]==types[0]%2)
	&& (values[1]==types[1]%2))
      {
	// Uniform
	// return (1.0/numValues/numValues);

	// Stephen's example
	// Independent values, prob of high is 2/3.
	double v0 = static_cast<double>(values[0]+1)/numValues;
	double v1 = static_cast<double>(values[1]+1)/numValues;
	double incr = 1.0/numValues;

	// return PDF(distribution,v0,v1,incr);
	prob = distribution.PDF(v0,v1,incr);

	// Code players own value with type%2 and other player's value with type/2
	if (values[0] == types[1]/2)
	  prob *= signalAccuracy;
	else
	  prob *= (1.0-signalAccuracy);
	if (values[1] == types[0]/2)
	  prob *= signalAccuracy;
	else
	  prob *= (1.0-signalAccuracy);
      } // if

    return prob;
  }

  double objective(int state, const vector<int> &actions, int objectiveIndex) const
  {
    // Convert the state into a pair of valuations
    vector<int> values(2,0);
    stateToPrivateValues(state,values);

    if (objectiveIndex==0)
      {
	// Player 1's payoff
	if (actions[0]>actions[1] || (actions[0]==actions[1] && values[0]>values[1]))
	  return (1.0*values[0]/(numValues-1.0)-1.0*actions[0]/(numActions[0]-1.0)
		  -(actions[0]? entryCost : 0.0));
	else if (actions[0]==actions[1] && values[0]==values[1])
	  return ((1.0*values[0]/(numValues-1.0)-1.0*actions[0]/(numActions[0]-1.0))/2.0
		  -(actions[0]? entryCost : 0.0));
	else
	  return -(actions[0]? entryCost : 0.0);
      }
    else if (objectiveIndex==1)
      {
	// Player 1's payoff
	if (actions[0]<actions[1] || (actions[0]==actions[1] && values[0]<values[1]))
	  return (1.0*values[1]/(numValues-1.0)-1.0*actions[1]/(numActions[1]-1.0)
		  -(actions[1]? entryCost : 0.0));
	else if (actions[0]==actions[1] && values[0]==values[1])
	  return ((1.0*values[1]/(numValues-1.0)-1.0*actions[1]/(numActions[1]-1.0))/2.0
		  -(actions[1]? entryCost : 0.0));
	else
	  return -(actions[1]? entryCost : 0.0);
      }
    else if (objectiveIndex==2)
      {
	// Revenue
	return (actions[0]>actions[1]? (1.0*actions[0])/(numActions[0]-1.0) : 
		(1.0*actions[1])/(numActions[1]-1.0));
      }
    else if (objectiveIndex==3)
      {
	// Surplus
	if (actions[0]>actions[1] || (actions[0]==actions[1] && values[0]>values[1]))
	  return (1.0*values[0]/(numValues-1.0)); // Player 1 won
	else
	  return (1.0*values[1]/(numValues-1.0)); // Either player 2 won or they tied with same val.
      }
    return 0;
  }

  bool dominated(int a, int t, int player)
  {
    if ((static_cast<double>(a))/(static_cast<double>(numActions[player]) - 1.0) 
    	> (static_cast<double>(t%2)))
      return (true);
    else
      return (false);
    // return (false);
  }
};
