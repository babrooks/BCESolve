#ifndef BCEPAYOFFTABLEMODEL_HPP
#define BCEPAYOFFTABLEMODEL_HPP

#include <QtWidgets>
#include <QMainWindow>
#include <QAbstractTableModel>
#include <QTableView>
#include "bcegame.hpp"
#include "bcetablemodel.hpp"

//! Derived class for payoff table models
/*! This class handles the interface between BCETableView and the
    payoff matrices in BCEGame. Adds functionality for
    header data (using the row player action/ column player action
    conventions. Also reimplements the data() method to display comma
    separated entries for objective payoffs. 
    
    \ingroup viewer
*/
class BCEPayoffTableModel : public BCETableModel
{
  Q_OBJECT

public:
  //! Constructor
  BCEPayoffTableModel(BCEGame * _game,
		     int _state):
    BCETableModel(_game,_state)
  {}
  
  //! Returns formatted data
  /*! Returns comma separated entries of objective payoffs for the
      action profile specified in index. Reads this data from the game
      object using BCEGame::objective. */
  QVariant data(const QModelIndex & index,
		int role) const Q_DECL_OVERRIDE;

  //! Returns formatted header data
  /*! Returns header data formatted to indicate the appropriate row/
      column action. */
  QVariant headerData(int section,
		      Qt::Orientation orientation,
		      int role) const Q_DECL_OVERRIDE;

  //! Sets data
  /*! Parses the data in value to set new payoffs for the objective. Sets the
      data using BCEGame::setObjective. */
  bool setData(const QModelIndex & index, const QVariant & value, int role);

  //! Returns the number of row player actions.
  int rowCount(const QModelIndex & parent = QModelIndex()) const Q_DECL_OVERRIDE {
    return game->getNumActions()[0];
  }
  
  //! Returns the number of column player actions.
  int columnCount(const QModelIndex & parent = QModelIndex()) const Q_DECL_OVERRIDE {
    return game->getNumActions()[1];
  }

}; // BCEPayoffTableModel

#endif
