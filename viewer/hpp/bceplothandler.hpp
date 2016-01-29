#ifndef BCEPLOTHANDLER_HPP
#define BCEPLOTHANDLER_HPP

#include <vector>
#include <QtWidgets>
#include "bcedatastate.hpp"
#include "qcustomplot.h"
#include "bceslider.hpp"
#include "bcelineedit.hpp"
#include "bcecheckbox.hpp"
#include "bcevaluesetplot.hpp"
#include "bceenumeration.hpp"
#include "bcelabelhandler.hpp"
#include <QPointer>

using namespace std;

//! This class handles all the plotting in the solution tab.
/*! Handles plotting and layout of the solution tab. Connects
  user interaction in the solution tab with BCEDataState to
  modify data elements for plotting. Gets data from
  BCEDataState for plotting. Connects data change signals
  in BCEDataState with slots to change plot titles dynamically.

  \ingroup viewer
*/
class BCEPlotHandler : public QMainWindow
{
  Q_OBJECT;

public:
  //! Constructor
  BCEPlotHandler(int resW,int resH);
  //! Destructor
  ~BCEPlotHandler() {
    delete guiData;
  }

signals:
  //! Sends data path to BCEDataState
  void sendingDataPath(QString location);

public slots:

  //! colorMap,conditionalMarginalPlot plotting function.
  /*! Removes currently plotted data, gets new data from 
    BCEDataState, and plots the new data in a heat map.
    Slot is triggered when the equilibrium matrix is 
    changed in BCEDataState.
  */
  void plotEqm();
  //! deviationBarGraphs plotting function.
  /*! Removes currently plotted data, gets new data from 
    BCEDataState, and plots the new data in a bar graph.
    Contains additional plottables for highlighting
    players' indifferent deviations and highlighting the
    currently selected action. Slot is triggered when a 
    player's objectiveValues vector matrix is changed in
    BCEDataState.
  */
  void plotDeviationObjectives(int player);
  //! payoffPlot plotting function.
  /*! Slot triggered when the vector of BCEquilibria is
    updated after loading or when a new equilibrium index
    is selected. Contains additional plottable to highlight
    the currently selected equilibrium.
  */
  void plotBCEValueSet();
  //! Sets a new title for the GUI using fileName information.
  /*! Gets guiTitle from BCEDataState and sets the main window
    title << "BCE Solution Viewer, Current File = "
    << getGUITitle_output
  */
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
  //! Screen Resolution Width.
  int resWidth;
  //! Screen Resolution Height.
  int resHeight;
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
  //! Not currently implemented.
  /*! Checkboxes are displayed in the GUI but
    are not fully implemented. Using them in the
    GUI will cause the program to crash.
  */
  QVector<BCECheckBox*> checkBoxGroup;
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
  QVector<QCustomPlot*> deviationBarGraphs;
  //! Plot titles for the deviation bar plots.
  QVector<BCELabel*> devPlotTitles;
  //! Sets up the layout of the plot handler.
  void setupLayout();

public:
  //! Returns Layout of the Plot Handler
  QHBoxLayout * getLayout() const {
    return mainTab;
  }
  //! Sets the current BCESolution in BCEDataState
  void setSolution(BCESolution &solution);

  //! Returns the current BCESolution in BCEDataState
  const BCESolution& getSolutionData() const {
    return guiData->getSolution();
  }

};

#endif
