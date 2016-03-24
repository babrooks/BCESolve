// The optimal mechanism on the MVS type space

#include "bce.hpp"

class MVSOpt : public BCEAbstractGame
{
private:
  int nv;
  double r;
  double sbar;

  double gamma(double s) const
  {
    if (s>=r)
      return 0.5*(1.0-CDF(r)/CDF(s)); 
    else
      return 0.0;
  } // gamma

  double price(double s) const
  {
    if (s>=r)
      return s*(0.5+r*r/(s*s-r*r)*(log(s)-log(r)));
    return 0.0;
  }
  
  double CDF(double s) const
  {
    return s*s;
  } // CDF

public:
  MVSOpt(int _nv,double _r, double _sbar):
    BCEAbstractGame(_nv,_nv+1,1,3),
    nv(_nv), r(_r), sbar(_sbar)
  {} // constructor

  double prior(int state, const vector<int> & types) const
  {
    return ( CDF(static_cast<double>(state)/static_cast<double>(numStates))
	     - CDF( (state-1.0)/static_cast<double>(numStates)) );
  } // prior

  double objective(int state,
		   const vector<int> & actions,
		   int objIndex) const
  {
    // Convert the state into a pair of valuations
    double value = static_cast<double>(state)/(nv-1.0);

    double obj = 0; 
    
    int winner = actions[1] >= actions[0];
    
    double winbid = actions[winner] * sbar / (numActions[0]-1.0);

    if (winbid < r)
      return 0.0;

    if (objIndex < 2)
      {
	int player = objIndex;

	if (actions[player]==0)
	  return 0;

	// Player 1's payoff
	if (actions[player]>actions[1-player])
	  obj = (value-price(winbid))*gamma(winbid);
	else if (actions[player]==actions[1-player])
	  obj = (value-price(winbid)*gamma(winbid)
		 - winbid*(1.0-gamma(winbid)))/2.0;
	else
	  obj = (value-winbid)*(1.0-gamma(winbid));
      }
    else if (objIndex==2)
      {
	// Revenue
	obj = winbid+gamma(winbid)*(price(winbid)-winbid);
      }

    return obj;
    
  } // objective
};

int main()
{
  int nv = 50;
  double r = 1.0/exp(1.0);
  double highbid = 1;

  try
    {
      MVSOpt opt(nv,r,highbid);
  
      // BCEGame game(opt);

      // BCEGame::save(opt,"mvsopt.bgm");

      BCESolver solver(opt);

      solver.populate();
      solver.model.setObjective(solver.getObjectiveFunction(2),GRB_MINIMIZE);
      solver.solve();
  
      cout << "REVENUE: " << solver.getObjectiveFunction(2).getValue() << endl;

      BCESolution soln;
      solver.getSolution(soln);
      BCESolution::save(soln,"mvsopt.bce");
    }
  catch (GRBException & e)
    {
      cout << e.getMessage() << endl;
    }
  return 0;
}

