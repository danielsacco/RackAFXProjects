#pragma once

#include <math.h>
#include "../Libraries/audioDetector.h"
#include "../Libraries/gainShaper.h"

/**
\enum dynamicsProcessorType
\ingroup Constants-Enums
\brief
Use this strongly typed enum to set the dynamics processor type.

- enum class dynamicsProcessorType { kCompressor, kDownwardExpander };
- limiting is the extreme version of kCompressor
- gating is the extreme version of kDownwardExpander

\author Will Pirkle http://www.willpirkle.com
\remark This object is included in Designing Audio Effects Plugins in C++ 2nd Ed. by Will Pirkle
\version Revision : 1.0
\date Date : 2018 / 09 / 7
*/
enum class dynProcessorType { kCompressor, kDownwardExpander };

enum class compressorTopology { kFeed_Forward, kFeed_Back, kLook_Ahead };


struct DynProcessorParameters : GainShaperParameters, AudioDetectorParameters
{
	bool enableSidechain = false;
};

/**
\class DynProcessor
\ingroup FX-Objects
\brief
The DynProcessor object implements a dynamics processor suite: compressor, limiter, downward expander, gate.

Audio I/O:
- Processes mono input to mono output.

Control I/F:
- Use DynProcessorParameters structure to get/set object params.

\author Will Pirkle http://www.willpirkle.com
\remark This object is included in Designing Audio Effects Plugins in C++ 2nd Ed. by Will Pirkle
\version Revision : 1.0
\date Date : 2018 / 09 / 7
*/
class DynProcessor : public IAudioSignalProcessor
{
public:
	DynProcessor() {}	/* C-TOR */
	~DynProcessor() {}	/* D-TOR */

public:
	/** reset members to initialized state */
	virtual bool reset(double _sampleRate);

	/** return false: this object only processes samples */
	virtual bool canProcessAudioFrame() { return false; }

	/** enable sidechain input */
	virtual void enableAuxInput(bool enableAuxInput) { 
		parameters.enableSidechain = enableAuxInput; 
	}

	/** process the sidechain by saving the value for the upcoming processAudioSample() call */
	virtual double processAuxInputAudioSample(double xn)
	{
		sidechainInputSample = xn;
		return sidechainInputSample;
	}

	double getDetectorEnvelope()
	{
		return detectorEnvelope_dB;
	}

	double getGainReduction()
	{
		return gainShaper.getParameters().gainReduction;
	}

	/** get parameters: note use of custom structure for passing param data */
	/**
	\return DynamicsProcessorParameters custom data structure
	*/
	DynProcessorParameters getParameters() { return parameters; }

	/** set parameters: note use of custom structure for passing param data */
	/**
	\param DynamicsProcessorParameters custom data structure
	*/
	virtual void setParameters(const DynProcessorParameters& _parameters);
	//virtual void setParameters(const AudioDetectorParameters& _detectorParams, const GainShaperParameters& _gainShaperParams);

	/** process audio using feed-forward dynamics processor flowchart */
	/*
		1. detect input signal
		2. calculate gain
		3. apply to input sample
	*/
	/**
	\param xn input
	\return the processed sample
	*/
	virtual double processAudioSample(double xn) = 0;

	//AudioDetectorParameters getDetectorParameters()
	//{
	//	return detector.getParameters();
	//}
	//	
	//GainShaperParameters getGainShaperParameters()
	//{
	//	return gainShaper.getParameters();
	//}

protected:
	DynProcessorParameters parameters; ///< object parameters
	AudioDetector detector; ///< the sidechain audio detector
	GainShaper gainShaper;

	// --- storage for sidechain audio input (mono only)
	double sidechainInputSample = 0.0; ///< storage for sidechain sample

	/** compute (and save) the current gain value based on detected input (dB) */
	//inline double computeGain(double detect_dB);

	double makeupGain = 1.0;

	// Holds the last value calculated by the audio detector
	double detectorEnvelope_dB = 0.0;

};