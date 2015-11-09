#include "bce.hpp"
#include "bcegame.hpp"

BCEGame::BCEGame():
  BCEAbstractGame(1,1,1,2),
  objectiveData(2,vector< vector<double> > (1, vector<double> (1,0.0))),
  priorData(1,vector<double>(1,1.0)),
  dominatedData(2,vector< vector<bool> >(1,vector<bool>(1,false))),
  feasibleDeviationData(2,vector< vector<bool> > (1,vector<bool>(1,false)))
{} // Default constructor

BCEGame::BCEGame(const BCEAbstractGame & game):
  BCEAbstractGame(game),
  objectiveData(game.getNumObjectives()),
  priorData(game.getNumStates()),
  dominatedData(2),
  feasibleDeviationData(2)
{
  int numActionsTotal = game.getNumActions()[0]*game.getNumActions()[1];
  int numTypesTotal = game.getNumTypes()[0]*game.getNumTypes()[1];

  // Copy objectives
  for (int obj = 0; obj < game.getNumObjectives(); obj++)
    {
      objectiveData[obj]
	= vector< vector<double> > (numStates,
				    vector< double> (numActionsTotal,0.0));
      for (int state = 0; state < game.getNumStates(); state++)
	{
	  vector<int> actions(2,0);
	  int actionCounter = 0;
	  while (actionCounter < numActionsTotal)
	    {
	      objectiveData[obj][state][actionCounter]
		= game.objective(state,actions,obj);
	      
	      if (actions[0] != numActions[0]-1)
		actions[0]++;
	      else
		{
		  actions[0]=0;
		  actions[1]++;
		}
	      actionCounter++;
	    } // while actions
	} // for state
    } // for obj

  // Copy prior
  for (int state = 0; state < game.getNumStates(); state++)
    {
      priorData[state] = vector<double>(numTypesTotal,0.0);
      vector<int> types(2,0);
      int typeCounter = 0;
      while (typeCounter < numTypesTotal)
	{
	  priorData[state][typeCounter]
	    = game.prior(state,types);
	      
	  if (types[0] != numTypes[0]-1)
	    types[0]++;
	  else
	    {
	      types[0]=0;
	      types[1]++;
	    }
	  typeCounter++;
	} // while types
    } // for state
  
  // Copy dominated and feasible data
  for (int player = 0; player < 2; player++)
    {
      dominatedData[player]
	= vector< vector<bool> >(game.getNumTypes()[player],
				 vector<bool>( game.getNumActions()[player],false));
      feasibleDeviationData[player]
	= vector< vector<bool> > (game.getNumTypes()[player],
				  vector<bool> (game.getNumActions()[player]
						*game.getNumActions()[player],false));
      for (int type = 0; type < game.getNumTypes()[player]; type++)
	{
	  for (int action=0; action < game.getNumActions()[player]; action++)
	    {
	      dominatedData[player][type][action] = game.dominated(action,type,player);

	      for (int dev = 0; dev < game.getNumActions()[player]; dev++)
		{
		  feasibleDeviationData[player][type]
		    [action+dev*game.getNumActions()[player]]
		    = game.feasibleDeviation(action,dev,type,player);
		}
	    } // for action
	  
	} // for type
    } // for player

} // Construct from BCEAbstractGame

bool BCEGame::addObjective(int position)
{
  if (position < 2 || position > numObjectives)
    return false;

  objectiveData.insert(objectiveData.begin() + position,
		       vector< vector<double> > (numStates,
						 vector<double>(numActions[0]*numActions[1],0.0)));
  
  numObjectives++;
  return true;
} // addObjective

bool BCEGame::removeObjective(int obj)
{
  if (obj < 2 || obj >= numObjectives)
    return false;

  objectiveData.erase(objectiveData.begin() + obj);
  numObjectives--;

  return true;
} // removeObjective

bool BCEGame::addState(int position)
{
  if (position < 0 || position > numStates)
    return false;

  numStates++;

  for (int obj = 0; obj < numObjectives; obj++)
    objectiveData[obj].insert(objectiveData[obj].begin()+position,
			      vector<double>(numActions[0]*numActions[1],0.0));
  priorData.insert(priorData.begin()+position,
		   vector<double>(numTypes[0]*numTypes[1],0.0));
    
} // add state

bool BCEGame::removeState(int state)
{
  if (state == 0 && numStates == 1)
    return false;
  if (state<0 || state >= numStates)
    return false;

  numStates--;
  for (int obj = 0; obj < numObjectives; obj++)
    objectiveData[obj].erase(objectiveData[obj].begin() + state);
  priorData.erase(priorData.begin()+state);
    
  return true;
} // remove state

bool BCEGame::addType(int player, int position)
{
  if (player < 0 || player > 1)
    return false;
  if (position < 0 || position > numTypes[player])
    return false;
    
  numTypes[player]++;

  dominatedData[player].insert(dominatedData[player].begin()+position,
			       vector<bool>(numActions[player],false));
  feasibleDeviationData[player]
    .insert(feasibleDeviationData[player].begin()+position,
	    vector<bool>(numActions[player]*numActions[player],true));

  int ownIncrement = 1+player*(numTypes[1-player]-1);
  int otherIncrement = 1+(1-player)*(numTypes[player]-1);
    
  for (int state = 0; state < numStates; state++)
    {
      priorData[state].reserve(priorData[state].size()+numTypes[1-player]);
      for (int otherType = 0; otherType < numTypes[1-player]; otherType++)
	priorData[state].insert(priorData[state].begin()
				+position*ownIncrement
				+otherType*otherIncrement,
				0.0);
    } // for state
    
  return true;
} // addType

bool BCEGame::removeType(int player, int type)
{
  if (player < 0 || player > 1)
    return false;
  if (numTypes[player]==1)
    return false;
  if (type < 0 || type >= numTypes[player])
    return false;
    
  dominatedData[player].erase(dominatedData[player].begin()+type);
  feasibleDeviationData[player]
    .erase(feasibleDeviationData[player].begin()+type);

  int ownIncrement = 1+player*(numTypes[1-player]-1);
  int otherIncrement = 1+(1-player)*(numTypes[player]-1);

  for (int state = 0; state < numStates; state++)
    {
      for (int otherType = numTypes[1-player]-1;
	   otherType >= 0;
	   otherType--)
	priorData[state].erase(priorData[state].begin()
			       +type*ownIncrement
			       +otherType*otherIncrement);
    } // for state

  numTypes[player]--;
    
  return true;
} // removeType

bool BCEGame::addAction(int player, int position)
{
  if (player < 0 || player > 1)
    return false;
  if (position < 0 || position > numActions[player])
    return false;
    
  numActions[player]++;

  for (int type = 0; type < numTypes[player]; type++)
    {
      dominatedData[player][type]
	.insert(dominatedData[player][type].begin()+position,false);

      feasibleDeviationData[player][type]
	.reserve(feasibleDeviationData[player][type].size()
		 + numActions[0]+numActions[1]-1);
	
      for (int dev = 0; dev < numActions[player]; dev++)
	{
	  if (dev == position)
	    {
	      // Insert values for the new action and all deviations
	      feasibleDeviationData[player][type]
		.insert(feasibleDeviationData[player][type].begin()
			+position+dev*numActions[player],
			numActions[player],true);
	    }
	  else
	    {
	      // Just insert value for the new deviation
	      feasibleDeviationData[player][type]
		.insert(feasibleDeviationData[player][type].begin()
			+position+dev*numActions[player],true);
	    }
	} // for dev
    } // for type

  int ownIncrement = 1+player*(numActions[1-player]-1);
  int otherIncrement = 1+(1-player)*(numActions[player]-1);
    
  for (int obj = 0; obj < numObjectives; obj++)
    {
      for (int state = 0; state < numStates; state++)
	{
	  objectiveData[obj][state]
	    .reserve(objectiveData[obj][state].size()+numActions[1-player]);

	  for (int otherAction = 0; 
	       otherAction < numActions[1-player]; 
	       otherAction++)
	    objectiveData[obj][state]
	      .insert(objectiveData[obj][state].begin()
		      +position*ownIncrement
		      +otherAction*otherIncrement,
		      0.0);
	} // for obj
    } // for state

  return true;
} // addAction

bool BCEGame::removeAction(int player, int action)
{
  if (player < 0 || player > 1)
    return false;
  if (numActions[player]==1)
    return false;
  if (action < 0 || action >= numActions[player])
    return false;

  for (int type = 0; type < numTypes[player]; type++)
    {
      dominatedData[player][type]
	.erase(dominatedData[player][type].begin() + action);

      for (int dev = numActions[player]-1; dev >= 0; dev--)
	{
	  if (dev == action)
	    {
	      // Insert values for the new action and all deviations
	      feasibleDeviationData[player][type]
		.erase(feasibleDeviationData[player][type].begin()
		       +action+action*numActions[player],
		       feasibleDeviationData[player][type].begin()
		       +action+(action+1)*numActions[player]);
	    }
	  else
	    {
	      // Just insert value for the new deviation
	      feasibleDeviationData[player][type]
		.erase(feasibleDeviationData[player][type].begin()
		       +action+dev*numActions[player]);
	    }
	} // for dev
    } // for type

  int ownIncrement = 1+player*(numActions[1-player]-1);
  int otherIncrement = 1+(1-player)*(numActions[player]-1);
    
  for (int obj = 0; obj < numObjectives; obj++)
    {
      for (int state = 0; state < numStates; state++)
	{
	  objectiveData[obj][state]
	    .reserve(objectiveData[obj][state].size()+numActions[1-player]);

	  for (int otherAction = numActions[1-player]-1; 
	       otherAction >= 0 ; 
	       otherAction--)
	    objectiveData[obj][state]
	      .erase(objectiveData[obj][state].begin()
		     +action*ownIncrement
		     +otherAction*otherIncrement);
	} // for obj
    } // for state

  numActions[player]--;

  return true;
} // removeAction
