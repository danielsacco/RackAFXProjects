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


/**
@doLinearInterpolation
\ingroup FX-Functions

@brief performs linear interpolation of x distance between two (x,y) points;
returns interpolated value

\param x1 - the x coordinate of the first point
\param x2 - the x coordinate of the second point
\param y1 - the y coordinate of the first point
\param y2 - the 2 coordinate of the second point
\param x - the interpolation location
\return the interpolated value or y1 if the x coordinates are unusable
*/
inline double doLinearInterpolation(double x1, double x2, double y1, double y2, double x)
{
	double denom = x2 - x1;
	if (denom == 0)
		return y1; // --- should not ever happen

	// --- calculate decimal position of x
	double dx = (x - x1) / (x2 - x1);

	// --- use weighted sum method of interpolating
	return dx * y2 + (1 - dx) * y1;
}

/**
@doLinearInterpolation
\ingroup FX-Functions

@brief performs linear interpolation of fractional x distance between two adjacent (x,y) points;
returns interpolated value

\param y1 - the y coordinate of the first point
\param y2 - the 2 coordinate of the second point
\param x - the interpolation location as a fractional distance between x1 and x2 (which are not needed)
\return the interpolated value or y2 if the interpolation is outside the x interval
*/
inline double doLinearInterpolation(double y1, double y2, double fractional_X)
{
	// --- check invalid condition
	if (fractional_X >= 1.0) return y2;

	// --- use weighted sum method of interpolating
	return fractional_X * y2 + (1.0 - fractional_X) * y1;
}


/**
@doLagrangeInterpolation
\ingroup FX-Functions

@brief implements n-order Lagrange Interpolation

\param x - Pointer to an array containing the x-coordinates of the input values
\param y - Pointer to an array containing the y-coordinates of the input values
\param n - the order of the interpolator, this is also the length of the x,y input arrays
\param xbar - The x-coorinates whose y-value we want to interpolate
\return the interpolated value
*/
inline double doLagrangeInterpolation(double* x, double* y, int n, double xbar)
{
	int i, j;
	double fx = 0.0;
	double l = 1.0;
	for (i = 0; i < n; i++)
	{
		l = 1.0;
		for (j = 0; j < n; j++)
		{
			if (j != i)
				l *= (xbar - x[j]) / (x[i] - x[j]);
		}
		fx += l * y[i];
	}
	return (fx);
}
