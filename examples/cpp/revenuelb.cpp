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

void revenuelb(int numVals, int numBids);

int main ()
{
  int numVals = 50;
  int numBids = numVals;
  numBids = 150;

  revenuelb(numVals,numBids);

  return 0;

  for (numVals = 2; numVals < 11; numVals += 1)
    revenuelb(numVals,numVals);

  return 0;
}

void revenuelb(int numVals, int numBids)
{
  int numPlayers = 2;

  int bid, val, player, dev,
    row, col;
  vector<int> bids(2,0), vals(2,0), deviations(2,0);

  double v, b, bp, F;

  double highBid = 0.3;

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

      int numVariables = numVals * numBids;

      GRBLinExpr bidderSurplus = 0.0;

      var = model.addVars(numVariables);
      model.update();


      for (val = 0; val < numVals; val ++)
	{
	  GRBLinExpr probSum = 0;

	  v = ((double) val)/(numVals-1);
	  F = ((double) val+0.5)/numVals;

	  for (bid = 0; bid < numBids; bid ++)
	    {
	      b = highBid*static_cast<double>(bid)/(numBids-1);
	      if (b>v)
		break;

	      col = val * numBids + bid;

	      bidderSurplus += var[col] * F * (v-b) / numVals;
	      probSum += var[col];

	      // model.addConstr(var[col]>=0);
	    } // val

	  model.addConstr(probSum == 1.0);
	} // bid

      for (dev = 0; dev < numBids; dev++)
	{
	  GRBLinExpr lhs = 0;
	  bp = highBid*static_cast<double>(dev)/(numBids-1);
	  F = (static_cast<double>(dev)+0.5)/(numBids);

	  lhs += (F-F*F);

	  for (val = 0; val < numVals; val ++)
	    {
	      v = static_cast<double>(val)/(numVals-1);
	      if (v <= bp)
		continue;

	      // Prob of winning is val/numVals+0.5/numVals, since
	      // lose to other player with same valuation w/ prob 0.5.
	      F = (static_cast<double>(val)+0.5)/numVals;

	      for (bid = 0; bid < dev; bid ++)
		{
		  b = highBid*static_cast<double>(bid)/numBids;

		  col = val * numBids + bid;

		  assert(col<numVariables);
		  
		  lhs += var[col]*F/numVals*((bp-b)/(v-bp)-1);
		} // val
	    } // bid

	  model.addConstr(lhs >= 0);
	} // dev

      model.setObjective(bidderSurplus);

      // -1 corresponds to maximize
      model.set(GRB_IntAttr_ModelSense,1);
      model.optimize();

      // cout << "numVals = " << numVals
      // 	   << ", numBids = " << numBids
      // 	   << ", bidder surplus = " << bidderSurplus.getValue() << endl;
      
      // cout << numVals << " " << bidderSurplus.getValue() << endl;
      
      GRBConstr * con = model.getConstrs();

      cout << endl;

      
      for (val = 0; val < numVals; val++)
      	fprintf(stdout,"val: %d %1.6f\n",val,con[val].get(GRB_DoubleAttr_Pi));

      for (dev = 0; dev < numBids; dev++)
      	{
      	  fprintf(stdout,"%d %1.4f %1.4f\n",dev,con[numVals+dev].get(GRB_DoubleAttr_Slack),
      		  con[numVals+dev].get(GRB_DoubleAttr_Pi));
      	}

      int bidStartIncr = min(numBids,20),
      	bidStartCnt = 0,
      	bidStart = 0;
      for (bidStartCnt = 0; bidStartCnt<min(10,numBids/bidStartIncr); bidStartCnt++)
      	{
	  
      	  cout << endl;
      	  fprintf(stdout,"  ");
      	  for (bid = bidStart; bid < bidStart+bidStartIncr; bid ++)
      	    fprintf(stdout,"%6d ",bid);
      	  cout << endl;
	  
      	  for (val = 0; val < numVals; val ++)
      	    {
      	      fprintf(stdout,"%3d ",val);
	  
      	      for (bid = bidStart; bid < bidStart+bidStartIncr; bid ++)
      		{
      		  col = val * numBids + bid;

      		  if (var[col].get(GRB_DoubleAttr_X)>1e-9)
      		    fprintf(stdout,"%1.4f ",var[col].get(GRB_DoubleAttr_X));
      		  else
      		    fprintf(stdout,"   %1.1f ",0);
      		} // val
      	      cout << endl;
      	    } // bid
      	  bidStart+=bidStartIncr;
      	}
      cout << endl;
      cout << setprecision(16) << bidderSurplus.getValue() << ";..." << endl;

      delete[] var;
      delete[] con;
    } // try
  catch (GRBException &e)
    {
      cout << e.getMessage() << endl;
    }
}
