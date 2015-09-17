// Header for hybrid auctions with reserve example.
// RJM 9-17-2015

#ifndef HYBRIDRESERVE_HPP
#define HYBRIDRESERVE_HPP

#include "bce.hpp"

class HybridReserveAuction : public BCEGame {

private:
  double weightOnOwnBid;
  double reservePrice;
  
public:
  HybridReserveAuction(int nv, int na, double _weightOnOwnBid, double _reservePrice):
    BCEGame(2,nv,na,1,3), weightOnOwnBid(_weightOnOwnBid), reservePrice(_reservePrice)
  { }

  double prior (int state, const vector<int> & types) const {
    return 1.0/numStates;    
  } // prior

  double objective (int state, 
		    const vector<int> & actions, 
		    int obj) const {

    if (obj<2)
      {
	double val = static_cast<double>(state)/numStates;
	double ownBid = static_cast<double>(actions[obj])/numActions[obj];
	double otherBid = static_cast<double>(actions[1-obj])/numActions[obj];

	double winningBid = ownBid;
	if (otherBid > ownBid)
	  winningBid = otherBid;

	if (winningBid < reservePrice)
	  return 0.0;

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

	double winningBid = b0;
	if (b1 > b0)
	  winningBid = b1;

	if (winningBid < reservePrice)
	  return 0.0;

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

};

#endif
