#include "bcedata.hpp"

BCEData::BCEData (int numPlayersArg,
		  int numStatesArg, 
		  const vector<int> & numActionsArg, 
		  const vector<int> & numTypesArg,
		  int numObjectivesArg):
  numPlayers(numPlayersArg),
  numStates(numStatesArg),
  numActions(numActionsArg),
  numTypes(numTypesArg),
  numObjectives(numObjectivesArg),
  numActions_total(1),
  numTypes_total(1),
  numActionsTypes_total(1),
  numActionsTypesPerPlayer_total(0),
  currentEquilibrium(0),
  isPrivateValues(false)
{
  countActionsTypes();

  objectives = vector< vector<double> >(numObjectives,vector<double> (numStates*numActions_total,0.0));
  prior = vector<double>(numStates*numTypes_total,0.0);
  dominated = vector<bool>(numActionsTypesPerPlayer_total,false);

  // initialize the objective names
  objectiveNames = vector<string>(numObjectives,"Objective ");
  for (int objectiveCounter=0; objectiveCounter<numObjectives; objectiveCounter++)
    objectiveNames[objectiveCounter] += objectiveCounter;

} // Constructor

void BCEData::countActionsTypes()
{
  // Set the numActions_total and numTypes_total
  int playerCounter;
  
  for (playerCounter=0; playerCounter<numPlayers; playerCounter++)
    {
      numActions_total *= numActions[playerCounter];
      numTypes_total *= numTypes[playerCounter];
      numActionsTypes_total *= numActions[playerCounter]*numTypes[playerCounter];
      numActionsTypesPerPlayer_total += numActions[playerCounter]*numTypes[playerCounter];

    }
} // countActionsTypes

void BCEData::setObjectivesPriorDominated(const vector< vector<double> > &objectivesArg,
					  const vector<double> &priorArg,
					  const vector<bool> &dominatedArg)
{
  assert(objectives.size()==objectivesArg.size());
  assert(prior.size()==priorArg.size());
  assert(dominated.size()==dominatedArg.size());

  for (int objectiveCounter=0; objectiveCounter<numObjectives; objectiveCounter++)
    {
      assert(objectivesArg[objectiveCounter].size()==objectives[objectiveCounter].size());

      objectives[objectiveCounter]=objectivesArg[objectiveCounter];
    }
  
  prior=priorArg;
  dominated=dominatedArg;
} // setObjectivesPriorDominated

void BCEData::setObjectiveNames(const vector<string> & names)
{
  assert(names.size()==numObjectives);
  
  objectiveNames = names;
} // setObjectiveNames

void BCEData::clearEquilibria()
{
  newEquilibria.clear();
  equilibria.clear();
} // clearEquilibria

void BCEData::addEquilibrium(const map<int,double> & distr)
{

  newEquilibria.push_back(BCEquilibrium(distr));
} // addEquilibrium

void BCEData::consolidateEquilibria()
{
  bool firstequilibria=(!equilibria.size());

  equilibria.reserve(equilibria.size()+newEquilibria.size());

  for (list<BCEquilibrium>::const_iterator it=newEquilibria.begin();
       it!=newEquilibria.end();
       it++)
    equilibria.push_back(*it);
  
  newEquilibria.clear();
} // Consolidate equilibria

void BCEData::getEquilibrium(int equilibriumIndex, 
			     BCEquilibrium &equilibrium) const
{
  if (!equilibria.size())
    throw(BCEException(BCEException::NoEquilibria));

  if (equilibriumIndex<equilibria.size())
    equilibrium=equilibria[equilibriumIndex];
  else
    throw(BCEException(BCEException::OutOfBounds));
} // getEquilibrium

void BCEData::setCurrentEquilibrium(int equilibriumIndex)
{
  if (!equilibria.size())
    throw(BCEException(BCEException::NoEquilibria));

  if (equilibriumIndex<equilibria.size() 
      && equilibriumIndex>=0)
    currentEquilibrium=equilibriumIndex;
  else
    throw(BCEException(BCEException::OutOfBounds));
} // setCurrentEquilibrium

void BCEData::setNumValues(const vector<int> & _numValues)
{
  if (_numValues.size()!=numPlayers)
    throw(BCEException(BCEException::BadArgument));

  int playerCounter, product=1;
  for (playerCounter=0; playerCounter<numPlayers; playerCounter++)
    product*=_numValues[playerCounter];
  if (product!=numStates)
    throw(BCEException(BCEException::BadArgument));
  
  numValues = _numValues;
  isPrivateValues = true;
}

// Overloaded =
BCEData& BCEData::operator=(const BCEData &rhs) 
{
  if (this!=&rhs)
    {
      this->numPlayers=rhs.numPlayers;
      this->numActions=rhs.numActions;
      this->numStates=rhs.numStates;
      this->numTypes=rhs.numTypes;
      this->numObjectives=rhs.numObjectives;
      this->objectives=rhs.objectives;
      this->prior=rhs.prior;
      this->dominated=rhs.dominated;
      this->newEquilibria=rhs.newEquilibria;
      this->equilibria=rhs.equilibria;
      this->numActions_total=rhs.numActions_total;
      this->numTypes_total=rhs.numTypes_total;
      this->numActionsTypesPerPlayer_total=rhs.numActionsTypesPerPlayer_total;
      this->numActionsTypes_total=rhs.numActionsTypes_total;
    }

  return *this;
} // =

void BCEData::getExpectedObjectives(vector<double> &objectiveValues) const
{
  if (!equilibria.size())
    throw(BCEException(BCEException::NoEquilibria));

  vector<double> currentEquilibriumCopy;
  // currentEquilibriumCopy=equilibria[currentEquilibrium].distribution;
  
  getExpectedObjectives(objectiveValues,equilibria[currentEquilibrium].distribution);
} // getExpectedObjectives

void BCEData::getExpectedObjectives(vector< vector<double> > &objectiveValues) const
{
  if (!equilibria.size())
    throw(BCEException(BCEException::NoEquilibria));

  int equilibriumCounter, variableCounter;

  objectiveValues.resize(equilibria.size());

  vector<double> currentObjectiveValues;

  cout << equilibria.size() << endl;

  for (equilibriumCounter=0; equilibriumCounter<equilibria.size(); equilibriumCounter++)
    {
      getExpectedObjectives(objectiveValues[equilibriumCounter],
			    equilibria[equilibriumCounter].distribution);

      // cout << "On equilibrium " << equilibriumCounter << endl;
    } // equilibriumCounter

} // getExpectedObjectives

void BCEData::getExpectedObjectives(vector<double> &objectiveValues,
				    const map<int,double> &distribution) const
{
  int objectiveCounter;

  if (!equilibria.size())
    throw(BCEException(BCEException::NoEquilibria));

  objectiveValues.resize(numObjectives);

  for (objectiveCounter=0; objectiveCounter<numObjectives; objectiveCounter++)
    {
      objectiveValues[objectiveCounter]=0.0;

    } // objectiveCounter

  BCECounter counter(numStates,numActions,numTypes,
		     vector<int>(0),vector< vector<int> > (2),vector< vector<int> > (2),
		     true,vector<bool>(2,true),vector<bool>(2,false));

  for (map<int,double>::const_iterator it = distribution.begin();
       it != distribution.end();
       it++)
    {
      while (counter.variable < it->first)
	++counter;
      
      if (counter.variable != it->first)
	continue;

      for (objectiveCounter=0; objectiveCounter<numObjectives; objectiveCounter++)
	{
	  objectiveValues[objectiveCounter]
	    +=objectives[objectiveCounter][counter.marginal]
	    *it->second
	    *prior[counter.stateType];
	  
	  } // objectiveCounter
    } 

} // getExpectedObjectives (for a particular distribution)

double BCEData::getDeviationObjectives(int player, int action, int type,
				     vector< vector<double> > &objectiveValues) const
{
  if (!equilibria.size())
    throw(BCEException(BCEException::NoEquilibria));

  int deviation, objectiveCounter, stateActionIndex;

  // state, types, actions
  int state;
  vector<int> deviations(2,0); 
  double currentProb, probSum=0;

  // Initialize objectiveValues
  objectiveValues = vector< vector<double> >(numObjectives, vector<double> (numActions[player],0.0));

  vector<int> stateConditions;
  vector< vector<int> > actionConditions(2); actionConditions[player] = vector<int>(1,action);
  vector< vector<int> > typeConditions(2); typeConditions[player] = vector<int>(1,type);
  
  bool stateMarginal = true;
  vector<bool> actionMarginal(2,true), typeMarginal(2,true);

  BCECounter counter(numStates,numActions,numTypes,
		     stateConditions,actionConditions,typeConditions,
		     stateMarginal,actionMarginal,typeMarginal);

  for (map<int,double>::const_iterator it = equilibria[currentEquilibrium].distribution.begin();
       it != equilibria[currentEquilibrium].distribution.end();
       it++)
    {
      while (counter.variable < it->first)
	if (!(++counter))
	  break;

      if (counter.variable != it->first)
	continue;

      currentProb = it->second*prior[counter.stateType];
	      
      probSum += currentProb;

      deviations[1-player] = counter.actions[1-player];
      assert(action==counter.actions[player]);

      for (deviations[player]=0; 
	   deviations[player]<numActions[player]; deviations[player]++)
	{
	  // Find the corresponding index on the objectives array.
	  stateActionIndex
	    =stateTypesActionsToMarginalIndex(counter.state,counter.types,deviations,
					      true,vector<bool>(2,true),
					      vector<bool>(2,false));
	  
	  for (objectiveCounter=0; 
	       objectiveCounter<numObjectives; 
	       objectiveCounter++)
	    {
	      objectiveValues[objectiveCounter][deviations[player]]
		+= currentProb*objectives[objectiveCounter][stateActionIndex];
	    } // objectives
	} // deviations
      
    } // distribution

  for (deviations[player]=0;
       deviations[player]<numActions[player];
       deviations[player]++)
    assert(objectiveValues[player][deviations[player]]<=objectiveValues[player][action]+1e-4);
  
  if (probSum)
    {
      for (deviation=0; deviation<numActions[player]; deviation++)
	{
	  for (objectiveCounter=0; objectiveCounter<numObjectives; objectiveCounter++)
	    objectiveValues[objectiveCounter][deviation] /= probSum;
	}
    }

  return probSum;
} // getDeviationObjectives

// getConditionalMarginal finds a conditional marginal
// distribution. The marginal distribution is over all variables for
// which <var>Marginal is true. The conditional distribution must have
// state be an element of stateConditions, action[i] a member of
// actionConditions[i], and type[i] a member of typeConditions[i]. If
// the corresponding array of conditions is empty, no restriction is
// placed on the variable.
double BCEData::getConditionalMarginal(const vector<int> &stateConditions, 
				     const vector< vector<int> > &actionConditions,
				     const vector< vector<int> > &typeConditions,
				     bool stateMarginal,
				     const vector<bool> &actionMarginal,
				     const vector<bool> &typeMarginal,
				     vector<double> &distribution) const
{
  // Calculates a conditional marginal distribution.
  if (!equilibria.size())
    throw(BCEException(BCEException::NoEquilibria));

  int numMarginalVariables=1, marginal;

  double probSum=0, currentProb;

  BCECounter counter(numStates,numActions,numTypes,
		     stateConditions,actionConditions,typeConditions,
		     stateMarginal,actionMarginal,typeMarginal);
  numMarginalVariables = counter.getNumMarginal();

  // Resize distribution and initialize to 0.0
  distribution = vector<double>(numMarginalVariables,0.0);

  for (map<int,double>::const_iterator it = equilibria[currentEquilibrium].distribution.begin();
       it != equilibria[currentEquilibrium].distribution.end();
       it++)
    {
      while (counter.variable < it->first)
	if (!++counter)
	  break;

      if (counter.variable != it->first)
	continue;

      currentProb = it->second * prior[counter.stateType];
      probSum += currentProb;
      distribution[counter.marginal] += currentProb;
    } 

  // Normalize if a non-zero probability event.
  if (probSum)
    {
      for (marginal=0; marginal<distribution.size(); marginal++)
	distribution[marginal]/=probSum;
    }

  return probSum;
} // getConditionalMarginal

void BCEData::indexToStateTypes(int index, int &state, vector<int> & types) const
{
  int playerCounter;

  // Make sure arguments are right size
  types.resize(numPlayers);
 
  assert(index>=0);
  assert(index<numStates*numTypes_total);

  state=index%numStates;
  index-=state; index/=numStates;
  assert(state>=0);
  assert(state<numStates);

  for (playerCounter=0; playerCounter<numPlayers; playerCounter++)
    {
      types[playerCounter]=index%numTypes[playerCounter];
      index-=types[playerCounter]; index/=numTypes[playerCounter];

      assert(types[playerCounter]<numTypes[playerCounter]);
      assert(types[playerCounter]>=0);
    }
} // indexToStateTypes

void BCEData::indexToStateActions(int index, int &state, vector<int> &actions) const
{
  int playerCounter;

  assert(index>=0);
  assert(index<numStates*numActions_total);

  state=index%numStates;
  index-=state; index/=numStates;
  assert(state>=0);
  assert(state<numStates);

  for (playerCounter=0; playerCounter<numPlayers; playerCounter++)
    {
      actions[playerCounter]=index%numActions[playerCounter];
      index-=actions[playerCounter]; index/=numActions[playerCounter];

      assert(actions[playerCounter]<numActions[playerCounter]);
      assert(actions[playerCounter]>=0);
    }
} // indexToStateActions

void BCEData::indexToTypeAction(int index, int player, int &type, int &action) const
{
  assert(index>=0);
  assert(index<numTypes[player]*numActions[player]);

  action=index%numActions[player];
  index-=action; index/=numActions[player];
  type=index;

  assert(type>=0);
  assert(action>=0);
  assert(type<numTypes[player]);
  assert(action<numActions[player]);
} // indexToTypeAction

void BCEData::indexToStateTypesActions(int index,
				      int &state, 
				      vector<int> &types,
				      vector<int> &actions) const
{
  int playerCounter;

  // Resize arguments
  types.resize(numPlayers);
  actions.resize(numPlayers);

  assert(index>=0);
  assert(index<numStates*numActionsTypes_total);

  state=index%numStates;
  index-=state; index/=numStates;
  assert(state>=0);
  assert(state<numStates);

  for (playerCounter=0; playerCounter<numPlayers; playerCounter++)
    {
      actions[playerCounter]=index%numActions[playerCounter];
      index-=actions[playerCounter]; index/=numActions[playerCounter];
      
      assert(actions[playerCounter]>=0);
      assert(actions[playerCounter]<numActions[playerCounter]);
    }
  for (playerCounter=0; playerCounter<numPlayers; playerCounter++)
    {
      types[playerCounter]=index%numTypes[playerCounter];
      index-=types[playerCounter]; index/=numTypes[playerCounter];

      assert(types[playerCounter]>=0);
      assert(types[playerCounter]<numTypes[playerCounter]);
    }

} // indexToStateTypesActions

int BCEData::stateTypesActionsToMarginalIndex(int state, 
					      const vector<int> &types, 
					      const vector<int> &actions,
					      bool stateMarginal,
					      const vector<bool> &actionMarginal,
					      const vector<bool> &typeMarginal) const
{
  // Put true for state, action, or type to include that in the index
  // calculation.

  int index=0, playerCounter;

  // Go in the reverse direction from indexToStateTypesActions
  for (playerCounter=numPlayers-1; playerCounter>=0; playerCounter--)
    {
      if (typeMarginal[playerCounter])
	index=index*numTypes[playerCounter]+types[playerCounter]; 
    }
  for (playerCounter=numPlayers-1; playerCounter>=0; playerCounter--)
    {
      if (actionMarginal[playerCounter])
	index=index*numActions[playerCounter]+actions[playerCounter];
    }
  if (stateMarginal)
    index=index*numStates+state;

  return index;
}

