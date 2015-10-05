#ifndef BCEABSTRACTGAME_HPP
#define BCEABSTRACTGAME_HPP

#include "bcecommon.hpp"
#include "bceexception.hpp"

//! The base class for games of incomplete information.
/*! This is a pure virtual base class from which to create classes
    that represent games of incomplete information. This class is used
    by the solver class, BCESolver.

    To create a game, you derive from this class and reimplement the
    BCEAbstractGame::prior and BCEAbstractGame::objective methods to return,
    respectively, the prior distribution over states and types, as
    well as the objective functions for the players and possible
    auxiliary objective functions. The user can also reimplement the
    BCEAbstractGame::dominated method to indicate if there are dominated
    actions that a given type should never play. 

    \ingroup src

*/
class BCEAbstractGame 
{

  /**
   *\example fpagame.hpp
   *\example duopoly.hpp
   */

protected:
  //! The number of players, always 2.
  int numPlayers; 
  //! The number of action profiles for each player. 
  vector<int> numActions; 
  //!  The number of payoff relevant states.
  int numStates; 
  //! The number of private types for each player.
  vector<int> numTypes; 
  //! The number of objective functions. Must be >= 2.
  int numObjectives; 

public:
  //! Prior over state and types
  /*! For each state and vector of types, returns the prior
      probability of those occuring. This is a pure virtual function
      that must be implemented by the derived class. */
  virtual double prior(int state, const vector<int> &types) const = 0;

  //! Marginal prior distribution
  /*! Calculates the marginal joint distribution of a given player's
      type and the state. */
  double prior(int state, int type, int player) const; // marginal distribution

  //! Objective function
  /*! Maps actions and values into objective functions. The argument
    objectiveIndex can be any number between 0 and numObjectives-1,
    inclusive.  objective=0 through numPlayers-1 are the payoffs of
    players 1 through numPlayers, respectively. This is a pure virtual
    function that must be implemented by the derived class. */
  virtual double objective(int state, const vector<int> &actions, 
			   int objectiveIndex) const = 0; 

  //! Indicates if a combination of actions and types is dominated.
  /*! If a combination of actions and types is dominated, the
      algorithm will force the probability of taking that action
      (given that type) to be zero. This is a virtual method that can
      be reimplemented. By default, no action is dominated. */
  virtual bool dominated(int action, int type, int player) const
  {return false;};

  //! Check if no player's action is dominated
  /*! Utility function that iteratively calls
      BCESolver::dominated(int,int,int) for each player, and returns
      false if all of the calls returned false. */
  bool dominated(const vector<int> &actions, const vector<int> &types) const; 

  //! Check if deviation is feasible
  /*! Indicates whether or not it is feasible to deviate from the
    action to the alternative dev, when the player's type is
    type. This is a virtual method that can be reimplemented. By
    default, all deviations are feasible. */
  virtual bool feasibleDeviation(int action, int dev, 
				 int type, int player) const
  {return true;};

  int getNumObjectives() const { return numObjectives; }
  int getNumStates() const { return numStates; }
  const vector<int> & getNumActions() const { return numActions; }
  const vector<int> & getNumTypes() const { return numTypes; }
  
  //! Default constructor
  BCEAbstractGame();
  //! Constructor
  /*! This constructor assumes that all of the players have the same
    numbers of types and actions. */
  BCEAbstractGame(int numStatesArg, 
		  int numActionsArg, 
		  int numTypesArg, 
		  int numObjectivesArg);
  //! Constructor
  /*! This constructor allows different players to have different
    numbers of types and actions. */
  BCEAbstractGame(int numStatesArg, 
		  const vector<int> & numActionsArg, 
		  const vector<int> & numTypesArg, 
		  int numObjectivesArg);

  //! Destructor
  ~BCEAbstractGame()
  {} 

  friend class BCESolver;
};

#endif
