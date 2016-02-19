// Calculates the new relaxed program in the discrete case
// BAB 6-24-2014

#include "gurobi_c++.h"
#include <stdio.h>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <cmath>

using namespace std;

double prior (int v, int numValues)
{
  if (v>=0 && v<numValues)
    return 1.0/(numValues-1.0);
  return 0.0;
}

double rpcdf( double r)
{
  double ub = 0.128;
  double lb = 0.123;

  if (r<lb)
    return 0;
  if (r<ub)
    return pow( (r-lb)/(ub-lb) , 2 );
  return 1.0;

  if (r<0)
    return 0;
  if (r<ub)
    return sqrt(r/ub);
  return 1.0;
}

int main () 
{
  int v, vp, b, row, col;

  int numBids = 150; 
  int numValues = numBids;
  double highBid = 0.5;

  vector<double> valueGrid(numValues,0);
  vector<double> bidGrid(numBids,0);
  
  for (v=0; v<numValues; v++)
    valueGrid[v] = (1.0*v)/(numValues-1.0);

  for (b=0; b<numBids; b++)
    bidGrid[b] = (highBid*b)/(numBids-1.0);

  try
    {
      GRBEnv env = GRBEnv();
      env.set(GRB_IntParam_Method,2);
      GRBModel model(env);
      GRBVar *var;

      int numVariables = numValues*numBids;

      var = model.addVars(numVariables);
      model.update();

      // Add probability constraints and define expected bid
      GRBLinExpr revenue = 0.0;
      for (v=0; v<numValues; v++)
	{
	  GRBLinExpr lhs = 0;
	  for (b=0; b<numBids; b++)
	    {
	      col = v * numBids + b;
		  
	      revenue += bidGrid[b]*rpcdf(bidGrid[b])
		*prior(v,numValues)*var[col];
	      lhs += var[col];
	    }

	  model.addConstr(lhs==1.0); // probabilities are conditional
				     // on the value
	} // v

      // Add aggregated constraints. One for each (v,b)
      for (b=0; b<numBids; b++)
	{
	  double upBid = bidGrid[b];
	  GRBLinExpr lhs = 0;
	  for (v=0; v<numValues; v++)
	    {
	      double val = valueGrid[v];
	      
	      for (int x = 0; x <= b; x++)
		{
		  double rec = bidGrid[x];
		  col = v * numBids + x;
		  
		  lhs += ( ( val*( rpcdf(upBid)-0.5*rpcdf(rec) )
			     - ( upBid*rpcdf(upBid)-rec*0.5*rpcdf(rec) ) )
			   * var[col]*prior(v,numValues) );
		} // x
	      lhs -= 0.25*( ( val-upBid)* rpcdf(upBid)
			    * var[col]*prior(v,numValues) );
	    } // v

	  model.addConstr(lhs<=0);
	} // b
      
      model.setObjective(revenue);
      model.set(GRB_IntAttr_ModelSense,1);
      model.optimize();

      // stringstream tmpstream;
      // col = 0;
      // for (v=0; v<numValues; v++)
      // 	{
      // 	  tmpstream.str(""); 
      // 	  tmpstream << setprecision(2) << "v=" << valueGrid[v] << ": " ;
      // 	  cout << setw(10) << left << tmpstream.str();
      // 	  for (b=0; b<numBids; b++)
      // 	    {
      // 	      if (var[col].get(GRB_DoubleAttr_X)==0.0
      // 		  || var[col].get(GRB_DoubleAttr_X)==1.0)
      // 		cout << setw(4) << setprecision(0) 
      // 		     << right << noshowpoint
      // 		     << var[col].get(GRB_DoubleAttr_X) << " ";
      // 	      else
      // 		cout << setw(4) << fixed << setprecision(2)
      // 		     << var[col].get(GRB_DoubleAttr_X) << " ";
      // 	      col++;
      // 	    } // b
      // 	  cout << endl;
      // 	} // v

      cout << endl << "Revenue = " << setprecision(8) << revenue.getValue() << endl;


    }
  catch (GRBException &e)
    {
      cout << e.getMessage() << endl;
    }

  return 0;
}

