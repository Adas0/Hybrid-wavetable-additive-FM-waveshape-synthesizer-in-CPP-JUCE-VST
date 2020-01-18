/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
GeneratorWt1AudioProcessorEditor::GeneratorWt1AudioProcessorEditor (GeneratorWt1AudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p),
	keyboardComponent(processor.keyboardState, MidiKeyboardComponent::horizontalKeyboard)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
	addAndMakeVisible(keyboardComponent);
	setSize(600, 160);
	startTimer(400);

	/*releaseSlider.setSliderStyle(Slider::Rotary);
	releaseSlider.setRange(0.999, 0.99999, 0.001);
	releaseSlider.setTextBoxStyle(Slider::NoTextBox, false, 20, 0);
	releaseSlider.setPopupDisplayEnabled(true, false, this);
	releaseSlider.setTextValueSuffix("release");
	releaseSlider.setValue(0.999);
	addAndMakeVisible(&releaseSlider);
	releaseSlider.addListener(this);*/
}

GeneratorWt1AudioProcessorEditor::~GeneratorWt1AudioProcessorEditor()
{
}

//==============================================================================
void GeneratorWt1AudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

    g.setColour (Colours::white);
    g.setFont (15.0f);
    g.drawFittedText ("Hello World!", getLocalBounds(), Justification::centred, 1);

	//releaseSlider.setBounds(0, 0, 200, 200);
}

void GeneratorWt1AudioProcessorEditor::resized()
{
	keyboardComponent.setBounds(10, 10, getWidth() - 20, getHeight() - 20);
	
	//processor.synthAudioSource.releaseTime = releaseSlider.getValue();


     /*This is generally where you'll want to lay out the positions of any
     subcomponents in your editor..*/
}
