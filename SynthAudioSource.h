/*
  ==============================================================================

	SynthAudioSource.h
	Created: 2 Dec 2018 2:47:15pm
	Author:  korowiow + Adam Korytowski

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
struct SineWaveSound : public SynthesiserSound
{
	SineWaveSound() {}
	bool appliesToNote(int) override { return true; }
	bool appliesToChannel(int) override { return true; }
};

//==============================================================================
struct SineWaveVoice : public SynthesiserVoice
{
	// ***
	int a = 0;
	SineWaveVoice(const AudioSampleBuffer& wavetableToUse, unsigned int voices) : wavetable(wavetableToUse)
	{
		globalLevel = 0.5 / (float)voices;
		jassert(wavetable.getNumChannels() == 1);
	}
	void setFrequency(float frequency, float sampleRate)
	{
		auto tableSizeOverSampleRate = wavetable.getNumSamples() / sampleRate;
		tableDelta = frequency * tableSizeOverSampleRate;
	}
	forcedinline float getNextSample() noexcept
	{
		auto tableSize = wavetable.getNumSamples();
		auto index0 = (unsigned int)currentIndex;
		auto index1 = index0 == (tableSize - 1) ? (unsigned int)0 : index0 + 1;
		auto frac = currentIndex - (float)index0;
		auto* table = wavetable.getReadPointer(0);
		auto value0 = table[index0];
		auto value1 = table[index1];
		auto currentSample = value0 + frac * (value1 - value0);
		if ((currentIndex += tableDelta) > tableSize)
			currentIndex -= tableSize;
		return currentSample;
	}
	// ***
	bool canPlaySound(SynthesiserSound* sound) override
	{
		return dynamic_cast<SineWaveSound*> (sound) != nullptr;
	}
	void startNote(int midiNoteNumber, float velocity,
		SynthesiserSound*, int /*currentPitchWheelPosition*/) override
	{
		currentIndex = 0.0; 
		level = velocity * globalLevel;
		//level = 0.;
		tailOff = 0.0;
		setFrequency(MidiMessage::getMidiNoteInHertz(midiNoteNumber), getSampleRate()); // ***
		noteVelocity = velocity;
		ADSRphase = 0;

	}
	void stopNote(float /*velocity*/, bool allowTailOff) override
	{
		ADSRphase = 3;
		if (allowTailOff)
		{
			if (tailOff == 0.0)

				tailOff = 1.0;
		}
		else
		{
			clearCurrentNote();
			tableDelta = 0.0f; 
		}
	}
	void pitchWheelMoved(int) override {}
	void controllerMoved(int, int) override {}
	void renderNextBlock(AudioSampleBuffer& outputBuffer, int startSample, int numSamples) override
	{
		attackTime = 0.5 * getSampleRate();

		if (tableDelta != 0.0) 
		{
			if (tailOff > 0.0)	
			{
				while (--numSamples >= 0)
				{

					auto currentSample = getNextSample() * level * tailOff;

					for (auto i = outputBuffer.getNumChannels(); --i >= 0;)
						outputBuffer.addSample(i, startSample, currentSample);

					++startSample;
					
					if (ADSRphase == 3)
						tailOff *= (1 - (1 / releaseTime));
					if (tailOff <= 0.005)
					{
						clearCurrentNote();
						tableDelta = 0.0f; // *** angleDelta = 0.0;
						break;
					}
				}
			}
			else
			{
				while (--numSamples >= 0)
				{
					auto currentSample = getNextSample() * level; // ***
					for (auto i = outputBuffer.getNumChannels(); --i >= 0;)
						outputBuffer.addSample(i, startSample, currentSample);
					++startSample;
				}
			}
		}
	}
	
private:
	float tailOff = 0.0; // ***
	const AudioSampleBuffer& wavetable;
	float currentIndex = 0.0f, tableDelta = 0.0f, level = 0.0f, globalLevel = 0.0f;
	float releaseTime = 10000.0f;
	int ADSRphase;
	float ADSRValue;
	float noteVelocity;
	float attackTime = 0.5 * getSampleRate();
	float ADSRvalueDelta;
};

//==============================================================================
class SynthAudioSource : public AudioSource/*, public GeneratorWt1AudioProcessorEditor*/
{
public:
	SynthAudioSource(MidiKeyboardState& keyState)
		: keyboardState(keyState)
	{
		for (auto i = 0; i < numVoices; ++i)
			synth.addVoice(new SineWaveVoice(signalTable, numVoices));
		synth.addSound(new SineWaveSound());
	}
	void setUsingSineWaveSound()
	{
		synth.clearSounds();
	}
	void prepareToPlay(int /*samplesPerBlockExpected*/, double sampleRate) override
	{
		synth.setCurrentPlaybackSampleRate(sampleRate);
	}
	void releaseResources() override {}
	void getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill) override
	{
		createWavetable();
		bufferToFill.clearActiveBufferRegion();
		MidiBuffer incomingMidi;
		midiCollector.removeNextBlockOfMessages(incomingMidi, bufferToFill.numSamples);
		keyboardState.processNextMidiBuffer(incomingMidi, bufferToFill.startSample,
			bufferToFill.numSamples, true);
		synth.renderNextBlock(*bufferToFill.buffer, incomingMidi,
			bufferToFill.startSample, bufferToFill.numSamples);
	}
	MidiMessageCollector* getMidiCollector()
	{
		return &midiCollector;
	}
	MidiMessageCollector midiCollector;
//private:
	MidiKeyboardState& keyboardState;
	Synthesiser synth;

	// ***
	int tableSize = 128;
	int numVoices = 4;
	AudioSampleBuffer signalTable;
	
	float sineRatio;
	float sine2Ratio;
	float squareRatio;
	float noiseRatio;

	float sineParameter, sine2Parameter, squareParameter;

	float square(float currentAngle)
	{
		float sample;
		float weirdSquare = std::sin(currentAngle) - (std::cos(squareParameter * currentAngle)) / std::sin(currentAngle);
		if (weirdSquare >= 0) sample = 0.9;
		else if (weirdSquare < 0) sample = -0.9;
		return sample;
	}

	float noise(float sample)
	{
		auto rand = Random::getSystemRandom().nextFloat();
		return sample * rand * noiseRatio;
	}

	void createWavetable()
	{	
		signalTable.setSize(1, tableSize);
		auto* samples = signalTable.getWritePointer(0);
		auto angleDelta =  MathConstants<float>::twoPi / (double)(tableSize - 1);
		auto currentAngle = 0.0;

		for (auto i = 0; i < tableSize; ++i)
		{
			auto sample = std::sin(currentAngle) * std::cos(currentAngle) - std::sin(currentAngle * sineParameter);
			auto sample2 = std::sin(currentAngle) * std::cos(currentAngle) - std::sin(currentAngle * sine2Parameter);

			auto rand = Random::getSystemRandom().nextFloat();

			float sineSample = sample * sineRatio;
			float noiseSample = rand * noiseRatio;
			float squareSample = square(currentAngle) * squareRatio;
			float logSample = sample2 * sine2Ratio;

			float finalSample = sineSample + noiseSample + squareSample + logSample;
			finalSample /= (1 + sineRatio + noiseRatio + squareRatio + sine2Ratio);

			samples[i] = (float)finalSample;
			currentAngle += angleDelta;

		}
	}
};
