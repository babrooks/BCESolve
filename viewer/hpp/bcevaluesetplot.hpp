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
