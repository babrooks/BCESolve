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
  BCELabel(LabelType _labelType,int _player):
    player(_player),labelType(_labelType)
  {
    std::stringstream titleName;
    titleName << "Player " << player << "'s Deviation Objectives";
    std::string titleStr = titleName.str();
    QString title = QString::fromStdString(titleStr);

    setText(title);
    setStyleSheet("background-color: white");
  } 

  //! Constructs slider labels for controls layout.
  BCELabel(LabelType _labelType,BCESliderType _sliderType,int _player):
    labelType(_labelType),sliderType(_sliderType),player(_player)
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

  //! Constructs plot titles for heatmap (right panel) and set of BCE plot (top left).
  BCELabel(LabelType _labelType):
    labelType(_labelType)
  {
    switch(_labelType) {
    case ValueSetPlot: {
      std::string staticString = "Set of BCE, Currently Selected at: ";
      QString title = QString::fromStdString(staticString);
      setText(title);
      setStyleSheet("background-color: white");
    }
      break;
    case HeatMap: {
      std::string staticString = "Bayes Correlated Equilibrium";
      QString title = QString::fromStdString(staticString);
      setText(title);
      setStyleSheet("background-color: white");
    }
      break;
    }
  }

public slots:

  void changeText(int emittedPlayer,
		  int action,
		  int type,
		  double objectiveValue) {

    if (labelType == DeviationPlot) {
      if (player == emittedPlayer) {
	dynamicString.str(std::string());
	dynamicString << "Player " << emittedPlayer << "'s Deviation Objectives, "
		      << "Action = " << action << ", "
		      << "Type = " << type << ", "
		      << "Payoff = " << objectiveValue << ", ";
      }
    }
  }

  void changeProbability(int emittedPlayer,double probability) {

    if (labelType == DeviationPlot 
	&& player == emittedPlayer) {

      std::stringstream titleName;
      std::string titleNameNoPrStr = dynamicString.str();
      titleName << titleNameNoPrStr
		<< "Probability = " << probability;
      std::string titleStr = titleName.str();
      QString title = QString::fromStdString(titleStr);

      setText(title);
      setAlignment(Qt::AlignCenter);

    }
  }

  void displayStateOrValues(bool isPrivateVals,
			    int emittedPlayer) {
    if (labelType = SliderLabel) {
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
  }

  void changeDisplayedCoords(double x,double y) {

    if (labelType == ValueSetPlot) {

      std::string staticString = "Set of BCE, Currently Selected at: ";

      dynamicString.str(std::string());
      dynamicString << staticString 
		    << "x = " << std::setprecision(3) << x << ", "
		    << "y = " << std::setprecision(3) << y;
      std::string titleStr = dynamicString.str();
      QString title = QString::fromStdString(titleStr);

      setText(title);
      setAlignment(Qt::AlignCenter);

    }
  }

  void changeDisplayedState(int value0,
			    int value1,
			    int state,
			    bool isPrivateVals) {

    if (labelType == HeatMap) {

      std::string staticString = "Bayes Correlated Equilibrium";

      dynamicString.str(std::string());
      dynamicString << staticString;
    
      if (isPrivateVals)
	dynamicString << ", State = " << state;
      else {
	dynamicString << ", Player 0's Value = " << value0
		      << ", Player 1's Value = " << value1;
      }

      std::string titleStr = dynamicString.str();
      QString title = QString::fromStdString(titleStr);

      setText(title);
      setAlignment(Qt::AlignCenter);
    }
  }

};

#endif
