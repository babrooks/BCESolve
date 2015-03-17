#ifndef MULTIPLICATIVE_HPP
#define MULTIPLICATIVE_HPP

#include "bce.hpp"
#include <boost/math/distributions/normal.hpp>
#include <math.h>
using boost::math::normal;

class Multiplicative : public BCEGame
{
private:
  int numRatios;
  double cV;
  double cS;
  double C;
  
    // normal cdf

public:

  Multiplicative() {};
  
  Multiplicative(int _numRatios, 
		 int _numActions) 
    : BCEGame(2,2*_numRatios,_numActions,1,2),
      numRatios(_numRatios),
      C(2)
  { };

  double prior(int state,const vector<int> & types)
  {
    if (state>0 && state < numStates-1)
      
  }

  double objective(int state, const vector<int> &actions, int player)
  {
    // Convert the state into a pair of valuations

    vector<double> shade(2,0);
    vector<double> bid(2,0);

    for (int p=0; p<numPlayers; p++)
      {
	bid[p] = pow(cS,-actions[p]-1);
      }
    
    if (bid[player]>bid[1-player]*
	pow(cV,-(1.0-2*player)*(state-numRatios+0.5)))
      return 1-bid[player]; 

    return 0.0;
  }

  bool dominated(int a, int t, int player)
  {
    return (false);
  }
};

#endif
