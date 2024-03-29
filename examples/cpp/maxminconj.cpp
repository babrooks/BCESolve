#include "bce.hpp"

class MyMech : public BCEAbstractGame
{
private:
  double a;
public:
  MyMech(int na):
    BCEAbstractGame(2,na,1,3),
    a(0.24)
  {
    
  }

  double prior(int state, const vector<int> & types) const
  { 
    if (state==1)
      return 0.83; 
    return 0.17;
  }

  double objective(int state, const vector<int> & actions,
		   int obj) const
  {
    vector<double> s(2);
    for (int p=0; p<2; p++)
      s[p] = (1.0-a)*actions[p]/(numActions[p]-1.0);

    double v = 0; // This is just the reported v of course
    if ((1.0-s[0])*(1.0-s[1])<=a || s[0] == numActions[0]-1 || s[1] == numActions[1]-1)
      v = 1.0;
    else
      v = a/((1.0-s[0])*(1.0-s[1]));

    double x = 0;
    double alpha = (1.0-log(a))/(1.0-log(a)+0.5*(log(a)*log(a)));
    
    if (obj<2)
      {
	double q = alpha*(1-(1-s[1-obj])*(1-s[1-obj]))
	  + alpha*(log(v)-log(a/((1-s[1-obj])*(1-s[1-obj]) ) ) );
	assert(q>=0);

	if (actions[obj] > actions[1-obj])
	  return q*static_cast<double>(state)-alpha*(v-a);
	else if (actions[obj]==actions[1-obj])
	  return (q*static_cast<double>(state)-alpha*(v-a))/2;
      }
    else if (obj==2)
      {
	return alpha*(v-a);
      }

    return 0.0;
  }
  
};

int main()
{
  int numA = 75;
  MyMech mech(numA);

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
      ss << "maxminconj_" << numA << ".bce";
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
