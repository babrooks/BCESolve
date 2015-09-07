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
  int a0, a1, t0, t1, v0, v1, s;
  BCEData data;
  bool margS0, margS1;
  vector<bool> margA, margT;
  int objective0 = 0,objective1 = 1;
  // player 0 and player 1 surplus, CAN BE CHANGED
  int currentEqmIndex=0;
  bool isPrivateVals;

signals:

  void valueChanged(int val,BCESliderType st,int player);

public:

  BCEDataState() {
    a0 = 0;
    a1 = 0;
    t0 = 0;
    t1 = 0;
    v0 = 0;
    v1 = 0;
    s = 0;
    BCEData data;
    margS0 = false;
    margS1 = false;
    margA = {true,true};
    margT = {false,false};
    isPrivateVals=false;

  } // Default Constructor

  int shareDataProperties(int index) {
    switch(index) {
    case 0: return data.numActions[0];
      break;
    case 1: return data.numTypes[0];
      break;
    case 2: {
      if (isPrivateVals == true) 
	return data.numStates;
      else
	return data.numValues[0];
    }
      break;
    case 3: return data.numActions[1];
      break;
    case 4: return data.numTypes[1];
      break;
    case 5: {
      if (isPrivateVals == true)
	return data.numStates;
      else
	return data.numValues[1];
    }
      break;
    }
  }

  void setData(const char* pathToData) {
    try
      {
	BCEData::load(data,pathToData);

	isPrivateVals = !(data.isPrivateValues);
	std::cout << isPrivateVals << std::endl;

      }
    catch (std::exception & e)
      {
	qDebug() << "Load solution didnt work :(" << endl;
      }

  }

  vector<vector<double>> getEqmMatrix() {

    vector<double> distribution;

    // State Conditions

    vector<int> stateConditions(1,s);
    if (isPrivateVals == false)
      stateConditions = vector<int>(1,v0+v1*data.numValues[0]); 

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

    vector<vector<double>> equilibriumMatrix(data.numActions[1],
					     vector<double>(data.numActions[0],0));

    for (int a1 = 0; a1 < data.numActions[1]; a1++)
      {
	for (int a0 = 0; a0 < data.numActions[0]; a0++)
	  {
	    int index = a1 * data.numActions[0] + a0;
	    equilibriumMatrix[a1][a0] = distribution[index];
	  } // for a0
      } // for a1

    return equilibriumMatrix;

  }

  vector<vector<double>> getObjectiveVals(int player) {

    vector<vector<double>> objectiveValues;

    if (player==0)
      data.getDeviationObjectives(player,a0,t0,objectiveValues);
    else if (player==1)
      data.getDeviationObjectives(player,a1,t1,objectiveValues);

    return objectiveValues; 

  }

  vector<vector<double>> getObjectiveMatrix() {

    vector<vector<double>> allEqmObjectives;
    data.getExpectedObjectives(allEqmObjectives);

    return allEqmObjectives;

  }

  void modifyEqmFocus(double x,double y) {

    vector<vector<double>> allEqm;
    data.getExpectedObjectives(allEqm);

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
    case Action: {
      if (player==0)
	a0 = value;
      else 
	a1 = value;
    }
      break;
    case Type: {
      if (player==0)
	t0 = value;
      else 
	t1 = value;
    }
      break;
    case State: {
      if (isPrivateVals == true) {
	s = value;
	v0 = value;
	v1 = value;
      }
      else {
	if (player==0)
	  v0 = value;
	else 
	  v1 = value;
      }
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

};

#endif
