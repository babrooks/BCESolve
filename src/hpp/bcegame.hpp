// Game class for BCE
// BAB 12/29/2011
#ifndef BCEGAME_HPP
#define BCEGAME_HPP

#include "bceabstractgame.hpp"
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>


//! The base class for games of incomplete information.
/*! This game is derived from BCEAbstractGame and is primarily for the
    purpose of serializing games.

    \ingroup src

*/
class BCEGame : public BCEAbstractGame
{
  /**
   *\example fpagame.hpp
   *\example duopoly.hpp
   */

private:
  vector< vector< vector<double> > > objectiveData;
  vector< vector<double> > priorData;
  vector< vector< vector<bool> > > dominatedData;
  vector< vector< vector<bool> > > feasibleDeviationData;
  
public:
  //! Default constructor
  BCEGame();

  //! Constructor
  /*! Copies a BCEAbstractGame into a BCEGame. */
  BCEGame(const BCEAbstractGame & game);
  
  // //! Constructor
  // /*! This constructor allows different players to have different
  //     numbers of types and actions. */
  // BCEGame(int numPlayersArg, 
  // 	  int numStatesArg, 
  // 	  const vector<int> & numActionsArg, 
  // 	  const vector<int> & numTypesArg, 
  // 	  int numObjectivesArg,
  // 	  const & vector< vector<double> > _objectives,
  // 	  const & vector< double > _prior,
  // 	  const & vector< bool > _dominated = vector<bool>(false),
  // 	  const & vector< bool > _feasibleDeviation = vector<bool>(false));

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
  }

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
  }

  //! Indicates if a combination of actions and types is dominated.
  /*! If a combination of actions and types is dominated, the
      algorithm will force the probability of taking that action
      (given that type) to be zero. This is a virtual method that can
      be reimplemented. By default, no action is dominated. */
  bool dominated(int action, int type, int player) const
  {
    return dominatedData[player][type][action];
  }

  //! Check if deviation is feasible
  /*! Indicates whether or not it is feasible to deviate from the
    action to the alternative dev, when the player's type is
    type. This is a virtual method that can be reimplemented. By
    default, all deviations are feasible. */
  bool feasibleDeviation(int action, int dev, 
				 int type, int player) const
  {
    return feasibleDeviationData[player][type][action+dev*numActions[player]];
  }

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
  
  //! Serialize a BCEData object using Boost.
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

  //! Deserialize a BCEData object using Boost.
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
