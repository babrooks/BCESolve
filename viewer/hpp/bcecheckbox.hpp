#ifndef BCECHECKBOX_HPP
#define BCECHECKBOX_HPP

#include <QtWidgets>
#include "bcedatastate.hpp"
#include "bceenumeration.hpp"

//! QCheckBox with added signal and slot.
/*!

    \ingroup viewer
 */
class BCECheckBox : public QCheckBox
{
  Q_OBJECT;

private:

  BCESliderType boxType;
  int player;  

public:

  BCECheckBox(BCESliderType _type,int _player):
    boxType(_type), player(_player)
  {
    connect(this,SIGNAL(clicked(bool)),
	    this,SLOT(changeValue(bool)));
  }

signals:
  void boolChanged(bool checked,BCESliderType type,int player);

public slots:
  void changeValue(bool checked)
  { emit(boolChanged(checked,boxType,player)); }


};

#endif
