// This file is part of the BCESolve library for games of incomplete
// information
// Copyright (C) 2016 Benjamin A. Brooks and Robert J. Minton
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
    sliderType(_type),player(_player),QLineEdit()
  {}

public slots:

  //! Ensures slider is set to the changed value.
  /*! For common state games, ensures both state
    sliders move upon interaction with just one.
   */
  void changeDisplayValue(int newValEmitted,
			  BCESliderType typeEmitted,
			  int playerEmitted) {
    if (sliderType == typeEmitted && player == playerEmitted)
      setText(QString::number(newValEmitted));
  }

};

#endif
