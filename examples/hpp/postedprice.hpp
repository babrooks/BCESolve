#ifndef POSTEDPRICE_HPP
#define POSTEDPRICE_HPP

#include "bce.hpp"

class PostedPrice : public BCEAbstractGame {

private:
  double price;
  vector<double> prPlayerWins;

public:
  PostedPrice(int nv,
	      double _price,
	      double _prPlayer0Wins):
    // 2 actions ("buy","don't buy"), 1 type, 3 objectives
    BCEAbstractGame(nv,2,1,3),
    price(_price)
  {
    prPlayerWins = vector<double>(2,0);
    prPlayerWins[0] = _prPlayer0Wins;
    prPlayerWins[1] = 1-_prPlayer0Wins;
  }

  double prior (int state, const vector<int> & types) const {
    return 1.0/numStates; 
  }

  double objective (int state, 
		    const vector<int> & actions, 
		    int obj) const {

    if (obj<2)
      {
	double val = static_cast<double>(state)/(numStates-1);
	int ownChoice = actions[obj];
	int otherChoice = actions[1-obj];

	if (ownChoice == 0)
	  return 0.0;

	if (ownChoice == 1 && otherChoice ==1)
	  return prPlayerWins[obj]*(val-price);
	
	if (ownChoice ==1)
	  return val-price;

      } // players' payoffs
    else if (obj==2)
      {
	// revenue
	
	if (actions[0] == 1 || actions[1] == 1)
	  return price;
	  
      }
  } // objective 

};

#endif
