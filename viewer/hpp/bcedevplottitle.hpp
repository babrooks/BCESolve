#ifndef BCEDEVPLOTTITLE_HPP
#define BCEDEVPLOTTITLE_HPP

#include <QtWidgets>
#include <sstream>

//! QLabel used for deviation plot titles.
/*!

    \ingroup viewer
 */
class BCEDevPlotTitle : public QLabel
{
  Q_OBJECT;

private:

  int player;
  std::stringstream titleNameNoPr;

public:

  BCEDevPlotTitle(int _player):
    player(_player)
  {
    std::stringstream titleName;
    titleName << "Player " << player << "'s Deviation Objectives";
    std::string titleStr = titleName.str();
    QString title = QString::fromStdString(titleStr);

    setText(title);
    setStyleSheet("background-color: white");
  } 

public slots:

  void changeText(int emittedPlayer,
		  int action,
		  int type,
		  double objectiveValue) {

    if (player == emittedPlayer) {
      titleNameNoPr.str(std::string());
      titleNameNoPr << "Player " << emittedPlayer << "'s Deviation Objectives, "
		    << "Action = " << action << ", "
		    << "Type = " << type << ", "
		    << "Payoff = " << objectiveValue << ", ";
    }
  }

  void changeProbability(double probability) {

    std::stringstream titleName;
    std::string titleNameNoPrStr = titleNameNoPr.str();
    titleName << titleNameNoPrStr
	      << "Probability = " << probability;
    std::string titleStr = titleName.str();
    QString title = QString::fromStdString(titleStr);

    setText(title);
    setAlignment(Qt::AlignCenter);

  }


};

#endif
