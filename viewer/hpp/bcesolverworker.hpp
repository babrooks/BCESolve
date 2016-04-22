// This file is part of the BCESolve library for games of incomplete
// information
// Copyright (C) 2016 Benjamin A. Brooks and Robert J. Minton
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
#include <QMessageBox>

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
  //! Weights on map boundary objectives, supplied in the game tab.
  vector<vector<double> > mapBoundaryData;
  //! Callback, allows communication with Gurobi solver.
  BCEGurobiCallback * callback; 
  //! True if the solver will be mapping the boundary.
  bool mapBoundaryOption;

public:

  //! Constructor
  BCESolverWorker(BCEGame _game,
		  vector<double> _weightData,
		  vector<vector<double> > _mapBoundaryData,
		  BCEGurobiCallback * _callback,
		  bool _mapBoundaryOption):
    game(_game), weightData(_weightData),
    mapBoundaryData(_mapBoundaryData), callback(_callback),
    mapBoundaryOption(_mapBoundaryOption)
  {}

  //! Returns a reference to the solution object.
  const BCESolution& getSolution() const {
    return solution;
  }

public slots:

  //! Triggered when the user clicks the "Solve" button in the game tab.
  /*! Uses GUROBI and BCESolver to solve a BCEGame. Objectives are
    multiplied by their respective weights, and then the sum of these
    objectives is maximized. 

    If weights are negative, then the solver is maxing the negative of an 
    objective, i.e. minimizing. 
  */
  void startSolve() {

    BCESolver solver(game);

    try {
      solver.populate();

      GRBLinExpr expr = weightData[0]*solver.getObjectiveFunction(0);
      vector<GRBLinExpr> mapBObj(2,0);
      mapBObj[0] = mapBoundaryData[0][0]*solver.getObjectiveFunction(0);
      mapBObj[1] = mapBoundaryData[1][0]*solver.getObjectiveFunction(0);
      int numObjs = game.getNumObjectives();

      if (numObjs == 2) {
	expr += weightData[1]*solver.getObjectiveFunction(1);
	for (int mBObj = 0; mBObj < 2; mBObj++)
	  mapBObj[mBObj] += mapBoundaryData[mBObj][1]*solver.getObjectiveFunction(1);
      }

      else if (numObjs > 2) {
	for (int obj = 1; obj < numObjs; obj++) {
	  expr += weightData[obj]*solver.getObjectiveFunction(obj);
	  for (int mBObj = 0; mBObj < 2; mBObj++)
	    mapBObj[mBObj] += mapBoundaryData[mBObj][obj]*solver.getObjectiveFunction(obj);
	} // for each objective
      }

      solver.model.setObjective(expr,GRB_MAXIMIZE);
      solver.model.setCallback(callback);

      callback->setFullOutput(true);
      solver.solve();

      if (mapBoundaryOption) {
	callback->setFullOutput(false);
	solver.mapBoundary(mapBObj[0],mapBObj[1]);
	callback->setFullOutput(true);
      }

      // 11 signals that the solve routine
      if (solver.model.get(GRB_IntAttr_Status)!=11) {
	solver.getSolution(solution);
	emit(sendSolution(&solution));
      }
    }
    catch(BCEException &e) {
      emit(exceptionSignal(QString::fromStdString(e.getMessage())));
    }
    catch(std::exception &e) {
      string str(e.what());
      emit(exceptionSignal(QString::fromStdString(str)));
    }
    catch(GRBException &e) {
      emit(exceptionSignal(QString::fromStdString(e.getMessage() + 
						  "The error code for this GRBException was: " +
						  to_string(e.getErrorCode()))));
    }
  

    emit(workFinished());

  }

signals:

  //! Signals that the solve routine has ended.
  void workFinished();
  //! Signals a pointer to the solution found by the solver.
  void sendSolution(BCESolution *soln);
  //! Signals an exception thrown during the solve routine.
  void exceptionSignal(QString message);

};

#endif

