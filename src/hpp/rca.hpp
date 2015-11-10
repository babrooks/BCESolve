// Superclass for robust counter-factual analysis.
// BAB 2/26/2012

#ifndef RCA_H
#define RCA_H

#include <ilcplex/ilocplex.h>
#include <stdio.h>
#include <string>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <time.h>
#include <math.h>
#include <vector>
#include <list>
#include <fstream>
#include <assert.h>

using namespace std;

class RCAPoint
{
public: 
  vector<double> data;
  
  RCAPoint():data(2,0.0) {};
  RCAPoint(vector<double> newData):data(newData) {};
  RCAPoint(double p1, double p2):data(2,0.0) {data[0]=p1; data[1]=p2;};
  ~RCAPoint() {};
  
  RCAPoint &operator=(const RCAPoint &rhs) 
  {
    this->data = rhs.data;
  }

  int operator==(const RCAPoint &rhs)
  {
    if ( this->data != rhs.data ) return 0;
    return 1;
  }

  int operator<(const RCAPoint & rhs)
  {
    return 0;
  }

  friend ostream& operator<<(ostream& output, const RCAPoint &rhs)
  {
    output << rhs.data[0] << " " << rhs.data[1] << endl;

    return output;
  }
};

// Class for storing equilibria from PSM
class RCAEquilibrium
{
public:
  int numPlayers;
  vector<int> numActions;
  int numStates;
  int numTypes;
  int numMechanisms;

  vector<double> distribution;

  RCAEquilibrium () {}
  RCAEquilibrium (int np, const vector<int> & na, int ns, int nt, 
	 const vector<double> & data):
    numPlayers(np),
    numActions(na),
    numStates(ns),
    numTypes(nt),
    distribution(data) {}

  ~RCAEquilibrium() {}

  RCAEquilibrium &operator=(const RCAEquilibrium & rhs)
  {
    this->numPlayers=rhs.numPlayers;
    this->numActions=rhs.numActions;
    this->numStates=rhs.numStates;
    this->numTypes=rhs.numTypes;
    this->distribution=rhs.distribution;
  }

  friend ostream& operator<<(ostream& output, const RCAEquilibrium &rhs)
  {
    vector<double>::const_iterator distributionIterator;

    for (distributionIterator=rhs.distribution.begin(); 
	 distributionIterator!=rhs.distribution.end(); 
	 distributionIterator++)
      output << *distributionIterator << " ";

    return output;
  }

};

class RCABase
{
  enum DoubleParameter
    {
      MinAngleIncrement, ICSlack
    };
  enum IntParameter
    {
      CurrentObjective
    };

protected:
  int numPlayers; // number of players.
  vector<int> numActions; // number of action profiles in mechanisms 0
			  // (observed) and 1 (counterfactual)
  int numActions_total;
  int numStates; // number of payoff relevant states
  int numTypes; // number of private types for each player
  int numObjectives; // number of objective functions. Must be >= 2.
  int numColumns; // number of columns in the constraint matrix.
  int numRows; // number of rows in the constraint matrix
  int numNonBasicVariables; // number of non-basic variables in the
			    // linear program
  int numMechanisms; // A lot of the code only works for two
		     // mechanisms. Just to make it easier in case
		     // later I decide to generalize.

  // Stores the # of IC constraints for each player and mechanism,
  // i.e. player i in mechanism j has numICConstraints[j][i]
  // constraints.
  vector< vector<int> >numICConstraints; 
  vector<int> numICConstraints_totalByMechanism; // total # of IC
						 // constraints in the
						 // observed mechanism
  int numICConstraints_total;
  int numProbabilityVariables;
  int numProbabilityEqualityConstraints;
  int numProbabilityUBConstraints;
  int numProbabilityLBConstraints;
  int numProbabilityConstraints;

  vector<double> observedDistribution;

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
  IloNumExpr objectiveIndex1;
  IloNumExpr objectiveIndex2;
  int boundaryObjectiveIndex1;
  int boundaryObjectiveIndex2;

  // Stores location of non-zeros amongst all possible variables.
  vector<int> nonZeroVariableLocations;

  // Parameters
  double minAngleIncrement; // Minimum angle for each iteration of the parametric simplex method
  double ICslack; // A deviation can benefit the deviator by no more than ICslack>=0.0
  int currentObjective; // An integer indicating the objective used to solve.

  // Lists for storing equilibria and boundary points.
  list<RCAPoint> boundaryPoints;
  list<RCAEquilibrium> equilibria;

  // Helper for mapboundary  
  void mapFrontier(int plusOrMinus1, int plusOrMinus2, bool reversePrint);

  // Overload the << operator.
  friend ostream& operator<<(ostream& output, RCABase &rhs);

  // Helper functions
  // These functions map between a linear index i and valuations, types, and states.
  void indexToStateTypeAction(int index, 
			      int &state, 
			      vector<int> &types, 
			      vector< vector<int> > &actions); 
  void indexToStateTypeAction(int index, 
			      int &state, 
			      int &type, 
			      vector<int> &action); 
  void indexToTypeActionDeviation(int index, 
				  int &type,
				  vector<int> &action, 
				  int &deviation, 
				  int mechanism);
  void indexToStateType(int index, 
			int &state, 
			vector<int> &types);
  void indexToStateAction(int index, 
			  int &state, 
			  vector<int> &actions,
			  int mechanism);
  int stateTypeToIndex(int state, 
		       const vector<int> & types);
  int actionToIndex(const vector< vector<int> > &actions,
		     int mechanism);

  // Returns the probability of the action profile being observed.
  double observedProbability(const vector<int> &actions) const;
  
  // Bounds on the conditional distribution over states given a
  // type. For example, when there is no minimum information, there
  // should be just one type for every player, LB should always return
  // 0.0 and UB should always return 1.0. For private values, players
  // should have one type for every value, LB always returns 0.0, and
  // UB returns 1.0 whenever the state-value matches the players
  // type-value, and 0.0 whenever the state-value differs from
  // type-value.
  virtual double conditionalUB(int player, int state, int type) {return 1.0;};
  virtual double conditionalLB(int player, int state, int type) {return 0.0;};

  // Maps actions and values into payoffs. The argument objective can
  // be any number between 0 and numObjectives-1, inclusive.
  // objective=0 through 2*numPlayers-1 are the payoffs of players 1
  // through numPlayers in mechanisms 0 and 1, i.e. player i in
  // mechanism j is j*numPlayers+i..
  virtual double payoff(int state, vector< vector<int> > actions, int objective) const = 0; 

  // Indicates if a combination of actions and types is dominated.
  virtual bool dominated(int action, int type, int player, int mechanism) const {return false;} ; 
  // Check if no players' action is dominated
  bool dominated(const vector<int> &actions, const vector<int> &types, int mechanism); 
  bool dominated(const vector< vector<int> > &actions, const vector<int> &types);
  bool dominated(const vector<int> &action, int type, int player);

public:
  // Constructors
  RCABase();
  RCABase(int numPlayersArg, 
	  vector<int> numActionsArg, 
	  int numStatesArg, 
	  int numTypesArg, 
	  int numObjectivesArg);
  RCABase(int numPlayersArg, 
	  vector<int> numActionsArg, 
	  int numStatesArg, 
	  int numTypesArg, 
	  int numObjectivesArg,
	  vector<double> distributionArg);

  // Destructor
  ~RCABase()
  { env.end(); } 

  // Main routines
  void populate(); 
  void solve();
  void mapBoundary();
  void clear();

  // Get methods
  IloCplex getCplex() { return cplex; }
  IloNumExpr getObjectiveFunction (int n) { return objectiveFunctions[n]; }
  IloObjective getObjective() { return cplexObjective; }

  // Set methods
  void setParameter(RCABase::DoubleParameter, double arg);
  void setParameter(RCABase::IntParameter, int arg);
  void setObfun (int n) { cplexObjective.setExpr(objectiveFunctions[n]); }

  void distributionToVector(vector<double> & distribution);
  void actionMarginalToVector(vector<double> & marginal);

  // Can't believe this doesn't exist in ANSI C++
  static int pow(int base, int exponent)
  {
    int result=1;
    for (int exponentCounter=0; exponentCounter<exponent; exponentCounter++)
      result*=base;
    return result;
  }
}; // RCABase

#endif
