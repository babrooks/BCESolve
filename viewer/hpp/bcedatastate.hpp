#ifndef BCEDATASTATE_HPP
#define BCEDATASTATE_HPP

#include "bcedata.hpp"
#include "bceenumeration.hpp"
#include <QtWidgets>
#include <cmath>
#include <boost/filesystem.hpp>

//! Class for storing data used by the GUI.
/*! Continuously manipulated upon user interaction.
  Holds all data for plotting and conditions the 
  user must supply.

  \ingroup viewer
*/
class BCEDataState : public QWidget
{
  Q_OBJECT;

private: // Properties. Private Functions near EOF.
  //! A 2 element vector holding the current action for each player.
  vector<int> actions;
  //! A 2 element vector holding the current type for each player.
  vector<int> types;
  //! A 2 element vector holding the current value/state for each player.
  vector<int> values;
  //! State of the game, used for private-value games.
  int state;
  //! Currently active BCEData in the GUI.
  BCEData data;
  //! Path to data.
  QString dataPath;
  //! File name of example currently displayed in the GUI.
  string guiTitle;
  //! If true, GUI displays marginal distribution for player's state.
  bool margS0, margS1;
  //! If true, GUI displays marginal distribution for player's action/type.
  /*! 2 element vectors holding whether marginal distributions for 
    actions or type are displayed.
  */
  vector<bool> margA, margT;
  //! Equilibrium objectives for mapBoundary in BCEGame.
  /* Equilibrium objectives displayed, currently set to players
     maximizing surplus.
  */
  int objective0 = 0,objective1 = 1;
  //! Index of equilibrium currently displayed in the GUI.
  int currentEqmIndex;
  //! If true, GUI is displaying solution of a private-values game.
  bool isPrivateVals;
  //! Data displayed in the GUI's heatmap.
  vector<vector<double>> equilibriumMatrix;
  //! Data displayed in the GUI's bar plots.
  vector<vector<double>> objectiveValues;
  //! Data displayed in the GUI's BCE Value Set Plot.
  vector<vector<double>> allEqm;

public:

  //! Default constructor.
  BCEDataState() {
    actions = vector<int>(2,0);
    types = vector<int>(2,0);
    values = vector<int>(2,0);
    state = 0;
    BCEData data;
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

  //! Loads new data into the GUI.
  void setData() {
    try
      {
	QByteArray ba = dataPath.toLocal8Bit();
	char * newPath_c = ba.data();

	// Get File Name for GUI's Title
	string filePath = dataPath.toStdString();
	boost::filesystem::path boostPath(filePath);
        guiTitle = boostPath.filename().string();

	// Load New Data on Path
	BCEData::load(data,newPath_c);

	isPrivateVals = !(data.isPrivateValues);

	currentEqmIndex = 0;
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

  //! Shares basic game data with BCEWindow.
  /*! Returns basic properties of currently stored BCEData
    for setting slider ranges. Given Action, Type, or State,
    returns number of actions, types, or states for the
    given player. This is always an integer value. 
  */
  int shareDataProperties(BCESliderType st,int player) {
    switch(st) {
    case Action: return data.numActions[player];
      break;
    case Type: return data.numTypes[player];
      break;
    case State: {
      if (isPrivateVals == true) 
	return data.numStates;
      else
	return data.numValues[player];
    }
      break;
    }
  }

  int getCurrentEqmIndex() {
    return currentEqmIndex;
  }

  int getCurrentSliderVal(BCESliderType st,int player) {
    switch(st) {
    case Action: return actions[player];
      break;
    case Type: return types[player];
      break;
    case State: {
      if (isPrivateVals)
	return state;
      else
	return values[player];
    }
      break;
    }
  }

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
		     int player) {
    switch(st) {
    case Action: actions[player] = value;
      break;
    case Type: types[player] = value;
      break;
    case State: {
      if (isPrivateVals == true) {
	state = value;
	values[player] = value;
      }
      else
	values[player] = value;

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
    setObjectiveMatrix();
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
    data.setCurrentEquilibrium(currentEqmIndex);

    double xClosest = 0;
    double yClosest = 0;

    xClosest = allEqm[currentEqmIndex][objective0];
    yClosest = allEqm[currentEqmIndex][objective1]; 

    emit(eqmCoordSignal(xClosest,yClosest));

    emit(selectedEqmChanged());

  }

  void setDataPath(QString newPath) {
    dataPath = newPath;
    setData();
  }

signals:

  //! Signal for a changed slider value.
  void valueChanged(int val,BCESliderType st,int player);
  //! Signals data for changing deviation bar plots' titles.
  void devPlotTitleChange(int player,
			  int aciton,
			  int type,
			  double payoff);
  //! Signals data for changing deviation bar plots' titles.
  void devPlotPrChange(double probability);
  //! Signals coordinates of the current equilibrium.
  void eqmCoordSignal(double xCoord,double yCoord);
  //! Signals what kind of state has been changed.
  void newStateSignal(int value0,
		      int value1,
		      int state,
		      bool privateVals);
  //! Signals that new data has been loaded.
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
  /*! Replaces data stored in "equilibriumMatrix" (function is called 
    after changes in value/state, a load action, or selection of a new 
    equilibrium). Also emits a signal containing data for title of the
    plot displaying this data.
  */
  void setEqmMatrix() {

    vector<double> distribution;

    // State Conditions

    vector<int> stateConditions(1,state);
    if (isPrivateVals == false)
      stateConditions = vector<int>(1,values[0]+values[1]*data.numValues[0]); 

    // Action Conditions

    vector<vector<int>> actionConditions(2, vector<int>(0));

    // Type Conditions

    vector<vector<int>> typeConditions(2,vector<int>(0));

    double prob
      = data.getConditionalMarginal(stateConditions,
				    actionConditions, 
				    typeConditions,
				    margS0,
				    margA,
				    margT,
				    distribution);

    assert(data.numActions_total == distribution.size());

    equilibriumMatrix = vector<vector<double>>(data.numActions[1],
					       vector<double>(data.numActions[0],0));

    for (int a1 = 0; a1 < data.numActions[1]; a1++)
      {
	for (int a0 = 0; a0 < data.numActions[0]; a0++)
	  {
	    int index = a1 * data.numActions[0] + a0;
	    equilibriumMatrix[a0][a1] = distribution[index];
	  } // for a0
      } // for a1

    emit(devPlotPrChange(equilibriumMatrix[actions[0]][actions[1]]));
    emit(equilibriumMatrixChanged());

  }

  //! Resets deviation bar plot data.
  /*! Replaces data in "objectiveValues" 
    (function called after a change in action, type, 
    or equilibrium; also after a load action).
    Also, emits that bar plot titles need to be changed
    and contains relevant data.
  */
  void setObjectiveVals(int player) {

    data.getDeviationObjectives(player,actions[player],types[player],objectiveValues);

    emit(devPlotTitleChange(player,
			    actions[player],
			    types[player],
			    objectiveValues[player][actions[player]]));


    emit(devPlotPrChange(equilibriumMatrix[actions[0]][actions[1]]));
    emit(objectiveValuesChanged(player));

  }

  //! Resets BCE Value Set Plot data.
  /*! Gets payoffs for BCE Value Set Plot. Called after 
    switching equilibrium indices or after a load action.
    Emits a signal containing the coordinates of the new 
    equilibrium for the plot's title.
  */
  void setObjectiveMatrix() {

    data.getExpectedObjectives(allEqm);

  }

  //! Helper function for modifyEqmFocus
  /*! Calculates distance between two points.
   */
  double euclidianDistance(double x1,double x2,double y1,double y2) {
    return sqrt(pow((y2-y1),2)+pow((x2-x1),2));
  }


}; // BCEDataState

#endif
