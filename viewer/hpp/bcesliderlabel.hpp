#ifndef BCESLIDERLABEL_HPP
#define BCESLIDERLABEL_HPP

#include <QtWidgets>
#include <sstream>
#include "bceenumeration.hpp"

//! QLabel with added slot. Used for slider labeling.
class BCESliderLabel : public QLabel {
  Q_OBJECT;

private:

  BCESliderType sliderType;
  int player;

public:

  BCESliderLabel(BCESliderType _sliderType,int _player):
    sliderType(_sliderType),player(_player)
  {
    std::stringstream dynamicString;

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

public slots:

  void displayStateOrValues(bool isPrivateVals,
			    int emittedPlayer) {
    if (isPrivateVals) {
      if (sliderType == State) {

	setText("Unified State");

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

};

#endif
