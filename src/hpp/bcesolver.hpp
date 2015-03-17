// class for solving BCE
// BAB 11-3-2012

#ifndef BCESOLVER_HPP
#define BCESOLVER_HPP

#include "bcecommon.hpp"
#include "bcegame.hpp"
#include "bcedata.hpp"
#include "bcepoint.hpp"
#include "bcecounter.hpp"
#include "bceexception.hpp"
#include "bceutilities.hpp"
#include <ilcplex/ilocplex.h>

//! Solves a BCEGame using cplex.
/*! This class contains routines for solving a BCEGame object using
    CPLEX. The main method is BCESolver::populate, which initializes a
    CPLEX model with the incentive constraints for the BCEGame, and
    also creates objectives corresponding to the number of objectives
    in the BCEGame object. The class also contains the method
    BCESolver::mapBoundary, which solves linear programs to trace out
    the frontier for the objectives in BCESolver::BoundaryObjective1
    and BCESolver::BoundaryObjective2. */
class BCESolver 
{
public:

  //! Double parameters
  /*! An enumeration type for parameters of type double. Pass to
      BCESolver::setParam and BCESolver::getParam to set or retrieve a
      double parameter value. */ 
  enum DoubleParameter
    {
      MinAngleIncrement /*!< When running the map boundary method,
                           this is the minimum change in the direction
                           on each iteration. */
    };
  //! Integer parameters
  /*! An enumeration type for parameters of type int. Pass to
      BCESolver::setParam and BCESolver::getParam to set or retrieve a
      double parameter value. */ 
  enum IntParameter
    {
      CurrentObjective, /*!< The index of the current objective. Not
                           currently used? */
      DisplayLevel, /*!< Controls the frequency and detail with which
                       progress of the algorithm is reported. 0
                       corresponds to no output, 1 is some output. Is
                       also passed to the BCESolver::cplex solver, so
                       it controls the level of output of CPLEX. */
      BoundaryObjective1, /*!< The first objective used when mapping
                             the boundary. */
      BoundaryObjective2 /*!< The second objective used when mapping
                             the boundary. */
    };
  
  //! Bool parameters
  /*! An enumeration type for parameters of type bool. Pass to
      BCESolver::setParam and BCESolver::getParam to set or retrieve a
      double parameter value. Will be replaced by new BCEGame
      architecture with a constraint checker. */ 
  enum BoolParameter
    {
    };

protected:
  BCEGame * game; /*!< Pointer to the BCEGame object that is being
                     solved. */

  long int numColumns; /*!< Number of columns in the constraint
                          matrix. */
  long int numRows; /*!< Number of rows in the constraint matrix. */
  long int numNonBasicVariables; /*!< Number of non-basic variables? */

  long int numActions_total; /*!< Total number of action profiles for
                                the players. */
  long int numTypes_total; /*!< Total number of types for the
                              players. */
  long int numActionsTypesPerPlayer_total; /*!< Total number of
                                              combinations of actions
                                              and types for each
                                              player. Used? */
  long int numActionsTypes_total; /*!< Total number of actions and
				    types. */

  // Stores the # of IC constraints for each player.
  vector<int> numICConstraints; /*!< The number of IC constraints for
                                   each player. */
  long int numICConstraints_total; /*!< The total number of IC
                                      constraints. */
  long int numProbabilityVariables; /*!< The number of probability
                                       variables. */
  long int numProbabilityConstraints; /*!< The total number of
                                         constraints on probabilities
                                         adding up to 1. */

  // Cplex components
  IloEnv env; /*!< The CPLEX environment. Parent of all of the CPLEX
                 objects that are members of BCESolver. */
  IloModel model; /*!< The CPLEX model. */
  IloCplex cplex; /*!< The CPLEX solver engine. */
  IloObjective cplexObjective; /*!< The objective for
                                  BCESolver::cplex. */

  // Cplex variables and constraints.
  IloNumVarArray variables;  /*!< The variables for
                                BCESolver::model. */
  IloRangeArray constraints; /*!< The constraints for
                                BCESolver::model. */

  // We allow the user to define any number of objective functions.
  // These are all stored in an array obfun. The index of the current
  // objective is currentobfun. 
  vector<IloNumExpr> objectiveFunctions; /*!< An array of objective
                                            functions, corresponding
                                            to the objectives in the
                                            BCEGame object. */
  vector<IloNumExpr> bndryObjectives; /*!< Boundary objectives, not
                                         currently being used. */
  int boundaryObjectiveIndex1; /*!< Index of the first boundary objective. */
  int boundaryObjectiveIndex2;/*!< Index of the second boundary
                                 objective. */

  // Stores location of non-zeros amongst all possible variables.
  list<int> nonZeroVariableLocations; /*!< The solver only creates
                                         variables for events that
                                         could occur with positive
                                         probability under the prior
                                         in the BCEGame object. This
                                         property stores the locations
                                         of the non-zero probability
                                         outcomes. */
  vector<int> probabilityConstraintLocations; /*!< Stores locations of
                                                 the probability
                                                 constraints in the
                                                 constraints
                                                 array. */
  unordered_map<int,int> variableLocationsMap; /*!< A map from
                                                  standard indices to
                                                  the corresponding
                                                  non-zero variable
                                                  location. (See
                                                  BCECounter.) */

  // Parameters
  double minAngleIncrement; /*!< The minimum increment of the angle
                               for BCESolver::mapBoundary. */
  int currentObjective; /*!< Index of the current objective. Not
                           currently being used. */
  int displayLevel; /*!< The display level. 0 is no output, 1 is some
                       output. Also passed to CPLEX, so 2+ controls
                       CPLEX output. */

  // Lists for storing equilibria and boundary points.
  list<double> boundaryXs; /*!< List of x-coordinates of boundary
			     points traced out by the
			     BCESolver::mapBoundary routine. */
  list<double> boundaryYs; /*!< List of y-coordinates of boundary
			     points traced out by the
			     BCESolver::mapBoundary routine. */
  BCEData data; /*!< BCEData object for serializing output of the
                   algorithm. */

  void countActionsTypes(); /*!< Initializes the properties that count
                               the number of actions and types. */

  // Helper for mapboundary  
  void mapFrontier(int plusOrMinus1, int plusOrMinus2, bool reversePrint); 
  /*!< Helper function for mapBoundary. Traces out one quadrant of the
     frontier. */

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
  BCESolver(BCEGame & _game);

  //! Destructor
  ~BCESolver()
  { env.end(); } 

  // Main routines

  //! Main populate routine
  /*! Populates the CPLEX model with constraints and initializes the
      objectives. */
  void populate(); 

  //! Solve method
  /*! Sets CPLEX solver parameters and calls IloCplex::solve. */
  void solve();

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

  //! Returns the data object
  /*! Copies the data object to output. */
  void getData(BCEData & output);

  //! Copies the BCE to a map
  /*! This method copies the BCE, which is a vector of doubles, to a
      map from standard indices to probabilities. Mainly for saving
      space in data files. */
  void bceToMap(map<int,double> & distribution);
  
  // Get methods
  //! Returns the BCESolver::cplex object.
  IloCplex& getCplex() { return cplex; }
  //! Returns the \f$n\f$th objective function. 
  IloNumExpr& getObjectiveFunction (int n) { return objectiveFunctions[n]; }
  //! Returns the objective for BCESolver::cplex.
  IloObjective& getObjective() { return cplexObjective; }

  // Clear
  //! Clears the BCESolver?
  void clear();

  // Set methods
  //! Set double parameter.
  void setParameter(BCESolver::DoubleParameter, double arg);
  //! Set integer parameter.
  void setParameter(BCESolver::IntParameter, int arg);
  //! Set bool parameter.
  void setParameter(BCESolver::BoolParameter, bool arg);
  //! Set objective function.
  void setObfun (int n) { cplexObjective.setExpr(objectiveFunctions[n]); }
  //! Set boundary objective.
  void setBndryObjective(int index, const IloNumExpr & expr);

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
