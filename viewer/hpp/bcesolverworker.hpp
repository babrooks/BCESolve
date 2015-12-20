#ifndef BCESOLVERWORKER_HPP
#define BCESOLVERWORKER_HPP

#include "bceabstractgame.hpp"
#include "bcegame.hpp"
#include "bce.hpp"
#include <QObject>
#include <ilcplex/ilocplex.h>

//! Class for solving games created in the GUI.
/*! Contains a game and a solution object. When
  the solverWorker is created, a BCEGame is supplied in its
  constructor. Once the solve routine has completed, the
  default solution is replaced with the solution from
  the game.
 */
class BCESolverWorker : public QObject {
  Q_OBJECT;

private:

  BCEGame game;
  BCESolution solution;

public:
  BCESolverWorker(BCEGame &_game):
    game(_game) 
  {}

  BCESolution& getSolution() {
    return solution;
  }

public slots:

  void startSolve() {

    double minAngleIncrement = 0.05;

    BCESolver solver(game);

    solver.setParameter(BCESolver::MinAngleIncrement,minAngleIncrement);
    solver.setParameter(BCESolver::DisplayLevel,1);

    solver.populate();

    solver.setParameter(BCESolver::BoundaryObjective1,0);
    solver.setParameter(BCESolver::BoundaryObjective2,1);

    IloCplex cplex = solver.getCplex();
    cplex.setOut(std::cout);
 
    for (int player = 0; player < 2; player++)
      {
	for (int val = 0; val < 2; val++)
	  {
	    cplex.getModel()
	      .add(solver.getObjectiveFunction(5+val+player*2)>=0.0);
	  } // val
      }

    cplex.setParam(IloCplex::RootAlg,IloCplex::Barrier);
    cplex.setParam(IloCplex::SimDisplay,0);
    solver.setParameter(BCESolver::DisplayLevel,1);

    cplex.getObjective().setSense(IloObjective::Maximize);
    cplex.getObjective().setExpr(-1.0*(solver.getObjectiveFunction(2)));
    cplex.setParam(IloCplex::Threads,4);
  
    solver.solve();

    cout << "Bidder 1's surplus: " 
	 << cplex.getValue(solver.getObjectiveFunction(0)) << endl;
    cout << "Bidder 2's surplus: "
	 << cplex.getValue(solver.getObjectiveFunction(1)) << endl;
    cout << "Revenue: " 
	 << cplex.getValue(solver.getObjectiveFunction(2)) << endl;
    cout << "Total surplus: " 
	 << cplex.getValue(solver.getObjectiveFunction(3)) << endl;

    solver.getSolution(solution);

    emit(sendSolution(&solution));
    emit(workFinished());

  }

signals:

  void workFinished();
  void sendSolution(BCESolution *soln);

};

#endif

