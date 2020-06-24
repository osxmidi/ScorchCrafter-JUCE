#include "csts_eq.h"

Cst_EQ::Cst_EQ() :
	sampL(0.0f),
	sampS(0.0),
	sampH(0.0),
	sRateL(44100.0f),
	sRateS(44100.0),
	sRateH(44100.0)
{
	rateV = new RateFC();
	ClearBuffers();
	SetBands(100.0, 3000.0);
}
void Cst_EQ::ClearBuffers()
{
	sampL = 0.f;
	sampS = 0.0;
	sampH = 0.0;

	for (unsigned int x = 0; x < 3; x++)
	{
		qPaL[0][x] = 0.f;
		qPaL[1][x] = 0.f;

		qPaS[0][x] = 0.0;
		qPaS[1][x] = 0.0;

		qPaH[0][x] = 0.0;
		qPaH[1][x] = 0.0;
	}
}
void Cst_EQ::SetSmpRate(const fsx nRate)
{
	sRateL = (flx) nRate;
	sRateS = (fsx) nRate;
	sRateH = (fhx) nRate;

	ClearBuffers();
}
void Cst_EQ::SetBands(fsx lo, fsx hi)
{
	if (hi > sRateS) hi = sRateS;
	// if (lo > hi) lo = hi;

	fhx bandFreq;

	bandFreq = (fhx) expl(fhx (-2.0) * fhx (M_PI) * hi / sRateS);
	pValLowH = fhx (fhx (1.0) - bandFreq);
	bValLowH = fhx (-bandFreq);
	pValLowS = fsx (pValLowH);
	bValLowS = fsx (bValLowH);
	pValLowL = flx (pValLowH);
	bValLowL = flx (bValLowH);

	bandFreq = (fhx) expl(fhx (-2.0) * fhx (M_PI) * lo / sRateS);
	pValHiH = fhx (fhx (1.0) - bandFreq);
	bValHiH = fhx (-bandFreq);
	pValHiS = fsx (pValHiH);
	bValHiS = fsx (bValHiH);
	pValHiL = flx (pValHiH);
	bValHiL = flx (bValHiH);
}
void Cst_EQ::SplInL(const flx inp) {sampL = flx (inp);}
void Cst_EQ::SplInS(const fsx inp) {sampS = fsx (inp);}
void Cst_EQ::SplInH(const fhx inp) {sampH = fhx (inp);}
flx Cst_EQ::GetOutputL() {return sampL;}
fsx Cst_EQ::GetOutputS() {return sampS;}
fhx Cst_EQ::GetOutputH() {return sampH;}
void Cst_EQ::RunLPL()
{
	sampL = flx (qPaL[0][0] = flx (pValLowL * sampL - bValLowL * qPaL[0][0] + rateV->zl));
	sampL = flx (qPaL[0][1] = flx (pValLowL * sampL - bValLowL * qPaL[0][1] + rateV->zl));
	sampL = flx (qPaL[0][2] = flx (pValLowL * sampL - bValLowL * qPaL[0][2] + rateV->zl));
}
void Cst_EQ::RunHPL()
{
	sampL -= flx (qPaL[1][0] = flx (pValHiL * sampL - bValHiL * qPaL[1][0] + rateV->zl));
	sampL -= flx (qPaL[1][1] = flx (pValHiL * sampL - bValHiL * qPaL[1][1] + rateV->zl));
	sampL -= flx (qPaL[1][2] = flx (pValHiL * sampL - bValHiL * qPaL[1][2] + rateV->zl));
}
void Cst_EQ::RunLPS()
{
	sampS = fsx (qPaS[0][0] = fsx ((pValLowS * sampS) - (bValLowS * qPaS[0][0]) + rateV->zs));
	sampS = fsx (qPaS[0][1] = fsx ((pValLowS * sampS) - (bValLowS * qPaS[0][1]) + rateV->zs));
	sampS = fsx (qPaS[0][2] = fsx ((pValLowS * sampS) - (bValLowS * qPaS[0][2]) + rateV->zs));
}
void Cst_EQ::RunHPS()
{
	sampS -= fsx (qPaS[1][0] = fsx ((pValHiS * sampS) - (bValHiS * qPaS[1][0]) + rateV->zs));
	sampS -= fsx (qPaS[1][1] = fsx ((pValHiS * sampS) - (bValHiS * qPaS[1][1]) + rateV->zs));
	sampS -= fsx (qPaS[1][2] = fsx ((pValHiS * sampS) - (bValHiS * qPaS[1][2]) + rateV->zs));
}
void Cst_EQ::RunLPH()
{
	sampH = fhx (qPaH[0][0] = fhx (pValLowH * sampH - bValLowH * qPaH[0][0] + rateV->zh));
	sampH = fhx (qPaH[0][1] = fhx (pValLowH * sampH - bValLowH * qPaH[0][1] + rateV->zh));
	sampH = fhx (qPaH[0][2] = fhx (pValLowH * sampH - bValLowH * qPaH[0][2] + rateV->zh));
}
void Cst_EQ::RunHPH()
{
	sampH -= fhx (qPaH[1][0] = fhx (pValHiH * sampH - bValHiH * qPaH[1][0] + rateV->zh));
	sampH -= fhx (qPaH[1][1] = fhx (pValHiH * sampH - bValHiH * qPaH[1][1] + rateV->zh));
	sampH -= fhx (qPaH[1][2] = fhx (pValHiH * sampH - bValHiH * qPaH[1][2] + rateV->zh));
}
void Cst_EQ::RunQL()
{
	RunHPL();
	RunLPL();
}
void Cst_EQ::RunQS()
{
	RunHPS();
	RunLPS();
}
void Cst_EQ::RunQH()
{
	RunHPH();
	RunLPH();
}
