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

	squareRatioSlider.setSliderStyle(Slider::LinearHorizontal);
	squareRatioSlider.setRange(0, 3, 0.1);
	squareRatioSlider.setTextBoxStyle(Slider::NoTextBox, false, 20, 0);
	squareRatioSlider.setPopupDisplayEnabled(true, false, this);
	squareRatioSlider.setTextValueSuffix("");
	squareRatioSlider.setValue(1);
	addAndMakeVisible(&squareRatioSlider);
	squareRatioSlider.addListener(this);

	sineRatioSlider.setSliderStyle(Slider::LinearHorizontal);
	sineRatioSlider.setRange(0, 10, 0.1);
	sineRatioSlider.setTextBoxStyle(Slider::NoTextBox, false, 20, 0);
	sineRatioSlider.setPopupDisplayEnabled(true, false, this);
	sineRatioSlider.setTextValueSuffix("");
	sineRatioSlider.setValue(5);
	addAndMakeVisible(&sineRatioSlider);
	sineRatioSlider.addListener(this);

	noiseRatioSlider.setSliderStyle(Slider::LinearHorizontal);
	noiseRatioSlider.setRange(0, 8, 0.1);
	noiseRatioSlider.setTextBoxStyle(Slider::NoTextBox, false, 20, 0);
	noiseRatioSlider.setPopupDisplayEnabled(true, false, this);
	noiseRatioSlider.setTextValueSuffix("");
	noiseRatioSlider.setValue(2.5);
	addAndMakeVisible(&noiseRatioSlider);
	noiseRatioSlider.addListener(this);

	sine2RatioSlider.setSliderStyle(Slider::LinearHorizontal);
	sine2RatioSlider.setRange(1, 40, 0.1);
	sine2RatioSlider.setTextBoxStyle(Slider::NoTextBox, false, 20, 0);
	sine2RatioSlider.setPopupDisplayEnabled(true, false, this);
	sine2RatioSlider.setTextValueSuffix("");
	sine2RatioSlider.setValue(1);
	addAndMakeVisible(&sine2RatioSlider);
	sine2RatioSlider.addListener(this);

	sineParameterSlider.setSliderStyle(Slider::LinearVertical);
	sineParameterSlider.setRange(1, 40, 0.1);
	sineParameterSlider.setTextBoxStyle(Slider::NoTextBox, false, 20, 0);
	sineParameterSlider.setPopupDisplayEnabled(true, false, this);
	sineParameterSlider.setTextValueSuffix("");
	sineParameterSlider.setValue(1);
	addAndMakeVisible(&sineParameterSlider);
	sineParameterSlider.addListener(this);

	squareParameterSlider.setSliderStyle(Slider::LinearVertical);
	squareParameterSlider.setRange(1, 40, 0.1);
	squareParameterSlider.setTextBoxStyle(Slider::NoTextBox, false, 20, 0);
	squareParameterSlider.setPopupDisplayEnabled(true, false, this);
	squareParameterSlider.setTextValueSuffix("");
	squareParameterSlider.setValue(1);
	addAndMakeVisible(&squareParameterSlider);
	squareParameterSlider.addListener(this);

	sine2ParameterSlider.setSliderStyle(Slider::LinearVertical);
	sine2ParameterSlider.setRange(1, 40, 0.1);
	sine2ParameterSlider.setTextBoxStyle(Slider::NoTextBox, false, 20, 0);
	sine2ParameterSlider.setPopupDisplayEnabled(true, false, this);
	sine2ParameterSlider.setTextValueSuffix("");
	sine2ParameterSlider.setValue(1);
	addAndMakeVisible(&sine2ParameterSlider);
	sine2ParameterSlider.addListener(this);


	addAndMakeVisible(sineL);
	sineL.setText("sin(x) * cos(x) - sin(x * N)", dontSendNotification);
	sineL.addListener(this);
	//sineL.attachToComponent(&sineRatioSlider, false);

	addAndMakeVisible(sine2L);
	sine2L.setText("sin(x) * cos(x) - sin(x * M)", dontSendNotification);
	sine2L.addListener(this);
	
	addAndMakeVisible(squareL);
	squareL.setText("square(sin(x) - cos(x * L) / sin(x))", dontSendNotification);
	squareL.addListener(this);
	addAndMakeVisible(noiseL);
	noiseL.setText("noise", dontSendNotification);
	noiseL.addListener(this);
	addAndMakeVisible(sineParamL);
	sineParamL.setText("N", dontSendNotification);
	sineParamL.addListener(this);
	addAndMakeVisible(sine2ParamL);
	sine2ParamL.setText("M", dontSendNotification);
	sine2ParamL.addListener(this);
	addAndMakeVisible(squareParamL);
	squareParamL.setText("L", dontSendNotification);
	squareParamL.addListener(this);

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
	/*sineRatioSlider.setBounds(210, 10, 30, 150);
	sine2RatioSlider.setBounds(410, 10, 30, 150);
	squareRatioSlider.setBounds(110, 10, 30, 150);
	noiseRatioSlider.setBounds(310, 10, 30, 150);
	sineParameterSlider.setBounds(110, 180, 150, 30);
	sine2ParameterSlider.setBounds(110, 380, 150, 30);
	squareParameterSlider.setBounds(110, 280, 150, 30);*/

	sineRatioSlider.setBounds(50, 50, 200, 20);
	sine2RatioSlider.setBounds(50, 120, 200, 20);
	squareRatioSlider.setBounds(50, 190, 200, 20);
	noiseRatioSlider.setBounds(50, 260, 200, 20);
	sineParameterSlider.setBounds(300, 45, 20, 180);
	sine2ParameterSlider.setBounds(360, 45, 20, 180);
	squareParameterSlider.setBounds(420, 45, 20, 180);
	
	sineL.setBounds(55, 71, 200, 20);
	sine2L.setBounds(55, 140, 200, 20);
	squareL.setBounds(50, 65+ 140, 250, 20);
	noiseL.setBounds(130, 65+ 210, 200, 20);
	sineParamL.setBounds(300, 250, 20, 20);
	sine2ParamL.setBounds(360, 250, 20, 20);
	squareParamL.setBounds(420, 250, 20, 20);

	//processor.synthAudioSource.releaseTime = releaseSlider.getValue();


     /*This is generally where you'll want to lay out the positions of any
     subcomponents in your editor..*/
}

void GeneratorWt1AudioProcessorEditor::sliderValueChanged(Slider* slider)
{
	//processor.synthAudioSource.signalTable.applyGain(gainSlider.getValue());
	processor.synthAudioSource.sineRatio = sineRatioSlider.getValue();
	processor.synthAudioSource.sine2Ratio = sine2RatioSlider.getValue();
	processor.synthAudioSource.squareRatio = squareRatioSlider.getValue();
	processor.synthAudioSource.noiseRatio = noiseRatioSlider.getValue();
	processor.synthAudioSource.sineParameter = sineParameterSlider.getValue();
	processor.synthAudioSource.sine2Parameter = sine2ParameterSlider.getValue();
	processor.synthAudioSource.squareParameter = squareParameterSlider.getValue();
	
	//SineWaveVoice::a 
}
