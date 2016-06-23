// This file is part of the BCESolve library for games of incomplete
// information
// Copyright (C) 2016 Benjamin A. Brooks and Robert J. Minton
// 
// BCESolve free software: you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// BCESolve is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see
// <http://www.gnu.org/licenses/>.
// 
// Benjamin A. Brooks
// ben@benjaminbrooks.net
// Chicago, IL

#include "bceabstractgame.hpp"

BCEAbstractGame::BCEAbstractGame ():
  numPlayers(2),
  numActions(numPlayers,0),
  numStates(0),
  numTypes(numPlayers,0),
  hasProductStructureData(false),
  numPrivateStates(2,0),
  objectiveLabels(2,"DefaultStringTitle")
{
  for (int obj = 0; obj < objectiveLabels.size(); obj++) 
    findLabelRedundancies(obj);
  nameEmptyLabels();
} // Default constructor

BCEAbstractGame::BCEAbstractGame (int numStatesArg, 
				  int numActionsArg, 
				  int numTypesArg, 
				  int numObjectivesArg):
  numPlayers(2),
  numActions(numPlayers,numActionsArg),
  numStates(numStatesArg),
  numTypes(numPlayers,numTypesArg),
  numObjectives(numObjectivesArg),
  hasProductStructureData(false),
  numPrivateStates(2,0),
  objectiveLabels(numObjectivesArg,"DefaultStringTitle")
{
  for (int obj = 0; obj < objectiveLabels.size(); obj++) 
    findLabelRedundancies(obj);
  nameEmptyLabels();
} 

BCEAbstractGame::BCEAbstractGame (int numStatesArg, 
				  const vector<int> & numActionsArg, 
				  const vector<int> & numTypesArg, 
				  int numObjectivesArg):
  numPlayers(2),
  numActions(numActionsArg),
  numStates(numStatesArg),
  numTypes(numTypesArg),
  numObjectives(numObjectivesArg),
  hasProductStructureData(false),
  numPrivateStates(2,0),
  objectiveLabels(numObjectivesArg,"DefaultStringTitle")
{  
  for (int obj = 0; obj < objectiveLabels.size(); obj++) 
    findLabelRedundancies(obj);
  nameEmptyLabels();
} 

// Overloaded version of prior that finds the marginal.
double BCEAbstractGame::prior(int state, int type, int player) const
{
  int stateTypeIndex, currentIndex, playerIndex;
  double probability=0.0;
  vector<int> types(numPlayers,0);

  if (state < 0 || type < 0 || player < 0 ||
      state >= numStates || type >= numTypes[player] ||
      player >= numPlayers)
    throw(BCEException(BCEException::BadArgumentBCEAbstractGame));
  // assert(state>=0);
  // assert(type>=0);
  // assert(player>=0);
  // assert(state<numStates);
  // assert(type<numTypes[player]);
  // assert(player<numPlayers);

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
bool BCEAbstractGame::dominated(const vector<int> &actions, const vector<int> &types) const
{
  if (actions.size() != numPlayers || types.size() != numPlayers)
    throw(BCEException(BCEException::BadArgumentBCEAbstractGame));
  // assert(actions.size()==numPlayers);
  // assert(types.size()==numPlayers);

  for (int playerCounter=0; playerCounter<numPlayers; playerCounter++)
    {
      if (actions[playerCounter] < 0 || types[playerCounter] < 0 ||
	  actions[playerCounter] >= numActions[playerCounter] ||
	  types[playerCounter] >= numTypes[playerCounter])
	throw(BCEException(BCEException::BadArgumentBCEAbstractGame));
      // assert(actions[playerCounter]>=0);
      // assert(types[playerCounter]>=0);
      // assert(actions[playerCounter]<numActions[playerCounter]);
      // assert(types[playerCounter]<numTypes[playerCounter]);

      if (dominated(actions[playerCounter],types[playerCounter],playerCounter))
	return (true);
    }
  return (false);
}

void BCEAbstractGame::nameEmptyLabels() {

  int numObjs = getNumObjectives();
  string compareStr = "DefaultStringTitle";

  for (int obj = 0; obj < 2; obj++) {
    if (objectiveLabels[obj].compare(compareStr) == 0)
      {
	stringstream ss;
	ss << "Player " << obj;
	objectiveLabels[obj] = ss.str();
      }
  }

  int currObj = 2;

  while (currObj < numObjs) {
    if (objectiveLabels[currObj].compare(compareStr) == 0)
      {
	stringstream ss;
	ss << "Objective " << currObj;
	objectiveLabels[currObj] = ss.str();
      }

    currObj++;
  }
}

void BCEAbstractGame::findLabelRedundancies(int obj) {

  string compareStr = objectiveLabels[obj];
  if (compareStr.compare("DefaultStringTitle") == 0)
    return;

  for (int i = 0; i < getNumObjectives(); i++) {
    if (i != obj) {
      if (compareStr.compare(objectiveLabels[i]) == 0) {
	objectiveLabels[obj] += "*";
	findLabelRedundancies(obj);
	return;
      }
    }
  }
}


