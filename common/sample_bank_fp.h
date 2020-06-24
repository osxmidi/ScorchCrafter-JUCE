#ifndef __shared_sample_bank_fp__
#define __shared_sample_bank_fp__

#include <cmath>
// #include <algorithm>
#include "custdata_types.h"
#include "lp_bworth.h"

class SampleBank_FP
{
public:
	SampleBank_FP(fsx iSrate = 44100.0);
	~SampleBank_FP();
	void setSampleRate(fsx nSRate);
	void zeroAllSamples();
	void setOverSampling(const int oSLevel);
	void runUpSampling();
	void runDownSampling();
	void SetFPprecision(const int nLevel);
	void InputSmp(const fhx inp);
	fsx GetOutP();

	flx smpl[16];
	fsx smps[16];
	fhx smph[16];

	RateFC *rfV;
	int osRate;
	int precisionLevel;

	flx sRateL;
	fsx sRateS;
	fhx sRateH;

	flx cRateL;
	fsx cRateS;
	fhx cRateH;

private:
	BTWorthLP_LD *UpFilterL[5];
	BTWorthLP_LD *DnFilterL[5];
	BTWorthLP_SD *UpFilterS[5];
	BTWorthLP_SD *DnFilterS[5];
	BTWorthLP_HD *UpFilterH[5];
	BTWorthLP_HD *DnFilterH[5];
};

#endif
