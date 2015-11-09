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
    conventions. Also reimplements the data() method to display an
    ordered pair of the two players' payoffs. 
    
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
  /*! Returns an ordered pair of the two players' payoffs for the
      action profile specified in index. Reads this data from the game
      object using SGGame::getPayoffs. */
  QVariant data(const QModelIndex & index,
		int role) const Q_DECL_OVERRIDE;

  //! Returns formatted header data
  /*! Returns header data formatted to indicate the appropriate row/
      column action. */
  QVariant headerData(int section,
		      Qt::Orientation orientation,
		      int role) const Q_DECL_OVERRIDE;

  //! Sets data
  /*! Parses the data in value to set a new pair of payoffs. Sets the
      data using SGGame::setData. */
  bool setData(const QModelIndex & index, const QVariant & value, int role);
  
}; // BCEPayoffTableModel

#endif
