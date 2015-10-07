// Header for hybrid auctions example.
// BAB 8-25-2015

#ifndef HYBRIDAUCTION_HPP
#define HYBRIDAUCTION_HPP

#include "bce.hpp"

class HybridAuction : public BCEAbstractGame
{
private:
  double weightOnOwnBid;
  
public:
  HybridAuction(int nv, int na, double _weightOnOwnBid):
    BCEAbstractGame(nv,na,1,3), weightOnOwnBid(_weightOnOwnBid)
  { }

  double prior (int state, const vector<int> & types) const
  {
    return 1.0/numStates;
    
    // return static_cast<double>(state)/(numStates*(numStates+1)/2);
  } // prior

  double objective (int state, 
		    const vector<int> & actions, 
		    int obj) const
  {
    if (obj<2)
      {
	// obj is the player whose payoff we are returning
	
	double val = static_cast<double>(state)/numStates;
	double ownBid = static_cast<double>(actions[obj])/numActions[obj];
	double otherBid = static_cast<double>(actions[1-obj])/numActions[obj];

	if (actions[obj]>actions[1-obj])
	  {
	    return val-ownBid*weightOnOwnBid-otherBid*(1.0-weightOnOwnBid);
	  }
	else if (actions[obj]==actions[1-obj])
	  {
	    return 0.5*(val-ownBid*weightOnOwnBid-otherBid*(1.0-weightOnOwnBid));
	  }
      } // players' payoffs
    else if (obj==2)
      {
	// revenue
	
	double b0 = static_cast<double>(actions[0])/numActions[0];
	double b1 = static_cast<double>(actions[1])/numActions[1];

	if (actions[0]>=actions[1])
	  {
	    return weightOnOwnBid*b0+(1.0-weightOnOwnBid)*b1;
	  }
	else
	  {
	    return weightOnOwnBid*b1+(1.0-weightOnOwnBid)*b0;
	  }
      }
  } // objective

}; // HybridAuction

#endif
