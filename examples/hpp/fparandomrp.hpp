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
  vector<vector<double> > cdf;
  int rpPrecision;
  
public:
  FPARandomRP(int nv,
	      int na,
	      double _rp):
    BCEAbstractGame(nv,na,1,3),
    reservePrice(_rp)
  {
    rpPrecision = 1000;
    // rp*rpPrecision will be a non-decimal for rp no more precise
    // than rpPrecision => actionsUnderRP0 is an int.
    // e.g. rpPrecision = 1000 => no more precise than 1000ths place.
    int actionsUnderRP0 = numActions[0]*reservePrice*rpPrecision;
    int actionsUnderRP1 = numActions[1]*reservePrice*rpPrecision;
    cdf = vector<vector<double> > (2,vector<double>
				   (max(actionsUnderRP0,actionsUnderRP1),0.0));

    double acc0 = 1.00/actionsUnderRP0;
    // Create a uniform distn
    for (int action = 0; action < actionsUnderRP0; action++) {
      cdf[0][action]=acc0;
      acc0 = acc0 + 1.00/actionsUnderRP0;
    }
    cout << "cdf0Max (should be 1): " << cdf[0][actionsUnderRP0-1] << endl;

    double acc1 = 1.00/actionsUnderRP1;
    for (int action = 0; action < actionsUnderRP1; action++) {
      cdf[1][action]=acc1;
      acc1 += 1.00/actionsUnderRP1;
    }
    cout << "cdf1Max (should be 1): " << cdf[1][actionsUnderRP1-1] << endl;

    //A Few Tests
    cout << "test1: " << cdf[0][floor(na/2)*rpPrecision];
    cout << "test2: " << cdf[0][ceil(na/4)*rpPrecision];
    cout << "test3: " << cdf[0][floor(na/5)*rpPrecision];
    cout << "test4: " << cdf[0][floor(na/2.5)*rpPrecision];
  }

  double prior (int state, const vector<int> & types) const {
    return 1.0/numStates;    
  } // prior

  double objective (int state, 
		    const vector<int> & actions, 
		    int obj) const {

    if (obj<2)
      {
	double val = static_cast<double>(state)/(numStates-1);
	double ownBid = static_cast<double>(actions[obj])/(numActions[obj]-1);
	double otherBid = static_cast<double>(actions[1-obj])/(numActions[obj]-1);

	if (ownBid == 0)
	  return 0.0;

	if (ownBid < otherBid)
	  return 0.0;

	if (ownBid > otherBid) {
	  if (ownBid < reservePrice) {
	    return cdf[obj][actions[obj]*rpPrecision]*(val-ownBid);
	  }
	  else if (ownBid >= reservePrice)
	    return val-ownBid;
	}

	else if (ownBid == otherBid) {
	  if (ownBid < reservePrice)
	    return cdf[obj][actions[obj]*rpPrecision]*.5*(val-ownBid);
	  else if (ownBid >= reservePrice)
	    return 0.5*(val-ownBid);
	}

      } // players' payoffs
    else if (obj==2)
      {
	// revenue
	
	double b0 = static_cast<double>(actions[0])/(numActions[0]-1);
	double b1 = static_cast<double>(actions[1])/(numActions[1]-1);

	double winningBid = b0;
	int winningPlayer = 0;
	if (b1 > b0) {
	  winningBid = b1;
	  winningPlayer = 1;
	}

	if (winningBid < reservePrice)
	  return cdf[winningPlayer][actions[winningPlayer]*rpPrecision] * winningBid;

	if (winningBid >= reservePrice)
	  return winningBid;	  

      }
    return 0;
  } // objective


};

#endif
