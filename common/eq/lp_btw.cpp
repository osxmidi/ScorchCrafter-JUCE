#include "lp_btw.h"

BTW_LP::BTW_LP(fsx inpFreq, fsx inpSRate) :
	l_PI(flx (M_PI)),
	s_PI(fsx (M_PI)),
	h_PI(fhx (M_PI)),
	l_cutOff(flx (inpFreq)),
	s_cutOff(fsx (inpFreq)),
	h_cutOff(fhx (inpFreq)),
	l_sRate(flx (inpSRate)),
	s_sRate(fsx (inpSRate)),
	h_sRate(fhx (inpSRate)),
	precLevel(1)
{
	FilterInit();
}
BTW_LP::~BTW_LP()
{
	// nothing
}
void BTW_LP::FilterInit()
{
	h_sT[0] = fhx (4.0 * h_sRate * h_sRate);
	h_sT[1] = fhx (8.0 * h_sRate * h_sRate);
	h_sT[2] = fhx (2.0 * h_sRate);
	h_sT[3] = fhx (h_PI / h_sRate);

	h_wp = fhx (h_sT[2] * tanl(h_sT[3] * h_cutOff));

	fhx b1 = fhx (0.7653670 / h_wp);
	fhx b2 = fhx (1.0 / (h_wp * h_wp));
	fhx bd_tmp = fhx (h_sT[0] * b2 + 1.0);
	fhx bd = fhx (1.0 / (bd_tmp + h_sT[2] * b1));
	h_sGain = bd;
	h_sCoef[2] = fhx (2.0 - h_sT[1] * b2);
	h_sCoef[0] = fhx (h_sCoef[2] * bd);
	h_sCoef[1] = fhx ((bd_tmp - h_sT[2] * b1) * bd);
	b1 = fhx (1.8477590 / h_wp);
	bd = fhx (1.0 / (bd_tmp + h_sT[2] * b1));
	h_sGain *= bd;
	h_sCoef[2] *= bd;
	h_sCoef[3] = fhx ((bd_tmp - h_sT[2] * b1) * bd);

	for (int x = 0; x < 4; x++)
	{
		s_sT[x] = fsx (h_sT[x]);
		s_sCoef[x] = fsx (h_sCoef[x]);
		l_sT[x] = flx (s_sT[x]);
		l_sCoef[x] = flx (s_sCoef[x]);
	}
	s_wp = fsx (h_wp);
	s_sGain = fsx (h_sGain);
	l_wp = flx (s_wp);
	l_sGain = flx (s_sGain);

	zeroBuffers();
}
void BTW_LP::setSampleRate(const fsx inpSRate)
{
	h_sRate = fhx (inpSRate);
	s_sRate = fsx (inpSRate);
	l_sRate = flx (inpSRate);

	FilterInit();
}
void BTW_LP::zeroBuffers()
{
	for (int x = 0; x < 5; x++)
	{
		l_sHist[x] = flx (0.f);
		s_sHist[x] = fsx (0.0);
		h_sHist[x] = fhx (0.0);
	}
}
void BTW_LP::changeFreq(const fsx nFreq)
{
	l_cutOff = flx (nFreq);
	s_cutOff = fsx (nFreq);
	h_cutOff = fhx (nFreq);

	FilterInit();
}
void BTW_LP::RunFilter()
{
	if (precLevel == 2)
	{
		fhx output = fhx (h_smp * h_sGain);
		output -= fhx (h_sHist[1] * h_sCoef[0]);
		fhx new_hist = fhx (output - h_sHist[2] * h_sCoef[1]);
		output = fhx (new_hist + h_sHist[1] * 2.0);
		output += h_sHist[2];
		h_sHist[2] = h_sHist[1];
		h_sHist[1] = new_hist;

		output -= fhx (h_sHist[3] * h_sCoef[2]);
		new_hist = fhx (output - h_sHist[4] * h_sCoef[3]);
		output = fhx (new_hist + h_sHist[3] * 2.0);
		output += h_sHist[4];
		h_sHist[4] = h_sHist[3];
		h_sHist[3] = new_hist;

		h_smp = output;
	}
	if (precLevel == 1)
	{
		fsx output = fsx (s_smp * s_sGain);
		output -= fsx (s_sHist[1] * s_sCoef[0]);
		fsx new_hist = fsx (output - s_sHist[2] * s_sCoef[1]);
		output = fsx (new_hist + s_sHist[1] * 2.0);
		output += s_sHist[2];
		s_sHist[2] = s_sHist[1];
		s_sHist[1] = new_hist;

		output -= fsx (s_sHist[3] * s_sCoef[2]);
		new_hist = fsx (output - s_sHist[4] * s_sCoef[3]);
		output = fsx (new_hist + s_sHist[3] * 2.0);
		output += s_sHist[4];
		s_sHist[4] = s_sHist[3];
		s_sHist[3] = new_hist;

		s_smp = output;
	}
	if (precLevel == 0)
	{
		flx output = flx (l_smp * l_sGain);
		output -= flx (l_sHist[1] * l_sCoef[0]);
		flx new_hist = flx (output - l_sHist[2] * l_sCoef[1]);
		output = flx (new_hist + l_sHist[1] * 2.0f);
		output += l_sHist[2];
		l_sHist[2] = l_sHist[1];
		l_sHist[1] = new_hist;

		output -= flx (l_sHist[3] * l_sCoef[2]);
		new_hist = flx (output - l_sHist[4] * l_sCoef[3]);
		output = flx (new_hist + l_sHist[3] * 2.0f);
		output += l_sHist[4];
		l_sHist[4] = l_sHist[3];
		l_sHist[3] = new_hist;

		l_smp = output;
	}
}
void BTW_LP::inpSplL(const flx inp)
{
	l_smp = inp;
	precLevel = 0;
}
void BTW_LP::inpSplS(const fsx inp)
{
	s_smp = inp;
	precLevel = 1;
}
void BTW_LP::inpSplH(const fhx inp)
{
	h_smp = inp;
	precLevel = 2;
}
flx BTW_LP::GetOutPutL()
{
	return l_smp;
}
fsx BTW_LP::GetOutPutS()
{
	return s_smp;
}
fhx BTW_LP::GetOutPutH()
{
	return h_smp;
}
