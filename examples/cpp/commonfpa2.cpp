// Test of the BCEModel class
// First price auction with common but not perfectly correlated values
// BAB 12-29-2011

#include "bce.hpp"
#include "bcebase.hpp"
#include "bceserialization.hpp"

class FPAModel : public BCEBase
{
private:
  int numValues;
  double entryCost;

public:
  FPAModel() {};
  
  FPAModel(int na, int nv, int no, double _entryCost) 
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
	  return (1.0*values[0]/(numValues-1.0)-1.0*actions[0]/(numActions[0]-1.0)
		  +(actions[0]>0? entryCost : 0.0));
	else if (actions[0]==actions[1] && values[0]==values[1])
	  return ((1.0*values[0]/(numValues-1.0)-1.0*actions[0]/(numActions[0]-1.0))/2.0
		  +(actions[0]>0? entryCost : 0.0));
	else
	  return (actions[0]>0? entryCost : 0.0);
      }
    else if (objectiveIndex==1)
      {
	// Player 1's payoff
	if (actions[0]<actions[1] || (actions[0]==actions[1] && values[0]<values[1]))
	  return (1.0*values[1]/(numValues-1.0)-1.0*actions[1]/(numActions[1]-1.0)
		  +(actions[1]>0? entryCost : 0.0));
	else if (actions[0]==actions[1] && values[0]==values[1])
	  return ((1.0*values[1]/(numValues-1.0)-1.0*actions[1]/(numActions[1]-1.0))/2.0
		  +(actions[1]>0? entryCost : 0.0));
	else
	  return (actions[1]>0? entryCost : 0.0);
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
  double entryCost=0.01;
  double minAngleIncrement = 0.025;
  int nv=100;

  int t, a, ahat;

  char filename[100];
  sprintf(filename,"noinfo_maxp0_nv=%d_entrycost_%1.2f.bce",nv,entryCost);

  cout << "Starting main" << endl;

  FPAModel fpa(nv,2,4,entryCost);
  fpa.setParameter(BCEBase::MinAngleIncrement,minAngleIncrement);

  cout << "Constructor finished" << endl;
  try
    {
      fpa.populate();
      cout << "Done populating" << endl;

      IloCplex cplex=fpa.getCplex();
      // // Sum of bidders' surplus
      // cplex.getObjective().setExpr(fpa.getObjectiveFunction(1)+fpa.getObjectiveFunction(0));
      cplex.getObjective().setExpr(1.0*fpa.getObjectiveFunction(0));
      cout << "Objective function set" << endl;
      
      cplex.setParam(IloCplex::BarDisplay,1);
      cplex.setParam(IloCplex::SimDisplay,1);
      cplex.setParam(IloCplex::RootAlg,IloCplex::Dual);

      fpa.solve();
      cout << "Solved" << endl;

      cout << "Objective = " << setprecision(16) << cplex.getObjValue() << endl;

      // fpa.mapBoundary();
      // cout << "Mapped boundary" << endl;

      BCEData data1, data2;
      fpa.getData(data1);
      data1.setNumValues(vector<int>(2,fpa.getNumValues()));

      cout << "Sorting data." << endl;
      vector<int> sortObj(2,0);
      sortObj[1]=1;
      data1.sortEquilibria(sortObj);

      cout << "Data1: " << endl;
      cout << data1.numPlayers << " " << data1.numObjectives << " " << endl;
      cout << data1.equilibria.size() << endl;

      saveBCEData(data1,filename);

      loadBCEData(data2,filename);
      
      cout << "Data2: " << endl;
      cout << data2.numPlayers << " " << data2.numObjectives << " " << endl;
      cout << data2.equilibria.size() << endl;
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

