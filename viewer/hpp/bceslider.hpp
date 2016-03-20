// This file is part of the BCESolve library for games of incomplete
// information
// Copyright (C) 2016 Benjamin A. Brooks, Robert J. Minton
// 
// BCESolve free software: you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// BCESolve is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see
// <http://www.gnu.org/licenses/>.
// 
// Benjamin A. Brooks
// ben@benjaminbrooks.net
// Chicago, IL

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
    sliderType(_type),player(_player),QScrollBar()
  {
    connect(this,SIGNAL(valueChanged(int)),
	    this,SLOT(changeValue(int)));
  }

signals:
  //! Signals that the user has moved the BCESlider.
  void valueChanged(int newValue,BCESliderType type,int player);

public slots:
  //! Changes value of the slider
  void changeValue(int newValue)
  { emit(valueChanged(newValue,sliderType,player)); }
     
  //! Changes position of the slider
  void changeSliderPosition(int newValEmitted,
			    BCESliderType typeEmitted,
			    int playerEmitted) {
    if (sliderType == typeEmitted && player == playerEmitted)
      setSliderPosition(newValEmitted);
  } 
 
};

#endif
