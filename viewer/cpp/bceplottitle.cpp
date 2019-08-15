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

#include "bceplottitle.hpp"

BCEPlotTitle::BCEPlotTitle(LabelType _labelType,
			   int _player,
			   QCustomPlot *parentPlot):
  player(_player),labelType(_labelType),QCPPlotTitle(parentPlot)
{
  std::stringstream titleName;
  if (player > 1)
  {
	  titleName << "Player " << player - 2 << "'s Objective Multipliers";
  }
  else
  {
	 titleName << "Player " << player << "'s Deviation Objectives";
  }
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
			  double objectiveValue,
		  	  double multiplierValue) {

  if (labelType == DeviationPlot) {
    if ((player-emittedPlayer) % 2 == 0) {
	  if (player > 1)
	  {
	    dynamicString.str("");
        dynamicString << "Player " << emittedPlayer << "'s Objective Multipliers, "
		    << "A" << action << ", "
		    << "T" << type << ", "
		    << "Multiplier = " << multiplierValue << ", ";
	  }
	  else
	  {
		dynamicString.str("");
        dynamicString << "Player " << emittedPlayer << "'s Deviation Objectives, "
  		    << "A" << action << ", "
  		    << "T" << type << ", "
  		    << "Payoff = " << objectiveValue << ", ";
	  }
    }
  }
}

void BCEPlotTitle::changeProbability(int emittedPlayer,double probability) {

  if (labelType == DeviationPlot
      && (player-emittedPlayer) % 2 == 0) {

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
		  << "x = " << std::setprecision(5) << x << ", " //2
		  << "y = " << std::setprecision(5) << y; //2
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
