// This file is part of the BCESolve library for games of incomplete
// information
// Copyright (C) 2016 Benjamin A. Brooks and Robert J. Minton
//
// BCESolve free software: you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// BCESolve is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see
// <http://www.gnu.org/licenses/>.
//
// Benjamin A. Brooks
// ben@benjaminbrooks.net
// Chicago, IL

#include "bcesolution.hpp"

BCESolution::BCESolution (const BCEGame & _game):
  game(_game),
  currentEquilibrium(0),
  boundaryMapped(false),
  mapBoundaryWeights(2,vector<double>(2,0))
{ } // Constructor

void BCESolution::clearEquilibria()
{
  newEquilibria.clear();
  equilibria.clear();
} // clearEquilibria

void BCESolution::addEquilibrium(const map<int,double> & distr,  map<int,double> & multipliers)
{

  newEquilibria.push_back(BCEquilibrium(distr, multipliers));
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
      this->currentEquilibrium = rhs.currentEquilibrium;
      this->mapBoundaryWeights = rhs.mapBoundaryWeights;
      this->boundaryMapped = rhs.boundaryMapped;
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

double BCESolution::getConstraintMultipliers(int player, int action, int type,
					   vector< vector<double> > &values) const
{
    values
      = vector< vector<double> >(game.getNumObjectives(),
                   vector<double> (game.getNumActions()[player],0.0));
    int mc = 0;
    for (int p=0; p<player; p++)
        {
          mc += game.getNumTypes()[p]*game.getNumActions()[p]*game.getNumActions()[p];
        }
    mc += type*game.getNumActions()[player]*game.getNumActions()[player];
    mc += action*game.getNumActions()[player];
    int flag = mc;
    while (equilibria[currentEquilibrium].multipliers.count(flag) < 1)
    {
        flag++;
    }
    map<int,double>::const_iterator it = equilibria[currentEquilibrium].multipliers.find(flag);
    for (int k = flag; k < mc + game.getNumActions()[player]; ++k) 
    {
        if (equilibria[currentEquilibrium].multipliers.count(k) < 1)
        {
            continue;
        }
        for (int i=0; i < game.getNumObjectives(); i++)
        {
            values[i][k-mc] = it->second;
        }
        it++;
    }
    return 0.0;
}//getConstraintMultipliers

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
