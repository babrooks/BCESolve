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

#ifndef BCECONDITIONALTABLEMODEL_HPP
#define BCECONDITIONALTABLEMODEL_HPP

#include <QtWidgets>
#include <QAbstractTableModel>
#include <QTableView>
#include "bcegame.hpp"
#include "bcetablemodel.hpp"

//! Table models for BCEViewer
/*! Specializes QAbstractTableModel for the BCEViewer program.  Sets
    flags, reimplements the rowCount and columnCount methods to return
    the numbers of actions. Also contains members to indicate which
    state the model is currently associated with.
  
  \ingroup viewer
*/
class BCEConditionalTableModel : public BCETableModel
{
  Q_OBJECT

public:
  //! Constructor
  BCEConditionalTableModel(BCEGame * _game,int _state):
    // 1 is the index for a conditional table model
    BCETableModel(_game,_state)
  { }

  //! Reimplements the data method
  /*! Retrieves the
    data using BCEGame::conditional. */
  QVariant data(const QModelIndex & index,
		int role) const Q_DECL_OVERRIDE;
    
  //! Reimplements the setData method
  /*! using BCEGame::setConditional. */
  bool setData(const QModelIndex & index, const QVariant & value, int role);
  
  //! Labels Rows and Columns in the Conditional Distn Table.
  QVariant headerData(int section,
		      Qt::Orientation orientation,
		      int role) const;

  //! Returns the number of row player types.
  int rowCount(const QModelIndex & parent = QModelIndex()) const Q_DECL_OVERRIDE {
    return game->getNumTypes()[0];
  }

  //! Returns the number of column player types.
  int columnCount(const QModelIndex & parent = QModelIndex()) const Q_DECL_OVERRIDE {
    return game->getNumTypes()[1];
  }

};





#endif
