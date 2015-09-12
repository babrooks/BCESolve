#ifndef BCEVALUESETPLOTTITLE_HPP
#define BCEVALUESETPLOTTITLE_HPP

#include <QtWidgets>
#include <sstream>
#include <iomanip>

class BCEValueSetPlotTitle : public QLabel {
  Q_OBJECT;

private:

  std::string staticString;
  std::stringstream dynamicString;

public:

  BCEValueSetPlotTitle():
    staticString("Set of BCE, Currently Selected at: ")
  {
    QString title = QString::fromStdString(staticString);
    setText(title);
  }

public slots:

  void changeDisplayedCoords(double x,double y) {

    dynamicString.str(std::string());
    dynamicString << staticString 
		  << "x = " << std::setprecision(3) << x << ", "
		  << "y = " << std::setprecision(3) << y;
    std::string titleStr = dynamicString.str();
    QString title = QString::fromStdString(titleStr);

    setText(title);
    setAlignment(Qt::AlignCenter);

  }

};

#endif
