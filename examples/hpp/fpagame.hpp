// Header file for fpaunknown example.
// BAB 11-3-2012
#ifndef FPAGAME_HPP
#define FPAGAME_HPP

#include "bce.hpp"

class FPAGame : public BCEGame
{
protected:
  int numValues;
  double exPostFee;
  double reservePrice;
  double highbid;
  double lowbid;

public:
  double exAnteFee;
  distributionArray distribution;

  FPAGame() {distribution.push_back(independent(),1.0);}
  
  FPAGame(int na, int nv, double _exPostFee, 
	  double _reservePrice,double _highbid)
    : BCEGame(2,nv*nv,na+1,1,5+2*nv+2), numValues(nv), 
      exPostFee(_exPostFee), highbid(_highbid),
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
    return PDF(distribution,v0,v1,incr);
  }

  double objective(int state, const vector<int> &actions, int objectiveIndex)
  {
    // Convert the state into a pair of valuations
    vector<int> values(2,0);
    stateToPrivateValues(state,values);

    double obj = 0; 

    double winbid = ((actions[0]>actions[1]? actions[0]: actions[1])-1.0)
      * (highbid-lowbid) / (numActions[0]-2.0) + lowbid;
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
	  obj = (1.0*values[player]/(numValues-1.0)-winbid);
	else if (actions[player]==actions[1-player] 
		 && values[player]==values[1-player])
	  obj = (1.0*values[player]/(numValues-1.0)-winbid)/2.0;
	
	obj -= (actions[player]>0? exPostFee : 0.0);
      }
    else if (objectiveIndex==2)
      {
	// Revenue
	obj = winbid;
	obj += (actions[0]>0? exPostFee + exAnteFee : 0.0);
	obj += (actions[1]>0? exPostFee + exAnteFee : 0.0);
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
    else if (objectiveIndex>=5 && objectiveIndex < 5+2*numValues)
      {
	// These objectives evaluate the surplus from positive bids,
	// versus surplus from a zero bid. For each primitive type,
	// this should be large enough to justify the ex-ante entry
	// cost. 
	int value = objectiveIndex - 5;
	int player = value/numValues;
	value -= player*numValues;
	
	if (actions[player]>0 && values[player]==value)
	  return objective(state,actions,player)-exAnteFee;
      }
    else if (objectiveIndex >= 5+2*numValues
	     && objectiveIndex < 5+2*numValues + 2)
      {
	int player = (objectiveIndex-2 - 5)/numValues;
	if (actions[player]>0)
	  return objective(state,actions,player)-exAnteFee;
      }
    
    return obj;
  } // objective

  bool dominated(int a, int t, int player)
  {
    return false;
  }

  friend class FPASolver;
};

#endif
