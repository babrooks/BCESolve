#include "bce.hpp"
#include <math.h>

class EntryGame : public BCEAbstractGame
{
private:
  int numValues;
  double delta;

public:
  EntryGame(int nv,double _delta):
    BCEAbstractGame(nv*nv,2,nv,5),
    numValues(nv),
    delta(_delta)
  {}

  int stateToPrivateValues(int state, vector<int> &values) const
  {
    values.resize(2);

    values[0]=state%numValues;
    state-=values[0]; 
    values[1]=state/numValues;
  }

  double prior (int state, const vector<int> & types) const
  {
    vector<int> values;
    stateToPrivateValues(state,values);

    if (types[0]==values[0]
	&& types[1]==values[1])
      return 1.0/(numValues*numValues);
    return 0.0;
  } // prior

  double objective(int state, const vector<int> & actions,
		   int obj) const
  {
    if (obj<2)
      {
	vector<int> values;
	stateToPrivateValues(state,values);

	double value = 2.0*values[obj]/(numValues-1.0)-1.0;
	if (actions[obj]==1 && actions[1-obj]==0)
	  return value;
	else if (actions[obj]==1)
	  return value+delta;
    
	return 0.0;
      }
    else if (obj<4)
      {
	// Return probability of player obj-2 entering
	return actions[obj-2]==1 && actions[3-obj]==0;
      }
    else if (obj==4)
      return ( (actions[0]==1) && (actions[1]==1) );
  } // objective

}; // Entry Game

int main ()
{
  std::ofstream ofs("entrygame.txt",std::ofstream::out);

  int numVals = 50;
  double delta = 0.5;

  EntryGame game(numVals,delta);
  
  BCESolver solver(game);
  solver.populate();

  IloCplex cplex = solver.getCplex();

  solver.setParameter(BCESolver::DisplayLevel,0);

  cplex.getObjective().setSense(IloObjective::Maximize);

  int K = 30;
  double incr = PI/K;
  for (double theta = -PI/2.0; theta < PI/2.0; theta+=incr)
    {
      // We want constant arc length of 1/K. Adjust the number of
      // points on each lvl to match this target.
      double r = cos(theta);
      double circ = 2*PI*r*r; // Circumference on the ring

      int littleK = r*r*K;
      double littleIncr = PI/littleK;
      for (double rho = 0; rho < 2*PI; rho+=littleIncr)
	{
	  double p10 = cos(theta)*cos(rho);
	  double p01 = cos(theta)*sin(rho);
	  double p11 = sin(theta);
	  
	  cplex.getObjective().setExpr(p10*solver.getObjectiveFunction(2)
				       +p01*solver.getObjectiveFunction(3)
				       +p11*solver.getObjectiveFunction(4));
	  solver.solve();

	  // ofs << p10 << " " << p01 << " " << p11 << endl;
	  ofs << cplex.getValue(solver.getObjectiveFunction(2)) << " "
	      << cplex.getValue(solver.getObjectiveFunction(3)) << " "
	      << cplex.getValue(solver.getObjectiveFunction(4))
	      << endl;
	} // rho
    } // theta
  
  ofs.close();

  return 0;
};

