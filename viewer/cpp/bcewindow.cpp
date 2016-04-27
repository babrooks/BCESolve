#include "bcewindow.hpp"
#include "bcegame.hpp"
#include <QtWidgets>
#include "bcelogstream.hpp"

BCEWindow::BCEWindow(BCELogHandler &logHandler) {

  // Point the stored logTab pointer at the supplied logHandler
  logTab = &logHandler;

  // Set the default path for loading examples.
  path=QString("../examples/");
  screenShotPath=QString("../examples/screenshots/");

  // Initialization of Pointers
  solutionTab = new BCEPlotHandler(this);
  gameTab = new BCEGameHandler(this);
  callback = new BCEGurobiCallback();

  // Menu Bar
  QMenu * fileMenu = menuBar()->addMenu(tr("&File"));
  QAction * loadSolutionAction = new QAction(tr("&Load Solution"),this);
  QAction * loadGameAction = new QAction(tr("Load &Game"),this);
  QAction * saveSolutionAction = new QAction(tr("&Save Solution"),this);
  QAction * saveGameAction = new QAction(tr("S&ave Game"),this);
  QAction * quitAction = new QAction(tr("&Quit GUI"),this);
  QMenu * viewMenu = menuBar()->addMenu(tr("&View"));
  QAction * linearScale = new QAction(tr("&Linear/Log Color Scale Toggle"),this);
  QAction * colorfulDistn = new QAction(tr("&Colorful/Blue Theme Toggle"),this);
  QAction * screenShotAction = new QAction(tr("&Save a screen shot"),this);
  QAction * conditionalBCE = new QAction(tr("&View BCE Conditional on State"),this);
  QMenu * toolMenu = menuBar()->addMenu(tr("&Tools"));
  QAction * generateHAGame = new QAction(tr("&Generate Hybrid Auction"),this);
  QAction * generateCVGame = new QAction(tr("&Generate Common Value Auction"),this);
  QAction * solveOption = new QAction(tr("&Solve Game"),this);
  QAction * cancelOption = new QAction(tr("&Cancel Solve"),this);

  fileMenu->addAction(loadSolutionAction);
  fileMenu->addAction(loadGameAction);
  fileMenu->addAction(saveSolutionAction);
  fileMenu->addAction(saveGameAction);
  viewMenu->addAction(linearScale);
  fileMenu->addAction(quitAction);
  linearScale->setCheckable(true);
  linearScale->setChecked(true);
  viewMenu->addAction(colorfulDistn);
  colorfulDistn->setCheckable(true);
  colorfulDistn->setChecked(true);
  viewMenu->addAction(screenShotAction);
  viewMenu->addAction(conditionalBCE);
  conditionalBCE->setCheckable(true);
  conditionalBCE->setChecked(true);
  toolMenu->addAction(generateHAGame);
  toolMenu->addAction(generateCVGame);
  toolMenu->addAction(solveOption);
  toolMenu->addAction(cancelOption);
  loadSolutionAction->setShortcut(tr("Ctrl+L"));
  loadGameAction->setShortcut(tr("Ctrl+G"));
  screenShotAction->setShortcut(tr("Ctrl+P"));
  quitAction->setShortcut(tr("Ctrl+W"));
  saveSolutionAction->setShortcut(tr("Ctrl+S"));

  // Menu Connections
  connect(loadSolutionAction,SIGNAL(triggered()),this,SLOT(loadSolution()));
  connect(loadGameAction,SIGNAL(triggered()),this,SLOT(loadGame()));
  connect(saveSolutionAction,SIGNAL(triggered()),this,SLOT(saveSolution()));
  connect(saveGameAction,SIGNAL(triggered()),this,SLOT(saveGame()));
  connect(quitAction,SIGNAL(triggered()),this,SLOT(close()));
  connect(linearScale,SIGNAL(toggled(bool)),solutionTab,SLOT(toggleLinearScale(bool)));
  connect(colorfulDistn,SIGNAL(toggled(bool)),solutionTab,SLOT(toggleColorfulTheme(bool)));
  connect(screenShotAction,SIGNAL(triggered()),this,SLOT(screenshot()));
  connect(conditionalBCE,SIGNAL(triggered(bool)),this,SLOT(conditionBCE(bool)));
  connect(generateHAGame,SIGNAL(triggered()),this,SLOT(generateHybridAuction()));
  connect(generateCVGame,SIGNAL(triggered()),this,SLOT(generateCommonValueAuction()));
  connect(solveOption,SIGNAL(triggered()),this,SLOT(runSolve()));
  connect(cancelOption,SIGNAL(triggered()),this,SIGNAL(setCancelFlag()));

  // Exception Connection
  connect(solutionTab->guiData,SIGNAL(sendException(QString)),
	  this,SLOT(displayException(QString)));

  // Solve Routine Connections
  connect(gameTab,SIGNAL(startSolveRoutine()),
	  this,SLOT(runSolve()));
  connect(gameTab,SIGNAL(cancelSolveRoutine()),
	  this,SIGNAL(setCancelFlag()));
  connect(this,SIGNAL(setCancelFlag()),
	  callback,SLOT(setCancelFlagTrue()));

  // Layout Setup
  tabWidget = new QTabWidget(this);
  QWidget *solutionTabWidget = new QWidget(this);
  solutionTabWidget->setLayout(solutionTab->getLayout());
  QWidget *gameTabWidget = new QWidget(this);
  gameTabWidget->setLayout(gameTab->getLayout());
  QWidget *logTabWidget = new QWidget(this);
  logTabWidget->setLayout(logTab->getLayout());

  tabWidget->addTab(gameTabWidget,"Game");
  tabWidget->addTab(solutionTabWidget,"Solution");
  tabWidget->addTab(logTabWidget,"Log");

  QHBoxLayout *mainLayout = new QHBoxLayout();
  mainLayout->addWidget(tabWidget);

  QWidget *mainPanel = new QWidget(this);
  mainPanel->setLayout(mainLayout);

  // Set Default Tab 
  setCentralWidget(mainPanel);

  // Maximize Window by Default
  setWindowState(Qt::WindowMaximized);

  // Window Title
  setWindowTitle(QApplication::translate("bceviewer","BCE Solution Viewer"));

  // Load in default Game
  BCEGame defaultGame;
  QString defaultPath = "../examples/prisonervsbattle.bgm";
  QByteArray ba = defaultPath.toLocal8Bit();
  const char* defaultPath_c = ba.data();
  BCEGame::load(defaultGame,defaultPath_c);

  gameTab->setGame(defaultGame);

} // Default Constructor

void BCEWindow::loadSolution() {

  QString newPath = QFileDialog::getOpenFileName(this,tr("Select a Solution File"),
						 path,
						 tr("BCESolve Solution Files (*.bce)"));


  if (newPath.isEmpty()) {
    cout << "Path for loading solution is empty." << endl;
    return;
  }

  try
    {
    QByteArray ba = newPath.toLocal8Bit();
    const char* newPath_c = ba.data();

    BCESolution loadedSolution;
    BCESolution::load(loadedSolution,newPath_c);
    BCEGame loadedGame = loadedSolution.getGame();

    solutionTab->setSolution(loadedSolution);
    gameTab->setGame(loadedGame);

    tabWidget->setCurrentIndex(1);

    }
  catch(BCEException & e)
    {
      displayException(QString("Load solution didnt work :( from the BCEWindow class. ") +
		       QString("A BCEException was thrown with message: ") +
		       QString::fromStdString(e.getMessage()));
    }
  catch (std::exception & e)
    {
      displayException("Load solution didnt work :( from the BCEWindow class");
    }

}

void BCEWindow::loadGame() {

  QString newPath = QFileDialog::getOpenFileName(this,tr("Select a Game File"),
						 path,
						 tr("BCESolve Game Files (*.bgm)"));

  if (newPath.isEmpty()) {
    cout << "Path for loading game is empty." << endl;
    return;
  }

  try
    {
    QByteArray ba = newPath.toLocal8Bit();
    const char* newPath_c = ba.data();

    BCEGame loadedGame;
    BCEGame::load(loadedGame,newPath_c);

    gameTab->setGame(loadedGame);

    tabWidget->setCurrentIndex(0);

    /* Note that the solution remains the same and isn't
       necessarily associated with the game. */

    }
  catch(BCEException & e)
    {
      displayException(QString("Load game didnt work :( from the BCEWindow class. ") +
		       QString("A BCEException was thrown with message: ") +
		       QString::fromStdString(e.getMessage()));
    }
  catch (std::exception & e)
    {
      displayException("Load game didnt work :( from the BCEWindow class");
    }

}

void BCEWindow::saveSolution() {

  QFileDialog saveSolnDialog(this,tr("Save a solution file"),
			     "../examples/",
			     tr("BCEViewer solution files (*.bce)"));
  saveSolnDialog.setDefaultSuffix("bce");

  if (saveSolnDialog.exec()
      && !saveSolnDialog.selectedFiles().isEmpty())
    {
      QString newPath = saveSolnDialog.selectedFiles().front();
      if (newPath.isEmpty())
	return;
  
      QFileInfo fi(newPath);

      try
	{
	  QByteArray ba = newPath.toLocal8Bit();
	  const char * newPath_c = ba.data();

	  BCESolution::save(solutionTab->getSolutionData(),
			newPath_c);
	}
      catch (std::exception & e)
	{
	  displayException("Save soln didnt work :( from BCEWindow.");
	}
    }
} // saveSolution

void BCEWindow::saveGame() {

  QFileDialog saveGameDialog(this,tr("Save a game file"),
			     "../examples/",
			     tr("BCEViewer game files (*.bgm)"));
  saveGameDialog.setDefaultSuffix("bgm");

  if (saveGameDialog.exec()
      && !saveGameDialog.selectedFiles().isEmpty())
    {
      QString newPath = saveGameDialog.selectedFiles().front();
      if (newPath.isEmpty())
	return;
  
      QFileInfo fi(newPath);

      try
	{
	  QByteArray ba = newPath.toLocal8Bit();
	  const char * newPath_c = ba.data();

	  BCEGame::save(gameTab->getGame(),
			newPath_c);
	}
      catch (std::exception & e)
	{
	  displayException("Save game didnt work :( from BCEWindow");
	}
    }
} // saveGame

void BCEWindow::runSolve() {

  // try
  //   {
      delete callback;
      callback = new BCEGurobiCallback();

      connect(this,SIGNAL(setCancelFlag()),
	      callback,SLOT(setCancelFlagTrue()));

      // Switch to the Log Tab (the third tab, so indexed at 2).
      tabWidget->setCurrentIndex(2);

      logTab->logText->append(QString(""));
      logTab->logText->append(QString("..........STARTING A NEW COMPUTATION.........."));
      logTab->logText->append(QString(""));
      
      // Reimplement when adding cancelGame()
      // cancelSolveFlag = false;
      
      QThread *solverWorkerThread = new QThread(this);
      solverWorker = new BCESolverWorker(gameTab->getGame(),
					 gameTab->getWeightsOnObjectives(),
					 gameTab->getMapBWeights(),
					 callback,
					 gameTab->getMapBoundaryOption(),
					 gameTab->getMinAngleIncr());
      solverWorker->moveToThread(solverWorkerThread);
      connect(solverWorker,SIGNAL(exceptionSignal(QString)),
	      this,SLOT(displayException(QString)));
      connect(solverWorkerThread,SIGNAL(started()),
	      solverWorker,SLOT(startSolve()));
      connect(solverWorker,SIGNAL(workFinished()),
	      solverWorkerThread,SLOT(quit()));
      connect(solverWorker,SIGNAL(sendSolution(BCESolution*)),
	      this,SLOT(tabToSolution(BCESolution*)));
      connect(solverWorkerThread,SIGNAL(finished()),
	      solverWorkerThread,SLOT(deleteLater()));
      connect(solverWorkerThread,SIGNAL(finished()),
	      solverWorker,SLOT(deleteLater()));
      solverWorkerThread->start();
  //   }
  // catch(BCEException &e) {
  //   displayException(QString::fromStdString(e.getMessage()));
  // }
  // catch(GRBException &e) {
  //   displayException(QString::fromStdString(e.getMessage() + 
  // 			     "The error code for this GRBException was: " +
  // 			     to_string(e.getErrorCode())));
  // }
  // catch(exception & e)
  //   {
  //     string str(e.what());
  //     QMessageBox::critical(this,tr("Solver failed"),
  // 			    QString::fromStdString("Gurobi was not able to solve your game. Message : " + str),
  // 			    QMessageBox::Ok);
  //   }
  
}

void BCEWindow::tabToSolution(BCESolution *soln) {
  solutionTab->setSolution(*soln);
  tabWidget->setCurrentIndex(1);
}

void BCEWindow::screenshot() {
  QString newPath = QFileDialog::getSaveFileName(this, tr("Save PNG"),
						 screenShotPath, tr("PNG files (*.png)"));
  if (newPath.isEmpty())
    return;
  newPath = newPath + ".png";
  QFileInfo fi(newPath);
  this->grab().save(newPath);
}

void BCEWindow::generateHybridAuction() {

  QDialog dialog(this);
  QFormLayout form(&dialog);

  form.addRow(new QLabel("Common Values Hybrid Auction"));

  int numParams = 6;
  vector<QLineEdit *> fields(numParams);
  for(int i = 0; i < numParams; ++i) {
    QLineEdit *lineEdit = new QLineEdit(&dialog);
    fields[i] = lineEdit;
  }

  form.addRow(QString("Number of Values (Integer)"), fields[0]);
  fields[0]->setText("30");
  form.addRow(QString("Number of Actions (Integer)"), fields[1]);
  fields[1]->setText("30");
  form.addRow(QString("Weight on Bid, 1 = FPA, 0 = SPA (Double in [0,1])"), fields[2]);
  fields[2]->setText("0.5");
  form.addRow(QString("Reserve Price (Double in [0,1])"), fields[3]);
  fields[3]->setText("0");
  form.addRow(QString("Entry Fee (Double in [0,1])"), fields[4]);
  fields[4]->setText("0");
  form.addRow(QString("High Bid (Double in [0,1])"), fields[5]);
  fields[5]->setText("1");

  QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
  			     Qt::Horizontal, &dialog);
  form.addRow(&buttonBox);
  QObject::connect(&buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));
  QObject::connect(&buttonBox, SIGNAL(rejected()), &dialog, SLOT(reject()));

  dialog.exec();

  if (dialog.result() == 0)
    return;

  vector<int> intParams(2);
  vector<double> doubleParams(4);

  for (int i=0; i < numParams; i++) {
    if (i<2)
      intParams[i] = fields[i]->text().toInt();
    else
      doubleParams[i-2] = fields[i]->text().toDouble();
  }

  HybridEntryReserve her(intParams[0],intParams[1],doubleParams[0],
  		     doubleParams[1],doubleParams[2],doubleParams[3]);

  // HybridEntryReserve her(30,30,1,.1,0,.75);
  gameTab->setGame(her);
  tabWidget->setCurrentIndex(0);

}

void BCEWindow::generateCommonValueAuction() {

  QDialog dialog(this);
  QFormLayout form(&dialog);

  form.addRow(new QLabel("Common Values Auction, Prior~v^alpha"));

  int numParams = 6;
  vector<QLineEdit *> fields(numParams);
  for(int i = 0; i < numParams; ++i) {
    QLineEdit *lineEdit = new QLineEdit(&dialog);
    fields[i] = lineEdit;
  }

  // Setting default values
  form.addRow(QString("Number of Values (Integer)"), fields[0]);
  fields[0]->setText("30");
  form.addRow(QString("Number of Actions (Integer)"), fields[1]);
  fields[1]->setText("30");
  form.addRow(QString("Alpha (Double > 0)"), fields[2]);
  fields[2]->setText("1");
  form.addRow(QString("High Bid (Double in [0,1])"), fields[3]);
  fields[3]->setText("1");
  form.addRow(QString("Reserve Price (Double in [0,1]"), fields[4]);
  fields[4]->setText("0");
  form.addRow(QString("Entry Fee (Double in [0,1])"), fields[5]);
  fields[5]->setText("0");

  QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
  			     Qt::Horizontal, &dialog);
  form.addRow(&buttonBox);
  QObject::connect(&buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));
  QObject::connect(&buttonBox, SIGNAL(rejected()), &dialog, SLOT(reject()));

  dialog.exec();

  if (dialog.result() == 0)
    return;

  vector<int> intParams(2);
  for (int i=0; i<intParams.size(); i++)
    intParams[i] = fields[i]->text().toInt();

  vector<double> doubleParams(4);
  for (int i=0; i < doubleParams.size(); i++) {
    doubleParams[i] = fields[i+2]->text().toDouble();
  }

  CommonValueAuction cva(intParams[0],intParams[1]
			 ,doubleParams[0],doubleParams[1],
			 doubleParams[2],doubleParams[3]);

  gameTab->setGame(cva);
  tabWidget->setCurrentIndex(0);

}

void BCEWindow::displayException(QString message) {
  QMessageBox msgBox;
  msgBox.setText("A BCEException was thrown with message: " 
		 + message);
  msgBox.exec();
}

void BCEWindow::conditionBCE(bool checked) {
  solutionTab->guiData->setConditionOnState(checked);
}
