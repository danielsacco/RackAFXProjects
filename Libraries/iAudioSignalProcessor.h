#pragma once

/**
\class IAudioSignalProcessor
\ingroup Interfaces
\brief
Use this interface for objects that process audio input samples to produce audio output samples. A derived class must implement the three abstract methods. The others are optional.

\author Will Pirkle http://www.willpirkle.com
\remark This object is included in Designing Audio Effects Plugins in C++ 2nd Ed. by Will Pirkle
\version Revision : 1.0
\date Date : 2018 / 09 / 7
*/
class IAudioSignalProcessor
{
public:
	// --- pure virtual, derived classes must implement or will not compile
	//     also means this is a pure abstract base class and is incomplete,
	//     so it can only be used as a base class
	//
	/** initialize the object with the new sample rate */
	virtual bool reset(double _sampleRate) = 0;

	/** process one sample in and out */
	virtual double processAudioSample(double xn) = 0;

	/** return true if the derived object can process a frame, false otherwise */
	virtual bool canProcessAudioFrame() = 0;

	/** set or change the sample rate; normally this is done during reset( ) but may be needed outside of initialzation */
	virtual void setSampleRate(double _sampleRate) {}

	/** switch to enable/disable the aux input */
	virtual void enableAuxInput(bool enableAuxInput) {}

	/** for processing objects with a sidechain input or other necessary aux input
	        the return value is optional and will depend on the subclassed object */
	virtual double processAuxInputAudioSample(double xn)
	{
		// --- do nothing
		return xn;
	}

	/** for processing objects with a sidechain input or other necessary aux input
	--- optional processing function
		e.g. does not make sense for some objects to implement this such as inherently mono objects like Biquad
			 BUT a processor that must use both left and right channels (ping-pong delay) would require it */
	virtual bool processAudioFrame(const float* inputFrame,		/* ptr to one frame of data: pInputFrame[0] = left, pInputFrame[1] = right, etc...*/
								   float* outputFrame,
		unsigned int inputChannels,
		unsigned int outputChannels)
	{
		// --- do nothing
		return false; // NOT handled
	}
};
