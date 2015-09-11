#ifndef BCEDEVPLOTTITLE_HPP
#define BCEDEVPLOTTITLE_HPP

#include <QtWidgets>
#include "bceenumeration.hpp"
#include <sstream>

class BCEDevPlotTitle : public QLabel
{
  Q_OBJECT;

private:

  int player;

public:

  BCEDevPlotTitle(int _player):
    player(_player)
  {
    std::stringstream titleName;
    titleName << "Player " << player << "'s Deviation Objectives";
    std::string titleStr = titleName.str();
    QString title = QString::fromStdString(titleStr);

    setText(title);
  } 

public slots:

  void changeText(int emittedPlayer,
		  int action,
		  int type,
		  double objectiveValue,
		  double probability) {

    std::stringstream titleName;
    titleName << "Player " << emittedPlayer << "'s Deviation Objectives, "
	      << "Action = " << action << ", "
	      << "Type = " << type << ", "
	      << "Payoff = " << objectiveValue << ", "
	      << "Probability = " << probability;
    std::string titleStr = titleName.str();
    QString title = QString::fromStdString(titleStr);

    if (player == emittedPlayer)
      setText(title);

  }


};

#endif
