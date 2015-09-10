#include "bcewindow.hpp"
#include "qcustomplot.h"
#include <QtWidgets>
#include <QMenuBar>
#include <QMainWindow>
#include "bceslider.hpp"
#include "bcedata.hpp"
#include "bceenumeration.hpp"

BCEWindow::BCEWindow() 
{
  path=QString("../examples/");
  screenShotPath=QString("../examples/screenshots/");

  // Resolution Settings

  resWidth = 1920;
  resHeight = 1080;

  // Menu Bar
  QMenu * fileMenu = menuBar()->addMenu(tr("&File"));
  QMenu * viewMenu = menuBar()->addMenu(tr("&View"));
  QAction * loadSolutionAction = new QAction(tr("&Load solution"),this);
  QAction * linearScale = new QAction(tr("&Linear/Log Color Scale Toggle)"),this);
  QAction * colorfulDistn = new QAction(tr("&Blue/Colorful Theme Toggle"),this);
  QAction * screenShotAction = new QAction(tr("&Save a screen shot"),this);
  QAction * quitAction = new QAction(tr("&Quit GUI"),this);
  fileMenu->addAction(loadSolutionAction);
  fileMenu->addAction(quitAction);
  viewMenu->addAction(linearScale);
  linearScale->setCheckable(true);
  linearScale->setChecked(true);
  viewMenu->addAction(colorfulDistn);
  colorfulDistn->setCheckable(true);
  colorfulDistn->setChecked(true);
  viewMenu->addAction(screenShotAction);
  loadSolutionAction->setShortcut(tr("Ctrl+L"));
  screenShotAction->setShortcut(tr("Ctrl+P"));
  quitAction->setShortcut(tr("Ctrl+W"));

  // Menu Connections
  connect(loadSolutionAction,SIGNAL(triggered()),this,SLOT(loadSolution()));
  connect(quitAction,SIGNAL(triggered()),this,SLOT(close()));
  connect(linearScale,SIGNAL(toggled(bool)),this,SLOT(toggleLinearScale(bool)));
  connect(colorfulDistn,SIGNAL(toggled(bool)),this,SLOT(toggleColorfulTheme(bool)));
  connect(screenShotAction,SIGNAL(triggered()),this,SLOT(screenShot()));

  ////////////////////////////////////////////////////
  // Slider, LineEdit, and CheckBox Controls Creation

  for (int player = 0; player < 2; player++) {
    // Note that player is irrelevant for private values
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
	    this,SLOT(changeSliderValue(int,BCESliderType,int)));
    connect(&gui,
	    SIGNAL(valueChanged(int,BCESliderType,int)),
	    lineEditGroup[widgetIndex],
	    SLOT(changeDisplayValue(int,BCESliderType,int)));
    connect(&gui,
	    SIGNAL(valueChanged(int,BCESliderType,int)),
	    sliderGroup[widgetIndex],
	    SLOT(changeSliderPosition(int,BCESliderType,int)));
    connect(checkBoxGroup[widgetIndex],
	    SIGNAL(boolChanged(bool,BCESliderType,int)),
	    this,SLOT(changeMarginalBool(bool,BCESliderType,int)));
  }

  // Slider Labels

  QVector<QLabel*> sliderLabels;

  sliderLabels.push_back(new QLabel(QApplication::translate("viewer","Player 0's Action")));
  sliderLabels.push_back(new QLabel(QApplication::translate("viewer","Player 0's Type")));
  sliderLabels.push_back(new QLabel(QApplication::translate("viewer","Player 0's Value")));
  sliderLabels.push_back(new QLabel(QApplication::translate("viewer","Player 1's Action")));
  sliderLabels.push_back(new QLabel(QApplication::translate("viewer","Player 1's Type")));
  sliderLabels.push_back(new QLabel(QApplication::translate("viewer","Player 1's Value")));

  for (int labelIt = 0; labelIt < 6; labelIt++)
    sliderLabels[labelIt]->setMaximumHeight(20);

  for (int i = 0; i < 2; i++)
    checkBoxGroup[3*i]->setChecked(true);

  // Slider, LineEdit, and CheckBox Layout Creation (Non Private-values)

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

  // End Slider, LineEdit, CheckBox Creation
  //////////////////////////////////////////

  // Payoff Plot Initialization
  payoffPlot = new BCEValueSetPlot();
  payoffPlot->xAxis->setLabel("Player 0");
  payoffPlot->yAxis->setLabel("Player 1");
  payoffPlot->setMinimumSize(resWidth/4,resHeight/3.5);
  connect(payoffPlot,SIGNAL(newEqmCoordinates(double,double)),
	  this,SLOT(setNewEqm(double,double)));

  // Bar Plot Initialization 
  deviationBarGraphs.push_back(new QCustomPlot());
  deviationBarGraphs[0]->xAxis->setLabel("Player 0's Actions");
  deviationBarGraphs.push_back(new QCustomPlot());
  deviationBarGraphs[1]->xAxis->setLabel("Player 1's Actions");
  for (int player = 0; player < 2; player++) {
    deviationBarGraphs[player]->yAxis->setLabel("Expected Payoff");
    deviationBarGraphs[player]->setMinimumHeight(resHeight/3.5);
  }

  // Payoff Plot and Sliders Horizontal Layout
  QHBoxLayout *topLeftPanel = new QHBoxLayout();
  topLeftPanel->addWidget(payoffPlot);
  topLeftPanel->addLayout(controlsGrid);

  // Left Viewer Panel, Bar Plots and Slider Box
  QVBoxLayout *leftSectorDivide = new QVBoxLayout();
  leftSectorDivide->addLayout(topLeftPanel);
  for (int player = 0; player < 2; player++)
    leftSectorDivide->addWidget(deviationBarGraphs[player]);

  ////////////////////////////////////////////////
  // Right Viewer Panel, Conditional-Marginal Distribution
  conditionalMarginalPlot = new QCustomPlot();

  // Plot Layout and Interaction Settings
  colorMap = new QCPColorMap(conditionalMarginalPlot->xAxis,
					  conditionalMarginalPlot->yAxis);

  conditionalMarginalPlot->addPlottable(colorMap);
  conditionalMarginalPlot->setInteractions(QCP::iRangeDrag|QCP::iRangeZoom);
  conditionalMarginalPlot->axisRect()->setupFullAxesBox(true);
  conditionalMarginalPlot->setMinimumWidth(resWidth/2); // 1080p resolution
  conditionalMarginalPlot->xAxis->setLabel("Player 0");
  conditionalMarginalPlot->yAxis->setLabel("Player 1");

  // Color Plottable Set-Up and Interaction Settings
  colorScale = new QCPColorScale(conditionalMarginalPlot);

  conditionalMarginalPlot->plotLayout()->addElement(0, 1, colorScale); 
  colorMap->setColorScale(colorScale); 
  colorScale->setType(QCPAxis::atRight); 
  colorScale->axis()->setLabel("Action Probability");
  colorScale->setRangeDrag(true);
  colorMap->setTightBoundary(true);

  // Color Scale Color Choice
  QCPColorGradient *mGradient = new QCPColorGradient();
  // mGradient->gpSpectrum;
  colorMap->setGradient(mGradient->gpSpectrum);
  colorMap->setInterpolate(false);

  // Line up Color Scale and Color Map
  QCPMarginGroup *marginGroup = new QCPMarginGroup(conditionalMarginalPlot);
  conditionalMarginalPlot->axisRect()->setMarginGroup(QCP::msBottom|QCP::msTop, marginGroup);
  colorScale->setMarginGroup(QCP::msBottom|QCP::msTop, marginGroup);

  // END CONDITIONAL-MARGINAL
  /////////////////////////////////////////////// 

  // Layout of Left and Right Viewer Panels
  QHBoxLayout *mainTab = new QHBoxLayout();
  mainTab->addLayout(leftSectorDivide);
  mainTab->addWidget(conditionalMarginalPlot);

  // Graph Tab Setup and Main Panel Initialization
  QWidget * mainPanel = new QWidget();
  QHBoxLayout * mainLayout = new QHBoxLayout();
  QWidget * graphTab = new QWidget();

  graphTab->setLayout(mainTab);

  // Main Widget
  QTabWidget *tabWidget = new QTabWidget();
  tabWidget->addTab(graphTab,"Graphs");
  mainLayout->addWidget(tabWidget);

  mainPanel->setLayout(mainLayout);

  // Set Default Tab 
  setCentralWidget(mainPanel);

  // Maximize Window by Default
  setWindowState(Qt::WindowMaximized);

  // Window Title
  setWindowTitle(QApplication::translate("viewer","BCESolve Work in Progress"));


} // constructor

/////////////////////////////////////////////
// Load Solution Slot

void BCEWindow::loadSolution() {

  QString newPath = QFileDialog::getOpenFileName(this,tr("Select a solution file"),
						 path,
						 tr("BCESolve solution files (*.bce)"));

  if (newPath.isEmpty())
    return;

  try
    {
      QByteArray ba = newPath.toLocal8Bit();
      const char * newPath_c = ba.data();

      gui.setData(newPath_c);
      
      // Set Up Sliders for Data

      for (int i = 0; i < 6; i++) {
	sliderGroup[i]->setRange(0,gui.shareDataProperties(i)-1);
	sliderGroup[i]->setSliderPosition(0);
	sliderGroup[i]->setSingleStep(1);
	lineEditGroup[i]->setText("0");
      }

      for (int player = 0; player < 2; player++) {
	gui.setSliderData(0,Action,player);
	gui.setSliderData(0,Type,player);
	gui.setSliderData(0,State,player);
      }     
    
      // Default Load Plot
      plotAllGraphics();

    }
  catch (std::exception & e)
    {
      qDebug() << "Load solution didnt work :(" << endl;
    }
} // slot for loading a solution

/////////////////////////////////////////////
// Plot Conditional-Marginal Distribution

void BCEWindow::plotEqm(vector<vector<double>> bceEqm) {

  colorMap->clearData();
  int nx = bceEqm[0].size();
  int ny = bceEqm[1].size();
  colorMap->data()->setSize(nx, ny); 
  colorMap->data()->setRange(QCPRange(0,nx), QCPRange(0,ny)); 
  double x, y;
  double maxEntry = 0;
  for (int xIndex=0; xIndex<nx; ++xIndex)
    {
      for (int yIndex=0; yIndex<ny; ++yIndex)
	{
	  double dataPoint = bceEqm[xIndex][yIndex];
	  colorMap->data()->setCell(yIndex, xIndex, dataPoint);
	  if (dataPoint > maxEntry)
	    maxEntry = dataPoint;
	}
    }

  colorScale->setDataRange(QCPRange(0,maxEntry*1.3));

  conditionalMarginalPlot->rescaleAxes();
  // conditionalMarginalPlot->xAxis->scaleRange(1.1,conditionalMarginalPlot->xAxis->range().center());
  // conditionalMarginalPlot->yAxis->scaleRange(1.1,conditionalMarginalPlot->yAxis->range().center());
  conditionalMarginalPlot->replot();

} // Plot Conditional-Marginal Distribution

////////////////////////////////////
// Plot Objectives

void BCEWindow::plotBCEValueSet() {

  // Erase Current Contents

  payoffPlot->clearGraphs();
  payoffPlot->clearPlottables();
  payoffPlot->addGraph();
  payoffPlot->addGraph();

  // Getting Data

  vector<vector<double>> objectives;
  objectives = gui.getObjectiveMatrix();

  QVector<double> objective0Payoffs;
  QVector<double> objective1Payoffs;

  for (int i = 0; i < objectives.size(); i++) {
    objective0Payoffs.push_back(objectives[i][gui.objective0]);
    objective1Payoffs.push_back(objectives[i][gui.objective1]);

  }

  // Graphing Curve

  QCPCurve * boundaryCurve = new QCPCurve(payoffPlot->xAxis,payoffPlot->yAxis);
  
  boundaryCurve->setData(objective0Payoffs,objective1Payoffs);

  payoffPlot->addPlottable(boundaryCurve);

  payoffPlot->graph(1)->setData(objective0Payoffs,objective1Payoffs);
  payoffPlot->graph(1)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc,
						       QPen(Qt::blue),
						       QBrush(Qt::blue),
						       5));
  payoffPlot->rescaleAxes();
  payoffPlot->xAxis->scaleRange(1.1, payoffPlot->xAxis->range().center());
  payoffPlot->yAxis->scaleRange(1.1, payoffPlot->yAxis->range().center());

  // Highlighting Current Equilibrium

  QVector<double> xCoordCurrentEqm;
  QVector<double> yCoordCurrentEqm;

  xCoordCurrentEqm.push_back(objective0Payoffs[gui.currentEqmIndex]);
  yCoordCurrentEqm.push_back(objective1Payoffs[gui.currentEqmIndex]);

  QPen pen;
  pen.setColor(Qt::green);
  pen.setWidth(4);

  payoffPlot->graph(0)->setData(xCoordCurrentEqm,yCoordCurrentEqm);
  payoffPlot->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCross,
							pen,
							QBrush(Qt::green),
							15));
  // Replot

  payoffPlot->replot();

}

////////////////////////////////////
// Plot Deviation Objectives

void BCEWindow::plotDeviationObjectives(int player, vector<vector<double>> devObjs) {

  // Erase Current Graphics

  deviationBarGraphs[player]->clearGraphs();
  deviationBarGraphs[player]->clearPlottables();

  // Setup Main Bar Graph

  QCPBars *barGraph = new QCPBars(deviationBarGraphs[player]->xAxis,
				   deviationBarGraphs[player]->yAxis);
  deviationBarGraphs[player]->addPlottable(barGraph);
  barGraph->setName("Expected Payoffs from Deviation");

  QVector<double> yData;

  yData = QVector<double>::fromStdVector(devObjs[player]);

  int sizeYData = yData.size();
  QVector<double> xData(sizeYData,0);

  for(int i = 0; i < sizeYData; i++) {
    xData[i]=(double)i;
  }

  barGraph->setData(xData,yData);
  barGraph->setPen(QColor(255, 131, 0));
  barGraph->setBrush(QColor(255, 131, 0, 50));

  // Highlight Recommended Action

  QCPBars *recAction = new QCPBars(deviationBarGraphs[player]->xAxis,
				   deviationBarGraphs[player]->yAxis);
  deviationBarGraphs[player]->addPlottable(recAction);

  int action = 0;
  switch(player) {
  case 0: action = gui.a0;
    break;
  case 1: action = gui.a1;
    break;
  }

  recAction->setData(QVector<double>(1,action),
		     QVector<double>(1,yData[action]));

  recAction->setPen(QPen(Qt::green));
  recAction->setBrush(QBrush(Qt::green));

  // Highlight Indifferent Deviations

  double maxDifference = .005; // .5% Difference for rounding
  double recValue = yData[action];

  QVector<double> xCoordIndiff;
  QVector<double> yCoordIndiff;

  for (int i = 0; i < sizeYData; i++) {
    if (abs((yData[i]-recValue)/recValue) < maxDifference) {
      xCoordIndiff.push_back(i);
      yCoordIndiff.push_back(yData[i]);
    }
    // cout << (yData[i]-recValue)/recValue << endl;
  }

  QCPBars *indiffAction = new QCPBars(deviationBarGraphs[player]->xAxis,
				      deviationBarGraphs[player]->yAxis);
  deviationBarGraphs[player]->addPlottable(indiffAction);

  indiffAction->setData(xCoordIndiff,yCoordIndiff);
  indiffAction->setPen(QColor(1, 92, 191));
  indiffAction->setBrush(QColor(1, 92, 191, 50));

  // Replot

  deviationBarGraphs[player]->rescaleAxes();
  deviationBarGraphs[player]->replot();

} // Plot Deviation Objectives

//////////////////////////////////
// Plot Graphics (Loading or Data Changed)

void BCEWindow::plotAllGraphics() {
    vector<vector<double>> conditionalMarginalDistn;
    conditionalMarginalDistn = gui.getEqmMatrix();
    vector<vector<double>> deviationObjectives0;
    vector<vector<double>> deviationObjectives1;
    deviationObjectives0 = gui.getObjectiveVals(0);
    deviationObjectives1 = gui.getObjectiveVals(1);
    plotEqm(conditionalMarginalDistn);
    plotDeviationObjectives(0,deviationObjectives0);
    plotDeviationObjectives(1,deviationObjectives1);
    plotBCEValueSet();
}

void BCEWindow::plotSelectGraphics(BCESliderType type,int player) {

  if (type==State) {
    vector<vector<double>> conditionalMarginalDistn;
    conditionalMarginalDistn = gui.getEqmMatrix();
    plotEqm(conditionalMarginalDistn);
  }

  else if (player==0) {
    vector<vector<double>> deviationObjectives0;
    deviationObjectives0 = gui.getObjectiveVals(0);
    plotDeviationObjectives(0,deviationObjectives0);
  }

  else if (player==1) {
    vector<vector<double>> deviationObjectives1;
    deviationObjectives1 = gui.getObjectiveVals(1);
    plotDeviationObjectives(1,deviationObjectives1);
  }

} // Plot Graphics

//////////////////////////////////
// Slider Slot

void BCEWindow::changeSliderValue(int newVal,
				  BCESliderType type,
				  int player) {
  gui.setSliderData(newVal,type,player);
  plotSelectGraphics(type,player);
} // Slot to alter Actions and Types

/////////////////////////////////
// Checkbox Slot

void BCEWindow::changeMarginalBool(bool newBool,
				   BCESliderType type,
				   int player) {
  gui.setMarginalConditions(newBool,type,player);
  plotSelectGraphics(type,player);
} // Slot to alter marginal distribution shown in Conditional Marginal  Plot

/////////////////////////////////
// BCE Value Set Slot

void BCEWindow::setNewEqm(double x,double y) {
  gui.modifyEqmFocus(x,y);
  plotAllGraphics();
}

/////////////////////////////////
// View Slots

void BCEWindow::toggleLinearScale(bool checked) {

  if (checked)
    colorScale->setDataScaleType(QCPAxis::stLinear);
  else 
    colorScale->setDataScaleType(QCPAxis::stLogarithmic);

  plotSelectGraphics(State,0);

} // Slot to set a linear color scale for the distribution's heat map

void BCEWindow::toggleColorfulTheme(bool checked) {

  QCPColorGradient *newGradient = new QCPColorGradient();

  if (checked)
    colorMap->setGradient(newGradient->gpSpectrum);
  else
    colorMap->setGradient(newGradient->inverted());

  plotSelectGraphics(State,0);

} // Slot to change color theme of heat map for conditional marginal distribution

void BCEWindow::screenShot() {

  QString newPath = QFileDialog::getSaveFileName(this, tr("Save PNG"),
						 screenShotPath, tr("PNG files (*.png)"));

  if (newPath.isEmpty())
    return;

  QFileInfo fi(newPath);

  grab().save(newPath);
}


