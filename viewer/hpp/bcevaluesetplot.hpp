// This file is part of the BCESolve library for games of incomplete
// information
// Copyright (C) 2022 Benjamin A. Brooks
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

private:

  //! Pointer to QAction for saving PNG files
  QAction *savePNGAction;
  //! Pointer to QAction for saving PDF files
  QAction *savePDFAction;
  //! Path to screenshots
  QString path;

public:

  //! Constructor
  BCEValueSetPlot(): QCustomPlot() {

    setContextMenuPolicy(Qt::CustomContextMenu);

    path = QString("../");

    savePDFAction = new QAction(tr("Save &PDF"), this);
    savePNGAction = new QAction(tr("Save P&NG"), this);

    connect(this,SIGNAL(mouseMove(QMouseEvent*)),
	    this,SLOT(showPointToolTip(QMouseEvent*)));
    connect(this,SIGNAL(mousePress(QMouseEvent*)),
	    this,SLOT(getClickCoordinates(QMouseEvent*)));
    connect(savePDFAction,SIGNAL(triggered()),
	    this,SLOT(savePDF()));
    connect(savePNGAction,SIGNAL(triggered()),
	    this,SLOT(savePNG()));
    connect(this,SIGNAL(customContextMenuRequested(const QPoint &)),
	    this,SLOT(ShowContextMenu(const QPoint &)));
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

private slots:

  //! Slot for showing context menu. 
  /*! Creates a context menu and shows the actions for saving PDF/PNG
      files. */
  void ShowContextMenu(const QPoint & pos)
  {

    QPoint globalPos = this->mapToGlobal(pos);
    
    QMenu contextMenu;
    contextMenu.addAction(savePDFAction);
    contextMenu.addAction(savePNGAction);
    
    contextMenu.exec(globalPos);
  }

  //! Saves graph as a PDF.
  void savePDF()
  {
    QString newPath = QFileDialog::getSaveFileName(this, tr("Save PDF"),
						   path, tr("PDF files (*.pdf)"));
  
    if (newPath.isEmpty())
      return;

    newPath = newPath + ".pdf";
    QFileInfo fi(newPath);
    path = fi.canonicalPath();

    savePdf(newPath);
  }

  //! Saves graph as a PNG.
  void savePNG()
  {
    QString newPath = QFileDialog::getSaveFileName(this, tr("Save PNG"),
						   path, tr("PNG files (*.png)"));
  
    if (newPath.isEmpty())
      return;

    newPath = newPath+".png";
    QFileInfo fi(newPath);
    path = fi.canonicalPath();

    savePng(newPath);
  }

};

#endif
