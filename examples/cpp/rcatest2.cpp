// Test program for rca.cpp
// BAB 2-27-2012
#include "rca.hpp"
#include "bce.hpp"

// Class to hold some utilities for the first price auction, used for
// both the BCE and RCA.
class FPABase
{
protected:
  // First-price sealed-bid auction with with private values and risk
  // neutral bidders and a reservation price.
  int numValues;

  double FPAPrior (int state, vector<int> types) const
  {
    // Use a uniform prior
    return 1.0/numValues;
  }

  double FPAPayoff (int state, vector<int> actions, 
		    int obj, int numBids, int reserveBid) const
  {
    // Convert the state into a pair of valuations
    if (obj<2)
      {
	if (actions[obj]>=reserveBid
	    && actions[obj]>actions[1-obj])
	  return (1.0*state/(numValues-1.0)-1.0*actions[obj]/(numBids-1.0));
	else if (actions[obj]>=reserveBid
		 && actions[obj]==actions[1-obj])
	  return 0.5*(1.0*state/(numValues-1.0)-1.0*actions[obj]/(numBids-1.0));
	else
	  return 0.0;
      }
    else if (obj==2)
      {
	// Revenue
	int maxBid = (actions[0]>actions[1]? actions[0]: actions[1]);
	if (maxBid>=reserveBid)
	  return ((static_cast<double>(maxBid))/(static_cast<double>(numBids)-1));
      }
  } // objective

public:
  FPABase() {};

  FPABase(int nv) : numValues(nv) {};
}; // FPABase

// BCEModel inherits BCEBase and FPABase. It treats the payoff,
// dominated, and prior virtual methods in the BCEBase class as
// wrappers for the corresponding methods of FPABase.
class FPA_BCEModel : public BCEAbstractGame, public FPABase
{
private:
  double prior (int state, const vector<int> & types) const
  { return FPAPrior(state,types); }

  double objective (int state, const vector<int> & actions, int objective) const
  { return FPAPayoff(state,actions,objective,numActions[0],0); }

public:
  FPA_BCEModel() {};
  FPA_BCEModel(int na, int nv, int no): BCEAbstractGame(nv,na,1,no), FPABase(nv)
  {};
};

// RCAModel inherits RCABase and FPABase. Mechanism 0 uses a a reserve
// bid of 0 when passing to FPABase methods, but mechanism 1 uses
// reserveBid.
class FPA_RCAModel : public RCABase, public FPABase
{
private:
  int reserveBid; // The exercise is to calculate what would happen to
		  // revenue under different reserve prices.

  // Don't overload the conditionalUB/LB methods, so that there is no
  // restriction, i.e. we let players have no information.
  
  // Payoff gets passed to the FPAPayoff method.
  double payoff(int state, vector< vector<int> > actions, int objective) const
  {
    // Objectives 0-2 correspond to mechanism 0, which has 0 reserve bid.
    if (objective<3)
      {
	return FPAPayoff(state,actions[0],objective,numActions[0],0);
      }
    else
      {
	objective-=3;
	return FPAPayoff(state,actions[1],objective,numActions[1],reserveBid);
      }
  }

  bool dominated(int action, int type, int player, int mechanism)
  {
    return false;
  }

public:
  FPA_RCAModel() {};
  FPA_RCAModel(vector<int> numActionsArg, 
	       int numValuesArg, 
	       int numObjectivesArg,
	       vector<double> distributionArg) :
    RCABase(2,numActionsArg,numValuesArg,1,numObjectivesArg,distributionArg),
    FPABase(numValuesArg)
  {};

  void setReserveBid(int newReserve) { reserveBid=newReserve; };
};

// In the main function, we use an FPA_BCEModel to solve for a BCE. We
// then take the marginal distribution from this equilibrium, and pass
// ot to a FPA_RCAModel to solve for what WOULD have happened under
// the counterfactual different reserve price.
int main (int argc, char **argv)
{
  int numBids=10;
  int numValues=3;
  int reserveBidIncrement=1;
  int reserveCounter, reserveBoundCounter;
  vector<int> numBidsVec(2,numBids);
  vector<double> empiricalDistribution, 
    revenueUB(numBids/reserveBidIncrement,0), 
    revenueLB(numBids/reserveBidIncrement,0); // Intentional integer division.

  FPA_BCEModel dataGenerator(numBids,numValues,4);
  BCESolver solver(dataGenerator);

  try
    {
      /**********************************/
      // Generate the data 
      /**********************************/

      // Set up the model to generate data.
      solver.populate();

      // Set the objective function (max p1 + p2)
      IloCplex dataGeneratorCplex = solver.getCplex();
      dataGeneratorCplex.getObjective().setExpr(solver.getObjectiveFunction(1)
						+solver.getObjectiveFunction(0));

      // A couple of parameters
      // dataGeneratorCplex.setParam(IloCplex::BarDisplay,1);
      // dataGeneratorCplex.setParam(IloCplex::SimDisplay,1);
      dataGeneratorCplex.setParam(IloCplex::RootAlg,IloCplex::Barrier);

      // Solve the model
      solver.solve();

      cout << "Solved data generator." << endl;

      // Save the marginal distribution of bids
      BCESolution soln;
      solver.getSolution(soln);
      soln.getConditionalMarginal(vector<int>(0),
				  vector< vector<int> > (2,vector<int>(0)), 
				  vector< vector<int> > (2,vector<int>(0)),
				  false,vector<bool>(2,true),vector<bool>(2,false),
				  empiricalDistribution);

      cout << "Retrieved empirical distribution." << endl;

      /**********************************/
      // Find counter factual max and min revenue
      /**********************************/
      // Set up the RCA model. Note we have 8 objectives; four for each mechanism.
      FPA_RCAModel counterFactual(numBidsVec,numValues,6,empiricalDistribution);
      IloNumExpr revenue;
      IloCplex counterFactualCplex;

      // Loop through to find the upper and lower bounds of revenue
      // for each reservation price.
      for (reserveCounter=0; reserveCounter<numBids; reserveCounter+=reserveBidIncrement)
	{
	  // Update the reserve bid
	  counterFactual.setReserveBid(reserveCounter);
	  
	  // Populate the model
	  counterFactual.populate();

	  // Find the objectives, which is revenue under the mechanism
	  // 1 (the second mechanism). It is in location 6. Make
	  revenue = counterFactual.getObjectiveFunction(5);
	  counterFactualCplex=counterFactual.getCplex();
	  counterFactualCplex.setParam(IloCplex::BarDisplay,1);
	  counterFactualCplex.setParam(IloCplex::SimDisplay,1);
	  counterFactualCplex.setParam(IloCplex::RootAlg,IloCplex::Dual);

	  // Solve for maximum revenue.
	  counterFactualCplex.getObjective().setExpr(revenue);
	  counterFactualCplex.solve();
	  revenueUB[reserveCounter/reserveBidIncrement]=
	    counterFactualCplex.getObjValue();

	  // Solve for minimum revenue.
	  counterFactualCplex.getObjective().setExpr(-revenue);
	  counterFactualCplex.solve();
	  revenueLB[reserveCounter/reserveBidIncrement]=
	    -(counterFactualCplex.getObjValue());
	  
	  // Clear the model (to get ready for the next call to populate).
	  counterFactual.clear();
	} // reserveCounter

      // Now print the data.
      ofstream outputfile;
      outputfile.open("rcatest2.dat");
      outputfile << setprecision(14);
      for (reserveBoundCounter=0; 
	   reserveBoundCounter<revenueUB.size(); 
	   reserveBoundCounter++)
	{
	  outputfile << revenueLB[reserveBoundCounter]
		     << " "
		     << revenueUB[reserveBoundCounter]
		     << endl;
	}
      outputfile.close();

    }
  catch (IloException & e)
    {
      cerr << "Concert exception caught: " << e << endl
	   << e.getMessage() << endl;
    }
  catch (...)
    {
      cout << "Unknown exception caught." << endl;
    }
  


  return 0;
}
