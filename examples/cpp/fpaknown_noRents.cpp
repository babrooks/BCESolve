// Test of the BCEModel class
// First price auction with private values
// Modified version that constraints bidders to get no variation in 
// rents from learning their bid over what they get based on valuation.
// BAB 8-15-2012

#include "bce.hpp"
#include "bcebase.hpp"
#include "bceserialization.hpp"

class FPAModel : public BCEBase
{
private:
  int numValues;
  double entryCost;

public:
  double alpha0;
  double alpha1;
  double highbid;

  FPAModel() {};
  
  FPAModel(int na, int nv, int no, double _entryCost) 
    : BCEBase(2,nv*nv,na,nv,no), numValues(nv), 
      entryCost(_entryCost), 
      alpha0(1.0), alpha1(1.0), highbid(1.0)
  {};

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
    vector<int> values(2,0);
    stateToPrivateValues(state,values);

    if (values[0]==types[0] && values[1]==types[1])
      {
	// v^alpha
	return ((std::pow(static_cast<double>(values[0]+1)/numValues,alpha0)
		 -std::pow(static_cast<double>(values[0])/numValues,alpha0))
		*(std::pow(static_cast<double>(values[1]+1)/numValues,alpha1)
		  -std::pow(static_cast<double>(values[1])/numValues,alpha1)));

	// // Uniform
	// return (1.0/(numValues*numValues));

	// // Stephen's example
	// if (values[0] && values[1])
	//   return 4.0/9.0;
	// else if (values[0] || values[1])
	//   return 2.0/9.0;
	// else
	//   return 1.0/9.0;
      }
    else
      return 0.0;
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
	  return (1.0*values[0]/(numValues-1.0)-highbid*actions[0]/(numActions[0]-1.0)
		  -(actions[0]? entryCost : 0.0));
	else if (actions[0]==actions[1] && values[0]==values[1])
	  return ((1.0*values[0]/(numValues-1.0)-highbid*actions[0]/(numActions[0]-1.0))/2.0
		  -(actions[0]? entryCost : 0.0));
	else
	  return -(actions[0]? entryCost : 0.0);
      }
    else if (objectiveIndex==1)
      {
	// Player 1's payoff
	if (actions[0]<actions[1] || (actions[0]==actions[1] && values[0]<values[1]))
	  return (1.0*values[1]/(numValues-1.0)-highbid*actions[1]/(numActions[1]-1.0)
		  -(actions[1]? entryCost : 0.0));
	else if (actions[0]==actions[1] && values[0]==values[1])
	  return ((1.0*values[1]/(numValues-1.0)-highbid*actions[1]/(numActions[1]-1.0))/2.0
		  -(actions[1]? entryCost : 0.0));
	else
	  return -(actions[1]? entryCost : 0.0);
      }
    else if (objectiveIndex==2)
      {
	// Revenue
	return highbid*(actions[0]>actions[1]? (1.0*actions[0])/(numActions[0]-1.0) : 
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
    if ((static_cast<double>(a))/(static_cast<double>(numActions[player]) - 1.0) 
    	> (static_cast<double>(t))/(static_cast<double>(numTypes[player]) - 1.0))
      return (true);
    else
      return (false);
    // return (false);
  }
};

int main(int argc, char ** argv)
{
  int t, a, ahat;
  double entryCost=0.00;
  double minAngleIncrement = 0.025;
  int nbids=25;
  int nvals=6;

  cout << "Starting main" << endl;

  try
    {
      FPAModel fpa(nbids,nvals,4,entryCost);
      fpa.setParameter(BCEBase::MinAngleIncrement,minAngleIncrement);
      fpa.alpha0 = 1.0; fpa.alpha1=1.0;
      fpa.highbid=1.0;

      fpa.setParameter(BCEBase::OnlyICUp,false);

      cout << "Constructor finished" << endl;

      fpa.populate();
      cout << "Done populating" << endl;

      char filename[100];
      sprintf(filename,"fpaknown_nv=%d_nb=%d_entrycost_%1.2f_alpha0=%1.2f_alpha1=%1.2f_highbid=%1.2f_bdry.bce",
	      nvals,nbids,entryCost,fpa.alpha0,fpa.alpha1,fpa.highbid);

      IloCplex cplex=fpa.getCplex();
      cplex.getObjective().setExpr(-1.0*fpa.getObjectiveFunction(2)); // revenue
      cout << "Objective function set" << endl;
      
      cplex.setParam(IloCplex::RootAlg,IloCplex::Dual);
      fpa.setParameter(BCEBase::DisplayLevel,1);

      fpa.solve();
      cout << "Solved" << endl;

      cout << "Objective = " << setprecision(16) << cplex.getObjValue() << endl;

      fpa.mapBoundary();

      BCEData data;
      fpa.getData(data);
      data.setNumValues(vector<int>(2,nvals));
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
