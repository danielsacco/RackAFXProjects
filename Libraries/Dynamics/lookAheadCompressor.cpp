#include "lookAheadCompressor.h"


bool LookAheadCompressor::reset(double _sampleRate)
{
	DynProcessor::reset(_sampleRate);

	delayLine.reset(_sampleRate);
	// TODO Buffer length in msecs must be configurable !!!!
	delayLine.createDelayBuffer(_sampleRate, 200);

	return true;
}

void LookAheadCompressor::setParameters(const DynProcessorParameters& _parameters)
{
	DynProcessor::setParameters(_parameters);

	SimpleDelayParameters delayParams = delayLine.getParameters();
	delayParams.delayTime_mSec = parameters.attackTime_mSec;
	delayLine.setParameters(delayParams);

}

double LookAheadCompressor::processAudioSample(double xn)
{
	// --- if using the sidechain, process the aux input
	if (parameters.enableSidechain)
	{
		detectorEnvelope_dB = detector.processAudioSample(sidechainInputSample);
	}

	detectorEnvelope_dB = detector.processAudioSample(xn);
	
	// --- compute gain
	double computedGain = gainShaper.processAudioSample(detectorEnvelope_dB);

	// Always feed the delay line so when we need to switch to look ahead, we already have the previous samples
	double delayedSample = delayLine.processAudioSample(xn);

	// should we use the current sample or a delayed one ?
	double sampleToProcess = delayedSample;

	// --- do DCA + makeup gain
	double yn = sampleToProcess * computedGain * makeupGain;

	return yn;
}

