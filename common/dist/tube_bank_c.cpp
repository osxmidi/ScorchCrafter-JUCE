#include "tube_bank_c.h"

Tube_BankC::Tube_BankC(fsx spRate, fhx mdfr) : SampleBank_FP(spRate),
	stMod(fhx (fhx (mdfr) * fhx (0.010))),
	lmtH(fhx (0.50)),
	lmtS(fsx (0.50)),
	lmtL(flx (0.50f)),
	currChannel(0),
	cBackH(fhx (0.498750)),
	cBackS(fsx (cBackH)),
	cBackL(flx (cBackH)),
	distFreqHi(fhx (3600.0)),
	distFreqLo(fhx (7200.0)),
	fEnvRateFac(fhx (1.0)),
	bGain(fhx (0.50))
{
	gFacH[0] = fhx (DIST_TBc_ST_POWER_A);
	gFacH[1] = fhx (DIST_TBc_ST_POWER_B);
	gFacH[2] = fhx (DIST_TBc_ST_POWER_C);
	gFacH[3] = fhx (DIST_TBc_ST_POWER_D);

	mdH = fhx (stMod);
	mdS = fsx (stMod);
	mdL = flx (stMod);

	for (unsigned int fx = 0; fx < DIST_TBc_MAX_STAGES; fx++)
	{
		FilterA[fx] = new Cst_EQ();
		FilterB[fx] = new Cst_EQ();

		FilterA[fx]->SetSmpRate(cRateS);
		FilterA[fx]->SetBands(1000.0, (fsx) distFreqLo);
		FilterB[fx]->SetSmpRate(cRateS);
		FilterB[fx]->SetBands(1000.0, (fsx) distFreqHi);
	}

	fShelfRateH = fhx (fEnvRateFac / sRateH);
	fShelfRateS = fsx (fShelfRateH);
	fShelfRateL = flx (fShelfRateH);

	fShelfPosH = fhx (0.0);
	fShelfPosS = fsx (0.0);
	fShelfPosL = flx (0.f);
}
Tube_BankC::~Tube_BankC()
{
	for (unsigned int fx = 0; fx < DIST_TBc_MAX_STAGES; fx++)
	{
		delete FilterA[fx];
		delete FilterB[fx];
	}
}
void Tube_BankC::InitTubes()
{
	clearBuffs();
}
void Tube_BankC::setSampleRate(fsx nSRate)
{
	SampleBank_FP::setSampleRate(nSRate);

	SampleBank_FP::zeroAllSamples();

	clearBuffs();

	for (unsigned int fx = 0; fx < DIST_TBc_MAX_STAGES; fx++)
	{
		FilterA[fx]->SetSmpRate(cRateS);
		FilterA[fx]->SetBands(100.0, (fsx) distFreqLo);
		FilterB[fx]->SetSmpRate(cRateS);
		FilterB[fx]->SetBands(100.0, (fsx) distFreqHi);
	}

	fShelfRateH = fhx (fEnvRateFac / sRateH);
	fShelfRateS = fsx (fShelfRateH);
	fShelfRateL = flx (fShelfRateH);
}
void Tube_BankC::SetGain(const fsx nGain)
{
	h_Gain = fhx (nGain);

	if(fhx (h_Gain) < fhx (rfV->zh)) h_Gain = fhx (rfV->zh);
	if(fhx (h_Gain) > fhx (1.0)) h_Gain = fhx (1.0);

	h_Gain *= fhx (fhx (2.0) - h_Gain);

	bGain = fhx (h_Gain);
//	h_Gain *= fhx (gFacH[currChannel]);

//	if(fhx (h_Gain) < fhx (1.0)) h_Gain = fhx (1.0);

	s_Gain = fsx (h_Gain);
	l_Gain = flx (h_Gain);
}
void Tube_BankC::clearBuffs()
{
	for(int y = 0; y < DIST_TBc_MAX_STAGES; y++)
	{
		h_dfltA[y] = fhx (rfV->zh);
		h_dfltB[y] = fhx (rfV->zh);
		s_dfltA[y] = fhx (rfV->zs);
		s_dfltB[y] = fhx (rfV->zs);
		l_dfltA[y] = fhx (rfV->zl);
		l_dfltB[y] = fhx (rfV->zl);
	}

	fShelfPosH = fhx (0.0);
	fShelfPosS = fsx (0.0);
	fShelfPosL = flx (0.f);
}
void Tube_BankC::zeroAllSamples()
{
	SampleBank_FP::zeroAllSamples();
}
void Tube_BankC::setOverSampling(const int oSLevel)
{
	SampleBank_FP::setOverSampling(oSLevel);

	clearBuffs();

	for (unsigned int fx = 0; fx < DIST_TBc_MAX_STAGES; fx++)
	{
		FilterA[fx]->SetSmpRate(cRateS);
		FilterA[fx]->SetBands(1000.0, (fsx) distFreqLo);
		FilterB[fx]->SetSmpRate(cRateS);
		FilterB[fx]->SetBands(1000.0, (fsx) distFreqHi);
	}

	fShelfRateH = fhx (fEnvRateFac / sRateH);
	fShelfRateS = fsx (fShelfRateH);
	fShelfRateL = flx (fShelfRateH);
}
void Tube_BankC::ApplyDistortion()
{
	if(osRate <= 0) return;
	if(osRate >= 6) return;

	unsigned int numStages = 0;
	if(currChannel == 0) numStages = DIST_TBc_STAGES_CLEAN;
	if(currChannel == 1) numStages = DIST_TBc_STAGES_CRUNCH;
	if(currChannel == 2) numStages = DIST_TBc_STAGES_HIGH;
	if(currChannel == 3) numStages = DIST_TBc_STAGES_MEGA;
	if(numStages == 0) return;
	if(numStages > DIST_TBc_MAX_STAGES) numStages = DIST_TBc_MAX_STAGES;

	if(precisionLevel == 0)
	{
		const flx modFiltA = flx (mdL * flx (flx (0.10f) + (flx) mdV) / rfV->vl[osRate]);
		const flx modFiltB = flx (flx (1.0f) - modFiltA);

		for(unsigned int stg = 0; stg < numStages; stg++)
		{
			for(unsigned int spNum = 0; spNum < rfV->vi[osRate]; spNum++)
			{
				l_smp = flx (smpl[spNum]);
				l_smp = (flx) fminf(flx (1.0f), fmaxf(l_smp, flx (-1.0f)));
				l_smp *= flx (0.999750f);
				l_smp = flx (l_smp - flx (l_dfltA[stg] = flx (l_smp * modFiltA + l_dfltA[stg] * modFiltB)));
				l_smp *= flx (0.90f);

				for (unsigned int dStage = 0; dStage < Dist_TBC_Num_SubStages; dStage++)
				{
					sSplitl[0] = flx (l_smp);
					sSplitl[1] = flx (flx (flx (4.0f)*l_smp*l_smp*l_smp) - flx (flx (3.0f)*l_smp));
					sSplitl[2] = flx (flx (flx (16.0f)*l_smp*l_smp*l_smp*l_smp*l_smp) - flx (flx (20.0f)*l_smp*l_smp*l_smp) + flx (flx (5.0f)*l_smp));

					sSplitl[0] *= flx (flx (flx (1.0f) + l_Gain) - flx (l_Gain * fabsf(sSplitl[0])));
					sSplitl[1] *= flx (flx (flx (1.0f) + l_Gain) - flx (l_Gain * fabsf(sSplitl[1])));
					sSplitl[2] *= flx (flx (flx (1.0f) + l_Gain) - flx (l_Gain * fabsf(sSplitl[2])));

					sSplitl[0] *= flx (Dist_TBc_Harmonic_Dist_A);
					sSplitl[1] *= flx (Dist_TBc_Harmonic_Dist_B);
					sSplitl[2] *= flx (Dist_TBc_Harmonic_Dist_C);

					l_smp = flx (sSplitl[0] + flx (sSplitl[1] + sSplitl[2]));
					l_smp *= flx (Dist_TBC_Harmonic_CutBack);
				}

				l_smp = (flx) fminf(flx (1.0f), fmaxf(l_smp, flx (-1.0f)));
				FilterB[stg]->SplInL(l_smp);
				FilterB[stg]->RunLPL();
				l_smp = FilterB[stg]->GetOutputL();

				smpl[spNum] += flx (l_smp);
				smpl[spNum] *= cBackL;
			}
		}
		return;
	}
	if(precisionLevel == 1)
	{
		const fsx modFiltA = (fsx) exp(fsx (-2.0) * fsx (M_PI) * fsx (120.0) / cRateS);
		const fsx modFiltB = fsx (fsx (1.0) - modFiltA);

		for(unsigned int stg = 0; stg < numStages; stg++)
		{
			for(unsigned int spNum = 0; spNum < rfV->vi[osRate]; spNum++)
			{
				s_smp = fsx (smps[spNum]);

		//		FilterA[stg]->SplInS(s_smp);
		//		FilterA[stg]->RunLPS();
		//		s_smp = FilterA[stg]->GetOutputS();

				s_smp = (fsx) fmin(fsx (1.0), fmax(s_smp, fsx (-1.0)));

				s_smp *= fsx (0.999750);
		//		const fsx oldSpl = fsx (s_smp * fsx (0.50));

			//	s_smp = fsx (s_smp - fsx (s_dfltA[stg] = fsx (s_smp * modFiltA + s_dfltA[stg] * modFiltB)));
			//	s_smp *= fsx (0.90);

//				for (unsigned int dStage = 0; dStage < Dist_TBC_Num_SubStages; dStage++)
//				{
					sSplits[0] = fsx (s_smp);
					sSplits[1] = fsx (fsx (fsx (4.0)*s_smp*s_smp*s_smp) - fsx (fsx (3.0)*s_smp));
					sSplits[2] = fsx (fsx (fsx (16.0)*s_smp*s_smp*s_smp*s_smp*s_smp) - fsx (fsx (20.0)*s_smp*s_smp*s_smp) + fsx (fsx (5.0)*s_smp));
/*
					sSplits[0] *= fsx (fsx (fsx (1.0) + s_Gain) - fsx (s_Gain * fabs(sSplits[0])));
					sSplits[1] *= fsx (fsx (fsx (1.0) + s_Gain) - fsx (s_Gain * fabs(sSplits[1])));
					sSplits[2] *= fsx (fsx (fsx (1.0) + s_Gain) - fsx (s_Gain * fabs(sSplits[2])));

					sSplits[0] *= fsx (Dist_TBc_Harmonic_Dist_A);
					sSplits[1] *= fsx (Dist_TBc_Harmonic_Dist_B);
					sSplits[2] *= fsx (Dist_TBc_Harmonic_Dist_C);

					s_smp = fsx (sSplits[0] + fsx (sSplits[1] + sSplits[2]));
					s_smp *= fsx (Dist_TBC_Harmonic_CutBack);
*/
					s_smp = fsx (sSplits[0] + fsx (sSplits[2] - sSplits[1]));
					s_smp *= fsx (0.250);
					fsx tmpVR = fsx (s_smp - fsx (s_dfltB[stg] = fsx (s_smp * modFiltB + s_dfltB[stg] * modFiltA)));
					s_smp += fsx (s_Gain * fsx (4.0) * s_smp * fsx (fsx (1.0) - fabs(tmpVR)));
					s_smp *= fsx (0.20);
//				}
/*
				fsx mg = (fsx) pow(fsx (2.0) - fabs(sSplits[0]), fsx (sSplits[0] * s_Gain * fsx (Dist_TBc_Harmonic_Dist_A)));
				sSplits[0] = fsx (fsx (mg - fsx (fsx (1.0) + sSplits[0])) / fsx (mg + fsx (fsx (1.0) + fsx (sSplits[0]))));
				mg = (fsx) pow(fsx (2.0) - fabs(sSplits[1]), fsx (sSplits[1] * s_Gain * fsx (Dist_TBc_Harmonic_Dist_B)));
				sSplits[1] = fsx (fsx (mg - fsx (fsx (1.0) + sSplits[1])) / fsx (mg + fsx (fsx (1.0) + fsx (sSplits[1]))));
				mg = (fsx) pow(fsx (2.0) - fabs(sSplits[2]), fsx (sSplits[2] * s_Gain * fsx (Dist_TBc_Harmonic_Dist_C)));
				sSplits[2] = fsx (fsx (mg - fsx (fsx (1.0) + sSplits[2])) / fsx (mg + fsx (fsx (1.0) + fsx (sSplits[2]))));
*/
				s_smp = (fsx) fmin(fsx (1.0), fmax(s_smp, fsx (-1.0)));

				FilterB[stg]->SplInS(s_smp);
				FilterB[stg]->RunLPS();
				s_smp = FilterB[stg]->GetOutputS();

		//		s_smp = fsx (s_smp - fsx (s_dfltB[stg] = fsx (s_smp * modFiltA + s_dfltB[stg] * modFiltB)));
		//		s_smp += fsx (oldSpl);
		//		s_smp *= fsx (0.50);

		//		FilterB[stg]->SplInS(oldSpl);
		//		FilterB[stg]->RunHPS();
		//		s_smp += FilterB[stg]->GetOutputS();
		//		s_smp *= fsx (0.50);

				smps[spNum] = fsx (s_smp);
				smps[spNum] *= fsx (0.950);
		//		smps[spNum] *= cBackS;
			}
		}
		return;
	}
	if(precisionLevel == 2)
	{
		const fhx modFiltA = fhx (mdH * fhx (fhx (0.10) + (fhx) mdV) / rfV->vh[osRate]);
		const fhx modFiltB = fhx (fhx (1.0) - modFiltA);

		for(unsigned int stg = 0; stg < numStages; stg++)
		{
			for(unsigned int spNum = 0; spNum < rfV->vi[osRate]; spNum++)
			{
				h_smp = fhx (smph[spNum]);
				h_smp = (fhx) fminl(fhx (1.0), fmaxl(h_smp, fhx (-1.0)));
				h_smp *= fhx (0.999750);
				h_smp = fhx (h_smp - fhx (h_dfltA[stg] = fhx (h_smp * modFiltA + h_dfltA[stg] * modFiltB)));
				h_smp *= fhx (0.90);

				for (unsigned int dStage = 0; dStage < Dist_TBC_Num_SubStages; dStage++)
				{
					sSplith[0] = fhx (h_smp);
					sSplith[1] = fhx (fhx (fhx (4.0)*h_smp*h_smp*h_smp) - fhx (fhx (3.0)*h_smp));
					sSplith[2] = fhx (fhx (fhx (16.0)*h_smp*h_smp*h_smp*h_smp*h_smp) - fhx (fhx (20.0)*h_smp*h_smp*h_smp) + fhx (fhx (5.0)*h_smp));

					sSplith[0] *= fhx (fhx (fhx (1.0) + h_Gain) - fhx (h_Gain * fabsl(sSplith[0])));
					sSplith[1] *= fhx (fhx (fhx (1.0) + h_Gain) - fhx (h_Gain * fabsl(sSplith[1])));
					sSplith[2] *= fhx (fhx (fhx (1.0) + h_Gain) - fhx (h_Gain * fabsl(sSplith[2])));

					sSplith[0] *= fhx (Dist_TBc_Harmonic_Dist_A);
					sSplith[1] *= fhx (Dist_TBc_Harmonic_Dist_B);
					sSplith[2] *= fhx (Dist_TBc_Harmonic_Dist_C);

					h_smp = fhx (sSplith[0] + fhx (sSplith[1] + sSplith[2]));
					h_smp *= fhx (Dist_TBC_Harmonic_CutBack);
				}

				h_smp = (fhx) fminl(fhx (1.0), fmaxl(h_smp, fhx (-1.0)));
				FilterB[stg]->SplInH(h_smp);
				FilterB[stg]->RunLPH();
				h_smp = FilterB[stg]->GetOutputH();

				smph[spNum] += fhx (h_smp);
				smph[spNum] *= cBackH;
			}
		}
		return;
	}
}
void Tube_BankC::SetChannel(const int nChan)
{
	if(currChannel == nChan) return;
	if(nChan < 0) return;
	if(nChan > 3) return;

//	h_Gain = fhx (gFacH[nChan]);
	currChannel = nChan;
//	h_Gain *= fhx (bGain);
//	s_Gain = fsx (h_Gain);
//	l_Gain = flx (h_Gain);

	zeroAllSamples();
}
void Tube_BankC::SetAmpVt(const fsx inp)
{
	const fhx ampl = (fhx) fabsl(fhx (inp * fhx (fhx (2.0) - (fhx) fabsl(inp))));
	if (ampl > mdV) mdV += fhx (fhx (32.0) / sRateH);
	if (ampl < mdV) mdV -= fhx (fhx (16.0) / sRateH);
	mdV = (fhx) fminl(fhx (1.0), fmaxl(fhx (0.0), mdV));
}
