#include "tonestack_d.h"
#include "tstack_wav_c.h"
#include <cstdlib>
#include <algorithm>

const fhx ToneStack_D::qBandRange_Low[2] = {SCr_C120_TStack_EqBand_Start_Low, SCr_C120_TStack_EqBand_End_Low};
const fhx ToneStack_D::qBandRange_Mid[2] = {SCr_C120_TStack_EqBand_Start_Mid, SCr_C120_TStack_EqBand_End_Mid};
const fhx ToneStack_D::qBandRange_High[2] = {SCr_C120_TStack_EqBand_Start_High, SCr_C120_TStack_EqBand_End_High};
const fhx ToneStack_D::qBandRange_ContourL[2] = {SCr_C120_TStack_EqBand_Start_CntrA, SCr_C120_TStack_EqBand_End_CntrA};
const fhx ToneStack_D::qBandRange_ContourH[2] = {SCr_C120_TStack_EqBand_Start_CntrB, SCr_C120_TStack_EqBand_End_CntrB};
const fhx ToneStack_D::qBandRange_Presence[2] = {SCr_C120_TStack_EqBand_Start_Pres, SCr_C120_TStack_EqBand_End_Pres};

ToneStack_D::ToneStack_D(fsx nRate, fsx CnvShift, fsx dcSkew) :
	sRate(fhx (nRate)),
	setShaped(true),
	fpPrec(int (1)),
	spLength(0),
	h_envPos(fhx (0.0)),
	s_envPos(fsx (0.0)),
	l_envPos(flx (0.f)),
	CabMic(false),
	cShiftPcnt(fsx (CnvShift)),
	cSkewPcnt(fsx (dcSkew)),
	currImpShift(0)
{
	s_bValue[0] = fsx (0.50);
	s_bValue[1] = fsx (0.50);
	s_bValue[2] = fsx (0.50);
	s_bValue[3] = fsx (0.50);
	s_bValue[4] = fsx (0.50);
	s_bValue[5] = fsx (0.50);

	rfV = new RateFC();

	for(int x = 0; x < 6; x++)
	{
		l_bValue[x] = flx (s_bValue[x]);
		h_bValue[x] = fhx (s_bValue[x]);

		qBand[x] = new Cst_EQ();
		qBand[x]->SetSmpRate(sRate);
	}

	DynBass = new Cst_EQ();
	DynBass->SetSmpRate(sRate);

	qBand[0]->SetBands(qBandRange_Low[0], qBandRange_Low[1]);
	qBand[1]->SetBands(qBandRange_Mid[0], qBandRange_Mid[1]);
	qBand[2]->SetBands(qBandRange_High[0], qBandRange_High[1]);
	qBand[3]->SetBands(qBandRange_ContourL[0], qBandRange_ContourL[1]);
	qBand[4]->SetBands(qBandRange_ContourH[0], qBandRange_ContourH[1]);
	qBand[5]->SetBands(qBandRange_Presence[0], qBandRange_Presence[1]);

	DynBass->SetBands(60.0, 215.0);

	InitToneImpulses();

	ClearBuffs();
}
ToneStack_D::~ToneStack_D()
{
	for (int x = 0; x < SC_NUM_TSTACK_D_IMPULSES; x++)
	{
		impLength[x] = 0;

		delete [] impulseL[x];
		delete [] impulseS[x];
		delete [] impulseH[x];
	}
	delete [] impulseL;
	delete [] impulseS;
	delete [] impulseH;

	delete [] smpH;
	delete [] smpS;
	delete [] smpL;

	for (int qb = 0; qb < 6; qb++)
	{
		delete qBand[qb];
	}

	delete DynBass;
	delete rfV;
}
void ToneStack_D::setNewSampleRate(const fsx nRate)
{
	sRate = fhx (nRate);

	spLength = 0;

	for (int x = 0; x < SC_NUM_TSTACK_D_IMPULSES; x++)
	{
		impLength[x] = 0;

		delete [] impulseL[x];
		delete [] impulseS[x];
		delete [] impulseH[x];
	}
	delete [] impulseL;
	delete [] impulseS;
	delete [] impulseH;

	delete [] smpH;
	delete [] smpS;
	delete [] smpL;

	InitToneImpulses();

	for (int x = 0; x < 6; x++)
	{
		qBand[x]->SetSmpRate(sRate);
	}

	DynBass->SetSmpRate(sRate);

	ClearBuffs();
}
void ToneStack_D::setBandValue(const int bNum, fsx value)
{
	if(bNum < 0) return;
	if(bNum > 5) return;

	l_bValue[bNum] = flx (value);
	s_bValue[bNum] = fsx (value);
	h_bValue[bNum] = fhx (value);
}
void ToneStack_D::setEvenShaped(const bool isShaped)
{
	if(setShaped != isShaped) ClearBuffs();
	setShaped = isShaped;
}
fsx ToneStack_D::GetProcessedOutput()
{
	if(fpPrec == 0) return fsx (l_spl);
	if(fpPrec == 1) return fsx (s_spl);
	if(fpPrec == 2) return fsx (h_spl);

	return fsx (0.0);
}
void ToneStack_D::ClearBuffs()
{
	h_envPos = fhx (0.0);
	s_envPos = fsx (0.0);
	l_envPos = flx (0.0);

	for (int x = 0; x < 6; x++)
	{
		qBand[x]->ClearBuffers();
	}
}
void ToneStack_D::SetInput(const fsx inp)
{
	h_spl = fhx (inp);
	s_spl = fsx (inp);
	l_spl = flx (inp);
}
void ToneStack_D::SetPrecision(int nLevel)
{
	if(nLevel < 0) nLevel = 0;
	if(nLevel > 2) nLevel = 2;

	fpPrec = nLevel;
}
void ToneStack_D::RunEQ()
{
	if(fpPrec == 0)
	{
		l_spl = (flx) fminf(flx (1.0f), fmaxf(l_spl, flx (-1.0f)));

		for (unsigned int x = 0; x < 6; x++)
		{
			qBand[x]->SplInL(l_spl);
			qBand[x]->RunQL();
		}
		for (unsigned int x = 0; x < 6; x++)
			l_spl += flx (qBand[x]->GetOutputL() * flx ((l_bValue[x] * l_bValue[x]) - flx (0.25f)) * flx (4.0f));

//		l_spl *= flx (0.80f);
//		if (setShaped)
//		{
//			DynBass->SplInL(l_spl);
//			DynBass->RunQL();
//			l_spl += flx (DynBass->GetOutputL() * (1.0f - (l_envPos * 2.0f)));
//		}
	} else
	if(fpPrec == 1)
	{
		s_spl = (fsx) fmin(fsx (1.0), fmax(s_spl, fsx (-1.0)));

		for (unsigned int x = 0; x < 6; x++)
		{
			qBand[x]->SplInS(s_spl);
			qBand[x]->RunQS();
		}
		for (unsigned int x = 0; x < 6; x++)
			s_spl += fsx (qBand[x]->GetOutputS() * fsx ((s_bValue[x] * s_bValue[x]) - fsx (0.25)) * fsx (4.0));

//		s_spl *= fsx (0.80);
//		if (setShaped)
//		{
//			DynBass->SplInS(s_spl);
//			DynBass->RunQS();
//			s_spl += fsx (DynBass->GetOutputS() * (1.0 - (s_envPos * 2.0)));
//		}
	} else
	if(fpPrec == 2)
	{
		h_spl = (fhx) fminl(fhx (1.0), fmaxl(h_spl, fhx (-1.0)));

		for (unsigned int x = 0; x < 6; x++)
		{
			qBand[x]->SplInH(h_spl);
			qBand[x]->RunQH();
		}
		for (unsigned int x = 0; x < 6; x++)
			h_spl += fhx (qBand[x]->GetOutputH() * fhx ((h_bValue[x] * h_bValue[x]) - fhx (0.25)) * fhx (4.0));

//		h_spl *= fhx (0.80);
//		if (setShaped)
//		{
//			DynBass->SplInH(h_spl);
//			DynBass->RunQH();
//			h_spl += fhx (DynBass->GetOutputH() * (fhx (1.0) - (h_envPos * 2.0)));
//		}
	}
}
void ToneStack_D::RunConvolution()
{
	if (spLength < 3) return;

	if ((!CabMic) && (!setShaped)) return;		// No convolution
	unsigned int impulseNum = 0;				// Head only
	if (CabMic && setShaped) impulseNum = 1;	// Head + Cab
	if (CabMic && (!setShaped)) impulseNum = 2;	// Cab Only

	if(fpPrec == 0)
	{
		smpL[smpPos] = l_spl;
		smpPos++;
		if(smpPos >= spLength) smpPos = 0;

		flx mix = flx (0.0f);
		int x = 0;
		int y = (smpPos - 1);
		if(y < 0) y = (spLength - 1);
		int z = 0;
		for (x = 0; x < spLength; x++)
		{
			mix += flx (impulseL[impulseNum][z] * smpL[y]);
			y--;
			if (y < 0) y = (spLength -1);
			z++;
			if (z >= spLength) z = 0;
		}
		l_spl = flx (mix);
//		l_spl *= flx (2.0f);
		if (CabMic) l_spl *= flx (0.250f);
		return;
	}
	if(fpPrec == 1)
	{
		smpS[smpPos] = s_spl;
		smpPos++;
		if(smpPos >= spLength) smpPos = 0;

		fsx mix = fsx (0.0);
		int x = 0;
		int y = (smpPos - 1);
		if(y < 0) y = (spLength - 1);
		int z = 0;
		for (x = 0; x < spLength; x++)
		{
			mix += fsx (impulseS[impulseNum][z] * smpS[y]);
			y--;
			if (y < 0) y = (spLength -1);
			z++;
			if (z >= spLength) z = 0;
		}
		s_spl = fsx (mix);
//		s_spl *= fsx (2.0);
		if (CabMic) s_spl *= fsx (0.250);
		return;
	}
	if(fpPrec == 2)
	{
		smpH[smpPos] = h_spl;
		smpPos++;
		if(smpPos >= spLength) smpPos = 0;

		fhx mix = fhx (0.0);
		int x = 0;
		int y = (smpPos - 1);
		if(y < 0) y = (spLength - 1);
		int z = 0;
		for (x = 0; x < spLength; x++)
		{
			mix += fhx (impulseH[impulseNum][z] * smpH[y]);
			y--;
			if (y < 0) y = (spLength -1);
			z++;
			if (z >= spLength) z = 0;
		}
		h_spl = fhx (mix);
//		h_spl *= fhx (2.0);
		if (CabMic) h_spl *= fhx (0.250);
		return;
	}
}
void ToneStack_D::InitToneImpulses()
{
	smpPos = 0;

	impulseL = new flx *[SC_NUM_TSTACK_D_IMPULSES];
	impulseS = new fsx *[SC_NUM_TSTACK_D_IMPULSES];
	impulseH = new fhx *[SC_NUM_TSTACK_D_IMPULSES];

	for (int x = 0; x < SC_NUM_TSTACK_D_IMPULSES; x++)
	{
		impLength[x] = 0;
	}

	int bnd = 0;

	const fhx syncRate = fhx (SC_TSTACK_D_IMPULSE_SyncRate_FloatingPnt);
	const long syncRateL = long (SC_TSTACK_D_IMPULSE_SyncRate_Integer);
	if (cShiftPcnt != fsx (0.0)) currImpShift = int (fsx (fsx (cShiftPcnt * 0.010) * fsx (syncRate)));
	else currImpShift = 0;
	currImpShift++;
	const long syncRateLmidP = long (syncRateL - currImpShift);
	const fhx sRateM = fhx (fhx (sRate) * fhx (syncRate));
	const fhx impulseRate = fhx (44100.0);
	const fhx mFac = fhx (fhx (1.0) / fhx (wConvFac));
	long r = 0;
	long m = 0;
	long cnt = 0;
	BTW_LP *SplReadFilter = new BTW_LP(fsx (fhx (impulseRate * fhx (0.250))), fsx (fhx (impulseRate * syncRate)));
	BTW_LP *inspFilter = new BTW_LP(fsx (sRate * (fsx) 0.250), fsx (sRateM));

	const long impLengthT = long (fhx (fhx (fhx (sRateM) / fhx (impulseRate)) * fhx (SC_TSTACK_D_IMPULSE_MAX_F)));
	fhx *impulseLG = new fhx[impLengthT+syncRateL+2];
	fhx *impulseR = new fhx[(SC_TSTACK_D_IMPULSE_MAX_I+2) * syncRateL];

	fhx sMarker = fhx (0.0);
	fhx sPartial = fhx (0.0);
	const fhx sAdvRate = fhx (fhx (impulseRate) / fhx (sRate));
	long x = 0;
	long a = 0;
	long b = 0;
	int newLength = 0;
	fhx sValA = fhx (0.0);
	fhx sValB = fhx (0.0);
	fhx tH = fhx (0.0);
	fhx maxV = fhx (0.0);

	fhx skw = fhx (0.0);
	bool skewImpulse = false;
	if (cSkewPcnt != fsx (0.0))
	{
		skewImpulse = true;
		skw = fhx (fhx (cSkewPcnt) * fhx (0.010));
	}

	for (bnd = 0; bnd < SC_NUM_TSTACK_D_IMPULSES; bnd++)
	{
		SplReadFilter->zeroBuffers();
		inspFilter->zeroBuffers();

		m = 0;
		cnt = 0;
		maxV = fhx (0.0);

		for(r = 0; r < ((SC_TSTACK_D_IMPULSE_MAX_I+2) * syncRateL); r++)
		{
			impulseR[r] = fhx (0.0);
		}

		for(r = 0; r < ((SC_TSTACK_D_IMPULSE_MAX_I) * syncRateL); r++)
		{
			if(cnt == syncRateLmidP)
			{
				impulseR[r] = fhx (0.0);
				if (bnd == 0) impulseR[r] = fhx (fhx (wavHeadG[m]) * fhx (mFac));
				if (bnd == 1) impulseR[r] = fhx (fhx (wavComboG[m]) * fhx (mFac));
				if (bnd == 2) impulseR[r] = fhx (fhx (wavCabC[m]) * fhx (mFac));

				if (skewImpulse == true)
				{
					impulseR[r] *= fhx (fhx (1.0) - fabsl(skw));
					impulseR[r] += fhx (skw);
				}
			}
			else impulseR[r] = fhx (0.0);

			cnt++;
			if(cnt >= syncRateL)
			{
				cnt = 0;
				m++;
			}
			if(m > SC_TSTACK_D_IMPULSE_MAX_I) break;
		}

		for(r = 0; r < ((SC_TSTACK_D_IMPULSE_MAX_I) * syncRateL); r++)
		{
			SplReadFilter->inpSplH(impulseR[r]);
			SplReadFilter->RunFilter();
			impulseR[r] = (fhx) SplReadFilter->GetOutPutH();
			impulseR[r] *= fhx (syncRate);
		}

		sMarker = fhx (0.0);
		sPartial = fhx (0.0);
		x = 0;
		a = 0;
		b = 0;
		sValA = fhx (0.0);
		sValB = fhx (0.0);
		for(x = 0; x < impLengthT; x++)
		{
			// a = long ((fhx) floorl(fhx (sMarker)));
			a = long (sMarker);
			if (a > (impLengthT)) a = (impLengthT);
			b = long (a + long (1));
			sPartial = fhx (fhx (sMarker) - (fhx) floorl(fhx (sMarker)));
			sPartial = (fhx) fminl(fhx (1.0), fmaxl(fhx (-1.0), (fhx) sPartial));
			sValA = fhx (impulseR[a]);
			sValB = fhx (impulseR[b]);
			sValA *= fhx (fhx (1.0) - fhx (sPartial));
			sValB *= fhx (sPartial);
			impulseLG[x] = fhx (fhx (fhx (sValA) + fhx (sValB)) * fhx (0.50));
			impulseLG[x] = (fhx) fminl(fhx (1.0), fmaxl(fhx (-1.0), (fhx) impulseLG[x]));

			sMarker += fhx (sAdvRate);

			if(fhx (sMarker) > fhx (fhx (SC_TSTACK_D_IMPULSE_MAX_F) * (fhx) syncRate))
			{
//				impLengthT = long (x - 2);
				break;
			}
		}

		for(x = impLengthT - (syncRateL * 16); x < impLengthT; x++) impulseLG[x] = fhx (0.0);

		for(x = 0; x < impLengthT; x++)
		{
			inspFilter->inpSplH(impulseLG[x]);
			inspFilter->RunFilter();
			impulseLG[x] = (fhx) inspFilter->GetOutPutH();
		//	impulseLG[x] *= fhx (syncRate);
			// if (impulseLG[x] > maxV) maxV = impulseLG[x];
			// if ((-impulseLG[x]) > maxV) maxV = (-impulseLG[x]);
			maxV += (fhx) impulseLG[x];
		}
		newLength = long (impLengthT / syncRateL);

		if (maxV != fhx (0.0)) maxV = fhx (fhx (1.0) / maxV);
		else maxV = fhx (1.0);

		// maxV *= fhx (syncRate);

		volAdj_h[bnd] = fhx (maxV);
		volAdj_s[bnd] = fsx (maxV);
		volAdj_l[bnd] = flx (maxV);

		impulseH[bnd] = new fhx[newLength+1];
		impulseS[bnd] = new fsx[newLength+1];
		impulseL[bnd] = new flx[newLength+1];

		newLength -= 1;

		for(x = 0; x < newLength; x++)
		{
			tH = fhx (impulseLG[x*syncRateL]);
//			tH *= fhx (maxV);

			impulseH[bnd][x] = fhx (tH);
			impulseS[bnd][x] = fsx (tH);
			impulseL[bnd][x] = flx (tH);
		}
		impLength[bnd] = newLength;
	}

	int nLong = 65535;

	for (int mnLong = 0; mnLong < SC_NUM_TSTACK_D_IMPULSES; mnLong++)
	{
		nLong = std::min(nLong, impLength[mnLong]);
		impLength[mnLong] = (int) nLong;
	}

	smpH = new fhx[nLong+1];
	smpS = new fsx[nLong+1];
	smpL = new flx[nLong+1];

	for (int f = 0; f <= nLong; f++)
	{
		smpH[f] = fhx (0.0);
		smpS[f] = fsx (0.0);
		smpL[f] = flx (0.f);
	}

	spLength = int (nLong);

	delete [] impulseLG;
	delete [] impulseR;
	delete SplReadFilter;
	delete inspFilter;
}
void ToneStack_D::SetDynamicQ(const fsx inp)
{
	const fhx aSpl = fhx (inp * inp);
//	aSpl *= fhx (fhx (2.0) - aSpl);
	if (aSpl > h_envPos) h_envPos += fhx (fhx (100.0) / fhx (sRate));
	if (aSpl < h_envPos) h_envPos -= fhx (fhx (10.0) / fhx (sRate));
	h_envPos = (fhx) fminl(fhx (1.0), fmaxl(h_envPos, fhx (0.0)));
	s_envPos = fsx (h_envPos);
	l_envPos = flx (s_envPos);
}
