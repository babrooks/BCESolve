// This file is part of the BCESolve library for games of incomplete
// information
// Copyright (C) 2016 Benjamin A. Brooks, Robert J. Minton
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

#ifndef BCEPLOTHANDLER_HPP
#define BCEPLOTHANDLER_HPP

#include <vector>
#include <QtWidgets>
#include "bcedatastate.hpp"
#include "qcustomplot.h"
#include "bcevaluesetplot.hpp"
#include "bceenumeration.hpp"
#include "bcelabel.hpp"
#include <QPointer>

using namespace std;

//! This class handles all the plotting in the solution tab.
/*! Handles plotting and layout of the solution tab. Gets data from
  BCEDataState for plotting. Connects data change signals
  in BCEDataState with slots to change plot titles dynamically.

  \ingroup viewer
*/
class BCEPlotHandler : public QMainWindow
{
  Q_OBJECT;

public:
  //! Constructor
  BCEPlotHandler(QWidget *parent);
  //! Destructor
  ~BCEPlotHandler() {
    delete guiData;
  }

signals:
  //! Sends data path to BCEDataState
  void sendingDataPath(QString location);

public slots:

  //! Plots Bayes Correlated Equilibria.
  /*! Removes currently plotted data, gets new data from 
    BCEDataState, and plots the new data in a heat map.
    Slot is triggered when the equilibrium matrix is 
    changed in BCEDataState.
  */
  void plotEqm();
  //! Plots Bar Graphs for Player Deviation Objectives.
  /*! Removes currently plotted data, gets new data from 
    BCEDataState, and plots the new data in a bar graph.
    Contains additional plottables for highlighting
    players' indifferent deviations and highlighting the
    currently selected action. Slot is triggered when a 
    player's objectiveValues vector matrix is changed in
    BCEDataState.
  */
  void plotDeviationObjectives(int player);
  //! Plots set of equilibria player payoffs.
  /*! Slot triggered when the vector of BCEquilibria is
    updated after loading or when a new equilibrium index
    is selected. Contains additional plottables to highlight
    the currently selected equilibrium.

    There is only one equilibrium unless mapBoundary() has 
    been called in the loaded example's .cpp file.
  */
  void plotBCEValueSet();
  //! Sets a new title for the GUI using fileName information.
  void setGUITitle();
  //! Toggles colorScale between linear and log scales.
  void toggleLinearScale(bool checked);
  //! Toggles color gradient between blue and gpSpectrum themes.
  void toggleColorfulTheme(bool checked);
  //! Takes a screenshot and opens a save file menu.
  /* The .png file name ending must be entered manually
     upon saving for proper save action functionality.
  */
  void screenShot();
  //! Receives location of new data from BCEWindow
  void loadData(QString location) {
    emit(sendingDataPath(location));
  }

private:

  //! Layout for the Plot Handler
  QHBoxLayout *mainTab;
  //! Data state. 
  /*! Stores all data currently held or 
    displayed in the GUI. "Held" data 
    refers to parameters the user supplies 
    through slider manipulation and loading
    actions. "Displayed" data refers to 
    the data BCEDataState has manipulated
    for plotting.

    Continuously manipulated upon user 
    interaction. This class has access to
    guiData through BCEDataState's "get"
    functions.
  */
  BCEDataState *guiData;
  //! Default path to gui screenshots (*.png files).
  /*! Currently set as ../examples/screenshots/
   */
  QString screenShotPath;
  //! Heatmap template.
  /*! Stores the colorMap plottable, which
    contains data in BCEDataState's 
    equilibriumMatrix. Also holds the colorScale,
    which provides a numerical scale for
    the heatmap's colors.
  */
  BCEValueSetPlot *conditionalMarginalPlot;
  //! Heatmap.
  /*! A QCustomPlot class. Contains built-in
    functionality for the heatmap displayed
    in the GUI.
  */
  QCPColorMap *colorMap;
  //! Scale for heatmap colors.
  QCPColorScale *colorScale;
  //! Linear/Log incrementation toggle for colorScale.
  QAction *linearScale;
  //! Colorful/Blue theme toggle for colorScale.
  QAction *colorfulDistn;
  //! Color of heatmap colorScale.
  QCPColorGradient *mGradient;
  //! Set of BCE Plot.
  BCEValueSetPlot *setOfBCEPlot;
  //! Bar Graphs.
  QVector<BCEValueSetPlot*> deviationBarGraphs;
  //! Plot titles for the deviation bar plots.
  QVector<BCELabel*> devPlotTitles;
  //! Sets up the layout of the plot handler.
  void setupLayout();
  //! Indicates if solution data is loaded in the soln tab.
  bool isSolnDataLoaded;

public:
  //! Returns Layout of the Plot Handler
  QHBoxLayout * getLayout() const {
    return mainTab;
  }
  //! Sets the current BCESolution in BCEDataState
  void setSolution(const BCESolution &solution);

  //! Returns the current BCESolution in BCEDataState
  const BCESolution& getSolutionData() const {
    return guiData->getSolution();
  }

private slots:

  //! Sets isSolnDataLoaded to true.
  /*! Allows usage of color toggles for plots and
    changing the colormap scale from linear to log.
    Prevents the program from crashing if user uses
    these toggles before solution data is loaded.
   */
  void indicateDataLoaded();

};

#endif
