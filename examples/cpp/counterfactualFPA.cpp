// A 1 player counterfactual example
#include "gurobi_c++.h"
#include <stdio.h>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <cmath>

using namespace std;

enum SolnMode{ObsMaxRev, ObsMinRev, ObsMeanRev,
	      ObsMaxRevVariableVal, ObsMinRevVariableVal,
	      VariableInfo, VariableInfoVariableVal};

double payoff(double bi, double bj, const vector<double> & Bgrid,
	      double v,int reserve)
{
  if (bi > bj && bi >= reserve)
    return v-Bgrid[bi];
  else if (bi==bj && bi>=reserve)
    return 0.5*(v-Bgrid[bi]);
  return 0.0;
}

double revenue(int b0, int b1,const vector<double> & Bgrid,int reserve)
{
  if (b0>b1 && b0 >= reserve)
    return Bgrid[b0];
  else if (b1 >= reserve)
    return Bgrid[b1];
  return 0.0;
}

void simulateFPA(int numVals, int numBids, SolnMode mode);

int main ()
{
  
  int numVals = 15;
  // int bidsPerVal = 2;
  // int numBids = (numVals-1)*bidsPerVal+1;
  int numBids = numVals;

  simulateFPA(numVals,numBids,ObsMaxRev);
  simulateFPA(numVals,numBids,ObsMinRev);
  simulateFPA(numVals,numBids,VariableInfo);
  simulateFPA(numVals,numBids,ObsMaxRevVariableVal);
  simulateFPA(numVals,numBids,ObsMinRevVariableVal);

  simulateFPA(numVals,numBids,ObsMeanRev);
  
  return 0;
}

void simulateFPA(int numVals, int numBids, SolnMode mode)
{
  stringstream ss;
  ss << "counterfactualFPAprivate_nv=" << numVals
     << "_nb=" << numBids;
  
  if (mode==ObsMaxRev)
    {
      cout << endl << "Mode: ObsMaxRev" << endl;
      ss << "_obsmaxrev";
    }
  else if (mode==ObsMeanRev)
    {
      cout << endl << "Mode: ObsMeanRev" << endl;
      ss << "_obsmeanrev";
    }
  else if (mode==ObsMinRev)
    {
      cout << endl << "Mode: ObsMinRev" << endl;
      ss << "_obsminrev";
    }
  else if (mode==ObsMaxRevVariableVal)
    {
      cout << endl << "Mode: ObsMaxRevVariableVal" << endl;
      ss << "_obsmaxrevvariableval";
    }
  else if (mode==ObsMinRevVariableVal)
    {
      cout << endl << "Mode: ObsMinRevVariableVal" << endl;
      ss << "_obsminrevvariableval";
    }
  else if (mode==VariableInfo)
    {
      cout << endl << "Mode: VariableInfo" << endl;
      ss << "_variableinfo";
    }
  else if (mode==VariableInfoVariableVal)
    {
      cout << endl << "Mode: VariableInfoVariableVal" << endl;
      ss << "_variableinfo";
    }
  ss << ".log";

  ofstream ofs(ss.str().c_str());

  int obsReserve = 0;
  int unobsReserve = 0;

  vector<double> Bgrid(numBids,0.0);
  cout << endl;
  for (int b = 0; b < numBids; b++)
    {
      Bgrid[b] = static_cast<double>(b)/(numBids-1.0);
      cout << Bgrid[b] << " ";
    }
  cout << endl;
  vector<double> Vgrid(numVals,0.0);
  for (int v = 0; v < numVals; v++)
    {
      Vgrid[v] = static_cast<double>(v)/(numVals-1.0);
      cout << Vgrid[v] << " ";
    }
  cout << endl;
  
  for (unobsReserve = 0; unobsReserve < numBids; unobsReserve++)
    {
      cout << "unobsReserve = " << unobsReserve << endl;
      try
	{
	  GRBEnv env = GRBEnv();
	  env.set(GRB_IntParam_Method,2);
	  env.set(GRB_IntParam_Crossover,0);
	  env.set(GRB_IntParam_OutputFlag,0);
	  GRBModel model(env);

	  int numStates=numVals*numVals;
	  int numVariables = numStates*numBids*numBids*numBids*numBids;

	  GRBVar * var = model.addVars(numVariables);
	  // formula for variable location:
	  // numBids^4*(val0*numVals+val1)+numBids^3*obsAction0+numBids^2*unObsAction0
	  // +numBids*obsAction1+unobsAction1.
      
	  model.update();
      
	  GRBLinExpr obsRevenue=0.0;
	  GRBLinExpr obsBidderSurplus=0.0;
	  GRBLinExpr obsWelfare=0.0;
	  GRBLinExpr unobsRevenue=0.0;
	  GRBLinExpr unobsBidderSurplus=0.0;
	  GRBLinExpr unobsWelfare=0.0;

	  // cout << "Adding probability constraints..." << endl;

	  // Add probability constraints and define objective
	  vector<GRBConstr> probConstr (numVals*numVals);
	  for (int v0=0; v0<numVals; v0++)
	    {
	      for (int v1=0; v1<numVals; v1++)
		{
		  GRBLinExpr stateProb = 0;
		  for (int bo0 = 0; bo0 < numBids; bo0++)
		    {
		      for (int bo1 = 0; bo1 < numBids; bo1++)
			{
			  for (int bu0 = 0; bu0 < numBids; bu0++)
			    {
			      for (int bu1 = 0; bu1 < numBids; bu1++)
				{
				  int row = numBids*(numBids*(numBids*(numBids*(numVals*v0+v1)+bo0)+bu0)+bo1)+bu1;

				  stateProb += var[row];

				  obsBidderSurplus += var[row]*(payoff(bo0,bo1,Bgrid,Vgrid[v0],obsReserve)
								+payoff(bo1,bo0,Bgrid,Vgrid[v1],obsReserve));
				  unobsBidderSurplus += var[row]*(payoff(bu0,bu1,Bgrid,Vgrid[v0],unobsReserve)
								  +payoff(bu1,bu0,Bgrid,Vgrid[v1],unobsReserve));
				  obsRevenue += var[row]*revenue(bo0,bo1,Bgrid,obsReserve);
				  unobsRevenue += var[row]*revenue(bu0,bu1,Bgrid,unobsReserve);

				  if (max(Bgrid[bo0],Bgrid[bu0]) > Vgrid[v0]+0.5/(numBids-1.0)
				      || max(Bgrid[bo1],Bgrid[bu1]) > Vgrid[v1]+0.5/(numBids-1.0))
				    {
				      var[row].set(GRB_DoubleAttr_UB,0.0);
				    }
				} // bu1
			    } // bu0
			} // bo1
		    } // bo0
		  // Uniform prior over values
		  stringstream ss;
		  ss << "prob_v0=" << v0 << "_v1=" << v1;
		  probConstr[v0*numVals+v1] = model.addConstr(stateProb == 1.0/static_cast<double>(numVals*numVals),
							      ss.str());
		} // v1
	    } // v0
	  obsWelfare += obsBidderSurplus + obsRevenue;
	  unobsWelfare += unobsBidderSurplus + unobsRevenue;

	  // cout << "Adding obedience constraints..." << endl;
      
	  // Obedience constraints
	  for (int v = 0; v < numVals; v++)
	    {
	      for (int bo = 0; bo < numBids; bo++)
		{
		  if (Bgrid[bo] > Vgrid[v] + 0.5/(numBids-1.0))
		    break;
		  for (int bu = 0; bu < numBids; bu++)
		    {
		      if (Bgrid[bu] > Vgrid[v] + 0.5/(numBids-1.0))
			break;
		      for (int bp = 0; bp < numBids; bp++)
			{
			  if (Bgrid[bp] > Vgrid[v] +0.5/(numBids-1.0))
			    break;
		      
			  // Deviation in observed action
			  GRBLinExpr obsDevGain0 = 0.0;
			  GRBLinExpr unobsDevGain0 = 0.0;
			  GRBLinExpr obsDevGain1 = 0.0;
			  GRBLinExpr unobsDevGain1 = 0.0;
			  for (int boj = 0; boj < numBids; boj++)
			    {
			      for (int buj = 0; buj < numBids; buj++)
				{
				  for (int vj = 0; vj < numVals; vj++)
				    {
				      int row0 = numBids*(numBids*(numBids*(numBids*(numVals*v+vj)+bo)+bu)+boj)+buj;
				      int row1 = numBids*(numBids*(numBids*(numBids*(numVals*vj+v)+boj)+buj)+bo)+bu;

				      obsDevGain0 += (payoff(bp,boj,Bgrid,Vgrid[v],obsReserve)
						      -payoff(bo,boj,Bgrid,Vgrid[v],obsReserve))*var[row0];
				      obsDevGain1 += (payoff(bp,boj,Bgrid,Vgrid[v],obsReserve)
						      -payoff(bo,boj,Bgrid,Vgrid[v],obsReserve))*var[row1];
				      unobsDevGain0 += (payoff(bp,buj,Bgrid,Vgrid[v],unobsReserve)
							-payoff(bu,buj,Bgrid,Vgrid[v],unobsReserve))*var[row0];
				      unobsDevGain1 += (payoff(bp,buj,Bgrid,Vgrid[v],unobsReserve)
							-payoff(bu,buj,Bgrid,Vgrid[v],unobsReserve))*var[row1];

				    } // vj
				} // buj
			    } // boj
			  if (bp != bo)
			    {
			      stringstream ss0, ss1;
			      ss0 << "obsIC0_v=" << v << "_bo=" << bo << "_bu=" << bu << "_bp=" << bp;
			      ss1 << "obsIC1_v=" << v << "_bo=" << bo << "_bu=" << bu << "_bp=" << bp;
			      model.addConstr(obsDevGain0 <= 0,ss0.str());
			      model.addConstr(obsDevGain1 <= 0,ss1.str());
			    }
			  if (bp != bu)
			    {
			      stringstream ss0, ss1;
			      ss0 << "unobsIC0_v=" << v << "_bo=" << bo << "_bu=" << bu << "_bp=" << bp;
			      ss1 << "unobsIC1_v=" << v << "_bo=" << bo << "_bu=" << bu << "_bp=" << bp;
			      model.addConstr(unobsDevGain0 <= 0,ss0.str());
			      model.addConstr(unobsDevGain1 <= 0,ss1.str());
			    }
			} // bp
		    } // bu
		} // bo
	    } // v

	  // cout << "Optimizing..." << endl;

	  if (mode != VariableInfo && mode != VariableInfoVariableVal)
	    {
	      model.setObjective(obsRevenue);
	      model.set(GRB_IntAttr_ModelSense,GRB_MINIMIZE);
	      if (mode==ObsMaxRev || mode==ObsMaxRevVariableVal || mode==ObsMeanRev)
		model.set(GRB_IntAttr_ModelSense,GRB_MAXIMIZE);
	      GRBConstr tempConstr;
	      if (mode == ObsMeanRev)
		{
		  tempConstr = model.addConstr(obsRevenue <= 0.33);
		}
	      model.optimize();
	      model.remove(tempConstr);


	      vector<double> targetVals(numVals*numVals*numBids*numBids,0);

	      if (model.get(GRB_IntAttr_Status)==GRB_INFEASIBLE)
		{
		  cout << "Model infeasible" << endl;
		  model.computeIIS();
		  GRBConstr * c  = model.getConstrs();
	  
		  for (int k = 0; k < model.get(GRB_IntAttr_NumConstrs); k++)
		    {
		      if (c[k].get(GRB_IntAttr_IISConstr))
			cout << c[k].get(GRB_StringAttr_ConstrName) << endl;
		    }
		}

	      // Add probability constraints to hold this equilibrium fixed
	      for (int bo0 = 0; bo0 < numBids; bo0++)
		{
		  for (int bo1 = 0; bo1 < numBids; bo1++)
		    {
		      GRBLinExpr actionProb = 0;
		      double obsActionProb = 0;
		      for (int v0=0; v0<numVals; v0++)
			{
			  for (int v1=0; v1<numVals; v1++)
			    {
			      GRBLinExpr prob = 0;
			      double obsProb = 0;
			      for (int bu0 = 0; bu0 < numBids; bu0++)
				{
				  for (int bu1 = 0; bu1 < numBids; bu1++)
				    {
				      int row = numBids*(numBids*(numBids*(numBids*(numVals*v0+v1)+bo0)+bu0)+bo1)+bu1;
				      prob += var[row];
				      obsProb += var[row].get(GRB_DoubleAttr_X);
				      actionProb += var[row];
				      obsActionProb += var[row].get(GRB_DoubleAttr_X);
				    } // bu1
				} // bu0
		  
			      if (mode == ObsMaxRev || mode==ObsMinRev
				  || mode == ObsMeanRev)
				model.addConstr(prob == obsProb);
			    } // v1
			} // v0
		      if (mode == ObsMaxRevVariableVal || mode==ObsMinRevVariableVal)
			model.addConstr(actionProb==obsActionProb);
		    } // bo1
		} // bo0

	    }
      
	  model.setObjective(unobsWelfare);
	  model.set(GRB_IntAttr_ModelSense,GRB_MINIMIZE);
	  model.optimize();

	  if (mode==ObsMaxRevVariableVal || mode==ObsMinRevVariableVal)
	    {
	      for (int s = 0; s< numVals*numVals; s++)
		model.remove(probConstr[s]);
	    }
      
	  // cout << "Done minimizing..." << endl;

	  ofs << Bgrid[unobsReserve] << " " << setprecision(8)
	      << obsWelfare.getValue() << " " 
	      << obsBidderSurplus.getValue() << " " 
	      << obsRevenue.getValue() << " " 
	      << unobsWelfare.getValue() << " ";

	  model.set(GRB_IntAttr_ModelSense,GRB_MAXIMIZE);
	  model.optimize();
	  ofs << unobsWelfare.getValue() << " ";

	  model.setObjective(unobsBidderSurplus);
	  model.set(GRB_IntAttr_ModelSense,GRB_MINIMIZE);
	  model.optimize();
	  ofs << unobsBidderSurplus.getValue() << " ";

	  model.set(GRB_IntAttr_ModelSense,GRB_MAXIMIZE);
	  model.optimize();
	  ofs << unobsBidderSurplus.getValue() << " ";

	  model.setObjective(unobsRevenue);
	  model.set(GRB_IntAttr_ModelSense,GRB_MINIMIZE);
	  model.optimize();
	  ofs << unobsRevenue.getValue() << " ";

	  model.set(GRB_IntAttr_ModelSense,GRB_MAXIMIZE);
	  model.optimize();
	  ofs << unobsRevenue.getValue() << " ";

	  ofs << endl;

	  // cout << "Done maximizing..." << endl;


	  // cout << "Done!" << endl;

	}
      catch (GRBException &e)
	{
	  cout << e.getMessage() << endl;
	}
    }
  ofs.close();

} // simulateFPA
