#pragma once

#include <math.h>
#include "dynamicsProcessor.h"

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
class FeedForwardDynamicsProcessor : public DynProcessor
{
public:
	FeedForwardDynamicsProcessor() {}	/* C-TOR */
	~FeedForwardDynamicsProcessor() {}	/* D-TOR */

public:

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

};