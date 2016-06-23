// Test of the BCEModel class
// First price auction with private values
// BAB 12-29-2011

#include "bce.hpp"
#include "bcebase.hpp"
#include "bceserialization.hpp"

class FPAModel : public BCEBase
{
private:
  int numValues;
  double probBehavioral;

public:
  FPAModel() {};
  
  FPAModel(int na, int nv, int nt, int no, 
	   double _probBehavioral) 
    : BCEBase(2,nv*nv,na,nt,no), numValues(nv), 
      probBehavioral(_probBehavioral)
  {};

  void stateToPrivateValues(int state, vector<int> &values) const
  {
    values.resize(2);

    values[0]=state%numValues;
    state-=values[0]; 
    values[1]=state/numValues;
  }

  double prior (int state, const vector<int> &types) const
  {
    // Use a uniform prior
    vector<int> values(2,0);
    stateToPrivateValues(state,values);

    if (values[0]==types[0] && values[1]==types[1])
      {
	// Stephen's example
	if (values[0] && values[1])
	  return 4.0/9.0;
	else if (values[0] || values[1])
	  return 2.0/9.0;
	else
	  return 1.0/9.0;
      }
    else
      return 0.0;
  }

  double objective(int state, const vector<int> &actions, int objectiveIndex) const
  {
    // Convert the state into a pair of valuations
    vector<int> values(2,0);
    stateToPrivateValues(state,values);

    if (objectiveIndex==0)
      {
	// Player 1's payoff
	if (actions[0]>actions[1] || (actions[0]==actions[1] && values[0]>values[1]))
	  return ((1.0-probBehavioral)*(1.0*values[0]/(numValues-1.0)
					-1.0*actions[1]/(numActions[1]-1.0))
		  +probBehavioral*0.5*(1.0*actions[0]/(numActions[0]-1.0)
				       *(1.0*values[0]/(numValues-1.0)
					 -0.5*actions[0]/(numActions[0]-1.0))));
	else if (actions[0]==actions[1] && values[0]==values[1])
	  return ((1.0-probBehavioral)*(1.0*values[0]/(numValues-1.0)
					-1.0*actions[1]/(numActions[1]-1.0))/2.0
		  +probBehavioral*0.5*(1.0*actions[0]/(numActions[0]-1.0)
				       *(1.0*values[0]/(numValues-1.0)
					 -0.5*actions[0]/(numActions[0]-1.0))));
	else
	  return (probBehavioral*0.5*(1.0*actions[0]/(numActions[0]-1.0)
				      *(1.0*values[0]/(numValues-1.0)
					-0.5*actions[0]/(numActions[0]-1.0))));
      }
    else if (objectiveIndex==1)
      {
	// Player 1's payoff
	if (actions[0]<actions[1] || (actions[0]==actions[1] && values[0]<values[1]))
	  return ((1.0-probBehavioral)*(1.0*values[1]/(numValues-1.0)
					-1.0*actions[0]/(numActions[0]-1.0))
		  +probBehavioral*0.5*(1.0*actions[1]/(numActions[1]-1.0)
				       *(1.0*values[1]/(numValues-1.0)
					 -0.5*actions[1]/(numActions[1]-1.0))));
	else if (actions[0]==actions[1] && values[0]==values[1])
	  return ((1.0-probBehavioral)*(1.0*values[1]/(numValues-1.0)
					-1.0*actions[0]/(numActions[0]-1.0))/2.0
		  +probBehavioral*0.5*(1.0*actions[1]/(numActions[1]-1.0)
				       *(1.0*values[1]/(numValues-1.0)
					 -0.5*actions[1]/(numActions[1]-1.0))));
	else
	  return (probBehavioral*0.5*(1.0*actions[1]/(numActions[1]-1.0)
				      *(1.0*values[1]/(numValues-1.0)
					-0.5*actions[1]/(numActions[1]-1.0))));
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

  bool dominated(int a, int t, int player) const
  {
    if ((static_cast<double>(a))/(static_cast<double>(numActions[player]) - 1.0) 
    	> (static_cast<double>(t))/(static_cast<double>(numTypes[player]) - 1.0))
      return (true);
    else
      return (false);
  }
};

int main(int argc, char ** argv)
{
  int t, a, ahat;
  double minAngleIncrement = 0.025;
  double probBehavioral = 0.01;

  int nbids=25;

  cout << "Starting main" << endl;

  FPAModel fpa(nbids,2,2,4,probBehavioral);
  fpa.setParameter(BCEBase::MinAngleIncrement,minAngleIncrement);

  cout << "Constructor finished" << endl;
  try
    {
      fpa.populate();
      cout << "Done populating" << endl;

      char filename[100];
      sprintf(filename,"spaknown_nb=%d_probBehavioral_%1.2f.bce",nbids,probBehavioral);

      IloCplex cplex=fpa.getCplex();
      cplex.getObjective().setExpr(fpa.getObjectiveFunction(1)+fpa.getObjectiveFunction(0));
      cout << "Objective function set" << endl;
      
      cplex.setParam(IloCplex::BarDisplay,1);
      cplex.setParam(IloCplex::SimDisplay,1);
      cplex.setParam(IloCplex::RootAlg,IloCplex::Dual);

      fpa.solve();
      cout << "Solved" << endl;

      cout << "Objective = " << setprecision(16) << cplex.getObjValue() << endl;

      fpa.mapBoundary();

      BCEData data;
      fpa.getData(data);
      data.setNumValues(vector<int>(2,2));
      cout << "number of equilibria " << data.equilibria.size() << endl;

      // vector<int> sortObj(2,0);
      // sortObj[1]=1;
      // data.sortEquilibria(sortObj);

      saveBCEData(data,filename);
    }
  catch (IloException & e)
    {
      cerr << "Concert exception caught: " << e << endl
	   << e.getMessage() << endl;
    }
  catch (BCEException & err)
    {
      cerr << "BCEException caught." << endl;
    }
  catch (...)
    {
      cout << "Unknown exception caught." << endl;
    }

  return 0;
}
