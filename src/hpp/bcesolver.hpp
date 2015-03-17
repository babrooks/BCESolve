// class for solving BCE
// BAB 11-3-2012

#ifndef BCESOLVER_HPP
#define BCESOLVER_HPP

#include "bcecommon.hpp"
#include "bcegame.hpp"
#include "bcedata.hpp"
#include "bceexception.hpp"
#include "bceutilities.hpp"
#include <ilcplex/ilocplex.h>

// The base class for all BCE calculations.
class BCESolver 
{
public:
  enum DoubleParameter
    {
      MinAngleIncrement
    };
  enum IntParameter
    {
      CurrentObjective,
      DisplayLevel,
      BoundaryObjective1,
      BoundaryObjective2
    };
  enum BoolParameter
    {
      OnlyICUp,
      OnlyICDown,
      OnlyICDownToZero,
      OnlyPlayer1Down,
      OnlyPlayer0Up,
      OnlyLocalICDown,
      OnlyLocalICUp
    };

protected:
  BCEGame * game;

  long int numColumns; 
  long int numRows;
  long int numNonBasicVariables;

  long int numActions_total;
  long int numTypes_total;
  long int numActionsTypesPerPlayer_total;
  long int numActionsTypes_total;

  // Stores the # of IC constraints for each player.
  vector<int> numICConstraints;
  long int numICConstraints_total;
  long int numProbabilityVariables;
  long int numProbabilityConstraints;

  // Cplex components
  IloEnv env;
  IloModel model;
  IloCplex cplex;
  IloObjective cplexObjective;

  // Cplex variables and constraints.
  IloNumVarArray variables;  
  IloRangeArray constraints;

  // We allow the user to define any number of objective functions.
  // These are all stored in an array obfun. The index of the current
  // objective is currentobfun. 
  vector<IloNumExpr> objectiveFunctions;
  vector<IloNumExpr> bndryObjectives;
  int boundaryObjectiveIndex1;
  int boundaryObjectiveIndex2;

  // Stores location of non-zeros amongst all possible variables.
  list<int> nonZeroVariableLocations;
  vector<int> probabilityConstraintLocations;
  unordered_map<int,int> variableLocationsMap;

  // Parameters
  double minAngleIncrement;
  int currentObjective;
  int displayLevel;
  bool onlyICUp;
  bool onlyICDown;
  bool onlyICDownToZero;
  bool onlyPlayer1Down;
  bool onlyPlayer0Up;
  bool onlyLocalICDown;
  bool onlyLocalICUp;

  // Lists for storing equilibria and boundary points.
  list<BCEPoint> boundaryPoints;
  BCEData data;

  void countActionsTypes();

  // Helper for mapboundary  
  void mapFrontier(int plusOrMinus1, int plusOrMinus2, bool reversePrint);

  // Helper functions. These functions map between a linear index i
  // and valuations, types, and states. Mostly deprecated because of
  // use of BCECounter class.
  void indexToTypeActionDeviation(int index, int player, int &type, int &action, int &deviation);

public:
  // Constructors
  BCESolver();
  BCESolver(BCEGame & _game);

  // Destructor
  ~BCESolver()
  { env.end(); } 

  // Main routines
  void populate(); 
  void solve();
  void mapBoundary();
  void mapBoundary(const char * fname);

  void getData(BCEData & output);

  void bceToMap(map<int,double> & distribution);
  
  // Get methods
  IloCplex& getCplex() { return cplex; }
  IloNumExpr& getObjectiveFunction (int n) { return objectiveFunctions[n]; }
  IloObjective& getObjective() { return cplexObjective; }

  // Clear
  void clear();

  // Set methods
  void setParameter(BCESolver::DoubleParameter, double arg);
  void setParameter(BCESolver::IntParameter, int arg);
  void setParameter(BCESolver::BoolParameter, bool arg);
  void setObfun (int n) { cplexObjective.setExpr(objectiveFunctions[n]); }
  void setBndryObjective(int index, const IloNumExpr & expr);

  // get methods
  double getParameter(BCESolver::DoubleParameter);
  int getParameter(BCESolver::IntParameter);
  bool  getParameter(BCESolver::BoolParameter);

  // Can't believe this doesn't exist in ANSI C++
  static int pow(int base, int exponent)
  {
    int result=1;
    for (int exponentCounter=0; exponentCounter<exponent; exponentCounter++)
      result*=base;
    return result;
  }
};

#endif
