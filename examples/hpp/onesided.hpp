#ifndef ONESIDED_HPP
#define ONESIDED_HPP

#include "bce.hpp"

class OneSided : public BCEGame
{
private:
  int numValues;
  double entryCost;
  double reservePrice; 
  double vstar;
  double p;

  vector<double> V1;
  vector<double> B;

public:
  double highbid;
  distributionArray distribution;

  OneSided() {};
  
  OneSided(int na, int nv, vector<int> _numTypes, 
	   double _entryCost, double _reservePrice, double _vstar, double _p) 
    : BCEGame(2,nv,vector<int>(2,na),_numTypes,4), numValues(nv), 
      entryCost(_entryCost), 
      reservePrice(_reservePrice),
      highbid(1.0),
      vstar(_vstar),
      p(_p),
      V1(numStates,0),
      B(numStates,0)
  {

    for (int v=0; v<numStates-1; v++)
      {
	V1[v] = vstar*v/(numStates-1);
	B[v] = V1[v];
      }
    V1.back() = 1.0;
    B.back() = vstar;

  };

  double prior (int state, const vector<int> &types)
  {
    // Use a uniform prior
    if (state == types[0])
      {
	if (state<numStates-1)
	  return p/numStates;
	else
	  return (1-p);
      }

    return 0.0;
  }

  double objective(int state, const vector<int> &actions, int objIdx)
  {
    // Convert the state into a pair of valuations
    double winningBid;
    vector<double> values(numPlayers,0);
    
    assert(state<V1.size());
    values[0] = V1[state];
    values[1] = vstar;

    if (objIdx < 2)
      {
	// Players 1 and 2
	double bid = B[actions[objIdx]];
	if (actions[objIdx] > actions[1-objIdx]
	    || (actions[0]==actions[1] && values[objIdx]>values[1-objIdx]+1e-9))
	  return values[objIdx] - bid;
	if (actions[0] == actions[1] && abs(values[0]-values[1])<1e-9)
	  return (values[objIdx] - bid)/2.0;
      }
    else if (objIdx==2)
      {
	// Revenue
	winningBid 
	  = (actions[0]>actions[1]? 
	     B[actions[0]] : B[actions[1]]);
	return (winningBid > reservePrice? winningBid : 0.0);
      }
    else if (objIdx==3)
      {
	// Surplus
	if (actions[0]>actions[1]
	    || (actions[0]==actions[1] && values[0]>values[1]))
	  return values[0]; // Player 1 won
	else
	  return values[1]; // Either player 2 won or they tied with same val.
      }
  }

  bool dominated(int a, int t, int player)
  {
    double bid = B[a];

    if (player==0)
      {
	if (bid > V1[t]+1e-9)
	  return (true);
	else if ( t < numStates-1 && a!=t)
	  return true;
      }
    else if (player==1)
      {
	assert(t==0);

	if ( bid > vstar +1e-9 )
	  return true;
      }
    return (false);
  }
};

#endif
