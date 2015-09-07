#ifndef BCESLIDER_H
#define BCESLIDER_H

#include <QtWidgets>
#include "bcedatastate.hpp"
#include "bceenumeration.hpp"

class BCESlider : public QSlider
{
  Q_OBJECT;

public:

  BCESliderType sliderType;
  int player;  

  BCESlider(BCESliderType _type,int _player):
    sliderType(_type), player(_player)
  {
    connect(this,SIGNAL(valueChanged(int)),
	    this,SLOT(changeValue(int)));
  }

signals:
  void valueChanged(int newValue,BCESliderType type,int player);

public slots:
  void changeValue(int newValue)
  { emit(valueChanged(newValue,sliderType,player)); }
     
  void changeSliderPosition(int newValEmitted,
			    BCESliderType typeEmitted,
			    int playerEmitted) {
    if (sliderType == typeEmitted && player == playerEmitted)
      setSliderPosition(newValEmitted);
  } 
 
};

#endif
