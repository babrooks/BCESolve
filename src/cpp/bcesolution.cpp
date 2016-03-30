#include "bcesolution.hpp"

BCESolution::BCESolution (const BCEGame & _game):
  game(_game)
{ } // Constructor

void BCESolution::clearEquilibria()
{
  newEquilibria.clear();
  equilibria.clear();
} // clearEquilibria

void BCESolution::addEquilibrium(const map<int,double> & distr)
{

  newEquilibria.push_back(BCEquilibrium(distr));
} // addEquilibrium

void BCESolution::consolidateEquilibria()
{
  bool firstequilibria=(!equilibria.size());

  equilibria.reserve(equilibria.size()+newEquilibria.size());

  for (list<BCEquilibrium>::const_iterator it=newEquilibria.begin();
       it!=newEquilibria.end();
       it++)
    equilibria.push_back(*it);
  
  newEquilibria.clear();
} // Consolidate equilibria

const BCEquilibrium & BCESolution::getEquilibrium(int equilibriumIndex) const
{
  if (!equilibria.size())
    throw(BCEException(BCEException::NoEquilibria));

  if (equilibriumIndex<equilibria.size())
    return equilibria[equilibriumIndex];
  else
    throw(BCEException(BCEException::OutOfBounds));
} // getEquilibrium

void BCESolution::setCurrentEquilibrium(int equilibriumIndex)
{
  if (!equilibria.size())
    throw(BCEException(BCEException::NoEquilibria));

  if (equilibriumIndex<equilibria.size() 
      && equilibriumIndex>=0)
    currentEquilibrium=equilibriumIndex;
  else
    throw(BCEException(BCEException::OutOfBounds));
} // setCurrentEquilibrium

// Overloaded =
BCESolution& BCESolution::operator=(const BCESolution &rhs) 
{
  if (this!=&rhs)
    {
      this->game=rhs.game;
      this->newEquilibria=rhs.newEquilibria;
      this->equilibria=rhs.equilibria;
    }

  return *this;
} // =

void BCESolution::getExpectedObjectives(vector<double> &values) const
{
  if (!equilibria.size())
    throw(BCEException(BCEException::NoEquilibria));

  getExpectedObjectives(values,equilibria[currentEquilibrium].distribution);
} // getExpectedObjectives

void BCESolution::getExpectedObjectives(vector< vector<double> > &values) const
{
  if (!equilibria.size())
    throw(BCEException(BCEException::NoEquilibria));

  int eq, variableCounter;

  values.resize(equilibria.size());

  vector<double> currentObjectiveValues;

  for (eq=0; eq<equilibria.size(); eq++)
    {
      getExpectedObjectives(values[eq],
			    equilibria[eq].distribution);

    } // for eq

} // getExpectedObjectives

void BCESolution::getExpectedObjectives(vector<double> &values,
					const map<int,double> &distribution) const
{
  int obj;

  if (!equilibria.size())
    throw(BCEException(BCEException::NoEquilibria));

  values.resize(game.getNumObjectives());

  for (obj=0; obj<game.getNumObjectives(); obj++)
    {
      values[obj]=0.0;

    } // obj

  BCECounter counter(game.getNumStates(),game.getNumActions(),
		     game.getNumTypes(),
		     vector<int>(0),vector< vector<int> > (2),
		     vector< vector<int> > (2),
		     true,vector<bool>(2,true),vector<bool>(2,false));

  for (map<int,double>::const_iterator it = distribution.begin();
       it != distribution.end();
       it++)
    {
      while (counter.getVariable() < it->first)
	++counter;
      
      if (counter.getVariable() != it->first)
	continue;

      for (obj=0; obj<game.getNumObjectives(); obj++)
	{
	  values[obj]
	    +=it->second
	    *game.objective(counter.getState(),counter.getActions(),obj)
	    *game.prior(counter.getState(),counter.getTypes());
	  
	  } // obj
    } 

} // getExpectedObjectives (for a particular distribution)

double BCESolution::getDeviationObjectives(int player, int action, int type,
					   vector< vector<double> > &values) const
{
  if (!equilibria.size())
    throw(BCEException(BCEException::NoEquilibria));

  int dev, obj, stateActionIndex;

  // state, types, actions
  int state;
  vector<int> devs(2,0); 
  double prob, probSum=0;

  // Initialize values
  values
    = vector< vector<double> >(game.getNumObjectives(), 
			       vector<double> (game.getNumActions()[player],0.0));

  vector<int> stateConditions;
  vector< vector<int> > actionConditions(2); 
  actionConditions[player] = vector<int>(1,action);
  vector< vector<int> > typeConditions(2); 
  typeConditions[player] = vector<int>(1,type);
  
  bool stateMarginal = true;
  vector<bool> actionMarginal(2,true), typeMarginal(2,true);

  BCECounter counter(game.getNumStates(),game.getNumActions(),game.getNumTypes(),
		     stateConditions,actionConditions,typeConditions,
		     stateMarginal,actionMarginal,typeMarginal);
  
  for (map<int,double>::const_iterator it
	 = equilibria[currentEquilibrium].distribution.begin();
       it != equilibria[currentEquilibrium].distribution.end();
       it++)
    {
      while (counter.getVariable() < it->first)
	if (!(++counter))
	  break;

      if (counter.getVariable() != it->first)
	continue;

      prob = it->second*game.prior(counter.getState(),counter.getTypes());
	      
      probSum += prob;

      devs[1-player] = counter.getActions()[1-player];
      if (action != counter.getActions()[player])
	throw(BCEException(BCEException::ConditionFailed));
      // assert(action==counter.getActions()[player]);

      for (devs[player]=0; 
	   devs[player]<game.getNumActions()[player]; devs[player]++)
	{
	  for (obj=0; obj<game.getNumObjectives(); obj++)
	    {
	      values[obj][devs[player]]
		+= prob*game.objective(counter.getState(),devs,obj);
	    } // objectives
	} // devs
      
    } // distribution


  for (devs[player]=0;
       devs[player]<game.getNumActions()[player];
       devs[player]++)
    if (values[player][devs[player]] > values[player][action] + 1e-4)
      throw(BCEException(BCEException::ICConstraintViolated));
    // assert(values[player][devs[player]]
    // 	   <=values[player][action]+1e-4);
  
  if (probSum)
    {
      for (dev=0; dev<game.getNumActions()[player]; dev++)
	{
	  for (obj=0; obj<game.getNumObjectives(); obj++)
	    values[obj][dev] /= probSum;
	}
    }

  return probSum/game.prior(player,type);
} // getDeviationObjectives

// getConditionalMarginal finds a conditional marginal
// distribution. The marginal distribution is over all variables for
// which <var>Marginal is true. The conditional distribution must have
// state be an element of stateConditions, action[i] a member of
// actionConditions[i], and type[i] a member of typeConditions[i]. If
// the corresponding array of conditions is empty, no restriction is
// placed on the variable.
double BCESolution::getConditionalMarginal(const vector<int> &stateConditions, 
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

  double probSum=0, prob;

  BCECounter counter(game.getNumStates(),game.getNumActions(),game.getNumTypes(),
		     stateConditions,actionConditions,typeConditions,
		     stateMarginal,actionMarginal,typeMarginal);
  numMarginalVariables = counter.getNumMarginal();

  // Resize distribution and initialize to 0.0
  distribution = vector<double>(numMarginalVariables,0.0);

  for (map<int,double>::const_iterator it = equilibria[currentEquilibrium].distribution.begin();
       it != equilibria[currentEquilibrium].distribution.end();
       it++)
    {
      while (counter.getVariable() < it->first)
	if (!++counter)
	  break;

      if (counter.getVariable() != it->first)
	continue;

      prob = it->second * game.prior(counter.getState(),
					    counter.getTypes());
      probSum += prob;
      distribution[counter.getMarginal()] += prob;
    } 

  // Normalize if a non-zero probability event.
  if (probSum)
    {
      for (marginal=0; marginal<distribution.size(); marginal++)
	distribution[marginal]/=probSum;
    }

  return probSum;
} // getConditionalMarginal


