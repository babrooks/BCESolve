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

#ifndef BCEPLOTTITLE_HPP
#define BCEPLOTTITLE_HPP

#include <QtWidgets>
#include <sstream>
#include "bceenumeration.hpp"
#include <iomanip>
#include "qcustomplot.h"

//! Class handling labeling of plots in the solution tab.
/*! This class has a series of constructors to create labels
  for plots. Also contains slots to change
  plot labels after the user alters parameters in the 
  solution tab.

  \ingroup viewer

 */
class BCEPlotTitle : public QCPTextElement {
  Q_OBJECT;

private:

  //! Player associated with the label.
  int player;
  //! Type of label (i.e. "DeviationPlot," "SliderLabel," etc.).
  LabelType labelType;
  //! Type of Slider associated with a slider label (i.e. "Type," "Action," "State").
  BCESliderType sliderType;
  //! String manipulated when data is changed.
  std::stringstream dynamicString;

public:

  //! Constructs plot titles for the deviation bar graphs.
  BCEPlotTitle(LabelType _labelType,int _player,QCustomPlot *parentPlot);

  //! Constructs plot titles for heatmap and plot of equilibria player payoffs (top left plot).
  BCEPlotTitle(LabelType _labelType,QCustomPlot *parentPlot);

public slots:

  //! Changes non-probability elements of the deviation plot titles
  void changeText(int emittedPlayer,
		  int action,
		  int type,
		  double objectiveValue);

  //! Changes probability element of the deviation plot titles
  void changeProbability(int emittedPlayer,double probability);

  //! Sets current equilibrium coordinates for the current eqm index.
  void changeDisplayedCoords(double x,double y);

  //! Sets current state displayed in the heatmap plot title.
  void changeDisplayedState(int value0,
			    int value1,
			    int state,
			    bool isPrivateVals);

};

#endif
