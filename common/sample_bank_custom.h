#ifndef __shared_sample_bank_Custom__
#define __shared_sample_bank_Custom__

class RateFC;
class BTW_LP;

#include <cmath>
// #include <algorithm>
#include "custdata_types.h"
#include "eq/lp_btw.h"

class SampleBank_Cst
{
public:
	SampleBank_Cst(fsx iSrate = 44100.0);
	~SampleBank_Cst();
	void setSampleRate(fsx nSRate);
	void zeroAllSamples();
	void setOverSampling(const int oSLevel);
	void runUpSampling();
	void runDownSampling();
	void SetFPprecision(const int nLevel);
	void InputSmp(const fsx inp);
	fsx GetOutP();
	flx GetOutPL();
	fsx GetOutPS();
	fhx GetOutPH();

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
	BTW_LP *UpFilter[2];
	BTW_LP *DnFilter[2];
};

#endif
