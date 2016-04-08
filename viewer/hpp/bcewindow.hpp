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

#ifndef BCEWINDOW_HPP
#define BCEWINDOW_HPP

#include <QMainWindow>
#include <QtWidgets>
#include "bcegamehandler.hpp"
#include "bceplothandler.hpp"
#include "bceloghandler.hpp"
#include "bcesolverworker.hpp"
#include "hybridentryreserve.hpp"
#include "commonvalueauction.hpp"

//! Assembles BCEGameHandler, BCEPlotHandler, and BCELogHandler into a Unified GUI.
/*! Sets up a menu bar and assembles bcegamehandler, bceplothandler, 
  and bcesolverworker objects into a tab widget. This tab
  widget is set as the layout of the GUI. Contains methods
  for loading a solution (loads game and solution) 
  or loading a game (loads just game). Contains similar methods
  for saving games and solutions, though these are saved
  independently.
 */
class BCEWindow : public QMainWindow
{
  Q_OBJECT;

public:
  //! Constructor
  BCEWindow(BCELogHandler &logHandler);
  //! Destructor
  ~BCEWindow() {
    delete callback;
  }

signals:

  //! Sends path to example to BCEDataState.
  /*! Gets path to example data from user interaction.
    Sends that path to BCEDataState in order to load
    new data.
  */
  void dataPathChanged(QString newPath);
  //! Signals the callback object to cancel optimization.
  void setCancelFlag();

private:

  //! Contains all information for the game tab.
  BCEGameHandler *gameTab;
  //! Contains all information for the solution tab.
  BCEPlotHandler *solutionTab;
  //! Pointer to the logTab in main.cpp
  BCELogHandler *logTab;
  //! Widget holding layouts for game, solution, and log tabs.
  QTabWidget *tabWidget;
  //! Default Path to data
  QString path;
  //! Default path to gui screenshots (*.png files).
  /*! Currently set as ../examples/screenshots/
   */
  QString screenShotPath;
  //! Thread for solverWorker
  QThread *solverWorkerThread;
  //! solverWorker Object for Solving Games
  BCESolverWorker *solverWorker;
  //! Callback object for communicating with solver.
  BCEGurobiCallback *callback;

private slots:

  //! Interacts with BCEPlotHandler to load new data.
  /*! Sends the new data path to BCEDataState to load
    the new data. Resets sliders and line-edits to 0.
    Sets range of sliders according to the number of
    actions, types, and states in the new data.
  */
  void loadSolution();
  //! Interacts with BCEGameHandler and BCEPlotHandler to load a new game.
  void loadGame();
  //! Interacts with BCEGameHandler and BCEPlotHandler to save a solution.
  void saveSolution();
  //! Interacts with BCEGameHandler and BCEPlotHandler to save a game.
  void saveGame();
  //! Interacts with BCEGameHandler and BCESolverWorker to solve a game.
  void runSolve();
  //! Switches the current tab to the solution tab.
  void tabToSolution(BCESolution *soln);
  //! Takes a screenshot and opens a save file menu.
  /* The .png file name ending must be entered manually
     upon saving for proper save action functionality.
  */
  void screenshot();
  //! Starts the routine to build the default hybrid auction.
  void generateHybridAuction();
  //! Builds the default common values auction.
  void generateCommonValueAuction();
  //! Displays exceptions signaled from other classes.
  void displayException(QString message);

};

#endif
