// Computes minimum revenue for the resale mechanism

#include "bce.hpp"

class ResaleMech : public BCEAbstractGame
{
private:
  double r;
public:
  ResaleMech(int na,int ns,double _r):
    BCEAbstractGame(ns,na,1,3),
    r(_r)
  {  }

  double prior(int state, const vector<int> & types) const
  { 
    return 1.0/static_cast<double>(numStates);
  }

  double objective(int state, const vector<int> & actions,
		   int obj) const
  {
    vector<double> s(2), t(2);
    double maxs = 0;
    for (int p=0; p<2; p++)
      {
	s[p] = actions[p]/(numActions[p]-1.0);
	maxs=max(maxs,s[p]);
      }

    double v = static_cast<double>(state)/(numStates-1.0);
    double T = r*(1.0-r);
    double g = (1-r/maxs);
    
    if (maxs<r)
      return 0;

    if (obj<2)
      {
	if (actions[obj]==0)
	  return 0;
	if (actions[obj] > actions[1-obj])
	  return (v*g-T);
	else if (actions[obj] == actions[1-obj])
	  return (0.5*v-T);
	else
	  return (v*(1-g)-T);
      }
    else if (obj==2)
      {
	return 2*T;
      }

    return 0.0;
  }
  
}; // resalemech

int main()
{
  int numA = 50;
  int numS = 2;
  double r=0.25;

  ResaleMech mech(numA,numS,r);

  BCESolver solver(mech);

  try
    {
      solver.populate();
      vector<double> objWeights(3,0);
      objWeights[2]=-1;
      solver.solve(objWeights);

      cout << "Social surplus: "
	   << ( solver.getObjectiveFunction(0).getValue()
		+ solver.getObjectiveFunction(1).getValue()
		+ solver.getObjectiveFunction(2).getValue() ) << endl;
      
      stringstream ss;
      ss << "maxminconj2_" << numA << ".bce";
      BCESolution soln;
      solver.getSolution(soln);
      BCESolution::save(soln,ss.str().c_str());
    }
  catch (GRBException & e)
    {
      cout << e.getMessage() << endl;
    }

  return 0;
}
