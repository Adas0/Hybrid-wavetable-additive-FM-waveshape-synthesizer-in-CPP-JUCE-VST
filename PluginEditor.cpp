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
	setSize(600, 500);
	startTimer(400);


	/*gainSlider.setSliderStyle(Slider::LinearVertical);
	gainSlider.setRange(0.0, 1.0, 0.1);
	gainSlider.setTextBoxStyle(Slider::NoTextBox, false, 20, 0);
	gainSlider.setPopupDisplayEnabled(true, false, this);
	gainSlider.setTextValueSuffix("Gain");
	gainSlider.setValue(0.5);
	addAndMakeVisible(&gainSlider);
	gainSlider.addListener(this);*/

	squareRatioSlider.setSliderStyle(Slider::LinearVertical);
	squareRatioSlider.setRange(0, 10, 0.1);
	squareRatioSlider.setTextBoxStyle(Slider::NoTextBox, false, 20, 0);
	squareRatioSlider.setPopupDisplayEnabled(true, false, this);
	squareRatioSlider.setTextValueSuffix("");
	squareRatioSlider.setValue(5);
	addAndMakeVisible(&squareRatioSlider);
	squareRatioSlider.addListener(this);

	sineRatioSlider.setSliderStyle(Slider::LinearVertical);
	sineRatioSlider.setRange(0, 10, 0.1);
	sineRatioSlider.setTextBoxStyle(Slider::NoTextBox, false, 20, 0);
	sineRatioSlider.setPopupDisplayEnabled(true, false, this);
	sineRatioSlider.setTextValueSuffix("");
	sineRatioSlider.setValue(5);
	addAndMakeVisible(&sineRatioSlider);
	sineRatioSlider.addListener(this);

	noiseRatioSlider.setSliderStyle(Slider::LinearVertical);
	noiseRatioSlider.setRange(0, 5, 0.1);
	noiseRatioSlider.setTextBoxStyle(Slider::NoTextBox, false, 20, 0);
	noiseRatioSlider.setPopupDisplayEnabled(true, false, this);
	noiseRatioSlider.setTextValueSuffix("");
	noiseRatioSlider.setValue(2.5);
	addAndMakeVisible(&noiseRatioSlider);
	noiseRatioSlider.addListener(this);

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
    //g.drawFittedText ("Hello World!", getLocalBounds(), Justification::centred, 1);

	//gainSlider.setBounds(0, 0, 100, 200);
}

void GeneratorWt1AudioProcessorEditor::resized()
{
	//keyboardComponent.setBounds(10, 10, getWidth() - 20, getHeight() - 20);
	//gainSlider
	//gainSlider.setBounds(10, 10, 30, 150);
	squareRatioSlider.setBounds(110, 10, 30, 150);
	sineRatioSlider.setBounds(210, 10, 30, 150);
	noiseRatioSlider.setBounds(310, 10, 30, 150);

	//processor.synthAudioSource.releaseTime = releaseSlider.getValue();


     /*This is generally where you'll want to lay out the positions of any
     subcomponents in your editor..*/
}

void GeneratorWt1AudioProcessorEditor::sliderValueChanged(Slider* slider)
{
	//processor.synthAudioSource.signalTable.applyGain(gainSlider.getValue());
	processor.synthAudioSource.squareRatio = squareRatioSlider.getValue();
	processor.synthAudioSource.sineRatio = sineRatioSlider.getValue();
	processor.synthAudioSource.noiseRatio = noiseRatioSlider.getValue();
	//SineWaveVoice::a 
}
