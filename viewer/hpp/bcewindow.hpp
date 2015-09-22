#ifndef PLOTTABHANDLER_H
#define PLOTTABHANDLER_H

#include <vector>
#include <QtWidgets>
#include "bcedatastate.hpp"
#include "qcustomplot.h"
#include "bceslider.hpp"
#include "bcelineedit.hpp"
#include "bcecheckbox.hpp"
#include "bcevaluesetplot.hpp"
#include "bceenumeration.hpp"
#include "bcedevplottitle.hpp"
#include "bcevaluesetplottitle.hpp"
#include "bceheatmaptitle.hpp"
#include "bcesliderlabel.hpp"

using namespace std;

//! Viewer Window Class
/*! Contains main viewer functions for displaying
  the GUI. Interacts with every viewer header
  file. Friend class of BCEDataState.

  \ingroup viewer
*/
class BCEWindow : public QMainWindow
{
  Q_OBJECT;

public:
  //! Constructor
  BCEWindow();

public slots:

  //! colorMap,conditionalMarginalPlot plotting function.
  void plotEqm();
  //! deviationBarGraphs plotting function.
  void plotDeviationObjectives(int player);
  //! payoffPlot plotting function.
  void plotBCEValueSet();

signals:

  //! Sends path to example to BCEDataState.
  void dataPathChanged(char* path);

private:

  QAction *linearScale;
  QAction *colorfulDistn;

  //! Screen Resolution Width.
  int resWidth;
  //! Screen Resolution Height.
  int resHeight;

  QVector<BCESlider*> sliderGroup;
  QVector<BCELineEdit*> lineEditGroup;
  QVector<BCECheckBox*> checkBoxGroup;
  QVector<QLabel*> sliderLabels;
  QVector<BCEDevPlotTitle*> devPlotTitles;

  //! Default path to saved .bce files.
  QString path;
  //! Default path to gui screenshots (*.png files).
  QString screenShotPath;

  //! Heatmap template.
  BCEValueSetPlot *conditionalMarginalPlot;
  //! Heatmap.
  QCPColorMap *colorMap;
  //! Scale for heatmap colors.
  QCPColorScale *colorScale;
  //! Color of heatmap colorScale.
  QCPColorGradient *mGradient;
  //! Set of BCE Plot.
  BCEValueSetPlot *setOfBCEPlot;
  //! Bar Graphs.
  QVector<QCustomPlot*> deviationBarGraphs;
  //! Data state.
  /*! Continuously manipulated after user 
    interaction.
  */
  BCEDataState guiData;

  QMenu *fileMenu;
  QAction *loadSolutionAction;

private slots:

  //! Interacts with BCEDataState to load new data.
  void loadSolution();
  //! Toggles colorScale between linear and log scales.
  void toggleLinearScale(bool checked);
  //! Toggles color gradient between blue and gpSpectrum themes.
  void toggleColorfulTheme(bool checked);
  //! Takes a screenshot and opens a save file menu.
  /* The .png file name ending must be entered manually
     upon saving for proper save action functionality.
  */
  void screenShot();

};

#endif
