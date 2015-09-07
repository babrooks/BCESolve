#ifndef BCECHECKBOX_HPP
#define BCECHECKBOX_HPP

#include <QtWidgets>
#include "bcedatastate.hpp"
#include "bceenumeration.hpp"

class BCECheckBox : public QCheckBox
{
  Q_OBJECT;

public:

  BCESliderType boxType;
  int player;  

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
