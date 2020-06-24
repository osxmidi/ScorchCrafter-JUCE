#include "tube_bank_a.h"
#include <cstdlib>

Tube_BankA::Tube_BankA(fsx spRate, fhx mdfr) :
stMod(fhx (mdfr))
{
	SampleBank_FP(fsx (spRate));

	FizzFiltL = new BTWorthLP_LD(fsx (7200.0), fsx (spRate));
	FizzFiltS = new BTWorthLP_SD(fsx (7200.0), fsx (spRate));
	FizzFiltH = new BTWorthLP_HD(fsx (7200.0), fsx (spRate));
}
Tube_BankA::~Tube_BankA()
{
	delete FizzFiltL;
	delete FizzFiltS;
	delete FizzFiltH;
}
void Tube_BankA::InitTubes()
{
	h_fxlm_a = fhx (0.650);	// 0.40, 0.60, 0.850, 0.950
	h_cutback = fhx (-0.50); // 0.55750

	s_fxlm_a = fsx (h_fxlm_a);
	s_cutback = fsx (h_cutback);

	l_fxlm_a = flx (h_fxlm_a);
	l_cutback = flx (h_cutback);

	kFMult[0] = fhx (fhx (DIST_TBa_STAGES_CLEAN_wRate) * fhx (0.010) * fhx (stMod));
	kFMult[1] = fhx (fhx (DIST_TBa_STAGES_CRUNCH_wRate) * fhx (0.010) * fhx (stMod));
	kFMult[2] = fhx (fhx (DIST_TBa_STAGES_HIGH_wRate) * fhx (0.010) * fhx (stMod));
	kFMult[3] = fhx (fhx (DIST_TBa_STAGES_MEGA_wRate) * fhx (0.010) * fhx (stMod));
	fhx kTF[4];
	kTF[0] = fhx (DIST_TBa_STAGES_CLEAN_tHrm);
	kTF[1] = fhx (DIST_TBa_STAGES_CRUNCH_tHrm);
	kTF[2] = fhx (DIST_TBa_STAGES_HIGH_tHrm);
	kTF[3] = fhx (DIST_TBa_STAGES_MEGA_tHrm);

	// const fhx kFactor = fhx (fhx (44100.0) / fhx (fhx (rfV->vh[osRate]) * fhx (sRateH)));
	const fhx kFactor = fhx (1.0);

	for(int x = 0; x< 4; x++)
	{
		h_rt_fx_b[x] = fhx (fhx (kFMult[x]) * fhx (kFactor));
		h_rt_fx_a[x] = fhx (fhx (1.0) - fhx (h_rt_fx_b[x]));
		h_base_drive[x] = fhx (fhx (kTF[x]) * fhx (1.0));
		h_d_eff_a[x] = fhx (fhx (kTF[x]) * fhx (1.0));
		h_d_eff_b[x] = fhx (fhx (kTF[x]) * fhx (1.0));

		s_base_drive[x] = fsx (h_base_drive[x]);
		s_d_eff_b[x] = fsx (h_d_eff_b[x]);
		s_d_eff_a[x] = fsx (h_d_eff_a[x]);
		s_rt_fx_b[x] = fsx (h_rt_fx_b[x]);
		s_rt_fx_a[x] = fsx (h_rt_fx_a[x]);

		l_base_drive[x] = flx (h_base_drive[x]);
		l_d_eff_b[x] = flx (h_d_eff_b[x]);
		l_d_eff_a[x] = flx (h_d_eff_a[x]);
		l_rt_fx_b[x] = flx (h_rt_fx_b[x]);
		l_rt_fx_a[x] = flx (h_rt_fx_a[x]);
	}

	clearBuffs();
}
void Tube_BankA::setSampleRate(fsx nSRate)
{
	SampleBank_FP::setSampleRate(nSRate);

	SampleBank_FP::zeroAllSamples();

	FizzFiltL->setSampleRate(fsx (cRateS));
	FizzFiltS->setSampleRate(fsx (cRateS));
	FizzFiltH->setSampleRate(fsx (cRateS));

	const fhx kFactor = fhx (fhx (44100.0) / fhx (cRateH));

	for(int x = 0; x< 4; x++)
	{
		h_rt_fx_b[x] = fhx (fhx (kFMult[x]) * fhx (kFactor));
		h_rt_fx_a[x] = fhx (fhx (1.0) - fhx (h_rt_fx_b[x]));

		s_rt_fx_b[x] = fsx (h_rt_fx_b[x]);
		s_rt_fx_a[x] = fsx (h_rt_fx_a[x]);

		l_rt_fx_b[x] = flx (h_rt_fx_b[x]);
		l_rt_fx_a[x] = flx (h_rt_fx_a[x]);
	}

	clearBuffs();
}
void Tube_BankA::SetGain(const fsx nGain)
{
	h_Gain = fhx (nGain);
	if(fhx (h_Gain) < fhx (rfV->zh)) h_Gain = fhx (rfV->zh);
	if(fhx (h_Gain) > fhx (1.0)) h_Gain = fhx (1.0);
	h_Gain *= fhx (128.0);

	s_Gain = fsx (h_Gain);
	l_Gain = flx (h_Gain);
}
void Tube_BankA::clearBuffs()
{
	for(int x = 0; x < 4; x++)
	{
		for(int y = 0; y < DIST_TBa_MAX_STAGES; y++)
		{
			h_dcFa[x][y] = fhx (0.0);
			h_dcFb[x][y] = fhx (0.0);
			h_dcFfa[x][y] = fhx (0.0);
			h_dcFfb[x][y] = fhx (0.0);
			h_fr_filt[x][y] = fhx (0.0);
			h_fxlm_b[x][y] = fhx (0.0);
			h_hist0[x][y] = fhx (0.0);
			h_hist1[x][y] = fhx (0.0);
			h_hist2[x][y] = fhx (0.0);
			h_hist3[x][y] = fhx (0.0);

			s_dcFa[x][y] = fsx (0.0);
			s_dcFb[x][y] = fsx (0.0);
			s_dcFfa[x][y] = fsx (0.0);
			s_dcFfb[x][y] = fsx (0.0);
			s_fr_filt[x][y] = fsx (0.0);
			s_fxlm_b[x][y] = fsx (0.0);
			s_hist0[x][y] = fsx (0.0);
			s_hist1[x][y] = fsx (0.0);
			s_hist2[x][y] = fsx (0.0);
			s_hist3[x][y] = fsx (0.0);

			l_dcFa[x][y] = flx (0.0f);
			l_dcFb[x][y] = flx (0.0f);
			l_dcFfa[x][y] = flx (0.0f);
			l_dcFfb[x][y] = flx (0.0f);
			l_fr_filt[x][y] = flx (0.0f);
			l_fxlm_b[x][y] = flx (0.0f);
			l_hist0[x][y] = flx (0.0f);
			l_hist1[x][y] = flx (0.0f);
			l_hist2[x][y] = flx (0.0f);
			l_hist3[x][y] = flx (0.0f);
		}
	}
}
void Tube_BankA::zeroAllSamples()
{
	SampleBank_FP::zeroAllSamples();
}
void Tube_BankA::setOverSampling(const int oSLevel)
{
	SampleBank_FP::setOverSampling(oSLevel);

	FizzFiltL->setSampleRate(fsx (cRateS));
	FizzFiltS->setSampleRate(fsx (cRateS));
	FizzFiltH->setSampleRate(fsx (cRateS));

	clearBuffs();

	const fhx kFactor = fhx (fhx (44100.0) / fhx (cRateH));

	for(int x = 0; x< 4; x++)
	{
		h_rt_fx_b[x] = fhx (fhx (kFMult[x]) * fhx (kFactor));
		h_rt_fx_a[x] = fhx (fhx (1.0) - fhx (h_rt_fx_b[x]));

		s_rt_fx_b[x] = fsx (h_rt_fx_b[x]);
		s_rt_fx_a[x] = fsx (h_rt_fx_a[x]);

		l_rt_fx_b[x] = flx (h_rt_fx_b[x]);
		l_rt_fx_a[x] = flx (h_rt_fx_a[x]);
	}
}
void Tube_BankA::ApplyDistortion()
{
	if(osRate <= 0) return;
	if(osRate >= 6) return;

	unsigned int numStages = 0;
	if(currChannel == 0) numStages = DIST_TBa_STAGES_CLEAN;
	if(currChannel == 1) numStages = DIST_TBa_STAGES_CRUNCH;
	if(currChannel == 2) numStages = DIST_TBa_STAGES_HIGH;
	if(currChannel == 3) numStages = DIST_TBa_STAGES_MEGA;
	if(numStages == 0) return;
	if(numStages > DIST_TBa_MAX_STAGES) numStages = DIST_TBa_MAX_STAGES;

	if(precisionLevel == 0)
	{
		for(int fzFilt = 0; fzFilt < rfV->vi[osRate]; fzFilt++)
		{
			smpl[fzFilt] = (flx) FizzFiltS->runFilterS(flx (smpl[fzFilt]));
		}
		for(int stg = 0; stg < numStages; stg++)
		{
			for(int spNum = 0; spNum < rfV->vi[osRate]; spNum++)
			{
				l_smp = flx (smpl[spNum]);
				if(flx (l_smp) == flx (0.0f)) l_smp = flx (rfV->zl);

				const flx cl_spl = flx (l_smp);
				const flx h0 = flx (l_d_eff_a[currChannel] + (tanf(cl_spl)) * l_d_eff_b[currChannel] * flx (0.30f));
				l_dcFa[currChannel][stg] = flx (flx (h0) - flx (l_dcFfa[currChannel][stg] =
													flx (h0*l_rt_fx_b[currChannel]+l_dcFfa[currChannel][stg]*l_rt_fx_a[currChannel])));
				l_fxlm_b[currChannel][stg] = (flx) fminf(fmaxf(l_dcFa[currChannel][stg]*l_Gain,flx (-l_fxlm_a)), l_fxlm_a);
				l_hist1[currChannel][stg] = flx (l_hist0[currChannel][stg]);
				l_hist3[currChannel][stg] = flx (l_hist2[currChannel][stg]);
				l_fr_filt[currChannel][stg] = flx (flx (0.50f) * flx (flx (l_hist3[currChannel][stg]) + flx (l_hist2[currChannel][stg] = (flx) flx (0.50f) *
													flx (l_hist1[currChannel][stg] + flx (l_hist0[currChannel][stg] = (flx) l_fxlm_b[currChannel][stg])))));
				l_dcFb[currChannel][stg] = flx (l_fr_filt[currChannel][stg] - flx (l_dcFfb[currChannel][stg] =
															(flx) l_fr_filt[currChannel][stg]*l_rt_fx_b[currChannel]+l_dcFfb[currChannel][stg]*l_rt_fx_a[currChannel]));

				l_smp = flx (flx (flx (l_smp) + flx (l_dcFb[currChannel][stg])) * flx (l_cutback));
				smpl[spNum] = flx (l_smp);
			}
		}
		return;
	}
	if(precisionLevel == 1)
	{
		for(unsigned int fzFilt = 0; fzFilt < rfV->vi[osRate]; fzFilt++)
		{
			smps[fzFilt] = (fsx) FizzFiltS->runFilterS(fsx (smps[fzFilt]));
		}
		for(unsigned int stg = 0; stg < numStages; stg++)
		{
			for(unsigned int spNum = 0; spNum < rfV->vi[osRate]; spNum++)
			{
				s_smp = fsx (smps[spNum]);
				if(fsx (s_smp) == fsx (0.0)) s_smp = fsx (rfV->zs);

				const fsx cs_spl = fsx (s_smp);
				const fsx h0 = fsx ((fsx) sin(fsx (s_d_eff_a[currChannel]) * fsx (cs_spl)) / (fsx) sin(fsx (cs_spl)) * fsx (s_d_eff_a[currChannel])
												+ fsx ((fsx) tan(fsx (cs_spl)) * fsx (s_d_eff_b[currChannel])) * fsx (s_d_eff_b[currChannel]) * fsx (0.250));

				s_dcFa[currChannel][stg] = fsx (fsx (h0) - fsx (s_dcFfa[currChannel][stg] =
						fsx (h0 * s_rt_fx_b[currChannel] + s_dcFfa[currChannel][stg] * s_rt_fx_a[currChannel])));

				s_fxlm_b[currChannel][stg] = (fsx) fmin((fsx) fmax(fsx (s_dcFa[currChannel][stg] * s_Gain),fsx (-s_fxlm_a)), fsx (s_fxlm_a));

				s_hist1[currChannel][stg] = fsx (s_hist0[currChannel][stg]);
				s_hist3[currChannel][stg] = fsx (s_hist2[currChannel][stg]);

				s_fr_filt[currChannel][stg] = fsx (fsx (0.50) * fsx (fsx (s_hist3[currChannel][stg]) + fsx (s_hist2[currChannel][stg] = (fsx) fsx (0.50) *
													fsx (s_hist1[currChannel][stg] + fsx (s_hist0[currChannel][stg] = (fsx) s_fxlm_b[currChannel][stg])))));

				s_dcFb[currChannel][stg] = fsx (s_fr_filt[currChannel][stg] - fsx (s_dcFfb[currChannel][stg] =
									fsx ((fsx) s_fr_filt[currChannel][stg] * s_rt_fx_b[currChannel] + s_dcFfb[currChannel][stg] * s_rt_fx_a[currChannel])));

				smps[spNum] = fsx (fsx (fsx (s_smp) + fsx (s_dcFb[currChannel][stg])) * fsx (s_cutback));
			}
		}
		return;
	}
	if(precisionLevel == 2)
	{
		for(unsigned int fzFilt = 0; fzFilt < rfV->vi[osRate]; fzFilt++)
		{
			smph[fzFilt] = (fhx) FizzFiltH->runFilterH(fhx (smph[fzFilt]));
		}
		for(unsigned int stg = 0; stg < numStages; stg++)
		{
			for(unsigned int spNum = 0; spNum < rfV->vi[osRate]; spNum++)
			{
				h_smp = fhx (smph[spNum]);
				if(fhx (h_smp) == fhx (0.0)) h_smp = fhx (rfV->zh);

				const fhx cs_spl = fhx (h_smp);
				const fhx h0 = fhx ((fhx) sinl(fhx (h_d_eff_a[currChannel]) * fhx (cs_spl)) / (fhx) sinl(fhx (cs_spl)) * fhx (h_d_eff_a[currChannel])
												+ fhx ((fhx) tanl(fhx (cs_spl)) * fhx (h_d_eff_b[currChannel])) * fhx (h_d_eff_b[currChannel]) * fhx (0.250));

				h_dcFa[currChannel][stg] = fhx (fhx (h0) - fhx (h_dcFfa[currChannel][stg] =
						fhx (h0 * h_rt_fx_b[currChannel] + h_dcFfa[currChannel][stg] * h_rt_fx_a[currChannel])));

				h_fxlm_b[currChannel][stg] = (fhx) fminl((fhx) fmaxl(fhx (h_dcFa[currChannel][stg] * h_Gain),fhx (-h_fxlm_a)), fhx (h_fxlm_a));

				h_hist1[currChannel][stg] = fhx (h_hist0[currChannel][stg]);
				h_hist3[currChannel][stg] = fhx (h_hist2[currChannel][stg]);

				h_fr_filt[currChannel][stg] = fhx (fhx (0.50) * fhx (fhx (h_hist3[currChannel][stg]) + fhx (h_hist2[currChannel][stg] = (fhx) fhx (0.50) *
													fhx (h_hist1[currChannel][stg] + fhx (h_hist0[currChannel][stg] = (fhx) h_fxlm_b[currChannel][stg])))));

				h_dcFb[currChannel][stg] = fhx (h_fr_filt[currChannel][stg] - fhx (h_dcFfb[currChannel][stg] =
									fhx ((fhx) h_fr_filt[currChannel][stg] * h_rt_fx_b[currChannel] + h_dcFfb[currChannel][stg] * h_rt_fx_a[currChannel])));

				smph[spNum] = fhx (fhx (fhx (h_smp) + fhx (h_dcFb[currChannel][stg])) * fhx (h_cutback));
			}
		}
		return;
	}
}
void Tube_BankA::SetChannel(const int nChan)
{
	if(currChannel == nChan) return;
	if(nChan < 0) return;
	if(nChan > 3) return;

	currChannel = nChan;
	zeroAllSamples();
}
