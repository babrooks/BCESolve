#ifndef FPAKNOWN_HPP
#define FPAKNOWN_HPP

#include "bce.hpp"
#include <boost/math/distributions/normal.hpp>
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
    : BCEGame(2,2*_k+1,4*_k+1,1,2),k(_k), mean(k), stdev(3), distr(mean,stdev)
  { };

  double prior (int state, const vector<int> &types)
  {

    if (state>0 || state < 2*k)
      return cdf(distr,(state+0.5))-cdf(distr,(state-0.5));
    else
      return cdf(distr,0.5);



    // Use a uniform prior
    // return static_cast<double>((k+1-abs(state-k))*(k+1-abs(state-k)))/(static_cast<double>((k+1)*(k+2)*(2*k+3))/3.0-(k+1)*(k+1));

    // if (state!=k)
    //   return 1.0/2.0/k;
    // return 0.0;
    
    return 1.0/(2.0*k+1);

  }

  double objective(int state, const vector<int> &_actions, int player)
  {
    // Convert the state into a pair of valuations

    int gap;
    vector<int> actions(2,0);

    for (int p=0; p < 2; p++)
      actions[p] = _actions[p];
    
    if (player == 0)
      {
	// Player 1.
	gap = state-k;

	if (actions[player]-actions[1-player]<gap
	    || actions[player]-actions[1-player]==gap && gap > 0)
	  {
	    return static_cast<double>(actions[player]);
	  }
	else if (actions[player]-actions[1-player]==0 && gap == 0)
	  {
	    return static_cast<double>(actions[player])/2.0;
	  }
	else
	  return 0.0;
      }
    else if (player == 1)
      {
	gap = k-state;

	if (actions[player]-actions[1-player]<gap
	    || actions[player]-actions[1-player]==gap && gap > 0)
	  {
	    return static_cast<double>(actions[player]);
	  }
	else if (actions[player]-actions[1-player]==0 && gap == 0)
	  {
	    return static_cast<double>(actions[player])/2.0;
	  }
	else
	  return 0.0;
      }
  }

  bool dominated(int a, int t, int player)
  {
    return (false);
  }
};

#endif
