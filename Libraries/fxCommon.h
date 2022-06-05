#pragma once

// --- constants & enumerations
//
// ---  by placing outside the class declaration, these will also be avaialble to anything
//      that #includes this file (sometimes needed)
const double kSmallestPositiveFloatValue = 1.175494351e-38;         /* min positive value */
const double kSmallestNegativeFloatValue = -1.175494351e-38;         /* min negative value */
const double kSqrtTwo = pow(2.0, 0.5);
const double kMinFilterFrequency = 20.0;
const double kMaxFilterFrequency = 20480.0; // 10 octaves above 20 Hz
const double ARC4RANDOMMAX = 4294967295.0;  // (2^32 - 1)

#define NEGATIVE       0
#define POSITIVE       1

/**
@checkFloatUnderflow
\ingroup FX-Functions

@brief Perform underflow check; returns true if we did underflow (user may not care)

\param value - the value to check for underflow
\return true if overflowed, false otherwise
*/
inline bool checkFloatUnderflow(double& value)
{
	bool retValue = false;
	if (value > 0.0 && value < kSmallestPositiveFloatValue)
	{
		value = 0;
		retValue = true;
	}
	else if (value < 0.0 && value > kSmallestNegativeFloatValue)
	{
		value = 0;
		retValue = true;
	}
	return retValue;
}
