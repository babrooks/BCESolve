// A 1 player counterfactual example
#include "gurobi_c++.h"
#include <stdio.h>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <cmath>

using namespace std;

double utility0(int a0, int a1, int s)
{
  return (2.0*(a0==a1)-1)*abs(2.0-2.0*s-a0-a1);
}

double utilityHat0(int a0, int a1, int s, double z)
{
  return utility0(a0,a1,s)*(2-z+s*(2*z-2));
}

int main () 
{
  bool variableInfo = false;
  stringstream ss;
  ss << "counterfactual2player";
  if (variableInfo)
    ss << "_variableinfo";
  ss << ".log";
  ofstream ofs(ss.str().c_str());

  // z is weight given to state 1 in counterfactual
  for (double z = 0; z<= 2.005; z += 0.01)
    {
      // Zero sum game, player 0's payoff is
      // (2*(a0==a1)-1)*|2*(1-s)-a0-a1|. So, if they mismatch, the first
      // term is -1 and the second term is 1, so player 0's payoff is
      // -1. If they match in state 0, player 0's payoff is 2 if a0=a1=0
      // and 0 if a0=a1=1. If they match in state 1, player 0's payoff is
      // 0 if a0=a1=0 and it's 2 if a0=a1=1.

      // In counterfactual, we multiply payoffs by z in state 2
      // (effectively doubling the ex ante probability of state 2).

      // Impose that all outcomes are equiprobable in observed game
      // (consistent with no info), so each state, ao0, ao1 combo has prob 0.125.
  
      try
	{
	  GRBEnv env = GRBEnv();
	  env.set(GRB_IntParam_Method,2);
	  env.set(GRB_IntParam_OutputFlag,0);
	  GRBModel model(env);

	  int numVariables = 32;

	  GRBVar * var = model.addVars(numVariables);
	  // formula for variable location: 16*state+8*obsAction0+4*unObsAction0
	  // +2*obsAction1+unobsAction1.
      
	  model.update();
      
	  // GRBLinExpr obsWelfare=0.0;
	  GRBLinExpr unobsU0=0.0;

	  // cout << "Adding probability constraints..." << endl;
      
	  // Add probability constraints and define objective
	  for (int s=0; s<2; s++)
	    {
	      GRBLinExpr stateProb = 0.0;
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

			      unobsU0 += var[row]*utilityHat0(au0,au1,s,z);
			  
			    } // au1
			} // au0
		      if (!variableInfo)
			model.addConstr(prob == 0.125); // uniform probabilities
		    } // ao1
		} // ao0
	      model.addConstr(stateProb == 0.5);
	    } // s

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
		  for (int s = 0; s < 2; s++)
		    {
		      for (int aoj = 0; aoj < 2; aoj++)
			{
			  for (int auj = 0; auj < 2; auj++)
			    {
			      int row0 = 16*s+8*ao+4*au+2*aoj+auj;
			      int row1 = 16*s+8*aoj+4*auj+2*ao+au;

			      obsDevGain0 += (utility0(1-ao,aoj,s)-utility0(ao,aoj,s))*var[row0];
			      obsDevGain1 -= (utility0(aoj,1-ao,s)-utility0(aoj,ao,s))*var[row1];
			      unobsDevGain0 += (utilityHat0(1-au,auj,s,z)-utilityHat0(au,auj,s,z))*var[row0];
			      unobsDevGain1 -= (utilityHat0(auj,1-au,s,z)-utilityHat0(auj,au,s,z))*var[row1];
			  
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
      
	  model.setObjective(unobsU0);
	  model.set(GRB_IntAttr_ModelSense,0);
	  model.optimize();

	  // cout << "Done minimizing..." << endl;

	  ofs << z << " " << setprecision(8)
	      << unobsU0.getValue();

	  model.set(GRB_IntAttr_ModelSense,1);
	  model.optimize();

	  // cout << "Done maximizing..." << endl;

	  ofs << " " << setprecision(8) << unobsU0.getValue() << endl;

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

