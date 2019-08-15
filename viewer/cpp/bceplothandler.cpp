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

#include "bceplothandler.hpp"

BCEPlotHandler::BCEPlotHandler(QWidget *parent):
  deviationBarGraphs(4),devPlotTitles(4)
{
  isSolnDataLoaded=false;
  guiData = new BCEDataState();
  setupLayout();
  setParent(parent);

} // constructor

void BCEPlotHandler::setupLayout() {
  // BCEDataState Connections to BCEWindow

  connect(guiData,SIGNAL(selectedEqmChanged()),
	  this,SLOT(plotBCEValueSet()));
  connect(guiData,SIGNAL(objectiveValuesChanged(int)),
	  this,SLOT(plotDeviationObjectives(int)));
  connect(guiData,SIGNAL(equilibriumMatrixChanged()),
	  this,SLOT(plotEqm()));
  connect(guiData,SIGNAL(newDataLoaded()),
	  this,SLOT(setGUITitle()));
  connect(guiData,SIGNAL(newDataLoaded()),
	  this,SLOT(indicateDataLoaded()));

  // End Data Connections
  /////////////////////////////////////////
  // Plot Initializations and Organization

  QFont font;
  font.setPointSize(12);

  QSizePolicy sp(QSizePolicy::Expanding,QSizePolicy::Expanding);
  sp.setVerticalStretch(1);
  sp.setHorizontalStretch(1);

  // Payoff Plot Initialization
  setOfBCEPlot = new BCEValueSetPlot();
  BCEPlotTitle *setOfBCEPlotTitle = new BCEPlotTitle(ValueSetPlot,
						     setOfBCEPlot);
  setOfBCEPlot->setInteractions(QCP::iRangeDrag|QCP::iRangeZoom);
  setOfBCEPlotTitle->setFont(font);
  setOfBCEPlot->plotLayout()->insertRow(0);
  setOfBCEPlot->
    plotLayout()->
    addElement(0,0,setOfBCEPlotTitle);

  setOfBCEPlot->xAxis->setLabel("Objective 0");
  setOfBCEPlot->yAxis->setLabel("Objective 1");
  connect(setOfBCEPlot,SIGNAL(newEqmCoordinates(double,double)),
	  guiData,SLOT(modifyEqmFocus(double,double)));
  connect(guiData,SIGNAL(eqmCoordSignal(double,double)),
	  setOfBCEPlotTitle,SLOT(changeDisplayedCoords(double,double)));

  // Bar Plot Initialization
  deviationBarGraphs[0] = new BCEValueSetPlot();
  deviationBarGraphs[0]->setSizePolicy(sp);
  deviationBarGraphs[0]->xAxis->setLabel("Player 0's Actions");
  deviationBarGraphs[1] = new BCEValueSetPlot();
  deviationBarGraphs[1]->setSizePolicy(sp);
  deviationBarGraphs[1]->xAxis->setLabel("Player 1's Actions");
  for (int player = 0; player < 2; player++) {
    deviationBarGraphs[player]->yAxis->setLabel("Expected Payoff");
    devPlotTitles[player] = new BCEPlotTitle(DeviationPlot,
					     player,
					     deviationBarGraphs[player]);
    devPlotTitles[player]->setFont(font);

    deviationBarGraphs[player]->plotLayout()->insertRow(0);
    deviationBarGraphs[player]->
      plotLayout()->
      addElement(0,0,devPlotTitles[player]);

    connect(guiData,
	    SIGNAL(devPlotTitleChange(int,int,int,double,double)),
	    devPlotTitles[player],
	    SLOT(changeText(int,int,int,double,double)));
    connect(guiData,
	    SIGNAL(devPlotPrChange(int,double)),
	    devPlotTitles[player],
	    SLOT(changeProbability(int,double)));
    }


    deviationBarGraphs[2] = new BCEValueSetPlot();
    deviationBarGraphs[2]->setSizePolicy(sp);
    deviationBarGraphs[2]->xAxis->setLabel("Player 0's Actions");
    deviationBarGraphs[3] = new BCEValueSetPlot();
    deviationBarGraphs[3]->setSizePolicy(sp);
    deviationBarGraphs[3]->xAxis->setLabel("Player 1's Actions");
    for (int player = 2; player < 4; player++) {
      deviationBarGraphs[player]->yAxis->setLabel("Multiplier");
      devPlotTitles[player] = new BCEPlotTitle(DeviationPlot,
                         player, //flip
                         deviationBarGraphs[player]);
      devPlotTitles[player]->setFont(font);

      deviationBarGraphs[player]->plotLayout()->insertRow(0);
      deviationBarGraphs[player]->
        plotLayout()->
        addElement(0,0,devPlotTitles[player]);

      connect(guiData,
        SIGNAL(devPlotTitleChange(int,int,int,double,double)),
        devPlotTitles[player],
        SLOT(changeText(int,int,int,double,double)));
      connect(guiData,
        SIGNAL(devPlotPrChange(int,double)),
        devPlotTitles[player],
        SLOT(changeProbability(int,double)));
  }

  QSizePolicy sp2(QSizePolicy::Expanding,QSizePolicy::Expanding);
  sp.setVerticalStretch(1);
  sp.setHorizontalStretch(3);

  QSizePolicy sp3(QSizePolicy::Preferred,QSizePolicy::Preferred);
  sp.setVerticalStretch(1);
  sp.setHorizontalStretch(1);

  setOfBCEPlot->setSizePolicy(sp2);

  QHBoxLayout *topLeftPanel = new QHBoxLayout();
  topLeftPanel->addWidget(setOfBCEPlot);
  guiData->controlsLayout->setSizePolicy(sp3);
  topLeftPanel->addWidget(guiData->controlsLayout);

  // Left Viewer Panel, Bar Plots and Slider Box
  QVBoxLayout *leftSectorDivide = new QVBoxLayout();

  QTabWidget *TabWidget;
  TabWidget = new QTabWidget(this);

  QWidget *ObjectiveTabWidget = new QWidget(this);
  QVBoxLayout *ObjectiveTabDivide = new QVBoxLayout();
  QWidget *MultiplierTabWidget = new QWidget(this);
  QVBoxLayout *MultiplierTabDivide = new QVBoxLayout();
  for (int player = 0; player < 2; player++) {
    ObjectiveTabDivide->addWidget(deviationBarGraphs[player]);
    MultiplierTabDivide->addWidget(deviationBarGraphs[player+2]);
  }
  ObjectiveTabWidget->setLayout(ObjectiveTabDivide);
  MultiplierTabWidget->setLayout(MultiplierTabDivide);

  TabWidget->addTab(ObjectiveTabWidget,"Deviation Payoffs");
  TabWidget->addTab(MultiplierTabWidget,"I.C. Multipliers");
  leftSectorDivide->addWidget(TabWidget);//

  // Right Viewer Panel, Conditional-Marginal Distribution
  conditionalMarginalPlot = new BCEValueSetPlot();
  conditionalMarginalPlot->setSizePolicy(sp);

  // Plot Layout and Interaction Settings
  colorMap = new QCPColorMap(conditionalMarginalPlot->xAxis,
			     conditionalMarginalPlot->yAxis);
  conditionalMarginalPlot->addPlottable(colorMap);
  conditionalMarginalPlot->setInteractions(QCP::iRangeDrag|QCP::iRangeZoom);
  conditionalMarginalPlot->axisRect()->setupFullAxesBox(true);
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

  BCEPlotTitle *colorMapTitle = new BCEPlotTitle(HeatMap,
						 conditionalMarginalPlot);
  colorMapTitle->setFont(font);
  conditionalMarginalPlot->plotLayout()->insertRow(0);
  conditionalMarginalPlot->
    plotLayout()->
    addElement(0,0,colorMapTitle);
  connect(guiData,SIGNAL(newStateSignal(int,int,int,bool)),
	  colorMapTitle,SLOT(changeDisplayedState(int,int,int,bool)));

  QSizePolicy sp1(QSizePolicy::Expanding,QSizePolicy::Expanding);
  sp1.setVerticalStretch(2);
  sp1.setHorizontalStretch(1);

  // Layout of Left and Right Viewer Panels
  QVBoxLayout *rightSide = new QVBoxLayout();
  rightSide->addWidget(conditionalMarginalPlot);
  QVBoxLayout *leftSide = new QVBoxLayout();
  QSplitter *leftSideSplitter = new QSplitter(Qt::Vertical);
  QWidget *topQSplit = new QWidget();
  topQSplit->setLayout(topLeftPanel);
  topQSplit->setSizePolicy(sp);
  QWidget *bottomQSplit = new QWidget();
  bottomQSplit->setLayout(leftSectorDivide);
  bottomQSplit->setSizePolicy(sp1);
  leftSideSplitter->addWidget(topQSplit);
  leftSideSplitter->addWidget(bottomQSplit);

  QWidget *rightSideWidget = new QWidget();
  rightSideWidget->setLayout(rightSide);

  QSplitter *mainSplitter = new QSplitter();
  mainSplitter->addWidget(leftSideSplitter);
  mainSplitter->addWidget(rightSideWidget);
  mainSplitter->setStretchFactor(0,4);
  mainSplitter->setStretchFactor(1,10);

  mainTab = new QHBoxLayout();
  mainTab->addWidget(mainSplitter);

  // End Plot Initializations and Organization
  ///////////////////////////////////////////////
}

/////////////////////////////////////////////
// Plot Conditional-Marginal Distribution

void BCEPlotHandler::plotEqm() {

  const vector< vector<double> > eqmMatrix(guiData->getEqmMatrix());

  colorMap->clearData();
  int nx = eqmMatrix.size();
  int ny = eqmMatrix[0].size();
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

void BCEPlotHandler::plotBCEValueSet() {

  // Erase Current Contents
  setOfBCEPlot->clearGraphs();
  setOfBCEPlot->clearPlottables();
  // setOfBCEPlot->addGraph();
  // setOfBCEPlot->addGraph();

  // Getting Data

  const vector< vector<double> > allEqm(guiData->getAllEqm());
  int sizeEqmPayoffs = allEqm[0].size();
  QVector<double> objective0Payoffs(sizeEqmPayoffs,0);
  QVector<double> objective1Payoffs(sizeEqmPayoffs,0);

  for (int i = 0; i < sizeEqmPayoffs; i++) {
    objective0Payoffs[i] = allEqm[0][i];
    objective1Payoffs[i] = allEqm[1][i];
  }

  // Graphing Curve

  QCPCurve * boundaryCurve = new QCPCurve(setOfBCEPlot->xAxis,setOfBCEPlot->yAxis);

  boundaryCurve->setData(objective0Payoffs,objective1Payoffs);

  boundaryCurve->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc,
							QPen(Qt::blue),
							QBrush(Qt::blue),
							5));
  setOfBCEPlot->addPlottable(boundaryCurve);

  setOfBCEPlot->rescaleAxes();
  setOfBCEPlot->xAxis->scaleRange(1.1, setOfBCEPlot->xAxis->range().center());
  setOfBCEPlot->yAxis->scaleRange(1.1, setOfBCEPlot->yAxis->range().center());

  // Highlighting Current Equilibrium

  QVector<double> xCoordCurrentEqm;
  QVector<double> yCoordCurrentEqm;
  int currentEqmIndex = guiData->getCurrentEqmIndex();

  xCoordCurrentEqm.push_back(objective0Payoffs[currentEqmIndex]);
  yCoordCurrentEqm.push_back(objective1Payoffs[currentEqmIndex]);

  QPen pen;
  pen.setColor(Qt::green);
  pen.setWidth(4);

  QCPCurve * xCurve = new QCPCurve(setOfBCEPlot->xAxis,setOfBCEPlot->yAxis);
  xCurve->setData(xCoordCurrentEqm,yCoordCurrentEqm);
  xCurve->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCross,
					  pen,
					  QBrush(Qt::green),
					  13));
  setOfBCEPlot->addPlottable(xCurve);
  // Replot

  setOfBCEPlot->replot();

}

////////////////////////////////////
// Plot Deviation Objectives

void BCEPlotHandler::plotDeviationObjectives(int player) {

  // Erase Current Graphics

  deviationBarGraphs[player]->clearGraphs();
  deviationBarGraphs[player]->clearPlottables();

  deviationBarGraphs[player+2]->clearGraphs();
  deviationBarGraphs[player+2]->clearPlottables();

  // Setup Main Bar Graph

  QCPBars *barGraph = new QCPBars(deviationBarGraphs[player]->xAxis,
				  deviationBarGraphs[player]->yAxis);
  deviationBarGraphs[player]->addPlottable(barGraph);
  barGraph->setName("Expected Payoffs from Deviation");

  const vector< vector<double> > objectiveValues(guiData->getObjectiveValues());

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



  QCPBars *barGraphm = new QCPBars(deviationBarGraphs[player+2]->xAxis,
				  deviationBarGraphs[player+2]->yAxis);
  deviationBarGraphs[player+2]->addPlottable(barGraphm);
  barGraphm->setName("Multiplier from Obedience Constraint");

  const vector< vector<double> > multiplierValues(guiData->getMultiplierValues());

  QVector<double> yDatam;

  yDatam = QVector<double>::fromStdVector(multiplierValues[player]);

  int sizeYDatam = yDatam.size();
  QVector<double> xDatam(sizeYDatam,0);

  for(int i = 0; i < sizeYDatam; i++) {
    xDatam[i]=(double)i;
  }

  barGraphm->setData(xDatam,yDatam);
  barGraphm->setPen(QColor(255, 131, 0));
  barGraphm->setBrush(QColor(255, 131, 0, 50));

  // Highlight Recommended Action

  QCPBars *recAction = new QCPBars(deviationBarGraphs[player]->xAxis,
				   deviationBarGraphs[player]->yAxis);
  deviationBarGraphs[player]->addPlottable(recAction);

  int action = guiData->getCurrentSliderVal(Action,player);

  recAction->setData(QVector<double>(1,action),
		     QVector<double>(1,yData[action]));

  recAction->setPen(QPen(Qt::green));
  recAction->setBrush(QBrush(Qt::green));



  QCPBars *recActionm = new QCPBars(deviationBarGraphs[player+2]->xAxis,
				   deviationBarGraphs[player+2]->yAxis);
  deviationBarGraphs[player+2]->addPlottable(recActionm);

  int actionm = guiData->getCurrentSliderVal(Action,player);

  recActionm->setData(QVector<double>(1,actionm),
		     QVector<double>(1,yDatam[actionm]));

  recActionm->setPen(QPen(Qt::green));
  recActionm->setBrush(QBrush(Qt::green));

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
  }

  QCPBars *indiffAction = new QCPBars(deviationBarGraphs[player]->xAxis,
				      deviationBarGraphs[player]->yAxis);
  deviationBarGraphs[player]->addPlottable(indiffAction);

  indiffAction->setData(xCoordIndiff,yCoordIndiff);
  indiffAction->setPen(QColor(1, 92, 191));
  indiffAction->setBrush(QColor(1, 92, 191, 50));



  double recValuem = yDatam[actionm];

  QVector<double> xCoordIndiffm;
  QVector<double> yCoordIndiffm;

  for (int i = 0; i < sizeYDatam; i++) {
    if (abs((yDatam[i]-recValuem)/recValuem) < maxDifference) {
      xCoordIndiffm.push_back(i);
      yCoordIndiffm.push_back(yDatam[i]);
    }
  }

  QCPBars *indiffActionm = new QCPBars(deviationBarGraphs[player+2]->xAxis,
				      deviationBarGraphs[player+2]->yAxis);
  deviationBarGraphs[player+2]->addPlottable(indiffActionm);

  indiffActionm->setData(xCoordIndiffm,yCoordIndiffm);
  indiffActionm->setPen(QColor(1, 92, 191));
  indiffActionm->setBrush(QColor(1, 92, 191, 50));

  // Replot

  deviationBarGraphs[player]->rescaleAxes();
  deviationBarGraphs[player]->replot();

  deviationBarGraphs[player+2]->rescaleAxes();
  deviationBarGraphs[player+2]->replot();

} // Plot Deviation Objectives

/////////////////////////////////
// View Slots

void BCEPlotHandler::toggleLinearScale(bool checked) {
  // Prevents Crashing
  if (isSolnDataLoaded == false)
    return;

  if (checked)
    colorScale->setDataScaleType(QCPAxis::stLinear);
  else
    colorScale->setDataScaleType(QCPAxis::stLogarithmic);
  plotEqm();
} // Slot to set a linear color scale for the distribution's heat map

void BCEPlotHandler::toggleColorfulTheme(bool checked) {
  // Prevents Crashing
  if (isSolnDataLoaded == false)
    return;

  QCPColorGradient *newGradient = new QCPColorGradient();

  if (checked)
    colorMap->setGradient(newGradient->gpSpectrum);
  else
    colorMap->setGradient(newGradient->inverted());
  plotEqm();
} // Slot to change color theme of heat map for conditional marginal distribution

void BCEPlotHandler::setGUITitle() {

  stringstream dynamicTitle;

  string guiTitle = guiData->getGUITitle();

  dynamicTitle << "BCE Solution Viewer, Current File = "
	       << guiTitle;

  string newTitleStr = dynamicTitle.str();
  QString newTitle = QString::fromStdString(newTitleStr);

  this->setWindowTitle(newTitle);
}

void BCEPlotHandler::setSolution(const BCESolution &solution,
				 const bool isBoundaryMapped) {
  guiData->setSolutionData(solution,isBoundaryMapped);
}

void BCEPlotHandler::indicateDataLoaded() {
  isSolnDataLoaded=true;
}
