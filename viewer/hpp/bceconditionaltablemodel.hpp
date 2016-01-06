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
