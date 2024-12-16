/*
  ==============================================================================

Date: 2018
Author: Juce + Adam Korytowski

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class GeneratorWt1AudioProcessorEditor  : public AudioProcessorEditor, private Timer, private Slider::Listener, private Label::Listener
{
public:
    GeneratorWt1AudioProcessorEditor (GeneratorWt1AudioProcessor&);
    ~GeneratorWt1AudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;
	void sliderValueChanged(Slider* slider) override;

	void Label::Listener::labelTextChanged(Label * labelThatHasChanged) {}

/*private:*/
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    GeneratorWt1AudioProcessor& processor;
	void timerCallback() override
	{
		keyboardComponent.grabKeyboardFocus();
		stopTimer();
	}

	Slider gainSlider, squareRatioSlider, sineRatioSlider, noiseRatioSlider, sine2RatioSlider;
	Slider sineParameterSlider, squareParameterSlider, sine2ParameterSlider;
	MidiKeyboardComponent keyboardComponent;

	Label sineL, sine2L, squareL, noiseL, sineParamL, sine2ParamL, squareParamL;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GeneratorWt1AudioProcessorEditor)
};
