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

  newEquilibria.push_back(BCEEquilibrium(distr));
} // addEquilibrium

void BCEData::consolidateEquilibria()
{
  bool firstequilibria=(!equilibria.size());

  equilibria.reserve(equilibria.size()+newEquilibria.size());

  for (list<BCEEquilibrium>::const_iterator it=newEquilibria.begin();
       it!=newEquilibria.end();
       it++)
    equilibria.push_back(*it);
  
  newEquilibria.clear();
} // Consolidate equilibria

void BCEData::sortEquilibria(vector<int> &obj)
{
  // Get expected objectives, and append a key.
  int equilibriumCounter;
  vector< vector<double> > expectedObjectives;

  cout << "Getting expected objectives" << endl;
  getExpectedObjectives(expectedObjectives);
  for (equilibriumCounter=0;
       equilibriumCounter<expectedObjectives.size();
       equilibriumCounter++)
    expectedObjectives[equilibriumCounter].push_back(static_cast<double>(equilibriumCounter));
  
  cout << "Starting convex hull routine." << endl;

  // Use the sorting list to reconstruct the equilibria array.
  grahamScan(expectedObjectives,obj);

  cout << "Done finding convex hull." << endl;
  vector<BCEEquilibrium> sortedEq;
  sortedEq.reserve(expectedObjectives.size());
  for (equilibriumCounter=0;
       equilibriumCounter<expectedObjectives.size();
       equilibriumCounter++)
    sortedEq.push_back(equilibria[static_cast<int>(expectedObjectives[equilibriumCounter].back())]);

  // Replace equilibria with sortedEq
  equilibria = sortedEq;
}

void BCEData::getEquilibrium(int equilibriumIndex, 
			     BCEEquilibrium &equilibrium) const
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


bool BCEComparator::operator() (const vector<double> & eq0, 
				const vector<double> & eq1)
{
  double absoluteZero=1E-12;
  int playerCounter;
  vector<double> diff0(2,0.0), diff1(2,0.0);

  switch (currentMode)
    {
    case ANGLE:
      for (playerCounter=0; playerCounter<2; playerCounter++)
	{
	  diff0[playerCounter] = eq0[obj[playerCounter]] - origin[playerCounter];
	  diff1[playerCounter] = eq1[obj[playerCounter]] - origin[playerCounter];
	}

      return ( (cosine(diff0) > cosine(diff1)+absoluteZero)
	       || (abs(cosine(diff0)-cosine(diff1))<absoluteZero
		   && norm(diff0)+absoluteZero<norm(diff1) ) );

    case EQUALITY:
      if (abs(eq0[obj[0]]-eq1[obj[0]])>absoluteZero
	  || abs(eq0[obj[1]]-eq1[obj[1]])>absoluteZero)
	return false;
      return true;
    }
}

double BCEComparator::operator() (const vector<double> & eq0, 
		   const vector<double> & eq1, 
		   const vector<double> & eq2)
{
  return( (eq1[obj[0]]-eq0[obj[0]])*(eq2[obj[1]]-eq0[obj[1]]) 
	  - (eq1[obj[1]]-eq0[obj[1]])*(eq2[obj[0]]-eq0[obj[0]]) );
}

double BCEComparator::cosine(const vector<double> & eq)
{
  double n = norm(eq);
  if (n)
    return (eq[obj[0]]/n);
  else
    return 1.0;
}

double BCEComparator::norm(const vector<double> & eq)
{
  return (sqrt(eq[obj[0]]*eq[obj[0]]+eq[obj[1]]*eq[obj[1]]));
}

void BCEComparator::setObjectives(const vector<int> &_obj)
{
  obj = _obj;
}

void BCEComparator::setOrigin(const vector<double> & input)
{
  assert(input.size()==2);
  
  origin=input;
}

void BCEComparator::setMode(Mode newMode)
{
  currentMode = newMode;
}


void BCEData::grahamScan(vector< vector<double> > & points,
			 vector<int> & obj)
{
  BCEComparator comparator;
  vector<double> origin(2,0), temp;

  double absoluteZero = 1e-12;

  int pointCounter, extremeCounter;

  assert(obj.size()==2);
  assert(obj[0]>=0);
  assert(obj[0]<numObjectives);
  assert(obj[1]>=0);
  assert(obj[1]<numObjectives);
  assert(obj[0]!=obj[1]);

  comparator.setObjectives(obj);
  comparator.setMode(BCEComparator::ANGLE);

  // Push first vector to the back. 
  points.push_back(points.front());

  // Find the vector with the lowest y coordinate
  origin[0]=points[1][obj[0]];
  origin[1]=points[1][obj[1]];
  for (pointCounter=2; pointCounter<points.size(); pointCounter++)
    {
      if (points[pointCounter][obj[1]]<origin[1])
  	{
  	  origin[0]=points[pointCounter][obj[0]];
  	  origin[1]=points[pointCounter][obj[1]];
  	}
    }
  comparator.setOrigin(origin);

  // Sort the points in area order. Move the last point to the front.
  sort(points.begin()+1,points.end(),comparator);
  points[0]=points.back();

  // Now do the Graham scan
  extremeCounter=1;
  for (pointCounter=2; pointCounter<points.size(); pointCounter++)
    {
      while (comparator(points[extremeCounter-1],
			points[extremeCounter],
			points[pointCounter])<=absoluteZero)
	{
	  if (extremeCounter>1)
	    extremeCounter--;
	  else if (pointCounter==points.size()-1)
	    break;
	  else
	    pointCounter++;
	}

      extremeCounter++;
      temp=points[extremeCounter];
      points[extremeCounter]=points[pointCounter];
      points[pointCounter]=temp;
    }

  // Move the first point to the end and delete the first point.
  points.erase(points.begin()+extremeCounter+1,points.end());
  points.back()=points.front();
  points.erase(points.begin());

  comparator.setMode(BCEComparator::EQUALITY);
  unique(points.begin()+1,points.end(),comparator);
}


