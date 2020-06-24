#ifndef __dist_tube_bank_b__
#define __dist_tube_bank_b__

#include <cmath>
#include "custdata_types.h"
#include "lp_bworth.h"
#include "sample_bank_fp.h"

#define DIST_TBa_MAX_STAGES 3

#define DIST_TBa_STAGES_CLEAN 2	// 3
#define DIST_TBa_STAGES_CRUNCH 2 // 4
#define DIST_TBa_STAGES_HIGH 3	// 10
#define DIST_TBa_STAGES_MEGA 3	// 16

#define DIST_TBb_ST_POWER_A 64.0 // 10.000
#define DIST_TBb_ST_POWER_B 256.0 // 12.000
#define DIST_TBb_ST_POWER_C 128.0 // 20.000
#define DIST_TBb_ST_POWER_D 256.0 // 25.000

class Tube_BankB : public SampleBank_FP
{
public:
	Tube_BankB(fsx spRate, fhx mdfr);
	~Tube_BankB();
	void InitTubes();
	void zeroAllSamples();
	void ApplyDistortion();
	void setSampleRate(fsx nSRate);
	void SetGain(const fsx nGain);
	void clearBuffs();
	void SetChannel(const int nChan);
	void setOverSampling(const int oSLevel);
	void SetAmpVt(const fsx inp);

private:
	BTWorthLP_LD *FizzFiltL;
	BTWorthLP_SD *FizzFiltS;
	BTWorthLP_HD *FizzFiltH;

	BTWorthLP_LD *DrvFL;
	BTWorthLP_SD *DrvFS;
	BTWorthLP_HD *DrvFH;

	const fhx stMod;
	const fhx lmtH;
	const fhx distFreqHi;
	const fhx distFreqLo;

	const fhx fEnvRateFac;

	int currChannel;

	fhx fltFreqH[2];
	fhx fltFreqS[2];
	fhx fltFreqL[2];

	fhx h_Gain;
	fhx h_smp;
	const fhx cBackH;
	fhx fShelfPosH;
	fhx fShelfRateH;
	fhx h_dfltA[DIST_TBa_MAX_STAGES];
	fhx h_dfltB[DIST_TBa_MAX_STAGES];
	fhx fltPosH[DIST_TBa_MAX_STAGES][2];

	fsx s_Gain;
	fsx s_smp;
	const fsx cBackS;
	fsx fShelfPosS;
	fsx fShelfRateS;
	fsx s_dfltA[DIST_TBa_MAX_STAGES];
	fsx s_dfltB[DIST_TBa_MAX_STAGES];
	fsx fltPosS[DIST_TBa_MAX_STAGES][2];

	flx l_Gain;
	flx l_smp;
	const flx cBackL;
	flx fShelfPosL;
	flx fShelfRateL;
	flx l_dfltA[DIST_TBa_MAX_STAGES];
	flx l_dfltB[DIST_TBa_MAX_STAGES];
	flx fltPosL[DIST_TBa_MAX_STAGES][2];

	fhx gFacH[4];
	fhx bGain;

	fhx mdH;
	fsx mdS;
	flx mdL;

	fhx mdV;
};

#endif
