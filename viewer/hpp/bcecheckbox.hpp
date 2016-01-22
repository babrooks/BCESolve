#ifndef BCECHECKBOX_HPP
#define BCECHECKBOX_HPP

#include <QtWidgets>
#include "bceenumeration.hpp"

//! QCheckBox with identifiers "Action," "Type," and "State."
/*! Created for ease of building the plot tab layout. This class
  allows clicking signals to be connected to the proper slot
  by running switch cases against the checkbox's type
  (either "Action," "Type," or "State").

  When clicked, this checkbox emits the signal boolChanged,
  which contains information about whether the checkbox is
  checked, whether the checkbox fulfills functions for action,
  type, or state, and which player the checkbox is associated with.

    \ingroup viewer
 */
class BCECheckBox : public QCheckBox
{
  Q_OBJECT;

private:

  //! Type associated with the checkbox (action, type, state)
  BCESliderType boxType;
  //! Player associated with the checkbox (0,1)
  int player;  

public:

  BCECheckBox(BCESliderType _type,int _player):
    boxType(_type), player(_player)
  {
    connect(this,SIGNAL(clicked(bool)),
	    this,SLOT(changeValue(bool)));
  }

signals:
  //! Signal emitted after a BCECheckBox is clicked.
  void boolChanged(bool checked,BCESliderType type,int player);

public slots:
  void changeValue(bool checked)
  { emit(boolChanged(checked,boxType,player)); }


};

#endif
