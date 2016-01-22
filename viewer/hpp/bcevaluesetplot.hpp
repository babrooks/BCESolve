#ifndef BCEVALUESETPLOT_HPP
#define BCEVALUESETPLOT_HPP

#include <QtWidgets>
#include "qcustomplot.h"

//! QCustomPlot with Tool-tip and click functionality.
/*! Plot template for the conditional-marginal plot and
  the plot of equilibria. The tool-tip provides the ability
  to hover the mouse over a point to see its coordinates. The
  click retrieves the coordinates of the click.

    \ingroup viewer
 */
class BCEValueSetPlot : public QCustomPlot {
  Q_OBJECT;

public:

  //! Constructor
  BCEValueSetPlot() {
  connect(this,SIGNAL(mouseMove(QMouseEvent*)),
	  this,SLOT(showPointToolTip(QMouseEvent*)));
  connect(this,SIGNAL(mousePress(QMouseEvent*)),
	  this,SLOT(getClickCoordinates(QMouseEvent*)));
  }

signals:

  //! Signals new coordinates after the user clicks on the plot.
  void newEqmCoordinates(double x,double y);

public slots:

  //! Shows coordinates for point the mouse is hovering over.
  void showPointToolTip(QMouseEvent *event) {

    double x = this->xAxis->pixelToCoord(event->pos().x());
    double y = this->yAxis->pixelToCoord(event->pos().y());

    setToolTip(QString("%1 , %2").arg(x).arg(y));
  }

  //! Gets coordinates of the point where a click is made.
  void getClickCoordinates (QMouseEvent *event) {

    double x = this->xAxis->pixelToCoord(event->pos().x());
    double y = this->yAxis->pixelToCoord(event->pos().y());

    emit(newEqmCoordinates(x,y));

  }


};

#endif
