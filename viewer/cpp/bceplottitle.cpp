#include "bceplottitle.hpp"

BCEPlotTitle::BCEPlotTitle(LabelType _labelType,
			   int _player,
			   QCustomPlot *parentPlot):
  player(_player),labelType(_labelType),QCPPlotTitle(parentPlot)
{
  std::stringstream titleName;
  titleName << "Player " << player << "'s Deviation Objectives";
  std::string titleStr = titleName.str();
  QString title = QString::fromStdString(titleStr);

  dynamicString.precision(2);
    
  setText(title);
}

BCEPlotTitle::BCEPlotTitle(LabelType _labelType,
			   QCustomPlot *parentPlot):
  labelType(_labelType),QCPPlotTitle(parentPlot)
{
  switch(_labelType) {
  case ValueSetPlot: {
    std::string staticString = "Set of BCE: ";
    QString title = QString::fromStdString(staticString);
    setText(title);
  }
    break;
  case HeatMap: {
    std::string staticString = "Bayes Correlated Equilibrium";
    QString title = QString::fromStdString(staticString);
    setText(title);
  }
    break;
  }
}

void BCEPlotTitle::changeText(int emittedPlayer,
			  int action,
			  int type,
			  double objectiveValue) {

  if (labelType == DeviationPlot) {
    if (player == emittedPlayer) {
      dynamicString.str("");
      dynamicString << "Player " << emittedPlayer << "'s Deviation Objectives, "
		    << "A" << action << ", "
		    << "T" << type << ", "
		    << "Payoff = " << objectiveValue << ", ";
    }
  }
}

void BCEPlotTitle::changeProbability(int emittedPlayer,double probability) {

  if (labelType == DeviationPlot 
      && player == emittedPlayer) {

    std::stringstream titleName;
    std::string titleNameNoPrStr = dynamicString.str();
    titleName << titleNameNoPrStr
	      << "Prob = " 
	      << std::setprecision(2) << probability;
    std::string titleStr = titleName.str();
    QString title = QString::fromStdString(titleStr);

    setText(title);

  }
}

void BCEPlotTitle::changeDisplayedCoords(double x,double y) {

  if (labelType == ValueSetPlot) {

    std::string staticString = "Set of BCE: ";

    dynamicString.str(std::string());
    dynamicString << staticString 
		  << "x = " << std::setprecision(2) << x << ", "
		  << "y = " << std::setprecision(2) << y;
    std::string titleStr = dynamicString.str();
    QString title = QString::fromStdString(titleStr);

    setText(title);

  }
}

void BCEPlotTitle::changeDisplayedState(int value0,
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
  }
}
