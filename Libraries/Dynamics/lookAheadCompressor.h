#pragma once

#include <math.h>
#include "dynamicsProcessor.h"
#include "../delay.h"

/**
\class FeedForwardDynamicsProcessor
\ingroup FX-Objects
\brief
The DynamicsProcessor object implements a dynamics processor suite: compressor, limiter, downward expander, gate.

Audio I/O:
- Processes mono input to mono output.

Control I/F:
- Use DynamicsProcessorParameters structure to get/set object params.

\author Will Pirkle http://www.willpirkle.com
\remark This object is included in Designing Audio Effects Plugins in C++ 2nd Ed. by Will Pirkle
\version Revision : 1.0
\date Date : 2018 / 09 / 7
*/
class LookAheadCompressor : public DynProcessor
{
public:
	LookAheadCompressor() {}	/* C-TOR */
	~LookAheadCompressor() {}	/* D-TOR */

public:

	/** reset members to initialized state */
	virtual bool reset(double _sampleRate);

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
	virtual double processAudioSample(double xn);

	/** set parameters: note use of custom structure for passing param data */
/**
\param DynamicsProcessorParameters custom data structure
*/
	virtual void setParameters(const DynProcessorParameters& _parameters);


private:
	SimpleDelay delayLine;

};