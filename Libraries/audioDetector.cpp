#include "audioDetector.h"

/**
\brief sets the new attack time and re-calculates the time constant

\param attack_in_ms the new attack timme
\param forceCalc flag to force a re-calculation of time constant even if values have not changed.
*/
void AudioDetector::setAttackTime(double attack_in_ms, bool forceCalc)
{
	if (!forceCalc && audioDetectorParameters.attackTime_mSec == attack_in_ms)
		return;

	audioDetectorParameters.attackTime_mSec = attack_in_ms;
	attackTime = exp(TLD_AUDIO_ENVELOPE_ANALOG_TC / (attack_in_ms * sampleRate * 0.001));
}


/**
\brief sets the new release time and re-calculates the time constant

\param release_in_ms the new relase timme
\param forceCalc flag to force a re-calculation of time constant even if values have not changed.
*/
void AudioDetector::setReleaseTime(double release_in_ms, bool forceCalc)
{
	if (!forceCalc && audioDetectorParameters.releaseTime_mSec == release_in_ms)
		return;

	audioDetectorParameters.releaseTime_mSec = release_in_ms;
	releaseTime = exp(TLD_AUDIO_ENVELOPE_ANALOG_TC / (release_in_ms * sampleRate * 0.001));
}
