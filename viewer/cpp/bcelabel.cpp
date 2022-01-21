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

#include "bcelabel.hpp"

BCELabel::BCELabel(LabelType _labelType,
		   BCESliderType _sliderType,
		   int _player):
  labelType(_labelType),sliderType(_sliderType),player(_player)
{
  std::stringstream dynamicString;
  dynamicString.precision(2);

  switch(_sliderType) {
  case Action: {
    dynamicString << "Player " << _player << "'s Action";
    std::string labelStr = dynamicString.str();
    QString label = QString::fromStdString(labelStr);

    setText(label);
  }
    break;
  case Type: {
    dynamicString << "Player " << _player << "'s Type";
    std::string labelStr = dynamicString.str();
    QString label = QString::fromStdString(labelStr);

    setText(label);
  }
    break;
  case State: {
    dynamicString << "Player " << _player << "'s Value";
    std::string labelStr = dynamicString.str();
    QString label = QString::fromStdString(labelStr);

    setText(label);
  }
    break;
  }
}

void BCELabel::displayStateOrValues(bool isPrivateVals,
				    int emittedPlayer) {
  if (labelType = SliderLabel) {
    if (isPrivateVals) {
      if (sliderType == State) {

	setText("Common State");

      }
    }
    else {
      if (sliderType == State && emittedPlayer == player) {

	std::stringstream dynamicString;

	dynamicString << "Player " << player << "'s Value";
	std::string labelStr = dynamicString.str();
	QString label = QString::fromStdString(labelStr);

	setText(label);

      }
    }
  }
}


