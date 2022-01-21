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

#ifndef BCELABELHANDLER_HPP
#define BCELABELHANDLER_HPP

#include <QtWidgets>
#include <sstream>
#include "bceenumeration.hpp"
#include <iomanip>

//! Class handling labeling of controls in the solution tab.
/*! This class constructs labels for the grid of controls
  in the solution tab. The class processes whether the game
  has a product structure or not and labels the state controls
  accordingly.

  \ingroup viewer

 */
class BCELabel : public QLabel {
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

  //! Constructs slider labels for controls layout.
  BCELabel(LabelType _labelType,BCESliderType _sliderType,int _player);

public slots:

  //! Sets the state slider label to reflect if game has common or private states.
  void displayStateOrValues(bool isPrivateVals,
			    int emittedPlayer);

};

#endif
