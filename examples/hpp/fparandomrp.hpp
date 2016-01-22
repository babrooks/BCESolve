/* Header for random reserve
   price first price auction
*/

#ifndef FPARANDOMRP_HPP
#define FPARANDOMRP_HPP

#include "bce.hpp"
#include <math.h>

class FPARandomRP : public BCEAbstractGame {

private:
  double reservePrice;
  double reserveProb;
  
public:
  FPARandomRP(int nv,
	      int na,
	      double _rp,
	      double _rpProb):
    BCEAbstractGame(nv,na,1,3),
    reservePrice(_rp),
    reserveProb(_rpProb)
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

	if (ownBid < otherBid)
	  return 0.0;

	if (ownBid > otherBid) {
	  if (ownBid < reservePrice)
	    return reserveProb*(val-ownBid);
	  else if (ownBid >= reservePrice)
	    return val-ownBid;
	}

	else if (ownBid == otherBid) {
	  if (ownBid < reservePrice)
	    return reserveProb*.5*(val-ownBid);
	  else if (ownBid >= reservePrice)
	    return 0.5*(val-ownBid);
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
	  return reserveProb * winningBid;

	if (winningBid >= reservePrice)
	  return winningBid;	  
      }
  } // objective


};

#endif
