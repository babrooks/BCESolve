#ifndef BCECOUNTER_HPP
#define BCECOUNTER_HPP

#include "bcecommon.hpp"
#include "bceexception.hpp"

//! Class for iterating through arrays.
/*! This class defines a "standard" order for quickly iterating
    through arrays indexed by states, actions profiles, and type
    profiles. The */
class BCECounter
{
public:
  int variable;
  int marginal;
  int stateType;

  int numPlayers;
  int state;
  vector<int> actions;
  vector<int> types;

  BCECounter() {}

  BCECounter(int _numStates,const vector<int> & _numActions, const vector<int> & _numTypes,
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
  
  void initialize();

  bool operator++(); 

  int getNumMarginal() {return numMarginalVariables;}
  
private:
  int stateIndex;
  vector<int> actionIndices;
  vector<int> typeIndices;

  int numMarginalVariables;
  int numVariables;

  vector<int> stateConditions;
  vector< vector<int> > actionConditions;
  vector< vector<int> > typeConditions;

  bool stateMarginal;
  vector<bool> actionMarginal;
  vector<bool> typeMarginal;
  
  int numStates;
  vector<int> numActions;
  vector<int> numTypes;

  vector<int> actionIncrements;
  vector<int> typeIncrements;
  vector<int> actionDecrements;
  vector<int> typeDecrements;

  int stateIncrementMarginal;
  vector<int> actionIncrementsMarginal;
  vector<int> typeIncrementsMarginal;
  vector<int> actionDecrementsMarginal;
  vector<int> typeDecrementsMarginal;

  vector<int> actionIncrementsStateType;
  vector<int> typeIncrementsStateType;
  vector<int> typeDecrementsStateType;
}; // BCECounter


#endif
