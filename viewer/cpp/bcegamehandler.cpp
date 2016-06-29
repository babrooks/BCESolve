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

#include "bcegamehandler.hpp"

BCEGameHandler::BCEGameHandler(QWidget *parent):
  game(BCEGame()),mapBoundaryOption(false),minAngleIncrement(1e-4)
{
  setupLayout();
  setParent(parent);
}

void BCEGameHandler::setupLayout() {
  ////////////////////////////////
  // LINEEDIT AND PUSHBUTTON SETUP

  numStatesEdit = new QLineEdit("1",this);
  numStatesEdit->setSizePolicy(QSizePolicy::Preferred,
			       QSizePolicy::Preferred);
  numStatesEdit->setReadOnly(true);

  QPushButton * addStateButton = new QPushButton("+",this);
  QPushButton * removeStateButton = new QPushButton(" -",this);
  QPushButton * nextStateButton = new QPushButton("->",this);
  QPushButton * prevStateButton = new QPushButton("<-",this);

  QSize buttonSize(50,addStateButton->height());
  addStateButton->resize(buttonSize);
  removeStateButton->resize(buttonSize);
  nextStateButton->resize(buttonSize);
  prevStateButton->resize(buttonSize);

  addStateButton->setMinimumWidth(buttonSize.width());
  removeStateButton->setMinimumWidth(buttonSize.width());
  nextStateButton->setMinimumWidth(buttonSize.width());
  prevStateButton->setMinimumWidth(buttonSize.width());

  addStateButton->setSizePolicy(QSizePolicy::Fixed,
				QSizePolicy::Preferred);
  removeStateButton->setSizePolicy(QSizePolicy::Fixed,
				   QSizePolicy::Preferred);
  nextStateButton->setSizePolicy(QSizePolicy::Fixed,
				 QSizePolicy::Preferred);
  prevStateButton->setSizePolicy(QSizePolicy::Fixed,
				 QSizePolicy::Preferred);

  // Setup initial vectors
  numActionsEdits = vector<QLineEdit*>(2);
  vector<BCEPushButton *> addActionButtons = vector<BCEPushButton*>(2);
  vector<BCEPushButton *> removeActionButtons = vector<BCEPushButton*>(2);
  numTypesEdits = vector<QLineEdit*>(2);
  vector<BCEPushButton *> addTypeButtons = vector<BCEPushButton*>(2);
  vector<BCEPushButton *> removeTypeButtons = vector<BCEPushButton*>(2);

  for (int player = 0; player < 2; player++)
    {
      numActionsEdits[player] = new QLineEdit("1",this);
      numActionsEdits[player]->setReadOnly(true);
      numActionsEdits[player]->setSizePolicy(QSizePolicy::Preferred,
					     QSizePolicy::Preferred);

      addActionButtons[player] = new BCEPushButton(player,"+",this);
      removeActionButtons[player] = new BCEPushButton(player," -",this);

      addActionButtons[player]->resize(buttonSize);
      removeActionButtons[player]->resize(buttonSize);

      addActionButtons[player]->setMinimumWidth(buttonSize.width());
      removeActionButtons[player]->setMinimumWidth(buttonSize.width());

      addActionButtons[player]->setSizePolicy(QSizePolicy::Fixed,
					      QSizePolicy::Preferred);
      removeActionButtons[player]->setSizePolicy(QSizePolicy::Fixed,
						 QSizePolicy::Preferred);
    }

  for (int player = 0; player < 2; player++)
    {
      numTypesEdits[player] = new QLineEdit("1",this);
      numTypesEdits[player]->setReadOnly(true);
      numTypesEdits[player]->setSizePolicy(QSizePolicy::Preferred,
					   QSizePolicy::Preferred);

      addTypeButtons[player] = new BCEPushButton(player,"+",this);
      removeTypeButtons[player] = new BCEPushButton(player," -",this);

      addTypeButtons[player]->resize(buttonSize);
      removeTypeButtons[player]->resize(buttonSize);

      addTypeButtons[player]->setMinimumWidth(buttonSize.width());
      removeTypeButtons[player]->setMinimumWidth(buttonSize.width());

      addTypeButtons[player]->setSizePolicy(QSizePolicy::Fixed,
					    QSizePolicy::Preferred);
      removeTypeButtons[player]->setSizePolicy(QSizePolicy::Fixed,
					       QSizePolicy::Preferred);
    }
  
  numObjectivesEdit = new QLineEdit("2",this);
  numObjectivesEdit->setReadOnly(true);
  numObjectivesEdit->setSizePolicy(QSizePolicy::Preferred,
				   QSizePolicy::Preferred);
  QPushButton *addObjectivesButton = new QPushButton("+",this);
  addObjectivesButton->resize(buttonSize);
  addObjectivesButton->setMinimumWidth(buttonSize.width());
  addObjectivesButton->setSizePolicy(QSizePolicy::Fixed,
				     QSizePolicy::Preferred);
  QPushButton *removeObjectivesButton = new QPushButton("- ",this);
  removeObjectivesButton->resize(buttonSize);
  removeObjectivesButton->setMinimumWidth(buttonSize.width());
  removeObjectivesButton->setSizePolicy(QSizePolicy::Fixed,
					QSizePolicy::Preferred);

  // END LINEEDIT AND PUSHBUTTON SETUP
  ////////////////////////////////////

  payoffTableView = new BCETableView();
  payoffTableView->setSelectionMode(QAbstractItemView::ContiguousSelection);
  priorTableView = new BCETableView();
  priorTableView->setSelectionMode(QAbstractItemView::ContiguousSelection);
  conditionalTableView = new BCETableView();
  conditionalTableView->setSelectionMode(QAbstractItemView::ContiguousSelection);
  weightsTableView = new BCETableView();
  weightsTableView->setSelectionMode(QAbstractItemView::ContiguousSelection);

  payoffModel = NULL;
  priorModel = NULL;
  conditionalModel = NULL;
  weightsModel = NULL;

  initializeModels();

  currentStateCombo = new QComboBox(this);
  currentStateCombo->addItem("0");
  currentStateCombo->setSizePolicy(QSizePolicy::Preferred,
				   QSizePolicy::Preferred);

  // Construct layout
  layout = new QVBoxLayout();
  QHBoxLayout * controlLayout = new QHBoxLayout();
  QFormLayout * leftControlLayout = new QFormLayout();
  QFormLayout * centerControlLayout = new QFormLayout();
  QVBoxLayout * rightControlLayout = new QVBoxLayout();
  QSplitter * tableLayout = new QSplitter();
  QVBoxLayout * payoffLayout = new QVBoxLayout();
  QVBoxLayout * probabilityLayout = new QVBoxLayout();
  QVBoxLayout * conditionalLayout = new QVBoxLayout();
  QVBoxLayout * weightsLayout = new QVBoxLayout();
  
  solveButton = new QPushButton(tr("Maximize Main Objective"),this);
  solveButton->setSizePolicy(QSizePolicy::Expanding,
			     QSizePolicy::Preferred);

  cancelButton = new QPushButton(tr("Cancel Solve"),this);
  cancelButton->setSizePolicy(QSizePolicy::Expanding,
			      QSizePolicy::Preferred);

  clearButton = new QPushButton(tr("Clear Game"),this);
  clearButton->setSizePolicy(QSizePolicy::Expanding,
			     QSizePolicy::Preferred);

  mapBoundaryButton = new QPushButton(tr("Map Main Vs. Secondary"),this);
  mapBoundaryButton->setSizePolicy(QSizePolicy::Expanding,
				     QSizePolicy::Preferred);

  QLineEdit *minAngleLineEdit = new QLineEdit(tr("1e-4"),this);
  minAngleLineEdit->setSizePolicy(QSizePolicy::Expanding,
				  QSizePolicy::Preferred);

  for (int player = 0; player < 2; player ++)
    {
      QHBoxLayout * numActionsLayout = new QHBoxLayout();
      numActionsLayout->addWidget(numActionsEdits[player]);
      numActionsLayout->addWidget(removeActionButtons[player]);
      numActionsLayout->addWidget(addActionButtons[player]);
      numActionsLayout->setSpacing(5);

      QString numActionsLabel = QString(tr("Player "))
	+QString::number(player+1)
	+QString(tr("'s number of actions ("));
      if (player == 0)
	numActionsLabel += QString(tr("row"));
      else
	numActionsLabel += QString(tr("column"));
      numActionsLabel += QString(tr("):"));
      
      leftControlLayout->addRow(numActionsLabel,
				numActionsLayout);      
    }

  for (int player = 0; player < 2; player ++)
    {
      QHBoxLayout * numTypesLayout = new QHBoxLayout();
      numTypesLayout->addWidget(numTypesEdits[player]);
      numTypesLayout->addWidget(removeTypeButtons[player]);
      numTypesLayout->addWidget(addTypeButtons[player]);
      numTypesLayout->setSpacing(5);

      QString numTypesLabel = QString(tr("Player "))
	+QString::number(player+1)
	+QString(tr("'s number of types ("));
      if (player == 0)
	numTypesLabel += QString(tr("row"));
      else
	numTypesLabel += QString(tr("column"));
      numTypesLabel += QString(tr("):"));
      
      leftControlLayout->addRow(numTypesLabel,
				  numTypesLayout);      
    }
       

  QHBoxLayout * currentStateLayout = new QHBoxLayout();
  currentStateLayout->addWidget(currentStateCombo);
  currentStateLayout->addWidget(prevStateButton);
  currentStateLayout->addWidget(nextStateButton);
  currentStateLayout->setSpacing(5);
  
  centerControlLayout->addRow(new QLabel(tr("Current state:"),this),
			      currentStateLayout);

  QHBoxLayout * numStatesLayout = new QHBoxLayout();
  numStatesLayout->addWidget(numStatesEdit);
  numStatesLayout->addWidget(removeStateButton);
  numStatesLayout->addWidget(addStateButton);
  numStatesLayout->setSpacing(5);
  centerControlLayout->addRow(new QLabel(tr("Number of states:"),this),
			    numStatesLayout);
  centerControlLayout->setSpacing(5);

  QHBoxLayout * numObjectivesLayout = new QHBoxLayout();
  numObjectivesLayout->addWidget(numObjectivesEdit);
  numObjectivesLayout->addWidget(removeObjectivesButton);
  numObjectivesLayout->addWidget(addObjectivesButton);
  numObjectivesLayout->setSpacing(5);

  QString numObjectivesLabel = QString(tr("Number of Objectives:"));
      
  centerControlLayout->addRow(numObjectivesLabel,
			     numObjectivesLayout); 
  centerControlLayout->addRow(clearButton);

  QHBoxLayout *minAngleLayout = new QHBoxLayout();
  QLabel *minAngleLabel = new QLabel(tr("Minimum Angle Increment:"),this);
  minAngleLayout->addWidget(minAngleLabel);
  minAngleLayout->addWidget(minAngleLineEdit);

  rightControlLayout->addLayout(minAngleLayout);
  rightControlLayout->addWidget(solveButton);
  rightControlLayout->addWidget(mapBoundaryButton);
  rightControlLayout->addWidget(cancelButton);

  controlLayout->addLayout(leftControlLayout);
  controlLayout->addLayout(centerControlLayout);
  controlLayout->addLayout(rightControlLayout);

  ////////////////////
  // SCROLL AREA SETUP

  // Scroll Area for Prior

  QScrollArea * probabilityScrollArea = new QScrollArea(this);
  QWidget * probabilityWidget = new QWidget(this);

  QVBoxLayout* probabilityTableLayout = new QVBoxLayout();
  probabilityTableLayout->addWidget(priorTableView);
  probabilityWidget->setLayout(probabilityTableLayout);
  probabilityScrollArea->setWidget(probabilityWidget);

  probabilityScrollArea->setWidgetResizable(true);
  probabilityScrollArea->setSizePolicy(QSizePolicy::Expanding,
  				       QSizePolicy::Expanding);
  probabilityScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

  // Scroll Area for Conditional Distribution of Types

  QScrollArea * payoffScrollArea = new QScrollArea(this);
  QWidget * payoffWidget = new QWidget(this);

  QVBoxLayout* payoffTableLayout = new QVBoxLayout();
  payoffTableLayout->addWidget(payoffTableView);
  payoffWidget->setLayout(payoffTableLayout);
  payoffScrollArea->setWidget(payoffWidget);

  payoffScrollArea->setWidgetResizable(true);
  payoffScrollArea->setSizePolicy(QSizePolicy::Expanding,
				  QSizePolicy::Expanding);
  payoffScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

  payoffLayout->addWidget(new QLabel(tr("Player Payoffs:"),this));
  payoffLayout->addWidget(payoffScrollArea);

  // Scroll Area for Payoff Matrix

  QScrollArea * conditionalScrollArea = new QScrollArea(this);
  QWidget * conditionalWidget = new QWidget(this);

  QVBoxLayout* conditionalTableLayout = new QVBoxLayout();
  conditionalTableLayout->addWidget(conditionalTableView);
  conditionalWidget->setLayout(conditionalTableLayout);
  conditionalScrollArea->setWidget(conditionalWidget);

  conditionalScrollArea->setWidgetResizable(true);
  conditionalScrollArea->setSizePolicy(QSizePolicy::Expanding,
  				       QSizePolicy::Expanding);
  conditionalScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

  conditionalLayout->addWidget(new QLabel(tr("Conditional Distribution of Types:"),this));
  conditionalLayout->addWidget(conditionalScrollArea);

  // Scroll Area for Weights Vector

  QScrollArea * weightsScrollArea = new QScrollArea(this);
  QWidget * weightsWidget = new QWidget(this);

  QVBoxLayout* weightsTableLayout = new QVBoxLayout();
  weightsTableLayout->addWidget(weightsTableView);
  weightsWidget->setLayout(weightsTableLayout);
  weightsScrollArea->setWidget(weightsWidget);

  weightsScrollArea->setWidgetResizable(true);
  weightsScrollArea->setSizePolicy(QSizePolicy::Expanding,
				   QSizePolicy::Expanding);
  weightsScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

  weightsLayout->addWidget(new QLabel(tr("Weights on Objectives:"),this));
  weightsLayout->addWidget(weightsScrollArea);
  weightsLayout->addWidget(new QLabel(tr("Prior over States:"),this));
  weightsLayout->addWidget(probabilityScrollArea);
 
  // END SCROLL AREA SETUP
  //////////////////////////////
  // ADDING WIDGETS TO QSPLITTER

  QWidget *payoffSplitterWidget = new QWidget(this);
  payoffSplitterWidget->setLayout(payoffLayout);
  tableLayout->addWidget(payoffSplitterWidget);
  QWidget *conditionalSplitterWidget = new QWidget(this);
  conditionalSplitterWidget->setLayout(conditionalLayout);
  tableLayout->addWidget(conditionalSplitterWidget);
  QWidget *weightsSplitterWidget = new QWidget(this);
  weightsSplitterWidget->setLayout(weightsLayout);
  tableLayout->addWidget(weightsSplitterWidget);

  QSizePolicy policy(QSizePolicy::Expanding,QSizePolicy::Expanding);
  tableLayout->setSizePolicy(policy);

  tableLayout->setStretchFactor(0,60);
  tableLayout->setStretchFactor(1,10);
  tableLayout->setStretchFactor(3,30);

  // END ADDING WIDGETS TO QSPLITTER
  //////////////////////////////////

  QVBoxLayout *fullLayout = new QVBoxLayout();
  fullLayout->addWidget(tableLayout);

  layout->addLayout(controlLayout);
  layout->addLayout(fullLayout);

  ////////////////////////
  // SLOT CONNECTION SETUP

  connect(currentStateCombo,SIGNAL(currentIndexChanged(int)),
	  this,SLOT(currentStateChanged(int)));

  for (int player = 0; player < 2; player++) {
    connect(addActionButtons[player],SIGNAL(clickedForPlayer(int)),
	    this,SLOT(actionAdded(int)));
    connect(addTypeButtons[player],SIGNAL(clickedForPlayer(int)),
	    this,SLOT(typeAdded(int)));
    connect(removeActionButtons[player],SIGNAL(clickedForPlayer(int)),
	    this,SLOT(actionRemoved(int)));
    connect(removeTypeButtons[player],SIGNAL(clickedForPlayer(int)),
	    this,SLOT(typeRemoved(int)));
  }

  connect(addObjectivesButton,SIGNAL(clicked()),
	  this,SLOT(objectiveAdded()));
  connect(removeObjectivesButton,SIGNAL(clicked()),
	  this,SLOT(objectiveRemoved()));

  connect(addStateButton,SIGNAL(clicked()),
	  this,SLOT(stateAdded()));
  connect(removeStateButton,SIGNAL(clicked()),
	  this,SLOT(stateRemoved()));

  connect(nextStateButton,SIGNAL(clicked()),
	  this,SLOT(nextState()));
  connect(prevStateButton,SIGNAL(clicked()),
	  this,SLOT(prevState()));
 
  connect(solveButton,SIGNAL(clicked()),
	  this,
	  SLOT(emitSolveSignal()));
  connect(cancelButton,SIGNAL(clicked()),
	  this,
	  SIGNAL(cancelSolveRoutine()));

  connect(clearButton,SIGNAL(clicked()),
	  this,
	  SLOT(clearCurrentGame()));

  connect(mapBoundaryButton,SIGNAL(clicked()),
	  this,SLOT(setMBSolveOption()));

  connect(minAngleLineEdit,SIGNAL(textChanged(QString)),
	  this,SLOT(setMinAngleIncr(QString)));

  // qDebug() << "Finished sggamehandler constructor" << endl;

  // Set Initial Line-edit Values
  for (int player=0;player<2;player++) {
    numActionsEdits[player]
      ->setText(QString::number(game.getNumActions()[player]));
    numTypesEdits[player]->setText(QString::number(game.getNumTypes()
						   [player]));
  }
  numStatesEdit->setText(QString::number(game.getNumStates()));
  numObjectivesEdit->setText(QString::number(game.getNumObjectives()));

}

BCEGameHandler::~BCEGameHandler()
{
  if (payoffModel != NULL)
    delete payoffModel;
  if (priorModel != NULL)
    delete priorModel;
  if (conditionalModel != NULL)
    delete conditionalModel;
  if (weightsModel != NULL)
    delete weightsModel;
  delete payoffTableView;
  delete priorTableView;
  delete conditionalTableView;
  delete weightsTableView;
}

void BCEGameHandler::setGame(const BCEGame & _game)
{
  game = _game;

  changeNumberOfStates(game.getNumStates());
  for (int player = 0; player < 2; player++) {
    numActionsEdits[player]
      ->setText(QString::number(game.getNumActions()[player]));
    numTypesEdits[player]
      ->setText(QString::number(game.getNumTypes()[player]));
  }
  numStatesEdit->setText(QString::number(game.getNumStates()));
  numObjectivesEdit->setText(QString::number(game.getNumObjectives()));  

  initializeModels();

  // qDebug() << "Finished setting game" << endl;
}

void BCEGameHandler::changeNumberOfStates(int newS)
{
  disconnect(currentStateCombo,SIGNAL(currentIndexChanged(int)),
	     this,SLOT(currentStateChanged(int)));

  for (int player = 0; player < 2; player++) {
    numActionsEdits[player]
      ->setText(QString::number(game.getNumActions()[player]));
    numTypesEdits[player]
      ->setText(QString::number(game.getNumTypes()[player]));
  }
  
  int state = currentStateCombo->count();
  // Remove items larger than the new number of states
  while (state > newS)
    currentStateCombo->removeItem(--state);
  // Add new states if number of states increased
  while (state < newS)
    currentStateCombo->addItem(QString::number(state++));

  currentStateCombo->setCurrentIndex(0);

  connect(currentStateCombo,SIGNAL(currentIndexChanged(int)),
	  this,SLOT(currentStateChanged(int)));
  
} // changeNumberOfStates

void BCEGameHandler::initializeModels()
{
  // Create new payoffModel
  if (payoffModel != NULL)
    delete payoffModel;
  if (priorModel != NULL)
    delete priorModel;
  if (conditionalModel != NULL)
    delete conditionalModel;
  if (weightsModel != NULL)
    delete weightsModel;

  payoffModel = new BCEPayoffTableModel(&game,0);
  payoffTableView->setEditTriggers(QAbstractItemView::AllEditTriggers);
  payoffTableView->setModel(payoffModel);
  payoffTableView->resizeColumnsToContents();

  priorModel = new BCEPriorTableModel(&game);
  priorTableView->setEditTriggers(QAbstractItemView::AllEditTriggers);
  priorTableView->setModel(priorModel);
  priorTableView->resizeColumnsToContents();

  conditionalModel = new BCEConditionalTableModel(&game,0);
  conditionalTableView->setEditTriggers(QAbstractItemView::AllEditTriggers);
  conditionalTableView->setModel(conditionalModel);
  conditionalTableView->resizeColumnsToContents();

  weightsModel = new BCEObjWeightsTableModel(&game);
  weightsTableView->setEditTriggers(QAbstractItemView::AllEditTriggers);
  weightsTableView->setModel(weightsModel);
  weightsTableView->resizeColumnsToContents();

} // initializeModels

void BCEGameHandler::setState(int state)
{
  for (int player = 0; player < 2; player++) {
    numActionsEdits[player]
      ->setText(QString::number(game.getNumActions()[player]));
    numTypesEdits[player]
      ->setText(QString::number(game.getNumTypes()[player]));
  }
  
  payoffModel->setState(state);
  payoffModel->emitLayoutChanged();
  conditionalModel->setState(state);
  conditionalModel->emitLayoutChanged();
  payoffTableView->resizeColumnsToContents();
  conditionalTableView->resizeColumnsToContents();
  
  disconnect(currentStateCombo,SIGNAL(currentIndexChanged(int)),
	     this,SLOT(currentStateChanged(int)));
  currentStateCombo->setCurrentIndex(state);
  connect(currentStateCombo,SIGNAL(currentIndexChanged(int)),
	  this,SLOT(currentStateChanged(int)));
  
} // setState

void BCEGameHandler::currentStateChanged(int newS)
{
  setState(newS);
}

void BCEGameHandler::nextState()
{
  int state = currentStateCombo->currentIndex();

  if (state < currentStateCombo->count()-1)
    setState(state+1);
}

void BCEGameHandler::prevState()
{
  int state = currentStateCombo->currentIndex();

  if (state > 0)
    setState(state-1);
}

void BCEGameHandler::actionAdded(int player)
{
  if (player < 0 || player > 1)
    return;
  
  int newAction = game.getNumActions()[player];
  
  if (payoffTableView->selectionModel()->hasSelection())
    {
      if (player==1)
	newAction = (payoffTableView->selectionModel()
		     ->selectedIndexes().front().column()+1);
      else
	newAction = (payoffTableView->selectionModel()
		     ->selectedIndexes().front().row()+1);
    }
    
  game.addAction(player,newAction);
  numActionsEdits[player]
    ->setText(QString::number(game.getNumActions()[player]));

  payoffModel->emitLayoutChanged();
  payoffTableView->resizeColumnToContents(newAction);

}  // actionAdded

void BCEGameHandler::typeAdded(int player)
{
  if (player < 0 || player > 1)
    return;
  
  int newType = game.getNumTypes()[player];
  
  if (conditionalTableView->selectionModel()->hasSelection())
    {
      if (player==1)
	newType = (conditionalTableView->selectionModel()
		   ->selectedIndexes().front().column()+1);
      else
	newType = (conditionalTableView->selectionModel()
		   ->selectedIndexes().front().row()+1);
    }
    
  game.addType(player,newType);
  numTypesEdits[player]
    ->setText(QString::number(game.getNumTypes()[player]));

  conditionalModel->emitLayoutChanged();
  conditionalTableView->resizeColumnToContents(newType);

}  // typeAdded

void BCEGameHandler::objectiveAdded() {
  
  int newObjective = game.getNumObjectives();
  
  if (weightsTableView->selectionModel()->hasSelection())
    {
      newObjective = (weightsTableView->selectionModel()
		      ->selectedIndexes().front().row()+1);
    } 

  game.addObjective(newObjective,"k");
  weightsModel->addObjective(newObjective);
  numObjectivesEdit
    ->setText(QString::number(game.getNumObjectives()));

  weightsModel->emitLayoutChanged();
  weightsTableView->resizeColumnToContents(newObjective);
  payoffModel->emitLayoutChanged();
} // objectiveAdded

void BCEGameHandler::stateAdded()
{
  int newState = currentStateCombo->currentIndex()+1;
  if (priorTableView->selectionModel()->hasSelection()) {
    newState = (priorTableView->selectionModel()
		->selectedIndexes().front().row()+1);
  }
  game.addState(newState);
  changeNumberOfStates(game.getNumStates());
  numStatesEdit->setText(QString::number(game.getNumStates()));
  
  priorModel->emitLayoutChanged();
  priorTableView->resizeColumnToContents(game.getNumStates());
  setState(newState);
} // stateAdded

void BCEGameHandler::actionRemoved(int player)
{
  int state = currentStateCombo->currentIndex();
  if (game.getNumActions()[player] == 1)
    return;
  
  int action = game.getNumActions()[player]-1;
  if (payoffTableView->selectionModel()->hasSelection())
    {
      if (player == 1)
	action = (payoffTableView->selectionModel()
		  ->selectedIndexes().front().column());
      else
	action = (payoffTableView->selectionModel()
		  ->selectedIndexes().front().row());
    }

  game.removeAction(player,action);
  numActionsEdits[player]->setText(QString::number(game.getNumActions()
						   [player]));

  payoffModel->emitLayoutChanged();

}

void BCEGameHandler::typeRemoved(int player)
{
  if (game.getNumTypes()[player] == 1)
    return;
  
  int type = game.getNumTypes()[player]-1;
  if (conditionalTableView->selectionModel()->hasSelection())
    {
      if (player == 1)
	type = (conditionalTableView->selectionModel()
		->selectedIndexes().front().column());
      else
	type = (conditionalTableView->selectionModel()
		->selectedIndexes().front().row());
    }

  game.removeType(player,type);
  numTypesEdits[player]->setText(QString::number(game.getNumTypes()
						 [player]));

  conditionalModel->emitLayoutChanged();

}

void BCEGameHandler::objectiveRemoved()
{
  if (game.getNumObjectives() == 1 || game.getNumObjectives() == 2)
    return;
  
  int objective = game.getNumObjectives()-1;
  if (weightsTableView->selectionModel()->hasSelection())
    {
      objective = (weightsTableView->selectionModel()
		   ->selectedIndexes().front().row());
    }

  game.removeObjective(objective);
  weightsModel->removeObjective(objective);
  numObjectivesEdit->setText(QString::number(game.getNumObjectives()));

  weightsModel->emitLayoutChanged();
  payoffModel->emitLayoutChanged();

} // objectiveRemoved

void BCEGameHandler::stateRemoved()
{
  if (game.getNumStates() == 1)
    return;

  int state = currentStateCombo->currentIndex();

  // popBackProbabilityTable();

  int newState;
  if (state > 0)
    newState = state-1;
  else
    newState = 0;

  disconnect(currentStateCombo,SIGNAL(currentIndexChanged(int)),
	     this,SLOT(currentStateChanged(int)));
  currentStateCombo->setCurrentIndex(min(state,newState));
  currentStateCombo->removeItem(currentStateCombo->count()-1);
  connect(currentStateCombo,SIGNAL(currentIndexChanged(int)),
	  this,SLOT(currentStateChanged(int)));
  
  game.removeState(state);

  priorModel->emitLayoutChanged();
  priorTableView->resizeColumnToContents(game.getNumStates());

  numStatesEdit->setText(QString::number(game.getNumStates()));

  setState(newState);
  
} // stateRemoved

void BCEGameHandler::clearCurrentGame() {
  game = BCEGame();
  conditionalModel->emitLayoutChanged();
  conditionalTableView->resizeColumnToContents(game.getNumStates());
  weightsModel->emitLayoutChanged();
  weightsModel->resetObjectiveWeights();
  weightsTableView->resizeColumnToContents(game.getNumStates());
  payoffModel->emitLayoutChanged();
  payoffTableView->resizeColumnToContents(game.getNumStates());
  priorModel->emitLayoutChanged();
  priorTableView->resizeColumnToContents(game.getNumStates());
  for (int player=0;player<2;player++) {
    numActionsEdits[player]
      ->setText(QString::number(game.getNumActions()[player]));
    numTypesEdits[player]->setText(QString::number(game.getNumTypes()
						   [player]));
  }
  numStatesEdit->setText(QString::number(game.getNumStates()));
  numObjectivesEdit->setText(QString::number(game.getNumObjectives()));
  currentStateCombo->clear();
  currentStateCombo->addItem("0");
}

const vector<double> BCEGameHandler::getWeightsOnObjectives() {
  return weightsModel->getSolverData();
}

const vector<vector<double> > BCEGameHandler::getMapBWeights() {

  return weightsModel->getMapBoundaryData();
}

void BCEGameHandler::emitSolveSignal() {
  mapBoundaryOption = false;
  emit(startSolveRoutine());
}
