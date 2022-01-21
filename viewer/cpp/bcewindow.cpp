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
  QAction * generateHAGame = new QAction(tr("&Common Unknown Values Hybrid Auction"),this);
  QAction * generateCVGame = new QAction(tr("&Unknown Values First Price Auction"),this);
  QAction * generateFPAGame = new QAction(tr("&Independent Known Values First Price Auction"),this);
  QAction * solveOption = new QAction(tr("&Solve Game"),this);
  QAction * cancelOption = new QAction(tr("&Cancel Solve"),this);
  QMenu * helpMenu = menuBar()->addMenu(tr("&Help"));
  QAction * displayAboutScreen = new QAction(tr("&About"),this);

  // File Menu
  fileMenu->addAction(loadSolutionAction);
  fileMenu->addAction(loadGameAction);
  fileMenu->addSeparator();
  fileMenu->addAction(saveSolutionAction);
  fileMenu->addAction(saveGameAction);
  fileMenu->addSeparator();
  fileMenu->addAction(quitAction);
  loadSolutionAction->setShortcut(tr("Ctrl+L"));
  loadGameAction->setShortcut(tr("Ctrl+G"));
  quitAction->setShortcut(tr("Ctrl+W"));
  saveSolutionAction->setShortcut(tr("Ctrl+S"));

  // View Menu
  viewMenu->addAction(linearScale);
  linearScale->setCheckable(true);
  linearScale->setChecked(true);
  viewMenu->addAction(colorfulDistn);
  colorfulDistn->setCheckable(true);
  colorfulDistn->setChecked(true);
  viewMenu->addAction(screenShotAction);
  viewMenu->addAction(conditionalBCE);
  conditionalBCE->setCheckable(true);
  conditionalBCE->setChecked(true);
  screenShotAction->setShortcut(tr("Ctrl+P"));

  // Tools Menu
  QMenu * gamesMenu = toolMenu->addMenu(tr("Special &games"));
  gamesMenu->addAction(generateHAGame);
  gamesMenu->addAction(generateCVGame);
  gamesMenu->addAction(generateFPAGame);
  toolMenu->addSeparator();
  toolMenu->addAction(solveOption);
  toolMenu->addAction(cancelOption);

  // Help Menu
  helpMenu->addAction(displayAboutScreen);

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
  connect(generateFPAGame,SIGNAL(triggered()),this,SLOT(generateFirstPriceAuction()));
  connect(solveOption,SIGNAL(triggered()),this,SLOT(runSolve()));
  connect(cancelOption,SIGNAL(triggered()),this,SIGNAL(setCancelFlag()));
  connect(displayAboutScreen,SIGNAL(triggered()),this,SLOT(displayAboutFromToolBar()));

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
  try
    {
      PrisonerVsBattle pvb;
      BCEGame defaultGame(pvb);
      gameTab->setGame(defaultGame);
    }
  catch (BCEException & e)
    {
      qDebug() << "BCEException caught: " 
	       << e.what();
    }
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

    bool isMapped = loadedSolution.getIsBoundaryMapped();

    solutionTab->setSolution(loadedSolution,isMapped);
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
  saveSolnDialog.setAcceptMode(QFileDialog::AcceptSave);

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

  try
    {
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
      connect(solverWorker,SIGNAL(sendSolution(BCESolution*,bool)),
	      this,SLOT(tabToSolution(BCESolution*,bool)));
      connect(solverWorkerThread,SIGNAL(finished()),
	      solverWorkerThread,SLOT(deleteLater()));
      connect(solverWorkerThread,SIGNAL(finished()),
	      solverWorker,SLOT(deleteLater()));
      solverWorkerThread->start();
    }
  catch(BCEException &e) {
    displayException(QString::fromStdString(e.getMessage()));
  }
  catch(GRBException &e) {
    displayException(QString::fromStdString(e.getMessage() + 
  			     "The error code for this GRBException was: " +
  			     to_string(e.getErrorCode())));
  }
  catch(exception & e)
    {
      string str(e.what());
      QMessageBox::critical(this,tr("Solver failed"),
  			    QString::fromStdString("Gurobi was not able to solve your game. Message : " + str),
  			    QMessageBox::Ok);
    }
  
}

void BCEWindow::tabToSolution(BCESolution *soln,bool isBoundaryMapped) {
  solutionTab->setSolution(*soln,isBoundaryMapped);
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
  dialog.setSizePolicy(QSizePolicy::Fixed,
		       QSizePolicy::Fixed);

  form.addRow(new QLabel("Common Unknown Values Hybrid Auction <br><br>This auction encompasses the first price and second price common unknown values auctions<br>and permits exploration of the spectrum in between. The parameter <b>Weight on Bid</b> governs <br>the fraction player <i>i</i> pays of bid <i>i</i>, if player <i>i</i> wins. Winning as player <i>i</i> entails paying <br> WeightOnBid*Bid<sub>i</sub> + (1-WeightOnBid)*Bid<sub>j</sub> + Entry Fee. The parameter <b>High Bid</b> can be set below<br> 1 to limit the highest bid a bidder can make. Values are distributed uniformly on [0,1].<br><br>"));

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

  form.addRow(new QLabel("Common Unknown Values First Price Auction. <br><br> Values are distributed according to v<sup>&alpha;</sup>, where <i>v</i> takes on values in [0,1].<br><br>"));

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

  QCheckBox * independentCheck = new QCheckBox(&dialog);
  form.addRow(QString("Independent (versus common) values"), independentCheck);

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

  if (independentCheck->isChecked())
    {
      
      FPAUnknown fpa(intParams[1],intParams[0],
		     doubleParams[0],doubleParams[1],
		     doubleParams[2],doubleParams[3]);
      gameTab->setGame(fpa);
      tabWidget->setCurrentIndex(0);
    }
  else
    {
      CommonValueAuction cva(intParams[1],intParams[0]
			     ,doubleParams[0],doubleParams[1],
			     doubleParams[2],doubleParams[3]);

      gameTab->setGame(cva);
      tabWidget->setCurrentIndex(0);
    }
}

void BCEWindow::generateFirstPriceAuction() {

  QDialog dialog(this);
  QFormLayout form(&dialog);

  form.addRow(new QLabel("Independent Known Values First Price Auction <br><br>"));

  int numParams = 5;
  vector<QLineEdit *> fields(numParams);
  for(int i = 0; i < numParams; ++i) {
    QLineEdit *lineEdit = new QLineEdit(&dialog);
    fields[i] = lineEdit;
  }

  // Setting default values
  form.addRow(QString("Number of Actions (Integer > 0)"), fields[0]);
  fields[0]->setText("30");
  form.addRow(QString("Number of Values (Integer > 0)"), fields[1]);
  fields[1]->setText("2");
  form.addRow(QString("Entry Cost (Double in [0,1])"), fields[2]);
  fields[2]->setText("0");
  form.addRow(QString("Reserve Price (Double in [0,1]"), fields[3]);
  fields[3]->setText("0");
  form.addRow(QString("High Bid (Double in [0,1])"), fields[4]);
  fields[4]->setText("1");

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

  vector<double> doubleParams(3);
  for (int i=0; i < doubleParams.size(); i++) {
    doubleParams[i] = fields[i+intParams.size()]->text().toDouble();
  }

  FPAKnown fpa(intParams[0],intParams[1],
	       doubleParams[0],doubleParams[1],
	       doubleParams[2],false);
  gameTab->setGame(fpa);
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

void BCEWindow::displayAbout() {
  QMessageBox aboutBox;
  aboutBox.setTextFormat(Qt::RichText); 

  aboutBox.setWindowTitle("About");
  aboutBox.setText("BCEViewer is part of the BCESolve library for solving games with incomplete information.<br><br>Copyright (C) 2016 Benjamin A. Brooks and Robert J. Minton<br><br>BCESolve implements a Gurobi linear programming algorithm to solve for Bayes Correlated Equilibria. Use the tabs at the top left to navigate through the program. The game tab is used to specify and solve a game. The solution tab can be used to explore the solution object of a solved game. The log tab outputs progress of the algorithm while the solve routine is running. For more about the viewer, see the documentation. <br><br>For more information about Gurobi, see their <a href='http://www.gurobi.com/'>website</a>. To download Gurobi and acquire an academic license, see <a href='http://www.gurobi.com/registration/academic-license-reg'>this link</a>. <br><br>Inquiries can be directed to:<br><br>Ben Brooks<br>Chicago, IL\nben@benjaminbrooks.net<br>February 5, 2016");
  
  aboutBox.exec();
}

void BCEWindow::displayAboutFromToolBar() {
  displayAbout();
}
