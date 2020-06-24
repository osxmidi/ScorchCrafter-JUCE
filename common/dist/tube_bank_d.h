#ifndef __dist_tube_bank_d__
#define __dist_tube_bank_d__

class SampleBank_Cst;
class Cst_EQ;

#include <cmath>
#include "custdata_types.h"
#include "lp_bworth.h"
#include "sample_bank_custom.h"
#include "csts_eq.h"
#include "plugin_c120.h"

class Tube_BankD : public SampleBank_Cst
{
public:
	Tube_BankD(fsx spRate, fhx mdfr);
	~Tube_BankD();
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
	Cst_EQ *FilterA;
	Cst_EQ *FilterB;

	const fhx stMod;
	const fhx h_distFreqHi;
	const fhx h_distFreqLo;
	const fsx s_distFreqHi;
	const fsx s_distFreqLo;
	const flx l_distFreqHi;
	const flx l_distFreqLo;

	const fhx h_tRatio;
	const fsx s_tRatio;
	const flx l_tRatio;

	int currChannel;

	fhx h_tSpeed;
	fsx s_tSpeed;
	flx l_tSpeed;
	fhx h_tPos[Dist_TBD_PreAmp_Stages_Max];
	fsx s_tPos[Dist_TBD_PreAmp_Stages_Max];
	flx l_tPos[Dist_TBD_PreAmp_Stages_Max];

	fhx h_Gain;
	fhx h_smp;
	const fhx h_pi;
	fhx h_dfltA;
	fhx h_dfltB;
	fhx h_qFiltT[Dist_TBD_PreAmp_Stages_Max][Dist_TBD_TubeFreqStages];
	fhx h_eff;

	fsx s_Gain;
	fsx s_smp;
	const fsx s_pi;
	fsx s_dfltA;
	fsx s_dfltB;
	fsx s_qFiltT[Dist_TBD_PreAmp_Stages_Max][Dist_TBD_TubeFreqStages];
	fsx s_eff;

	flx l_Gain;
	flx l_smp;
	const flx l_pi;
	flx l_dfltA;
	flx l_dfltB;
	flx l_qFiltT[Dist_TBD_PreAmp_Stages_Max][Dist_TBD_TubeFreqStages];
	flx l_eff;

	fhx h_md;
	fsx s_md;
	flx l_md;

	fhx h_sv;
	fsx s_sv;
	flx l_sv;

	fhx mdV;
};

#endif
