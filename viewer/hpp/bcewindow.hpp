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

using namespace std;

class BCEWindow : public QMainWindow
{
  Q_OBJECT;

public:
  BCEWindow();

public slots:

  void changeSliderValue(int newVal,
			 BCESliderType type,
			 int player);
  void changeMarginalBool(bool newBool,BCESliderType type,int player);
  void setNewEqm(double x,double y);

private:

  QVector<BCESlider*> sliderGroup;
  QVector<BCELineEdit*> lineEditGroup;
  QVector<BCECheckBox*> checkBoxGroup;

  QString path;

  QCustomPlot *conditionalMarginalPlot;
  QCPColorMap *colorMap;
  QCPColorScale *colorScale;
  QCPColorGradient *mGradient;

  BCEValueSetPlot *payoffPlot;

  QVector<QCustomPlot*> deviationBarGraphs;

  BCEDataState gui;
  void plotEqm(vector<vector<double>> bceEqm);
  void plotDeviationObjectives(int index, vector<vector<double>> deviationObjectives);
  void plotBCEValueSet();
  void plotSelectGraphics(BCESliderType type,int player);
  void plotAllGraphics();

  QMenu *fileMenu;
  QAction *loadSolutionAction;

private slots:

  void loadSolution();
  void setLinearScale();
  void setLogScale();
  void setColorfulTheme();
  void setBlueTheme();

};

#endif