#pragma once

#include <math.h>

enum class gainShaperType { kCompressor, kDownwardExpander };

/**
\struct GainShaperParameters
\ingroup FX-Objects
\brief
Custom parameter structure for the GainShaper object. Ths struct includes all information needed from GUI controls.

\author Will Pirkle http://www.willpirkle.com
\remark This object is included in Designing Audio Effects Plugins in C++ 2nd Ed. by Will Pirkle
\version Revision : 1.0
\date Date : 2018 / 09 / 7
*/
struct GainShaperParameters
{
	GainShaperParameters() {}
	/** all FXObjects parameter objects require overloaded= operator so remember to add new entries if you add new variables. */
	GainShaperParameters& operator=(const GainShaperParameters& params)
	{
		if (this == &params)
			return *this;

		ratio = params.ratio;
		threshold_dB = params.threshold_dB;
		kneeWidth_dB = params.kneeWidth_dB;
		hardLimitGate = params.hardLimitGate;
		softKnee = params.softKnee;
		calculation = params.calculation;
		outputGain_dB = params.outputGain_dB;
		// --- NOTE: do not set outbound variables??
		gainReduction = params.gainReduction;
		gainReduction_dB = params.gainReduction_dB;
		return *this;
	}

	// --- individual parameters
	double ratio = 50.0;				///< processor I/O gain ratio
	double threshold_dB = -10.0;		///< threshold in dB
	double kneeWidth_dB = 10.0;			///< knee width in dB for soft-knee operation
	bool hardLimitGate = false;			///< threshold in dB
	bool softKnee = true;				///< soft knee flag
	gainShaperType calculation = gainShaperType::kCompressor; ///< processor calculation type
	double outputGain_dB = 0.0;			///< make up gain

	// --- outbound values, for owner to use gain-reduction metering
	double gainReduction = 1.0;			///< output value for gain reduction that occurred
	double gainReduction_dB = 0.0;		///< output value for gain reduction that occurred in dB
};

/**
\class GainComputer
\ingroup FX-Objects
\brief
The GainComputer object implements a simple Gain Computing block.
It applies the gain or attenuation feed into the sidechain to the audio input
to produce the audio output

Audio I/O:
- Processes mono input to mono output.

Control I/F:
- Use DynamicsProcessorParameters structure to get/set object params.

\author Daniel Sacco
\version Revision : 1.0
\date Date : 2022 / 04 / 26
*/
class GainShaper : public IAudioSignalProcessor
{
public:
	GainShaper() {}	/* C-TOR */
	~GainShaper() {}	/* D-TOR */

public:

	/** reset members to initialized state */
	virtual bool reset(double _sampleRate) {
		return true;
	}

	/** return false: this object only processes samples */
	virtual bool canProcessAudioFrame() { return false; }

	/**
	* \brief apply gain to the audio
	\param xn input
	\return the processed sample
	*/
	virtual double processAudioSample(double xn);

	/** get parameters: note use of custom structure for passing param data */
	/**
	\return DynamicsProcessorParameters custom data structure
	*/
	GainShaperParameters getParameters() { return parameters; }

	/** set parameters: note use of custom structure for passing param data */
	/**
	\param GainModelerParameters custom data structure
	*/
	void setParameters(const GainShaperParameters& _parameters);


	virtual double processAuxInputAudioSample(double xn)
	{
		currentGain = gainInDB ? pow(10.0, xn / 20.0) : xn;
		return xn;
	}


private:
	double currentGain = 0.0;
	bool gainInDB = true;

protected:
	GainShaperParameters parameters; ///< object parameters
	double makeupGain = 1.0;

};