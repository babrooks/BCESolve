#ifndef FPASOLVER_HPP
#define FPASOLVER_HPP

#include "bce.hpp"

class FPASolver : public BCESolver
{
protected:
  FPAGame & fpa;

public:
  FPASolver(FPAGame & _fpa)
    : fpa(_fpa), BCESolver(_fpa)
  { }

  void populate()
  {
    BCESolver::populate();

    // Now add extra constraints corresponding to an ex-ante entry
    // fee. Just the integral of incentive constraints across all
    // positive bids, versus a zero bid. Should be imposed for each
    // possible type. Do this using auxiliary objective functions that
    // correspond to payoff from positive bids.

    cout << "objective at (1,1) = " 
	 << fpa.objective(15,vector<int>(2,1),1) << endl;
    cout << "objective at (0,0) = " 
	 << fpa.objective(89,vector<int>(2,1),0) << endl;
    
    for (int player = 0; player < 2; player++)
      {
	if (fpa.numTypes[0]>1)
	  {
	    for (int val = 0; val < fpa.numValues; val++)
	      {
		cplex.getModel().add(getObjectiveFunction(5+val+player*fpa.numValues)>=-1e-6);
	      } // val
	  }
	else
	  {
	    IloNumExpr payoff = 0;
	    for (int val = 0; val < fpa.numValues; val++)
	      {
		payoff += getObjectiveFunction(5+val+player*fpa.numValues);
	      } // val
	    cplex.getModel().add(payoff>=0.0);
	  }
      } // player
    
  } // populate
};

#endif
