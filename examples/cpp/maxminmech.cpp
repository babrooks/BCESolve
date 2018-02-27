// Computes the welfare set for the maxmin mechanism of Brooks and Du (2018)
#include "bce.hpp"
#include <istream>

class MyMech : public BCEAbstractGame
{
private:
  int na;
  int ns;
  vector< vector<double> > Q;
  vector< vector<double> > T;
  
public:
  MyMech(int _ns,
	 int _na,
	 const vector< vector<double> > & _Q,
	 const vector< vector<double> > & _T):
    BCEAbstractGame(_ns,_na,1,3),
    na(_na),
    ns(_ns),
    Q(_Q),
    T(_T)
  {
  }

  double prior(int state, const vector<int> & types) const
  { 
    return 1.0/numStates; // uniform distribution
  }

  double objective(int state, const vector<int> & actions,
		   int obj) const
  {
    double v=(1.0*state)/(numStates-1.0);

    vector<double> q(2,0), t(2,0);
    for (int player = 0; player < numPlayers; player++)
      {
	q[player] = Q[actions[player]][actions[1-player]];
	t[player] = T[actions[player]][actions[1-player]];
      }
    if (obj<2)
      {
	return v*q[obj]-t[obj];
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
  int numS = 30;
  int numA = 30;

  ifstream fs;
  fs.open("maxminmech_a=30.txt");

  vector< vector<double> > Q(numA, vector<double> (numA,0));
  vector< vector<double> > T(Q);

  for (int a1 = 0; a1 < numA; a1++)
    for (int a0 = 0; a0 < numA; a0++)
      fs >> Q[a0][a1];
  for (int a1 = 0; a1 < numA; a1++)
    for (int a0 = 0; a0 < numA; a0++)
      fs >> T[a0][a1];
  fs.close();
  
  MyMech mech(numS,numA,Q,T);

  BCESolver solver(mech);

  try
    {
      solver.populate();
      vector<double> objWeights(3,0);
      objWeights[2]=1;
      solver.solve(objWeights);

      // cout << "Social surplus: "
      // 	   << ( solver.getObjectiveFunction(0).getValue()
      // 		+ solver.getObjectiveFunction(1).getValue()
      // 		+ solver.getObjectiveFunction(2).getValue() ) << endl;
      
      stringstream ss;
      ss << "maxrevinfo_" << numA << ".bce";
      BCESolution soln;
      solver.getSolution(soln);
      BCESolution::save(soln,ss.str().c_str());

      // Weights are set, by default, to 0.
      vector<vector<double> > mapBoundaryWeights(2,vector<double>(3,0));
      // Sets objective 0 weights for the x-axis
      mapBoundaryWeights[0][0]=1;
      // Sets objective 1 weights for the x-axis
      mapBoundaryWeights[0][1]=1;
      // Sets objective 2 weights on the y-axis
      mapBoundaryWeights[1][2]=1;      

      solver.setMinAngleIncr(5e-2);
      // solver.mapBoundary("maxminbndry.dat",mapBoundaryWeights);

    }
  catch (GRBException & e)
    {
      cout << e.getMessage() << endl;
    }
  catch (BCEException & e)
    {
      cout << "BCE Exception caught: " << e.what() << endl;
    }

  return 0;
}
