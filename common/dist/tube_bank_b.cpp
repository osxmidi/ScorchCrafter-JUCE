#include "tube_bank_b.h"
// #include <cstdlib>

Tube_BankB::Tube_BankB(fsx spRate, fhx mdfr) : SampleBank_FP(spRate),
	stMod(fhx (fhx (mdfr) * fhx (0.010))),
	lmtH(fhx (0.50)),
	currChannel(0),
	cBackH(fhx (0.9750)),
	cBackS(fsx (cBackH)),
	cBackL(flx (cBackH)),
	distFreqHi(fhx (7000.0)),
	distFreqLo(fhx (6400.0)),
	fEnvRateFac(fhx (1.0)),
	bGain(fhx (0.50))
{
//	SampleBank_FP(fsx (spRate));

	FizzFiltL = new BTWorthLP_LD(fsx (7150.0), fsx (spRate));
	FizzFiltS = new BTWorthLP_SD(fsx (7150.0), fsx (spRate));
	FizzFiltH = new BTWorthLP_HD(fsx (7150.0), fsx (spRate));

	DrvFL = new BTWorthLP_LD(fsx (7200.0), fsx (spRate));
	DrvFS = new BTWorthLP_SD(fsx (7200.0), fsx (spRate));
	DrvFH = new BTWorthLP_HD(fsx (7200.0), fsx (spRate));

	gFacH[0] = fhx (DIST_TBb_ST_POWER_A);
	gFacH[1] = fhx (DIST_TBb_ST_POWER_B);
	gFacH[2] = fhx (DIST_TBb_ST_POWER_C);
	gFacH[3] = fhx (DIST_TBb_ST_POWER_D);

	mdH = fhx (stMod);
	mdS = fsx (stMod);
	mdL = flx (stMod);

	fltFreqH[0] = fhx ((fhx) 1.0 - (fhx) expl((fhx) -2.0 * (fhx) M_PI * (fhx) distFreqHi / fhx (cRateH)));
	fltFreqH[1] = fhx ((fhx) 1.0 - (fhx) expl((fhx) -2.0 * (fhx) M_PI * (fhx) distFreqLo / fhx (cRateH)));
	fltFreqS[0] = fsx (fltFreqH[0]);
	fltFreqS[1] = fsx (fltFreqH[1]);
	fltFreqL[0] = flx (fltFreqH[0]);
	fltFreqL[1] = flx (fltFreqH[1]);

	fShelfRateH = fhx (fEnvRateFac / sRateH);
	fShelfRateS = fsx (fShelfRateH);
	fShelfRateL = flx (fShelfRateH);

	fShelfPosH = fhx (0.0);
	fShelfPosS = fsx (0.0);
	fShelfPosL = flx (0.f);
}
Tube_BankB::~Tube_BankB()
{
	delete FizzFiltL;
	delete FizzFiltS;
	delete FizzFiltH;

	delete DrvFL;
	delete DrvFS;
	delete DrvFH;
}
void Tube_BankB::InitTubes()
{
	clearBuffs();
}
void Tube_BankB::setSampleRate(fsx nSRate)
{
	SampleBank_FP::setSampleRate(nSRate);

	SampleBank_FP::zeroAllSamples();

	FizzFiltL->setSampleRate(fsx (cRateS));
	FizzFiltS->setSampleRate(fsx (cRateS));
	FizzFiltH->setSampleRate(fsx (cRateS));

	DrvFL->setSampleRate(fsx (cRateS));
	DrvFS->setSampleRate(fsx (cRateS));
	DrvFH->setSampleRate(fsx (cRateS));

	clearBuffs();

	fltFreqH[0] = fhx ((fhx) 1.0 - (fhx) expl((fhx) -2.0 * (fhx) M_PI * (fhx) distFreqHi / fhx (cRateH)));
	fltFreqH[1] = fhx ((fhx) 1.0 - (fhx) expl((fhx) -2.0 * (fhx) M_PI * (fhx) distFreqLo / fhx (cRateH)));
	fltFreqS[0] = fsx (fltFreqH[0]);
	fltFreqS[1] = fsx (fltFreqH[1]);
	fltFreqL[0] = flx (fltFreqH[0]);
	fltFreqL[1] = flx (fltFreqH[1]);

	fShelfRateH = fhx (fEnvRateFac / sRateH);
	fShelfRateS = fsx (fShelfRateH);
	fShelfRateL = flx (fShelfRateH);
}
void Tube_BankB::SetGain(const fsx nGain)
{
	h_Gain = fhx (nGain);

	if(fhx (h_Gain) < fhx (rfV->zh)) h_Gain = fhx (rfV->zh);
	if(fhx (h_Gain) > fhx (1.0)) h_Gain = fhx (1.0);

	bGain = fhx (h_Gain);
	h_Gain *= fhx (gFacH[currChannel]);

	s_Gain = fsx (h_Gain);
	l_Gain = flx (h_Gain);
}
void Tube_BankB::clearBuffs()
{
	for(int y = 0; y < DIST_TBa_MAX_STAGES; y++)
	{
		fltPosH[y][0] = fhx (0.0);
		fltPosS[y][0] = fsx (0.0);
		fltPosL[y][0] = flx (0.0f);

		fltPosH[y][1] = fhx (0.0);
		fltPosS[y][1] = fsx (0.0);
		fltPosL[y][1] = flx (0.0f);

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
void Tube_BankB::zeroAllSamples()
{
	SampleBank_FP::zeroAllSamples();
}
void Tube_BankB::setOverSampling(const int oSLevel)
{
	SampleBank_FP::setOverSampling(oSLevel);

	FizzFiltL->setSampleRate(fsx (cRateS));
	FizzFiltS->setSampleRate(fsx (cRateS));
	FizzFiltH->setSampleRate(fsx (cRateS));

	clearBuffs();

	fltFreqH[0] = fhx ((fhx) 1.0 - (fhx) expl((fhx) -2.0 * (fhx) M_PI * (fhx) distFreqHi / fhx (cRateH)));
	fltFreqH[1] = fhx ((fhx) 1.0 - (fhx) expl((fhx) -2.0 * (fhx) M_PI * (fhx) distFreqLo / fhx (cRateH)));
	fltFreqS[0] = fsx (fltFreqH[0]);
	fltFreqS[1] = fsx (fltFreqH[1]);
	fltFreqL[0] = flx (fltFreqH[0]);
	fltFreqL[1] = flx (fltFreqH[1]);

	fShelfRateH = fhx (fEnvRateFac / sRateH);
	fShelfRateS = fsx (fShelfRateH);
	fShelfRateL = flx (fShelfRateH);
}
void Tube_BankB::ApplyDistortion()
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
		const flx modFiltA = flx (mdL * flx (flx (1.0f) + (flx) mdV) / rfV->vl[osRate]);
		const flx modFiltB = flx (flx (1.0f) - modFiltA);

		for(unsigned int fzFilt = 0; fzFilt < rfV->vi[osRate]; fzFilt++)
		{
			if(flx (smpl[fzFilt]) == flx (0.0f)) smpl[fzFilt] = flx (rfV->zl);
			smpl[fzFilt] = (flx) FizzFiltL->runFilterL(flx (smpl[fzFilt]));
			smpl[fzFilt] = (flx) fminf(flx (1.0f), fmaxf(smpl[fzFilt], flx (-1.0f)));
		}
		for(unsigned int stg = 0; stg < numStages; stg++)
		{
			for(unsigned int spNum = 0; spNum < rfV->vi[osRate]; spNum++)
			{
				l_smp = flx (smpl[spNum]);

				l_smp = (flx) fminf(flx (1.0f), fmaxf(l_smp, flx (-1.0f)));

				if(flx (l_smp) == flx (0.0f))
				{
					l_smp = flx (rfV->zl);
				} else
				{
					l_smp *= flx (0.990f);
					flx oldSpl = flx (l_smp);

					l_smp = flx (l_smp - flx (l_dfltA[stg] = flx (l_smp * modFiltA + l_dfltA[stg] * modFiltB)));
					const flx mg = (flx) powf(flx (2.0f), flx (l_smp * l_Gain));
					l_smp = flx (flx (mg - flx (flx (1.0f) + l_smp)) / flx (mg + flx (flx (1.0f) + flx (l_smp))));
					l_smp = flx (fltPosL[stg][1] += flx (flx ((flx) l_smp - (flx) fltPosL[stg][1]) * flx (fltFreqL[1])));
					l_smp = flx (l_smp - flx (l_dfltB[stg] = flx (l_smp * modFiltA + l_dfltB[stg] * modFiltB)));
					l_smp += flx (oldSpl);
					l_smp *= flx (0.50f);
					l_smp = flx (fltPosL[stg][0] += flx (flx ((flx) l_smp - (flx) fltPosL[stg][0]) * flx (fltFreqL[0])));
					l_smp *= flx (0.50f);
				}
				smpl[spNum] = flx (l_smp);
				smpl[spNum] *= cBackL;
			}
		}
		return;
	}
	if(precisionLevel == 1)
	{
		const fsx modFiltA = fsx (mdS * fsx (fsx (1.0) + mdV) / rfV->vs[osRate]);
		const fsx modFiltB = fsx (fsx (1.0) - modFiltA);

		for(unsigned int fzFilt = 0; fzFilt < rfV->vi[osRate]; fzFilt++)
		{
			if(fsx (smps[fzFilt]) == fsx (0.0)) smps[fzFilt] = fsx (rfV->zs);
			smps[fzFilt] = (fsx) FizzFiltS->runFilterS(fsx (smps[fzFilt]));
			smps[fzFilt] = (fsx) fmin(fsx (1.0), fmax(smps[fzFilt], fsx (-1.0)));
		}
		for(unsigned int stg = 0; stg < numStages; stg++)
		{
			for(unsigned int spNum = 0; spNum < rfV->vi[osRate]; spNum++)
			{
				s_smp = fsx (smps[spNum]);

				s_smp = (fsx) fmin(fsx (1.0), fmax(s_smp, fsx (-1.0)));

				if(fsx (s_smp) == fsx (0.0))
				{
					s_smp = fsx (rfV->zs);
				} else
				{
					s_smp *= fsx (0.990);
					fsx oldSpl = fsx (s_smp);

					s_smp = fsx (s_smp - fsx (s_dfltA[stg] = fsx (s_smp * modFiltA + s_dfltA[stg] * modFiltB)));
					const fsx mg = (fsx) pow(fsx (2.0), fsx (s_smp * s_Gain));
					s_smp = fsx (fsx (mg - fsx (fsx (1.0) + s_smp)) / fsx (mg + fsx (fsx (1.0) + fsx (s_smp))));
					s_smp = fsx (fltPosS[stg][1] += fsx (fsx ((fsx) s_smp - (fsx) fltPosS[stg][1]) * fsx (fltFreqS[1])));
					s_smp = fsx (s_smp - fsx (s_dfltB[stg] = fsx (s_smp * modFiltA + s_dfltB[stg] * modFiltB)));
					s_smp += fsx (oldSpl);
					s_smp *= fsx (0.50);
					s_smp = fsx (fltPosS[stg][0] += fsx (fsx ((fsx) s_smp - (fsx) fltPosS[stg][0]) * fsx (fltFreqS[0])));
					s_smp *= fsx (0.50);
				}
				smps[spNum] = fsx (s_smp);
				smps[spNum] *= cBackS;
			}
		}
		return;
	}
	if(precisionLevel == 2)
	{
		const fhx modFiltA = fhx (mdH * fhx (fhx (1.0) + mdV) / rfV->vh[osRate]);
		const fhx modFiltB = fhx (fhx (1.0) - modFiltA);

		for(unsigned int fzFilt = 0; fzFilt < rfV->vi[osRate]; fzFilt++)
		{
			if(fhx (smph[fzFilt]) == fhx (0.0)) smph[fzFilt] = fhx (rfV->zh);
			smph[fzFilt] = (fhx) FizzFiltH->runFilterH(fhx (smph[fzFilt]));
			smph[fzFilt] = (fhx) fminl(fhx (1.0), fmaxl(smph[fzFilt], fhx (-1.0)));
		}
		for(unsigned int stg = 0; stg < numStages; stg++)
		{
			for(unsigned int spNum = 0; spNum < rfV->vi[osRate]; spNum++)
			{
				h_smp = fhx (smph[spNum]);

				h_smp = (fhx) fminl(fhx (1.0), fmaxl(h_smp, fhx (-1.0)));

				if(fhx (h_smp) == fhx (0.0))
				{
					h_smp = fhx (rfV->zh);
				} else
				{
					h_smp *= fhx (0.990);
					fhx oldSpl = fhx (h_smp);

					h_smp = fhx (h_smp - fhx (h_dfltA[stg] = fhx (h_smp * modFiltA + h_dfltA[stg] * modFiltB)));
					const fhx mg = (fhx) powl(fhx (2.0), fhx (h_smp * h_Gain));
					h_smp = fhx (fhx (mg - fhx (fhx (1.0) + h_smp)) / fhx (mg + fhx (fhx (1.0) + fhx (h_smp))));
					h_smp = fhx (fltPosH[stg][1] += fhx (fhx ((fhx) h_smp - (fhx) fltPosH[stg][1]) * fhx (fltFreqH[1])));
					h_smp = fhx (h_smp - fhx (h_dfltB[stg] = fhx (h_smp * modFiltA + h_dfltB[stg] * modFiltB)));
					h_smp += fhx (oldSpl);
					h_smp *= fhx (0.50);
					h_smp = fhx (fltPosH[stg][0] += fhx (fhx ((fhx) h_smp - (fhx) fltPosH[stg][0]) * fhx (fltFreqH[0])));
					h_smp *= fhx (0.50);
				}
				smph[spNum] = fhx (h_smp);
				smph[spNum] *= cBackH;
			}
		}
		return;
	}
}
void Tube_BankB::SetChannel(const int nChan)
{
	if(currChannel == nChan) return;
	if(nChan < 0) return;
	if(nChan > 3) return;

	h_Gain = fhx (gFacH[nChan]);
	currChannel = nChan;
	h_Gain *= fhx (bGain);
	s_Gain = fsx (h_Gain);
	l_Gain = flx (h_Gain);

	zeroAllSamples();
}
void Tube_BankB::SetAmpVt(const fsx inp)
{
	const fhx ampl = (fhx) fabsl(fhx (inp * fhx (fhx (2.0) - (fhx) fabsl(inp))));
	// const fhx ampl = (fhx) fabsl(inp);
	if (ampl > mdV) mdV += fhx (fhx (16.0) / sRateH);
	if (ampl < mdV) mdV -= fhx (fhx (16.0) / sRateH);
	mdV = (fhx) fminl(fhx (1.0), fmaxl(fhx (0.0), mdV));
}
