#include "lp_bworth.h"

//! -------------------------------------------------------------------------------------------------------
BTWorthLP::BTWorthLP(fsx inpFreq, fsx inpSRate) :
	PI((fhx) ((fhx) 4.00 * (fhx) atanl((fhx) 1.00))),
	hPI((fhx) ((fhx) PI * (fhx) 0.50)),
	PId(fhx (fhx (PI) * fhx (2.0))),
	zeroF(fhx (0.000000000000000001)),	//! Used to avoid denormal-related issues in parts of the code
	cutOff(fhx (inpFreq))
{
	if(fsx (inpSRate) < fsx (0.0)) setSampleRate(fsx (44100.0));
	else setSampleRate(fsx (inpSRate));
}
//! -------------------------------------------------------------------------------------------------------
void BTWorthLP::setSampleRate(const fsx inpSRate)
{
	sRate = fhx (inpSRate);
	if(fhx (sRate) < fhx (22050.0)) sRate = fhx (22050.0);
	// if(fhx (sRate) > fhx (1536000.0)) sRate = fhx (1536000.0);

	zeroBuffers();

	sT[0] = fhx (fhx (4.0) * fhx (sRate) * fhx (sRate));
	sT[1] = fhx (fhx (8.0) * fhx (sRate) * fhx (sRate));
	sT[2] = fhx (fhx (2.0) * fhx (sRate));
	sT[3] = fhx (fhx (PI) / fhx (sRate));
	wp = fhx (fhx (sT[2]) * (fhx) tanl(fhx (sT[3]) * fhx (cutOff)));

	fhx b1, b2, bd, bd_tmp;
	b1 = fhx (fhx (0.7653670) / fhx (wp));
	b2 = fhx (fhx (1.0) / fhx (fhx (wp) * fhx (wp)));
	bd_tmp = fhx (fhx (fhx (sT[0]) * fhx (b2)) + fhx (1.0));
	bd = fhx (fhx (1.0) / fhx (fhx (bd_tmp) + fhx (fhx (sT[2]) * fhx (b1))));
	sGain = fhx (fhx (bd) * fhx (1.0));
	sCoef[2] = fhx (fhx (2.0) - fhx (fhx (sT[1]) * fhx (b2)));
	sCoef[0] = fhx (fhx (sCoef[2]) * fhx (bd));
	sCoef[1] = fhx (fhx (fhx (bd_tmp) - fhx (fhx (sT[2]) * fhx (b1))) * fhx (bd));
	b1 = fhx (fhx (1.8477590) / fhx (wp));
	bd = fhx (fhx (1.0) / fhx (fhx (bd_tmp) + fhx (fhx (sT[2]) * fhx (b1))));
	sGain *= fhx (bd);
	sCoef[2] *= fhx (bd);
	sCoef[3] = fhx (fhx (fhx (bd_tmp) - fhx (fhx (sT[2]) * fhx (b1))) * fhx (bd));
}
//! -------------------------------------------------------------------------------------------------------
void BTWorthLP::reset()
{
	zeroBuffers();
}
//! -------------------------------------------------------------------------------------------------------
void BTWorthLP::zeroBuffers()
{
	sHist[0] = fhx (0.0);
	sHist[1] = fhx (0.0);
	sHist[2] = fhx (0.0);
	sHist[3] = fhx (0.0);
}
//! -------------------------------------------------------------------------------------------------------
flx BTWorthLP::runFilter(flx inp)
{
	if(flx (inp) == flx (0.0f)) inp = flx (zeroF);

	flx outputS = flx (flx (inp) * flx (sGain));
	outputS -= flx (flx (sHist[0]) * flx (sCoef[0]));
	flx new_hist = flx (flx (outputS) - flx (flx (sHist[1]) * flx (sCoef[1])));

	outputS = flx (flx (new_hist) + flx (flx (sHist[0]) * flx (2.0f)));
	outputS += flx (sHist[1]);

	sHist[1] = flx (sHist[0]);
	sHist[0] = flx (new_hist);

	outputS -= flx (flx (sHist[2]) * flx (sCoef[2]));
	new_hist = flx (flx (outputS) - flx (flx (sHist[3]) * flx (sCoef[3])));

	outputS = flx (flx (new_hist) + flx (flx (sHist[2]) * flx (2.0f)));
	outputS += flx (sHist[3]);

	sHist[3] = flx (sHist[2]);
	sHist[2] = flx (new_hist);

	return flx (outputS);
}
//! -------------------------------------------------------------------------------------------------------
fsx BTWorthLP::runFilter(fsx inp)
{
	if(fsx (inp) == fsx (0.0)) inp = fsx (zeroF);

	fsx outputS = fsx (fsx (inp) * fsx (sGain));
	outputS -= fsx (fsx (sHist[0]) * fsx (sCoef[0]));
	fsx new_hist = fsx (fsx (outputS) - fsx (fsx (sHist[1]) * fsx (sCoef[1])));

	outputS = fsx (fsx (new_hist) + fsx (fsx (sHist[0]) * fsx (2.0)));
	outputS += fsx (sHist[1]);

	sHist[1] = fsx (sHist[0]);
	sHist[0] = fsx (new_hist);

	outputS -= fsx (fsx (sHist[2]) * fsx (sCoef[2]));
	new_hist = fsx (fsx (outputS) - fsx (fsx (sHist[3]) * fsx (sCoef[3])));

	outputS = fsx (fsx (new_hist) + fsx (fsx (sHist[2]) * fsx (2.0)));
	outputS += fsx (sHist[3]);

	sHist[3] = fsx (sHist[2]);
	sHist[2] = fsx (new_hist);

	return fsx (outputS);
}
//! -------------------------------------------------------------------------------------------------------
fhx BTWorthLP::runFilter(fhx inp)
{
	if(fhx (inp) == fhx (0.0)) inp = fhx (zeroF);

	fhx outputS = fhx (fhx (inp) * fhx (sGain));
	outputS -= fhx (fhx (sHist[0]) * fhx (sCoef[0]));
	fhx new_hist = fhx (fhx (outputS) - fhx (fhx (sHist[1]) * fhx (sCoef[1])));

	outputS = fhx (fhx (new_hist) + fhx (fhx (sHist[0]) * fhx (2.0)));
	outputS += fhx (sHist[1]);

	sHist[1] = fhx (sHist[0]);
	sHist[0] = fhx (new_hist);

	outputS -= fhx (fhx (sHist[2]) * fhx (sCoef[2]));
	new_hist = fhx (fhx (outputS) - fhx (fhx (sHist[3]) * fhx (sCoef[3])));

	outputS = fhx (fhx (new_hist) + fhx (fhx (sHist[2]) * fhx (2.0)));
	outputS += fhx (sHist[3]);

	sHist[3] = fhx (sHist[2]);
	sHist[2] = fhx (new_hist);

	return fhx (outputS);
}
//! -------------------------------------------------------------------------------------------------------
void BTWorthLP::changeFreq(fsx nFreq)
{
	cutOff = fhx (nFreq);

	wp = fhx (fhx (sT[2]) * (fhx) tanl(fhx (sT[3]) * fhx (cutOff)));

	fhx b1, b2, bd, bd_tmp;
	b1 = fhx (fhx (0.7653670) / fhx (wp));
	b2 = fhx (fhx (1.0) / fhx (fhx (wp) * fhx (wp)));
	bd_tmp = fhx (fhx (fhx (sT[0]) * fhx (b2)) + fhx (1.0));
	bd = fhx (fhx (1.0) / fhx (fhx (bd_tmp) + fhx (fhx (sT[2]) * fhx (b1))));
	sGain = fhx (fhx (bd) * fhx (1.0));
	sCoef[2] = fhx (fhx (2.0) - fhx (fhx (sT[1]) * fhx (b2)));
	sCoef[0] = fhx (fhx (sCoef[2]) * fhx (bd));
	sCoef[1] = fhx (fhx (fhx (bd_tmp) - fhx (fhx (sT[2]) * fhx (b1))) * fhx (bd));
	b1 = fhx (fhx (1.8477590) / fhx (wp));
	bd = fhx (fhx (1.0) / fhx (fhx (bd_tmp) + fhx (fhx (sT[2]) * fhx (b1))));
	sGain *= fhx (bd);
	sCoef[2] *= fhx (bd);
	sCoef[3] = fhx (fhx (fhx (bd_tmp) - fhx (fhx (sT[2]) * fhx (b1))) * fhx (bd));
}
//! -------------------------------------------------------------------------------------------------------
//! -------------------------------------------------------------------------------------------------------
//! -------------------------------------------------------------------------------------------------------

//! -------------------------------------------------------------------------------------------------------
BTWorthLP_LD::BTWorthLP_LD(fsx inpFreq, fsx inpSRate) :
	PI((flx) ((fsx) 4.00f * (fsx) atanl((fsx) 1.00f))),
	hPI((flx) ((fsx) PI * (fsx) 0.50f)),
	PId(flx (fsx (PI) * fsx (2.0f))),
	zeroF(flx (0.000000001f)),						//! Used to avoid denormal-related issues in parts of the code
	cutOff(flx (inpFreq))
{
	if(fsx (inpSRate) < fsx (0.0f)) setSampleRate(fsx (44100.0f));
	else setSampleRate(fsx (inpSRate));
}
//! -------------------------------------------------------------------------------------------------------
void BTWorthLP_LD::setSampleRate(const fsx inpSRate)
{
	sRate = fsx (inpSRate);
	if(fsx (sRate) < fsx (44100.0)) sRate = fsx (44100.0);
	// if(fsx (sRate) > fsx (1536000.0)) sRate = fsx (1536000.0);

	zeroBuffers();

	sT[0] = flx (flx (4.0f) * flx (sRate) * flx (sRate));
	sT[1] = flx (flx (8.0f) * flx (sRate) * flx (sRate));
	sT[2] = flx (flx (2.0f) * flx (sRate));
	sT[3] = flx (flx (PI) / flx (sRate));
	wp = flx (flx (sT[2]) * (flx) tan(flx (sT[3]) * flx (cutOff)));

	flx b1, b2, bd, bd_tmp;
	b1 = flx (flx (0.7653670f) / flx (wp));
	b2 = flx (flx (1.0f) / flx (flx (wp) * flx (wp)));
	bd_tmp = flx (flx (flx (sT[0]) * flx (b2)) + flx (1.0f));
	bd = flx (flx (1.0f) / flx (flx (bd_tmp) + flx (flx (sT[2]) * flx (b1))));
	sGain = flx (flx (bd) * flx (1.0));
	sCoef[2] = flx (flx (2.0f) - flx (flx (sT[1]) * flx (b2)));
	sCoef[0] = flx (flx (sCoef[2]) * flx (bd));
	sCoef[1] = flx (flx (flx (bd_tmp) - flx (flx (sT[2]) * flx (b1))) * flx (bd));
	b1 = flx (flx (1.8477590f) / flx (wp));
	bd = flx (flx (1.0f) / flx (flx (bd_tmp) + flx (flx (sT[2]) * flx (b1))));
	sGain *= flx (bd);
	sCoef[2] *= flx (bd);
	sCoef[3] = flx (flx (flx (bd_tmp) - flx (flx (sT[2]) * flx (b1))) * flx (bd));
}
//! -------------------------------------------------------------------------------------------------------
void BTWorthLP_LD::reset()
{
	zeroBuffers();
}
//! -------------------------------------------------------------------------------------------------------
void BTWorthLP_LD::zeroBuffers()
{
	sHist[0] = flx (0.0f);
	sHist[1] = flx (0.0f);
	sHist[2] = flx (0.0f);
	sHist[3] = flx (0.0f);
}
//! -------------------------------------------------------------------------------------------------------
flx BTWorthLP_LD::runFilterL(flx inp)
{
	if(flx (inp) == fsx (0.0f)) inp = flx (zeroF);

	flx outputS = flx (flx (inp) * flx (sGain));
	outputS -= flx (flx (sHist[0]) * flx (sCoef[0]));
	flx new_hist = flx (flx (outputS) - flx (flx (sHist[1]) * flx (sCoef[1])));

	outputS = flx (flx (new_hist) + flx (flx (sHist[0]) * flx (2.0)));
	outputS += flx (sHist[1]);

	sHist[1] = flx (sHist[0]);
	sHist[0] = flx (new_hist);

	outputS -= flx (flx (sHist[2]) * flx (sCoef[2]));
	new_hist = flx (flx (outputS) - flx (flx (sHist[3]) * flx (sCoef[3])));

	outputS = flx (flx (new_hist) + flx (flx (sHist[2]) * flx (2.0)));
	outputS += flx (sHist[3]);

	sHist[3] = flx (sHist[2]);
	sHist[2] = flx (new_hist);

	return flx (outputS);
}
//! -------------------------------------------------------------------------------------------------------
void BTWorthLP_LD::changeFreq(fsx nFreq)
{
	cutOff = fsx (nFreq);

	sT[0] = flx (flx (4.0f) * flx (sRate) * flx (sRate));
	sT[1] = flx (flx (8.0f) * flx (sRate) * flx (sRate));
	sT[2] = flx (flx (2.0f) * flx (sRate));
	sT[3] = flx (flx (PI) / flx (sRate));
	wp = flx (flx (sT[2]) * (flx) tan(flx (sT[3]) * flx (cutOff)));

	flx b1, b2, bd, bd_tmp;
	b1 = flx (flx (0.7653670f) / flx (wp));
	b2 = flx (flx (1.0f) / flx (flx (wp) * flx (wp)));
	bd_tmp = flx (flx (flx (sT[0]) * flx (b2)) + flx (1.0f));
	bd = flx (flx (1.0f) / flx (flx (bd_tmp) + flx (flx (sT[2]) * flx (b1))));
	sGain = flx (flx (bd) * flx (1.0f));
	sCoef[2] = flx (flx (2.0f) - flx (flx (sT[1]) * flx (b2)));
	sCoef[0] = flx (flx (sCoef[2]) * flx (bd));
	sCoef[1] = flx (flx (flx (bd_tmp) - flx (flx (sT[2]) * flx (b1))) * flx (bd));
	b1 = flx (flx (1.8477590f) / flx (wp));
	bd = flx (flx (1.0f) / flx (flx (bd_tmp) + flx (flx (sT[2]) * flx (b1))));
	sGain *= flx (bd);
	sCoef[2] *= flx (bd);
	sCoef[3] = flx (flx (flx (bd_tmp) - flx (flx (sT[2]) * flx (b1))) * flx (bd));
}
//! -------------------------------------------------------------------------------------------------------
//! -------------------------------------------------------------------------------------------------------
//! -------------------------------------------------------------------------------------------------------

//! -------------------------------------------------------------------------------------------------------
BTWorthLP_SD::BTWorthLP_SD(fsx inpFreq, fsx inpSRate) :
	PI((fsx) ((fsx) 4.00 * (fsx) atanl((fsx) 1.00))),
	hPI((fsx) ((fsx) PI * (fsx) 0.50)),
	PId(fsx (fsx (PI) * fsx (2.0))),
	zeroF(fsx (0.000000000001)),						//! Used to avoid denormal-related issues in parts of the code
	cutOff(fsx (inpFreq))
{
	if(fsx (inpSRate) < fsx (0.0)) setSampleRate(fsx (44100.0));
	else setSampleRate(fsx (inpSRate));
}
//! -------------------------------------------------------------------------------------------------------
void BTWorthLP_SD::setSampleRate(const fsx inpSRate)
{
	sRate = fsx (inpSRate);
	if(fsx (sRate) < fsx (44100.0)) sRate = fsx (44100.0);
	// if(fsx (sRate) > fsx (1536000.0)) sRate = fsx (1536000.0);

	zeroBuffers();

	sT[0] = fsx (fsx (4.0) * fsx (sRate) * fsx (sRate));
	sT[1] = fsx (fsx (8.0) * fsx (sRate) * fsx (sRate));
	sT[2] = fsx (fsx (2.0) * fsx (sRate));
	sT[3] = fsx (fsx (PI) / fsx (sRate));
	wp = fsx (fsx (sT[2]) * (fsx) tan(fsx (sT[3]) * fsx (cutOff)));

	fsx b1, b2, bd, bd_tmp;
	b1 = fsx (fsx (0.7653670) / fsx (wp));
	b2 = fsx (fsx (1.0) / fsx (fsx (wp) * fsx (wp)));
	bd_tmp = fsx (fsx (fsx (sT[0]) * fsx (b2)) + fsx (1.0));
	bd = fsx (fsx (1.0) / fsx (fsx (bd_tmp) + fsx (fsx (sT[2]) * fsx (b1))));
	sGain = fsx (fsx (bd) * fsx (1.0));
	sCoef[2] = fsx (fsx (2.0) - fsx (fsx (sT[1]) * fsx (b2)));
	sCoef[0] = fsx (fsx (sCoef[2]) * fsx (bd));
	sCoef[1] = fsx (fsx (fsx (bd_tmp) - fsx (fsx (sT[2]) * fsx (b1))) * fsx (bd));
	b1 = fsx (fsx (1.8477590) / fsx (wp));
	bd = fsx (fsx (1.0) / fsx (fsx (bd_tmp) + fsx (fsx (sT[2]) * fsx (b1))));
	sGain *= fsx (bd);
	sCoef[2] *= fsx (bd);
	sCoef[3] = fsx (fsx (fsx (bd_tmp) - fsx (fsx (sT[2]) * fsx (b1))) * fsx (bd));
}
//! -------------------------------------------------------------------------------------------------------
void BTWorthLP_SD::reset()
{
	zeroBuffers();
}
//! -------------------------------------------------------------------------------------------------------
void BTWorthLP_SD::zeroBuffers()
{
	sHist[0] = fsx (0.0);
	sHist[1] = fsx (0.0);
	sHist[2] = fsx (0.0);
	sHist[3] = fsx (0.0);
}
//! -------------------------------------------------------------------------------------------------------
fsx BTWorthLP_SD::runFilterS(fsx inp)
{
	if(fsx (inp) == fsx (0.0)) inp = fsx (zeroF);

	fsx outputS = fsx (fsx (inp) * fsx (sGain));
	outputS -= fsx (fsx (sHist[0]) * fsx (sCoef[0]));
	fsx new_hist = fsx (fsx (outputS) - fsx (fsx (sHist[1]) * fsx (sCoef[1])));

	outputS = fsx (fsx (new_hist) + fsx (fsx (sHist[0]) * fsx (2.0)));
	outputS += fsx (sHist[1]);

	sHist[1] = fsx (sHist[0]);
	sHist[0] = fsx (new_hist);

	outputS -= fsx (fsx (sHist[2]) * fsx (sCoef[2]));
	new_hist = fsx (fsx (outputS) - fsx (fsx (sHist[3]) * fsx (sCoef[3])));

	outputS = fsx (fsx (new_hist) + fsx (fsx (sHist[2]) * fsx (2.0)));
	outputS += fsx (sHist[3]);

	sHist[3] = fsx (sHist[2]);
	sHist[2] = fsx (new_hist);

	return fsx (outputS);
}
//! -------------------------------------------------------------------------------------------------------
void BTWorthLP_SD::changeFreq(fsx nFreq)
{
	cutOff = fsx (nFreq);

	sT[0] = fsx (fsx (4.0) * fsx (sRate) * fsx (sRate));
	sT[1] = fsx (fsx (8.0) * fsx (sRate) * fsx (sRate));
	sT[2] = fsx (fsx (2.0) * fsx (sRate));
	sT[3] = fsx (fsx (PI) / fsx (sRate));
	wp = fsx (fsx (sT[2]) * (fsx) tan(fsx (sT[3]) * fsx (cutOff)));

	fsx b1, b2, bd, bd_tmp;
	b1 = fsx (fsx (0.7653670) / fsx (wp));
	b2 = fsx (fsx (1.0) / fsx (fsx (wp) * fsx (wp)));
	bd_tmp = fsx (fsx (fsx (sT[0]) * fsx (b2)) + fsx (1.0));
	bd = fsx (fsx (1.0) / fsx (fsx (bd_tmp) + fsx (fsx (sT[2]) * fsx (b1))));
	sGain = fsx (fsx (bd) * fsx (1.0));
	sCoef[2] = fsx (fsx (2.0) - fsx (fsx (sT[1]) * fsx (b2)));
	sCoef[0] = fsx (fsx (sCoef[2]) * fsx (bd));
	sCoef[1] = fsx (fsx (fsx (bd_tmp) - fsx (fsx (sT[2]) * fsx (b1))) * fsx (bd));
	b1 = fsx (fsx (1.8477590) / fsx (wp));
	bd = fsx (fsx (1.0) / fsx (fsx (bd_tmp) + fsx (fsx (sT[2]) * fsx (b1))));
	sGain *= fsx (bd);
	sCoef[2] *= fsx (bd);
	sCoef[3] = fsx (fsx (fsx (bd_tmp) - fsx (fsx (sT[2]) * fsx (b1))) * fsx (bd));
}
//! -------------------------------------------------------------------------------------------------------
//! -------------------------------------------------------------------------------------------------------
//! -------------------------------------------------------------------------------------------------------

//! -------------------------------------------------------------------------------------------------------
BTWorthLP_HD::BTWorthLP_HD(fsx inpFreq, fsx inpSRate) :
	PI((fhx) ((fhx) 4.00 * (fhx) atanl((fhx) 1.00))),
	hPI((fhx) ((fhx) PI * (fhx) 0.50)),
	PId(fhx (fhx (PI) * fhx (2.0))),
	zeroF(fhx (0.000000000000001)),						//! Used to avoid denormal-related issues in parts of the code
	cutOff(fhx (inpFreq))
{
	if(fsx (inpSRate) < fsx (0.0)) setSampleRate(fsx (44100.0));
	else setSampleRate(fsx (inpSRate));
}
//! -------------------------------------------------------------------------------------------------------
void BTWorthLP_HD::setSampleRate(const fsx inpSRate)
{
	sRate = fhx (inpSRate);
	if(fhx (sRate) < fhx (44100.0)) sRate = fhx (44100.0);
	// if(fhx (sRate) > fhx (1536000.0)) sRate = fhx (1536000.0);

	zeroBuffers();

	sT[0] = fhx (fhx (4.0) * fhx (sRate) * fhx (sRate));
	sT[1] = fhx (fhx (8.0) * fhx (sRate) * fhx (sRate));
	sT[2] = fhx (fhx (2.0) * fhx (sRate));
	sT[3] = fhx (fhx (PI) / fhx (sRate));
	wp = fhx (fhx (sT[2]) * (fhx) tanl(fhx (sT[3]) * fhx (cutOff)));

	fhx b1, b2, bd, bd_tmp;
	b1 = fhx (fhx (0.7653670) / fhx (wp));
	b2 = fhx (fhx (1.0) / fhx (fhx (wp) * fhx (wp)));
	bd_tmp = fhx (fhx (fhx (sT[0]) * fhx (b2)) + fhx (1.0));
	bd = fhx (fhx (1.0) / fhx (fhx (bd_tmp) + fhx (fhx (sT[2]) * fhx (b1))));
	sGain = fhx (fhx (bd) * fhx (1.0));
	sCoef[2] = fhx (fhx (2.0) - fhx (fhx (sT[1]) * fhx (b2)));
	sCoef[0] = fhx (fhx (sCoef[2]) * fhx (bd));
	sCoef[1] = fhx (fhx (fhx (bd_tmp) - fhx (fhx (sT[2]) * fhx (b1))) * fhx (bd));
	b1 = fhx (fhx (1.8477590) / fhx (wp));
	bd = fhx (fhx (1.0) / fhx (fhx (bd_tmp) + fhx (fhx (sT[2]) * fhx (b1))));
	sGain *= fhx (bd);
	sCoef[2] *= fhx (bd);
	sCoef[3] = fhx (fhx (fhx (bd_tmp) - fhx (fhx (sT[2]) * fhx (b1))) * fhx (bd));
}
//! -------------------------------------------------------------------------------------------------------
void BTWorthLP_HD::reset()
{
	zeroBuffers();
}
//! -------------------------------------------------------------------------------------------------------
void BTWorthLP_HD::zeroBuffers()
{
	sHist[0] = fhx (0.0);
	sHist[1] = fhx (0.0);
	sHist[2] = fhx (0.0);
	sHist[3] = fhx (0.0);
}
//! -------------------------------------------------------------------------------------------------------
fhx BTWorthLP_HD::runFilterH(fhx inp)
{
	if(fhx (inp) == fhx (0.0)) inp = fhx (zeroF);

	fhx outputS = fhx (fhx (inp) * fhx (sGain));
	outputS -= fhx (fhx (sHist[0]) * fhx (sCoef[0]));
	fhx new_hist = fhx (fhx (outputS) - fhx (fhx (sHist[1]) * fhx (sCoef[1])));

	outputS = fhx (fhx (new_hist) + fhx (fhx (sHist[0]) * fhx (2.0)));
	outputS += fhx (sHist[1]);

	sHist[1] = fhx (sHist[0]);
	sHist[0] = fhx (new_hist);

	outputS -= fhx (fhx (sHist[2]) * fhx (sCoef[2]));
	new_hist = fhx (fhx (outputS) - fhx (fhx (sHist[3]) * fhx (sCoef[3])));

	outputS = fhx (fhx (new_hist) + fhx (fhx (sHist[2]) * fhx (2.0)));
	outputS += fhx (sHist[3]);

	sHist[3] = fhx (sHist[2]);
	sHist[2] = fhx (new_hist);

	return fhx (outputS);
}
//! -------------------------------------------------------------------------------------------------------
void BTWorthLP_HD::changeFreq(fsx nFreq)
{
	cutOff = fhx (nFreq);

	sT[0] = fhx (fhx (4.0) * fhx (sRate) * fhx (sRate));
	sT[1] = fhx (fhx (8.0) * fhx (sRate) * fhx (sRate));
	sT[2] = fhx (fhx (2.0) * fhx (sRate));
	sT[3] = fhx (fhx (PI) / fhx (sRate));
	wp = fhx (fhx (sT[2]) * (fhx) tanl(fhx (sT[3]) * fhx (cutOff)));

	fhx b1, b2, bd, bd_tmp;
	b1 = fhx (fhx (0.7653670) / fhx (wp));
	b2 = fhx (fhx (1.0) / fhx (fhx (wp) * fhx (wp)));
	bd_tmp = fhx (fhx (fhx (sT[0]) * fhx (b2)) + fhx (1.0));
	bd = fhx (fhx (1.0) / fhx (fhx (bd_tmp) + fhx (fhx (sT[2]) * fhx (b1))));
	sGain = fhx (fhx (bd) * fhx (1.0));
	sCoef[2] = fhx (fhx (2.0) - fhx (fhx (sT[1]) * fhx (b2)));
	sCoef[0] = fhx (fhx (sCoef[2]) * fhx (bd));
	sCoef[1] = fhx (fhx (fhx (bd_tmp) - fhx (fhx (sT[2]) * fhx (b1))) * fhx (bd));
	b1 = fhx (fhx (1.8477590) / fhx (wp));
	bd = fhx (fhx (1.0) / fhx (fhx (bd_tmp) + fhx (fhx (sT[2]) * fhx (b1))));
	sGain *= fhx (bd);
	sCoef[2] *= fhx (bd);
	sCoef[3] = fhx (fhx (fhx (bd_tmp) - fhx (fhx (sT[2]) * fhx (b1))) * fhx (bd));
}
//! -------------------------------------------------------------------------------------------------------
//! -------------------------------------------------------------------------------------------------------
//! -------------------------------------------------------------------------------------------------------
