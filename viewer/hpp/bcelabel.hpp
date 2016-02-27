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

#ifndef BCELABELHANDLER_HPP
#define BCELABELHANDLER_HPP

#include <QtWidgets>
#include <sstream>
#include "bceenumeration.hpp"
#include <iomanip>

//! Class handling all labeling in the solution tab.
/*! This class has a series of constructors to create labels
  for plots and controls. Also contains slots to change
  plot labels after the user alters parameters in the 
  solution tab.

  \ingroup viewer

 */
class BCELabel : public QLabel {
  Q_OBJECT;

private:

  //! Player associated with the label.
  int player;
  //! Type of label (i.e. "DeviationPlot," "SliderLabel," etc.).
  LabelType labelType;
  //! Type of Slider associated with a slider label.
  BCESliderType sliderType;

  std::stringstream dynamicString;

public:

  //! Constructs the plot titles for the deviation bar graphs.
  BCELabel(LabelType _labelType,int _player);

  //! Constructs slider labels for controls layout.
  BCELabel(LabelType _labelType,BCESliderType _sliderType,int _player);

  //! Constructs plot titles for heatmap (right panel) and set of BCE plot (top left).
  BCELabel(LabelType _labelType);

public slots:

  void changeText(int emittedPlayer,
		  int action,
		  int type,
		  double objectiveValue);

  void changeProbability(int emittedPlayer,double probability);

  void displayStateOrValues(bool isPrivateVals,
			    int emittedPlayer);

  void changeDisplayedCoords(double x,double y);

  void changeDisplayedState(int value0,
			    int value1,
			    int state,
			    bool isPrivateVals);

};

#endif
