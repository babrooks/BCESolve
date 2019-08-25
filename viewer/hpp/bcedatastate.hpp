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

#ifndef BCEDATASTATE_HPP
#define BCEDATASTATE_HPP

#include "bcesolution.hpp"
#include "bcegame.hpp"
#include "bceabstractgame.hpp"
#include "bceenumeration.hpp"
#include "bceslider.hpp"
#include "bcelineedit.hpp"
#include "bcelabel.hpp"
#include <QtWidgets>
#include <QMainWindow>
#include <vector>
#include <cmath>
#include <boost/filesystem.hpp>

//! Class for storing and manipulating solution data used by the plot handler.
/*! Continuously manipulated as user interacts with solution tab controls.
  Holds all data for plotting. Also holds parameters the
  user supplies through solutions tab controls, such as the currently selected equilibrium.
  Contains private "set" methods for changing private data.
  Contains public "get" methods that make minor integer or string copies
  or supply a const reference to the memory location
  of larger data structures. These get methods are called
  primarily by BCEPlothandler.

  \ingroup viewer
*/
class BCEDataState : public QMainWindow
{
  Q_OBJECT;

signals:

  //! Signal that data for the set of BCE plot has been changed.
  /*! Connected to the plotter for the BCE value set plot
    in BCEWindow (top left plot in the solution tab).
  */
  void selectedEqmChanged();
  //! Signal that new data has been loaded.
  /*! Connected to changing the title of the graphical
    user interface. The GUI gets the file name from the
    path and displays it in the GUI's title bar.

    Also connected to a slot notifying the program that data has
    been loaded. Certain functions in the program will become
    activated and functional. If these functions were used
    before data was loaded, the program would crash.

    These functions include toggleLinearScale(_) and
    toggleColorfulTheme(_) in the BCEPlothandler.
   */
  void newDataLoaded();
  //! Signal for a changed slider value.
  /*! Connected to BCESlider and BCELineEdit. Slots serve
    to change read-only line edits when a slider has been
    moved. Also, for common value/state games, moves both
    state sliders simultaneously upon user interaction
    with one.
  */
  void valueChanged(int val,BCESliderType st,int player);
  //! Signals data for changing deviation bar plots' titles.
  /*! Connected to BCELabel. Slot serves to change
    QLabel information displayed about players' actions and
    types in the GUI's deviation plot titles.
  */
  void devPlotTitleChange(int player,
			  int action,
			  int type,
			  double payoff,
              double multiplier);
  //! Signals data for changing deviation bar plots' titles.
  /*! Connected to BCELabel. Slot changes QLabel info
    displayed about the probability of a player's action.
  */
  void devPlotPrChange(int player,double probability);
  //! Signals coordinates of the current equilibrium.
  /*! Connected to BCELabel. Slot changes
    coordinates of equilibrium displayed in the GUI's
    Set of BCE plot title (top left plot in the solution tab).
  */
  void eqmCoordSignal(double xCoord,double yCoord);
  //! Signals changed state information and type of state (common or private).
  /*! Connected to BCELabel. Slot changes
    information displayed about each player's state (in
    the private-states case) or the state of the
    game (in the common values case) in the GUI's
    heat map plot title (right plot in the solution tab).
  */
  void newStateSignal(int value0,
		      int value1,
		      int state,
		      bool privateVals);
  //! Signals that a slider control has been moved by the user.
  /*! Connected to BCELabel. Slot tells state slider
    labels to display "Player 0's Values" and "Player 1's
    Values" (in the private-states case) or "Common State"
    (in the common-state case).
  */
  void sliderLabelsChanged(bool privateVals,int player);
  //! Signals to the solution tab that objectiveValues have been manipulated.
  /*! Connected to a slot handling plotting of the deviation
    plots in the solution tab (lower left bar plots). This signal
    is emitted when objective values are changed, and data is
    replotted.
   */
  void objectiveValuesChanged(int player);
  //! Signals to the solution tab that equilibriumMatrix has been manipulated.
  /*! Connected to a slot handling plotting of the equilibria player payoffs
    in the solution tab (top left plot). This is emitted when the equilibrium
    matrix is changed by this class, and the data is then plotted in the solution
    tab.
   */
  void equilibriumMatrixChanged();
  //! Sends an exception message to BCEWindow to be displayed
  void sendException(QString message);


private: // Private Properties. Private Functions near EOF.
  //! Notes if data is currently loaded into the GUI.
  /*! Prevents program from crashing if user clicks on
    GUI controls before any data is loaded.
   */
  bool isDataLoaded;
  //! Gives if boundary was mapped in the solution object.
  bool isMapped;
  //! A 2 element vector holding the current action for each player.
  vector<int> actions;
  //! A 2 element vector holding the current type for each player.
  vector<int> types;
  //! A 2 element vector holding the current state for each player.
  vector<int> values;
  //! Holds the current state of a common-states game.
  int state;
  //! If true, GUI is displaying solution of a private-states game.
  bool isPrivateVals;
  //! Currently active BCESolution in the GUI.
  BCESolution solutionData;
  //! Currently active BCEGame in the GUI.
  BCEGame gameData;
  //! File name of example currently displayed in the GUI.
  string guiTitle;
  //! If true, GUI displays marginal distribution for player 0's state.
  bool margS0;
  //! If true, GUI displays marginal distribution for player 1's state.
  bool margS1;
  //! If true, GUI displays marginal distribution for player's action.
  /*! 2 element vectors holding whether marginal distributions for
    actions are displayed. Not currently in use.
  */
  vector<bool> margA;
  //! If true, GUI displays marginal distribution for player's type.
  /*! 2 element vectors holding whether marginal distributions for
    types are displayed. Not currently in use.
  */
  vector<bool> margT;
  //! Index of equilibrium currently displayed in the GUI.
  int currentEqmIndex;
  //! Data displayed in the GUI's heatmap.
  vector< vector<double> > equilibriumMatrix;
  //! Data displayed in the GUI's objective bar plots.
  vector< vector<double> > objectiveValues;
  //! Data displayed in the GUI's multiplier bar plots.
  vector< vector<double> > multiplierValues;
  //! Data displayed in the GUI's BCE Value Set Plot.
  /*! Holds information about all the equilibria in a saved
    example file. If allEqm contains more than one
    equilibrium, BCESolver's mapBoundary function has been
    called in the example's .cpp file.
  */
  vector< vector<double> > allEqm;
  //! Vector of Slider Controls
  /*! Each element is identified by slider
    type and player. There are 2 players and
    3 slider types (Action, Type, State). The
    size of the vector is 6.
  */
  QVector<BCESlider*> sliderGroup;
  //! Vector of Line Edits
  /*! Displays value of corresponding slider.
    Each element is identified by slider
    type and player. There are 2 players and
    3 slider types (Action, Type, State). The
    size of the vector is 6.
  */
  QVector<BCELineEdit*> lineEditGroup;
  //! Tells whether equilibriumMatrix is conditioned on State.
  bool conditionedOnState;

public:

  //! Default constructor.
  /*! Provides initial conditions for several private
     properties: actions, types, values, state, margS0,
     margS1, margA, margT, currentEqmIndex, isPrivateVals.
     Further connects signals and slots when both are
     contained within this class.
  */
  BCEDataState();

  //! Loads new data into the GUI.
  /*! Receives emitted signal that there is a new data
    path from BCEWindow. This is called when the user
    interacts with the loadAction within the GUI's file menu.
    This function extracts the filename from the path for use
    in the GUI's title. It also loads data at the path into
    the class's privately stored "data" member. It then
    determines if the game has private values and sets initial
    conditions for a new load (i.e. actions, types, values, and
    state are reset to 0). Emits that
  */
  void setSolutionData(const BCESolution &solution, const bool isBoundaryMapped);

  //! Layout Holding Sliders and Other Data Controls
  QWidget *controlsLayout;

  //! Sends map boundary weights to the plot handler.
  const vector<vector<double> >& getMapBWeights() const {
    return solutionData.getMapBoundaryWeights();
  }

  //! Sends the number of objectives to the plot handler.
  const int getNumObjs() const {
    gameData.getNumObjectives();
  }

  //! Shares basic game data (e.g. numActions) with BCEWindow.
  /*! Returns basic properties of currently stored BCEData
    for setting slider ranges. Given Action, Type, or State,
    returns number of actions, types, or states for the
    given player. This is always an integer value.
  */
  int shareDataProperties(BCESliderType st,int player);

  //! Returns the current eqm index stored in BCEDataState.
  const int getCurrentEqmIndex() const {
    return currentEqmIndex;
  }

  //! Returns current integer value of a slider.
  /*! Given whether a slider controls Action, Type, or
     State, returns the integer value at the current
     position of the slider for the given player.
  */
  const int getCurrentSliderVal(BCESliderType st,int player) const {
    switch(st) {
    case Action: return actions[player];
      break;
    case Type: return types[player];
      break;
    case State: return state;
      break;
    }
  }

  //! Shares memory location of equilibriumMatrix with BCEPlotHandler.
  /*! BCEPlotHandler cannot edit BCEDataState's private member
    equilibriumMatrix but needs read access to plot the
    GUI's heatmap.
  */
  const vector< vector<double> >& getEqmMatrix() const {
    return equilibriumMatrix;
  }

  //! Shares memory location of objectiveValues with BCEPlotHandler.
  /*! BCEPlotHandler cannot edit BCEDataState's private member
    objectiveValues but needs read access to plot the GUI's
    bar plots of player's deviation objectives.
  */
  const vector< vector<double> >& getObjectiveValues() const {
    return objectiveValues;
  }

  //! Shares memory location of multiplierValues with BCEPlotHandler.
  /*! BCEPlotHandler cannot edit BCEDataState's private member
    multiplierValues but needs read access to plot the GUI's
    bar plots of player's deviation objectives.
  */
  const vector< vector<double> >& getMultiplierValues() const {
    return multiplierValues;
  }

  //! Shares memory location of allEqm with BCEPlotHandler.
  /*! BCEPlotHandler cannot edit BCEDataState's private member
    allEqm but needs read access to plot the set of equilibria.
  */
  const vector< vector<double> >& getAllEqm() const {
    return allEqm;
  }

  //! Shares the current BCESolution by reference.
  /*! Used to save the current solution. saveGame() in
    BCEWindow calls this function to serialize and save
    a BCESolution.
  */
  const BCESolution& getSolution() const {
    return solutionData;
  }

  //! Shares file name of currently loaded file with BCEWindow.
  const string getGUITitle() const {
    return guiTitle;
  }

  //! Alters whether data in equilibriumMatrix is conditional on state.
  void setConditionOnState(bool conditioned) {
    conditionedOnState = conditioned;
    setEqmMatrix();
  }

public slots:

  //! Changes stored slider data.
  /*! Called when a slider has been moved or after a load action.
     Sets data equal to the new slider value and emits information
     used by read-only line-edits and slider titles.
  */
  void setSliderValue(int value,
		      BCESliderType st,
		      int player);

  //! Manipulates data after user interaction with controls or new load action.
  /*! Sets new data for some plots in the solution tab. If given
    BCESliderType::Action, sets new deviation
    objectives for the given player. If given BCESliderType::Type,
    sets new deviation objectives for the given player. If given
    BCESliderType::State, sets new data for the heatmap.
  */
  void resetManipulatedData(BCESliderType st,int player);

  //! Calls all "set" data functions.
  /*! Called after user interaction with
    the plot of equilibria player payoffs, this helper function
    sets new data for every plot in the solution tab.
  */
  void resetManipulatedData();

  //! Changes current equilibrium index for stored BCEData.
  /*! Switches equilibrium indices. Takes in coordinates of a
    click on the plot, minimizes Euclidian distance to find
    the closest equilibrium, and sets the index of that
    equilibrium as the current equilibrium index.
  */
  void modifyEqmFocus(double x,double y);

private: //functions

  //! Resets heatmap data.
  /*! Replaces data stored in "equilibriumMatrix."
    Also emits a signal containing data for title of the
    plot displaying this data.
  */
  void setEqmMatrix();

  //! Resets deviation bar plot data.
  /*! Replaces data in "objectiveValues."
    Also, emits that bar plot titles need to be changed
    and contains relevant data for those changes.
  */
  void setObjectiveVals(int player);
  //! Resets constraint bar plot data.
  /*! Replaces data in "multiplierValues."
    Also, emits that bar plot titles need to be changed
    and contains relevant data for those changes.
  */
  void setMultiplierVals(int player);

  //! Resets BCE Value Set Plot data.
  /*! Sets payoffs for BCE Value Set Plot. Called after
    switching equilibrium indices or after a load action.
  */
  void setAllEqm();

  //! Helper function for modifyEqmFocus
  /*! Calculates distance between two points.
   */
  double euclidianDistance(double x1,double x2,double y1,double y2) {
    return sqrt(pow((y2-y1),2)+pow((x2-x1),2));
  }

  //! Builds the controls layout in the solution tab.
  void setupControlsLayout();

}; // BCEDataState

#endif
