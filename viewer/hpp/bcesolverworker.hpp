#ifndef BCESOLVERWORKER_HPP
#define BCESOLVERWORKER_HPP

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

  const BCESolution& getSolution() const {
    return solution;
  }

  void startSolve() {

    BCESolver solver(game);

    solver.populate();

    IloCplex cplex = solver.getCplex();
    cplex.setOut(std::cout);
  
    cplex.getObjective().setSense(IloObjective::Minimize);
    cplex.getObjective().setExpr(solver.getObjectiveFunction(2));
  
    solver.solve();

    solver.getSolution(solution);

  }

};

#endif

