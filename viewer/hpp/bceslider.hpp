#ifndef BCESLIDER_HPP
#define BCESLIDER_HPP

#include <QtWidgets>
#include "bceenumeration.hpp"

//! QScrollBar with added signal and slots.
/*!

    \ingroup viewer
 */
class BCESlider : public QScrollBar
{
  Q_OBJECT;

private:

  BCESliderType sliderType;
  int player;  

public:

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
