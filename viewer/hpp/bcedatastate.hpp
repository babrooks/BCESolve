#ifndef BCEDATASTATE_HPP
#define BCEDATASTATE_HPP

#include "bcedata.hpp"
#include "bceenumeration.hpp"
#include <QtWidgets>
#include <cmath>

//! Class for storing data used by the GUI.
/*! Continuously manipulated upon user interaction.
  Holds all data for plotting and conditions the 
  user must supply.

  \ingroup viewer
*/
class BCEDataState : public QWidget
{
  Q_OBJECT;
  friend class BCEWindow;

private:
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
  void loadDataSignal(bool privateVals,int player);

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

  }

  //! Shares basic data with BCEWindow.
  /*! Returns basic properties of currently stored BCEData
    for setting slider ranges.
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

  //! Loads new data into the GUI.
  void setData(const char* pathToData) {
    try
      {
	BCEData::load(data,pathToData);

	isPrivateVals = !(data.isPrivateValues);

	currentEqmIndex = 0;

	for (int player = 0; player < 2; player++)
	  emit(loadDataSignal(isPrivateVals,player));

      }
    catch (std::exception & e)
      {
	qDebug() << "Load solution didnt work :(" << endl;
      }
  }

  //! Resets heatmap data.
  /*! Replaces data stored in "equilibriumMatrix" (function is called 
    after changes in value/state, a load action, or selection of a new 
    equilibrium). Also emits a signal containing data for title of the
    plot displaying this data.
  */
  void getEqmMatrix() {

    vector<double> distribution;

    // State Conditions

    vector<int> stateConditions(1,state);
    if (isPrivateVals == false)
      stateConditions = vector<int>(1,values[0]+values[1]*data.numValues[0]); 

    // Action Conditions

    vector< vector<int> > actionConditions(2, vector<int>(0));
    // vector< vector<int> > actionConditions(2, vector<int>(1,0));
    // vector<int> player0AConditions(1,a0);
    // vector<int> player1AConditions(1,a1);
    // actionConditions[0] = player0AConditions;
    // actionConditions[1] = player1AConditions;

    // if (a0 == 0 && a1 == 0) {
    //   actionConditions[0].clear();
    //   actionConditions[1].clear();
    // }
    // else if (a0 == 0) 
    //   actionConditions[1].clear();
    // else if (a1 == 0) 
    //   actionConditions[0].clear();

    // Type Conditions

    vector< vector<int> > typeConditions(2,vector<int>(0));
    // vector< vector<int> > typeConditions(2, vector<int>(1,0));
    // vector<int> player0TConditions(1,a0);
    // vector<int> player1TConditions(1,a1);
    // typeConditions[0] = player0TConditions;
    // typeConditions[1] = player1TConditions;

    // if (t0 == 0 && t1 == 0) {
    //   typeConditions[0].clear();
    //   typeConditions[1].clear();
    // }
    // else if (t0 == 0) 
    //   typeConditions[1].clear();
    // else if (t1 == 0) 
    //   typeConditions[0].clear();

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

  }

  //! Resets deviation bar plot data.
  /*! Replaces data in "objectiveValues" 
    (function called after a change in action, type, 
    or equilibrium; also after a load action).
    Also, emits that bar plot titles need to be changed
    and contains relevant data.
  */
  void getObjectiveVals(int player) {

    data.getDeviationObjectives(player,actions[player],types[player],objectiveValues);

    emit(devPlotTitleChange(player,
			    actions[player],
			    types[player],
			    objectiveValues[player][actions[player]]));


    emit(devPlotPrChange(equilibriumMatrix[actions[0]][actions[1]]));

  }

  //! Resets BCE Value Set Plot data.
  /*! Gets payoffs for BCE Value Set Plot. Called after 
    switching equilibrium indices or after a load action.
    Emits a signal containing the coordinates of the new 
    equilibrium for the plot's title.
  */
  void getObjectiveMatrix() {

    data.getExpectedObjectives(allEqm);

    double xClosest = 0;
    double yClosest = 0;

    xClosest = allEqm[currentEqmIndex][objective0];
    yClosest = allEqm[currentEqmIndex][objective1]; 

    emit(eqmCoordSignal(xClosest,yClosest));

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

  }

  //! Helper function for modifyEqmFocus
  /*! Calculates distance between two points.
   */
  double euclidianDistance(double x1,double x2,double y1,double y2) {
    return sqrt(pow((y2-y1),2)+pow((x2-x1),2));
  }

  //! Changes stored slider data.
  /*! Called when a slider has been moved or after a load action.
     Sets data equal to the new slider value and emits information
     used by read-only line-edits and slider titles.
  */
  void setSliderData(int value,
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
      else {
	values[player] = value;
      }
      emit(newStateSignal(values[0],values[1],state,isPrivateVals));
    }
      break;
    }

    if (isPrivateVals == true && st == State) {
      for (int playerIt=0; playerIt < data.numPlayers; playerIt++)
	emit(valueChanged(value,st,playerIt));
    }

    else 
      emit(valueChanged(value,st,player));
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
    plotting.
  */
  void resetManipulatedData(BCESliderType st,int player) {
    switch(st) {
    case Action: getObjectiveVals(player);
      break;
    case Type: getObjectiveVals(player);
      break;
    case State: getEqmMatrix();
    }
  }
  
  //! Calls above functions for a specific case.
  /*! Called within the BCEWindow class after user interaction with
    the BCE Value Set Plot, this helper function manipulates relevant 
    data before plotting. 
  */
  void resetManipulatedData() {
    getObjectiveMatrix();
    getEqmMatrix();
  }

}; // BCEDataState

#endif
