#include "gurobi_c++.h"
#include <stdio.h>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <vector>

using namespace std;

int colFn(int v, 
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
  if (bid >= reserveBid)
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

void solveAuction(int numBids, int numVals,
		  int reserveBid,
		  double ICSlack, double distrSlack,
		  ofstream & dataFile);

int main()
{
  int numBids = 15;
  int numVals = numBids;

  double ICSlack = 0;
  double distrSlack = 1e-6;

  stringstream ss;
  ss << "rcacommon_justbids_nbids=" << numBids
     << "_nvals=" << numVals
     << ".dat";
  // stringstream ss;
  // ss << "rcacommon_nbids=" << numBids
  //    << "_nvals=" << numVals
  //    << ".dat";
  
  ofstream ofs(ss.str().c_str(),
	       std::ofstream::out);

  for (int reserveBid = 0;
       reserveBid < numBids;
       reserveBid++)
    {
      cout << "On reserve bid " << reserveBid << " of " << numBids << endl;

      solveAuction(numBids,numVals,reserveBid,
		   ICSlack,distrSlack,
		   ofs);
    }

  ofs.close();
}

void solveAuction(int numBids, int numVals,
		  int reserveBid,
		  double ICSlack, double distrSlack,
		  ofstream & dataFile)
{  
  GRBEnv env = GRBEnv();
  env.set(GRB_IntParam_Method,2);
  env.set(GRB_IntParam_Crossover,0);
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

  // // Constrain value distribution to be uniform
  // for (int v = 0; v < numVals; v++)
  //   {
  //     GRBLinExpr LHS = 0;
  //     for (int bids = 0; bids < numBids*numBids*numBids*numBids;
  // 	   bids++)
  // 	{
  // 	  LHS += var[v+bids*numVals];
  // 	} // bids
  //     model.addConstr(LHS>=1.0/numVals-distrSlack);
  //     model.addConstr(LHS<=1.0/numVals+distrSlack);
  //   } // v

  // // Impose symmetric EMW information structure
  // // Fix marginal over bids as independent over [0,1/2]
  // int midBid = numBids/2;
  // for (int b0_m0=0; b0_m0<midBid; b0_m0++)
  //   {
  //     for (int b1_m0=0; b1_m0<midBid; b1_m0++)
  // 	{
  // 	  for (int v=0; v<numVals; v++)
  // 	    {
  // 	      GRBLinExpr LHS = 0;
	  
  // 	      for (int otherBids = 0;
  // 		   otherBids < numBids*numBids;
  // 		   otherBids++)
  // 		{
  // 		  int col = v+b0_m0*numVals+b1_m0*numVals*numBids
  // 		    + otherBids*numVals*numBids*numBids;
  // 		  LHS += var[col];
  // 		} // for otherBids
  // 	      double prob = 0;

  // 	      if (b0_m0==v/2
  // 		  && b1_m0==b0_m0)
  // 		{
  // 		  prob = 1.0/numVals/midBid;
  // 		}
  // 	      else if (b0_m0==v/2
  // 		  || b1_m0==v/2)
  // 		{
  // 		  prob = 0.5/numVals/midBid;
  // 		}

  // 	      model.addConstr(LHS >= prob - distrSlack );
  // 	      model.addConstr(LHS <= prob + distrSlack );
  // 	    } // for v
  // 	} // for b1_m0
  //   } // for b0_m0

  // Now add IC constraints
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
		      for (b_m0[1-player]=0; b_m0[1-player]<numBids; 
			   b_m0[1-player]++)
			{
			  for (b_m1[1-player]=0;
			       b_m1[1-player]<numBids; 
			       b_m1[1-player]++)
			    {
			      int col = colFn(v,b_m0[0],b_m0[1],
					     b_m1[0],b_m1[1],
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

  GRBLinExpr R_m1 = 0;
  GRBLinExpr R_m0 = 0;
  GRBLinExpr totalProb = 0;
  GRBLinExpr U0_m1 = 0;
  GRBLinExpr U1_m1 = 0;

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

		      U0_m1 += var[col]*payoffFPA(v,b_m1[0],b_m1[1],
						  reserveBid,numVals,numBids);
		      U1_m1 += var[col]*payoffFPA(v,b_m1[1],b_m1[0],
						  reserveBid,numVals,numBids);

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
  cout << "U0_m1: " << U0_m1.getValue() << endl;
  cout << "U1_m1: " << U1_m1.getValue() << endl;

  dataFile << static_cast<double>(reserveBid)/(numBids-1.0)
	   << " " << R_m1.getValue();

  model.set(GRB_IntAttr_ModelSense,-1);
  model.optimize();

  cout << "Revenue_m1: " << R_m1.getValue() << endl;
  cout << "U0_m1: " << U0_m1.getValue() << endl;
  cout << "U1_m1: " << U1_m1.getValue() << endl;

  dataFile << " " << R_m1.getValue()
	   << endl;

}

