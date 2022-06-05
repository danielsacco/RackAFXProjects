#include "dynamicsProcessor.h"


bool DynProcessor::reset(double _sampleRate)
{
	sidechainInputSample = 0.0;
	detector.reset(_sampleRate);

	AudioDetectorParameters detectorParams = detector.getParameters();
	detectorParams.clampToUnityMax = false;
	detectorParams.detect_dB = true;
	detector.setParameters(detectorParams);

	//GainShaperParameters gainGainShaperParams = gainShaper.getParameters();
	gainShaper.reset(_sampleRate);

	return true;
}

void DynProcessor::setParameters(const DynProcessorParameters& _params)
{
	parameters = _params;

	AudioDetectorParameters detectorParams = detector.getParameters();
	detectorParams.attackTime_mSec = _params.attackTime_mSec;
	detectorParams.clampToUnityMax = _params.clampToUnityMax;
	detectorParams.detectMode = _params.detectMode;
	//detectorParams.detect_dB = _params.detect_dB;
	detectorParams.releaseTime_mSec = _params.releaseTime_mSec;
	detector.setParameters(detectorParams);

	GainShaperParameters gainShaperParams = gainShaper.getParameters();
	gainShaperParams.calculation = _params.calculation;
	//gainShaperParams.gainReduction = _params.gainReduction;
	//gainShaperParams.gainReduction_dB = _params.gainReduction_dB;
	gainShaperParams.hardLimitGate = _params.hardLimitGate;
	gainShaperParams.kneeWidth_dB = _params.kneeWidth_dB;
	gainShaperParams.outputGain_dB = _params.outputGain_dB;
	gainShaperParams.ratio = _params.ratio;
	gainShaperParams.softKnee = _params.softKnee;
	gainShaperParams.threshold_dB = _params.threshold_dB;
	gainShaper.setParameters(gainShaperParams);
}

