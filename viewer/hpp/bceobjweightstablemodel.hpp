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

#ifndef BCEOBJWEIGHTSTABLEMODEL_HPP
#define BCEOBJWEIGHTSTABLEMODEL_HPP

#include <QtWidgets>
#include <QAbstractTableModel>
#include <QTableView>
#include "bcegame.hpp"
#include "bceenumeration.hpp"

//! Model for Objective Weights in the BCEViewer
/*! Specializes QAbstractTableModel for the BCEViewer program.  Sets
    flags, reimplements the rowCount method to return
    the numbers of objectives.
  
    Also contains two data members, mainObjWeights and 
    secondaryObjWeights, that store the objective
    weights set by the user in the game tab. Interfacing with the
    game occurs through the add and remove objective functions in the
    bcegamehandler.

  \ingroup viewer
*/
class BCEObjWeightsTableModel : public QAbstractTableModel
{
  Q_OBJECT

private:
  //! Vector storing weights on main objective.
  vector<double> mainObjWeights;
  //! Vector storing weights on secondary objective.
  vector<double> secondaryObjWeights;
  //! Sets data in the objective weights vectors.
  void setWeightData(vector<int> cellLocation,double value) {
    if (cellLocation[1]==0)
      mainObjWeights[cellLocation[0]] = value;
    else
      secondaryObjWeights[cellLocation[0]] = value;
  }
  //! Vector storing objective labels
  vector<string> objectiveLabels;

public:
  //! Constructor
  /*! Sets the size of the weightData vector equal to
    the number of objectives in the game. Sets default
    weight data to .5 on player 0 and player 1's objectives.
    Gets any existing objective labels from the game. 
  */
  BCEObjWeightsTableModel(BCEGame * _game):
    game(_game)
  {

    mainObjWeights = vector<double>(game->getNumObjectives(),0);
    secondaryObjWeights = vector<double>(game->getNumObjectives(),0);

    mainObjWeights[0] = 1;
    secondaryObjWeights[1] = 1;

    objectiveLabels = game->getObjLabels();

    if (objectiveLabels.size() < game->getNumObjectives()) {
      int currLabelIndex = objectiveLabels.size();
      while (currLabelIndex < game->getNumObjectives() + 1) {
	objectiveLabels.push_back(string());
	currLabelIndex++;
      }
    }
  }

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
  { return 2; }

  //! Emits layoutChanged signal.
  void emitLayoutChanged()
  { emit layoutChanged(); }

  //! Reimplements the data method
  /*! Retrieves the current weight set for a specified
    objective. */
  QVariant data(const QModelIndex & index,
		int role) const Q_DECL_OVERRIDE;
    
  //! Reimplements the setData method
  /*! Sets the weight on the specified objective. */
  bool setData(const QModelIndex & index, const QVariant & value, int role);

  //! Returns formatted header data
  /*! Returns header data formatted to indicate the appropriate 
    objective labels. */
  QVariant headerData(int section,
		      Qt::Orientation orientation,
		      int role) const Q_DECL_OVERRIDE;

  //! Adds an objective to the weight data vector.
  void addObjective(int position) {
    mainObjWeights.insert(mainObjWeights.begin()+position,0);
    secondaryObjWeights.insert(secondaryObjWeights.begin()+position,0);
    objectiveLabels.insert(objectiveLabels.begin()+position,
			   game->getObjLabels(position));
  }

  //! Removes an objective from the weight data and label vectors.
  /*! Note that BCEGameHandler performs the check
    to determine if the vector has at least 1 element. */
  void removeObjective(int position) {
    mainObjWeights.erase(mainObjWeights.begin() + position);
    secondaryObjWeights.erase(secondaryObjWeights.begin() + position);
    objectiveLabels.erase(objectiveLabels.begin() + position);
  }

  //! Returns a reference to the weight data vector.
  const vector<double> getSolverData() {
    return mainObjWeights;
  }

  //! Returns a reference to the weight data vector.
  const vector<vector<double> > getMapBoundaryData() {
    vector<vector<double> > weightData;
    weightData.push_back(mainObjWeights);
    weightData.push_back(secondaryObjWeights);
    return weightData;
  }
  
protected:
  //! Pointer to the associated game.
  BCEGame * game;
  
};


#endif
