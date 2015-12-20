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
    BCETableModel(_game,_state,1)
  { }

  //! Reimplements the data method
  /*! Retrieves the probability of going to nextState from state, when
    the action profile indicated by index is played. Retrieves the
    data using SGGame::getProbabilities. */
  QVariant data(const QModelIndex & index,
		int role) const Q_DECL_OVERRIDE;
    
  //! Reimplements the setData method
  /*! Sets the transition probability from state to nextState to
    value, using SGGame::setProbability. */
  bool setData(const QModelIndex & index, const QVariant & value, int role);
  
  //! Labels Rows and Columns in the Conditional Distn Table.
  QVariant headerData(int section,
		      Qt::Orientation orientation,
		      int role) const;
};





#endif
