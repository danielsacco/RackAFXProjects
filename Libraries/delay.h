#pragma once

#include <memory>
#include "fxCommon.h"


/**
\class CircularBuffer
\ingroup FX-Objects
\brief
The CircularBuffer object implements a simple circular buffer. It uses a wrap mask to wrap the read or write index quickly.

\author Will Pirkle http://www.willpirkle.com
\remark This object is included in Designing Audio Effects Plugins in C++ 2nd Ed. by Will Pirkle
\version Revision : 1.0
\date Date : 2018 / 09 / 7
*/
/** A simple cyclic buffer: NOTE - this is NOT an IAudioSignalProcessor or IAudioSignalGenerator
	S must be a power of 2.
*/
template <typename T>
class CircularBuffer
{
public:
	CircularBuffer() {}		/* C-TOR */
	~CircularBuffer() {}	/* D-TOR */

							/** flush buffer by resetting all values to 0.0 */
	void flushBuffer() { memset(&buffer[0], 0, bufferLength * sizeof(T)); }

	/** Create a buffer based on a target maximum in SAMPLES
	//	   do NOT call from realtime audio thread; do this prior to any processing */
	void createCircularBuffer(unsigned int _bufferLength)
	{
		// --- find nearest power of 2 for buffer, and create
		createCircularBufferPowerOfTwo((unsigned int)(pow(2, ceil(log(_bufferLength) / log(2)))));
	}

	/** Create a buffer based on a target maximum in SAMPLESwhere the size is
		pre-calculated as a power of two */
	void createCircularBufferPowerOfTwo(unsigned int _bufferLengthPowerOfTwo)
	{
		// --- reset to top
		writeIndex = 0;

		// --- find nearest power of 2 for buffer, save it as bufferLength
		bufferLength = _bufferLengthPowerOfTwo;

		// --- save (bufferLength - 1) for use as wrapping mask
		wrapMask = bufferLength - 1;

		// --- create new buffer
		buffer.reset(new T[bufferLength]);

		// --- flush buffer
		flushBuffer();
	}

	/** write a value into the buffer; this overwrites the previous oldest value in the buffer */
	void writeBuffer(T input)
	{
		// --- write and increment index counter
		buffer[writeIndex++] = input;

		// --- wrap if index > bufferlength - 1
		writeIndex &= wrapMask;
	}

	/** read an arbitrary location that is delayInSamples old */
	T readBuffer(int delayInSamples)//, bool readBeforeWrite = true)
	{
		// --- subtract to make read index
		//     note: -1 here is because we read-before-write,
		//           so the *last* write location is what we use for the calculation
		int readIndex = (writeIndex - 1) - delayInSamples;

		// --- autowrap index
		readIndex &= wrapMask;

		// --- read it
		return buffer[readIndex];
	}

	/** read an arbitrary location that includes a fractional sample */
	T readBuffer(double delayInFractionalSamples)
	{
		// --- truncate delayInFractionalSamples and read the int part
		T y1 = readBuffer((int)delayInFractionalSamples);

		// --- if no interpolation, just return value
		if (!interpolate) return y1;

		// --- else do interpolation
		//
		// --- read the sample at n+1 (one sample OLDER)
		T y2 = readBuffer((int)delayInFractionalSamples + 1);

		// --- get fractional part
		double fraction = delayInFractionalSamples - (int)delayInFractionalSamples;

		// --- do the interpolation (you could try different types here)
		return doLinearInterpolation(y1, y2, fraction);
	}

	/** enable or disable interpolation; usually used for diagnostics or in algorithms that require strict integer samples times */
	void setInterpolate(bool b) { interpolate = b; }

private:
	std::unique_ptr<T[]> buffer = nullptr;	///< smart pointer will auto-delete
	unsigned int writeIndex = 0;		///> write index
	unsigned int bufferLength = 1024;	///< must be nearest power of 2
	unsigned int wrapMask = 1023;		///< must be (bufferLength - 1)
	bool interpolate = true;			///< interpolation (default is ON)
};


/**
\struct SimpleDelayParameters
\ingroup FX-Objects
\brief
Custom parameter structure for the SimpleDelay object. Used for reverb algorithms in book.

\author Will Pirkle http://www.willpirkle.com
\remark This object is included in Designing Audio Effects Plugins in C++ 2nd Ed. by Will Pirkle
\version Revision : 1.0
\date Date : 2018 / 09 / 7
*/
struct SimpleDelayParameters
{
	SimpleDelayParameters() {}
	/** all FXObjects parameter objects require overloaded= operator so remember to add new entries if you add new variables. */
	SimpleDelayParameters& operator=(const SimpleDelayParameters& params)	// need this override for collections to work
	{
		if (this == &params)
			return *this;

		delayTime_mSec = params.delayTime_mSec;
		interpolate = params.interpolate;
		delay_Samples = params.delay_Samples;
		return *this;
	}

	// --- individual parameters
	double delayTime_mSec = 0.0;	///< delay tine in mSec
	bool interpolate = false;		///< interpolation flag (diagnostics usually)

	// --- outbound parameters
	double delay_Samples = 0.0;		///< current delay in samples; other objects may need to access this information
};

/**
\class SimpleDelay
\ingroup FX-Objects
\brief
The SimpleDelay object implements a basic delay line without feedback.

Audio I/O:
- Processes mono input to mono output.

Control I/F:
- Use SimpleDelayParameters structure to get/set object params.

\author Will Pirkle http://www.willpirkle.com
\remark This object is included in Designing Audio Effects Plugins in C++ 2nd Ed. by Will Pirkle
\version Revision : 1.0
\date Date : 2018 / 09 / 7
*/
class SimpleDelay : public IAudioSignalProcessor
{
public:
	SimpleDelay(void) {}	/* C-TOR */
	~SimpleDelay(void) {}	/* D-TOR */

public:
	/** reset members to initialized state */
	virtual bool reset(double _sampleRate)
	{
		// --- if sample rate did not change
		if (sampleRate == _sampleRate)
		{
			// --- just flush buffer and return
			delayBuffer.flushBuffer();
			return true;
		}

		// --- create new buffer, will store sample rate and length(mSec)
		createDelayBuffer(_sampleRate, bufferLength_mSec);

		return true;
	}

	/** get parameters: note use of custom structure for passing param data */
	/**
	\return SimpleDelayParameters custom data structure
	*/
	SimpleDelayParameters getParameters()
	{
		return simpleDelayParameters;
	}

	/** set parameters: note use of custom structure for passing param data */
	/**
	\param SimpleDelayParameters custom data structure
	*/
	void setParameters(const SimpleDelayParameters& params)
	{
		simpleDelayParameters = params;
		simpleDelayParameters.delay_Samples = simpleDelayParameters.delayTime_mSec*(samplesPerMSec);
		delayBuffer.setInterpolate(simpleDelayParameters.interpolate);
	}

	/** process MONO audio delay */
	/**
	\param xn input
	\return the processed sample
	*/
	virtual double processAudioSample(double xn)
	{
		// --- read delay
		if (simpleDelayParameters.delay_Samples == 0)
			return xn;

		double yn = delayBuffer.readBuffer(simpleDelayParameters.delay_Samples);

		// --- write to delay buffer
		delayBuffer.writeBuffer(xn);

		// --- done
		return yn;
	}

	/** reset members to initialized state */
	virtual bool canProcessAudioFrame() { return false; }

	/** create a new delay buffer */
	void createDelayBuffer(double _sampleRate, double _bufferLength_mSec)
	{
		// --- store for math
		bufferLength_mSec = _bufferLength_mSec;
		sampleRate = _sampleRate;
		samplesPerMSec = sampleRate / 1000.0;

		// --- total buffer length including fractional part
		bufferLength = (unsigned int)(bufferLength_mSec*(samplesPerMSec)) + 1; // +1 for fractional part

		// --- create new buffer
		delayBuffer.createCircularBuffer(bufferLength);
	}

	/** read delay at current location */
	double readDelay()
	{
		// --- simple read
		return delayBuffer.readBuffer(simpleDelayParameters.delay_Samples);
	}

	/** read delay at current location */
	double readDelayAtTime_mSec(double _delay_mSec)
	{
		// --- calculate total delay time in samples + fraction
		double _delay_Samples = _delay_mSec*(samplesPerMSec);

		// --- simple read
		return delayBuffer.readBuffer(_delay_Samples);
	}

	/** read delay at a percentage of total length */
	double readDelayAtPercentage(double delayPercent)
	{
		// --- simple read
		return delayBuffer.readBuffer((delayPercent / 100.0)*simpleDelayParameters.delay_Samples);
	}

	/** write a new value into the delay */
	void writeDelay(double xn)
	{
		// --- simple write
		delayBuffer.writeBuffer(xn);
	}

private:
	SimpleDelayParameters simpleDelayParameters; ///< object parameters

	double sampleRate = 0.0;		///< sample rate
	double samplesPerMSec = 0.0;	///< samples per millisecond (for arbitrary access)
	double bufferLength_mSec = 0.0; ///< total buffer lenth in mSec
	unsigned int bufferLength = 0;	///< buffer length in samples

	// --- delay buffer of doubles
	CircularBuffer<double> delayBuffer; ///< circular buffer for delay
};


