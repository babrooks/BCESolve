#include "bce.hpp"

class MyMech : public BCEAbstractGame
{
private:
  double a;
public:
  MyMech(int na):
    BCEAbstractGame(2,na,1,3),
    a(0.068972)
  {
    
  }

  double prior(int state, const vector<int> & types) const
  { return 0.5; }

  double u(double s) const
  { return (log(a/(1-s))-a/(1-s)+1.0)/log(a); }

  double objective(int state, const vector<int> & actions,
		   int obj) const
  {
    vector<double> s(2);
    for (int p=0; p<2; p++)
      s[p] = (1.0-a)*actions[p]/(numActions[p]-1.0);


    double x = 0;
    
    if (obj<2)
      {
	if ((1-s[0])*(1-s[1])>a)
	  x= (static_cast<double>(state)*log(1-s[obj])/log(a)
	      + a/log(a)/(1.0-s[1-obj])*(1.0/(1.0-s[obj])-1.0));
	else if (u(s[0])+u(s[1])>1e-9) // make probability of allocation proportional to u
	  x= (static_cast<double>(state)-1.0)*(u(s[1-obj])
					       +(1.0-u(s[obj])-u(s[1-obj]))*0.5)
	    + u(s[1-obj]);
	else
	  x=0;
      }
    else if (obj==2)
      {
	if ((1-s[0])*(1-s[1])>a)
	  x = (-a/log(a)*(2.0/(1-s[0])/(1-s[1])-1.0/(1-s[0])-1.0/(1-s[1])));
	else
	  x = 1.0-u(s[0])-u(s[1]);
      }


    if (x>1e6)
      cout << "x was bigger than 1e6" << endl;
    if (isnan(x) )
      {
	cout << "x was NaN" << endl;
	cout << s[0] << " " << s[1] << endl;
	cout << obj << endl;
      }
    return x;
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
