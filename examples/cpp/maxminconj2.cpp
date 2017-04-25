#include "bce.hpp"

class MyMech : public BCEAbstractGame
{
private:
  double A;
  double X;
public:
  MyMech(int na,double _A, double _X):
    BCEAbstractGame(2,na,1,3),
    A(_A), X(_X)
  {  }

  double prior(int state, const vector<int> & types) const
  { 
    return 0.5;
  }

  double objective(int state, const vector<int> & actions,
		   int obj) const
  {
    vector<double> s(2), t(2);
    for (int p=0; p<2; p++)
      {
	s[p] = actions[p]/(numActions[p]-1.0);
	t[p] = X*(exp(s[p]/A)-1.0);
      }

    double v = static_cast<double>(state);

    double mins = min(s[0],s[1]),
      maxs = max(s[0],s[1]);
    
    double highq = maxs-mins/2;
    double lowq = mins/2;
    
    if (obj<2)
      {
	if (actions[obj] > actions[1-obj])
	  return (v*highq-t[obj]);
	else if (actions[obj] == actions[1-obj])
	  return (v*(highq+lowq)/2.0-t[obj]);
	else
	  return (v*lowq-t[obj]);
      }
    else if (obj==2)
      {
	return t[0]+t[1];
      }

    return 0.0;
  }
  
};

int main()
{
  int numA = 50;


  // R = 0.212
  double X = 0.1;
  double A = 0.6; 
    

  X = 0.0410;
  A = 9.4795;
  // double X = 0.09;
  // double A = 0.59; 

  MyMech mech(numA,A,X);

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
