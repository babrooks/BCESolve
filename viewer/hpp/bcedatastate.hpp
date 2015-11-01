#ifndef BCEDATASTATE_HPP
#define BCEDATASTATE_HPP

#include "bcesolution.hpp"
#include "bcegame.hpp"
#include "bceabstractgame.hpp"
#include "bceenumeration.hpp"
#include "bceslider.hpp"
#include "bcelineedit.hpp"
#include "bcecheckbox.hpp"
#include "bcelabelhandler.hpp"
#include <QtWidgets>
#include <QMainWindow>
#include <vector>
#include <cmath>
#include <boost/filesystem.hpp>

//! Class for storing data used by the GUI.
/*! Continuously manipulated after user interactions.
  Holds all data for plotting. Also holds conditions the 
  user must supply. Contains private "set" methods.
  BCEWindow retrieves data through public "get"
  methods that make minor integer or string copies
  or supply a const reference to the memory location
  of larger data structures.

  \ingroup viewer
*/
class BCEDataState : public QObject
{
  Q_OBJECT;

signals:

  //! Signal that data for the set of BCE plot has been changed.
  /*! Connected to the plotter for the BCE value set plot
    in BCEWindow.
  */
  void selectedEqmChanged();
  //! Signal that new data has been loaded.
  /*! Connected to changing the title of the graphical 
    user interface. The GUI gets the file name from the
    path and displays it in the GUI's title bar.
   */
  void newDataLoaded();
  //! Signal for a changed slider value.
  /*! Connected to BCESlider and BCELineEdit. Slots serve 
    to change read-only line edits when a slider has been
    moved. Also, for private-value games, moves both 
    state sliders simultaneously upon user interaction
    with one (one of the state sliders is redundant
    for private-value games).
  */
  void valueChanged(int val,BCESliderType st,int player);
  //! Signals data for changing deviation bar plots' titles.
  /*! Connected to BCEDevPlotTitle. Slot serves to change
    QLabel information displayed about players' actions and
    types in the GUI's deviation plot titles.
  */
  void devPlotTitleChange(int player,
			  int aciton,
			  int type,
			  double payoff);
  //! Signals data for changing deviation bar plots' titles.
  /*! Connected to BCEDevPlotTitle. Slot changes QLabel info
    displayed about the probability of a player's action.
  */
  void devPlotPrChange(int player,double probability);
  //! Signals coordinates of the current equilibrium.
  /*! Connected to BCEValueSetPlotTitle. Slot changes 
    coordinates of equilibrium displayed in the GUI's
    Set of BCE plot title.
  */
  void eqmCoordSignal(double xCoord,double yCoord);
  //! Signals what kind of state has been changed.
  /*! Connected to BCEHeatMapTitle. Slot changes 
    information displayed about player values (in 
    the non private-values case) or the state of the
    game (in the private-values case) in the GUI's
    heat map plot title.
  */
  void newStateSignal(int value0,
		      int value1,
		      int state,
		      bool privateVals);
  //! Signals that new data has been loaded.
  /*! Connected to BCESliderLabel. Slot tells state slider
    labels to display "Player 0's Values" and "Player 1's
    Values" (in the non private-values case) or "Unified State"
    (in the private-values case).
  */
  void sliderLabelsChanged(bool privateVals,int player);
  //! Signals that objectiveValues have been manipulated.
  void objectiveValuesChanged(int player);
  //! Signals that equilibriumMatrix has been manipulated.
  void equilibriumMatrixChanged();


private: // Private Properties. Private Functions near EOF.
  //! A 2 element vector holding the current action for each player.
  vector<int> actions;
  //! A 2 element vector holding the current type for each player.
  vector<int> types;
  //! A 2 element vector holding the current value for each player.
  vector<int> values;
  //! Holds the number of states.
  int state;
  //! If true, GUI is displaying solution of a private-values game.
  bool isPrivateVals;
  //! Currently active BCESolution in the GUI.
  BCESolution solutionData;
  //! Currently active BCEGame in the GUI.
  BCEGame gameData;
  //! File name of example currently displayed in the GUI.
  string guiTitle;
  //! If true, GUI displays marginal distribution for player's state.
  bool margS0, margS1;
  //! If true, GUI displays marginal distribution for player's action/type.
  /*! 2 element vectors holding whether marginal distributions for 
    actions or type are displayed. Not currently in use.
  */
  vector<bool> margA, margT;
  //! Equilibrium objectives for mapBoundary in BCEGame.
  /* Equilibrium objectives displayed, currently set to player
     0 and player 1 maximizing surplus.
  */
  int objective0 = 0,objective1 = 1;
  //! Index of equilibrium currently displayed in the GUI.
  int currentEqmIndex;
  //! Data displayed in the GUI's heatmap.
  vector<vector<double>> equilibriumMatrix;
  //! Data displayed in the GUI's bar plots.
  vector<vector<double>> objectiveValues;
  //! Data displayed in the GUI's BCE Value Set Plot.
  /*! Holds information about all the equilibria in a saved
    example file. If allEqm contains more than one
    equilibrium, BCEGame's mapBoundary function has been
    called in the example .cpp file.
  */
  vector<vector<double>> allEqm;
  //! Vector of Controls Check Boxes
  QVector<BCECheckBox*> checkBoxGroup;
  //! Vector of Controls Sliders
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

public:

  //! Default constructor.
  /*! Provides initial conditions for several private
     properties: actions, types, values, state, margS0,
     margS1, margA, margT, currentEqmIndex, isPrivateVals.
     Further connects signals and slots when both are 
     contained within this class.
  */
  BCEDataState();

  int resWidth = 1920;
  int resHeight = 1080;

  //! Layout Holding Sliders and Other Data Controls
  QGridLayout *controlsLayout;

  //! Shares basic game data (e.g. numActions) with BCEWindow.
  /*! Returns basic properties of currently stored BCEData
    for setting slider ranges. Given Action, Type, or State,
    returns number of actions, types, or states for the
    given player. This is always an integer value. 
  */
  int shareDataProperties(BCESliderType st,int player);

  //! Returns the current eqm index stored in BCEDataState.
  int getCurrentEqmIndex() {
    return currentEqmIndex;
  }

  //! Returns current integer value of a slider.
  /*! Given whether a slider controls Action, Type, or
     State, returns the integer value at the current
     position of the slider for the given player.
  */
  int getCurrentSliderVal(BCESliderType st,int player) {
    switch(st) {
    case Action: return actions[player];
      break;
    case Type: return types[player];
      break;
    case State: return state;
      break;
    }
  }

  //! Returns currently set player objectives.
  /*! Currently returns 0 for player 0 and 1
    for player 1. These correspond to players
    maximizing their own supluses.
  */
  int getPlayerObjective(int player) {
    switch(player) {
    case 0: return objective0;
      break;
    case 1: return objective1;
      break;
    }
  }

  //! Shares memory location of equilibriumMatrix with BCEWindow.
  /*! BCEWindow cannot edit BCEDataState's private member
    equilibriumMatrix but needs read access to plot the 
    GUI's heatmap.
  */
  const vector<vector<double>>& getEqmMatrix() const {
    return equilibriumMatrix;
  }

  //! Shares memory location of objectiveValues with BCEWindow.
  /*! BCEWindow cannot edit BCEDataState's private member
    objectiveValues but needs read access to plot the GUI's
    bar plots of player's deviation objectives.
  */
  const vector<vector<double>>& getObjectiveValues() const {
    return objectiveValues;
  }

  //! Shares memory location of allEqm with BCEWindow.
  /*! BCEWindow cannot edit BCEDataState's private member
    allEqm but needs read access to plot the GUI's
    BCE Value Set Plot.
  */
  const vector<vector<double>>& getAllEqm() const {
    return allEqm;
  }

  //! Shares file name of currently loaded file with BCEWindow.
  string getGUITitle() {
    return guiTitle;
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
  void setData(QString dataPath);

  //! Not currently in use.
  /*! May be implemented to view certain marginal distributions
    in the heatmap. Connected to GUI's checkboxes.
  */
  void setMarginalConditions(bool newBool,BCESliderType st,int player);

  //! Calls "set" data  functions for specific cases.
  /*! Called within the BCEWindow class after user interaction with
    sliders, this helper function manipulates relevant data before 
    plotting. If given BCESliderType::Action, sets new deviation
    objectives for the given player. If given BCESliderType::Type,
    sets new deviation objectives for the given player. If given
    BCESliderType::State, sets new equilibrium matrix of probabilities. 
  */
  void resetManipulatedData(BCESliderType st,int player);
  
  //! Calls all "set" data functions.
  /*! Called within the BCEWindow class after user interaction with
    the BCE Value Set Plot, this helper function manipulates relevant 
    data before plotting. 
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

  void setControlsLayout(QGridLayout *layout) {
    controlsLayout = layout;
  }

  void setupControlsLayout();

}; // BCEDataState

#endif
