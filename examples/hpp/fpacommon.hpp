// Header file for fpacommon example.
// BAB 11-3-2012
#ifndef FPACOMMON_HPP
#define FPACOMMON_HPP

#include "bce.hpp"

class FPACommon : public BCEGame
{
private:
  int numValues;
  double entryCost;
  double reservePrice;
  double highbid;
  double lowbid;

public:

  FPACommon() {}
  
  FPACommon(int na, int nv, double _entryCost, 
	     double _reservePrice,double _highbid) 
    : BCEGame(2,nv,na,1,3), numValues(nv), 
      entryCost(_entryCost), highbid(_highbid),
      lowbid(0.0), reservePrice(_reservePrice)
  { }

  double prior (int state, const vector<int> &types) const
  {
    return 1.0/numValues;
  }

  double objective(int state, const vector<int> &actions, int objectiveIndex) const
  {
    // Convert the state into a pair of valuations
    double value = static_cast<double>(state)/(numValues-1);

    double obj = 0; 

    double winbid = (actions[0]>actions[1]? actions[0]: actions[1])
      * (highbid-lowbid) / (numActions[0]-1.0) + lowbid;
    // If winning bid is less than reserve price, all objectives are
    // zero.
    if (winbid < reservePrice)
      return 0.0;

    if (objectiveIndex < 2)
      {
	int player = objectiveIndex;

	double mybid = lowbid
	  + (highbid-lowbid)*actions[player]/(numActions[player]-1.0);

	// Player 1's payoff
	if (actions[player]>actions[1-player])
	  obj = (value-mybid);
	else if (actions[player]==actions[1-player])
	  obj = (value-mybid)/2.0;
	
	obj -= (actions[player]>0? entryCost : 0.0);
      }
    else if (objectiveIndex==2)
      {
	// Revenue
	obj = winbid;
	obj += (actions[0]>0? entryCost : 0.0);
	obj += (actions[1]>0? entryCost : 0.0);
      }
    
    return obj;
  } // objective

  bool dominated(int a, int t, int player) const
  {
    return false;
  }
};

#endif
