#include "bcewindow.hpp"
#include "qcustomplot.h"
#include <QtWidgets>
#include <QMenuBar>
#include <QMainWindow>
#include "bceslider.hpp"
#include "bcedata.hpp"
#include "bceenumeration.hpp"
#include "bcedevplottitle.hpp"
#include "bcevaluesetplottitle.hpp"
#include "bceheatmaptitle.hpp"
#include "bcesliderlabel.hpp"

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
  QAction * linearScale = new QAction(tr("&Linear/Log Color Scale Toggle"),this);
  QAction * colorfulDistn = new QAction(tr("&Colorful/Blue Theme Toggle"),this);
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
	    &guiData,SLOT(setSliderValue(int,BCESliderType,int)));
    connect(&guiData,
	    SIGNAL(valueChanged(int,BCESliderType,int)),
	    lineEditGroup[widgetIndex],
	    SLOT(changeDisplayValue(int,BCESliderType,int)));
    connect(&guiData,
	    SIGNAL(valueChanged(int,BCESliderType,int)),
	    sliderGroup[widgetIndex],
	    SLOT(changeSliderPosition(int,BCESliderType,int)));
    connect(checkBoxGroup[widgetIndex],
	    SIGNAL(boolChanged(bool,BCESliderType,int)),
	    &guiData,SLOT(setMarginalConditions(bool,BCESliderType,int)));
  }

  // Slider Labels

  QVector<BCESliderLabel*> sliderLabels;

  for (int player = 0; player < 2; player++) {
    sliderLabels.push_back(new BCESliderLabel(Action,player));
    sliderLabels.push_back(new BCESliderLabel(Type,player));
    sliderLabels.push_back(new BCESliderLabel(State,player));
  }
	 
  for (int labelIt = 0; labelIt < 6; labelIt++) {
    sliderLabels[labelIt]->setMaximumHeight(resHeight/54);
    connect(&guiData,SIGNAL(sliderLabelsChanged(bool,int)),
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

  // End Slider, LineEdit, CheckBox Creation
  //////////////////////////////////////////
  // Data Plotting Connections

  connect(&guiData,SIGNAL(selectedEqmChanged()),
	  this,SLOT(plotBCEValueSet()));
  connect(&guiData,SIGNAL(objectiveValuesChanged(int)),
	  this,SLOT(plotDeviationObjectives(int)));
  connect(&guiData,SIGNAL(equilibriumMatrixChanged()),
	  this,SLOT(plotEqm()));
  connect(&guiData,SIGNAL(newDataLoaded()),
	  this,SLOT(plotBCEValueSet()));
  connect(this,SIGNAL(dataPathChanged(QString)),
	  &guiData,SLOT(setDataPath(QString)));
  connect(&guiData,SIGNAL(newDataLoaded()),
	  this,SLOT(setGUITitle()));

  // End Data Connections
  /////////////////////////////////////////
  // Plot Initializations and Organization
  
  // Payoff Plot Initialization
  BCEValueSetPlotTitle *setOfBCEPlotTitle = new BCEValueSetPlotTitle();
  setOfBCEPlot = new BCEValueSetPlot();
  setOfBCEPlot->xAxis->setLabel("Player 0");
  setOfBCEPlot->yAxis->setLabel("Player 1");
  setOfBCEPlot->setMinimumSize(resWidth/4,resHeight/3.5);
  connect(setOfBCEPlot,SIGNAL(newEqmCoordinates(double,double)),
	  &guiData,SLOT(modifyEqmFocus(double,double)));
  connect(&guiData,SIGNAL(eqmCoordSignal(double,double)),
	  setOfBCEPlotTitle,SLOT(changeDisplayedCoords(double,double)));

  // Bar Plot Initialization 
  deviationBarGraphs.push_back(new QCustomPlot());
  deviationBarGraphs[0]->xAxis->setLabel("Player 0's Actions");
  deviationBarGraphs.push_back(new QCustomPlot());
  deviationBarGraphs[1]->xAxis->setLabel("Player 1's Actions");
  for (int player = 0; player < 2; player++) {
    deviationBarGraphs[player]->yAxis->setLabel("Expected Payoff");
    deviationBarGraphs[player]->setMinimumHeight(resHeight/3.5);

    devPlotTitles.push_back(new BCEDevPlotTitle(player));
    connect(&guiData,
	    SIGNAL(devPlotTitleChange(int,int,int,double)),
	    devPlotTitles[player],
	    SLOT(changeText(int,int,int,double)));
    connect(&guiData,
	    SIGNAL(devPlotPrChange(double)),
	    devPlotTitles[player],
	    SLOT(changeProbability(double)));
  }

  // BCE Set Plot and Sliders Horizontal Layout
  QVBoxLayout *setOfBCEPlotWithTitle = new QVBoxLayout();
  setOfBCEPlotWithTitle->addWidget(setOfBCEPlotTitle);
  setOfBCEPlotWithTitle->addWidget(setOfBCEPlot);
  QHBoxLayout *topLeftPanel = new QHBoxLayout();
  topLeftPanel->addLayout(setOfBCEPlotWithTitle);
  topLeftPanel->addLayout(controlsGrid);

  // Left Viewer Panel, Bar Plots and Slider Box
  QVBoxLayout *leftSectorDivide = new QVBoxLayout();
  leftSectorDivide->addLayout(topLeftPanel);
  for (int player = 0; player < 2; player++) {
    leftSectorDivide->addWidget(devPlotTitles[player]);
    leftSectorDivide->addWidget(deviationBarGraphs[player]);
  }

  // Right Viewer Panel, Conditional-Marginal Distribution
  conditionalMarginalPlot = new BCEValueSetPlot();
  BCEHeatMapTitle *colorMapTitle = new BCEHeatMapTitle();
  colorMapTitle->setMaximumHeight(resHeight/54);
  connect(&guiData,SIGNAL(newStateSignal(int,int,int,bool)),
	  colorMapTitle,SLOT(changeDisplayedState(int,int,int,bool)));

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

  // Color Scale Color Choice
  QCPColorGradient *mGradient = new QCPColorGradient();
  // mGradient->gpSpectrum;
  colorMap->setGradient(mGradient->gpSpectrum);
  colorMap->setInterpolate(false);

  // Line up Color Scale and Color Map
  QCPMarginGroup *marginGroup = new QCPMarginGroup(conditionalMarginalPlot);
  conditionalMarginalPlot->axisRect()->setMarginGroup(QCP::msBottom|QCP::msTop, marginGroup);
  colorScale->setMarginGroup(QCP::msBottom|QCP::msTop, marginGroup);

  // Layout of Left and Right Viewer Panels
  QVBoxLayout *colorMapWithTitle = new QVBoxLayout();
  colorMapWithTitle->addWidget(colorMapTitle);
  colorMapWithTitle->addWidget(conditionalMarginalPlot);
  QHBoxLayout *mainTab = new QHBoxLayout();
  mainTab->addLayout(leftSectorDivide);
  mainTab->addLayout(colorMapWithTitle);

  // End Plot Initializations and Organization
  /////////////////////////////////////////////// 

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
  setWindowTitle(QApplication::translate("bceviewer","BCE Solution Viewer"));


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
      emit(dataPathChanged(newPath));
      
      // Set Up Sliders for Data

      for (int player = 0; player < 2; player++) {
	sliderGroup[3*player]->setRange(0,guiData.shareDataProperties(Action,player)-1);
	sliderGroup[3*player+1]->setRange(0,guiData.shareDataProperties(Type,player)-1);
	sliderGroup[3*player+2]->setRange(0,guiData.shareDataProperties(State,player)-1);
      }

      for (int i = 0; i < 6; i++) {
	sliderGroup[i]->setSliderPosition(0);
	sliderGroup[i]->setSingleStep(1);
	lineEditGroup[i]->setText("0");
      }

    }
  catch (std::exception & e)
    {
      qDebug() << "Load solution didnt work :(" << endl;
    }
} // slot for loading a solution

/////////////////////////////////////////////
// Plot Conditional-Marginal Distribution

void BCEWindow::plotEqm() {

  vector<vector<double>> eqmMatrix = guiData.getEqmMatrix();

  colorMap->clearData();
  int nx = eqmMatrix[0].size();
  int ny = eqmMatrix[1].size();
  colorMap->data()->setSize(nx, ny); 
  colorMap->data()->setRange(QCPRange(0,nx), QCPRange(0,ny)); 
  double x, y;
  double maxEntry = 0;
  for (int xIndex=0; xIndex<nx; ++xIndex)
    {
      for (int yIndex=0; yIndex<ny; ++yIndex)
	{
	  double dataPoint = eqmMatrix[xIndex][yIndex];
	  colorMap->data()->setCell(xIndex,yIndex,dataPoint);
	  if (dataPoint > maxEntry)
	    maxEntry = dataPoint;
	}
    }

  colorScale->setDataRange(QCPRange(0,maxEntry*1.3));

  conditionalMarginalPlot->rescaleAxes();
  conditionalMarginalPlot->xAxis->scaleRange(1.05,conditionalMarginalPlot->xAxis->range().center());
  conditionalMarginalPlot->yAxis->scaleRange(1.05,conditionalMarginalPlot->yAxis->range().center());
  conditionalMarginalPlot->replot();

} // Plot Conditional-Marginal Distribution

////////////////////////////////////
// Plot Set of BCE

void BCEWindow::plotBCEValueSet() {

  // Erase Current Contents

  setOfBCEPlot->clearGraphs();
  setOfBCEPlot->clearPlottables();
  setOfBCEPlot->addGraph();
  setOfBCEPlot->addGraph();

  // Getting Data

  vector<vector<double>> allEqm = guiData.getAllEqm();
  QVector<double> objective0Payoffs;
  QVector<double> objective1Payoffs;
  vector<int> playerObjectives;

  for (int player = 0; player < 2; player++)
    playerObjectives.push_back(guiData.getPlayerObjective(player));

  for (int i = 0; i < allEqm.size(); i++) {
    objective0Payoffs.push_back(allEqm[i][playerObjectives[0]]);
    objective1Payoffs.push_back(allEqm[i][playerObjectives[1]]);
  }

  // Graphing Curve

  QCPCurve * boundaryCurve = new QCPCurve(setOfBCEPlot->xAxis,setOfBCEPlot->yAxis);
  
  boundaryCurve->setData(objective0Payoffs,objective1Payoffs);

  setOfBCEPlot->addPlottable(boundaryCurve);

  setOfBCEPlot->graph(1)->setData(objective0Payoffs,objective1Payoffs);
  setOfBCEPlot->graph(1)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc,
							QPen(Qt::blue),
							QBrush(Qt::blue),
							5));
  setOfBCEPlot->rescaleAxes();
  setOfBCEPlot->xAxis->scaleRange(1.1, setOfBCEPlot->xAxis->range().center());
  setOfBCEPlot->yAxis->scaleRange(1.1, setOfBCEPlot->yAxis->range().center());

  // Highlighting Current Equilibrium

  QVector<double> xCoordCurrentEqm;
  QVector<double> yCoordCurrentEqm;
  int currentEqmIndex = guiData.getCurrentEqmIndex();

  xCoordCurrentEqm.push_back(objective0Payoffs[currentEqmIndex]);
  yCoordCurrentEqm.push_back(objective1Payoffs[currentEqmIndex]);

  QPen pen;
  pen.setColor(Qt::green);
  pen.setWidth(4);

  setOfBCEPlot->graph(0)->setData(xCoordCurrentEqm,yCoordCurrentEqm);
  setOfBCEPlot->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCross,
							pen,
							QBrush(Qt::green),
							15));
  // Replot

  setOfBCEPlot->replot();

}

////////////////////////////////////
// Plot Deviation Objectives

void BCEWindow::plotDeviationObjectives(int player) {

  // Erase Current Graphics

  deviationBarGraphs[player]->clearGraphs();
  deviationBarGraphs[player]->clearPlottables();

  // Setup Main Bar Graph

  QCPBars *barGraph = new QCPBars(deviationBarGraphs[player]->xAxis,
				  deviationBarGraphs[player]->yAxis);
  deviationBarGraphs[player]->addPlottable(barGraph);
  barGraph->setName("Expected Payoffs from Deviation");

  vector<vector<double>> objectiveValues = guiData.getObjectiveValues();

  QVector<double> yData;

  yData = QVector<double>::fromStdVector(objectiveValues[player]);

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

  int action = guiData.getCurrentSliderVal(Action,player);

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

/////////////////////////////////
// View Slots

void BCEWindow::toggleLinearScale(bool checked) {
  if (checked)
    colorScale->setDataScaleType(QCPAxis::stLinear);
  else 
    colorScale->setDataScaleType(QCPAxis::stLogarithmic);
  plotEqm();
} // Slot to set a linear color scale for the distribution's heat map

void BCEWindow::toggleColorfulTheme(bool checked) {
  QCPColorGradient *newGradient = new QCPColorGradient();
  if (checked)
    colorMap->setGradient(newGradient->gpSpectrum);
  else
    colorMap->setGradient(newGradient->inverted());
  plotEqm();
} // Slot to change color theme of heat map for conditional marginal distribution

void BCEWindow::screenShot() {
  QString newPath = QFileDialog::getSaveFileName(this, tr("Save PNG"),
						 screenShotPath, tr("PNG files (*.png)"));
  if (newPath.isEmpty())
    return;
  QFileInfo fi(newPath);
  grab().save(newPath);
}

void BCEWindow::setGUITitle() {

  stringstream dynamicTitle;

  string guiTitle = guiData.getGUITitle();

  dynamicTitle << "BCE Solution Viewer, Current File = "
	       << guiTitle;
    
  string newTitleStr = dynamicTitle.str();
  QString newTitle = QString::fromStdString(newTitleStr);

  this->setWindowTitle(newTitle);
}

