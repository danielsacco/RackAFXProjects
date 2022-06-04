#include "gainShaper.h"


void GainShaper::setParameters(const GainShaperParameters& _parameters)
{
	// Calculate the makeupGain only when it changes
	if (_parameters.outputGain_dB != parameters.outputGain_dB)
		makeupGain = pow(10.0, _parameters.outputGain_dB / 20.0);

	parameters = _parameters;

}

double GainShaper::processAudioSample(double detect_dB)
{
	double output_dB = 0.0;

	// We may use this value several times
	double dB_OverThreshold = detect_dB - parameters.threshold_dB;

	if (parameters.calculation == gainShaperType::kCompressor)
	{
		// --- hard knee
		if (!parameters.softKnee)
		{
			// --- below threshold, unity
			if (detect_dB <= parameters.threshold_dB)
				output_dB = detect_dB;
			else// --- above threshold, compress
			{
				if (parameters.hardLimitGate) // is limiter?
					output_dB = parameters.threshold_dB;
				else
					output_dB = parameters.threshold_dB + dB_OverThreshold / parameters.ratio;
			}
		}
		else // --- calc gain with knee
		{
			// --- left side of knee, outside of width, unity gain zone
			if (2.0 * dB_OverThreshold < -parameters.kneeWidth_dB)
				output_dB = detect_dB;
			// --- else inside the knee,
			else if (2.0 * (fabs(dB_OverThreshold)) <= parameters.kneeWidth_dB)
			{
				if (parameters.hardLimitGate)	// --- is limiter?
					output_dB = detect_dB - pow((dB_OverThreshold + (parameters.kneeWidth_dB / 2.0)), 2.0) / (2.0 * parameters.kneeWidth_dB);
				else // --- 2nd order poly
					output_dB = detect_dB + (((1.0 / parameters.ratio) - 1.0) * pow((dB_OverThreshold + (parameters.kneeWidth_dB / 2.0)), 2.0)) / (2.0 * parameters.kneeWidth_dB);
			}
			// --- right of knee, compression zone
			else if (2.0 * dB_OverThreshold > parameters.kneeWidth_dB)
			{
				if (parameters.hardLimitGate) // --- is limiter?
					output_dB = parameters.threshold_dB;
				else
					output_dB = parameters.threshold_dB + dB_OverThreshold / parameters.ratio;
			}
		}
	}
	else if (parameters.calculation == gainShaperType::kDownwardExpander)
	{
		// --- hard knee
		// --- NOTE: soft knee is not technically possible with a gate because there
		//           is no "left side" of the knee
		if (!parameters.softKnee || parameters.hardLimitGate)
		{
			// --- above threshold, unity gain
			if (detect_dB >= parameters.threshold_dB)
				output_dB = detect_dB;
			else
			{
				if (parameters.hardLimitGate) // --- gate: -inf(dB)
					output_dB = -1.0e34;
				else
					output_dB = parameters.threshold_dB + dB_OverThreshold * parameters.ratio;
			}
		}
		else // --- calc gain with knee
		{
			// --- right side of knee, unity gain zone
			if (2.0 * dB_OverThreshold > parameters.kneeWidth_dB)
				output_dB = detect_dB;
			// --- in the knee
			else if (2.0 * (fabs(dB_OverThreshold)) > -parameters.kneeWidth_dB)
				output_dB = ((parameters.ratio - 1.0) * pow((dB_OverThreshold - (parameters.kneeWidth_dB / 2.0)), 2.0)) / (2.0 * parameters.kneeWidth_dB);
			// --- left side of knee, downward expander zone
			else if (2.0 * dB_OverThreshold <= -parameters.kneeWidth_dB)
				output_dB = parameters.threshold_dB + dB_OverThreshold * parameters.ratio;
		}
	}

	// --- convert gain; store values for user meters
	parameters.gainReduction_dB = output_dB - detect_dB;
	parameters.gainReduction = pow(10.0, (parameters.gainReduction_dB) / 20.0);

	// --- the current gain coefficient value
	return parameters.gainReduction;
}
