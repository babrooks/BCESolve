#include "bcecounter.hpp"

void BCECounter::initialize()
{
  int player;
  int action, type;

  int totalIncrement = 1;
  int totalIncrementMarginal = 1;
  int totalIncrementStateType = 1;

  int totalDecrement = 0;
  int decrementMarginal = 0;
  int decrementStateType = 0;

  int numStateTypeVariables = numStates;
  int numVariables = numStates;

  numMarginalVariables = 1;

  // Replace empty sets with sets that contain everything.
  for (player=0; player<numPlayers; player++)
    {
      if (actionConditions[player].size()==0)
	{
	  actionConditions[player].resize(numActions[player]);
	  for (action = 0; action < numActions[player]; action++)
	    actionConditions[player][action] = action;
	}
      if (typeConditions[player].size()==0)
	{
	  typeConditions[player].resize(numTypes[player]);
	  for (type = 0; type < numTypes[player]; type++)
	    typeConditions[player][type] = type;
	}
    }
  if (stateConditions.size()==0)
    {
      stateConditions.resize(numStates);
      for(state = 0; state < numStates; state++)
	stateConditions[state]=state;
    }

  // Find the increments for each variable.  
  if (stateMarginal)
    {
      stateIncrementMarginal = 1;
      decrementMarginal = stateConditions.back() - stateConditions.front();
      decrementStateType = decrementMarginal;

      numMarginalVariables *= numStates;
    }
  totalDecrement = stateConditions.back() - stateConditions.front();
  decrementStateType = stateConditions.back() - stateConditions.front();

  for (player = 0; player < numPlayers; player++)
    {
      // Marginal action increment/decrement
      actionDecrementsMarginal[player] = decrementMarginal;
      if (actionMarginal[player])
	{
	  actionIncrementsMarginal[player] = numMarginalVariables;
	  decrementMarginal += (actionConditions[player].back()
				- actionConditions[player].front())*numMarginalVariables;

	  numMarginalVariables *= numActions[player];
	}

      // Action increment/decrement
      actionIncrements[player] = numVariables;
      actionDecrements[player] = totalDecrement;

      totalDecrement += (actionConditions[player].back()
			 - actionConditions[player].front()) * numVariables;
      numVariables *= numActions[player];
    }
  for (player = 0; player < numPlayers; player++)
    {
      // Marginal type increment/decrement
      typeDecrementsMarginal[player] = decrementMarginal;
      if (typeMarginal[player])
	{
	  typeIncrementsMarginal[player] = numMarginalVariables;
	  decrementMarginal += (typeConditions[player].back()
				- typeConditions[player].front())*numMarginalVariables;

	  numMarginalVariables *= numTypes[player];
	}

      // Type increment/decrement
      typeIncrements[player] = numVariables;
      typeDecrements[player] = totalDecrement;

      totalDecrement += (typeConditions[player].back()
			 - typeConditions[player].front()) * numVariables;
      numVariables *= numTypes[player];

      // StateType type increment/decrement
      typeIncrementsStateType[player] = numStateTypeVariables;
      typeDecrementsStateType[player] = decrementStateType;
      decrementStateType += (typeConditions[player].back()
			     - typeConditions[player].front())*numStateTypeVariables;
      numStateTypeVariables *= numTypes[player];

    }

  // Initialize counters
  state = stateConditions[0];
  for (player = 0; player < numPlayers; player++)
    {
      actions[player] = actionConditions[player][0]; 
      types[player] = typeConditions[player][0];
    }  

  variable = 0;
  stateType = 0;
  marginal = 0;
  for (player=numPlayers-1; player>=0; player--)
    {
      variable = variable * numTypes[player] + types[player];
      stateType = stateType * numTypes[player] + types[player];

      if (typeMarginal[player])
	marginal = marginal * numTypes[player] + types[player];
    }
  for (player=numPlayers-1; player >= 0; player--)
    {
      variable = variable * numActions[player] + actions[player];
      
      if (actionMarginal[player])
	marginal = marginal * numActions[player] + actions[player];
    }
  variable = variable * numStates + state;
  stateType = stateType * numStates + state;

  if (stateMarginal)
    marginal = marginal * numStates + state;

  stateIndex = 0; 
  actionIndices = vector<int>(2,0); 
  typeIndices = vector<int>(2,0);

  // state = 0;
  // actions = vector<int>(2,0);
  // types = vector<int>(2,0);
} // initialize


bool BCECounter::operator++()
{
  int player;
  int increment;

  // Look at state.
  if (stateIndex != stateConditions.size()-1)
    {
      stateIndex ++;
      state = stateConditions[stateIndex];
	      
      increment = stateConditions[stateIndex] - stateConditions[stateIndex-1];
      if (increment <= 0)
	throw(BCEException(BCEException::BCECounterConditionFailed));
      assert(increment > 0);

      variable += increment;
      stateType += increment;
      marginal += increment * stateIncrementMarginal;

      return true;
    }

  // Look at actions
  for (player = 0; player < numPlayers; player++)
    {
      if (actionIndices[player] != actionConditions[player].size() - 1)
	{
	  // We can increment this action for this player.
		  
	  actionIndices[player]++;
		  
	  increment = actionConditions[player][actionIndices[player]] - actions[player];
	  if (increment <= 0)
	    throw(BCEException(BCEException::BCECounterConditionFailed));
	  assert(increment > 0);

	  variable += increment * actionIncrements[player]
	    - actionDecrements[player];
	  marginal += increment * actionIncrementsMarginal[player] 
	    - actionDecrementsMarginal[player];
	  stateType += stateConditions.front() - stateConditions.back();

	  // Reset actionIndices
	  while (--player >= 0)
	    actionIndices[player] = 0;

	  for (player=0; player<numPlayers; player++)
	    actions[player]=actionConditions[player][actionIndices[player]];
	  stateIndex = 0;
	  state = stateConditions[0];

	  return true;
	}
    } // player

  // Look at typeIndices
  for (player = 0; player<numPlayers; player++)
    {
      if (typeIndices[player] != typeConditions[player].size() - 1)
	{
	  // We can increment this type.

	  typeIndices[player]++;

	  increment = typeConditions[player][typeIndices[player]] - types[player];
	  if (increment <= 0)
	    throw(BCEException(BCEException::BCECounterConditionFailed));
	  assert(increment > 0);

	  variable += increment * typeIncrements[player]
	    - typeDecrements[player];
	  stateType += increment * typeIncrementsStateType[player] 
	    - typeDecrementsStateType[player];
	  marginal += increment * typeIncrementsMarginal[player]
	    - typeDecrementsMarginal[player];
	  
	  // Reset actionIndices and typeIndices.
	  while (--player >= 0)
	    typeIndices[player] = 0;

	  stateIndex = 0;
	  state = stateConditions[0];
	  actionIndices = vector<int>(2,0);
	  for (player=0; player<numPlayers; player++)
	    {
	      actions[player] = actionConditions[player][actionIndices[player]];
	      types[player] = typeConditions[player][typeIndices[player]];
	    }

	  return true;
	}
    } // player

  return false;
} // incrementCounters


