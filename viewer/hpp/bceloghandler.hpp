// This file is part of the BCESolve library for games of incomplete
// information
// Copyright (C) 2022 Benjamin A. Brooks
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

#ifndef BCELOGHANDLER_HPP
#define BCELOGHANDLER_HPP

#include<QTextEdit>
#include<QVBoxLayout>
#include <QObject>

//! Log tab for the BCEViewer.
/*! This class handles the log tab in the viewer.
  It contains a QTextEdit for displaying log output.

  \ingroup viewer

 */
class BCELogHandler : public QObject {
  Q_OBJECT;

private:

  //! Layout for the log handler.
  QVBoxLayout *layout;

public:

  //! Text edit contained in the log handler.
  QTextEdit *logText;

  //! Constructor
  BCELogHandler() {

    logText = new QTextEdit();
    layout = new QVBoxLayout();

    // logText->setTextFormat(Qt::LogText);

    layout->addWidget(logText);
  }

  //! Returns a pointer to the log handler's layout
  QVBoxLayout* getLayout() const {
    return layout;
  }

};

#endif
