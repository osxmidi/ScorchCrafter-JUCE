#include "sample_bank_fp.h"

SampleBank_FP::SampleBank_FP(fsx iSRate) :
	sRateH(fhx (iSRate)),
	sRateS(fsx (iSRate)),
	sRateL(flx (iSRate)),
	cRateH(fhx (iSRate)),
	cRateS(fsx (iSRate)),
	cRateL(flx (iSRate)),
	osRate(int (1)),
	precisionLevel(int (0))
{
	rfV = new RateFC();

	const fhx filtFactorA = fhx (0.25000);
	const fhx filtFactorB = fhx (0.50000);
	const fhx filtFactorC = fhx (1.00000);
	const fhx filtFactorD = fhx (2.00000);
	const fhx filtFactorE = fhx (4.00000);

	UpFilterL[0] = new BTWorthLP_LD(fsx (fhx (sRateH) * fhx (filtFactorA)), fsx (sRateH));
	UpFilterL[1] = new BTWorthLP_LD(fsx (fhx (sRateH) * fhx (filtFactorB)), fsx (sRateH * 2.0));
	UpFilterL[2] = new BTWorthLP_LD(fsx (fhx (sRateH) * fhx (filtFactorC)), fsx (sRateH * 4.0));
	UpFilterL[3] = new BTWorthLP_LD(fsx (fhx (sRateH) * fhx (filtFactorD)), fsx (sRateH * 8.0));
	UpFilterL[4] = new BTWorthLP_LD(fsx (fhx (sRateH) * fhx (filtFactorE)), fsx (sRateH * 16.0));

	DnFilterL[0] = new BTWorthLP_LD(fsx (fhx (sRateH) * fhx (filtFactorA)), fsx (sRateH));
	DnFilterL[1] = new BTWorthLP_LD(fsx (fhx (sRateH) * fhx (filtFactorB)), fsx (sRateH * 2.0));
	DnFilterL[2] = new BTWorthLP_LD(fsx (fhx (sRateH) * fhx (filtFactorC)), fsx (sRateH * 4.0));
	DnFilterL[3] = new BTWorthLP_LD(fsx (fhx (sRateH) * fhx (filtFactorD)), fsx (sRateH * 8.0));
	DnFilterL[4] = new BTWorthLP_LD(fsx (fhx (sRateH) * fhx (filtFactorE)), fsx (sRateH * 16.0));

	UpFilterS[0] = new BTWorthLP_SD(fsx (fhx (sRateH) * fhx (filtFactorA)), fsx (sRateH));
	UpFilterS[1] = new BTWorthLP_SD(fsx (fhx (sRateH) * fhx (filtFactorB)), fsx (sRateH * 2.0));
	UpFilterS[2] = new BTWorthLP_SD(fsx (fhx (sRateH) * fhx (filtFactorC)), fsx (sRateH * 4.0));
	UpFilterS[3] = new BTWorthLP_SD(fsx (fhx (sRateH) * fhx (filtFactorD)), fsx (sRateH * 8.0));
	UpFilterS[4] = new BTWorthLP_SD(fsx (fhx (sRateH) * fhx (filtFactorE)), fsx (sRateH * 16.0));

	DnFilterS[0] = new BTWorthLP_SD(fsx (fhx (sRateH) * fhx (filtFactorA)), fsx (sRateH));
	DnFilterS[1] = new BTWorthLP_SD(fsx (fhx (sRateH) * fhx (filtFactorB)), fsx (sRateH * 2.0));
	DnFilterS[2] = new BTWorthLP_SD(fsx (fhx (sRateH) * fhx (filtFactorC)), fsx (sRateH * 4.0));
	DnFilterS[3] = new BTWorthLP_SD(fsx (fhx (sRateH) * fhx (filtFactorD)), fsx (sRateH * 8.0));
	DnFilterS[4] = new BTWorthLP_SD(fsx (fhx (sRateH) * fhx (filtFactorE)), fsx (sRateH * 16.0));

	UpFilterH[0] = new BTWorthLP_HD(fsx (fhx (sRateH) * fhx (filtFactorA)), fsx (sRateH));
	UpFilterH[1] = new BTWorthLP_HD(fsx (fhx (sRateH) * fhx (filtFactorB)), fsx (sRateH * 2.0));
	UpFilterH[2] = new BTWorthLP_HD(fsx (fhx (sRateH) * fhx (filtFactorC)), fsx (sRateH * 4.0));
	UpFilterH[3] = new BTWorthLP_HD(fsx (fhx (sRateH) * fhx (filtFactorD)), fsx (sRateH * 8.0));
	UpFilterH[4] = new BTWorthLP_HD(fsx (fhx (sRateH) * fhx (filtFactorE)), fsx (sRateH * 16.0));

	DnFilterH[0] = new BTWorthLP_HD(fsx (fhx (sRateH) * fhx (filtFactorA)), fsx (sRateH));
	DnFilterH[1] = new BTWorthLP_HD(fsx (fhx (sRateH) * fhx (filtFactorB)), fsx (sRateH * 2.0));
	DnFilterH[2] = new BTWorthLP_HD(fsx (fhx (sRateH) * fhx (filtFactorC)), fsx (sRateH * 4.0));
	DnFilterH[3] = new BTWorthLP_HD(fsx (fhx (sRateH) * fhx (filtFactorD)), fsx (sRateH * 8.0));
	DnFilterH[4] = new BTWorthLP_HD(fsx (fhx (sRateH) * fhx (filtFactorE)), fsx (sRateH * 16.0));

	zeroAllSamples();
}
SampleBank_FP::~SampleBank_FP()
{
	for(int x = 0; x < 5; x++)
	{
		delete UpFilterL[x];
		delete DnFilterL[x];
		delete UpFilterS[x];
		delete DnFilterS[x];
		delete UpFilterH[x];
		delete DnFilterH[x];
	}

	delete rfV;
}
void SampleBank_FP::InputSmp(const fhx inp)
{
	if(precisionLevel == 0) smpl[0] = flx (inp);
	if(precisionLevel == 1) smps[0] = fsx (inp);
	if(precisionLevel == 2) smph[0] = fhx (inp);
}
void SampleBank_FP::setSampleRate(fsx nSRate)
{
	sRateL = flx (nSRate);
	sRateS = fsx (nSRate);
	sRateH = fhx (nSRate);

	cRateL = flx (rfV->vl[osRate] * flx (sRateL));
	cRateS = fsx (rfV->vs[osRate] * fsx (sRateS));
	cRateH = fhx (rfV->vh[osRate] * fhx (sRateH));

	UpFilterL[0]->setSampleRate(fsx (nSRate));
	UpFilterL[1]->setSampleRate(fsx (nSRate * 2.0));
	UpFilterL[2]->setSampleRate(fsx (nSRate * 4.0));
	UpFilterL[3]->setSampleRate(fsx (nSRate * 8.0));
	UpFilterL[4]->setSampleRate(fsx (nSRate * 16.0));

	DnFilterL[0]->setSampleRate(fsx (nSRate));
	DnFilterL[1]->setSampleRate(fsx (nSRate * 2.0));
	DnFilterL[2]->setSampleRate(fsx (nSRate * 4.0));
	DnFilterL[3]->setSampleRate(fsx (nSRate * 8.0));
	DnFilterL[4]->setSampleRate(fsx (nSRate * 16.0));

	UpFilterS[0]->setSampleRate(fsx (nSRate));
	UpFilterS[1]->setSampleRate(fsx (nSRate * 2.0));
	UpFilterS[2]->setSampleRate(fsx (nSRate * 4.0));
	UpFilterS[3]->setSampleRate(fsx (nSRate * 8.0));
	UpFilterS[4]->setSampleRate(fsx (nSRate * 16.0));

	DnFilterS[0]->setSampleRate(fsx (nSRate));
	DnFilterS[1]->setSampleRate(fsx (nSRate * 2.0));
	DnFilterS[2]->setSampleRate(fsx (nSRate * 4.0));
	DnFilterS[3]->setSampleRate(fsx (nSRate * 8.0));
	DnFilterS[4]->setSampleRate(fsx (nSRate * 16.0));

	UpFilterH[0]->setSampleRate(fsx (nSRate));
	UpFilterH[1]->setSampleRate(fsx (nSRate * 2.0));
	UpFilterH[2]->setSampleRate(fsx (nSRate * 4.0));
	UpFilterH[3]->setSampleRate(fsx (nSRate * 8.0));
	UpFilterH[4]->setSampleRate(fsx (nSRate * 16.0));

	DnFilterH[0]->setSampleRate(fsx (nSRate));
	DnFilterH[1]->setSampleRate(fsx (nSRate * 2.0));
	DnFilterH[2]->setSampleRate(fsx (nSRate * 4.0));
	DnFilterH[3]->setSampleRate(fsx (nSRate * 8.0));
	DnFilterH[4]->setSampleRate(fsx (nSRate * 16.0));

	fsx filtFC[] = {0.25, 0.50, 1.00, 2.00, 4.00};

	for(int x = 0; x < 5; x++)
	{
		UpFilterL[x]->changeFreq(fsx (filtFC[x]) * fsx (sRateS));
		UpFilterS[x]->changeFreq(fsx (filtFC[x]) * fsx (sRateS));
		UpFilterH[x]->changeFreq(fsx (filtFC[x]) * fsx (sRateS));

		DnFilterL[x]->changeFreq(fsx (filtFC[x]) * fsx (sRateS));
		DnFilterS[x]->changeFreq(fsx (filtFC[x]) * fsx (sRateS));
		DnFilterH[x]->changeFreq(fsx (filtFC[x]) * fsx (sRateS));
	}

	zeroAllSamples();
}
void SampleBank_FP::zeroAllSamples()
{
	if(precisionLevel == 0)
	{
		smpl[0] = flx (0.0f);
		smpl[1] = flx (0.0f);
		smpl[2] = flx (0.0f);
		smpl[3] = flx (0.0f);
		smpl[4] = flx (0.0f);
		smpl[5] = flx (0.0f);
		smpl[6] = flx (0.0f);
		smpl[7] = flx (0.0f);
		smpl[8] = flx (0.0f);
		smpl[9] = flx (0.0f);
		smpl[10] = flx (0.0f);
		smpl[11] = flx (0.0f);
		smpl[12] = flx (0.0f);
		smpl[13] = flx (0.0f);
		smpl[14] = flx (0.0f);
		smpl[15] = flx (0.0f);
	}
	if(precisionLevel == 1)
	{
		smps[0] = fsx (0.0);
		smps[1] = fsx (0.0);
		smps[2] = fsx (0.0);
		smps[3] = fsx (0.0);
		smps[4] = fsx (0.0);
		smps[5] = fsx (0.0);
		smps[6] = fsx (0.0);
		smps[7] = fsx (0.0);
		smps[8] = fsx (0.0);
		smps[9] = fsx (0.0);
		smps[10] = fsx (0.0);
		smps[11] = fsx (0.0);
		smps[12] = fsx (0.0);
		smps[13] = fsx (0.0);
		smps[14] = fsx (0.0);
		smps[15] = fsx (0.0);
	}
	if(precisionLevel == 2)
	{
		smph[0] = fhx (0.0);
		smph[1] = fhx (0.0);
		smph[2] = fhx (0.0);
		smph[3] = fhx (0.0);
		smph[4] = fhx (0.0);
		smph[5] = fhx (0.0);
		smph[6] = fhx (0.0);
		smph[7] = fhx (0.0);
		smph[8] = fhx (0.0);
		smph[9] = fhx (0.0);
		smph[10] = fhx (0.0);
		smph[11] = fhx (0.0);
		smph[12] = fhx (0.0);
		smph[13] = fhx (0.0);
		smph[14] = fhx (0.0);
		smph[15] = fhx (0.0);
	}
}
void SampleBank_FP::setOverSampling(const int oSLevel)
{
	if(osRate < 0) return;
	if(osRate > 6) return;

	osRate = int (oSLevel);
	cRateL = flx (rfV->vl[osRate] * flx (sRateL));
	cRateS = fsx (rfV->vs[osRate] * fsx (sRateS));
	cRateH = fhx (rfV->vh[osRate] * fhx (sRateH));

	zeroAllSamples();
}
void SampleBank_FP::runUpSampling()
{
	if(osRate > 5) return;

	if(precisionLevel == 0)
	{
		if(osRate>=2)
		{
			smpl[1]=flx (0.0f);
			smpl[0]=(flx) UpFilterL[1]->runFilterL(flx (smpl[0]));
			smpl[1]=(flx) UpFilterL[1]->runFilterL(flx (smpl[1]));
			smpl[0]*= flx (2.0f);
			smpl[1]*= flx (2.0f);
		}
		if(osRate>=3)
		{
			smpl[2]=flx (smpl[1]);
			smpl[1]=flx (0.0f);
			smpl[3]=flx (0.0f);
			smpl[0]=(flx) UpFilterL[2]->runFilterL(flx (smpl[0]));
			smpl[1]=(flx) UpFilterL[2]->runFilterL(flx (smpl[1]));
			smpl[2]=(flx) UpFilterL[2]->runFilterL(flx (smpl[2]));
			smpl[3]=(flx) UpFilterL[2]->runFilterL(flx (smpl[3]));
			smpl[0]*= flx (2.0f);
			smpl[1]*= flx (2.0f);
			smpl[2]*= flx (2.0f);
			smpl[3]*= flx (2.0f);
		}
		if(osRate>=4)
		{
			smpl[6]=flx (smpl[3]);
			smpl[4]=flx (smpl[2]);
			smpl[2]=flx (smpl[1]);
			smpl[1]=flx (0.0f);
			smpl[3]=flx (0.0f);
			smpl[5]=flx (0.0f);
			smpl[7]=flx (0.0f);
			smpl[0]=(flx) UpFilterL[3]->runFilterL(flx (smpl[0]));
			smpl[1]=(flx) UpFilterL[3]->runFilterL(flx (smpl[1]));
			smpl[2]=(flx) UpFilterL[3]->runFilterL(flx (smpl[2]));
			smpl[3]=(flx) UpFilterL[3]->runFilterL(flx (smpl[3]));
			smpl[4]=(flx) UpFilterL[3]->runFilterL(flx (smpl[4]));
			smpl[5]=(flx) UpFilterL[3]->runFilterL(flx (smpl[5]));
			smpl[6]=(flx) UpFilterL[3]->runFilterL(flx (smpl[6]));
			smpl[7]=(flx) UpFilterL[3]->runFilterL(flx (smpl[7]));
			smpl[0]*= flx (2.0f);
			smpl[1]*= flx (2.0f);
			smpl[2]*= flx (2.0f);
			smpl[3]*= flx (2.0f);
			smpl[4]*= flx (2.0f);
			smpl[5]*= flx (2.0f);
			smpl[6]*= flx (2.0f);
			smpl[7]*= flx (2.0f);
		}
		if(osRate==5)
		{
			smpl[14]=flx (smpl[7]);
			smpl[12]=flx (smpl[6]);
			smpl[10]=flx (smpl[5]);
			smpl[8]=flx (smpl[4]);
			smpl[6]=flx (smpl[3]);
			smpl[4]=flx (smpl[2]);
			smpl[2]=flx (smpl[1]);
			smpl[1]=flx (0.0f);
			smpl[3]=flx (0.0f);
			smpl[5]=flx (0.0f);
			smpl[7]=flx (0.0f);
			smpl[9]=flx (0.0f);
			smpl[11]=flx (0.0f);
			smpl[13]=flx (0.0f);
			smpl[15]=flx (0.0f);
			smpl[0]=(flx) UpFilterL[4]->runFilterL(flx (smpl[0]));
			smpl[1]=(flx) UpFilterL[4]->runFilterL(flx (smpl[1]));
			smpl[2]=(flx) UpFilterL[4]->runFilterL(flx (smpl[2]));
			smpl[3]=(flx) UpFilterL[4]->runFilterL(flx (smpl[3]));
			smpl[4]=(flx) UpFilterL[4]->runFilterL(flx (smpl[4]));
			smpl[5]=(flx) UpFilterL[4]->runFilterL(flx (smpl[5]));
			smpl[6]=(flx) UpFilterL[4]->runFilterL(flx (smpl[6]));
			smpl[7]=(flx) UpFilterL[4]->runFilterL(flx (smpl[7]));
			smpl[8]=(flx) UpFilterL[4]->runFilterL(flx (smpl[8]));
			smpl[9]=(flx) UpFilterL[4]->runFilterL(flx (smpl[9]));
			smpl[10]=(flx) UpFilterL[4]->runFilterL(flx (smpl[10]));
			smpl[11]=(flx) UpFilterL[4]->runFilterL(flx (smpl[11]));
			smpl[12]=(flx) UpFilterL[4]->runFilterL(flx (smpl[12]));
			smpl[13]=(flx) UpFilterL[4]->runFilterL(flx (smpl[13]));
			smpl[14]=(flx) UpFilterL[4]->runFilterL(flx (smpl[14]));
			smpl[15]=(flx) UpFilterL[4]->runFilterL(flx (smpl[15]));
			smpl[0]*= flx (2.0f);
			smpl[1]*= flx (2.0f);
			smpl[2]*= flx (2.0f);
			smpl[3]*= flx (2.0f);
			smpl[4]*= flx (2.0f);
			smpl[5]*= flx (2.0f);
			smpl[6]*= flx (2.0f);
			smpl[7]*= flx (2.0f);
			smpl[8]*= flx (2.0f);
			smpl[9]*= flx (2.0f);
			smpl[10]*= flx (2.0f);
			smpl[11]*= flx (2.0f);
			smpl[12]*= flx (2.0f);
			smpl[13]*= flx (2.0f);
			smpl[14]*= flx (2.0f);
			smpl[15]*= flx (2.0f);
		}
	}
	if(precisionLevel == 1)
	{
		if(osRate>=2)
		{
			smps[1]=fsx (0.0);
			smps[0]=(fsx) UpFilterS[1]->runFilterS(fsx (smps[0]));
			smps[1]=(fsx) UpFilterS[1]->runFilterS(fsx (smps[1]));
			smps[0]*= fsx (2.0);
			smps[1]*= fsx (2.0);
		}
		if(osRate>=3)
		{
			smps[2]=fsx (smps[1]);
			smps[1]=fsx (0.0);
			smps[3]=fsx (0.0);
			smps[0]=(fsx) UpFilterS[2]->runFilterS(fsx (smps[0]));
			smps[1]=(fsx) UpFilterS[2]->runFilterS(fsx (smps[1]));
			smps[2]=(fsx) UpFilterS[2]->runFilterS(fsx (smps[2]));
			smps[3]=(fsx) UpFilterS[2]->runFilterS(fsx (smps[3]));
			smps[0]*= fsx (2.0);
			smps[1]*= fsx (2.0);
			smps[2]*= fsx (2.0);
			smps[3]*= fsx (2.0);
		}
		if(osRate>=4)
		{
			smps[6]=fsx (smps[3]);
			smps[4]=fsx (smps[2]);
			smps[2]=fsx (smps[1]);
			smps[1]=fsx (0.0);
			smps[3]=fsx (0.0);
			smps[5]=fsx (0.0);
			smps[7]=fsx (0.0);
			smps[0]=(fsx) UpFilterS[3]->runFilterS(fsx (smps[0]));
			smps[1]=(fsx) UpFilterS[3]->runFilterS(fsx (smps[1]));
			smps[2]=(fsx) UpFilterS[3]->runFilterS(fsx (smps[2]));
			smps[3]=(fsx) UpFilterS[3]->runFilterS(fsx (smps[3]));
			smps[4]=(fsx) UpFilterS[3]->runFilterS(fsx (smps[4]));
			smps[5]=(fsx) UpFilterS[3]->runFilterS(fsx (smps[5]));
			smps[6]=(fsx) UpFilterS[3]->runFilterS(fsx (smps[6]));
			smps[7]=(fsx) UpFilterS[3]->runFilterS(fsx (smps[7]));
			smps[0]*= fsx (2.0);
			smps[1]*= fsx (2.0);
			smps[2]*= fsx (2.0);
			smps[3]*= fsx (2.0);
			smps[4]*= fsx (2.0);
			smps[5]*= fsx (2.0);
			smps[6]*= fsx (2.0);
			smps[7]*= fsx (2.0);
		}
		if(osRate==5)
		{
			smps[14]=fsx (smps[7]);
			smps[12]=fsx (smps[6]);
			smps[10]=fsx (smps[5]);
			smps[8]=fsx (smps[4]);
			smps[6]=fsx (smps[3]);
			smps[4]=fsx (smps[2]);
			smps[2]=fsx (smps[1]);
			smps[1]=fsx (0.0);
			smps[3]=fsx (0.0);
			smps[5]=fsx (0.0);
			smps[7]=fsx (0.0);
			smps[9]=fsx (0.0);
			smps[11]=fsx (0.0);
			smps[13]=fsx (0.0);
			smps[15]=fsx (0.0);
			smps[0]=(fsx) UpFilterS[4]->runFilterS(fsx (smps[0]));
			smps[1]=(fsx) UpFilterS[4]->runFilterS(fsx (smps[1]));
			smps[2]=(fsx) UpFilterS[4]->runFilterS(fsx (smps[2]));
			smps[3]=(fsx) UpFilterS[4]->runFilterS(fsx (smps[3]));
			smps[4]=(fsx) UpFilterS[4]->runFilterS(fsx (smps[4]));
			smps[5]=(fsx) UpFilterS[4]->runFilterS(fsx (smps[5]));
			smps[6]=(fsx) UpFilterS[4]->runFilterS(fsx (smps[6]));
			smps[7]=(fsx) UpFilterS[4]->runFilterS(fsx (smps[7]));
			smps[8]=(fsx) UpFilterS[4]->runFilterS(fsx (smps[8]));
			smps[9]=(fsx) UpFilterS[4]->runFilterS(fsx (smps[9]));
			smps[10]=(fsx) UpFilterS[4]->runFilterS(fsx (smps[10]));
			smps[11]=(fsx) UpFilterS[4]->runFilterS(fsx (smps[11]));
			smps[12]=(fsx) UpFilterS[4]->runFilterS(fsx (smps[12]));
			smps[13]=(fsx) UpFilterS[4]->runFilterS(fsx (smps[13]));
			smps[14]=(fsx) UpFilterS[4]->runFilterS(fsx (smps[14]));
			smps[15]=(fsx) UpFilterS[4]->runFilterS(fsx (smps[15]));
			smps[0]*= fsx (2.0);
			smps[1]*= fsx (2.0);
			smps[2]*= fsx (2.0);
			smps[3]*= fsx (2.0);
			smps[4]*= fsx (2.0);
			smps[5]*= fsx (2.0);
			smps[6]*= fsx (2.0);
			smps[7]*= fsx (2.0);
			smps[8]*= fsx (2.0);
			smps[9]*= fsx (2.0);
			smps[10]*= fsx (2.0);
			smps[11]*= fsx (2.0);
			smps[12]*= fsx (2.0);
			smps[13]*= fsx (2.0);
			smps[14]*= fsx (2.0);
			smps[15]*= fsx (2.0);
		}
	}
	if(precisionLevel == 2)
	{
		if(osRate>=2)
		{
			smph[1]=fhx (0.0);
			smph[0]=(fhx) UpFilterH[1]->runFilterH(fhx (smph[0]));
			smph[1]=(fhx) UpFilterH[1]->runFilterH(fhx (smph[1]));
			smph[0]*= fhx (2.0);
			smph[1]*= fhx (2.0);
		}
		if(osRate>=3)
		{
			smph[2]=fhx (smph[1]);
			smph[1]=fhx (0.0);
			smph[3]=fhx (0.0);
			smph[0]=(fhx) UpFilterH[2]->runFilterH(fhx (smph[0]));
			smph[1]=(fhx) UpFilterH[2]->runFilterH(fhx (smph[1]));
			smph[2]=(fhx) UpFilterH[2]->runFilterH(fhx (smph[2]));
			smph[3]=(fhx) UpFilterH[2]->runFilterH(fhx (smph[3]));
			smph[0]*= fhx (2.0);
			smph[1]*= fhx (2.0);
			smph[2]*= fhx (2.0);
			smph[3]*= fhx (2.0);
		}
		if(osRate>=4)
		{
			smph[6]=fhx (smph[3]);
			smph[4]=fhx (smph[2]);
			smph[2]=fhx (smph[1]);
			smph[1]=fhx (0.0);
			smph[3]=fhx (0.0);
			smph[5]=fhx (0.0);
			smph[7]=fhx (0.0);
			smph[0]=(fhx) UpFilterH[3]->runFilterH(fhx (smph[0]));
			smph[1]=(fhx) UpFilterH[3]->runFilterH(fhx (smph[1]));
			smph[2]=(fhx) UpFilterH[3]->runFilterH(fhx (smph[2]));
			smph[3]=(fhx) UpFilterH[3]->runFilterH(fhx (smph[3]));
			smph[4]=(fhx) UpFilterH[3]->runFilterH(fhx (smph[4]));
			smph[5]=(fhx) UpFilterH[3]->runFilterH(fhx (smph[5]));
			smph[6]=(fhx) UpFilterH[3]->runFilterH(fhx (smph[6]));
			smph[7]=(fhx) UpFilterH[3]->runFilterH(fhx (smph[7]));
			smph[0]*= fhx (2.0);
			smph[1]*= fhx (2.0);
			smph[2]*= fhx (2.0);
			smph[3]*= fhx (2.0);
			smph[4]*= fhx (2.0);
			smph[5]*= fhx (2.0);
			smph[6]*= fhx (2.0);
			smph[7]*= fhx (2.0);
		}
		if(osRate==5)
		{
			smph[14]=fhx (smph[7]);
			smph[12]=fhx (smph[6]);
			smph[10]=fhx (smph[5]);
			smph[8]=fhx (smph[4]);
			smph[6]=fhx (smph[3]);
			smph[4]=fhx (smph[2]);
			smph[2]=fhx (smph[1]);
			smph[1]=fhx (0.0);
			smph[3]=fhx (0.0);
			smph[5]=fhx (0.0);
			smph[7]=fhx (0.0);
			smph[9]=fhx (0.0);
			smph[11]=fhx (0.0);
			smph[13]=fhx (0.0);
			smph[15]=fhx (0.0);
			smph[0]=(fhx) UpFilterH[4]->runFilterH(fhx (smph[0]));
			smph[1]=(fhx) UpFilterH[4]->runFilterH(fhx (smph[1]));
			smph[2]=(fhx) UpFilterH[4]->runFilterH(fhx (smph[2]));
			smph[3]=(fhx) UpFilterH[4]->runFilterH(fhx (smph[3]));
			smph[4]=(fhx) UpFilterH[4]->runFilterH(fhx (smph[4]));
			smph[5]=(fhx) UpFilterH[4]->runFilterH(fhx (smph[5]));
			smph[6]=(fhx) UpFilterH[4]->runFilterH(fhx (smph[6]));
			smph[7]=(fhx) UpFilterH[4]->runFilterH(fhx (smph[7]));
			smph[8]=(fhx) UpFilterH[4]->runFilterH(fhx (smph[8]));
			smph[9]=(fhx) UpFilterH[4]->runFilterH(fhx (smph[9]));
			smph[10]=(fhx) UpFilterH[4]->runFilterH(fhx (smph[10]));
			smph[11]=(fhx) UpFilterH[4]->runFilterH(fhx (smph[11]));
			smph[12]=(fhx) UpFilterH[4]->runFilterH(fhx (smph[12]));
			smph[13]=(fhx) UpFilterH[4]->runFilterH(fhx (smph[13]));
			smph[14]=(fhx) UpFilterH[4]->runFilterH(fhx (smph[14]));
			smph[15]=(fhx) UpFilterH[4]->runFilterH(fhx (smph[15]));
			smph[0]*= fhx (2.0);
			smph[1]*= fhx (2.0);
			smph[2]*= fhx (2.0);
			smph[3]*= fhx (2.0);
			smph[4]*= fhx (2.0);
			smph[5]*= fhx (2.0);
			smph[6]*= fhx (2.0);
			smph[7]*= fhx (2.0);
			smph[8]*= fhx (2.0);
			smph[9]*= fhx (2.0);
			smph[10]*= fhx (2.0);
			smph[11]*= fhx (2.0);
			smph[12]*= fhx (2.0);
			smph[13]*= fhx (2.0);
			smph[14]*= fhx (2.0);
			smph[15]*= fhx (2.0);
		}
	}
}
void SampleBank_FP::runDownSampling()
{
	if(osRate > 5) return;

	if(precisionLevel == 0)
	{
		if(osRate==5)
		{
			smpl[0]=(flx) DnFilterL[4]->runFilterL(flx (smpl[0]));
			smpl[1]=(flx) DnFilterL[4]->runFilterL(flx (smpl[1]));
			smpl[2]=(flx) DnFilterL[4]->runFilterL(flx (smpl[2]));
			smpl[3]=(flx) DnFilterL[4]->runFilterL(flx (smpl[3]));
			smpl[4]=(flx) DnFilterL[4]->runFilterL(flx (smpl[4]));
			smpl[5]=(flx) DnFilterL[4]->runFilterL(flx (smpl[5]));
			smpl[6]=(flx) DnFilterL[4]->runFilterL(flx (smpl[6]));
			smpl[7]=(flx) DnFilterL[4]->runFilterL(flx (smpl[7]));
			smpl[8]=(flx) DnFilterL[4]->runFilterL(flx (smpl[8]));
			smpl[9]=(flx) DnFilterL[4]->runFilterL(flx (smpl[9]));
			smpl[10]=(flx) DnFilterL[4]->runFilterL(flx (smpl[10]));
			smpl[11]=(flx) DnFilterL[4]->runFilterL(flx (smpl[11]));
			smpl[12]=(flx) DnFilterL[4]->runFilterL(flx (smpl[12]));
			smpl[13]=(flx) DnFilterL[4]->runFilterL(flx (smpl[13]));
			smpl[14]=(flx) DnFilterL[4]->runFilterL(flx (smpl[14]));
			smpl[15]=(flx) DnFilterL[4]->runFilterL(flx (smpl[15]));
			smpl[1]=flx (smpl[2]);
			smpl[2]=flx (smpl[4]);
			smpl[3]=flx (smpl[6]);
			smpl[4]=flx (smpl[8]);
			smpl[5]=flx (smpl[10]);
			smpl[6]=flx (smpl[12]);
			smpl[7]=flx (smpl[14]);
		}
		if(osRate>=4)
		{
			smpl[0]=(flx) DnFilterL[3]->runFilterL(flx (smpl[0]));
			smpl[1]=(flx) DnFilterL[3]->runFilterL(flx (smpl[1]));
			smpl[2]=(flx) DnFilterL[3]->runFilterL(flx (smpl[2]));
			smpl[3]=(flx) DnFilterL[3]->runFilterL(flx (smpl[3]));
			smpl[4]=(flx) DnFilterL[3]->runFilterL(flx (smpl[4]));
			smpl[5]=(flx) DnFilterL[3]->runFilterL(flx (smpl[5]));
			smpl[6]=(flx) DnFilterL[3]->runFilterL(flx (smpl[6]));
			smpl[7]=(flx) DnFilterL[3]->runFilterL(flx (smpl[7]));
			smpl[1]=flx (smpl[2]);
			smpl[2]=flx (smpl[4]);
			smpl[3]=flx (smpl[6]);
		}
		if(osRate>=3)
		{
			smpl[0]=(flx) DnFilterL[2]->runFilterL(flx (smpl[0]));
			smpl[1]=(flx) DnFilterL[2]->runFilterL(flx (smpl[1]));
			smpl[2]=(flx) DnFilterL[2]->runFilterL(flx (smpl[2]));
			smpl[3]=(flx) DnFilterL[2]->runFilterL(flx (smpl[3]));
			smpl[1]=flx (smpl[2]);
		}
		if(osRate>=2)
		{
			smpl[0]=(flx) DnFilterL[1]->runFilterL(flx (smpl[0]));
			smpl[1]=(flx) DnFilterL[1]->runFilterL(flx (smpl[1]));
		}
		// smpl[0]=(flx) DnFilterL[0]->runFilterL(flx (smpl[0]));
	}
	if(precisionLevel == 1)
	{
		if(osRate==5)
		{
			smps[0]=(fsx) DnFilterS[4]->runFilterS(fsx (smps[0]));
			smps[1]=(fsx) DnFilterS[4]->runFilterS(fsx (smps[1]));
			smps[2]=(fsx) DnFilterS[4]->runFilterS(fsx (smps[2]));
			smps[3]=(fsx) DnFilterS[4]->runFilterS(fsx (smps[3]));
			smps[4]=(fsx) DnFilterS[4]->runFilterS(fsx (smps[4]));
			smps[5]=(fsx) DnFilterS[4]->runFilterS(fsx (smps[5]));
			smps[6]=(fsx) DnFilterS[4]->runFilterS(fsx (smps[6]));
			smps[7]=(fsx) DnFilterS[4]->runFilterS(fsx (smps[7]));
			smps[8]=(fsx) DnFilterS[4]->runFilterS(fsx (smps[8]));
			smps[9]=(fsx) DnFilterS[4]->runFilterS(fsx (smps[9]));
			smps[10]=(fsx) DnFilterS[4]->runFilterS(fsx (smps[10]));
			smps[11]=(fsx) DnFilterS[4]->runFilterS(fsx (smps[11]));
			smps[12]=(fsx) DnFilterS[4]->runFilterS(fsx (smps[12]));
			smps[13]=(fsx) DnFilterS[4]->runFilterS(fsx (smps[13]));
			smps[14]=(fsx) DnFilterS[4]->runFilterS(fsx (smps[14]));
			smps[15]=(fsx) DnFilterS[4]->runFilterS(fsx (smps[15]));
			smps[1]=fsx (smps[2]);
			smps[2]=fsx (smps[4]);
			smps[3]=fsx (smps[6]);
			smps[4]=fsx (smps[8]);
			smps[5]=fsx (smps[10]);
			smps[6]=fsx (smps[12]);
			smps[7]=fsx (smps[14]);
		}
		if(osRate>=4)
		{
			smps[0]=(fsx) DnFilterS[3]->runFilterS(fsx (smps[0]));
			smps[1]=(fsx) DnFilterS[3]->runFilterS(fsx (smps[1]));
			smps[2]=(fsx) DnFilterS[3]->runFilterS(fsx (smps[2]));
			smps[3]=(fsx) DnFilterS[3]->runFilterS(fsx (smps[3]));
			smps[4]=(fsx) DnFilterS[3]->runFilterS(fsx (smps[4]));
			smps[5]=(fsx) DnFilterS[3]->runFilterS(fsx (smps[5]));
			smps[6]=(fsx) DnFilterS[3]->runFilterS(fsx (smps[6]));
			smps[7]=(fsx) DnFilterS[3]->runFilterS(fsx (smps[7]));
			smps[1]=fsx (smps[2]);
			smps[2]=fsx (smps[4]);
			smps[3]=fsx (smps[6]);
		}
		if(osRate>=3)
		{
			smps[0]=(fsx) DnFilterS[2]->runFilterS(fsx (smps[0]));
			smps[1]=(fsx) DnFilterS[2]->runFilterS(fsx (smps[1]));
			smps[2]=(fsx) DnFilterS[2]->runFilterS(fsx (smps[2]));
			smps[3]=(fsx) DnFilterS[2]->runFilterS(fsx (smps[3]));
			smps[1]=fsx (smps[2]);
		}
		if(osRate>=2)
		{
			smps[0]=(fsx) DnFilterS[1]->runFilterS(fsx (smps[0]));
			smps[1]=(fsx) DnFilterS[1]->runFilterS(fsx (smps[1]));
		}
		// smps[0]=(fsx) DnFilterS[0]->runFilterS(fsx (smps[0]));
	}
	if(precisionLevel == 2)
	{
		if(osRate==5)
		{
			smph[0]=(fhx) DnFilterH[4]->runFilterH(fhx (smph[0]));
			smph[1]=(fhx) DnFilterH[4]->runFilterH(fhx (smph[1]));
			smph[2]=(fhx) DnFilterH[4]->runFilterH(fhx (smph[2]));
			smph[3]=(fhx) DnFilterH[4]->runFilterH(fhx (smph[3]));
			smph[4]=(fhx) DnFilterH[4]->runFilterH(fhx (smph[4]));
			smph[5]=(fhx) DnFilterH[4]->runFilterH(fhx (smph[5]));
			smph[6]=(fhx) DnFilterH[4]->runFilterH(fhx (smph[6]));
			smph[7]=(fhx) DnFilterH[4]->runFilterH(fhx (smph[7]));
			smph[8]=(fhx) DnFilterH[4]->runFilterH(fhx (smph[8]));
			smph[9]=(fhx) DnFilterH[4]->runFilterH(fhx (smph[9]));
			smph[10]=(fhx) DnFilterH[4]->runFilterH(fhx (smph[10]));
			smph[11]=(fhx) DnFilterH[4]->runFilterH(fhx (smph[11]));
			smph[12]=(fhx) DnFilterH[4]->runFilterH(fhx (smph[12]));
			smph[13]=(fhx) DnFilterH[4]->runFilterH(fhx (smph[13]));
			smph[14]=(fhx) DnFilterH[4]->runFilterH(fhx (smph[14]));
			smph[15]=(fhx) DnFilterH[4]->runFilterH(fhx (smph[15]));
//			smph[0]=fhx (fhx (fhx (smph[0]) + fhx (smph[1])) * fhx (0.50));
//			smph[2]=fhx (fhx (fhx (smph[2]) + fhx (smph[3])) * fhx (0.50));
//			smph[4]=fhx (fhx (fhx (smph[4]) + fhx (smph[5])) * fhx (0.50));
//			smph[6]=fhx (fhx (fhx (smph[6]) + fhx (smph[7])) * fhx (0.50));
//			smph[8]=fhx (fhx (fhx (smph[8]) + fhx (smph[9])) * fhx (0.50));
//			smph[10]=fhx (fhx (fhx (smph[10]) + fhx (smph[11])) * fhx (0.50));
//			smph[12]=fhx (fhx (fhx (smph[12]) + fhx (smph[13])) * fhx (0.50));
//			smph[14]=fhx (fhx (fhx (smph[14]) + fhx (smph[15])) * fhx (0.50));
			smph[1]=fhx (smph[2]);
			smph[2]=fhx (smph[4]);
			smph[3]=fhx (smph[6]);
			smph[4]=fhx (smph[8]);
			smph[5]=fhx (smph[10]);
			smph[6]=fhx (smph[12]);
			smph[7]=fhx (smph[14]);
		}
		if(osRate>=4)
		{
			smph[0]=(fhx) DnFilterH[3]->runFilterH(fhx (smph[0]));
			smph[1]=(fhx) DnFilterH[3]->runFilterH(fhx (smph[1]));
			smph[2]=(fhx) DnFilterH[3]->runFilterH(fhx (smph[2]));
			smph[3]=(fhx) DnFilterH[3]->runFilterH(fhx (smph[3]));
			smph[4]=(fhx) DnFilterH[3]->runFilterH(fhx (smph[4]));
			smph[5]=(fhx) DnFilterH[3]->runFilterH(fhx (smph[5]));
			smph[6]=(fhx) DnFilterH[3]->runFilterH(fhx (smph[6]));
			smph[7]=(fhx) DnFilterH[3]->runFilterH(fhx (smph[7]));
//			smph[0]=fhx (fhx (fhx (smph[0]) + fhx (smph[1])) * fhx (0.50));
//			smph[2]=fhx (fhx (fhx (smph[2]) + fhx (smph[3])) * fhx (0.50));
//			smph[4]=fhx (fhx (fhx (smph[4]) + fhx (smph[5])) * fhx (0.50));
//			smph[6]=fhx (fhx (fhx (smph[6]) + fhx (smph[7])) * fhx (0.50));
			smph[1]=fhx (smph[2]);
			smph[2]=fhx (smph[4]);
			smph[3]=fhx (smph[6]);
		}
		if(osRate>=3)
		{
			smph[0]=(fhx) DnFilterH[2]->runFilterH(fhx (smph[0]));
			smph[1]=(fhx) DnFilterH[2]->runFilterH(fhx (smph[1]));
			smph[2]=(fhx) DnFilterH[2]->runFilterH(fhx (smph[2]));
			smph[3]=(fhx) DnFilterH[2]->runFilterH(fhx (smph[3]));
//			smph[0]=fhx (fhx (fhx (smph[0]) + fhx (smph[1])) * fhx (0.50));
//			smph[2]=fhx (fhx (fhx (smph[2]) + fhx (smph[3])) * fhx (0.50));
			smph[1]=fhx (smph[2]);
		}
		if(osRate>=2)
		{
			smph[0]=(fhx) DnFilterH[1]->runFilterH(fhx (smph[0]));
			smph[1]=(fhx) DnFilterH[1]->runFilterH(fhx (smph[1]));
//			smph[0]=fhx (fhx (fhx (smph[0]) + fhx (smph[1])) * fhx (0.50));
		}
		// smph[0]=(fhx) DnFilterH[0]->runFilterH(fhx (smph[0]));
	}
}
void SampleBank_FP::SetFPprecision(const int nLevel)
{
	precisionLevel = nLevel;
	if(precisionLevel < 0) precisionLevel = 0;
	if(precisionLevel > 2) precisionLevel = 2;
}
fsx SampleBank_FP::GetOutP()
{
	if(precisionLevel == 0) return fsx (smpl[0]);
	if(precisionLevel == 1) return fsx (smps[0]);
	if(precisionLevel == 2) return fsx (smph[0]);
}
