// Game class for BCE
// BAB 12/29/2011
#ifndef BCEGAME_HPP
#define BCEGAME_HPP

#include "bcecommon.hpp"
#include "bceexception.hpp"

// The base class for all BCE calculations.
class BCEGame 
{
protected:
  int numPlayers; // Number of players.
  vector<int> numActions; // number of action profiles for each player
  int numStates; // number of payoff relevant states
  vector<int> numTypes; // number of private types for each player
  int numObjectives; // number of objective functions. Must be >= 2.

  // The prior over types and values
  virtual double prior(int state, const vector<int> &types) = 0;
  double prior(int state, int type, int player); // marginal distribution

  // Maps actions and values into objective functions. The argument
  // objectiveIndex can be any number between 0 and numObjectives-1,
  // inclusive.  objective=0 through numPlayers-1 are the payoffs of
  // players 1 through numPlayers, respectively.
  virtual double objective(int state, const vector<int> &actions, int objectiveIndex) = 0; 

  // Indicates if a combination of actions and types is dominated.
  virtual bool dominated(int action, int type, int player) {return false;};
  // Check if no players' action is dominated
  bool dominated(const vector<int> &actions, const vector<int> &types); 

public:
  // Constructors
  BCEGame();
  BCEGame(int numPlayersArg, 
	  int numStatesArg, 
	  int numActionsArg, 
	  int numTypesArg, 
	  int numObjectivesArg);
  BCEGame(int numPlayersArg, 
	  int numStatesArg, 
	  const vector<int> & numActionsArg, 
	  const vector<int> & numTypesArg, 
	  int numObjectivesArg);

  // Destructor
  ~BCEGame()
  {} 

  friend class BCESolver;
};

#endif
