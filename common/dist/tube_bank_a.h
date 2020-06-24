#ifndef __dist_tube_bank_a__
#define __dist_tube_bank_a__

#include <cmath>
#include "custdata_types.h"
#include "lp_bworth.h"
#include "sample_bank_fp.h"

#define DIST_TBa_MAX_STAGES 5

#define DIST_TBa_STAGES_CLEAN 3	// 3
#define DIST_TBa_STAGES_CRUNCH 4	// 4
#define DIST_TBa_STAGES_HIGH 4	// 10
#define DIST_TBa_STAGES_MEGA 5	// 16

#define DIST_TBa_STAGES_CLEAN_wRate 0.2500 // 1.50, 1.1250, 0.750
#define DIST_TBa_STAGES_CRUNCH_wRate 0.2500 // 0.750, 0.350
#define DIST_TBa_STAGES_HIGH_wRate 0.2500 // 0.6250, 0.2750
#define DIST_TBa_STAGES_MEGA_wRate 0.2500 // 0.7250, 0.2000

#define DIST_TBa_STAGES_CLEAN_tHrm 0.20000
#define DIST_TBa_STAGES_CRUNCH_tHrm 0.32000
#define DIST_TBa_STAGES_HIGH_tHrm 0.40000	// 0.20
#define DIST_TBa_STAGES_MEGA_tHrm 0.55000	// 0.20

class Tube_BankA : public SampleBank_FP
{
public:
	Tube_BankA(fsx spRate, fhx mdfr);
	~Tube_BankA();
	void InitTubes();
	void zeroAllSamples();
	void ApplyDistortion();
	void setSampleRate(fsx nSRate);
	void SetGain(const fsx nGain);
	void clearBuffs();
	void SetChannel(const int nChan);
	void setOverSampling(const int oSLevel);

private:
	BTWorthLP_LD *FizzFiltL;
	BTWorthLP_SD *FizzFiltS;
	BTWorthLP_HD *FizzFiltH;

	const fhx stMod;

	int currChannel;

	fhx rndMF;

	fhx kFMult[4];

	fhx h_Gain;
	fhx h_smp;

	fsx s_Gain;
	fsx s_smp;

	flx l_Gain;
	flx l_smp;

	fhx h_fxlm_a;
	fhx h_base_drive[4];
	fhx h_d_eff_b[4];
	fhx h_d_eff_a[4];
	fhx h_cutback;
	fhx h_rt_fx_b[4];
	fhx h_rt_fx_a[4];
	fhx h_hist0[4][DIST_TBa_MAX_STAGES];
	fhx h_hist1[4][DIST_TBa_MAX_STAGES];
	fhx h_hist2[4][DIST_TBa_MAX_STAGES];
	fhx h_hist3[4][DIST_TBa_MAX_STAGES];
	fhx h_dcFa[4][DIST_TBa_MAX_STAGES];
	fhx h_dcFb[4][DIST_TBa_MAX_STAGES];
	fhx h_dcFfa[4][DIST_TBa_MAX_STAGES];
	fhx h_dcFfb[4][DIST_TBa_MAX_STAGES];
	fhx h_fr_filt[4][DIST_TBa_MAX_STAGES];
	fhx h_fxlm_b[4][DIST_TBa_MAX_STAGES];

	fsx s_fxlm_a;
	fsx s_base_drive[4];
	fsx s_d_eff_b[4];
	fsx s_d_eff_a[4];
	fsx s_cutback;
	fsx s_rt_fx_b[4];
	fsx s_rt_fx_a[4];
	fsx s_hist0[4][DIST_TBa_MAX_STAGES];
	fsx s_hist1[4][DIST_TBa_MAX_STAGES];
	fsx s_hist2[4][DIST_TBa_MAX_STAGES];
	fsx s_hist3[4][DIST_TBa_MAX_STAGES];
	fsx s_dcFa[4][DIST_TBa_MAX_STAGES];
	fsx s_dcFb[4][DIST_TBa_MAX_STAGES];
	fsx s_dcFfa[4][DIST_TBa_MAX_STAGES];
	fsx s_dcFfb[4][DIST_TBa_MAX_STAGES];
	fsx s_fr_filt[4][DIST_TBa_MAX_STAGES];
	fsx s_fxlm_b[4][DIST_TBa_MAX_STAGES];

	flx l_fxlm_a;
	flx l_base_drive[4];
	flx l_d_eff_b[4];
	flx l_d_eff_a[4];
	flx l_cutback;
	flx l_rt_fx_b[4];
	flx l_rt_fx_a[4];
	flx l_hist0[4][DIST_TBa_MAX_STAGES];
	flx l_hist1[4][DIST_TBa_MAX_STAGES];
	flx l_hist2[4][DIST_TBa_MAX_STAGES];
	flx l_hist3[4][DIST_TBa_MAX_STAGES];
	flx l_dcFa[4][DIST_TBa_MAX_STAGES];
	flx l_dcFb[4][DIST_TBa_MAX_STAGES];
	flx l_dcFfa[4][DIST_TBa_MAX_STAGES];
	flx l_dcFfb[4][DIST_TBa_MAX_STAGES];
	flx l_fr_filt[4][DIST_TBa_MAX_STAGES];
	flx l_fxlm_b[4][DIST_TBa_MAX_STAGES];
};

#endif
