// Calculates the new relaxed program in the discrete case
// BAB 6-24-2014

#include "gurobi_c++.h"
#include <stdio.h>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <vector>

using namespace std;

double prior (int v1, int v2,int numValues)
{
  // if (val==0)
  //   return 1.0/8.0;
  // else if (val==1)
  //   return 6.0/8.0;
  // else
  //   return 1.0/8.0;

  // if (val==0)
  //   return 0.1;
  // else if (val==1)
  //   return 0.7;
  // else if (val==2)
  //   return 0.1;
  // else if (val==3)
  //   return 0.1;

  // return 1.0/numValues;
 
  // Assume v1>=v2;
  if (numValues!=3)
    return 1.0/numValues/numValues;

  if (v1 == 2 && v2 == 2)
    return 0.4;
  if (v1 == 2 && v2 == 1)
    return 0.1;
  if (v1 == 2 && v2 == 0)
    return 0.05;
  if (v1 == 1 && v2 == 1)
    return 0.1;
  if (v1 == 1 && v2 == 0)
    return 0.1;
  if (v2 < v1)
    return prior(v2,v1,numValues);

  return 0;
}

int main () 
{
  int v, vp, b, row, col;

  int numBids = 150; 
  int numValues = 3;
  double highBid = 0.6;

  vector<double> valueGrid(numValues,0);
  vector<double> bidGrid(numBids,0);
  
  for (v=0; v<numValues; v++)
    valueGrid[v] = (1.0*v)/(numValues-1.0);

  // valueGrid[2] = 0.35;

  for (b=0; b<numBids; b++)
    bidGrid[b] = (highBid*b)/(numBids-1.0);

  try
    {
      GRBEnv env = GRBEnv();
      env.set(GRB_IntParam_Method,2);
      GRBModel model(env);
      GRBVar *var;

      int numVariables = numValues*numValues*numBids;

      var = model.addVars(numVariables);
      model.update();

      for (int k = 0; k < numVariables; k++)
      	{
      	  var[k].set(GRB_DoubleAttr_LB,0.0);
      	  // var[k].set(GRB_DoubleAttr_UB,1.0);
      	}

      // Add probability constraints and define expected bid
      GRBLinExpr expWinBid = 0.0;
      GRBLinExpr expSurplus = 0.0;
      GRBLinExpr expBidderSurplus = 0.0;
      for (v=0; v<numValues; v++)
	{
	  for (vp=0; vp<numValues; vp++)
	    {
	      for (b=0; b<numBids; b++)
		{
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

	    } // vp
	} // v

      // Adding up constraints
      for (v=0; v<numValues; v++)
	{
	  for (vp=0; vp<numValues; vp++)
	    {
	      for (b=0; b<numBids; b++)
		{
		  model.addConstr(var[v*numValues*numBids + vp*numBids + b]
				  + var[vp*numValues*numBids + v*numBids + b]
				  <= 1.0 );
		}
	    } // vp
	} // v
      
      // Add aggregated constraints. One for each (v,b)
      for (v=0; v<numValues; v++)
      	{
      	  GRBLinExpr sum = 0;
      	  for (b=0; b<numBids-1; b++)
      	    {
      	      GRBLinExpr increment = 0;
      	      GRBLinExpr RHS = 0;
      	      for (vp=0; vp<numValues; vp++)
      		{
      		  RHS  += prior(vp,v,numValues) 
      		    * var[vp * numValues*numBids + v * numBids + b];
      		  increment += prior(v,vp,numValues) 
      		    * var[v * numValues*numBids + vp * numBids + b];
      		} // vp
      	      sum += increment*(bidGrid[b+1]-bidGrid[b]);
      	      model.addConstr(sum >= (valueGrid[v]-bidGrid[b]) * RHS);
      	    } // b
      	} // v
      
      expBidderSurplus = 2*(expSurplus - expWinBid);

      // model.setObjective(expBidderSurplus);
      // model.set(GRB_IntAttr_ModelSense,-1);
      model.setObjective(expWinBid);
      model.set(GRB_IntAttr_ModelSense,-1);
      model.optimize();

      cout << "Total surplus = " << expSurplus.getValue() << endl
	   << "Revenue = " << expWinBid.getValue() << endl
	   << "Bidder surplus = " << expBidderSurplus.getValue() << endl;

      cout << "valueGrid: (";
      for (v=0; v<numValues; v++)
	cout << valueGrid[v] << ",";
      cout << ")" << endl;
      cout << setw(20) << setprecision(2) << "bidGrid:          ";
      for (b=0; b<numBids; b++)
	cout << setw(4) << fixed << setprecision(2) << bidGrid[b] << " ";
      cout << endl << setprecision(2); 

      stringstream tmpstream;
      col = 0;
      for (v=0; v<numValues; v++)
	{
	  for (vp=0; vp<numValues; vp++)
	    {
	      tmpstream.str(""); 
	      tmpstream << setprecision(2) << "(v,vp)=(" << valueGrid[v] << "," << valueGrid[vp] << "): " ;
	      cout << setw(20) << left << tmpstream.str();
	      for (b=0; b<numBids; b++)
		{
		  if (var[col].get(GRB_DoubleAttr_X)==0.0 || var[col].get(GRB_DoubleAttr_X)==1.0)
		    cout << setw(4) << setprecision(0) << right << noshowpoint << var[col].get(GRB_DoubleAttr_X) << " ";
		  else
		    cout << setw(4) << fixed << setprecision(2) << var[col].get(GRB_DoubleAttr_X) << " ";
		  col++;
		}
	      cout << endl;
	    } // vp
	} // v
    }
  catch (GRBException &e)
    {
      cout << e.getMessage() << endl;
    }

  return 0;
}

