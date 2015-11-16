#include <QtWidgets>
#include "bcegamehandler.hpp"
#include "bcepushbutton.hpp"

BCEGameHandler::BCEGameHandler():
  game(BCEGame())
{
  numStatesEdit = new QLineEdit("1");
  numStatesEdit->setSizePolicy(QSizePolicy::Preferred,
			       QSizePolicy::Preferred);
  numStatesEdit->setReadOnly(true);
  QPushButton * addStateButton = new QPushButton("+");
  QPushButton * removeStateButton = new QPushButton(" -");
  QPushButton * nextStateButton = new QPushButton("->");
  QPushButton * prevStateButton = new QPushButton("<-");
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

  numActionsEdits = vector<QLineEdit*>(2);
  vector<BCEPushButton *> addActionButtons = vector<BCEPushButton*>(2);
  vector<BCEPushButton *> removeActionButtons = vector<BCEPushButton*>(2);
  for (int player = 0; player < 2; player++)
    {
      numActionsEdits[player] = new QLineEdit("1");
      numActionsEdits[player]->setReadOnly(true);
      numActionsEdits[player]->setSizePolicy(QSizePolicy::Preferred,
					     QSizePolicy::Preferred);

      addActionButtons[player] = new BCEPushButton(player,"+");
      removeActionButtons[player] = new BCEPushButton(player," -");

      addActionButtons[player]->resize(buttonSize);
      removeActionButtons[player]->resize(buttonSize);

      addActionButtons[player]->setMinimumWidth(buttonSize.width());
      removeActionButtons[player]->setMinimumWidth(buttonSize.width());

      addActionButtons[player]->setSizePolicy(QSizePolicy::Fixed,
					      QSizePolicy::Preferred);
      removeActionButtons[player]->setSizePolicy(QSizePolicy::Fixed,
						 QSizePolicy::Preferred);
    }

  numTypesEdits = vector<QLineEdit*>(2);
  vector<BCEPushButton *> addTypeButtons = vector<BCEPushButton*>(2);
  vector<BCEPushButton *> removeTypeButtons = vector<BCEPushButton*>(2);
  for (int player = 0; player < 2; player++)
    {
      numTypesEdits[player] = new QLineEdit("1");
      numTypesEdits[player]->setReadOnly(true);
      numTypesEdits[player]->setSizePolicy(QSizePolicy::Preferred,
					     QSizePolicy::Preferred);

      addTypeButtons[player] = new BCEPushButton(player,"+");
      removeTypeButtons[player] = new BCEPushButton(player," -");

      addTypeButtons[player]->resize(buttonSize);
      removeTypeButtons[player]->resize(buttonSize);

      addTypeButtons[player]->setMinimumWidth(buttonSize.width());
      removeTypeButtons[player]->setMinimumWidth(buttonSize.width());

      addTypeButtons[player]->setSizePolicy(QSizePolicy::Fixed,
					      QSizePolicy::Preferred);
      removeTypeButtons[player]->setSizePolicy(QSizePolicy::Fixed,
						 QSizePolicy::Preferred);
    }
  
  payoffTableView = new BCETableView();
  payoffTableView->setSelectionMode(QAbstractItemView::ContiguousSelection);
  priorTableView = new BCETableView();
  priorTableView->setSelectionMode(QAbstractItemView::ContiguousSelection);
  conditionalTableView = new BCETableView();
  conditionalTableView->setSelectionMode(QAbstractItemView::ContiguousSelection);

  payoffModel = NULL;
  priorModel = NULL;
  conditionalModel = NULL;

  QVBoxLayout* probabilityTableLayout = new QVBoxLayout();
  probabilityTableLayout->addWidget(priorTableView);

  initializeModels();

  currentStateCombo = new QComboBox();
  currentStateCombo->addItem("0");
  currentStateCombo->setSizePolicy(QSizePolicy::Preferred,
				   QSizePolicy::Preferred);

  // Construct layout
  layout = new QVBoxLayout();
  QHBoxLayout * controlLayout = new QHBoxLayout();
  QFormLayout * leftControlLayout = new QFormLayout();
  QFormLayout * centerControlLayout = new QFormLayout();
  QFormLayout * rightControlLayout = new QFormLayout();
  QHBoxLayout * tableLayout = new QHBoxLayout();
  QVBoxLayout * payoffLayout = new QVBoxLayout();
  QVBoxLayout * probabilityLayout = new QVBoxLayout();
  QVBoxLayout * conditionalLayout = new QVBoxLayout();
  
  solveButton = new QPushButton(tr("Solve"));
  solveButton->setSizePolicy(QSizePolicy::Fixed,
			     QSizePolicy::Preferred);
  solveButton->resize(300,solveButton->height());

  cancelButton = new QPushButton(tr("Cancel"));
  cancelButton->setSizePolicy(QSizePolicy::Fixed,
			      QSizePolicy::Preferred);
  cancelButton->resize(300,cancelButton->height());

  // qDebug() << "I got to here!!!" << endl;

  QHBoxLayout * currentStateLayout = new QHBoxLayout();
  currentStateLayout->addWidget(currentStateCombo);
  currentStateLayout->addWidget(prevStateButton);
  currentStateLayout->addWidget(nextStateButton);
  currentStateLayout->setSpacing(5);
  
  centerControlLayout->addRow(new QLabel(tr("Current state:")),
			      currentStateLayout);
  // leftControlLayout->addRow(removeStateButton,
  // 			    addStateButton);
  // leftControlLayout->setSpacing(0);

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

  QHBoxLayout * numStatesLayout = new QHBoxLayout();
  numStatesLayout->addWidget(numStatesEdit);
  numStatesLayout->addWidget(removeStateButton);
  numStatesLayout->addWidget(addStateButton);
  numStatesLayout->setSpacing(5);
  leftControlLayout->addRow(new QLabel(tr("Number of states:")),
			    numStatesLayout);
  leftControlLayout->setSpacing(5);

  rightControlLayout->addRow(solveButton);
  rightControlLayout->addRow(cancelButton);
  
  controlLayout->addLayout(leftControlLayout);
  controlLayout->addLayout(centerControlLayout);
  controlLayout->addLayout(rightControlLayout);

  payoffLayout->addWidget(new QLabel(tr("Stage payoffs:")));
  payoffLayout->addWidget(payoffTableView);

  conditionalLayout->addWidget(new QLabel(tr("Conditional Distn of Types:")));
  conditionalLayout->addWidget(conditionalTableView);

  QScrollArea * probabilityScrollArea = new QScrollArea();
  QWidget * probabilityWidget = new QWidget();

  probabilityWidget->setLayout(probabilityTableLayout);
  probabilityScrollArea->setWidget(probabilityWidget);

  probabilityScrollArea->setWidgetResizable(true);
  probabilityScrollArea->setSizePolicy(QSizePolicy::Expanding,
  				       QSizePolicy::Expanding);
  probabilityScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

  probabilityLayout->addWidget(new QLabel(tr("Prior Distribution of States:")));
  probabilityLayout->addWidget(probabilityScrollArea);

  tableLayout->addLayout(payoffLayout);
  tableLayout->addLayout(probabilityLayout);
  tableLayout->addLayout(conditionalLayout);

  // resizePayoffTable(0,2,0,2);
  // resizeProbabilityTable(0,2,0,2,0,1);
  
  layout->addLayout(controlLayout);
  layout->addLayout(tableLayout);

  // Connect slots

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

  connect(addStateButton,SIGNAL(clicked()),
	  this,SLOT(stateAdded()));
  connect(removeStateButton,SIGNAL(clicked()),
	  this,SLOT(stateRemoved()));

  connect(nextStateButton,SIGNAL(clicked()),
	  this,SLOT(nextState()));
  connect(prevStateButton,SIGNAL(clicked()),
	  this,SLOT(prevState()));

  // qDebug() << "Finished sggamehandler constructor" << endl;

}

BCEGameHandler::~BCEGameHandler()
{
  if (payoffModel != NULL)
    delete payoffModel;
  if (priorModel != NULL)
    delete priorModel;
  if (conditionalModel != NULL)
    delete conditionalModel;
  // for (int state = 0; state < game.getNumStates(); state++)
  //   delete probabilityModels[state];
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
  int state = currentStateCombo->currentIndex();
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
