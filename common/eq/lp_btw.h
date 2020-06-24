#ifndef __shared_eq_BTW_LP__
#define __shared_eq_BTW_LP__

#include <cmath>
#include "custdata_types.h"

class BTW_LP
{
public:
	BTW_LP(fsx inpFreq = 22050.0, fsx inpSRate = 44100.0);
	~BTW_LP();

	void FilterInit();
	void setSampleRate(const fsx inpSRate);
	void zeroBuffers();
	void changeFreq(const fsx nFreq);
	void RunFilter();
	void inpSplL(const flx inp);
	void inpSplS(const fsx inp);
	void inpSplH(const fhx inp);
	flx GetOutPutL();
	fsx GetOutPutS();
	fhx GetOutPutH();

private:
	const flx l_PI;
	const fsx s_PI;
	const fhx h_PI;

	flx l_smp;
	fsx s_smp;
	fhx h_smp;

	int precLevel;

	flx l_sRate;
	flx l_cutOff;
	flx l_sT[4];
	flx l_sCoef[4];
	flx l_sHist[5];
	flx l_wp;
	flx l_sGain;

	fsx s_sRate;
	fsx s_cutOff;
	fsx s_sT[4];
	fsx s_sCoef[4];
	fsx s_sHist[5];
	fsx s_wp;
	fsx s_sGain;

	fhx h_sRate;
	fhx h_cutOff;
	fhx h_sT[4];
	fhx h_sCoef[4];
	fhx h_sHist[5];
	fhx h_wp;
	fhx h_sGain;
};

#endif
