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
	squareRatioSlider.setRange(0, 3, 0.1);
	squareRatioSlider.setTextBoxStyle(Slider::NoTextBox, false, 20, 0);
	squareRatioSlider.setPopupDisplayEnabled(true, false, this);
	squareRatioSlider.setTextValueSuffix("");
	squareRatioSlider.setValue(1);
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
	noiseRatioSlider.setRange(0, 8, 0.1);
	noiseRatioSlider.setTextBoxStyle(Slider::NoTextBox, false, 20, 0);
	noiseRatioSlider.setPopupDisplayEnabled(true, false, this);
	noiseRatioSlider.setTextValueSuffix("");
	noiseRatioSlider.setValue(2.5);
	addAndMakeVisible(&noiseRatioSlider);
	noiseRatioSlider.addListener(this);

	sineParameterSlider.setSliderStyle(Slider::LinearHorizontal);
	sineParameterSlider.setRange(1, 40, 0.1);
	sineParameterSlider.setTextBoxStyle(Slider::NoTextBox, false, 20, 0);
	sineParameterSlider.setPopupDisplayEnabled(true, false, this);
	sineParameterSlider.setTextValueSuffix("");
	sineParameterSlider.setValue(1);
	addAndMakeVisible(&sineParameterSlider);
	sineParameterSlider.addListener(this);

	squareParameterSlider.setSliderStyle(Slider::LinearHorizontal);
	squareParameterSlider.setRange(1, 40, 0.1);
	squareParameterSlider.setTextBoxStyle(Slider::NoTextBox, false, 20, 0);
	squareParameterSlider.setPopupDisplayEnabled(true, false, this);
	squareParameterSlider.setTextValueSuffix("");
	squareParameterSlider.setValue(1);
	addAndMakeVisible(&squareParameterSlider);
	squareParameterSlider.addListener(this);

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
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

}

void GeneratorWt1AudioProcessorEditor::resized()
{
	//keyboardComponent.setBounds(10, 10, getWidth() - 20, getHeight() - 20);
	//gainSlider
	//gainSlider.setBounds(10, 10, 30, 150);
	squareRatioSlider.setBounds(110, 10, 30, 150);
	sineRatioSlider.setBounds(210, 10, 30, 150);
	noiseRatioSlider.setBounds(310, 10, 30, 150);
	sineParameterSlider.setBounds(110, 180, 150, 30);
	squareParameterSlider.setBounds(110, 280, 150, 30);
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
	processor.synthAudioSource.sineParameter = sineParameterSlider.getValue();
	processor.synthAudioSource.squareParameter = squareParameterSlider.getValue();
	//SineWaveVoice::a 
}
