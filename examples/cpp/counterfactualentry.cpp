// A 1 player counterfactual example
#include "gurobi_c++.h"
#include <stdio.h>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <cmath>

using namespace std;

double profit(int s, int n)
{
  double A = s+1.0;
  if (n==1)
    return A*A/4.0;
  if (n==2)
    return A*A/9.0;
  return 0.0;
}

double payoff(int ai, int aj, int s,double fee)
{
  if (ai==1)
    return profit(s,ai+aj)-fee;
  return 0.0;
}

double consumerSurplus(int s, int n)
{
  double A = s+1.0;
  if (n==1)
    return A*A/8.0;
  if (n==2)
    return 4.0*A*A/9.0;
  return 0.0;
}

int main () 
{
  bool variableInfo = false;
  stringstream ss;
  ss << "counterfactualentry";
  if (variableInfo)
    ss << "_variableinfo";
  ss << ".log";

  ofstream ofs(ss.str().c_str());

  double obsFee = 0.75;
  
  // z is weight given to state 1 in counterfactual
  for (double unobsFee = 0.0; unobsFee <= 2.5; unobsFee += 0.001)
    {
      try
	{
	  GRBEnv env = GRBEnv();
	  env.set(GRB_IntParam_Method,2);
	  env.set(GRB_IntParam_OutputFlag,0);
	  GRBModel model(env);

	  int numStates=3;
	  int numVariables = 16*numStates;

	  GRBVar * var = model.addVars(numVariables);
	  // formula for variable location: 16*state+8*obsAction0+4*unObsAction0
	  // +2*obsAction1+unobsAction1.
      
	  model.update();
      
	  GRBLinExpr obsProfit=0.0;
 	  GRBLinExpr obsCS=0.0;
	  GRBLinExpr obsWelfare=0.0;
	  GRBLinExpr unobsProfit=0.0;
	  GRBLinExpr unobsCS=0.0;
	  GRBLinExpr unobsWelfare=0.0;

	  // cout << "Adding probability constraints..." << endl;

	  // States are equally likely, in state 2, both enter with
	  // prob 1, in state 1, exactly one enters, with equal prob,
	  // in state 0, neither enter.

	  // Add probability constraints and define objective
	  for (int s=0; s<numStates; s++)
	    {
	      GRBLinExpr stateProb = 0;
	      for (int ao0 = 0; ao0 < 2; ao0++)
		{
		  for (int ao1 = 0; ao1 < 2; ao1++)
		    {
		      GRBLinExpr prob = 0;
		      for (int au0 = 0; au0 < 2; au0++)
			{
			  for (int au1 = 0; au1 < 2; au1++)
			    {
			      int row = 16*s+8*ao0 + 4*au0 + 2*ao1 + au1;

			      prob += var[row];
			      stateProb += var[row];

			      obsProfit += var[row]*(payoff(ao0,ao1,s,obsFee)+payoff(ao1,ao0,s,obsFee));
			      unobsProfit += var[row]*(payoff(au0,au1,s,unobsFee)+payoff(au1,au0,s,unobsFee));
			      // obsProfit += var[row]*(ao0+ao1)*(profit(s,ao0+ao1)-obsFee);
			      // unobsProfit += var[row]*(au0+au1)*(profit(s,au0+au1)-unobsFee);
			      obsCS += var[row]*consumerSurplus(s,ao0+ao1);
			      unobsCS += var[row]*consumerSurplus(s,au0+au1);
			      
			    } // au1
			} // au0
		      if (!variableInfo)
			{
			  if (s==0)
			    {
			      if (ao0==0 && ao1==0)
				model.addConstr(prob == 1.0/3.0);
			      else
				model.addConstr(prob == 0.0); 
			    }
			  if (s==1)
			    {
			      if (ao0!=ao1)
				model.addConstr(prob == 1.0/6.0);
			      else
				model.addConstr(prob == 0.0);
			    }
			  if (s==2)
			    {
			      if (ao0==1 && ao1==1)
				model.addConstr(prob == 1.0/3.0);
			      else
				model.addConstr(prob == 0.0);
			    }
			}
		    } // ao1
		} // ao0
	      model.addConstr(stateProb == 1.0/3.0);
	    } // s
	  obsWelfare += obsCS + obsProfit;
	  unobsWelfare += unobsCS + unobsProfit;

	  // cout << "Adding obedience constraints..." << endl;
      
	  // Obedience constraints
	  for (int ao = 0; ao < 2; ao++)
	    {
	      for (int au = 0; au < 2; au++)
		{
		  // Deviation in observed action
		  GRBLinExpr obsDevGain0 = 0.0;
		  GRBLinExpr unobsDevGain0 = 0.0;
		  GRBLinExpr obsDevGain1 = 0.0;
		  GRBLinExpr unobsDevGain1 = 0.0;
		  for (int s = 0; s < numStates; s++)
		    {
		      for (int aoj = 0; aoj < 2; aoj++)
			{
			  for (int auj = 0; auj < 2; auj++)
			    {
			      int row0 = 16*s+8*ao+4*au+2*aoj+auj;
			      int row1 = 16*s+8*aoj+4*auj+2*ao+au;

			      obsDevGain0 += (payoff(1-ao,aoj,s,obsFee)-payoff(ao,aoj,s,obsFee))*var[row0];
			      obsDevGain1 += (payoff(1-ao,aoj,s,obsFee)-payoff(ao,aoj,s,obsFee))*var[row1];
			      unobsDevGain0 += (payoff(1-au,auj,s,unobsFee)-payoff(au,auj,s,unobsFee))*var[row0];
			      unobsDevGain1 += (payoff(1-au,auj,s,unobsFee)-payoff(au,auj,s,unobsFee))*var[row1];
			  
			    } // auj
			} // aoj
		    } // s
		  model.addConstr(obsDevGain0 <= 0);
		  model.addConstr(obsDevGain1 <= 0);
		  model.addConstr(unobsDevGain0 <= 0);
		  model.addConstr(unobsDevGain1 <= 0);
	      
		} // au
	    } // ao

	  // cout << "Optimizing..." << endl;
      
	  model.setObjective(unobsWelfare);
	  model.set(GRB_IntAttr_ModelSense,0);
	  model.optimize();

	  // cout << "Done minimizing..." << endl;

	  ofs << unobsFee << " " << setprecision(8)
	      << obsWelfare.getValue() << " " 
	      << obsCS.getValue() << " " 
	      << obsProfit.getValue() << " " 
	      << unobsWelfare.getValue() << " ";

	  model.set(GRB_IntAttr_ModelSense,1);
	  model.optimize();
	  ofs << unobsWelfare.getValue() << " ";

	  model.setObjective(unobsCS);
	  model.set(GRB_IntAttr_ModelSense,0);
	  model.optimize();
	  ofs << unobsCS.getValue() << " ";

	  model.set(GRB_IntAttr_ModelSense,1);
	  model.optimize();
	  ofs << unobsCS.getValue() << " ";

	  model.setObjective(unobsProfit);
	  model.set(GRB_IntAttr_ModelSense,0);
	  model.optimize();
	  ofs << unobsProfit.getValue() << " ";

	  model.set(GRB_IntAttr_ModelSense,1);
	  model.optimize();
	  ofs << unobsProfit.getValue() << " ";

	  ofs << endl;

	  // cout << "Done maximizing..." << endl;


	  // cout << "Done!" << endl;

	}
      catch (GRBException &e)
	{
	  cout << e.getMessage() << endl;
	}
    } // for z

  ofs.close();
  
  return 0;
}

