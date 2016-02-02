// Header file for duopoly example.
// BAB 11-3-2012
#ifndef DUOPOLY_HPP
#define DUOPOLY_HPP

#include "bce.hpp"

class Duopoly : public BCEGame
{
private:
  int numValues;
  double entryCost;
  double margCost;

public:
  distributionArray distribution;

  Duopoly() {distribution.push_back(independent(),1.0);}
  
  Duopoly(int na, int nv, int no, double _entryCost, double _margCost) 
    : BCEAbstractGame(nv*nv,na,1,no), numValues(nv), entryCost(_entryCost)
  {
    distribution.push_back(uniform(),1.0);
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

    // return PDF(distribution,v0,v1,incr);
    return PDF(distribution,v0,v1,incr);
  }

  double objective(int state, const vector<int> &actions, int objectiveIndex)
  {
    // Convert the state into a pair of valuations
    vector<int> values(2,0);
    stateToPrivateValues(state,values);
    
    double valuations[2], prices[2], surpluses[2], entryCosts[2];
    valuations[0] = 1.0*values[0]/(numValues-1.0);
    valuations[1] = 1.0*values[1]/(numValues-1.0);
    prices[0] = 1.0*actions[0]/(numActions[0]-1.0);
    prices[1] = 1.0*actions[1]/(numActions[1]-1.0);
    surpluses[0] = valuations[0] - prices[0];
    surpluses[1] = valuations[1] - prices[1];
    entryCosts[0] = (actions[0]>0? entryCost : 0.0);
    entryCosts[1] = (actions[1]>0? entryCost : 0.0);

    if (objectiveIndex==0)
      {
	// Player 0's payoff
	if (surpluses[0]>=0)
	  {
	    if (surpluses[0]>surpluses[1])
	      return (prices[0]-margCost-entryCosts[0]);
	    else if (surpluses[0] == surpluses[1])
	      return ((prices[0]-margCost)/2-entryCosts[0]);
	  }
	else
	  return -entryCosts[0];
      }
    else if (objectiveIndex==1)
      {
	// Player 1's payoff
	if (surpluses[1]>=0)
	  {
	    if (surpluses[1]>surpluses[0])
	      return (prices[1]-margCost-entryCosts[1]);
	    else if (surpluses[1] == surpluses[0])
	      return ((prices[1]-margCost)/2-entryCosts[1]);
	  }
	else
	  return -entryCosts[1];
      }
    else if (objectiveIndex==2)
      {
	// CS
	return (max(0.0, max(surpluses[0],surpluses[1])));
      }
    else if (objectiveIndex==3)
      {
	// Social surplus
	if (surpluses[0]>max(0.0,surpluses[1]))
	  return (valuations[0]-margCost); // Player 1 won
	else if (surpluses[1]>max(0.0,surpluses[0]))
	  return (valuations[1]-margCost); // Player 1 won
	else if (surpluses[0]==surpluses[1] 
		 && surpluses[0] >= 0);
	return (0.5 * valuations[0] + 0.5 * valuations[1] - margCost);
      }
  }

  bool dominated(int a, int t, int player)
  {
    return false;
  }
};

#endif
