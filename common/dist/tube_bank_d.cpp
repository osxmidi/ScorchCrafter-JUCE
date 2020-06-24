#include "tube_bank_d.h"

// Yes, I know these values start at 1.f and not 0.f, this is intentional and NOT an error or mistake!
// const flx Tube_BankD::l_hrmCnt[12] = {1.f, 2.f, 3.f, 4.f, 5.f, 6.f, 7.f, 8.f, 9.f, 10.f, 11.f, 12.f};
// const fsx Tube_BankD::s_hrmCnt[12] = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0, 11.0, 12.0};
// const fhx Tube_BankD::h_hrmCnt[12] = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0, 11.0, 12.0};

Tube_BankD::Tube_BankD(fsx spRate, fhx mdfr) : SampleBank_Cst(spRate),
	stMod(fhx (fhx (mdfr))),
	currChannel(0),
	h_distFreqHi(fhx (Dist_TBD_TubeFreqB)),
	h_distFreqLo(fhx (Dist_TBD_TubeFreqA)),
	s_distFreqHi(fsx (Dist_TBD_TubeFreqB)),
	s_distFreqLo(fsx (Dist_TBD_TubeFreqA)),
	l_distFreqHi(flx (Dist_TBD_TubeFreqB)),
	l_distFreqLo(flx (Dist_TBD_TubeFreqA)),
	l_pi(flx (M_PI)),
	s_pi(fsx (M_PI)),
	h_pi(fhx (M_PI)),
	h_tRatio(fhx (Dist_TBD_T_Ratio)),
	s_tRatio(fsx (h_tRatio)),
	l_tRatio(flx (s_tRatio))
{
	h_sv = fhx (stMod);
	s_sv = fsx (stMod);
	l_sv = flx (stMod);

	FilterA = new Cst_EQ();
	FilterB = new Cst_EQ();

	FilterA->SetSmpRate(cRateS);
	FilterA->SetBands(100.0, (fsx) s_distFreqLo);
	FilterB->SetSmpRate(cRateS);
	FilterB->SetBands(100.0, (fsx) s_distFreqHi);

	h_tSpeed = fhx (fhx (Dist_TBD_T_Speed) / cRateH);
	s_tSpeed = fsx (h_tSpeed);
	l_tSpeed = flx (h_tSpeed);

	clearBuffs();
}
Tube_BankD::~Tube_BankD()
{
	delete FilterA;
	delete FilterB;
}
void Tube_BankD::InitTubes()
{
	clearBuffs();
}
void Tube_BankD::setSampleRate(fsx nSRate)
{
	SampleBank_Cst::setSampleRate(nSRate);

	SampleBank_Cst::zeroAllSamples();

	clearBuffs();

	h_tSpeed = fhx (fhx (Dist_TBD_T_Speed) / cRateH);
	s_tSpeed = fsx (h_tSpeed);
	l_tSpeed = flx (h_tSpeed);

	FilterA->SetSmpRate(cRateS);
	FilterA->SetBands(100.0, (fsx) s_distFreqLo);
	FilterB->SetSmpRate(cRateS);
	FilterB->SetBands(100.0, (fsx) s_distFreqHi);
}
void Tube_BankD::SetGain(const fsx nGain)
{
	h_Gain = fhx (nGain);

	if(fhx (h_Gain) < fhx (rfV->zh)) h_Gain = fhx (rfV->zl);
	if(fhx (h_Gain) > fhx (1.0)) h_Gain = fhx (1.0);

	h_Gain *= fhx (2.0 - h_Gain);
	h_Gain *= fhx (Dist_TBD_Gain_Mult_Factor);

	s_Gain = fsx (h_Gain);
	l_Gain = flx (s_Gain);
}
void Tube_BankD::clearBuffs()
{
	h_dfltA = fhx (rfV->zh);
	h_dfltB = fhx (rfV->zh);
	s_dfltA = fsx (rfV->zs);
	s_dfltB = fsx (rfV->zs);
	l_dfltA = flx (rfV->zl);
	l_dfltB = flx (rfV->zl);

	for(int y = 0; y < Dist_TBD_PreAmp_Stages_Max; y++)
	{
		l_tPos[y] = flx (0.0f);
		s_tPos[y] = fsx (0.0);
		h_tPos[y] = fhx (0.0);

		for (int z = 0; z < Dist_TBD_TubeFreqStages; z++)
		{
			l_qFiltT[y][z] = flx (rfV->zl);
			s_qFiltT[y][z] = fsx (rfV->zs);
			h_qFiltT[y][z] = fhx (rfV->zh);
		}
	}
}
void Tube_BankD::zeroAllSamples()
{
	SampleBank_Cst::zeroAllSamples();
}
void Tube_BankD::setOverSampling(const int oSLevel)
{
	SampleBank_Cst::setOverSampling(oSLevel);

	clearBuffs();

	h_tSpeed = fhx (fhx (Dist_TBD_T_Speed) / cRateH);
	s_tSpeed = fsx (h_tSpeed);
	l_tSpeed = flx (h_tSpeed);

	FilterA->SetSmpRate(cRateS);
	FilterA->SetBands(100.0, (fsx) s_distFreqLo);
	FilterB->SetSmpRate(cRateS);
	FilterB->SetBands(100.0, (fsx) s_distFreqHi);
}
void Tube_BankD::ApplyDistortion()
{
	if(osRate <= 0) return;
	if(osRate >= 6) return;

	int numStages = 0;
	if(currChannel == 0) numStages = DIST_TBD_STAGES_CLEAN;
	if(currChannel == 1) numStages = DIST_TBD_STAGES_CRUNCH;
	if(currChannel == 2) numStages = DIST_TBD_STAGES_HIGH;
	if(currChannel == 3) numStages = DIST_TBD_STAGES_MEGA;
	if(numStages == 0) return;
	int preStages = Dist_TBD_PreAmp_Stages_BoostOff;
	if((currChannel == 1) || (currChannel == 3)) preStages = Dist_TBD_PreAmp_Stages_Max;
	if(numStages > DIST_TBD_MAX_STAGES) numStages = DIST_TBD_MAX_STAGES;

//! ----------------------------------------------------------------------------------------------------------------
//! 32-bit Processing
//! ----------------------------------------------------------------------------------------------------------------
	if(precisionLevel == 0)
	{
		const flx modFiltA = (flx) expf(flx (-2.0) * l_pi * flx (l_sv) / (cRateL));
		const flx modFiltB = flx (1.0f - modFiltA);
		const flx mDFAa = (flx) expf(flx (-2.0f) * l_pi * l_distFreqLo / cRateL);
		const flx mDFAb = flx (1.0f - mDFAa);
//!		Input Filtering
		for(int x = 0; x < rfV->vi[osRate]; x++)
		{
			smpl[x] = (flx) fminf(flx (1.0f), fmaxf(smpl[x], flx (-1.0f)));
			FilterA->SplInL(smpl[x]);
			FilterA->RunLPL();
			smpl[x] = (flx) FilterA->GetOutputL();
			smpl[x] = flx (smpl[x] - flx (l_dfltA = flx (smpl[x] * modFiltB + l_dfltA * modFiltA)));
		}
		for(int spNum = 0; spNum < rfV->vi[osRate]; spNum++)
		{
			l_smp = flx (smpl[spNum]);
//!		Pre-Amp
			for(int preAmpStg = 0; preAmpStg < preStages; preAmpStg++)
			{
				if (l_eff >= l_tPos[preAmpStg]) l_tPos[preAmpStg] += l_tSpeed;
					else l_tPos[preAmpStg] -= flx (l_tSpeed * l_tRatio);

				l_eff = (flx) fabsf(l_smp);
				const flx dFX = flx (l_Gain * (0.950f - (0.50f * l_tPos[preAmpStg])));
				l_smp *= flx ((1.0f + dFX) - (dFX * fabsf(l_smp)));
				l_smp *= flx ((1.0f + dFX) - (dFX * fabsf(l_smp)));

				for (int frStage = 0; frStage < Dist_TBD_TubeFreqStages; frStage++)
						l_smp = flx (l_qFiltT[preAmpStg][frStage] = flx (l_smp *
										mDFAb + l_qFiltT[preAmpStg][frStage] * mDFAa));
			}
//!		Rectify
			if (l_smp < flx (0.0f)) l_smp += (flx) sinf(flx (3.0f) * asinf(l_smp));
			l_smp *= flx (0.640f);
//!		Harmonics / Power-Amp
			for(int stg = 0; stg < numStages; stg++)
			{
				l_smp *= flx (0.2750f);
				const flx rSamp = (flx) acosf(l_smp);
				l_smp += flx (cosf(flx (3.0f) * rSamp) + cosf(flx (5.0f) * rSamp));
				l_smp *= flx (1.920f);
			}
			smpl[spNum] = flx (l_smp * flx (0.80f));
		}
//!		Output Filtering
		for(int x = 0; x < rfV->vi[osRate]; x++)
		{
			smpl[x] = (flx) fminf(flx (1.0f), fmaxf(smpl[x], flx (-1.0f)));
			FilterB->SplInL(smpl[x]);
			FilterB->RunLPL();
			smpl[x] = (flx) FilterB->GetOutputL();
			smpl[x] = flx (smpl[x] - flx (l_dfltB = flx (smpl[x] * modFiltB + l_dfltB * modFiltA)));
		}
	} else
//! ----------------------------------------------------------------------------------------------------------------
//! 64-bit Processing
//! ----------------------------------------------------------------------------------------------------------------
	if(precisionLevel == 1)
	{
		const fsx modFiltA = (fsx) exp(fsx (-2.0) * s_pi * fsx (s_sv) / (cRateS));
		const fsx modFiltB = fsx (1.0 - modFiltA);
		const fsx mDFAa = (fsx) exp(fsx (-2.0) * s_pi * s_distFreqLo / cRateS);
		const fsx mDFAb = fsx (1.0 - mDFAa);
//!		Input Filtering
		for(int x = 0; x < rfV->vi[osRate]; x++)
		{
			smps[x] = (fsx) fmin(fsx (1.0), fmax(smps[x], fsx (-1.0)));
			FilterA->SplInS(smps[x]);
			FilterA->RunLPS();
			smps[x] = (fsx) FilterA->GetOutputS();
			smps[x] = fsx (smps[x] - fsx (s_dfltA = fsx (smps[x] * modFiltB + s_dfltA * modFiltA)));
		}
		for(int spNum = 0; spNum < rfV->vi[osRate]; spNum++)
		{
			s_smp = fsx (smps[spNum]);
//!		Pre-Amp
			for(int preAmpStg = 0; preAmpStg < preStages; preAmpStg++)
			{
				if (s_eff >= s_tPos[preAmpStg]) s_tPos[preAmpStg] += s_tSpeed;
					else s_tPos[preAmpStg] -= fsx (s_tSpeed * s_tRatio);

				s_eff = (fsx) fabs(s_smp);
				const fsx dFX = fsx (s_Gain * (0.950 - (0.50 * s_tPos[preAmpStg])));
				s_smp *= fsx ((1.0 + dFX) - (dFX * fabs(s_smp)));
				s_smp *= fsx ((1.0 + dFX) - (dFX * fabs(s_smp)));

				for (int frStage = 0; frStage < Dist_TBD_TubeFreqStages; frStage++)
						s_smp = fsx (s_qFiltT[preAmpStg][frStage] = fsx (s_smp *
										mDFAb + s_qFiltT[preAmpStg][frStage] * mDFAa));
			}
//!		Rectify
			if (s_smp < fsx (0.0)) s_smp += (fsx) sin(fsx (3.0) * asin(s_smp));
			s_smp *= fsx (0.640);
//!		Harmonics / Power-Amp
			for(int stg = 0; stg < numStages; stg++)
			{
				s_smp *= fsx (0.2750);
				const fsx rSamp = (fsx) acos(s_smp);
				s_smp += fsx (cos(fsx (3.0) * rSamp) + cos(fsx (5.0) * rSamp));
				s_smp *= fsx (1.920);
			}
			smps[spNum] = fsx (s_smp * fsx (0.80));
		}
//!		Output Filtering
		for(int x = 0; x < rfV->vi[osRate]; x++)
		{
			smps[x] = (fsx) fmin(fsx (1.0), fmax(smps[x], fsx (-1.0)));
			FilterB->SplInS(smps[x]);
			FilterB->RunLPS();
			smps[x] = (fsx) FilterB->GetOutputS();
			smps[x] = fsx (smps[x] - fsx (s_dfltB = fsx (smps[x] * modFiltB + s_dfltB * modFiltA)));
		}
	} else
//! ----------------------------------------------------------------------------------------------------------------
//! 128-bit Processing
//! ----------------------------------------------------------------------------------------------------------------
	if(precisionLevel == 2)
	{
		const fhx modFiltA = (fhx) expl(fhx (-2.0) * h_pi * fhx (h_sv) / (cRateS));
		const fhx modFiltB = fhx (1.0 - modFiltA);
		const fhx mDFAa = (fhx) expl(fhx (-2.0) * h_pi * h_distFreqLo / cRateS);
		const fhx mDFAb = fhx (1.0 - mDFAa);
//!		Input Filtering
		for(int x = 0; x < rfV->vi[osRate]; x++)
		{
			smph[x] = (fhx) fminl(fhx (1.0), fmaxl(smph[x], fhx (-1.0)));
			FilterA->SplInH(smph[x]);
			FilterA->RunLPH();
			smph[x] = (fhx) FilterA->GetOutputH();
			smph[x] = fhx (smph[x] - fhx (h_dfltA = fhx (smph[x] * modFiltB + h_dfltA * modFiltA)));
		}
		for(int spNum = 0; spNum < rfV->vi[osRate]; spNum++)
		{
			h_smp = fhx (smph[spNum]);
//!		Pre-Amp
			for(int preAmpStg = 0; preAmpStg < preStages; preAmpStg++)
			{
				if (h_eff >= h_tPos[preAmpStg]) h_tPos[preAmpStg] += h_tSpeed;
					else h_tPos[preAmpStg] -= fhx (h_tSpeed * h_tRatio);

				h_eff = (fhx) fabsl(h_smp);
				const fhx dFX = fhx (h_Gain * (0.950 - (0.50 * h_tPos[preAmpStg])));
				h_smp *= fhx ((1.0 + dFX) - (dFX * fabsl(h_smp)));
				h_smp *= fhx ((1.0 + dFX) - (dFX * fabsl(h_smp)));

				for (int frStage = 0; frStage < Dist_TBD_TubeFreqStages; frStage++)
						h_smp = fhx (h_qFiltT[preAmpStg][frStage] = fhx (h_smp *
										mDFAb + h_qFiltT[preAmpStg][frStage] * mDFAa));
			}
//!		Rectify
			if (h_smp < fhx (0.0)) h_smp += (fhx) sinl(fhx (3.0) * asinl(h_smp));
			h_smp *= fhx (0.640);
//!		Harmonics / Power-Amp
			for(int stg = 0; stg < numStages; stg++)
			{
				h_smp *= fhx (0.2750);
				const fhx rSamp = (fhx) acosl(h_smp);
				h_smp += fhx (cosl(fhx (3.0) * rSamp) + cosl(fhx (5.0) * rSamp));
				h_smp *= fhx (1.920);
			}
			smph[spNum] = fhx (h_smp * fhx (0.80));
		}
//!		Output Filtering
		for(int x = 0; x < rfV->vi[osRate]; x++)
		{
			smph[x] = (fhx) fminl(fhx (1.0), fmaxl(smph[x], fhx (-1.0)));
			FilterB->SplInH(smph[x]);
			FilterB->RunLPH();
			smph[x] = (fhx) FilterB->GetOutputH();
			smph[x] = fhx (smph[x] - fhx (h_dfltB = fhx (smph[x] * modFiltB + h_dfltB * modFiltA)));
		}
	}
}
void Tube_BankD::SetChannel(const int nChan)
{
	if(currChannel == nChan) return;
	if(nChan < 0) return;
	if(nChan > 3) return;

	currChannel = nChan;

	zeroAllSamples();
}
void Tube_BankD::SetAmpVt(const fsx inp)
{
	return;
/*
//	const fhx ampl = fhx (inp * inp);
	const fhx ampl = (fhx) fabsl(inp);
//	const fhx ampl = (fhx) fabsl(fhx (inp * fhx (fhx (2.0) - (fhx) fabsl(inp))));
	if (ampl > mdV) mdV += fhx (fhx (192.0) / sRateH);
	if (ampl < mdV) mdV -= fhx (fhx (16.0) / sRateH);
	mdV = (fhx) fminl(fhx (1.0), fmaxl(fhx (0.0), mdV));
	l_md = flx (mdV);
	s_md = fsx (mdV);
	h_md = fhx (mdV);
*/
}
