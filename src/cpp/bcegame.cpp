#include "bce.hpp"
#include "bcegame.hpp"

BCEGame::BCEGame ():
  BCEAbstractGame()
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

	      for (int dev; dev < game.getNumActions()[player]; dev++)
		{
		  feasibleDeviationData[player][type]
		    [action+dev*game.getNumActions()[player]]
		    = game.feasibleDeviation(action,dev,type,player);
		}
	    } // for action
	} // for type
    } // for player
} // Construct from BCEAbstractGame

