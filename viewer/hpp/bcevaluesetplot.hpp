#ifndef BCEVALUESETPLOT_HPP
#define BCEVALUESETPLOT_HPP

#include <QtWidgets>
#include "qcustomplot.h"

class BCEValueSetPlot : public QCustomPlot {
  Q_OBJECT;

public:

  BCEValueSetPlot() {
  connect(this,SIGNAL(mouseMove(QMouseEvent*)),
	  this,SLOT(showPointToolTip(QMouseEvent*)));
  connect(this,SIGNAL(mousePress(QMouseEvent*)),
	  this,SLOT(getClickCoordinates(QMouseEvent*)));
  }

signals:

  void newEqmCoordinates(double x,double y);

public slots:

  void showPointToolTip(QMouseEvent *event) {

    double x = this->xAxis->pixelToCoord(event->pos().x());
    double y = this->yAxis->pixelToCoord(event->pos().y());

    setToolTip(QString("%1 , %2").arg(x).arg(y));
  }

  void getClickCoordinates (QMouseEvent *event) {

    double x = this->xAxis->pixelToCoord(event->pos().x());
    double y = this->yAxis->pixelToCoord(event->pos().y());

    emit(newEqmCoordinates(x,y));

  }


};

#endif
