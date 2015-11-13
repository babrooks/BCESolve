#include "bcedatastate.hpp"

BCEDataState::BCEDataState() {
    actions = vector<int>(2,0);
    types = vector<int>(2,0);
    values = vector<int>(2,0);
    state = 0;
    margS0 = false;
    margS1 = false;
    margA = {true,true};
    margT = {false,false};
    currentEqmIndex = 0;
    isPrivateVals=false;
    setupControlsLayout();  
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

void BCEDataState::setData(QString dataPath) {

    QByteArray ba = dataPath.toLocal8Bit();
    const char* newPath_c = ba.data();

    cout << dataPath.toStdString() << endl;

    // Load New Data on Path
    BCESolution::load(solutionData,newPath_c);
    gameData = BCEGame(solutionData.getGame());

    // Get File Name for GUI's Title
    string filePath = dataPath.toStdString();
    boost::filesystem::path boostPath(filePath);
    guiTitle = boostPath.filename().string();
		
    isPrivateVals = gameData.hasProductStructure();
    cout << isPrivateVals << endl;

    currentEqmIndex = 0;
    actions = vector<int>(2,0);
    types = vector<int>(2,0);
    values = vector<int>(2,0);
    state = 0;

    for (int player = 0; player < 2; player++)
      emit(sliderLabelsChanged(isPrivateVals,player));

    resetManipulatedData();
    emit(newDataLoaded());

    cout << "prob: " << equilibriumMatrix[0][0] << endl;

    vector<int> numActions = gameData.getNumActions();
    cout << numActions[0] << endl;
    vector<int> numTypes = gameData.getNumTypes();
    cout << numTypes[0] << endl;
    int numStates = gameData.getNumStates();
    cout << numStates << endl;

    for (int player = 0; player < 2; player++) {
      sliderGroup[3*player]->setRange(0,numActions[player]-1);
      sliderGroup[3*player+1]->setRange(0,numTypes[player]-1);
      if (isPrivateVals)
	sliderGroup[3*player+2]->setRange(0,numStates-1);
      else
	sliderGroup[3*player+2]->setRange(0,sqrt(numStates)-1);
      cout << "Slider setting completed." << endl;
    }

    for (int i = 0; i < 6; i++) {
      sliderGroup[i]->setSliderPosition(0);
      sliderGroup[i]->setSingleStep(1);
      lineEditGroup[i]->setText("0");
    }

  // catch (std::exception & e)
  //   {
  //     qDebug() << "Load solution didnt work :( from BCEDataState" << endl;
  //   }
}

void BCEDataState::setupControlsLayout() {

  // Slider, LineEdit, and CheckBox Controls Creation

  for (int player = 0; player < 2; player++) {
    sliderGroup.push_back(new BCESlider(Action,player));
    sliderGroup.push_back(new BCESlider(Type,player));
    sliderGroup.push_back(new BCESlider(State,player));

    lineEditGroup.push_back(new BCELineEdit(Action,player));
    lineEditGroup.push_back(new BCELineEdit(Type,player));
    lineEditGroup.push_back(new BCELineEdit(State,player));

    checkBoxGroup.push_back(new BCECheckBox(Action,player));
    checkBoxGroup.push_back(new BCECheckBox(Type,player));
    checkBoxGroup.push_back(new BCECheckBox(State,player));
  }

  for (int widgetIndex = 0; widgetIndex < 6; widgetIndex++) {
    lineEditGroup[widgetIndex]->setReadOnly(true);
    sliderGroup[widgetIndex]->setMaximumHeight(resHeight/54);
    sliderGroup[widgetIndex]->setMinimumWidth(resWidth/16);
    sliderGroup[widgetIndex]->setOrientation(Qt::Horizontal);
    lineEditGroup[widgetIndex]->setMaximumSize(resWidth/64,resHeight/54);
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
    connect(checkBoxGroup[widgetIndex],
	    SIGNAL(boolChanged(bool,BCESliderType,int)),
	    this,SLOT(setMarginalConditions(bool,BCESliderType,int)));
  }

  // Slider Labels

  QVector<BCELabel*> sliderLabels;

  for (int player = 0; player < 2; player++) {
    sliderLabels.push_back(new BCELabel(SliderLabel,Action,player));
    sliderLabels.push_back(new BCELabel(SliderLabel,Type,player));
    sliderLabels.push_back(new BCELabel(SliderLabel,State,player));	 
  }


  for (int labelIt = 0; labelIt < 6; labelIt++) {
    sliderLabels[labelIt]->setMaximumHeight(resHeight/54);
    connect(this,SIGNAL(sliderLabelsChanged(bool,int)),
	    sliderLabels[labelIt],SLOT(displayStateOrValues(bool,int)));
  }

  for (int i = 0; i < 2; i++)
    checkBoxGroup[3*i]->setChecked(true);

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
      gridSubLayouts[3*i+j]->addWidget(checkBoxGroup[3*i+j]);
      subLayoutWithLabels[3*i+j]->addWidget(sliderLabels[3*i+j]);
      subLayoutWithLabels[3*i+j]->addLayout(gridSubLayouts[3*i+j]);
      controlsGrid->addLayout(subLayoutWithLabels[3*i+j],j,i); // Layout Matrix
    } // Rows
  } // Columns

  controlsLayout = controlsGrid;

  // End Slider, LineEdit, CheckBox Creation
  //////////////////////////////////////////
}

void BCEDataState::setSliderValue(int value,
				  BCESliderType st,
				  int player) {
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
      state = values[0]+sqrt(gameData.getNumStates())*values[1];
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

void BCEDataState::setMarginalConditions(bool newBool,BCESliderType st,int player) {
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
  for (int player = 0; player < 2; player++)
    setObjectiveVals(player);
}

void BCEDataState::modifyEqmFocus(double x,double y) {

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

  resetManipulatedData();

}

void BCEDataState::setEqmMatrix() {

  vector<double> distribution;

  // State Conditions

  vector<int> stateConditions(1,state);
  if (isPrivateVals == false)
    stateConditions = vector<int>(1,state); 

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

  // assert(solutionData.numActions_total == distribution.size());

  vector<int> numActions = gameData.getNumActions();

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

void BCEDataState::setObjectiveVals(int player) {

  solutionData.getDeviationObjectives(player,actions[player],types[player],objectiveValues);

  emit(devPlotTitleChange(player,
			  actions[player],
			  types[player],
			  objectiveValues[player][actions[player]]));

  // Gets marginal probability of an action from the joint distn.
  vector<int> numActions = gameData.getNumActions();
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

void BCEDataState::setAllEqm() {

  solutionData.getExpectedObjectives(allEqm);
  cout << "setAllEqm Function Hit" << endl;
  emit(selectedEqmChanged());

}
