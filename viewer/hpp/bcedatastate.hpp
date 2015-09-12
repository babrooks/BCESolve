#ifndef BCEDATASTATE_HPP
#define BCEDATASTATE_HPP

#include "bcedata.hpp"
#include "bceenumeration.hpp"
#include <QtWidgets>
#include <cmath>

class BCEDataState : public QWidget
{
  Q_OBJECT;
  friend class BCEWindow;

private:
  vector<int> actions;
  vector<int> types;
  vector<int> values;
  int state;
  BCEData data;
  bool margS0, margS1;
  vector<bool> margA, margT;
  int objective0 = 0,objective1 = 1;
  // player 0 and player 1 surplus, CAN BE CHANGED
  int currentEqmIndex;
  bool isPrivateVals;
  vector<vector<double>> equilibriumMatrix;
  vector<vector<double>> objectiveValues;
  vector<vector<double>> allEqm;

signals:

  void valueChanged(int val,BCESliderType st,int player);
  void devPlotTitleChange(int player,
			  int aciton,
			  int type,
			  double payoff);
  void devPlotPrChange(double probability);
  void eqmCoordSignal(double xCoord,double yCoord);
  void newStateSignal(int value0,
		      int value1,
		      int state,
		      bool privateVals);
  void loadDataSignal(bool privateVals,int player);

public:

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

  } // Default Constructor

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

  void getObjectiveVals(int player) {

    data.getDeviationObjectives(player,actions[player],types[player],objectiveValues);

    emit(devPlotTitleChange(player,
			    actions[player],
			    types[player],
			    objectiveValues[player][actions[player]]));


    emit(devPlotPrChange(equilibriumMatrix[actions[0]][actions[1]]));

  }

  void getObjectiveMatrix() {

    data.getExpectedObjectives(allEqm);

    double xClosest = 0;
    double yClosest = 0;

    xClosest = allEqm[currentEqmIndex][objective0];
    yClosest = allEqm[currentEqmIndex][objective1]; 

    emit(eqmCoordSignal(xClosest,yClosest));

  }

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

  double euclidianDistance(double x1,double x2,double y1,double y2) {
    return sqrt(pow((y2-y1),2)+pow((x2-x1),2));
  }

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

  void resetManipulatedData(BCESliderType st,int player) {
    switch(st) {
    case Action: getObjectiveVals(player);
      break;
    case Type: getObjectiveVals(player);
      break;
    case State: getEqmMatrix();
    }
  }

  void resetManipulatedData() {
    getObjectiveMatrix();
    getEqmMatrix();
  }

};

#endif
