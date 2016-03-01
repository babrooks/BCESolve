// This file is part of the BCESolve library for games of incomplete
// information
// Copyright (C) 2016 Benjamin A. Brooks, Robert J. Minton
// 
// BCESolve free software: you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// BCESolve is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see
// <http://www.gnu.org/licenses/>.
// 
// Benjamin A. Brooks
// ben@benjaminbrooks.net
// Chicago, IL

#ifndef BCESOLVERWORKER_HPP
#define BCESOLVERWORKER_HPP

#include "bceabstractgame.hpp"
#include "bcegame.hpp"
#include "bce.hpp"
#include <QObject>
#include "bcegurobicallback.hpp"

//! Class for solving games created in the game tab.
/*! Contains a game and a solution object. When
  the solverWorker is created, a BCEGame is supplied in its
  constructor. The solverWorker solves the game, and the
  private member "solution" is set to the solution of the solved
  game. A signal sends a pointer to the solution to the BCEWindow
  object, which sends the solution to BCEPlotHandler for plotting.
  BCEWindow also automatically switches the tab currently displayed
  to the solution window after the solution has been plotted.

  \ingroup viewer
 */
class BCESolverWorker : public QObject {
  Q_OBJECT;

private:

  //! Game
  BCEGame game;
  //! Solution, either default or the custom game (see class description).
  BCESolution solution;
  //! Weights on the objectives, as supplied by the user in the game tab.
  vector<double> weightData;
  //! Callback, allows communication with Gurobi solver.
  BCEGurobiCallback * callback; 

public:

  //! Constructor
  BCESolverWorker(BCEGame _game,
			vector<double> _weightData,
			BCEGurobiCallback * _callback):
    game(_game), weightData(_weightData), callback(_callback)
  {}

  //! Returns a reference to the solution object.
  const BCESolution& getSolution() const {
    return solution;
  }

public slots:

  //! Triggered when the user clicks the "Solve" button in the game tab.
  /*! Uses GUROBI and BCESolver to solve a BCEGame. Objectives are
    multiplied by their respective weights, and then the sum of these
    objectives is maximized. The weights should sum, in magnitude, to 1.
    If weights are negative, then the solver is maxing the negative of an 
    objective, i.e. minimizing. 
   */
  void startSolve() {

    BCESolver solver(game);

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
    solver.model.setCallback(callback);

    solver.solve();

    solver.getSolution(solution);

    emit(sendSolution(&solution));
    emit(workFinished());

  }

signals:

  //! Signals that the solve routine has ended.
  void workFinished();
  //! Signals a pointer to the solution found by the solver.
  void sendSolution(BCESolution *soln);

};

#endif

