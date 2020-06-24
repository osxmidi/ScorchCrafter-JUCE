#ifndef __shared_eq_bworthLP__
#define __shared_eq_bworthLP__

#include <cmath>
#include "custdata_types.h"

class BTWorthLP
{
public:
	BTWorthLP(fsx inpFreq, fsx inpSRate);
	~BTWorthLP() {};

	void setSampleRate(const fsx inpSRate);
	void reset();
	void zeroBuffers();
	void changeFreq(fsx nFreq);
	flx runFilter(flx inp);
	fsx runFilter(fsx inp);
	fhx runFilter(fhx inp);

private:
	const fhx PI;
	const fhx hPI;
	const fhx PId;
	const fhx zeroF;

	fhx sRate;
	fhx cutOff;

	fhx sT[4];
	fhx sCoef[4];
	fhx sHist[4];
	fhx wp;
	fhx sGain;
};

class BTWorthLP_LD
{
public:
	BTWorthLP_LD(fsx inpFreq, fsx inpSRate);
	~BTWorthLP_LD() {};

	void setSampleRate(const fsx inpSRate);
	void reset();
	void zeroBuffers();
	void changeFreq(fsx nFreq);
	flx runFilterL(flx inp);

private:
	const flx PI;
	const flx hPI;
	const flx PId;
	const flx zeroF;

	fsx sRate;
	fsx cutOff;

	flx sT[4];
	flx sCoef[4];
	flx sHist[4];
	flx wp;
	flx sGain;
};

class BTWorthLP_SD
{
public:
	BTWorthLP_SD(fsx inpFreq, fsx inpSRate);
	~BTWorthLP_SD() {};

	void setSampleRate(const fsx inpSRate);
	void reset();
	void zeroBuffers();
	void changeFreq(fsx nFreq);
	fsx runFilterS(fsx inp);

private:
	const fsx PI;
	const fsx hPI;
	const fsx PId;
	const fsx zeroF;

	fsx sRate;
	fsx cutOff;

	fsx sT[4];
	fsx sCoef[4];
	fsx sHist[4];
	fsx wp;
	fsx sGain;
};

class BTWorthLP_HD
{
public:
	BTWorthLP_HD(fsx inpFreq, fsx inpSRate);
	~BTWorthLP_HD() {};

	void setSampleRate(const fsx inpSRate);
	void reset();
	void zeroBuffers();
	void changeFreq(fsx nFreq);
	fhx runFilterH(fhx inp);

private:
	const fhx PI;
	const fhx hPI;
	const fhx PId;
	const fhx zeroF;

	fhx sRate;
	fhx cutOff;

	fhx sT[4];
	fhx sCoef[4];
	fhx sHist[4];
	fhx wp;
	fhx sGain;
};

#endif
