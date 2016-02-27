#ifndef BCEGUROBISOLVERWORKER_HPP
#define BCEGUROBISOLVERWORKER_HPP

#include "bceabstractgame.hpp"
#include "bcegame.hpp"
#include "bce.hpp"
#include <QObject>
#include "bcegurobisolver.hpp"
#include "bcegurobicallback.hpp"

//! Class for solving games created in the game tab.
/*! Contains a game and a solution object. When
  the solverWorker is created, a BCEGame is supplied in its
  constructor. Once the solve routine has completed, the
  default solution is replaced with the solution from
  the game.

  \ingroup viewer
 */
class BCEGurobiSolverWorker : public QObject {
  Q_OBJECT;

private:

  //! Game
  BCEGame game;
  //! Solution, either default or to the game (see class description).
  BCESolution solution;
  //! Weights on the objectives, as supplied by the user in the game tab.
  vector<double> weightData;
  //! Callback, allows communication with solver.
  BCEGurobiCallback callback; 

public:

  //! Constructor
  BCEGurobiSolverWorker(BCEGame _game,
			vector<double> _weightData,
			BCEGurobiCallback &_callback):
    game(_game), weightData(_weightData), callback(_callback)
  {}

  //! Returns a reference to the solution object.
  BCESolution& getSolution() {
    return solution;
  }

public slots:

  //! Triggered when the user clicks the "Solve" button in the game tab.
  /*! Uses CPLEX and BCESolver to solve a BCEGame. Objectives are
    multiplied by their respective weights, and then the sum of these
    objectives is maximized. The weights should sum, in magnitude, to 1.
    If weights are negative, then the solver is maxing the negative of an 
    objective.
   */
  void startSolve() {

    BCEGurobiSolver solver(game);

    solver.populate();

    GRBLinExpr expr = weightData[0]*solver.getObjectiveFunction(0);
    int numObjs = game.getNumObjectives();

    if (numObjs == 2) {
      expr += weightData[1]*solver.getObjectiveFunction(1);
    }

    else if (numObjs > 2) {
      for (int obj = 1; obj < numObjs; obj++)
	expr += weightData[obj]*solver.getObjectiveFunction(obj);
    }

    solver.model.setObjective(expr,GRB_MAXIMIZE);
    solver.model.setCallback(&callback);

    solver.solve();

    solver.getSolution(solution);

    emit(sendSolution(&solution));
    emit(workFinished());

  }

signals:

  //! Signals that the solve routine has ended.
  void workFinished();
  //! Signals the solution found by the solver.
  void sendSolution(BCESolution *soln);

};

#endif

