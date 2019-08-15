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

#include "bcedatastate.hpp"

BCEDataState::BCEDataState():
  sliderGroup(0),lineEditGroup(0),allEqm(2,vector<double>(2,0)),
  equilibriumMatrix(0,vector<double>(0))
{
  isDataLoaded = false;
  actions = vector<int>(2,0);
  types = vector<int>(2,0);
  values = vector<int>(2,0);
  state = 0;
  margS0 = false;
  margS1 = false;
  margA = vector<bool>(2,true);
  margT = vector<bool>(2,false);
  currentEqmIndex = 0;
  isPrivateVals=false;
  setupControlsLayout();
  conditionedOnState = true;
  isMapped = false;
}

int BCEDataState::shareDataProperties(BCESliderType st,int player) {
  switch(st) {
  case Action: return gameData.getNumActions()[player];
    break;
  case Type: return gameData.getNumTypes()[player];
    break;
  case State: return gameData.getNumStates();
    break;
  }
}

void BCEDataState::setSolutionData(const BCESolution &solution,
				   const bool isBoundaryMapped) {
  try{
    solutionData = BCESolution(solution);
    gameData = BCEGame(solutionData.getGame());

    isPrivateVals = !(gameData.hasProductStructure());
    isMapped = isBoundaryMapped;

    // Reset Initial Parameters
    currentEqmIndex = 0;
    actions = vector<int>(2,0);
    types = vector<int>(2,0);
    values = vector<int>(2,0);
    state = 0;

    for (int player = 0; player < 2; player++) //flag
      emit(sliderLabelsChanged(isPrivateVals,player));

    resetManipulatedData();
    emit(newDataLoaded());

    vector<int> numActions = gameData.getNumActions();
    vector<int> numTypes = gameData.getNumTypes();
    int numStates = gameData.getNumStates();

    // Set Slider Ranges
    for (int player = 0; player < 2; player++) {
      sliderGroup[3*player]->setRange(0,numActions[player]-1);
      sliderGroup[3*player+1]->setRange(0,numTypes[player]-1);
      if (isPrivateVals)
	sliderGroup[3*player+2]->setRange(0,numStates-1);
      else
	sliderGroup[3*player+2]->setRange(0,gameData.getNumPrivateStates()[player]-1);
    }
    // Set Sliders to 0
    for (int i = 0; i < 6; i++) {
      sliderGroup[i]->setSliderPosition(0);
      sliderGroup[i]->setSingleStep(1);
      lineEditGroup[i]->setText("0");
    }
  }
  catch (BCEException & e)
    {
      emit(sendException(QString::fromStdString(e.getMessage())));
    }
  catch (std::exception & e)
    {
      string str(e.what());
      emit(sendException(QString::fromStdString(str)));
    }
  isDataLoaded = true;
}

void BCEDataState::setupControlsLayout() {

  // Slider, LineEdit, and CheckBox Controls Creation

  QSizePolicy sp(QSizePolicy::MinimumExpanding,QSizePolicy::Expanding);
  sp.setVerticalStretch(1);
  sp.setHorizontalStretch(2);

  QSizePolicy sp1(QSizePolicy::Preferred,QSizePolicy::Expanding);
  sp1.setVerticalStretch(1);
  sp1.setHorizontalStretch(1);

  for (int player = 0; player < 2; player++) {
    sliderGroup.push_back(new BCESlider(Action,player));
    sliderGroup.back()->setSizePolicy(sp);
    sliderGroup.push_back(new BCESlider(Type,player));
    sliderGroup.back()->setSizePolicy(sp);
    sliderGroup.push_back(new BCESlider(State,player));
    sliderGroup.back()->setSizePolicy(sp);

    lineEditGroup.push_back(new BCELineEdit(Action,player));
    lineEditGroup.back()->setSizePolicy(sp1);
    lineEditGroup.push_back(new BCELineEdit(Type,player));
    lineEditGroup.back()->setSizePolicy(sp1);
    lineEditGroup.push_back(new BCELineEdit(State,player));
    lineEditGroup.back()->setSizePolicy(sp1);
  }

  for (int widgetIndex = 0; widgetIndex < 6; widgetIndex++) {
    lineEditGroup[widgetIndex]->setReadOnly(true);
    sliderGroup[widgetIndex]->setOrientation(Qt::Horizontal);
    connect(sliderGroup[widgetIndex],
	    SIGNAL(valueChanged(int,BCESliderType,int)),
	    this,SLOT(setSliderValue(int,BCESliderType,int)));
    connect(this,
	    SIGNAL(valueChanged(int,BCESliderType,int)),
	    lineEditGroup[widgetIndex],
	    SLOT(changeDisplayValue(int,BCESliderType,int)));
    connect(this,
	    SIGNAL(valueChanged(int,BCESliderType,int)),
	    sliderGroup[widgetIndex],
	    SLOT(changeSliderPosition(int,BCESliderType,int)));
  }

  // Slider Labels

  QVector<BCELabel*> sliderLabels;

  QSizePolicy sp2(QSizePolicy::Expanding,QSizePolicy::Expanding);
  sp2.setVerticalStretch(3);
  sp2.setHorizontalStretch(2);

  for (int player = 0; player < 2; player++) {
    sliderLabels.push_back(new BCELabel(SliderLabel,Action,player));
    sliderLabels.back()->setSizePolicy(sp2);
    sliderLabels.push_back(new BCELabel(SliderLabel,Type,player));
    sliderLabels.back()->setSizePolicy(sp2);
    sliderLabels.push_back(new BCELabel(SliderLabel,State,player));
    sliderLabels.back()->setSizePolicy(sp2);
  }


  for (int labelIt = 0; labelIt < 6; labelIt++) {
    connect(this,SIGNAL(sliderLabelsChanged(bool,int)),
	    sliderLabels[labelIt],SLOT(displayStateOrValues(bool,int)));
  }

  // Slider, LineEdit, and CheckBox Layout Creation

  QVector<QHBoxLayout*> gridSubLayouts;
  QVector<QVBoxLayout*> subLayoutWithLabels;
  QGridLayout *controlsGrid = new QGridLayout();

  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < 3; j++) {
      gridSubLayouts.push_back(new QHBoxLayout());
      subLayoutWithLabels.push_back(new QVBoxLayout());
      gridSubLayouts[3*i+j]->addWidget(sliderGroup[3*i+j]);
      gridSubLayouts[3*i+j]->addWidget(lineEditGroup[3*i+j]);
      subLayoutWithLabels[3*i+j]->addWidget(sliderLabels[3*i+j]);
      subLayoutWithLabels[3*i+j]->addLayout(gridSubLayouts[3*i+j]);
      controlsGrid->addLayout(subLayoutWithLabels[3*i+j],j,i); // Layout Matrix
    }
  }

  controlsLayout = new QWidget(this);
  controlsLayout->setLayout(controlsGrid);

  // End Slider, LineEdit, CheckBox Creation
  //////////////////////////////////////////
}

void BCEDataState::setSliderValue(int value,
				  BCESliderType st,
				  int player) {
  if (isDataLoaded == false)
    return;

  switch(st) {
  case Action: actions[player] = value;
    break;
  case Type: types[player] = value;
    break;
  case State: {
    values[player] = value;
    if (isPrivateVals)
      state = value;
    else
      state = values[0]+gameData.getNumPrivateStates()[0]*values[1];
    emit(newStateSignal(values[0],values[1],state,isPrivateVals));
  }
    break;
  }

  /* Handles adjustment of read-only line-edits
     and changing two sliders at once for private-values */
  if (isPrivateVals == true && st == State) {
    for (int playerIt=0; playerIt < gameData.getNumPlayers(); playerIt++)
      emit(valueChanged(value,st,playerIt));
  }
  else
    emit(valueChanged(value,st,player));

  // Changes data in the gui must be changed.
  resetManipulatedData(st,player);
}

void BCEDataState::resetManipulatedData(BCESliderType st,int player) {
  switch(st) {
  case Action: setObjectiveVals(player);
    break;
  case Type: setObjectiveVals(player);
    break;
  case State: setEqmMatrix();
    break;
  }
}

void BCEDataState::resetManipulatedData() {
  setAllEqm();
  setEqmMatrix();
  for (int player = 0; player < 2; player++) //flip
    setObjectiveVals(player);
}

void BCEDataState::modifyEqmFocus(double x,double y) {

  if (isDataLoaded==false)
    return;

  int newEqmIndex = 0;
  double smallestEuclidianDistance = 1e10; // This just needs to be large
  double currentEuclidianDistance;
  for (int i = 0; i < allEqm[0].size(); i++) {
    currentEuclidianDistance = euclidianDistance(x,allEqm[0][i],
						 y,allEqm[1][i]);

    if (currentEuclidianDistance < smallestEuclidianDistance) {
      newEqmIndex = i;
      smallestEuclidianDistance = currentEuclidianDistance;
    }
  }

  currentEqmIndex = newEqmIndex;
  solutionData.setCurrentEquilibrium(currentEqmIndex);

  double xClosest = allEqm[0][currentEqmIndex];
  double yClosest = allEqm[1][currentEqmIndex];

  emit(eqmCoordSignal(xClosest,yClosest));

  // resetManipulatedData();

  setEqmMatrix();
  for (int player = 0; player < 2; player++)//flip
    setObjectiveVals(player);

  emit(selectedEqmChanged());
  emit(eqmCoordSignal(allEqm[0][currentEqmIndex],allEqm[1][currentEqmIndex]));

}

void BCEDataState::setEqmMatrix() {

  vector<double> distribution;

  // State Conditions

  vector<int> stateConditions(0);
  if (conditionedOnState)
    stateConditions = vector<int>(1,state);

  // Action Conditions

  vector< vector<int> > actionConditions(2, vector<int>(0));

  // Type Conditions

  vector< vector<int> > typeConditions(2,vector<int>(0));

  double prob
    = solutionData.getConditionalMarginal(stateConditions,
					  actionConditions,
					  typeConditions,
					  margS0,
					  margA,
					  margT,
					  distribution);

  int numActions_total = solutionData.getGame().getNumActions()[0]
    * solutionData.getGame().getNumActions()[1];
  if (numActions_total != distribution.size())
    throw(BCEException(BCEException::WrongDistnSize));

  vector<int> numActions = gameData.getNumActions();

  equilibriumMatrix = vector< vector<double> >(numActions[0],
					       vector<double>(numActions[1],0));

  for (int a1 = 0; a1 < numActions[1]; a1++)
    {
      for (int a0 = 0; a0 < numActions[0]; a0++)
	{
	  int index = a1 * numActions[0] + a0;
	  equilibriumMatrix[a0][a1] = distribution[index];
	} // for a0
    } // for a1

  // for (int player=0; player<2; player++)
  //   emit(devPlotPrChange(player,gameData.prior(player,types[player])));
  emit(equilibriumMatrixChanged());

}

void BCEDataState::setObjectiveVals(int player) {

    // int mvp = 0;
    // if (player > 1)
    // {
    //     player = player - 2;
    //     mvp = 2;
    // }

  double prob = solutionData.getDeviationObjectives(player,
						    actions[player],
						    types[player],
						    objectiveValues);

  double i = solutionData.getConstraintMultipliers(player,
                             actions[player],
                             types[player],
                             multiplierValues);

  // if (mvp > 1)
  // {
  emit(devPlotTitleChange(player,
			  actions[player],
			  types[player],
			  objectiveValues[player][actions[player]],
              multiplierValues[player][actions[player]]));
  //second one determines payoff also which player #


  // }
  // else
  // {
  // }
  emit(devPlotPrChange(player,prob));
  emit(objectiveValuesChanged(player));

}

void BCEDataState::setAllEqm() {

  vector<vector<double> > allEqmTemp;
  solutionData.getExpectedObjectives(allEqmTemp);

  int numObjs = gameData.getNumObjectives();

  vector<vector<double> > unadjMapBWeights = solutionData.getMapBoundaryWeights();
  vector<double> mainObjectiveWeights = solutionData.getMainObjectiveWeights();
  bool isMapped = solutionData.getIsBoundaryMapped();
  vector<vector<double> > mapBWeights(2,vector<double>(numObjs,0));
  int sizeEqmData = allEqmTemp.size();

  allEqm.clear();
  allEqm.push_back(vector<double>(sizeEqmData,0));
  allEqm.push_back(vector<double>(sizeEqmData,0));

  cout << isMapped << endl;
  if (!isMapped) {
    for (int eqm=0; eqm<sizeEqmData; eqm++) {
      allEqm[0][eqm] = allEqmTemp[eqm][0];
      allEqm[1][eqm] = allEqmTemp[eqm][1];
    }
    cout << allEqm[0][0] << endl;
    cout << allEqm[1][0] << endl;
  }
  else if (isMapped) {

    vector<double> summedWeights(2,0);
    summedWeights[0] = 0;
    summedWeights[1] = 0;

    // Sum the weights
    for (int obj=0; obj<numObjs; obj++) {
      summedWeights[0] += unadjMapBWeights[0][obj];
      summedWeights[1] += unadjMapBWeights[1][obj];
    }

    // Rescale weights so that they sum to 1
    for (int i=0; i<2; i++) {
      for (int obj=0; obj<numObjs; obj++) {
	mapBWeights[i][obj] = unadjMapBWeights[i][obj]/summedWeights[i];
      }
    }

    vector<vector<double> > eqmAccumulator(2,vector<double>(sizeEqmData,0));

    if (isMapped) {
      for (int eqm=0; eqm<sizeEqmData; eqm++) {
	double xVal = 0;
	double yVal = 0;
	int objCounter = 0;
	while (objCounter < numObjs) {
	  xVal += mapBWeights[0][objCounter]*allEqmTemp[eqm][objCounter];
	  yVal += mapBWeights[1][objCounter]*allEqmTemp[eqm][objCounter];
	  objCounter++;
	}
	eqmAccumulator[0][eqm] = xVal;
	eqmAccumulator[1][eqm] = yVal;
      }
    }

    for (int eqm=0; eqm<sizeEqmData; eqm++) {
      allEqm[0][eqm] = eqmAccumulator[0][eqm];
      allEqm[1][eqm] = eqmAccumulator[1][eqm];

    }
  }

  emit(selectedEqmChanged());
  emit(eqmCoordSignal(allEqm[0][currentEqmIndex],allEqm[1][currentEqmIndex]));

}
