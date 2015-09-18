/* Header for hybrid auctions with 
   reserve and entree fee example.
   RJM 9-18-2015
*/

#ifndef HYBRIDENTRYRESERVE_HPP
#define HYBRIDENTRYRESERVE_HPP

#include "bce.hpp"
#include <math.h>

class HybridEntryReserve : public BCEGame {

private:
  double weightOnOwnBid;
  double reservePrice;
  double entryFee;
  
public:
  HybridEntryReserve(int nv,
		     int na,
		     double _weightOnOwnBid,
		     double _reservePrice,
		     double _entryFee):
    BCEGame(2,nv,na,1,3),
    weightOnOwnBid(_weightOnOwnBid),
    reservePrice(_reservePrice),
    entryFee(_entryFee)
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
	
	double b0 = static_cast<double>(actions[0])/numActions[0];
	double b1 = static_cast<double>(actions[1])/numActions[1];

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
  } // objective


};

#endif
