/*
  ==============================================================================

	SynthAudioSource.h
	Created: 2 Dec 2018 2:47:15pm
	Author:  korowiow

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
//#include "PluginEditor.h"

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
		
		/*auto rand = Random::getSystemRandom().nextFloat();
		currentSample += (0.001*rand);*/
		/*if (currentSample >= 0.5)
			currentSample = 1;
		else currentSample = 0;*/
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
		//level = 0.;
		tailOff = 0.0;
		setFrequency(MidiMessage::getMidiNoteInHertz(midiNoteNumber), getSampleRate()); // ***
		noteVelocity = velocity;


		ADSRphase = 0;
		/*while (true)
		{
			if (!(level >= velocity))
			{
				level *= 1.1;
			}
		}*/
		/*while (level >= velocity)
		{
			level *= 1.1;
		}
*/
		//wyliczenie liniowej funkcji  - nachylenia - czyli  ADSRvalueDelta;

		// *** angleDelta = cyclesPerSample * 2.0 * MathConstants<double>::pi;

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
			tableDelta = 0.0f; // *** angleDelta = 0.0;
		}
	}
	void pitchWheelMoved(int) override {}
	void controllerMoved(int, int) override {}
	void renderNextBlock(AudioSampleBuffer& outputBuffer, int startSample, int numSamples) override
	{
		//createWavetable();
		attackTime = 0.5 * getSampleRate();

		if (tableDelta != 0.0) // ***	//je¿eli jest naciœniêty klawisz
		{
			if (tailOff > 0.0)	//sprawdzac faze adsr, jezeli jest 0 to znaczy ze nie gramy
			{
				//tailOff = 0.1;

				while (--numSamples >= 0)
				{

					auto currentSample = getNextSample() * level * tailOff;	//zamiast tailoff adsrvalue

					//attackTime += currentSample;

					for (auto i = outputBuffer.getNumChannels(); --i >= 0;)
						outputBuffer.addSample(i, startSample, currentSample);

					//if (ADSRphase == 0)
					


					++startSample;
					
					if (ADSRphase == 3)
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
	/*dsp::ProcessSpec processSpec;
	dsp::IIR::Coefficients<float>lowPassCoefficients;*/

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
		//createWavetable(); /*/ 

		//a = IIRCoefficients::makeHighPass(sampleRate, 1000, 1.0);
		//a.makeLowPass(sampleRate, 1000);
		////filter.setCoefficients(a);
		//processSpec.sampleRate = sampleRate;
		//processSpec.numChannels = 2;
		//processSpec.maximumBlockSize = tableSize;
		//lowPassFilter.prepare(processSpec);
		//lowPassFilter.reset();
		//lowPassCoefficients.process(context);
		//dsp::IIR::Filter<float>::process(context);
		//lowPassCoefficients.process
		//dsp::IIR::Filter filter = dsp::IIR::Filter(lowPassCoefficients);
		//IIRCoefficients coeffs;
		//coeffs.makeLowPass(sampleRate, tableSize / sampleRate);
		//dsp::IIR::Filter<float>filter(coeffs);
		
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

		/*//int sampleRate = 44100;
		//
		//lowPassCoefficients.makeLowPass(sampleRate, tableSize / sampleRate, 2.0f);
		//dsp::IIR::Filter<float>::CoefficientsPtr lowPassCoefficientsPtr = lowPassCoefficients;

		//
		//lowPassFilter(dsp::IIR::Coefficients<float>)
		//dsp::IIR::Filter<float> filter = lowPassCoefficients;

		//dsp::AudioBlock<float> block = *bufferToFill.buffer;
		//const dsp::ProcessContextReplacing<float> context(block);
		////lowPassCoefficients.process(context);
		//filter.process(context);

		/**lowPassFilter.state = *dsp::IIR::Coefficients<float>::makeLowPass(sampleRate, 300, 0.5);
		for (int i = 0; i < static_cast<int>(bufferToFill.buffer->getNumSamples); ++i)
			filter.processSamples(bufferToFill.buffer->getWritePointer(i), bufferToFill.buffer->getNumSamples());*/

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
	
	/*//dsp::ProcessorDuplicator < dsp::IIR::Filter<float>, dsp::IIR::Coefficients<float>> lowPassFilter;
	juce::IIRFilter filter;
	juce::IIRCoefficients a;*/


	float noiseRatio;
	//float sineSquaredRatio;
	float sineRatio;
	float squareRatio;

	float square(float sample)
	{
		if (sample >= 0) sample = 0.9;
		else if (sample < 0) sample = -0.9;
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
			//sineNoiseRatio = 30;
			auto sample = std::sin(currentAngle);
			
			//float sineSquared = sample * sample;
			//sample *= sample;
			//sample = static_cast<float>(sample);
			//sample += sample * sineRatio;
			auto rand = Random::getSystemRandom().nextFloat();
			//float noise_ = noise(sample);
			/*sample *= sineRatio;
			sample += (rand * noiseRatio);
			sample += (square(sample) * squareRatio);*/

			float sineSample = sample * sineRatio;
			float noiseSample = rand * noiseRatio;
			float squareSample = square(sample) * squareRatio;

			float finalSample = sineSample + noiseSample + squareSample;
			//finalSample /= 3;
			finalSample /= (sineRatio + noiseRatio + squareRatio);
			//sample = sample / (sineRatio + noiseRatio + squareRatio);

			samples[i] = (float)finalSample;
			currentAngle += angleDelta;


			/*samples[i] = (float)sample;
			currentAngle += angleDelta;*/
		}
	}
	// ***
};
