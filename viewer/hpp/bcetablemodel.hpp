#ifndef BCETABLEMODEL_HPP
#define BCETABLEMODEL_HPP

#include <QtWidgets>
#include <QAbstractTableModel>
#include <QTableView>
#include "bcegame.hpp"

//! Table models for BCEViewer
/*! Specializes QAbstractTableModel for the BCEViewer program.  Sets
    flags, reimplements the rowCount and columnCount methods to return
    the numbers of actions. Also contains members to indicate which
    state the model is currently associated with.
  
  \ingroup viewer
*/
class BCETableModel : public QAbstractTableModel
{
  Q_OBJECT

public:
  //! Constructor
  BCETableModel(BCEGame * _game,
		int _state,
		int _index):
    game(_game), state(_state), index(_index)
  {}
  
  //! Returns flags.
  /*! Returns flags that indicate the model is enabled, can be edited,
      and can be selected. */
  Qt::ItemFlags flags(const QModelIndex & index) const 
  { return Qt::ItemIsEditable | Qt::ItemIsEnabled | Qt::ItemIsSelectable; }

  //! Returns the number of row player actions or types.
  int rowCount(const QModelIndex & parent = QModelIndex()) const Q_DECL_OVERRIDE
  {
    if (index == 0)
      return game->getNumActions()[0]; 
    if (index == 1)
      return game->getNumTypes()[0];
  }

  //! Returns the number of column player actions or types.
  int columnCount(const QModelIndex & parent = QModelIndex()) const Q_DECL_OVERRIDE
  { 
    if (index == 0)
      return game->getNumActions()[1];
    if (index == 1)
      return game->getNumTypes()[0];
  }

  //! Emits layoutChanged signal.
  void emitLayoutChanged()
  { emit layoutChanged(); }

  //! Sets the state to newState.
  bool setState(int newState)
  {
    if (newState < 0 || newState > game->getNumStates())
      return false;
    state = newState;
    return true;
  }
  
protected:
  //! The state that the model is associated with
  int state;
  //! Pointer to the associated game.
  BCEGame * game;
  //! Index, 0 = payoff model, 1 = conditional types model
  int index;
  
};





#endif
