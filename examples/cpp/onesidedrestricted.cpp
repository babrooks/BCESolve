// Calculates the lower bound on revenue and upper bound on bidders'
// surplus over all BCE of the first price auction.
// 
// BAB 5-30-2013

#include "gurobi_c++.h"
#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <vector>

using namespace std;

void restrictedModel(int numVals, int numBids);

int main ()
{
  int numVals = 50;
  int numBids = numVals;
  double vstar = 0.5;

  restrictedModel(numVals,numBids);

  return 0;

}



void restrictedModel(int numVals, int numBids)
{
  int numPlayers = 2;

  int bid, bid1, bid2, val, val1, val2, player, dev,
    row, col;
  vector<int> bids(2,0), vals(2,0), deviations(2,0);

  double v, b, bp, F;
  double vstar = 0.5;

  double highBid = 0.5;

  try
    {
      GRBEnv env = GRBEnv();
      
      env.set(GRB_IntParam_Method,2); // barrier
      env.set(GRB_IntParam_Crossover,0); // disable crossover
      env.set(GRB_DoubleParam_BarConvTol,1e-12);
      env.set(GRB_DoubleParam_OptimalityTol,1e-9);
      env.set(GRB_DoubleParam_MarkowitzTol,0.999);
      env.set(GRB_DoubleParam_FeasibilityTol,1e-9);

      // env.set(GRB_IntParam_Method,1); // dual
      // env.set(GRB_IntParam_Method,0); // primal
      // model.getEnv().set(GRB_IntParam_OutputFlag,0);
      

      GRBModel model(env);
      GRBVar * var;

      int numVariables = numVals * numBids * numBids;

      GRBLinExpr bidderSurplus = 0.0;

      var = model.addVars(numVariables);
      model.update();

      for (val1 = 0; val1 < numVals; val1 ++)
	{
	  GRBLinExpr probSum = 0;

	  v = 0.5 + 0.5 * ((double) val1)/(numVals-1);

	  for (bid1 = 0; bid1 < numBids; bid1 ++)
	    {
	      for (bid2 = 0; bid2 < numBids; bid2++)
		{
		  b = highBid*static_cast<double>(bid1)/(numBids-1);

		  col = val1 * numBids * numBids + bid1 * numBids + bid2;

		  if (bid1 < bid2)
		    {
		      model.addConstr(var[col]==0);
		      continue;
		    }

		  bidderSurplus += var[col] * (v-b);
		  probSum += var[col];

		} // bid2
	    } // bid1

	  model.addConstr(probSum == 0.5/numVals);
	} // val

      // Bidder 2's deviations
      for (bid2 = 0; bid2 < numBids; bid2++)
	{
	  b = highBid * ((double) bid2)/(numBids-1);
	  
	  for (dev = bid2+1; dev < numBids; dev++)
	    {

	      GRBLinExpr lhs = 0;

	      bp = highBid*((double) dev)/(numBids-1);
	      
	      lhs += (bp - b)*0.5/numVals;

	      for (val = 0; val < numVals; val ++)
		{
		  for (bid1 = 0; bid1 < dev; bid1++)
		    {
		      col = val*numBids * numBids + bid1 * numBids + bid2;

		      lhs -= var[col] * (vstar - bp);
		    } // bid1
		} // val

	      model.addConstr(lhs >= 0);
	    } // dev
	} // bid2

      // Bidder 1's deviations
      for (val = 0; val < numVals; val ++)
	{

	  v = 0.5 + 0.5 * ((double) val)/(numVals-1);
	  for (bid1 = 0; bid1 < numBids; bid1++)
	    {
	      b = highBid * ((double) bid1) / (numBids-1);
	      for (dev = 0; dev < bid1; dev++)
		{
		  bp = highBid * ((double) dev) / (numBids-1);

		  GRBLinExpr lhs = 0;

		  for (bid2 = 0; bid2 <= bid1; bid2++)
		    {
		      col = val*numBids * numBids + bid1 * numBids + bid2;
		      
		      if (bid2 <= dev)
			lhs += (bp-b)*var[col];
		      else
			lhs += (v-b)*var[col];
		      

		    } // bid2
		  model.addConstr(lhs>=0);
		} // dev
	    } // bid1
	} // val

      model.setObjective(bidderSurplus);

      // -1 corresponds to maximize
      model.set(GRB_IntAttr_ModelSense,-1);
      model.optimize();

      cout << endl << bidderSurplus.getValue() << endl << endl;

      // for (bid2 = 0; bid2 < numBids; bid2++)
      // 	{
      // 	  for (val = 0; val < numVals; val++)
      // 	    {
      // 	      for (bid1 = 0; bid1 < numBids; bid1++)
      // 		{
      // 		  col = val * numBids * numBids + bid1 * numBids + bid2;

      // 		  if (var[col].get(GRB_DoubleAttr_X)>1e-10)
      // 		    fprintf(stdout,"%1.4f ", var[col].get(GRB_DoubleAttr_X));
      // 		  else
      // 		    fprintf(stdout,"%1.2f   ",0);
      // 		} // bid1
      // 	      cout << endl;
      // 	    } // val
      // 	  cout << endl;
      // 	} // bid2 

      delete[] var;
    } // try
  catch (GRBException &e)
    {
      cout << e.getMessage() << endl;
    }
}
