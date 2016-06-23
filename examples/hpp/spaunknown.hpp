// Header file for spaunknown example.
// BAB 11-3-2012
#ifndef SPAUNKNOWN_HPP
#define SPAUNKNOWN_HPP

#include "bce.hpp"

class SPAUnknown : public BCEAbstractGame
{
private:
  int numValues;
  double entryCost;
  double highbid;
  double lowbid;

public:
  BCEDistrArray distribution;

  SPAUnknown() {distribution.push_back(new uniform(),1.0);}
  
  SPAUnknown(int na, int nv, double _entryCost,double _highbid) 
    : BCEAbstractGame(nv*nv,na,1,5), numValues(nv), 
      entryCost(_entryCost), highbid(_highbid),
      lowbid(-0.1)
  {
    setNumPrivateStates(vector<int>(2,nv));
    distribution.push_back(new vToTheAlpha(1.0),1.0);
  }

  int getNumValues(){return numValues;} const

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

    if (numValues == 2)
      {
	double p = 0.4;
	if (v0 == 1 && (v1 == 0
			|| (v0 == 0 && v1 == 1) ))
	  return p;
	return (1.0-2*p)/2.0;
      }

    return distribution.PDF(v0,v1,incr);
  }

  double objective(int state, const vector<int> &actions, int objectiveIndex) const
  {
    // Convert the state into a pair of valuations
    vector<int> values(2,0);
    stateToPrivateValues(state,values);

    double obj; 
    if (objectiveIndex < 2)
      {
	int player = objectiveIndex;

	double otherbid = lowbid
	  + (highbid-lowbid)*actions[1-player]/(numActions[1-player]-1.0);

	// Player 1's payoff
	if (actions[player]>actions[1-player] 
	    || (actions[player]==actions[1-player]
		&& values[player]>values[1-player]))
	  obj = (1.0*values[player]/(numValues-1.0)
		 -otherbid
		 -(actions[player]>0? entryCost : 0.0));
	else if (actions[player]==actions[1-player] 
		 && values[player]==values[1-player])
	  obj = ((1.0*values[player]/(numValues-1.0)
		  -otherbid)/2.0-(actions[player]>0? entryCost : 0.0));
	else
	  obj = -(actions[player]>0? entryCost : 0.0);
      }
    else if (objectiveIndex==2)
      {
	// Revenue
	obj = lowbid
	  + (actions[0]>actions[1]? 
	     ((highbid-lowbid)*actions[1])/(numActions[0]-1.0) : 
	     ((highbid-lowbid)*actions[0])/(numActions[1]-1.0));
      }
    else if (objectiveIndex==3)
      {
	// Surplus
	if (actions[0]>actions[1] || (actions[0]==actions[1] && values[0]>values[1]))
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

};

#endif
