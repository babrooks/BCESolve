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

#ifndef BCESOLVER_HPP
#define BCESOLVER_HPP

#include "bcecommon.hpp"
#include "bcegame.hpp"
#include "bcesolution.hpp"
#include "bcecounter.hpp"
#include "bceexception.hpp"
#include "bceutilities.hpp"
#include "gurobi_c++.h"

//! Solves a BCEAbstractGame using gurobi.
/*! This class contains routines for solving a BCEAbstractGame object using
  GUROBI. The main method is BCESolver::populate, which initializes a
  GUROBI model with the incentive constraints for the BCEAbstractGame, and also
  creates objectives corresponding to the number of objectives in the
  BCEAbstractGame object. The class also contains the method
  BCESolver::mapBoundary, which solves linear programs to trace out
  the frontier for the objectives in BCESolver::BoundaryObjective1 and
  BCESolver::BoundaryObjective2.

  \ingroup src
*/
class BCESolver 
{
public:

  //! Double parameters
  /*! An enumeration type for parameters of type double. Pass to
    BCESolver::setParam and BCESolver::getParam to set or retrieve a
    double parameter value. */ 
  enum DoubleParameter
    {
      /*! When running the map boundary method,
	this is the minimum change in the direction
	on each iteration. */
      MinAngleIncrement
    };
  //! Integer parameters
  /*! An enumeration type for parameters of type int. Pass to
    BCESolver::setParam and BCESolver::getParam to set or retrieve a
    double parameter value. */ 
  enum IntParameter
    {
      //! The index of the current objective. Not currently used? 
      CurrentObjective,
      //! Control algorithm output
      /*! Controls the frequency and detail with which progress of the
	algorithm is reported. 0 corresponds to no output, 1 is some
	output. Is also passed to the BCESolver::model solver, so it
	controls the level of output of Gurobi. */
      DisplayLevel, 
      //! The first objective used when mapping the boundary. 
      BoundaryObjective1, 
      //! The second objective used when mapping the boundary. 
      BoundaryObjective2 
    };
  
  //! Bool parameters
  /*! An enumeration type for parameters of type bool. Pass to
    BCESolver::setParam and BCESolver::getParam to set or retrieve a
    double parameter value. Will be replaced by new BCEAbstractGame
    architecture with a constraint checker. */ 
  enum BoolParameter
    {
    };

  //! Used to set the minimum angle increment. Called by bcesolverworker.hpp.
  void setMinAngleIncr(double incr) {
    minAngleIncrement = incr;
  }

protected:
  //! Pointer to the BCEAbstractGame object that is being solved. 
  BCEAbstractGame * game; 

  //! Number of columns in the constraint matrix. 
  long int numColumns; 
  //! Number of rows in the constraint matrix. 
  long int numRows; 
  //! Number of non-basic variables? 
  long int numNonBasicVariables; 

  //! Total number of action profiles for the players. 
  long int numActions_total; 
  //! Total number of types for the players. 
  long int numTypes_total; 
  //! Total number of of actions and types for each player. Used?
  long int numActionsTypesPerPlayer_total; 
  //! Total number of actions and types. 
  long int numActionsTypes_total; 

  // Stores the # of IC constraints for each player.
  //! The number of IC constraints for each player. 
  vector<int> numICConstraints; 
  //! The total number of IC constraints. 
  long int numICConstraints_total;
  //! The number of probability variables.
  long int numProbabilityVariables; 
  //! The total number of constraints on probabilities
  long int numProbabilityConstraints; 

  // Gurobi components

  GRBEnv env;  
  //! The objective for BCESolver::model.
  GRBLinExpr gurobiObjective; 

  //! The variables for BCESolver::model.
  GRBVar* variables; 
  //! The constraints for BCESolver::model. 
  vector<GRBLinExpr> constraints; 

  // We allow the user to define any number of objective functions.
  // These are all stored in an array obfun. The index of the current
  // objective is currentobfun. 
  //! Objectives
  /*! An array of objective functions, corresponding to the objectives
    in the BCEAbstractGame object. */
  vector<GRBLinExpr> objectiveFunctions; 

  //! List of indices of non-zero variables
  /*! The solver only creates variables for events that could occur
    with positive probability under the prior in the BCEAbstractGame
    object. This property stores the locations of the non-zero
    probability outcomes. */
  list<int> nonZeroVariableLocations;
  //! Locations of probability constraints
  /*! Stores locations of the probability constraints in the
    constraints array. */
  vector<int> probabilityConstraintLocations; 
  //! Map from variable indices to non-zero variable locations
  /*! A map from standard indices to the corresponding non-zero
    variable location. (See BCECounter.) */
  map<int,int> variableLocationsMap;

  //! Minimum angle increment when mapping boundary
  /*! The minimum increment of the angle for
    BCESolver::mapBoundary. */
  double minAngleIncrement;
  //! Index of the current objective. Not currently being used. 
  int currentObjective;
  //! The display level. 
  /*! 0 is no output, 1 is some output. Also passed to Gurobi, so 2+
    controls Gurobi output. */
  int displayLevel; 

  // Lists for storing equilibria and boundary points.
  //! X coordinates of frontier.
  /*! List of x-coordinates of boundary points traced out by the
    BCESolver::mapBoundary routine. */
  list<double> boundaryXs;
  //! Y coordinates of frontier.
  /*! List of y-coordinates of boundary
    points traced out by the
    BCESolver::mapBoundary routine. */
  list<double> boundaryYs; 
  //! BCESolution object for serializing output of the algorithm.
  BCESolution soln; 

  /*! Initializes the properties that count the number of actions and
  types. */
  void countActionsTypes();

  // Helper functions. These functions map between a linear index i
  // and valuations, types, and states. Mostly deprecated because of
  // use of BCECounter class.
  void indexToTypeActionDeviation(int index, int player, int &type, int &action, int &deviation);

public:
  //! Default constructor.
  /*! Creates a  new BCESolver object. */
  BCESolver(); 

  //! Constructor
  /*! Creates a new BCESolver object and initializes it with the given
    game. */
  BCESolver(BCEAbstractGame & _game);

  //! The GUROBI model. 
  GRBModel model;

  //! Destructor
  ~BCESolver()
  { delete[] variables; } 

  // Main routines

  //! Main populate routine
  /*! Populates the Gurobi model with constraints and initializes the
    objectives. */
  void populate(); 

  //! Solve method
  /*! Sets Gurobi solver parameters and calls Gurobi::optimize(). */
  void solve(vector<double>& objectiveWeights);

  //! Maps the frontier
  /*! Maps the frontier for the boundary objectives indicated in
    BCESolver::boundaryObjective1 and BCESolver::boundaryObjective2,
    and stores the calculated frontier in a file created at
    fname. */
  void mapBoundary(const char * fname);

  //! Maps the frontier
  /*! Calls BCESolver::mapBoundary(const char*) with a default file
    name. */
  void mapBoundary();

  //! Maps the frontier
  /*! Maps the boundary for specified objectives, not necessarily the
   same as BCESolver::boundaryObjective1 and BCESOlver::boundaryObjective2 */
  void mapBoundary(const char * fname,
		   const vector<vector<double> >& weights);

  //! Maps the frontier
  /*! Maps the boundary for specified objectives, not necessarily the
    same as BCESolver::boundaryObjective1 and BCESOlver::boundaryObjective2.
   Saves to a default file name. */
  void mapBoundary(const vector<vector<double> >& weights);

  //! Returns the data object
  /*! Copies the data object to output. */
  void getSolution(BCESolution & output);

  //! Copies the BCE to a map
  /*! This method copies the BCE, which is a vector of doubles, to a
    map from standard indices to probabilities. Mainly for saving
    space in data files. */
  void bceToMap(map<int,double> & distribution);
  
  // Get methods
  //! Returns the BCESolver::model object.
  GRBModel& getModel() { return model; }
  //! Returns the \f$n\f$th objective function. 
  const GRBLinExpr& getObjectiveFunction (int n) { return objectiveFunctions[n]; }
  //! Returns the objective for BCESolver::model.
  GRBLinExpr& getObjective() { return gurobiObjective; }

  // Clear
  //! Clears the BCESolver
  void clear();

  // Set methods
  //! Set double parameter.
  void setParameter(BCESolver::DoubleParameter, double arg);
  //! Set integer parameter.
  void setParameter(BCESolver::IntParameter, int arg);
  //! Set bool parameter.
  void setParameter(BCESolver::BoolParameter, bool arg);
  //! Set objective function.
  void setObfun(int n) { gurobiObjective = objectiveFunctions[n]; }

  // get methods
  //! Get double parameter.
  double getParameter(BCESolver::DoubleParameter);
  //! Get integer parameter
  int getParameter(BCESolver::IntParameter);
  //! Get bool parameter.
  bool getParameter(BCESolver::BoolParameter);

  // Can't believe this doesn't exist in ANSI C++
  //! returns \f$base^{exponent}\f$.
  static int pow(int base, int exponent)
  {
    int result=1;
    for (int exponentCounter=0; exponentCounter<exponent; exponentCounter++)
      result*=base;
    return result;
  }
};

#endif
