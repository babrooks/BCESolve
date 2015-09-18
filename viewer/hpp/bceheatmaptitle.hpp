#ifndef BCEHEATMAPTITLE_HPP
#define BCEHEATMAPTITLE_HPP

#include <QtWidgets>
#include <sstream>

//! QLabel for heat map's title.
class BCEHeatMapTitle : public QLabel {
  Q_OBJECT;

private:

  std::string staticString;
  std::stringstream dynamicString;

public:

  BCEHeatMapTitle():
    staticString("Bayes Correlated Equilibrium")
  {
    QString title = QString::fromStdString(staticString);
    setText(title);
    setStyleSheet("background-color: white");
  }

public slots:

  void changeDisplayedState(int value0,
			    int value1,
			    int state,
			    bool isPrivateVals) {

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


};

#endif
