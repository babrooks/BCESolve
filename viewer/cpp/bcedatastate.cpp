#include "bcedatastate.hpp"

void BCEDataState::setupControlsLayout() {

  // Slider, LineEdit, and CheckBox Controls Creation

  for (int player = 0; player < 2; player++) {
    sliderGroup.push_back(new BCESlider(Action,player));
    sliderGroup.push_back(new BCESlider(Type,player));
    sliderGroup.push_back(new BCESlider(State,player));

    lineEditGroup.push_back(new BCELineEdit(Action,player));
    lineEditGroup.push_back(new BCELineEdit(Type,player));
    lineEditGroup.push_back(new BCELineEdit(State,player));

    checkBoxGroup.push_back(new BCECheckBox(Action,player));
    checkBoxGroup.push_back(new BCECheckBox(Type,player));
    checkBoxGroup.push_back(new BCECheckBox(State,player));
  }


  for (int widgetIndex = 0; widgetIndex < 6; widgetIndex++) {
    lineEditGroup[widgetIndex]->setReadOnly(true);
    sliderGroup[widgetIndex]->setMaximumHeight(resHeight/54);
    sliderGroup[widgetIndex]->setMinimumWidth(resWidth/16);
    sliderGroup[widgetIndex]->setOrientation(Qt::Horizontal);
    lineEditGroup[widgetIndex]->setMaximumSize(resWidth/64,resHeight/54);
    connect(sliderGroup[widgetIndex],
	    SIGNAL(valueChanged(int,BCESliderType,int)),
	    this,SLOT(setSliderValue(int,BCESliderType,int)));
    connect(this,
	    SIGNAL(valueChanged(int,BCESliderType,int)),
	    lineEditGroup[widgetIndex],
	    SLOT(changeDisplayValue(int,BCESliderType,int)));
    connect(this,
	    SIGNAL(valueChanged(int,BCESliderType,int)),
	    sliderGroup[widgetIndex],
	    SLOT(changeSliderPosition(int,BCESliderType,int)));
    connect(checkBoxGroup[widgetIndex],
	    SIGNAL(boolChanged(bool,BCESliderType,int)),
	    this,SLOT(setMarginalConditions(bool,BCESliderType,int)));
  }

  // Slider Labels

  QVector<BCELabel*> sliderLabels;

  for (int player = 0; player < 2; player++) {
    sliderLabels.push_back(new BCELabel(SliderLabel,Action,player));
    sliderLabels.push_back(new BCELabel(SliderLabel,Type,player));
    sliderLabels.push_back(new BCELabel(SliderLabel,State,player));	 
  }


  for (int labelIt = 0; labelIt < 6; labelIt++) {
    sliderLabels[labelIt]->setMaximumHeight(resHeight/54);
    connect(this,SIGNAL(sliderLabelsChanged(bool,int)),
	    sliderLabels[labelIt],SLOT(displayStateOrValues(bool,int)));
  }

  for (int i = 0; i < 2; i++)
    checkBoxGroup[3*i]->setChecked(true);

  // Slider, LineEdit, and CheckBox Layout Creation

  QVector<QHBoxLayout*> gridSubLayouts;
  QVector<QVBoxLayout*> subLayoutWithLabels;
  QGridLayout *controlsGrid = new QGridLayout();

  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < 3; j++) {
      gridSubLayouts.push_back(new QHBoxLayout());
      subLayoutWithLabels.push_back(new QVBoxLayout());
      gridSubLayouts[3*i+j]->addWidget(sliderGroup[3*i+j]);
      gridSubLayouts[3*i+j]->addWidget(lineEditGroup[3*i+j]);
      gridSubLayouts[3*i+j]->addWidget(checkBoxGroup[3*i+j]);
      subLayoutWithLabels[3*i+j]->addWidget(sliderLabels[3*i+j]);
      subLayoutWithLabels[3*i+j]->addLayout(gridSubLayouts[3*i+j]);
      controlsGrid->addLayout(subLayoutWithLabels[3*i+j],j,i); // Layout Matrix
    } // Rows
  } // Columns

  controlsLayout = controlsGrid;

  // End Slider, LineEdit, CheckBox Creation
  //////////////////////////////////////////
}
