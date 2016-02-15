#ifndef BCELABELHANDLER_HPP
#define BCELABELHANDLER_HPP

#include <QtWidgets>
#include <sstream>
#include "bceenumeration.hpp"
#include <iomanip>

//! Class handling all labeling in the solution tab.
/*! This class has a series of constructors to create labels
  for plots and controls. Also contains slots to change
  plot labels after the user alters parameters in the 
  solution tab.

  \ingroup viewer

 */
class BCELabel : public QLabel {
  Q_OBJECT;

private:

  //! Player associated with the label.
  int player;
  //! Type of label (i.e. "DeviationPlot," "SliderLabel," etc.).
  LabelType labelType;
  //! Type of Slider associated with a slider label.
  BCESliderType sliderType;

  std::stringstream dynamicString;

public:

  //! Constructs the plot titles for the deviation bar graphs.
  BCELabel(LabelType _labelType,int _player);

  //! Constructs slider labels for controls layout.
  BCELabel(LabelType _labelType,BCESliderType _sliderType,int _player);

  //! Constructs plot titles for heatmap (right panel) and set of BCE plot (top left).
  BCELabel(LabelType _labelType);

public slots:

  void changeText(int emittedPlayer,
		  int action,
		  int type,
		  double objectiveValue);

  void changeProbability(int emittedPlayer,double probability);

  void displayStateOrValues(bool isPrivateVals,
			    int emittedPlayer);

  void changeDisplayedCoords(double x,double y);

  void changeDisplayedState(int value0,
			    int value1,
			    int state,
			    bool isPrivateVals);

};

#endif
