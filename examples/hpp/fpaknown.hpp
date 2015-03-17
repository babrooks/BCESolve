#ifndef FPAKNOWN_HPP
#define FPAKNOWN_HPP

#include "fpagame.hpp"

class FPAKnown : public FPAGame
{
private:

public:

  FPAKnown() {};
  
  FPAKnown(int na, int nv, double _entryCost, double _reservePrice, 
	   double _highbid) 
    : FPAGame(na,nv,_entryCost,_reservePrice,_highbid)
  { 
    numTypes = vector<int>(2,nv);
  };

  double prior (int state, const vector<int> &types)
  {
    // Use a uniform prior
    vector<int> values(2,0);
    stateToPrivateValues(state,values);
    
    if (values[0]==types[0] && values[1]==types[1])
      {
  	return FPAGame::prior(state,types);
      }
    else
      return 0.0;
  }

  bool dominated(int a, int t, int player)
  {
    if (a>0 && (lowbid + (highbid-lowbid)*(a-1.0)/(numActions[player]-2.0) 
		> (1.0*t)/(numValues-1.0)))
      return (true);
    else
      return (false);
    // return (false);
  }
};

#endif
