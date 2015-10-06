#ifndef BCECOUNTER_HPP
#define BCECOUNTER_HPP

#include "bcecommon.hpp"
#include "bceexception.hpp"

//! Class for iterating through arrays.
/*! This class defines a "standard" order for quickly iterating
  through arrays indexed by states, actions profiles, and type
  profiles. The class contains functionality for iterating only
  through subsets of types and actions, to facilitate the
  computation of marginal and conditional distributions, assist with
  construction of constraints in BCESolver::populate, etc. 

  \ingroup src
*/
class BCECounter
{
private:
  //! Variable counter
  /*! Index of the variable over all state/action profile/type profile
    combinations. */
  int variable; 
  //! Marginal counter
  /*! Marginal index over the subset of dimensions specified by
    marginal conditions. */
  int marginal;
  //! State/type counter
  /*! Marginal index over the state and types. */
  int stateType;
  //! The number of players, always 2. 
  int numPlayers;
  //! Current index of the state. 
  int state;
  //! Current action profile.
  vector<int> actions; 
  //! Current type profile.
  vector<int> types; 
  //! Index of the current state
  /*! The index of the current state within the stateConditions array.
   */
  int stateIndex; 
  //! Indices of the current actions
  /*! actionIndices[k] is the index of that player k's current action
    in the array actionConditions[k]. */
  vector<int> actionIndices; 
  //! Indices of the current types
  /*! typeIndices[k] is the index of that player k's current type in
    the array typeConditions[k]. */
  vector<int> typeIndices;

  //! Maximum marginal index
  /*! The maximum marginal index, or equivalently, the number of
    combinations of the variables whose marginal is true. */  
  int numMarginalVariables; 
  //! Maximum variable index
  /*! The maximum variable index, or equivalently, the total number of
    combinations of states, action profiles, and type profiles. */
  int numVariables; 

  //! Conditions on the state
  /*! A vector of admissible values for the state. */
  vector<int> stateConditions; 
  //! Conditions on the action profile
  /*! actionConditions[k] is a vector of admissible values for player
    k's actions. */
  vector< vector<int> > actionConditions; 
  //! Conditions on the type profile
  /*! typeConditions[k] is a vector of admissible values for player
    k's types. */
  vector< vector<int> > typeConditions; 

  //! Determines if the state is a marginal dimension
  /*! If ture, the marginal index will iterate when the state
    changes.  */
  bool stateMarginal; 
  //! Determines if actions are marginal dimensions
  /*! If actionMarginal[k] is true, the marginal index will iterate
    when player k's action changes. */ 
  vector<bool> actionMarginal; 
  //! Determines if types are marginal directions.
  /*! If typeMarginal[k] is true, the marginal index will iterate when
    player k's action changes. */
  vector<bool> typeMarginal;
  
  //! The number of states
  int numStates; 
  //! Each player's number ofactions
  vector<int> numActions;
  //! Each player's number of types
  vector<int> numTypes;

  //! Variable index increment when actions increase
  vector<int> actionIncrements;
  //! Variable index increment when types increase
  vector<int> typeIncrements;
  //! Variable index decrement when actions decrease
  vector<int> actionDecrements;
  //! Variable index decrement when actions decrease
  vector<int> typeDecrements;

  //! Marginal index increment when state increases
  int stateIncrementMarginal;
  //! Marginal index increment when actions increase
  vector<int> actionIncrementsMarginal;
  //! Marginal index increment when types increase
  vector<int> typeIncrementsMarginal;
  //! Marginal index decrement when actions decrease
  vector<int> actionDecrementsMarginal;
  //! Marginal index decrement when types decrease
  vector<int> typeDecrementsMarginal;

  //! State/type index increment when actions increase
  vector<int> actionIncrementsStateType;
  //! State/type index increment when types increase
  vector<int> typeIncrementsStateType;
  //! State/type index decrement when types decrease
  vector<int> typeDecrementsStateType;

public:
  //! Default constructor
  BCECounter() {}

  //! Constructor
  /*! Initializes a new BCECounter for a game with the given numbers
    of states, actions, and types. When the counter iterates, it
    will skip those indices that do not satisfy the various state,
    action, and type conditions. If a given condition is empty, that
    means that all types will be allowed. The counter also
    calculates the marginal index over the dimensions specified in
    the various marginal arrays. 

    \param _numStates The number of states.

    \param _numActions The number of actions for each player. 

    \param _numTypes The number of types for each player. 

    \param _stateConditions A vector of permissible values for the
    state. If this vector is empty, any state value is allowed.

    \param _actionConditions A vector of
    vectors. _actionConditions[player] is a vector of admissible
    values for that player's action. If _actionConditions[player] is
    empty, any action is allowed.

    \param _typeConditions A vector of
    vectors. _typeConditions[player] is a vector of admissible
    values for that player's type. If _typeConditions[player] is
    empty, any type is allowed.

    \param _stateMarginal A bool. If true, the state is one of the
    dimensions over which the marginal index iterates.

    \param _actionMarginal A vector of bools. If
    _actionMarginal[player] is true, the marginal index will iterate
    over that player's action.

    \param _typeMarginal A vector of bools. If
    _typeMarginal[player] is true, the marginal index will iterate
    over that player's type.

    See also the descriptions of corresponding property values.
  */ 
  BCECounter(int _numStates,const vector<int> & _numActions, 
	     const vector<int> & _numTypes,
	     const vector<int> &_stateConditions, 
	     const vector< vector<int> > & _actionConditions, 
	     const vector< vector<int> > & _typeConditions,
	     bool _stateMarginal,
	     const vector<bool> & _actionMarginal,
	     const vector<bool> & _typeMarginal):
    numPlayers(2),
    numStates(_numStates), numActions(_numActions),
    numTypes(_numTypes), stateConditions(_stateConditions),
    actionConditions(_actionConditions), 
    typeConditions(_typeConditions),
    stateMarginal(_stateMarginal),
    actionMarginal(_actionMarginal),
    typeMarginal(_typeMarginal),
    actions(2,0), types(2,0),
    actionIncrements(2,0), typeIncrements(2,0),
    actionDecrements(2,0), typeDecrements(2,0),
    stateIncrementMarginal(0),
    actionIncrementsMarginal(2,0), typeIncrementsMarginal(2,0),
    actionDecrementsMarginal(2,0), typeDecrementsMarginal(2,0),
    actionIncrementsStateType(2,0), typeIncrementsStateType(2,0),
    typeDecrementsStateType(2,0)
  {
    initialize();
  }
  
  //! Initialization routine
  /*! This method is called by the constructor to initialize the
    counter. If any of the conditions vectors are empty, they are
    replaced with vectors allowing every possible condition. It
    calculates the increments that shoudld be used for the different
    indices when they are incremented, and initializes the indices
    to the appropriate values, given the conditions and
    marginals. */
  void initialize();

  //! Increments the counter.
  /*! This method increments each of the indices in the counter. If
    the counter is at the end, it returns false, and otherwise it
    returns true. */
  bool operator++(); 

  //! Returns the maximum marginal index
  int getNumMarginal() {return numMarginalVariables;}
  //! Return the variable index
  int getVariable() const { return variable; }
  //! Return the marginal index
  int getMarginal() const { return marginal; }
  //! Return the current state
  int getState() const { return state; }
  //! Return the current vector of types
  const vector<int> & getTypes() const { return types; }
  //! Return the current vector of actions
  const vector<int> & getActions() const { return actions; }
  
}; // BCECounter


#endif
