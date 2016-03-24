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


