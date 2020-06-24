#include "sample_bank_custom.h"

SampleBank_Cst::SampleBank_Cst(fsx iSRate) :
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

	UpFilter[0] = new BTW_LP(fsx (fhx (sRateH * 0.50)), fsx (fhx (sRateH * 2.0)));
	UpFilter[1] = new BTW_LP(fsx (fhx (sRateH * 0.50)), fsx (fhx (sRateH * 2.0)));
	DnFilter[0] = new BTW_LP(fsx (fhx (sRateH * 0.50)), fsx (fhx (sRateH * 2.0)));
	DnFilter[1] = new BTW_LP(fsx (fhx (sRateH * 0.50)), fsx (fhx (sRateH * 2.0)));

	zeroAllSamples();
}
SampleBank_Cst::~SampleBank_Cst()
{
	for(int x = 0; x < 2; x++)
	{
		delete UpFilter[x];
		delete DnFilter[x];
	}

	delete rfV;
}
void SampleBank_Cst::InputSmp(const fsx inp)
{
	if(precisionLevel == 0) smpl[0] = flx (inp);
	if(precisionLevel == 1) smps[0] = fsx (inp);
	if(precisionLevel == 2) smph[0] = fhx (inp);
}
void SampleBank_Cst::setSampleRate(fsx nSRate)
{
	sRateL = flx (nSRate);
	sRateS = fsx (nSRate);
	sRateH = fhx (nSRate);

	cRateL = flx (rfV->vl[osRate] * flx (sRateL));
	cRateS = fsx (rfV->vs[osRate] * fsx (sRateS));
	cRateH = fhx (rfV->vh[osRate] * fhx (sRateH));

	UpFilter[0]->setSampleRate(fsx (cRateS));
	UpFilter[1]->setSampleRate(fsx (cRateS));

	DnFilter[0]->setSampleRate(fsx (cRateS));
	DnFilter[1]->setSampleRate(fsx (cRateS));

	UpFilter[0]->changeFreq(sRateS * 0.250);
	UpFilter[1]->changeFreq(sRateS * 0.250);

	DnFilter[0]->changeFreq(sRateS * 0.250);
	DnFilter[1]->changeFreq(sRateS * 0.250);

	zeroAllSamples();
}
void SampleBank_Cst::zeroAllSamples()
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
void SampleBank_Cst::setOverSampling(const int oSLevel)
{
	if(osRate < 0) return;
	if(osRate > 6) return;

	osRate = int (oSLevel);
	cRateL = flx (rfV->vl[osRate] * flx (sRateL));
	cRateS = fsx (rfV->vs[osRate] * fsx (sRateS));
	cRateH = fhx (rfV->vh[osRate] * fhx (sRateH));

	UpFilter[0]->setSampleRate(fsx (cRateS));
	UpFilter[1]->setSampleRate(fsx (cRateS));

	DnFilter[0]->setSampleRate(fsx (cRateS));
	DnFilter[1]->setSampleRate(fsx (cRateS));

	UpFilter[0]->changeFreq(sRateS * 0.250);
	UpFilter[1]->changeFreq(sRateS * 0.250);

	DnFilter[0]->changeFreq(sRateS * 0.250);
	DnFilter[1]->changeFreq(sRateS * 0.250);

	zeroAllSamples();
}
void SampleBank_Cst::runUpSampling()
{
	if(osRate > 5) return;
	if(osRate < 2) return;

	if(precisionLevel == 0)
	{
		for (unsigned int x = 1; x < rfV->vi[osRate]; x++)
			smpl[x] = 0.0f;
		for (unsigned int y = 0; y < rfV->vi[osRate]; y++)
		{
			UpFilter[0]->inpSplL(smpl[y]);
			UpFilter[0]->RunFilter();
			smpl[y] = UpFilter[0]->GetOutPutL();

			UpFilter[1]->inpSplL(smpl[y]);
			UpFilter[1]->RunFilter();
			smpl[y] = UpFilter[1]->GetOutPutL();

			smpl[y] *= flx (rfV->vl[osRate]);
		}
	}
	if(precisionLevel == 1)
	{
		for (unsigned int x = 1; x < rfV->vi[osRate]; x++)
			smps[x] = 0.0;
		for (unsigned int y = 0; y < rfV->vi[osRate]; y++)
		{
			UpFilter[0]->inpSplS(smps[y]);
			UpFilter[0]->RunFilter();
			smpl[y] = UpFilter[0]->GetOutPutS();

			UpFilter[1]->inpSplS(smps[y]);
			UpFilter[1]->RunFilter();
			smpl[y] = UpFilter[1]->GetOutPutS();

			smps[y] *= fsx (rfV->vs[osRate]);
		}
	}
	if(precisionLevel == 2)
	{
		for (unsigned int x = 1; x < rfV->vi[osRate]; x++)
			smph[x] = 0.0;
		for (unsigned int y = 0; y < rfV->vi[osRate]; y++)
		{
			UpFilter[0]->inpSplH(smph[y]);
			UpFilter[0]->RunFilter();
			smpl[y] = UpFilter[0]->GetOutPutH();

			UpFilter[1]->inpSplH(smph[y]);
			UpFilter[1]->RunFilter();
			smpl[y] = UpFilter[1]->GetOutPutH();

			smph[y] *= fhx (rfV->vh[osRate]);
		}
	}
}
void SampleBank_Cst::runDownSampling()
{
	if(osRate > 5) return;
	if(osRate < 2) return;

	if(precisionLevel == 0)
	{
		for (unsigned int y = 0; y < rfV->vi[osRate]; y++)
		{
			DnFilter[0]->inpSplL(smpl[y]);
			DnFilter[0]->RunFilter();
			smpl[y] = (flx) DnFilter[0]->GetOutPutL();

			DnFilter[1]->inpSplL(smpl[y]);
			DnFilter[1]->RunFilter();
			smpl[y] = (flx) DnFilter[1]->GetOutPutL();
		}
	}
	if(precisionLevel == 1)
	{
		for (unsigned int y = 0; y < rfV->vi[osRate]; y++)
		{
			DnFilter[0]->inpSplS(smps[y]);
			DnFilter[0]->RunFilter();
			smps[y] = (fsx) DnFilter[0]->GetOutPutS();

			DnFilter[1]->inpSplS(smps[y]);
			DnFilter[1]->RunFilter();
			smps[y] = (fsx) DnFilter[1]->GetOutPutS();
		}
	}
	if(precisionLevel == 2)
	{
		for (unsigned int y = 0; y < rfV->vi[osRate]; y++)
		{
			DnFilter[0]->inpSplH(smph[y]);
			DnFilter[0]->RunFilter();
			smph[y] = (fhx) DnFilter[0]->GetOutPutH();

			DnFilter[1]->inpSplH(smph[y]);
			DnFilter[1]->RunFilter();
			smph[y] = (fhx) DnFilter[1]->GetOutPutH();
		}
	}
}
void SampleBank_Cst::SetFPprecision(const int nLevel)
{
	precisionLevel = nLevel;
	if(precisionLevel < 0) precisionLevel = 0;
	if(precisionLevel > 2) precisionLevel = 2;
}
fsx SampleBank_Cst::GetOutP()
{
	if(precisionLevel == 0) return fsx (smpl[0]);
	if(precisionLevel == 1) return fsx (smps[0]);
	if(precisionLevel == 2) return fsx (smph[0]);
}
flx SampleBank_Cst::GetOutPL()
{
	return flx (smpl[0]);
}
fsx SampleBank_Cst::GetOutPS()
{
	return fsx (smps[0]);
}
fhx SampleBank_Cst::GetOutPH()
{
	return fhx (smph[0]);
}
