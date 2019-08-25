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

/*!
  \mainpage BCESolve Documentation

  \section introsec Introduction

  BCESolve is a library of routines for representing, solving, and
  analyzing Bayes correlated equilibria (BCE), a solution concept that
  generalized correlated equilibrium to games of incomplete
  information. BCE, like correlated equilibria, are joint
  distributions that satisfy a series of linear constraints. The
  library utilizes Gurobi's linear programming package to solve for
  BCE that maximize a given linear objective, such as expected payoffs
  of the players.

  The package consists of two main components. The first piece is a
  library of C++ classes for representing and solving games of
  incomplete information, and the second is a graphical user interface
  (BCEViewer) for using and interacting with the solver library. The
  purpose of this guide is to give advanced users an overview of how
  the library and graphical interface are structured.

  BCESolve makes use of external software packages: the Boost
  libraries are used for serialalization of data relating to games and
  solutions, which are represented with the BCEGame and BCESolution
  classes and are saved as .bgm and .bce files, respectively. The
  graphical interface uses the Qt framework and the Qt plotting
  library QCustomPlot.

  \section installsec Installation

  To use this software, you can directly download the BCEViewer
  binaries, which are precompiled for Linux, OS X, and MS Windows. The
  source code is also available at
  www.benjaminbrooks.net/research.shtml. The code has been compiled
  on: CentOS 7 using gcc version 4.8.3 and Boost 1.53; and on OS X
  using LLVM version 7.0.2 and Boost 1.57; and on MS Windows using
  MinGW 4.92. In both cases, the BCEViewer was compiled using Qt
  5.5. The class BCESolver uses Gurobi for linear programming, and the
  code was compiled with Gurobi 5.63 on Linux and Gurobi 6.50 on OS
  X. To compile the source and example code yourself, you need to
  change the relevant variables in localsettings.mk and
  locasettings2.mk to direct the compiler to Boost and
  Gurobi. Importantly, just directing the compiler to Gurobi is not
  sufficient; your Gurobi installation must be associated with a valid
  license. A free academic license may be obtained here: <a
  href="http://www.gurobi.com/registration/academic-license-reg">Gurobi
  University License</a>. You can then build the desired examples by
  calling "make examplename" (the .cpp or .hpp ending is not included)
  in the example dirctory. After you build the source, you can build
  the BCEViewer program from the viewer directory by first calling
  "qmake" and then running "make".

  \section solversec Overview of the solver library

  The BCESolve library contains a series of classes for representing,
  solving, and analyzing BCE. The BCEAbstractGame class is a base
  class for representing games of incomplete information. One must
  derive from this class, and reimplement its BCEGame::objective and
  BCEGame::prior methods to reflect the payoff functions and the
  distribution of the state and types for the particular game of
  interest.

  The BCESolver class uses Gurobi to solve a game represented by an
  object derived from the BCEAbstractGame class, by maximizing a
  particular linear objective over all BCE for that game. The output
  from the BCESolver class is stored in a BCESolution
  object. BCESolution objects can be serialized and de-serialized, and
  the class also contains routines for analyzing the BCE contained
  within.

  For a more detailed description of the solver library and its usage,
  see \subpage sourcepage.

  \section viewersec Overview of the graphical interface

  BCESolve includes a graphical interface for interacting with the
  library called BCEViewer. The interface contains three tabs: a game
  tab, a solution tab, and a log tab. The game tab facilitates both
  creation of custom games and editing of games that were generated
  outside the interface. The solution tab has various visualizations
  for understanding equilibria. The log tab outputs progress and
  numerical solutions for any games games solved within the interface.

  For more details about the object model underlying BCEViewer, see
  the \subpage viewerpage.

  \section examplesec Examples

  We have included several examples of how to use the BCESolve
  package. The files allpayauction.hpp and allpayauction.cpp are an
  example of a standard, common-values allpay auction in which bidders
  pay their bids, irrespective of whether or not they win the
  good. The header file allpayauction.hpp shows how to construct a
  simple BCEGame by specifying the objectives and prior over the
  states (uniform, in this example). The example
  commonvalueauction.hpp specifies a prior over values distributed
  according to the CDF \f$ v^{\alpha} \f$, where\f$ v \f$ denotes the
  value in [0,1].

  The header fpaknown.hpp formulates a private-values first-price
  auction. The header file shows to how implement private values
  within the library's framework. The file fpaknown.cpp solves the
  model using the mapBoundary method, which traces out the equilibria
  that maximize the weighted sum of a given pair of objectives for all
  possible weights. This payoffs themselves are saved in a data file,
  "fpaknown.dat," so that it can also be analyzed elsewhere. The model
  in fpaunknown.cpp is of a first-price auction in which bidders do
  not know their own values.

  \section conclusionsec Final thoughts

  The package has many more features that the user will no doubt
  discover. Within the src folder is a src/MATLAB subfolder, that
  contains tools for interfacing between MATLAB and BCESolve. In
  particular, bcemex.cpp is a mex program that can be used to create
  games and load solution files from within MATLAB. There is also a
  matlab m-file bcemexbuild.m that was used to build bcemex on Linux
  and with MATLAB R2014a. (Note that building bcemex is a bit tricky
  since the mex file needs to be linked to position independent code,
  and the default Boost libraries obtained through most package
  managers were compiled without the -fPIC compiler flag. You may need
  to build the Boost serialization library from the source code.)

  This program would not have been possible without the support of
  numerous groups and the contributions of others, and it is entirely
  fitting and appropriate that their contributions should be
  acknowledged. In particular, this program was developed with and
  incorporates elements of a number of other open source projects,
  including the Qt application framework (www.qt.io), Emmanuel
  Eichhammer's QCustomPlot (www.qcustomplot.com), the Boost libraries
  (www.boost.org), and the GNU project (www.gnu.org). We would also
  like to gratefully acknowledge support from the Becker Friedman
  Institute and the University of Chicago.

  Finally, it should go without saying that this program is a work in
  progress. Feedback, bug reports, and contributions are much
  appreciated.

  Enjoy!

  Ben Brooks
  Chicago, IL
  ben@benjaminbrooks.net

  Robbie Minton
  Chicago, IL
  rminton@uchicago.edu
*/

/*! \defgroup src BCESolve library

  @brief The main solver library.

  For a detailed description of the BCESolve library, see \ref
  sourcepage.
 */

/*! \defgroup viewer BCEViewer graphical interface

  @brief The graphical user interface.

  For a detailed description of the BCEViewer object model, see \ref
  viewerpage.
 */

/** \example allpayauction.hpp
    \example allpayauction.cpp
    \example fparandomrp.hpp
    \example fparandomrp.cpp
    \example hybridentryreserve.hpp
    \example hybridentryreserve.cpp
    \example postedprice.hpp
    \example postedprice.cpp
    \example fpagame.hpp
    \example fpaknown.hpp
    \example fpaknown.cpp
    \example fpaunknown.hpp
    \example fpaunknown.cpp
 */

/*! \page sourcepage The BCESolve library

  @brief A detailed description of the BCESolve library.

  \section srcoverviewsec Overview

  The source module is comprised of a library of classes for
  specifying, solving, and exploring the solutions of two player games
  with incomplete information. This library provides the core
  functionality underling the BCEViewer program which is described in
  \ref viewerpage. A list of classes is contained here: \ref src. This
  page provides an overview of the object model, with additional
  details in the respective class pages.

  \section srcgamesec Specifying a game

  The first step in using the BCESolve library is translating the
  abstract formulation of a game into a format that can be
  understood by the library. This translation is facilitated by the
  BCEAbstractGame and BCEGame classes.

  BCEAbstractGame has protected data members that store the number of
  actions, number of states, number of types, number of objectives,
  and whether the game has a product structure. BCEAbstractGame has
  multiple constructors. The default constructor is used primarily for
  creating "blank" games within the graphical interface. The second
  constructor takes four integer inputs, each specifying one of the
  above-mentioned parameters, and creates a game where each player has
  the same number of types and actions. The third constructor instead
  takes two-element vectors of actions and types. This constructor
  allows players to have different numbers of actions and types.

  The BCEGame class derives from BCEAbstractGame. Its private members
  include the payoffs for each objective, the prior over each state,
  the conditional distribution of types, data for dominated actions,
  and data for feasible deviations.

  BCEGame objects can be serialized and deserialized through static
  BCEGame::save and BCEGame::load methods. See allpayauction.cpp for an
  example. Serialized BCEGame objects can be loaded by BCEViewer.

  \section srcsolversec Solving a game

  Once one has specified the game, the next step is to solve that game
  by constructing a BCESolver from a BCEAbstractGame type
  reference. Once the BCESolver is constructed, the computation of the
  Bayes correlated equilibrium is invoked first by calling
  BCESolver::populate(), which constructs the linear probability and
  incentive constraints and objectives, and then by calling using
  BCESolver::solve, which uses Gurobi to solve the resulting linear
  program. The solve routine takes as an input a vector of weights on
  objectives in the associated BCEAbstractGame, which are used to
  define the objective for the linear program. One can also use
  BCESolver::mapBoundary to solve for all BCE that maximize weighted
  sum of a given pair of objectives.

  \section srcsolutionsec Using the solution

  The output of the algorithm is stored in a BCESolution object, which
  can be accessed using BCESolver::getSolution. BCESolution contains
  methods for accessing data of interest within the solution and
  accompanying game objects. The getExpectedObjectives method, for
  example, returns the expected objectives for the BCE stored in the
  given data.

  BCESolution contains static methods for serialization
  (BCESolution::save) and deserialization (BCESolution::load). See
  allpayauction.cpp for an example of how these are used. Serialized
  BCESolution objects can then be loaded by BCEViewer.

 */

/*! \page viewerpage The BCEViewer graphical interface
  @brief An detailed description of the BCEViewer graphical interface.
  \section vieweroverviewsec Introduction

  The BCEViewer module is a graphical interface for specifying,
  solving, and exploring the solutions of games with incomplete
  information. The interface is written using Qt 5.5 and the
  QCustomPlot library (www.qcustomplot.com). This section of the
  describes the object model underlying the program. For a brief
  description of how to use the graphical interface, please see \ref
  viewersec.

  The graphical interface is initialized by the BCEWindow class. This
  class constructs the rest of the interface and handles some high
  level functions such as loading and saving games and solutions,
  solving games, and keyboard commands.

  The layout has three tabs: these are the "solution tab", the "game
  tab", and the "log tab". The solution tab is for exploring the
  solution of games, and the game tab is for specifying and viewing
  games. The log tab's function is primarily for displaying the
  progress of the algorithm during computation. Each of the game and
  solution tabs has a separate class associated with handling the
  functionality of that tab.

  \section viewergametabsec The game tab

  The game tab is managed by an object of the BCEGameHandler
  class. The game handler contains a copy of a game, and handles the
  interface between various tables and controls for editing payoffs,
  the prior, the conditional distribution of types, and weights on
  objectives for optimization. The game tab displays the payoffs and
  conditional distribution of types for one state at a time. Different
  states are selected using controls at the top of the tab.

  The editing of payoffs and probabilities is implemented using Qt's
  model-view framework. The tables are BCETableView objects, derived
  from QTableView. Each table has a model associated with it which is
  derived from BCETableModel, which is in turn derived from
  QAbstractTableModel. BCETableModel adds private members: a pointer
  to an associated BCEGame object and an int "state" member, which is
  the state that is currently being edited. For payoff tables, the
  model is BCEPayoffTableModel, which adds methods for generating
  header data to indicate action profiles and also defines
  setData/getData methods for interfacing with the BCEGame object. The
  table displaying the conditional distribution of types, a
  BCEConditionalTableModel, replicates the functionality of the payoff
  table model. It handles modifying data related to the distribution
  of types in the BCEGame object. Correspondingly, the table
  displaying the prior over the states, a BCEPriorTableModel, handles
  modifying the prior data in the BCEGame object. The final table,
  BCEObjWeightsTableModel, manages its data internally, since the
  BCEGame object contains no data pertaining to which objectives will
  be maximized or minimized when the solve routine is run. Instead,
  when the solve routine is run, BCEGameHandler retrieves the
  objective weights from the BCEObjWeightsTableModel object.

  Since some table models are state dependent, when the current state
  is changed by the user, BCEGameHandler simply updates the state
  parameters of all of the table models and sends out signals to
  update the displayed data. Data displayed in the payoff table model
  and the conditional table model will reflect the new state. The
  prior table model and objective weights table model exist
  independently of the state and will not change.

  The game tab also has controls for changing the numbers of actions,
  states, and types. When these options are selected, BCEGameHandler
  simply invokves the corresponding method in the BCEGame class and
  signals that the models need to update their layouts.

  Finally, the game tab has a "Solve" push button which triggers the
  solve routine, and a "Cancel" push button for interrupting the
  computation. More on this in the next subsection.

  \section viewersolvesec Solving a game

  When the user presses the "Solve" button on the game tab, the signal
  is handled by the BCEWindow class which begins a computation using
  the algorithm. This computation is handled via an intermediary class
  called BCESolverWorker. To start the algorithm, the main window
  constructs a BCESolverWorker and moves it to a new thread so as not
  to freeze the program while the computation progresses. The worker
  constructs a BCESolver object for the given game. The main window
  and the worker communicate back and forth to manage the progress of
  the algorithm and output via a BCEGurobiCallback object. The
  callback object checks periodically to determine if the user has
  triggered the cancel flag, either by hitting the cancel button in
  the game tab or by clicking "Cancel Solve" in the tools menu of the
  menu bar at the top of the graphical interface. At each of these
  periodic checks, the callback object uses std::cout to emit
  information about the progress of the alorithm solving the linear
  program. The main window and the worker communicate back and forth
  in this manner until the computation terminates, at which point the
  BCESolution object generated by the computation is copied to
  BCEPlotHandler and the worker is destroyed.

  \section viewersolutionsec Interacting with the solution

  When a solution is loaded through BCEWindow or when one is produced
  by solving a game, it is passed to BCEPlotHandler, which is the
  class that controls the solution tab. This tab contains various
  plots for visualizing the computations performed by the algorithm
  and the final solution of the game. BCEPlotHandler also passes the
  solution to BCEDataState, which contains methods for manipulating
  the BCESolution into vectors and matrices easily amenable to
  plotting.

  The solution tab presents a series of BCEValueSetPlot objects, which
  are derived from QCustomPlot. These serve to add tooltip
  functionality such as displaying coordinates when hovering the mouse
  over the plot. A right-click on the plots opens a menu with options
  to save a single plot as a .png or .pdf.  A click on points in the
  plot in the top left of the GUI will select new equilibria, if more
  than one equilibrium is contained within the solution object.

  The primary function of the BCEPlotHandler is to handle the plotting
  of the data from its BCEDataState member. Which part of the solution
  is plotted depends on a number of parameters that are controlled by
  the user through various widgets. These widgets control the plots
  indirectly through the BCEDataState object.  This object aggregates
  all of the settings in the widgets into one set of parameters and
  manipulates relevant data for plotting.  In particular, when one of
  the controls is changed, the corresponding signal is connected to a
  slot in BCEDataState that updates the parameter value, manipulates
  relevant data for plotting, and then signals to BCEPlotHandler to
  replot.

 */
