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

#ifndef BCEPUSHBUTTON_HPP
#define BCEPUSHBUTTON_HPP

#include <QtWidgets>

//! QPushButton that emits the player when clicked.
/*! This class is a QPushButton that emits the index
  of the player associated with it when it is clicked.

  \ingroup viewer
*/

class BCEPushButton : public QPushButton {
  Q_OBJECT;

private:
  //! Associated player
  int player;

public:

  //! Constructor
  BCEPushButton(int _player, QString _label, QWidget *parent):
    QPushButton(_label),player(_player)
  {
    connect(this,SIGNAL(clicked()),
	    this,SLOT(bceClick()));
    setParent(parent);
  }

signals:
  //! Signals the player associated with a BCEPushButton
  void clickedForPlayer(int player);

public slots:

  //! Emits the clickedForPlayer(_) signal.
  void bceClick() {
    emit(clickedForPlayer(player));
  }


};

#endif
