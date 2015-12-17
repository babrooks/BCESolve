#include "bcewindow.hpp"
#include "bcegame.hpp"
#include <QtWidgets>
#include "bcesolverworker.hpp"
#include "bcelogstream.hpp"

BCEWindow::BCEWindow() {

  // Set the default path for loading examples.
  path=QString("../examples/");

  // Menu Bar
  QMenu * fileMenu = menuBar()->addMenu(tr("&File"));
  QMenu * viewMenu = menuBar()->addMenu(tr("&View"));
  QAction * loadSolutionAction = new QAction(tr("&Load Solution"),this);
  QAction * loadGameAction = new QAction(tr("&Load Game"),this);
  QAction * saveSolutionAction = new QAction(tr("&Save Solution"),this);
  QAction * saveGameAction = new QAction(tr("&Save Game"),this);
  QAction * linearScale = new QAction(tr("&Linear/Log Color Scale Toggle"),this);
  QAction * colorfulDistn = new QAction(tr("&Colorful/Blue Theme Toggle"),this);
  QAction * screenShotAction = new QAction(tr("&Save a screen shot"),this);
  QAction * quitAction = new QAction(tr("&Quit GUI"),this);
  fileMenu->addAction(loadSolutionAction);
  fileMenu->addAction(loadGameAction);
  fileMenu->addAction(quitAction);
  fileMenu->addAction(saveSolutionAction);
  fileMenu->addAction(saveGameAction);
  viewMenu->addAction(linearScale);
  linearScale->setCheckable(true);
  linearScale->setChecked(true);
  viewMenu->addAction(colorfulDistn);
  colorfulDistn->setCheckable(true);
  colorfulDistn->setChecked(true);
  viewMenu->addAction(screenShotAction);
  loadSolutionAction->setShortcut(tr("Ctrl+L"));
  screenShotAction->setShortcut(tr("Ctrl+P"));
  quitAction->setShortcut(tr("Ctrl+Q"));
  saveSolutionAction->setShortcut(tr("Ctrl+S"));

  // Menu Connections
  connect(loadSolutionAction,SIGNAL(triggered()),this,SLOT(loadSolution()));
  connect(loadGameAction,SIGNAL(triggered()),this,SLOT(loadGame()));
  connect(saveSolutionAction,SIGNAL(triggered()),this,SLOT(saveSolution()));
  connect(saveGameAction,SIGNAL(triggered()),this,SLOT(saveGame()));
  connect(quitAction,SIGNAL(triggered()),this,SLOT(close()));
  connect(linearScale,SIGNAL(toggled(bool)),&solutionTab,SLOT(toggleLinearScale(bool)));
  connect(colorfulDistn,SIGNAL(toggled(bool)),&solutionTab,SLOT(toggleColorfulTheme(bool)));
  connect(screenShotAction,SIGNAL(triggered()),&solutionTab,SLOT(screenShot()));

  // Loading Connection
  connect(this,SIGNAL(dataPathChanged(QString)),
  	  &solutionTab,SLOT(loadData(QString)));

  // Solve Routine Connections
  connect(&gameTab,SIGNAL(startSolveRoutine()),
	  this,SLOT(runSolve()));

  // Layout Setup
  QTabWidget *tabWidget = new QTabWidget();
  QWidget *solutionTabWidget = new QWidget();
  solutionTabWidget->setLayout(solutionTab.getLayout());
  QWidget *gameTabWidget = new QWidget();
  gameTabWidget->setLayout(gameTab.getLayout());
  QWidget *logTabWidget = new QWidget();
  logTabWidget->setLayout(logTab.getLayout());

  tabWidget->addTab(solutionTabWidget,"Solution");
  tabWidget->addTab(gameTabWidget,"Game");
  tabWidget->addTab(logTabWidget,"Log");

  QHBoxLayout *mainLayout = new QHBoxLayout();
  mainLayout->addWidget(tabWidget);

  QWidget *mainPanel = new QWidget();
  mainPanel->setLayout(mainLayout);

  // Set Default Tab 
  setCentralWidget(mainPanel);

  // Maximize Window by Default
  setWindowState(Qt::WindowMaximized);

  // Window Title
  setWindowTitle(QApplication::translate("bceviewer","BCE Solution Viewer"));

} // Default Constructor

void BCEWindow::loadSolution() {

  QString newPath = QFileDialog::getOpenFileName(this,tr("Select a Solution File"),
						 path,
						 tr("BCESolve Solution Files (*.bce)"));


  if (newPath.isEmpty()) {
    cout << "isEmpty" << endl;
    return;
  }

  try
    {
    QByteArray ba = newPath.toLocal8Bit();
    const char* newPath_c = ba.data();

    BCESolution loadedSolution;
    BCESolution::load(loadedSolution,newPath_c);
    BCEGame loadedGame = loadedSolution.getGame();

    solutionTab.setSolution(loadedSolution);
    gameTab.setGame(loadedGame);

    emit(dataPathChanged(newPath));

    }
  catch (std::exception & e)
    {
      qDebug() << "Load solution didnt work :( from BCEPlotHandler" << endl;
    }

}

void BCEWindow::loadGame() {

  QString newPath = QFileDialog::getOpenFileName(this,tr("Select a Game File"),
						 path,
						 tr("BCESolve Game Files (*.bce)"));


  if (newPath.isEmpty()) {
    cout << "isEmpty" << endl;
    return;
  }

  try
    {
    QByteArray ba = newPath.toLocal8Bit();
    const char* newPath_c = ba.data();

    BCEGame loadedGame;
    BCEGame::load(loadedGame,newPath_c);

    gameTab.setGame(loadedGame);

    emit(dataPathChanged(newPath));

    /* Note that the solution remains the same and isn't
       necessarily associated with the game. */

    }
  catch (std::exception & e)
    {
      qDebug() << "Load solution didnt work :( from BCEPlotHandler" << endl;
    }

}

void BCEWindow::saveSolution() {

  QString newPath = QFileDialog::getSaveFileName(this,tr("Select a solution file"),
						 "./",
						 tr("BCEViewer solution files (*.bce)"));

  if (newPath.isEmpty())
    return;

  QFileInfo fi(newPath);
  path = fi.canonicalPath();

  try
    {
      QByteArray ba = newPath.toLocal8Bit();
      const char * newPath_c = ba.data();

      BCESolution::save(solutionTab.getSolutionData(),
		       newPath_c);
    }
  catch (std::exception & e)
    {
      qDebug() << "Save solution didnt work :(" << endl;
    }
} // saveSolution

void BCEWindow::saveGame() {

  QString newPath = QFileDialog::getSaveFileName(this,tr("Select a game file"),
						 "./",
						 tr("BCEViewer game files (*.bce)"));

  if (newPath.isEmpty())
    return;

  QFileInfo fi(newPath);
  path = fi.canonicalPath();

  try
    {
      QByteArray ba = newPath.toLocal8Bit();
      const char * newPath_c = ba.data();

      BCEGame::save(gameTab.getGame(),
		   newPath_c);
    }
  catch (std::exception & e)
    {
      qDebug() << "Save game didnt work :(" << endl;
    }
} // saveGame

void BCEWindow::runSolve() {
  try
    {
      // Redirect all subsequent cout to the BCELogHandler.
      BCELogStream qout(std::cout,logTab.logText);

      logTab.logText->append(QString(""));
      logTab.logText->append(QString("..........STARTING A NEW COMPUTATION.........."));
      logTab.logText->append(QString(""));

      // std::cout << "Testing cout Redirect from runSolve()" << std::endl;
      
      // Reimplement when adding cancelGame()
      // cancelSolveFlag = false;
      
      BCESolverWorker solverWorker(gameTab.getGame());

      solverWorker.moveToThread(&solverWorkerThread);
      solverWorker.startSolve();
      // connect(solverWorker,SIGNAL(resultReady(bool)),
      // 	      this,SLOT(iterationFinished(bool)));
      // connect(solverWorker,SIGNAL(exceptionCaught()),
      // 	      this,SLOT(solverException()));
      connect(&solverWorkerThread,SIGNAL(finished()),
	      &solverWorkerThread,SLOT(deleteLater()));
      solverWorkerThread.start();
      
      // timer.restart();
      
      // emit startIteration();
    }
  catch (exception & e)
    {
      QMessageBox::critical(this,tr("Solver failed"),
			    tr("CPLEX was not able to solve your game."),
			    QMessageBox::Ok);
    }
}
