#ifndef __SC_eq_tstack_d__
#define __SC_eq_tstack_d__

class RateFC;
class Cst_EQ;

#include <cmath>
#include "lp_btw.h"
#include "csts_eq.h"
#include "../custdata_types.h"
#include "tstack_wav_c.h"
#include "plugin_c120.h"

#define SC_NUM_TSTACK_D_IMPULSES 3

class ToneStack_D
{
public:
	ToneStack_D(fsx nRate = 44100.0, fsx CnvShift = 0.0, fsx dcSkew = 0.0);
	~ToneStack_D();

	void setNewSampleRate(const fsx nRate);
	void setBandValue(const int bNum, fsx value);
	void SetInput(const fsx inp);
	fsx GetProcessedOutput();
	void setEvenShaped(const bool isShaped);
	void setCabMic(const bool inp) {CabMic = inp;};
	void ClearBuffs();
	void SetPrecision(int nLevel);
	void RunEQ();
	void RunConv() { if(setShaped || CabMic) RunConvolution(); };
	void SetDynamicQ(const fsx inp);

private:
	void InitToneImpulses();
	void RunConvolution();

	static const fhx qBandRange_Low[2];
	static const fhx qBandRange_Mid[2];
	static const fhx qBandRange_High[2];
	static const fhx qBandRange_ContourL[2];
	static const fhx qBandRange_ContourH[2];
	static const fhx qBandRange_Presence[2];

	const fsx cShiftPcnt;
	const fsx cSkewPcnt;
	int currImpShift;

	RateFC *rfV;

	Cst_EQ *qBand[6];
	Cst_EQ *DynBass;

	fhx h_envPos;
	fsx s_envPos;
	flx l_envPos;

	flx l_spl;
	fsx s_spl;
	fhx h_spl;

	fhx sRate;
	bool setShaped;
	bool CabMic;

	flx l_bValue[6];
	fsx s_bValue[6];
	fhx h_bValue[6];

	flx *smpL;
	fsx *smpS;
	fhx *smpH;

	int spLength;

	int fpPrec;

	flx **impulseL;
	fsx **impulseS;
	fhx **impulseH;

	int impLength[SC_NUM_TSTACK_D_IMPULSES];

	int smpPos;

	flx volAdj_l[SC_NUM_TSTACK_D_IMPULSES];
	fsx volAdj_s[SC_NUM_TSTACK_D_IMPULSES];
	fhx volAdj_h[SC_NUM_TSTACK_D_IMPULSES];
};

#endif
