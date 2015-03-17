// Second price auction with common but not perfectly correlated values
// BAB 12-29-2011

#include "bce.hpp"
#include "bcebase.hpp"
#include "bceserialization.hpp"

class SPAModel : public BCEBase
{
private:
  int numValues;
  double entryCost;
  double probPayOwn;

public:
  SPAModel() {};
  
  SPAModel(int na, int nv, int no, double _entryCost, double _probPayOwn) 
    : BCEBase(2,nv*nv,na,1,no), numValues(nv), entryCost(_entryCost)
  {};

  int getNumValues(){return numValues;}

  int stateToPrivateValues(int state, vector<int> &values)
  {
    values.resize(2);

    values[0]=state%numValues;
    state-=values[0]; 
    values[1]=state/numValues;
  }

  double prior (int state, const vector<int> &types)
  {
    // Use a uniform prior
    // return 1.0/(numValues*numValues);

    // Stephen's example
    vector<int> values;
    stateToPrivateValues(state,values);
    if (values[0] && values[1])
      return 4.0/9.0;
    else if (values[0] || values[1])
      return 2.0/9.0;
    else
      return 1.0/9.0;
  }

  double objective(int state, const vector<int> &actions, int objectiveIndex)
  {
    // Convert the state into a pair of valuations
    vector<int> values(2,0);
    stateToPrivateValues(state,values);

    if (objectiveIndex==0)
      {
	// Player 1's payoff
	if (actions[0]>actions[1] || (actions[0]==actions[1] && values[0]>values[1]))
	  return (1.0*values[0]/(numValues-1.0)-(1.0-probPayOwn)*actions[1]/(numActions[1]-1.0)
		  -(actions[0]>0? entryCost : 0.0) - probPayOwn*actions[0]/(numActions[0]-1.0));
	else if (actions[0]==actions[1] && values[0]==values[1])
	  return ((1.0*values[0]/(numValues-1.0)-(1.0-probPayOwn)*actions[1]/(numActions[1]-1.0))/2.0
		  -(actions[0]>0? entryCost : 0.0) - probPayOwn*actions[0]/(numActions[0]-1.0));
	else
	  return -(actions[0]>0? entryCost : 0.0);
      }
    else if (objectiveIndex==1)
      {
	// Player 1's payoff
	if (actions[0]<actions[1] || (actions[0]==actions[1] && values[0]<values[1]))
	  return (1.0*values[1]/(numValues-1.0)-(1.0-probPayOwn)*actions[0]/(numActions[0]-1.0)
		  -(actions[1]>0? entryCost : 0.0) - probPayOwn*actions[1]/(numActions[1]-1.0));
	else if (actions[0]==actions[1] && values[0]==values[1])
	  return ((1.0*values[1]/(numValues-1.0)-(1.0-probPayOwn)*actions[0]/(numActions[0]-1.0))/2.0
		  -(actions[1]>0? entryCost : 0.0) - probPayOwn*actions[1]/(numActions[1]-1.0));
	else
	  return -(actions[1]>0? entryCost : 0.0);
      }
    else if (objectiveIndex==2)
      {
	// Revenue
	return (actions[0]>actions[1]? (1.0*actions[0])/(numActions[0]-1.0) : 
		(1.0*actions[1])/(numActions[1]-1.0));
      }
    else if (objectiveIndex==3)
      {
	// Surplus
	if (actions[0]>actions[1] || (actions[0]==actions[1] && values[0]>values[1]))
	  return (1.0*values[0]/(numValues-1.0)); // Player 1 won
	else
	  return (1.0*values[1]/(numValues-1.0)); // Either player 2 won or they tied with same val.
      }
  }

  bool dominated(int a, int t, int player)
  {
    return false;
  }
};

int main(int argc, char ** argv)
{
  double entryCost=0.0;
  double minAngleIncrement = 0.025;
  double probPayOwn = 0.01;
  int nb=35;

  int t, a, ahat;

  char filename[100];
  sprintf(filename,"spa_nb=%d_entrycost_%1.2f_payown_%1.2f.bce",nb,entryCost,probPayOwn);

  cout << "Starting main" << endl;

  SPAModel spa(nb,2,4,entryCost,probPayOwn);
  spa.setParameter(BCEBase::MinAngleIncrement,minAngleIncrement);

  cout << "Constructor finished" << endl;
  try
    {
      spa.populate();
      cout << "Done populating" << endl;

      IloCplex cplex=spa.getCplex();
      // // Sum of bidders' surplus
      // cplex.getObjective().setExpr(spa.getObjectiveFunction(1)+spa.getObjectiveFunction(0));
      cplex.getObjective().setExpr(1.0*spa.getObjectiveFunction(0));
      cout << "Objective function set" << endl;
      
      cplex.setParam(IloCplex::BarDisplay,1);
      cplex.setParam(IloCplex::SimDisplay,1);
      cplex.setParam(IloCplex::RootAlg,IloCplex::Dual);

      spa.solve();
      cout << "Solved" << endl;

      cout << "Objective = " << setprecision(16) << cplex.getObjValue() << endl;

      spa.mapBoundary();
      cout << "Mapped boundary" << endl;

      BCEData data;
      spa.getData(data);
      data.setNumValues(vector<int>(2,spa.getNumValues()));

      cout << "Sorting data." << endl;
      vector<int> sortObj(2,0);
      sortObj[1]=1;
      data.sortEquilibria(sortObj);

      saveBCEData(data,filename);
    }
  catch (IloException & e)
    {
      cerr << "Concert exception caught: " << e << endl
	   << e.getMessage() << endl;
    }
  catch (BCEException & bcee)
    {
      cerr << "BCEException caught: " << endl;
      if (bcee.errorType == BCEException::NotProbDistr)
	cerr << "Not a probability distribution" << endl;
    }
  catch (...)
    {
      cout << "Unknown exception caught." << endl;
    }

  return 0;
}

