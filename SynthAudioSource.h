/*
  ==============================================================================

	SynthAudioSource.h
	Created: 2 Dec 2018 2:47:15pm
	Author:  korowiow

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
		currentIndex = 0.0; // *** currentAngle = 0.0;
		level = velocity * globalLevel;
		tailOff = 0.0;
		// *** auto cyclesPerSecond = MidiMessage::getMidiNoteInHertz(midiNoteNumber);
		// *** auto cyclesPerSample = cyclesPerSecond / getSampleRate();
		setFrequency(MidiMessage::getMidiNoteInHertz(midiNoteNumber), getSampleRate()); // ***

		// *** angleDelta = cyclesPerSample * 2.0 * MathConstants<double>::pi;

	}
	void stopNote(float /*velocity*/, bool allowTailOff) override
	{
		if (allowTailOff)
		{
			if (tailOff == 0.0)

				tailOff = 1.0;
		}
		else
		{
			clearCurrentNote();
			tableDelta = 0.0f; // *** angleDelta = 0.0;
			/*while (-- numSamples > 0)
			{

			}*/
		}
	}
	void pitchWheelMoved(int) override {}
	void controllerMoved(int, int) override {}
	void renderNextBlock(AudioSampleBuffer& outputBuffer, int startSample, int numSamples) override
	{
		float attackTime_Sample = 0.5 * getSampleRate();

		if (tableDelta != 0.0) // ***
		{
			if (tailOff > 0.0)
			{
				//tailOff = 0.1;

				while (--numSamples >= 0)
				{

					auto currentSample = getNextSample() * level * tailOff;

					attackTime_Sample += currentSample;

					for (auto i = outputBuffer.getNumChannels(); --i >= 0;)
						outputBuffer.addSample(i, startSample, currentSample);

					
					++startSample;
		
					tailOff *= (1 - (1 / releaseTime));
					//tailOff *= 0.999999;

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
					// *** auto currentSample = (float)(std::sin(currentAngle) * level);
					auto currentSample = getNextSample() * level; // ***
					for (auto i = outputBuffer.getNumChannels(); --i >= 0;)
						outputBuffer.addSample(i, startSample, currentSample);
					// *** currentAngle += angleDelta;
					++startSample;
				}
			}
		}
	}
	
private:
	// *** double currentAngle = 0.0, angleDelta = 0.0, level = 0.0,
	float tailOff = 0.0; // ***
	const AudioSampleBuffer& wavetable;
	float currentIndex = 0.0f, tableDelta = 0.0f, level = 0.0f, globalLevel = 0.0f;
	float releaseTime = 10000.0f;
};

//==============================================================================
class SynthAudioSource : public AudioSource
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
		createWavetable(); // ***
	}
	void releaseResources() override {}
	void getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill) override
	{
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
private:
	MidiKeyboardState& keyboardState;
	Synthesiser synth;

	// ***
	int tableSize = 128;
	int numVoices = 4;
	AudioSampleBuffer signalTable;
	void createWavetable()
	{
		signalTable.setSize(1, tableSize);
		auto* samples = signalTable.getWritePointer(0);
		auto angleDelta = MathConstants<double>::twoPi / (double)(tableSize - 1);
		auto currentAngle = 0.0;
		for (auto i = 0; i < tableSize; ++i)
		{
			auto sample = std::sin(currentAngle);
			samples[i] = (float)sample;
			currentAngle += angleDelta;
		}
	}
	// ***
};
