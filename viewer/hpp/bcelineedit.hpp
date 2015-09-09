#ifndef BCELINEEDIT_HPP
#define BCELINEEDIT_HPP

#include <QtWidgets>
#include "bcedatastate.hpp"
#include "bceenumeration.hpp"

class BCELineEdit : public QLineEdit
{
  Q_OBJECT;

public:

  BCESliderType sliderType;
  int player;

  BCELineEdit(BCESliderType _type, int _player):
    sliderType(_type), player(_player)
  {}

public slots:

  void changeDisplayValue(int newValEmitted,
			  BCESliderType typeEmitted,
			  int playerEmitted) {
    if (sliderType == typeEmitted && player == playerEmitted)
      setText(QString::number(newValEmitted));
  }

};

#endif