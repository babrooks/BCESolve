#ifndef BCELINEEDIT_HPP
#define BCELINEEDIT_HPP

#include <QtWidgets>
#include "bceenumeration.hpp"

//! Read-only QLineEdits displaying slider values.
/*! This class is a QLineEdit containing additional
  information about whether the line edit is associated
  with the action, type, or state of a given player.

    \ingroup viewer
 */
class BCELineEdit : public QLineEdit
{
  Q_OBJECT;

public:

  //! Type of BCELineEdit (Action, Type, State)
  BCESliderType sliderType;
  //! Player associated with the BCELineEdit
  int player;

  //! Constructor
  BCELineEdit(BCESliderType _type, int _player):
    sliderType(_type), player(_player)
  {}

public slots:

  //! Ensures slider is set to the changed value (REIMPLEMENT).
  void changeDisplayValue(int newValEmitted,
			  BCESliderType typeEmitted,
			  int playerEmitted) {
    if (sliderType == typeEmitted && player == playerEmitted)
      setText(QString::number(newValEmitted));
  }

};

#endif
