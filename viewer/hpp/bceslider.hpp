#ifndef BCESLIDER_HPP
#define BCESLIDER_HPP

#include <QtWidgets>
#include "bceenumeration.hpp"

//! QScrollBar with added signal and slots.
/*! This class is a QScrollBar containing additional
  information about whether the scroll bar is associated
  with the action, type, or state of a given player.

    \ingroup viewer
 */
class BCESlider : public QScrollBar
{
  Q_OBJECT;

private:

  //! Type of BCEScrollBar
  BCESliderType sliderType;
  //! Player associated with the BCEScrollBar
  int player;  

public:

  //! Constructor
  BCESlider(BCESliderType _type,int _player):
    sliderType(_type), player(_player)
  {
    connect(this,SIGNAL(valueChanged(int)),
	    this,SLOT(changeValue(int)));
  }

signals:
  //! Signals that the user has moved the BCESlider.
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
