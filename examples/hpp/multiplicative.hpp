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
  
    // normal cdf

public:

  Multiplicative() {};
  
  Multiplicative(double _cV, int _numRatios, 
		 double _cS, int _numActions) 
    : BCEGame(2,2*_numRatios,_numActions,1,2), cV(_cV), 
      numRatios(_numRatios), cS(_cS)
  { };

  double prior(int state,const vector<int> & types)
  {
    return 1.0/( (double) numStates);
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
    
    double ratio = pow(cV,-(1.0-2*player)*(state-numRatios+0.5));
    //double ratio = pow(cV,-(1-2*player)*(state-numRatios));

    if (bid[player]>bid[1-player]*ratio)
      return 1-bid[player]; 
    // else if (bid[player]==bid[1-player] && state==numRatios)
    //   return (1-bid[player])/2.0;

    return 0.0;
  }

  bool dominated(int a, int t, int player)
  {
    return (false);
  }
};

#endif
