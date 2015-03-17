#include "bce.hpp"
#include "bcegame.hpp"

BCEGame::BCEGame ():
  numPlayers(2),
  numActions(numPlayers,0),
  numStates(0),
  numTypes(numPlayers,0)
{} // Default constructor

BCEGame::BCEGame (int numPlayersArg, 
		  int numStatesArg, 
		  int numActionsArg, 
		  int numTypesArg, 
		  int numObjectivesArg):
  numPlayers(numPlayersArg),
  numActions(numPlayers,numActionsArg),
  numStates(numStatesArg),
  numTypes(numPlayers,numTypesArg),
  numObjectives(numObjectivesArg)
{} 

BCEGame::BCEGame (int numPlayersArg, 
		  int numStatesArg, 
		  const vector<int> & numActionsArg, 
		  const vector<int> & numTypesArg, 
		  int numObjectivesArg):
  numPlayers(numPlayersArg),
  numActions(numActionsArg),
  numStates(numStatesArg),
  numTypes(numTypesArg),
  numObjectives(numObjectivesArg)
{} 

// Overloaded version of prior that finds the marginal.
double BCEGame::prior(int state, int type, int player)
{
  int stateTypeIndex, currentIndex, playerIndex;
  double probability=0.0;
  vector<int> types(numPlayers,0);

  assert(state>=0);
  assert(type>=0);
  assert(player>=0);
  assert(state<numStates);
  assert(type<numTypes[player]);
  assert(player<numPlayers);

  // Fill in player's valuation and type.
  types[player]=type;
  
  int numTypes_total = 1;
  for (playerIndex = 0; playerIndex < numPlayers; playerIndex++)
    numTypes_total *= numTypes[playerIndex];

  for (stateTypeIndex=0; 
       stateTypeIndex<numStates*(numTypes_total-numTypes[player]); 
       stateTypeIndex++)
    {
      currentIndex=stateTypeIndex;

      // Find other players' types/valuations
      for (playerIndex=0; playerIndex<numPlayers; playerIndex++)
	{
	  // Skip p==player
	  if (playerIndex==numPlayers-1 && playerIndex==player)
	    break;
	  else if (playerIndex==player)
	    playerIndex++;
	  
	  types[playerIndex]=currentIndex%numTypes[playerIndex];
	  currentIndex-=types[playerIndex]; currentIndex/=numTypes[playerIndex];
	}
      // The state is currentIndex
      probability+=prior(currentIndex,types);
    }

  return (probability);
} // prior

// Checks if any player has a dominated action/type.
bool BCEGame::dominated(const vector<int> &actions, const vector<int> &types)
{
  assert(actions.size()==numPlayers);
  assert(types.size()==numPlayers);

  for (int playerCounter=0; playerCounter<numPlayers; playerCounter++)
    {
      assert(actions[playerCounter]>=0);
      assert(types[playerCounter]>=0);
      assert(actions[playerCounter]<numActions[playerCounter]);
      assert(types[playerCounter]<numTypes[playerCounter]);

      if (dominated(actions[playerCounter],types[playerCounter],playerCounter))
	return (true);
    }
  return (false);
}

