#include "gurobi_c++.h"
#include <stdio.h>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <vector>

using namespace std;

int hash(int v, 
	 int b0_m0, int b1_m0,
	 int b0_m1, int b1_m1,
	 int numVals, int numBids)
{
  return (v
	  +b0_m0*numVals
	  +b1_m0*numVals*numBids
	  +b0_m1*numVals*numBids*numBids
	  +b1_m1*numVals*numBids*numBids*numBids);
}

double payoffFPA(int v,int bid, int otherBid,
		 int reserveBid,
		 int numVals, int numBids)
{
  if (bid > reserveBid)
    {
      double surplus = (static_cast<double>(v)/(numVals-1.0)
			- static_cast<double>(bid)/(numBids-1.0));
      if (bid>otherBid)
	return surplus;
      else if (bid == otherBid)
	return surplus/2.0;
    }
  return 0.0;
}

int main()
{
  int numBids = 20;
  int numVals = numBids;

  double ICSlack = 1e-6;
  double distrSlack = 1e-6;
  
  GRBEnv env = GRBEnv();
  env.set(GRB_IntParam_Method,2);
  GRBModel model(env);
  GRBVar *var;

  int numVariables = numValues*numBids*numBids*numBids*numBids;

  var = model.addVars(numVariables);
  model.update();

  for (int k = 0; k < numVariables; k++)
    {
      var[k].set(GRB_DoubleAttr_LB,0.0);
      // var[k].set(GRB_DoubleAttr_UB,1.0);
    }

  // Add probability constraints and define expected bid
  GRBLinExpr expWinBid0 = 0.0;
  GRBLinExpr expBidderSurplus0 = 0.0;

  // Fix marginal over bids as independent over [0,1/2]
  midBid = numBids/2;
  for (int b0_m0=0; b0_m0<midBid; b0_m0++)
    {
      for (int b1_m0=0; b1_m0<midBid; b1_m0++)
	{
	  GRBLinExpr LHS = 0;
	  
	  for (int v=0; v<numValues; v++)
	    {
	      for (int otherBids = 0;
		   otherBids < numBids*numBids;
		   otherBids++)
		{
		  col = v+b0_m0*numVals+b1_m0*numVals*numBids
		    + otherBids*numVals*numBids*numBids;
		  LHS += var[col];
		} // for otherBids
	    } // for v
	  model.addConstr(LHS >= 1.0/(midBid*midBid) - distrSlack );
	  model.addConstr(LHS <= 1.0/(midBid*midBid) + distrSlack );
	} // for b1_m0
    } // for b0_m0

  // Now add IC constraints
  // Add IC for all of player 0's recommendations
  for (int b0_m0=0; b0_m0<numBids; b0_m0++)
    {
      for (int b0_m1=0; b0_m1<numBids; b0_m1++)
	{
	  GRBLinExpr eqPayoff_m0;
	  GRBLinExpr eqPayoff_m1;
	  
	  for (int v = 0; v < numVals; v++)
	    {
	      for (int b1_m0=0; b1_m0<numBids; b1_m0++)
		{
		  for (int b1_m1=0; b1_m1<numBids; b1_m1++)
		    {
		      
		      

		    } // b1_m1
		} // b1_m0
	    } // for v


	  for (int dev0 = 0; dev0 < numBids; dev0++)
	    {
	      if (dev0==b0_m0)
		continue;
	      
	    } // dev0
	  
	  for (int dev1 = 0; dev1 < numBids; dev1++)
	    {
	      if (dev1==b0_m1)
		continue;
	      
	    } // dev1
	  
	} // for b0_m1
    } // for b0_m0
	  GRBLinExpr LHS = 0;
	  
	  for (int v=0; v<numValues; v++)
	    {
	      for (int otherBids = 0;
		   otherBids < numBids*numBids;
		   otherBids++)
		{
		  col = v+b0_m0*numVals+b1_m0*numVals*numBids
		    + otherBids*numVals*numBids*numBids;
		  LHS += var[col];
		} // for otherBids
	    } // for v
	  model.addConstr(LHS >= 1.0/(midBid*midBid) - distrSlack );
	  model.addConstr(LHS <= 1.0/(midBid*midBid) + distrSlack );


	  col = v * numValues*numBids
	    + vp * numBids + b;

	  if (v==vp)
	    var[col].set(GRB_DoubleAttr_UB,0.5);
	  else
	    var[col].set(GRB_DoubleAttr_UB,1.0);
		    
		  
	  if (b>0)
	    {
	      // expWinBid += bidGrid[b]*prior(v,numValues)
	      //   *prior(vp,numValues)*(var[col]-var[col-1]);
	      expWinBid += (bidGrid[b]-bidGrid[b-1])
		*prior(v,vp,numValues)*var[col];

	      // Monotonicity constraints
	      model.addConstr(var[col]>=var[col-1]);
	    }

	  // Efficiency
	  if (v<vp)
	    model.addConstr(var[v*numValues*numBids + vp*numBids + b]==0);
	} // b
	      
      expSurplus += valueGrid[v]*prior(v,vp,numValues)
	*var[v*numValues*numBids + vp*numBids + numBids-1];
    } // v

      // // Adding up constraints
      // for (v=0; v<numValues; v++)
      // 	{
      // 	  for (vp=0; vp<numValues; vp++)
      // 	    {
      // 	      for (b=0; b<numBids; b++)
      // 		{
      // 		  model.addConstr(var[v*numValues*numBids + vp*numBids + b]
      // 				  + var[vp*numValues*numBids + v*numBids + b]
      // 				  <= 1.0 );
      // 		}
      // 	    } // vp
      // 	} // v
      
      // // Add aggregated constraints. One for each (v,b)
      // for (v=0; v<numValues; v++)
      // 	{
      // 	  GRBLinExpr sum = 0;
      // 	  for (b=0; b<numBids-1; b++)
      // 	    {
      // 	      GRBLinExpr increment = 0;
      // 	      GRBLinExpr RHS = 0;
      // 	      for (vp=0; vp<numValues; vp++)

  return 0;
}

