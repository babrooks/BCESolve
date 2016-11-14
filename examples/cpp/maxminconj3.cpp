#include "bce.hpp"

class MyMech : public BCEAbstractGame
{
private:
  double a;
  double scale;
  double midpnt;
public:
  MyMech(int na):
    BCEAbstractGame(2,na,1,3),
    a(0.0689716096279091),
    scale(-log(a)),
    midpnt(0.5*(1.0-1.0/log(a)))
  {
    
  }

  double prior(int state, const vector<int> & types) const
  {
    return 0.5;
  }

  double Psi(double x) const
  {
    return scale*(pow(a,x)*(pow(log(a),2.0)*pow(midpnt-x,2.0)+2*log(a)*(midpnt-x)+2))/pow(log(a),3);
  }
  double h(double x) const
  { return scale*(x-midpnt)*(x-midpnt);
      //(x-b)*(x-c)*(x-d);
  }
    
  double objective(int state, const vector<int> & actions,
		   int obj) const
  {
    vector<double> m(2,0);
    for (int p=0; p<2; p++)
      m[p] = static_cast<double>(actions[p])/(numActions[p]-1.0);
    vector<double> q(m);
    if (actions[0]+actions[1]>=numActions[0]-1)
      {
	for (int p=0; p<2; p++)
	  q[p] = 0.5*(1+m[p]-m[1-p]);
      }
    double minq = min(q[0],q[1]);
    double maxq = max(q[0],q[1]);
    vector<double> t(2,0);

    double gamma0 = a*(1-log(a))+1.0/log(a);
    double gamma1 = a*(1-log(a))-1.0/log(a);
    double mu = -1.0/log(a);
    for (int p = 0; p < 2; p++)
      {
	t[p] = gamma0*0.5*(1.0-pow(a,-m[p]));
	if (actions[p] + actions[1-p] >= numActions[p]-1)
	  t[p] = 0.5*gamma0*(pow(a,1-m[0]-m[1])-pow(a,-m[p]))
	    + 0.5*(a*(1-log(a))+h(m[1-p]))*(1-pow(a,1-m[0]-m[1]))
	    - 0.5*log(a)*pow(a,-m[p])*(Psi(m[p])-Psi(1-m[1-p]));
	
      }

    if (obj<2)
      {
	return q[obj]*static_cast<double>(state)-t[obj];
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
  int numA = 200;
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
      ss << "maxminconj3_" << numA << ".bce";
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
