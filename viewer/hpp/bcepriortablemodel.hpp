#ifndef BCEPRIORTABLEMODEL_HPP
#define BCEPRIORTABLEMODEL_HPP

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
class BCEPriorTableModel : public QAbstractTableModel
{
  Q_OBJECT

public:
  //! Constructor
  BCEPriorTableModel(BCEGame * _game):
    game(_game)
  { }
  
  //! Returns flags.
  /*! Returns flags that indicate the model is enabled, can be edited,
      and can be selected. */
  Qt::ItemFlags flags(const QModelIndex & index) const 
  { return Qt::ItemIsEditable | Qt::ItemIsEnabled | Qt::ItemIsSelectable; }

  //! Returns the number of row player actions.
  int rowCount(const QModelIndex & parent = QModelIndex()) const Q_DECL_OVERRIDE
  { return game->getNumStates(); }
  //! Returns the number of column player actions.
  int columnCount(const QModelIndex & parent = QModelIndex()) const Q_DECL_OVERRIDE
  { return 1; }

  //! Emits layoutChanged signal.
  void emitLayoutChanged()
  { emit layoutChanged(); }

  //! Reimplements the data method
  /*! Retrieves the prior over a state. Retrieves the
      data using BCEGame::prior. */
  QVariant data(const QModelIndex & index,
		int role) const Q_DECL_OVERRIDE;
    
  //! Reimplements the setData method
  /*! Sets the prior over a state, using BCEGame::setPrior. */
  bool setData(const QModelIndex & index, const QVariant & value, int role);

  //! Returns formatted header data
  /*! Returns header data formatted to indicate the appropriate 
    objective labels. */
  QVariant headerData(int section,
		      Qt::Orientation orientation,
		      int role) const Q_DECL_OVERRIDE;
  
protected:
  //! Pointer to the associated game.
  BCEGame * game;
  
};





#endif
