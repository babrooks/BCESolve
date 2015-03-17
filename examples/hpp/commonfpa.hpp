#include "bce.hpp"

class CommonFPA : public BCEGame
{
public:
  CommonFPA() {};
  CommonFPA(int nv, int na)
    : BCEGame(2,nv,na,1,3)
  {}; // number of valuations and number of actions

  double prior (int state, const vector<int> &types)
  { return 1.0/numStates;}

  // double prior(int state, const vector<int> &types)
  // {
  //   // Only for two value models;
  //   switch (state)
  //     {
  //     case 0:
  // 	return 1.0/9.0;
  // 	break;

  //     case 1:
  // 	return 2.0/9.0;
  // 	break;

  //     case 2:
  // 	return 2.0/9.0;
  // 	break;

  //     case 3:
  // 	return 4.0/9.0;
  // 	break;

  //     }
  //   return -1.0;
  // }

  double objective(int state, const vector<int> &actions, int objectiveIndex)
  {
    if (objectiveIndex==0)
      {
	// Player 1's payoff
	if (actions[0]>actions[1])
	  return (1.0*state/(numStates-1.0)-1.0*actions[0]/(numActions[0]-1.0));
	else if (actions[0]==actions[1])
	  return (1.0*state/(numStates-1.0)-1.0*actions[0]/(numActions[0]-1.0))/2.0;
	else
	  return (0.0);
      }
    else if (objectiveIndex==1)
      {
	// Player 1's payoff
	if (actions[0]<actions[1])
	  return (1.0*state/(numStates-1.0)-1.0*actions[1]/(numActions[1]-1.0));
	else if (actions[0]==actions[1])
	  return (1.0*state/(numStates-1.0)-1.0*actions[1]/(numActions[1]-1.0))/2.0;
	else
	  return (0.0);
      }
    else if (objectiveIndex==2)
      {
	// Revenue
	if (actions[0]>actions[1])
	  return (1.0*actions[0]/(numActions[0]-1));
	else
	  return (1.0*actions[1]/(numActions[1]-1));
      }
    // Surplus is trivial here. Everyone has the same valuation.
  }

  bool dominated(int a, int t, int player)
  {return false;}
};
