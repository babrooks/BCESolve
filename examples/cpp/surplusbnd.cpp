// Calculates the lower bound on revenue and upper bound on bidders'
// surplus over all BCE of the first price auction.
// 
// BAB 5-30-2013

#include "gurobi_c++.h"
#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <vector>
#include <time.h>
#include <stdlib.h>
#include <cmath>

using namespace std;

double algorithm(int n, double vstar, double p, double vbar, 
		 const vector<double> & P, bool verbose, FILE *fp);

double solveLP(int n, double vstar, double p, double vbar, 
	       const vector<double> &P, bool verbose, FILE *fp);

int main ()
{
  int n=50, b;
  double vstar = 0.5, 
    p = 0.3,
    vbar = 1.0;
  double lpObj, algObj;

  FILE *fp;
  // fp = fopen("surplusbnd.txt","w");
  fp = stdout;

  srand ( time ( NULL ) );

  vector<double> P(n,0);
  // for (b=0; b<n; b++)
  //   P[b] = 1.0/n*p;


  int numIter = 0;
  double maxDiff = 0;
  while (lpObj - algObj < 0.001
	 && numIter < 1)
    {
      double sum = 0;
      for (b=0; b<n; b++)
	{
	  P[b] = pow((rand() % ((int) 1e10)) /(1e10+1),2.0);
	  sum += P[b];
	}
      for (b=0; b<n; b++)
	P[b] *= p/sum;

      lpObj = solveLP(n,vstar,p,vbar,P,false,fp);

      cout << endl;

      algObj = algorithm(n,vstar,p,vbar,P,false,fp);
      
      cout << endl << "Num iterations = " << numIter
	   << ", Difference = " << lpObj - algObj << endl;
      
      if (lpObj - algObj > maxDiff)
	{
	  maxDiff = lpObj - algObj;
	  break;
	}

      numIter++;
    }

  for (b=0; b<n; b++)
    fprintf(fp,"%1.10f\n",P[b]);
  fprintf(fp,"\n");

  lpObj = solveLP(n,vstar,p,vbar,P,true,fp);

  // algObj = algorithm(n,vstar,p,vbar,P,true,fp);
      
  fprintf(fp, "\nMax difference = %1.16f\n",maxDiff);

  return 0;
}

double algorithm(int n, double vstar, double p, double vbar,
		 const vector<double> &P, bool verbose, FILE *fp)
{
  vector< vector<double> > x(n,vector<double>(n,0));
  vector< vector<bool> > bind1(n,vector<bool>(n,false)),
    bind2(n,vector<bool>(n,false));
  
  int numPlayers = 2;

  int bid, val, player, dev,
    row, col, b1, b2, bhat;
  vector<int> bids(2,0), vals(2,0), deviations(2,0);

  double v, b, bp, F;
  double bound, tmpBnd;
  
  vector<double> B(n,0);

  double probSum = 0;

  for (b=0; b<n; b++)
    B[b]=vstar*b/(n-1);

  double bidderSurplus = (1-p)*vbar;
  for (b1 = 0; b1 < n-1; b1 ++)
    {
      bound = (B[b1+1] - B[b1])/(vstar-B[b1+1])*P[b1];
      if (probSum + bound > 1-p)
	{
	  x[b1][b1] = 1-p-probSum;
	  probSum =1-p;
	  break;
	} 

      x[b1][b1] = bound; 
      probSum += bound;

      for (b2 = b1-1; b2 >= 0; b2 --)
	{
	  bound = (B[b1+1]-B[b2])/(vstar-B[b1+1])*P[b2];
	  for (bhat = b2; bhat <= b1-1; bhat ++)
	    bound -= x[bhat][b2];

	  int tightDev = 0;
	  for (dev = b2; dev < b1; dev++)
	    {
	      tmpBnd = 0;
	      for (bhat = b2 + 1; bhat <= dev; bhat++)
		tmpBnd -= x[b1][bhat];
	      for (bhat = dev+1; bhat <= b1; bhat++)
		tmpBnd += (vbar - B[b1])/(B[b1]-B[dev])*x[b1][bhat];

	      if (tmpBnd < bound)
		{
		  bound = tmpBnd;
		  tightDev = dev;
		}
	      
	    } // dev

	  if (tightDev)
	    bind1[b1][tightDev] = true;
	  else
	    bind2[b1][b2] = true;
	  

	  
	  if (bound > 1-p-probSum)
	    {
	      x[b1][b2]=1-p-probSum;
	      probSum = 1-p;
	      break;
	    }
	  else
	    {
	      x[b1][b2] = bound;
	      probSum += bound;
	    }

	  if (!bind2[b1][b2])
	    break;

	} // b2
      
      if (probSum >= 1-p)
	break;
    } // b1

  probSum = 0;
  for (b1 = 0; b1 < n; b1 ++)
    {
      for (b2 = 0; b2 <= b1; b2 ++)
	{
	  bidderSurplus -= B[b1] * x[b1][b2];
	  probSum += x[b1][b2];
	} // b2
    } // b1

  if (verbose)
    {
      fprintf(fp,"Alg Multipliers on bidder 1's constraints\n");
	  
      fprintf(fp," b1 ");
      for (b1=0; b1<n; b1++)
	fprintf(fp,"%2d ",b1);
      fprintf(fp,"\n");
		    
      for (b1 = 0; b1 < n; b1 ++)
	{
	  fprintf(fp,"%3d ",b1);
	  for (dev = 0; dev < b1; dev++)
	    {
	      if (bind1[b1][dev])
		{
		  // fprintf(fp,"%1.3f ",con[row].get(GRB_DoubleAttr_Pi));
		  fprintf(fp,"XX ");
		}
	      else
		{
		  //fprintf(fp,"%1.1f   ",0.0);
		  fprintf(fp,"0  ");
		}

	      row++;
	    } // dev
	  for (dev = b1; dev < n; dev ++)
	    {
	      //fprintf(fp,"%1.1f   ",0.0);
	      fprintf(fp,".  ");
	    }
	  
	  fprintf(fp,"\n");
	} // b1

      fprintf(fp,"\n");

      fprintf(fp,"Alg Multipliers on bidder 2's constraints\n");
      // Bidder 2
      fprintf(fp," b1 ");
      for (b1=0; b1<n; b1++)
	fprintf(fp,"%2d ",b1);
      fprintf(fp,"\n");

      for (b2 = 0; b2 < n; b2 ++)
	{
	  fprintf(fp,"%3d ",b2);
	  for (dev = 0; dev < b2+1; dev ++)
	    {
	      // fprintf(fp,"%1.1f   ",0.0);
	      fprintf(fp,".  ");
	    }

	  for (dev = b2+1; dev < n; dev++)
	    {
	      if (bind2[dev][b2])
		{
		  // fprintf(fp,"%1.3f ",con[row].get(GRB_DoubleAttr_Pi));
		  fprintf(fp,"XX ");
		}
	      else
		{
		  // fprintf(fp,"%1.1f   ",0.0);
		  fprintf(fp,"0  ");
		}

	      row++;
	    } // dev
	  
	  fprintf(fp,"\n");
	} // b2


      fprintf(fp, "Algorithm output\n ");
      for (b1 = 0; b1 < n; b1 ++)
	{
	  double sum = 0;
	  for (b2 = 0; b2 <= b1; b2 ++)
	    {
	      if (x[b1][b2]>1e-9)
	        {
	      fprintf(fp,"%1.3f ",x[b1][b2]);
	      sum += x[b1][b2];
	        }
	      else
	        fprintf(fp,"%1.1f   ",0.0);
	    }		
	  
	  for (b2 = b1+1; b2 < n; b2 ++)
	    fprintf(fp,"%1.1f   ",0.0);
	  
	  fprintf(fp,"%1.4f \n",sum);
	} // b1
    } 
  fprintf(fp,"Objective = %1.16f\n",bidderSurplus);
  fprintf(fp,"Prob sum = %1.16f\n",probSum);

  return bidderSurplus;
  
}

double solveLP(int n, double vstar, double p, double vbar, 
	       const vector<double> & P, bool verbose, FILE *fp)
{
  int numPlayers = 2;

  int bid, val, player, dev,
    row, col, b1, b2;
  vector<int> bids(2,0), vals(2,0), deviations(2,0);

  double v, b, bp, F;

  double returnVal;

  vector<double> B(n,0);

  for (b=0; b<n; b++)
    B[b]=vstar*b/(n-1);
  
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
      env.set(GRB_IntParam_OutputFlag,0);

      GRBModel model(env);
      GRBVar * var;
      GRBConstr * con;

      int numVariables = n*n;

      var = model.addVars(numVariables);
      model.update();

      // Adding up constraint and objective
      GRBLinExpr probSum = 0;
      GRBLinExpr bidderSurplus = (1-p)*vbar;

      int numInitialConstraints = 0;
      for (b1 = 0; b1 < n; b1 ++)
	{
	  // Adding up constraint
	  for (b2 = 0; b2 <= b1; b2 ++)
	    {
	      probSum += var[b2*n+b1];
	      bidderSurplus -= var[b2*n+b1]*B[b1];
	    } // b2
	  
	  // Impose efficiency
	  for (b2 = b1+1; b2 < n; b2 ++)
	    {
	      model.addConstr(var[b2*n+b1]==0);
	      numInitialConstraints++;
	    }

	} // b1
      model.addConstr(probSum == 1.0-p);
      model.setObjective(bidderSurplus);
      numInitialConstraints++;
      model.update();

      int numIC = 0;
      // IC constraints
      // Bidder 1
      for (b1 = 0; b1 < n; b1 ++)
	{
	  for (dev = 0; dev < b1; dev++)
	    {
	      GRBLinExpr lhs = 0;
	      
	      for (b2 = 0; b2 <= dev; b2 ++)
		lhs += (B[b1] - B[dev])*var[b2*n+b1];

	      for (b2 = dev+1; b2 <= b1; b2 ++)
		lhs -= (vbar-B[b1])*var[b2*n+b1];

	      model.addConstr(lhs <= 0);
	      numIC++;
	    } // dev
	} // b1

      // Bidder 2
      for (b2 = 0; b2 < n; b2 ++)
	{
	  for (dev = b2+1; dev < n; dev ++)
	    {
	      GRBLinExpr lhs = 0;
	      for (b1 = b2; b1 < dev; b1 ++)
		lhs += var[b2*n+b1]*(vstar-B[dev]);
		  
	      model.addConstr(lhs <= (B[dev]-B[b2])*P[b2]);
	      numIC++;
	    } // dev
	} // b2

      fprintf(fp,"Num IC %d\n",numIC);
      fprintf(fp,"Num constraints: %d\n",model.get(GRB_IntAttr_NumConstrs));


      // -1 corresponds to maximize
      model.set(GRB_IntAttr_ModelSense,-1);
      model.optimize();

      con = model.getConstrs();

      if (verbose)
	{

	  fprintf(fp,"Multipliers on bidder 1's constraints\n");
	  row = numInitialConstraints;
	  
	  fprintf(fp," b1 ");
	  for (b1=0; b1<n; b1++)
	    fprintf(fp,"%2d ",b1);
	  fprintf(fp,"\n");
		    
	  for (b1 = 0; b1 < n; b1 ++)
	    {
	      fprintf(fp,"%3d ",b1);
	      for (dev = 0; dev < b1; dev++)
		{
		  if (abs(con[row].get(GRB_DoubleAttr_Slack))<1e-9)
		    {
		      // fprintf(fp,"%1.3f ",con[row].get(GRB_DoubleAttr_Pi));
		      fprintf(fp,"XX ");
		    }
		  else
		    {
		      // fprintf(fp,"%1.1f   ",0.0);
		      fprintf(fp,"0  ");
		    }

		  row++;
		} // dev
	      for (dev = b1; dev < n; dev ++)
		{
		  // fprintf(fp,"%1.1f   ",0.0);
		  // fprintf(fp,".     ");
		  fprintf(fp,".  ");
		}
	  
	      fprintf(fp,"\n");
	    } // b1

	  fprintf(fp,"\n");

	  fprintf(fp,"Multipliers on bidder 2's constraints\n");
	  // Bidder 2
	  fprintf(fp," b1 ");
	  for (b1=0; b1<n; b1++)
	    fprintf(fp,"%2d ",b1);
	  fprintf(fp,"\n");

	  for (b2 = 0; b2 < n; b2 ++)
	    {
	      fprintf(fp,"%3d ",b2);
	      for (dev = 0; dev < b2+1; dev ++)
		{
		  // fprintf(fp,"%1.1f   ",0.0);
		  fprintf(fp,".  ");
		}

	      for (dev = b2+1; dev < n; dev++)
		{
		  if (abs(con[row].get(GRB_DoubleAttr_Slack))<1e-9)
		    {
		      // fprintf(fp,"%1.3f ",con[row].get(GRB_DoubleAttr_Pi));
		      fprintf(fp,"XX ");
		    }
		  else
		    {
		      // fprintf(fp,"%1.1f   ",0.0);
		      fprintf(fp,"0  ");
		    }

		  row++;
		} // dev
	  
	      fprintf(fp,"\n");
	    } // b2

	  // fprintf(fp,"\nSlacks on bidder 1's constraints\n");
	  // row = numInitialConstraints;
	  // for (b1 = 0; b1 < n; b1 ++)
	  //   {
	  //     for (dev = 0; dev < b1; dev++)
	  // 	{
	  // 	  if (abs(con[row].get(GRB_DoubleAttr_Slack))>1e-9)
	  // 	    {
	  // 	      fprintf(fp,"%1.3f ",con[row].get(GRB_DoubleAttr_Slack));
	  // 	    }
	  // 	  else
	  // 	    fprintf(fp,"%1.1f   ",0.0);

	  // 	  row++;
	  // 	} // dev
	  //     for (dev = b1; dev < n; dev ++)
	  // 	fprintf(fp,"%1.1f   ",0.0);
	  
	  //     fprintf(fp,"\n");
	  //   } // b1

	  // fprintf(fp,"\n");

	  // fprintf(fp,"Slacks on bidder 2's constraints\n");
	  // // Bidder 2
	  // for (b2 = 0; b2 < n; b2 ++)
	  //   {
	  //     for (dev = 0; dev < b2+1; dev ++)
	  // 	fprintf(fp,"%1.1f   ",0.0);

	  //     for (dev = b2+1; dev < n; dev++)
	  // 	{
	  // 	  if (abs(con[row].get(GRB_DoubleAttr_Slack))>1e-9)
	  // 	    {
	  // 	      fprintf(fp,"%1.3f ",con[row].get(GRB_DoubleAttr_Slack));
	  // 	    }
	  // 	  else
	  // 	    fprintf(fp,"%1.1f   ",0.0);

	  // 	  row++;
	  // 	} // dev
	  
	  //     fprintf(fp,"\n");
	  //   } // b2


	  fprintf(fp,"\n");

	  // fprintf(fp,"LP output\n");
	  // for (b1 = 0; b1 < n; b1 ++)
	  //   {
	  //     double sum = 0;
	  //     for (b2 = 0; b2 <= b1; b2 ++)
	  // 	{
	  // 	  if (var[b2*n+b1].get(GRB_DoubleAttr_X)>1e-9)
	  // 	    {
	  // 	      fprintf(fp,"%1.3f ",var[b2*n+b1].get(GRB_DoubleAttr_X));
	  // 	      sum += var[b2*n+b1].get(GRB_DoubleAttr_X);
	  // 	    }
	  // 	  else
	  // 	    fprintf(fp,"%1.1f   ",0.0);
	  // 	}		
	  
	  //     for (b2 = b1+1; b2 < n; b2 ++)
	  // 	fprintf(fp,"%1.1f   ",0.0);
	  
	  //     fprintf(fp,"%1.4f \n",sum);
	  //   } // b1


	} // if verbose

      fprintf(fp,"Objective = %1.16f\n",bidderSurplus.getValue());
      returnVal = bidderSurplus.getValue();
      
      delete[] var;
      delete[] con;
    } // try
  catch (GRBException &e)
    {
      cout << e.getMessage() << endl;
    }

  return returnVal;
}
