#ifndef BCEOBJWEIGHTSTABLEMODEL_HPP
#define BCEOBJWEIGHTSTABLEMODEL_HPP

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
class BCEObjWeightsTableModel : public QAbstractTableModel
{
  Q_OBJECT

private:
  //! Vector storing weights on objectives.
  /*! Default set to a vector of 2 elements,
    each of weight 0.5.
   */
  vector<double> weightData = vector<double>(2,.5);
  //! Called by the model to change weight data.
  void setWeightData(int objective,int value)
  { weightData[objective] = value; }

public:
  //! Constructor
  BCEObjWeightsTableModel(BCEGame * _game):
    game(_game)
  { }
  
  //! Returns flags.
  /*! Returns flags that indicate the model is enabled, can be edited,
      and can be selected. */
  Qt::ItemFlags flags(const QModelIndex & index) const 
  { return Qt::ItemIsEditable | Qt::ItemIsEnabled | Qt::ItemIsSelectable; }

  //! Returns the number of row player actions.
  int rowCount(const QModelIndex & parent = QModelIndex()) const Q_DECL_OVERRIDE
  { return game->getNumObjectives(); }
  //! Returns the number of column player actions.
  int columnCount(const QModelIndex & parent = QModelIndex()) const Q_DECL_OVERRIDE
  { return 1; }

  //! Emits layoutChanged signal.
  void emitLayoutChanged()
  { emit layoutChanged(); }

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

  //! Adds an objective to the weight data vector.
  void addObjective(int position) {
    weightData.insert(weightData.begin()+position,0);
  }

  //! Removes an objective from the weight data vector.
  /*! Note that BCEGameHandler performs the check
    to determine if the vector has at least 1 element.
   */
  void removeObjective(int position) {
    weightData.erase(weightData.begin() + position);
  }

  vector<double>& getSolverData() {
    return weightData;
  }
  
protected:
  //! Pointer to the associated game.
  BCEGame * game;
  
};


#endif
