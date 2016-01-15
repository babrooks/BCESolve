// Header for all pay auction example.
// RJM 9-17-2015

#ifndef ALLPAYAUCTION_HPP
#define ALLPAYAUCTION_HPP

#include "bce.hpp"
#include "bceabstractgame.hpp"
#include "bcegame.hpp"

class AllPayAuction : public BCEAbstractGame {

public:

  AllPayAuction(int nv,int na):
    BCEAbstractGame(nv,na,1,3) 
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
	
	double val = static_cast<double>(state)/(numStates-1.0);
	double ownBid = static_cast<double>(actions[obj])/(numActions[obj]-1.0);
	double otherBid = static_cast<double>(actions[1-obj])/(numActions[obj]-1.0);

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
	
	double b0 = static_cast<double>(actions[0])/(numActions[0]-1.0);
	double b1 = static_cast<double>(actions[1])/(numActions[1]-1.0);

	return b0 + b1;

      }
    
    return 0.0;
  } // objective

};

#endif
