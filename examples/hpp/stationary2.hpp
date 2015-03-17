#ifndef FPAKNOWN_HPP
#define FPAKNOWN_HPP

#include "bce.hpp"
#include <boost/math/distributions/normal.hpp>
#include <math.h>
using boost::math::normal;

class Stationary : public BCEGame
{
private:
  int k;
    // normal cdf
  double mean, stdev;
  normal distr;

public:

  Stationary() {};
  
  Stationary(int _k) 
    : BCEGame(2,2*_k+1,70,1,2),k(_k), mean(k), stdev(3), distr(mean,stdev)
  { };

  double prior (int state, const vector<int> &types)
  {
    return 1.0/(2.0*k+1);

    if (state>0 || state < 2*k)
      return cdf(distr,(state+0.5))-cdf(distr,(state-0.5));
    else
      return cdf(distr,0.5);



    // Use a uniform prior
    // return static_cast<double>((k+1-abs(state-k))*(k+1-abs(state-k)))/(static_cast<double>((k+1)*(k+2)*(2*k+3))/3.0-(k+1)*(k+1));

    // if (state!=k)
    //   return 1.0/2.0/k;
    // return 0.0;
    

  }


  double objective(int state, const vector<int> &actions, int player)
  {
    // Convert the state into a pair of valuations

    vector<double> shade(2,0);
    vector<double> ratio(2,0);

    for (int p=0; p<numPlayers; p++)
      {
	// shade[p] = exp(-0.45-1*static_cast<double>(actions[p])/numActions[p]); 
	// ratio[p] = exp((1.0-2.0*p)*(1.0*state-k)/k);
	shade[p] = 0.1+0.6*static_cast<double>(actions[p])/(numActions[p]-1);
	ratio[p] = exp((1.0-2.0*p)*(1.0*state-k)/k);
      }
    // cout << actions[0] << " " << actions[1] << " " << state << endl;
    // cout << shade[0] << " " << shade[1] << " " << ratio[0] << endl;
    
    if (shade[player]>shade[1-player]*ratio[player]// +1e-12
	|| (shade[player]>=shade[1-player]*ratio[player]//  -1e-12
	    && ratio[player] > 1.0))
      {
	// cout << actions[player] << " " << 1.0-shade[player] << endl;
	return (1.0-shade[player]); 
      }
    else if (shade[player]>=shade[1-player]*ratio[player]//-1e-12
	     && state==k)
      {
	return (1.0-shade[player])/2.0;
      }

    return 0.0;
  }

  bool dominated(int a, int t, int player)
  {
    return (false);
  }
};

#endif
