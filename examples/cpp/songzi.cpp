#include "bce.hpp"

class MyMech : public BCEAbstractGame
{
private:
  double A;
  double X;
public:
  MyMech(int na):
    BCEAbstractGame(2,na,1,3),
    A(0.470707070707071),
    X(0.0563636363636364)
  {
    
  }

  double prior(int state, const vector<int> & types) const
  { return 0.5; }

  double objective(int state, const vector<int> & actions,
		   int obj) const
  {
    vector<double> s(2);
    for (int p=0; p<2; p++)
      s[p] = actions[p]/(numActions[p]-1.0);


    double x = 0;
    
    if (obj<2)
      {
	double prob = 0;
	if (actions[obj]>actions[1-obj])
	  prob = s[obj]-s[1-obj]/2.0;
	else
	  prob = s[obj]/2.0;
	return (static_cast<double>(state)*prob-X*(exp(s[obj]/A)-1.0));
      }
    else if (obj==2)
      {
	return X*(exp(s[0]/A)-1.0)+X*(exp(s[1]/A)-1.0);
      }
    return 0;
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
      ss << "songzi_" << numA << ".bce";
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
