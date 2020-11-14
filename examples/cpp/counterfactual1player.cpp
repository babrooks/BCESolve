// A 1 player counterfactual example
#include "gurobi_c++.h"
#include <stdio.h>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <cmath>

using namespace std;

int main () 
{
  std::vector<double> Alphas = {0.25, 0.375, 0.5};
  for (auto alpha : Alphas) // Probability that action matches, between 0.25 and 0.5
    {
      stringstream ss;
      ss << "counterfactual1player_" << alpha << ".log";
      ofstream ofs(ss.str().c_str());
  
      for (double z = 0.0; z <= 2.005; z+=0.01)
	{
	  try
	    {
	      GRBEnv env = GRBEnv();
	      env.set(GRB_IntParam_Method,2);
	      env.set(GRB_IntParam_OutputFlag,0);
	      GRBModel model(env);

	      int numVariables = 8;

	      GRBVar * var = model.addVars(numVariables);
	      // formula for variable location: 4*state+2*obsAction+unobsAction
      
	      model.update();
      
	      GRBLinExpr obsWelfare=0.0;
	      GRBLinExpr unobsWelfare=0.0;

	      // Add probability constraints and define objective
	      for (int s=0; s<2; s++)
		{
		  for (int ao = 0; ao < 2; ao++)
		    {
		      GRBLinExpr prob = 0;
		      for (int au = 0; au < 2; au++)
			{
			  int row = 4*s+2*ao+au;

			  prob += var[row];

			  if (au==s && s==1)
			    unobsWelfare += var[row]*z;
			  if (au==s && s==0)
			    unobsWelfare += var[row]*(2-z);
			  if (ao==s)
			    obsWelfare += var[row];
			} // au
		      if (ao==s)
			model.addConstr(prob == alpha);
		      else
			model.addConstr(prob == 0.5-alpha);
		    } // ao
		} // s

	      // Obedience constraints
	      for (int ao = 0; ao < 2; ao++)
		{
		  for (int au = 0; au < 2; au++)
		    {
		      // Deviation in observed action
		      GRBLinExpr obsDevGain = 0.0;
		      for (int s = 0; s < 2; s++)
			{
			  int row = 4*s+2*ao+au;

			  if (ao == s)
			    obsDevGain -= var[row];
			  else
			    obsDevGain += var[row];
			} // s
		      model.addConstr(obsDevGain <= 0);
	      
		      // Deviation in unobserved action
		      GRBLinExpr unobsDevGain = 0.0;
		      for (int s = 0; s < 2; s++)
			{
			  int row = 4*s+2*ao+au;
		      
			  if (au == s && s == 1)
			    unobsDevGain -= var[row]*z;
			  if (au == s && s == 0)
			    unobsDevGain -= var[row]*(2-z);
			  else if (au != s && s==1)
			    unobsDevGain += var[row]*z;
			  else if (au != s && s==0)
			    unobsDevGain += var[row]*(2-z);
			} // s
		      model.addConstr(unobsDevGain <= 0);
	      
		    } // au
		} // ao
      
	      model.setObjective(unobsWelfare);
	      model.set(GRB_IntAttr_ModelSense,0);
	      model.optimize();

	      // cout << endl << "Observed welfare = " << setprecision(8) << obsWelfare.getValue() << endl;
	      // cout << endl << "Unobserved welfare = " << setprecision(8) << unobsWelfare.getValue() << endl;

	      ofs << z << " " << setprecision(8)
		  << obsWelfare.getValue() << " " << setprecision(8)
		  << unobsWelfare.getValue();

	      model.set(GRB_IntAttr_ModelSense,1);
	      model.optimize();
	      ofs << " " << setprecision(8) << unobsWelfare.getValue() << endl;

	    }
	  catch (GRBException &e)
	    {
	      cout << e.getMessage() << endl;
	    }
	} // for z

      ofs.close();
    }
  
  return 0;
}

