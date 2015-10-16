#ifndef BCEDATASTATE_HPP
#define BCEDATASTATE_HPP

#include "bcesolution.hpp"
#include "bcegame.hpp"
#include "bceabstractgame.hpp"
#include "bceenumeration.hpp"
#include <QtWidgets>
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
class BCEDataState : public QWidget
{
  Q_OBJECT;

private: // Private Properties. Private Functions near EOF.
  //! A 2 element vector holding the current action for each player.
  vector<int> actions;
  //! A 2 element vector holding the current type for each player.
  vector<int> types;
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
  //! Layout Holding Sliders and Other Data Controls
  QVBoxLayout *controlsLayout;

public:

  //! Default constructor.
  /*! Provides initial conditions for several private
     properties: actions, types, values, state, margS0,
     margS1, margA, margT, currentEqmIndex, isPrivateVals.
     Further connects signals and slots when both are 
     contained within this class.
  */
  BCEDataState() {
    actions = vector<int>(2,0);
    types = vector<int>(2,0);
    state = 0;
    margS0 = false;
    margS1 = false;
    margA = {true,true};
    margT = {false,false};
    currentEqmIndex = 0;
    isPrivateVals=false;
    connect(this,SIGNAL(sliderDataChanged(BCESliderType,int)),
	    this,SLOT(resetManipulatedData(BCESliderType,int)));
    connect(this,SIGNAL(selectedEqmChanged()),
	    this,SLOT(resetManipulatedData()));
    connect(this,SIGNAL(newDataLoaded()),
	    this,SLOT(resetManipulatedData()));
  }

  //! Shares basic game data (e.g. numActions) with BCEWindow.
  /*! Returns basic properties of currently stored BCEData
    for setting slider ranges. Given Action, Type, or State,
    returns number of actions, types, or states for the
    given player. This is always an integer value. 
  */
  int shareDataProperties(BCESliderType st,int player) {
    switch(st) {
    case Action: return gameData.getNumActions[player];
      break;
    case Type: return gameData.getNumTypes[player];
      break;
    case State: return gameData.getNumStates;
      break;
    }
  }

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

  const QVBoxLayout& getLayout() const {
    return controlsLayout;
  }


public slots:

  //! Changes stored slider data.
  /*! Called when a slider has been moved or after a load action.
     Sets data equal to the new slider value and emits information
     used by read-only line-edits and slider titles.
  */
  void setSliderValue(int value,
		     BCESliderType st,
		     int player) {
    switch(st) {
    case Action: actions[player] = value;
      break;
    case Type: types[player] = value;
      break;
    case State: {
      state = value;
      emit(newStateSignal(values[0],values[1],state,isPrivateVals));
    }
      break;
    }

    /* Handles adjustment of read-only line-edits
       and changing two sliders at once for private-values */
    if (isPrivateVals == true && st == State) {
      for (int playerIt=0; playerIt < data.numPlayers; playerIt++)
	emit(valueChanged(value,st,playerIt));
    }
    else 
      emit(valueChanged(value,st,player));

    // Signals that manipulated data in the gui must be changed.
    emit(sliderDataChanged(st,player));

  }

  //! Not currently in use.
  /*! May be implemented to view certain marginal distributions
    in the heatmap. Connected to GUI's checkboxes.
  */
  void setMarginalConditions(bool newBool,BCESliderType st,int player) {
    switch(st) {
    case Action:
      margA[player]=newBool;
      break;
    case Type: 
      margT[player]=newBool;
      break;
    case State: {
      if (player==0)
	margS0 = newBool;
      else 
	margS1 = newBool;
    }
      break;
    }
  }

  //! Calls above functions for specific cases.
  /*! Called within the BCEWindow class after user interaction with
    sliders, this helper function manipulates relevant data before 
    plotting. If given BCESliderType::Action, sets new deviation
    objectives for the given player. If given BCESliderType::Type,
    sets new deviation objectives for the given player. If given
    BCESliderType::State, sets new equilibrium matrix of probabilities. 
  */
  void resetManipulatedData(BCESliderType st,int player) {
    switch(st) {
    case Action: setObjectiveVals(player);
      break;
    case Type: setObjectiveVals(player);
      break;
    case State: setEqmMatrix();
    }
  }
  
  //! Calls above functions for a specific case.
  /*! Called within the BCEWindow class after user interaction with
    the BCE Value Set Plot, this helper function manipulates relevant 
    data before plotting. 
  */
  void resetManipulatedData() {
    setAllEqm();
    setEqmMatrix();
    for (int player = 0; player < 2; player++)
      setObjectiveVals(player);
  }

  //! Changes current equilibrium index for stored BCEData.
  /*! Switches equilibrium indices. Takes in coordinates of a
    click on the plot, minimizes Euclidian distance to find
    the closest equilibrium, and sets the index of that 
    equilibrium as the current equilibrium index.
  */
  void modifyEqmFocus(double x,double y) {

    int newEqmIndex = 0;
    double smallestEuclidianDistance = 5000.0000; // This just needs to be large
    double currentEuclidianDistance;
    for (int i = 0; i < allEqm.size(); i++) {
      currentEuclidianDistance = euclidianDistance(x,allEqm[i][objective0],
						   y,allEqm[i][objective1]);

      if (currentEuclidianDistance < smallestEuclidianDistance) {
	newEqmIndex = i;
	smallestEuclidianDistance = currentEuclidianDistance;
      }
    }

    currentEqmIndex = newEqmIndex;
    solutionData.setCurrentEquilibrium(currentEqmIndex);

    double xClosest = 0;
    double yClosest = 0;

    xClosest = allEqm[currentEqmIndex][objective0];
    yClosest = allEqm[currentEqmIndex][objective1]; 

    emit(eqmCoordSignal(xClosest,yClosest));

    emit(selectedEqmChanged());

  }

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
  void setData(QString dataPath) {
    try
      {
	QByteArray ba = dataPath.toLocal8Bit();
	char * newPath_c = ba.data();

	// Get File Name for GUI's Title
	string filePath = dataPath.toStdString();
	boost::filesystem::path boostPath(filePath);
        guiTitle = boostPath.filename().string();

	// Load New Data on Path
	BCEGame::load(gameData,newPath_c);

	isPrivateVals = !(data.isPrivateValues);

	currentEqmIndex = 0;
	emit(selectedEqmChanged());
	actions = vector<int>(2,0);
	types = vector<int>(2,0);
	values = vector<int>(2,0);
	state = 0;

	for (int player = 0; player < 2; player++)
	  emit(sliderLabelsChanged(isPrivateVals,player));

	emit(newDataLoaded());

      }
    catch (std::exception & e)
      {
	qDebug() << "Load solution didnt work :(" << endl;
      }
  }

signals:

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
  //! Signals that stored slider data has been changed.
  void sliderDataChanged(BCESliderType st,int player);
  //! Signals that the equilibrium index has been changed.
  void selectedEqmChanged();
  //! Signals that objectiveValues have been manipulated.
  void objectiveValuesChanged(int player);
  //! Signals that equilibriumMatrix has been manipulated.
  void equilibriumMatrixChanged();
  //! Signals that a load action has occurred.
  void newDataLoaded();

private: //functions

  //! Resets heatmap data.
  /*! Replaces data stored in "equilibriumMatrix."
    Also emits a signal containing data for title of the
    plot displaying this data.
  */
  void setEqmMatrix() {

    vector<double> distribution;

    // State Conditions

    vector<int> stateConditions(1,state);

    // Action Conditions

    vector<vector<int>> actionConditions(2, vector<int>(0));

    // Type Conditions

    vector<vector<int>> typeConditions(2,vector<int>(0));

    double prob
      = solutionData.getConditionalMarginal(stateConditions,
				    actionConditions, 
				    typeConditions,
				    margS0,
				    margA,
				    margT,
				    distribution);

    assert(solutionData.numActions_total == distribution.size());

    vector<int> numActions = gameData.getNumActions;

    equilibriumMatrix = vector<vector<double>>(numActions[1],
					       vector<double>(numActions[0],0));

    for (int a1 = 0; a1 < numActions[1]; a1++)
      {
	for (int a0 = 0; a0 < numActions[0]; a0++)
	  {
	    int index = a1 * numActions[0] + a0;
	    equilibriumMatrix[a0][a1] = distribution[index];
	  } // for a0
      } // for a1

    // Gets marginal probability of an action from the joint distn.
    double acc0 = 0.0;
    for (int a = 0; a < numActions[0]; a++)
      acc0 += equilibriumMatrix[actions[0]][a];

    double acc1 = 0.0;
    for (int a = 0; a < numActions[1]; a++)
      acc1 += equilibriumMatrix[a][actions[1]];

    emit(devPlotPrChange(0,acc0));
    emit(devPlotPrChange(1,acc1));
    emit(equilibriumMatrixChanged());

  }

  //! Resets deviation bar plot data.
  /*! Replaces data in "objectiveValues."
    Also, emits that bar plot titles need to be changed
    and contains relevant data for those changes.
  */
  void setObjectiveVals(int player) {

    solutionData.getDeviationObjectives(player,actions[player],types[player],objectiveValues);

    emit(devPlotTitleChange(player,
			    actions[player],
			    types[player],
			    objectiveValues[player][actions[player]]));

    // Gets marginal probability of an action from the joint distn.
    vector<int> numActions = gameData.getNumActions;
    double acc = 0.0;
    for (int a = 0; a < numActions[1-player]; a++) {
      if (player == 0)
	acc += equilibriumMatrix[actions[player]][a];
      else
	acc += equilibriumMatrix[a][actions[player]];
    }

    emit(devPlotPrChange(player,acc));
    emit(objectiveValuesChanged(player));

  }

  //! Resets BCE Value Set Plot data.
  /*! Sets payoffs for BCE Value Set Plot. Called after 
    switching equilibrium indices or after a load action.
  */
  void setAllEqm() {

    solutionData.getExpectedObjectives(allEqm);

  }

  //! Helper function for modifyEqmFocus
  /*! Calculates distance between two points.
   */
  double euclidianDistance(double x1,double x2,double y1,double y2) {
    return sqrt(pow((y2-y1),2)+pow((x2-x1),2));
  }

  void setupControlsLayout() {

  }

}; // BCEDataState

#endif
