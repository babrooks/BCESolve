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

  int stateToPrivateValues(int state, vector<int> &values)
  {
    values.resize(2);

    values[0]=state%numValues;
    state-=values[0]; 
    values[1]=state/numValues;
  }

  double FPAPrior (int state, vector<int> types)
  {
    // Use a uniform prior
    vector<int> values(2,0);
    stateToPrivateValues(state,values);

    if (values[0]==types[0] && values[1]==types[1])
      return (1.0/numValues/numValues);
    else
      return 0.0;
  }

  double FPAPayoff (int state, vector<int> actions, int objective, int numBids, int reserveBid)
  {
    // Convert the state into a pair of valuations
    vector<int> values(2,0);
    stateToPrivateValues(state,values);
    int maxBid = *max_element(actions.begin(),actions.end());
    int maxVal = *max_element(values.begin(),values.end());

    if (objective==0)
      {
	// Player 1's payoff
	if (actions[0]>=reserveBid 
	    && actions[0]>actions[1] 
	    || (actions[0]==actions[1] 
		&& values[0]>values[1]))
	  return (1.0*values[0]/(numValues-1.0)-1.0*actions[0]/(numBids-1.0));
	else if (actions[0]>=reserveBid
		 && actions[0]==actions[1] 
		 && values[0]==values[1])
	  return (1.0*values[0]/(numValues-1.0)-1.0*actions[0]/(numBids-1.0))/2.0;
	else
	  return (0.0);
      }
    else if (objective==1)
      {
	// Player 2's payoff
	if (actions[1]>=reserveBid 
	    && actions[0]<actions[1] 
	    || (actions[0]==actions[1] 
		&& values[0]<values[1]))
	  return (1.0*values[1]/(numValues-1.0)-1.0*actions[1]/(numBids-1.0));
	else if (actions[1]>=reserveBid
		 && actions[0]==actions[1] 
		 && values[0]==values[1])
	  return (1.0*values[1]/(numValues-1.0)-1.0*actions[1]/(numBids-1.0))/2.0;
	else
	  return (0.0);
      }
    else if (objective==2)
      {
	// Revenue
	
	if (maxBid>=reserveBid)
	  return ((static_cast<double>(maxBid))/(static_cast<double>(numBids)-1));
      }
    else if (objective==3)
      {
	// Surplus
	if (actions[0]>=reserveBid
	    && actions[0]>actions[1] 
	    || (actions[0]==actions[1] 
		&& values[0]>values[1]))
	  return (1.0*values[0]/(numValues-1.0)); // Player 1 won
	else if (actions[1]>=reserveBid)
	  return (1.0*values[1]/(numValues-1.0)); // Either player 2 won or they tied with same val.
	else
	  return (0); // No one won. Both bids were below the reserve price.
      }
  }

  bool FPADominated(int a, int t, int player, int numBids)
  {
    if ((static_cast<double>(a))/(static_cast<double>(numBids) - 1.0) 
	> (static_cast<double>(t))/(static_cast<double>(numValues) - 1.0))
      return (true);
    else
      return (false);
  }

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
  double prior (int state, vector<int> types) const
  { return FPAPrior(state,types); }

  double objective (int state, vector<int> actions, int objective) const
  { return FPAPayoff(state,actions,objective,numActions,0); }

  bool dominated(int a, int t, int player) const
  { return FPADominated(a,t,player,numActions); }

public:
  FPA_BCEModel() {};
  FPA_BCEModel(int na, int nv, int nt, int no): BCEBase(2,na,nv*nv,nt,no), FPABase(nv)
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
  double payoff(int state, vector< vector<int> > actions, int objective)
  {
    // Objectives 0-3 correspond to mechanism 0, which has 0 reserve bid.
    if (objective<4)
      {
	return FPAPayoff(state,actions[0],objective,numActions[0],0);
      }
    else
      {
	objective-=4;
	return FPAPayoff(state,actions[1],objective,numActions[1],reserveBid);
      }
  }

  bool dominated(int action, int type, int player, int mechanism)
  {
    return FPADominated(action,type,player,numActions[mechanism]);
  }

public:
  FPA_RCAModel() {};
  FPA_RCAModel(int numPlayersArg, 
	       vector<int> numActionsArg, 
	       int numValuesArg, 
	       int numTypesArg, 
	       int numObjectivesArg,
	       vector<double> distributionArg) :
    RCABase(numPlayersArg,numActionsArg,numValuesArg*numValuesArg,numTypesArg,numObjectivesArg,distributionArg),
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
  int numBids=40;
  int numValues=3;
  int reserveBidIncrement=1;
  int reserveCounter, reserveBoundCounter;
  vector<int> numBidsVec(2,numBids);
  vector<double> empiricalDistribution, 
    revenueUB(numBids/reserveBidIncrement,0), 
    revenueLB(numBids/reserveBidIncrement,0); // Intentional integer division.

  FPA_BCEModel dataGenerator(numBids,numValues,numValues,4);

  try
    {
      /**********************************/
      // Generate the data 
      /**********************************/

      // Set up the model to generate data.
      dataGenerator.populate();

      // Set the objective function (max p1 + p2)
      IloCplex dataGeneratorCplex = dataGenerator.getCplex();
      dataGeneratorCplex.getObjective().setExpr(
						dataGenerator.getObjectiveFunction(1)
						+dataGenerator.getObjectiveFunction(0)
						);

      // A couple of parameters
      dataGeneratorCplex.setParam(IloCplex::BarDisplay,1);
      dataGeneratorCplex.setParam(IloCplex::SimDisplay,1);
      dataGeneratorCplex.setParam(IloCplex::RootAlg,IloCplex::Dual);

      // Solve the model
      dataGeneratorCplex.solve();

      // Save the marginal distribution of bids
      dataGenerator.actionMarginalToVector(empiricalDistribution);

      /**********************************/
      // Find counter factual max and min revenue
      /**********************************/
      // Set up the RCA model. Note we have 8 objectives; four for each mechanism.
      FPA_RCAModel counterFactual(2,numBidsVec,numValues,1,8,empiricalDistribution);
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
	  revenue = counterFactual.getObjectiveFunction(6);
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
      outputfile.open("rcatest.dat");
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
