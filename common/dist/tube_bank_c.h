#ifndef __dist_tube_bank_c__
#define __dist_tube_bank_c__

#include <cmath>
#include "custdata_types.h"
#include "lp_bworth.h"
#include "sample_bank_fp.h"
#include "csts_eq.h"

#define DIST_TBc_MAX_STAGES 4

#define DIST_TBc_STAGES_CLEAN (unsigned int) 1
#define DIST_TBc_STAGES_CRUNCH (unsigned int) 2
#define DIST_TBc_STAGES_HIGH (unsigned int) 3
#define DIST_TBc_STAGES_MEGA (unsigned int) 4

#define Dist_TBC_Num_SubStages (unsigned int) 6

#define DIST_TBc_ST_POWER_A 20.0 // 10.000
#define DIST_TBc_ST_POWER_B 80.0 // 12.000
#define DIST_TBc_ST_POWER_C 100.0 // 20.000
#define DIST_TBc_ST_POWER_D 500.0 // 25.000

#define Dist_TBc_Harmonic_Dist_A 1.0
#define Dist_TBc_Harmonic_Dist_B 0.10
#define Dist_TBc_Harmonic_Dist_C 0.060

#define Dist_TBC_Harmonic_CutBack double (1.0 / (Dist_TBc_Harmonic_Dist_A + Dist_TBc_Harmonic_Dist_B + Dist_TBc_Harmonic_Dist_C))

class Tube_BankC : public SampleBank_FP
{
public:
	Tube_BankC(fsx spRate, fhx mdfr);
	~Tube_BankC();
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
	Cst_EQ *FilterA[DIST_TBc_MAX_STAGES];
	Cst_EQ *FilterB[DIST_TBc_MAX_STAGES];

	const fhx stMod;
	const fhx distFreqHi;
	const fhx distFreqLo;

	const fhx fEnvRateFac;

	int currChannel;

	fhx sSplith[3];
	fsx sSplits[3];
	flx sSplitl[3];

	fhx h_Gain;
	fhx h_smp;
	const fhx cBackH;
	const fhx lmtH;
	fhx fShelfPosH;
	fhx fShelfRateH;
	fhx h_dfltA[DIST_TBc_MAX_STAGES];
	fhx h_dfltB[DIST_TBc_MAX_STAGES];

	fsx s_Gain;
	fsx s_smp;
	const fsx cBackS;
	const fhx lmtS;
	fsx fShelfPosS;
	fsx fShelfRateS;
	fsx s_dfltA[DIST_TBc_MAX_STAGES];
	fsx s_dfltB[DIST_TBc_MAX_STAGES];

	flx l_Gain;
	flx l_smp;
	const flx cBackL;
	const fhx lmtL;
	flx fShelfPosL;
	flx fShelfRateL;
	flx l_dfltA[DIST_TBc_MAX_STAGES];
	flx l_dfltB[DIST_TBc_MAX_STAGES];

	fhx gFacH[4];
	fhx bGain;

	fhx mdH;
	fsx mdS;
	flx mdL;

	fhx mdV;
};

#endif
