#include "feedForwardDynamicsProcessor.h"

double FeedForwardDynamicsProcessor::processAudioSample(double xn)
{
	// --- if using the sidechain, process the aux input
	detectorEnvelope_dB = detector.processAudioSample(
		parameters.enableSidechain ? sidechainInputSample : xn);

	// --- compute gain
	double computedGain = gainShaper.processAudioSample(detectorEnvelope_dB);

	// --- do DCA + makeup gain
	double yn = xn * computedGain * makeupGain;


	return yn;
}

