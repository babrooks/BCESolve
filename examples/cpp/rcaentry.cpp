#include "gurobi_c++.h"
#include <stdio.h>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <vector>

using namespace std;

int colFn(int v0, int e0, int e1,int numVals)
{
  return (state);
}

double payoffEntry(int theta, int e0, int e1, int numTheta)
{
  double value = 2.0*theta/
  if (e0==1)
    {
      if (e1==1)
	{
	  
	} // if e1==1
      else
	{

	}
    } // if e0 == 1
  return 0.0;
}

int main()
{
  int numBids = 15;
  int numVals = numBids;

  int reserveBid = 0;

  double ICSlack = 1e-6;
  double distrSlack = 1e-6;
  
  GRBEnv env = GRBEnv();
  env.set(GRB_IntParam_Method,2);
  GRBModel model(env);
  GRBVar *var;

  int numVariables = numVals*numBids*numBids*numBids*numBids;

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

  double probSum = 0;
  // Fix marginal over bids as independent over [0,1/2]
  int midBid = numBids/2;
  for (int b0_m0=0; b0_m0<midBid; b0_m0++)
    {
      for (int b1_m0=0; b1_m0<midBid; b1_m0++)
	{
	  GRBLinExpr LHS = 0;
	  
	  for (int v=0; v<numVals; v++)
	    {
	      for (int otherBids = 0;
		   otherBids < numBids*numBids;
		   otherBids++)
		{
		  int col = v+b0_m0*numVals+b1_m0*numVals*numBids
		    + otherBids*numVals*numBids*numBids;
		  LHS += var[col];
		} // for otherBids
	    } // for v
	  double prob = 1.0/(midBid*midBid);
	  model.addConstr(LHS >= prob - distrSlack );
	  model.addConstr(LHS <= prob + distrSlack );

	  probSum += prob;
	} // for b1_m0
    } // for b0_m0
  cout << "probSum is " << probSum << endl;

  // Now add IC constraints
  // Add IC for all of player 0's recommendations
  vector<int> b_m0 (2,0);
  vector<int> b_m1 (2,0);

  for (int player=0; player < 2; player++)
    {
      for (b_m0[player]=0; b_m0[player]<numBids; b_m0[player]++)
	{
	  for (b_m1[player]=0; b_m1[player]<numBids; b_m1[player]++)
	    {

	      for (int dev = 0; dev < numBids; dev++)
		{
		  if (dev==b_m0[player])
		    continue;
		  
		  GRBLinExpr LHS_m0 = 0;
		  GRBLinExpr LHS_m1 = 0;
		  for (int v = 0; v < numVals; v++)
		    {
		      for (b_m0[1-player]=0; b_m0[1-player]<numBids; b_m0[1-player]++)
			{
			  for (b_m1[1-player]=0;
			       b_m1[1-player]<numBids; b_m1[1-player]++)
			    {
			      int col = colFn(v,b_m0[player],b_m0[1-player],
					     b_m1[player],b_m1[1-player],
					     numVals,numBids);
			      LHS_m0 += var[col]
				* (payoffFPA(v,b_m0[player],b_m0[1-player],0,
					     numVals,numBids)
				   -payoffFPA(v,dev,b_m0[1-player],0,
					      numVals,numBids));
			
			      LHS_m1 += var[col]
				* (payoffFPA(v,b_m1[player],b_m1[1-player],
					     reserveBid,
					     numVals,numBids)
				   -payoffFPA(v,dev,b_m1[1-player],
					      reserveBid,
					      numVals,numBids));
			
			    } // b_m1[1-player]
			} // b_m0[1-player]
		    } // for v

		  model.addConstr(LHS_m0 >= -ICSlack);
		  model.addConstr(LHS_m1 >= -ICSlack);
		} // devs[player]
	  
	    } // for b_m1[player]
	} // for b_m0[player]
    } // player

  // Constrain value distribution to be uniform
  for (int v = 0; v < numVals; v++)
    {
      GRBLinExpr LHS = 0;
      for (int bids = 0; bids < numBids*numBids*numBids*numBids;
	   bids++)
	{
	  LHS += var[v+bids*numVals];
	} // bids
      model.addConstr(LHS>=1.0/numVals-distrSlack);
      model.addConstr(LHS<=1.0/numVals+distrSlack);
    } // v

  GRBLinExpr R_m1 = 0;
  GRBLinExpr R_m0 = 0;
  GRBLinExpr totalProb = 0;

  // Set up objectives
  for (b_m0[0]=0; b_m0[0]<numBids; b_m0[0]++)
    {
      for (b_m1[0]=0; b_m1[0]<numBids; b_m1[0]++)
	{
	  for (int v = 0; v < numVals; v++)
	    {
	      for (b_m0[1]=0; b_m0[1]<numBids; b_m0[1]++)
		{
		  for (b_m1[1]=0; b_m1[1]<numBids; b_m1[1]++)
		    {
		      int col = colFn(v,b_m0[0],b_m0[1],b_m1[0],b_m1[1],
				      numVals,numBids);

		      int maxBid = max(b_m1[0],b_m1[1]);
		      if (maxBid>=reserveBid)
			R_m1 += var[col]*static_cast<double>(maxBid)/(numBids-1.0);

		      maxBid = max(b_m0[0],b_m0[1]);
		      R_m0 += var[col]*static_cast<double>(maxBid)/(numBids-1.0);
		      totalProb += var[col];
		    } // b_m1[1]
		} // b_m0[1]
	    } // for v
	} // b_m1[0]
    } // b_m0[0]
  model.addConstr(totalProb <= 1.0);
  

  model.setObjective(R_m1);
  model.set(GRB_IntAttr_ModelSense,+1);
  model.optimize();

  cout << "Total probability: " << totalProb.getValue() << endl;
  cout << "Revenue_m0: " << R_m0.getValue() << endl;
  cout << "Revenue_m1: " << R_m1.getValue() << endl;
  return 0;
}

