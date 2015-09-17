// Header for all pay auction example.
// RJM 9-17-2015

#ifndef ALLPAYAUCTION_HPP
#define ALLPAYAUCTION_HPP

#include "bce.hpp"

class AllPayAuction : public BCEGame {

public:

  AllPayAuction(int nv,int na):
    BCEGame(2,nv,na,1,3) 
  {}

  double prior (int state,const vector<int> & types) const {

    return 1.0/numStates;

  }

  double objective (int state,
		    const vector<int> & actions,
		    int obj) const {
  
    if (obj<2)
      {
	// obj is the player whose payoff we are returning
	
	double val = static_cast<double>(state)/numStates;
	double ownBid = static_cast<double>(actions[obj])/numActions[obj];
	double otherBid = static_cast<double>(actions[1-obj])/numActions[obj];

	if (actions[obj]>actions[1-obj])
	  return val-ownBid;
	else if (actions[obj]==actions[1-obj])
	  return 0.5*(val-ownBid);
	else if (actions[obj] < actions[1-obj])
	  return -ownBid;

      } // players' payoffs
    else if (obj==2)
      {
	// revenue
	
	double b0 = static_cast<double>(actions[0])/numActions[0];
	double b1 = static_cast<double>(actions[1])/numActions[1];

	return b0 + b1;

      }
  } // objective

};

#endif
