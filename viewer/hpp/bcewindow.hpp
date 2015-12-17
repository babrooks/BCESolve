#ifndef BCEWINDOW_HPP
#define BCEWINDOW_HPP

#include <QMainWindow>
#include <QtWidgets>
#include "bcegamehandler.hpp"
#include "bceplothandler.hpp"
#include "bceloghandler.hpp"

//! Assembles BCEGameHandler, BCEPlotHandler, and BCESolverWorker into a Unified GUI.
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
  BCEWindow();

signals:

  //! Sends path to example to BCEDataState.
  /*! Gets path to example data from user interaction.
    Sends that path to BCEDataState in order to load
    new data.
  */
  void dataPathChanged(QString newPath);

private:

  //! Contains all information for the game tab.
  BCEGameHandler gameTab;
  //! Contains all information for the solution tab.
  BCEPlotHandler solutionTab;
  //! Contains all information for the log tab.
  BCELogHandler logTab;
  //! Default Path to data
  QString path;
  //! Thread for solverWorker
  QThread solverWorkerThread;

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

};

#endif
