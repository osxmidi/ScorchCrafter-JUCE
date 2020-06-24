#ifndef __eq_tstack_a__
#define __eq_tstack_a__

#include <cmath>
#include "lp_bworth.h"
#include "../custdata_types.h"
// #include "sample_bank.h"

class ToneStack_A
{
public:
	ToneStack_A(fsx nRate = 44100.0);
	~ToneStack_A();

	void setNewSampleRate(const fsx nRate);
	void setBandValue(const int bNum, const fsx value);
	void SetInput(const fsx inp);
	fsx GetProcessedOutput();
	void setEvenShaped(const bool isShaped);
	void ClearBuffs();
	void SetPrecision(int nLevel);
	void RunEQ();

private:
	flx l_spl;
	fsx s_spl;
	fhx h_spl;

	fhx sRate;
	bool setShaped;

	flx l_bValue[6];
	fsx s_bValue[6];
	fhx h_bValue[6];

	int fpPrec;

	flx l_EqBand_Freq[6][2];
	flx l_EqBand_Pos[6][2];
	flx l_subSpl[6];

	fsx s_EqBand_Freq[6][2];
	fsx s_EqBand_Pos[6][2];
	fsx s_subSpl[6];

	fhx h_EqBand_Freq[6][2];
	fhx h_EqBand_Pos[6][2];
	fhx h_subSpl[6];

	flx l_outputF_a_pos;
	flx l_outputF_a_frq;
	flx l_outputF_b_pos;
	flx l_outputF_b_frq;
	flx l_outputF_spl[2];

	fsx s_outputF_a_pos;
	fsx s_outputF_a_frq;
	fsx s_outputF_b_pos;
	fsx s_outputF_b_frq;
	fsx s_outputF_spl[2];

	fhx h_outputF_a_pos;
	fhx h_outputF_a_frq;
	fhx h_outputF_b_pos;
	fhx h_outputF_b_frq;
	fhx h_outputF_spl[2];

	BTWorthLP_LD *DeFizzFL;
	BTWorthLP_SD *DeFizzFS;
	BTWorthLP_HD *DeFizzFH;
};

#endif
