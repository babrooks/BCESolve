// Game class for BCE
// BAB 12/29/2011
#ifndef BCEGAME_HPP
#define BCEGAME_HPP

#include "bceabstractgame.hpp"
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>


//! The base class for games of incomplete information.
/*! This game is derived from BCEAbstractGame and is primarily for the
    purpose of constructing and serializing games.

    \ingroup src
*/
class BCEGame : public BCEAbstractGame
{
  /**
   *\example fpagame.hpp
   *\example duopoly.hpp
   */

private:
  //! Data for the objectives
  /*! Access via objectiveData[obj][state][actionIndex] */
  vector< vector< vector<double> > > objectiveData;
  //! Data for the prior
  /*! Access via priorData[state][typeIndex] */
  vector< vector<double> > priorData;
  //! Data for the dominated array
  /*! Access via dominatedData[player][type][action] */
  vector< vector< vector<bool> > > dominatedData;
  //! Data for the feasibleDeviation array
  /*! Access via feasibleDeviationData[player][type][action + dev*numActions[player]] */
  vector< vector< vector<bool> > > feasibleDeviationData;
  
public:
  //! Default constructor
  BCEGame();

  //! Constructor
  /*! Copies a BCEAbstractGame into a BCEGame. */
  BCEGame(const BCEAbstractGame & game);
  
  //! Destructor
  ~BCEGame()
  {} 

  //! Prior over state and types
  /*! For each state and vector of types, returns the prior
      probability of those occuring. This is a pure virtual function
      that must be implemented by the derived class. */
  double prior(int state, const vector<int> &types) const
  {
    return priorData[state][types[0] + types[1] * numTypes[0]];
  } // prior

  //! Sets the prior
  bool setPrior(int state, const vector<int> &types, double value)
  {
    priorData[state][types[0] + types[1] * numTypes[0]] = value;

    return true;
  } // setPrior

  //! Objective function
  /*! Maps actions and values into objective functions. The argument
    objectiveIndex can be any number between 0 and numObjectives-1,
    inclusive.  objective=0 through numPlayers-1 are the payoffs of
    players 1 through numPlayers, respectively. This is a pure virtual
    function that must be implemented by the derived class. */
  double objective(int state, const vector<int> &actions, 
			   int objectiveIndex) const
  {
    return objectiveData[objectiveIndex][state]
      [actions[0] + actions[1]*numActions[0]];
  } // objective

  //! Sets the objective
  bool setObjective(int state, const vector<int> &actions, 
		    int objectiveIndex, double value)
  {
    objectiveData[objectiveIndex][state]
      [actions[0] + actions[1]*numActions[0]] = value;
    return true;
  } // setObjective

  //! Indicates if a combination of actions and types is dominated.
  /*! If a combination of actions and types is dominated, the
      algorithm will force the probability of taking that action
      (given that type) to be zero. This is a virtual method that can
      be reimplemented. By default, no action is dominated. */
  bool dominated(int action, int type, int player) const
  {
    return dominatedData[player][type][action];
  } // dominated

  //! Set dominated
  bool setDominated(int action, int type, int player, bool value)
  {
    dominatedData[player][type][action] = value;
  } // setDominated

  //! Check if deviation is feasible
  /*! Indicates whether or not it is feasible to deviate from the
    action to the alternative dev, when the player's type is
    type. This is a virtual method that can be reimplemented. By
    default, all deviations are feasible. */
  bool feasibleDeviation(int action, int dev, 
				 int type, int player) const
  {
    return feasibleDeviationData[player][type][action+dev*numActions[player]];
  } // feasibleDeviation

  //! Set feasible
  bool setFeasibleDeviation(int action, int dev, 
			    int type, int player, bool value)
  {
    feasibleDeviationData[player][type][action+dev*numActions[player]] = value;

    return true;
  } // setFeasibleDeviation

  //! Adds a new objective after position
  bool addObjective(int position);
  //! Removes the objective obj
  bool removeObjective(int obj);
  //! Adds a new state after position
  bool addState(int position);
  //! Removes state from the game
  bool removeState(int state);
  //! Add type for player at position
  bool addType(int player, int position);
  //! Remove type for player
  bool removeType(int player, int type);
  //! Add action for player at position
  bool addAction(int player, int position);
  //! Remove action for player
  bool removeAction(int player, int action);

  //! Serialization routine
  template <class Archive>
  void serialize(Archive & ar, const unsigned int version)
  {
    ar & numPlayers;
    ar & numActions;
    ar & numTypes;
    ar & numStates;
    ar & numObjectives;
    ar & objectiveData;
    ar & priorData;
    ar & dominatedData;
    ar & feasibleDeviationData;
    ar & hasProductStructure;
    ar & numPrivateStates;
  } // serialize
  
  //! Serialize a BCEGame object using Boost.
  static void save(const BCEGame & game, const char* filename)
  {
    ofstream ofs(filename);

    if (ofs.good())
      {
	boost::archive::text_oarchive oa(ofs);
	oa << game;
	ofs.close();
      }
    else
      throw(BCEException(BCEException::FailedOpen));
  }

  //! Deserialize a BCEGame object using Boost.
  static void load(BCEGame & game, const char* filename)
  {
    ifstream ifs(filename);

    if (ifs.good())
      {
	boost::archive::text_iarchive ia(ifs);
	ia >> game;
	ifs.close();
      }
    else
      throw(BCEException(BCEException::FailedOpen));
  }
  
  friend class boost::serialization::access;
}; // BCEGame

#endif
