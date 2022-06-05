#pragma once

#include "iAudioSignalProcessor.h"
#include "constants.h"
#include <math.h>
#include "fxCommon.h"

/**
\struct AudioDetectorParameters
\ingroup FX-Objects
\brief
Custom parameter structure for the AudioDetector object. NOTE: this object uses constant defintions:

- const unsigned int TLD_AUDIO_DETECT_MODE_PEAK = 0;
- const unsigned int TLD_AUDIO_DETECT_MODE_MS = 1;
- const unsigned int TLD_AUDIO_DETECT_MODE_RMS = 2;
- const double TLD_AUDIO_ENVELOPE_ANALOG_TC = -0.99967234081320612357829304641019; // ln(36.7%)

\author Will Pirkle http://www.willpirkle.com
\remark This object is included in Designing Audio Effects Plugins in C++ 2nd Ed. by Will Pirkle
\version Revision : 1.0
\date Date : 2018 / 09 / 7
*/
struct AudioDetectorParameters
{
	AudioDetectorParameters() {}
	/** all FXObjects parameter objects require overloaded= operator so remember to add new entries if you add new variables. */
	AudioDetectorParameters& operator=(const AudioDetectorParameters& params)	// need this override for collections to work
	{
		if (this == &params)
			return *this;
		attackTime_mSec = params.attackTime_mSec;
		releaseTime_mSec = params.releaseTime_mSec;
		detectMode = params.detectMode;
		detect_dB = params.detect_dB;
		clampToUnityMax = params.clampToUnityMax;
		return *this;
	}

	// --- individual parameters
	double attackTime_mSec = 0.0; ///< attack time in milliseconds
	double releaseTime_mSec = 0.0;///< release time in milliseconds
	unsigned int  detectMode = 0;///< detect mode, see TLD_ constants above
	bool detect_dB = false;	///< detect in dB  DEFAULT  = false (linear NOT log)
	bool clampToUnityMax = true;///< clamp output to 1.0 (set false for true log detectors)
};


class AudioDetector : public IAudioSignalProcessor
{
public:
	AudioDetector() {}	/* C-TOR */
	~AudioDetector() {}	/* D-TOR */

public:
	/** set sample rate dependent time constants and clear last envelope output value */
	virtual bool reset(double _sampleRate)
	{
		setSampleRate(_sampleRate);
		lastEnvelope = 0.0;
		return true;
	}

	/** return false: this object only processes samples */
	virtual bool canProcessAudioFrame() { return false; }

	// --- process audio: detect the log envelope and return it in dB
	/**
	\param xn input
	\return the processed sample
	*/
	virtual double processAudioSample(double xn)
	{
		// --- all modes do Full Wave Rectification
		double input = fabs(xn);

		// --- square it for MS and RMS
		if (audioDetectorParameters.detectMode == TLD_AUDIO_DETECT_MODE_MS ||
			audioDetectorParameters.detectMode == TLD_AUDIO_DETECT_MODE_RMS)
			input *= input;

		// --- to store current
		double currEnvelope = 0.0;

		// --- do the detection with attack or release applied
		if (input > lastEnvelope)
			currEnvelope = attackTime * (lastEnvelope - input) + input;
		else
			currEnvelope = releaseTime * (lastEnvelope - input) + input;

		// --- we are recursive so need to check underflow
		checkFloatUnderflow(currEnvelope);

		// --- bound them; can happen when using pre-detector gains of more than 1.0
		if (audioDetectorParameters.clampToUnityMax)
			currEnvelope = fmin(currEnvelope, 1.0);

		// --- can not be (-)
		currEnvelope = fmax(currEnvelope, 0.0);

		// --- store envelope prior to sqrt for RMS version
		lastEnvelope = currEnvelope;

		// --- if RMS, do the SQRT
		if (audioDetectorParameters.detectMode == TLD_AUDIO_DETECT_MODE_RMS)
			currEnvelope = pow(currEnvelope, 0.5);

		// --- if not dB, we are done
		if (!audioDetectorParameters.detect_dB)
			return currEnvelope;

		// --- setup for log( )
		if (currEnvelope <= 0)
		{
			return -96.0;
		}

		// --- true log output in dB, can go above 0dBFS!
		return 20.0*log10(currEnvelope);
	}

	/** get parameters: note use of custom structure for passing param data */
	/**
	\return AudioDetectorParameters custom data structure
	*/
	AudioDetectorParameters getParameters()
	{
		return audioDetectorParameters;
	}

	/** set parameters: note use of custom structure for passing param data */
	/**
	\param AudioDetectorParameters custom data structure
	*/
	void setParameters(const AudioDetectorParameters& parameters)
	{
		audioDetectorParameters = parameters;

		// --- update structure
		setAttackTime(audioDetectorParameters.attackTime_mSec, true);
		setReleaseTime(audioDetectorParameters.releaseTime_mSec, true);

	}

	/** set sample rate - our time constants depend on it */
	virtual void setSampleRate(double _sampleRate)
	{
		if (sampleRate == _sampleRate)
			return;

		sampleRate = _sampleRate;

		// --- recalculate RC time-constants
		setAttackTime(audioDetectorParameters.attackTime_mSec, true);
		setReleaseTime(audioDetectorParameters.releaseTime_mSec, true);
	}

protected:
	AudioDetectorParameters audioDetectorParameters; ///< parameters for object
	double attackTime = 0.0;	///< attack time coefficient
	double releaseTime = 0.0;	///< release time coefficient
	double sampleRate = 44100;	///< stored sample rate
	double lastEnvelope = 0.0;	///< output register

	/** set our internal atack time coefficients based on times and sample rate */
	void setAttackTime(double attack_in_ms, bool forceCalc = false);

	/** set our internal release time coefficients based on times and sample rate */
	void setReleaseTime(double release_in_ms, bool forceCalc = false);
};
