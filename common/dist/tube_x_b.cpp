#include "tube_x_b.h"

DstTubeXb::DstTubeXb(fsx inSRate, const fhx kFF) :
	PI((fhx) ((fhx) 4.00 * (fhx) atanl((fhx) 1.00))),
	hPI((fhx) ((fhx) PI * (fhx) 0.50)),
	PId(fhx (fhx (PI) * fhx (2.0))),
	zeroF(fhx (0.0000000000000000000000000001)),		//! Used to avoid denormal-related issues in parts of the code
	sRate(fhx (inSRate)),
	oSampling(int (1)),
	rateFac(fhx (1.0)),
	kFMult(fhx (fhx (kFF) * fhx (0.010))),
	gGain(fhx (zeroF))
{
	if(fhx (sRate) < fhx (44100.0)) sRate = fhx (44100.0);
	if(fhx (sRate) > fhx (1536000.0)) sRate = fhx (1536000.0);

	const fhx kFactor = fhx (fhx (44100.0) / fhx (fhx (sRate) * fhx (rateFac)));
	kb = fhx (fhx (kFMult) * fhx (kFactor));
	ka = fhx (fhx (1.0) - fhx (kb));
	lim = fhx (0.40);
	tgt_drve = fhx (1.0);
	drve = fhx (1.0);
	drvo = fhx (1.0);
	kr = fhx (0.150);
	kabs = fhx (0.150);
	trim = fhx ((fhx) powl(fhx (2.0), fhx (fhx (-3.50)) / fhx (6.0)));
	dcFa = fhx (0.0);
	dcFb = fhx (0.0);
	dcFfa = fhx (0.0);
	dcFfb = fhx (0.0);
	fir0 = fhx (0.0);
	lim0 = fhx (0.0);
	m00 = fhx (0.0);
	m01 = fhx (0.0);
	m02 = fhx (0.0);
	m03 = fhx (0.0);
	drve_rnd0 = fhx (1.0);
	drvo_rnd0 = fhx (1.0);
	seed0 = fhx (int (rand() %1000));
	seed1 = fhx (int (rand() %1000));
}
DstTubeXb::~DstTubeXb()
{
	// Nothing Special
}
void DstTubeXb::setSmpRate(fsx nRate)
{
	sRate = fhx (nRate);

	const fhx kFactor = fhx (fhx (44100.0) / fhx (fhx (sRate) * fhx (rateFac)));
	kb = fhx (fhx (kFMult) * fhx (kFactor));
	ka = fhx (fhx (1.0) - fhx (kb));
}
void DstTubeXb::setGain(const fhx nGain)
{
	gGain = fhx (nGain);
	if(fhx (gGain) < fhx (zeroF)) gGain = fhx (zeroF);
	if(fhx (gGain) > fhx (1.0)) gGain = fhx (1.0);
}
void DstTubeXb::clearBuffs()
{
	dcFa = fhx (0.0);
	dcFb = fhx (0.0);
	dcFfa = fhx (0.0);
	dcFfb = fhx (0.0);
	fir0 = fhx (0.0);
	lim0 = fhx (0.0);
	m00 = fhx (0.0);
	m01 = fhx (0.0);
	m02 = fhx (0.0);
	m03 = fhx (0.0);
}
void DstTubeXb::setOverSamplingRate(int inp)
{
	if(inp == 0) rateFac = fhx (1.0);
	if(inp == 1) rateFac = fhx (1.0);
	if(inp == 2) rateFac = fhx (2.0);
	if(inp == 3) rateFac = fhx (4.0);
	if(inp == 4) rateFac = fhx (8.0);
	if(inp == 5) rateFac = fhx (16.0);
	if(inp == 6) rateFac = fhx (1.0);

	clearBuffs();

	const fhx kFactor = fhx (fhx (44100.0) / fhx (fhx (sRate) * fhx (rateFac)));
	kb = fhx (fhx (kFMult) * fhx (kFactor));
	ka = fhx (fhx (1.0) - fhx (kb));
}
fhx DstTubeXb::applyDistortion(fhx inSample)
{
	if(fhx (inSample) > fhx (1.0)) inSample = fhx (1.0);
	if(fhx (inSample) < fhx (-1.0)) inSample = fhx (-1.0);
	if(fhx (inSample) == fhx (0.0)) inSample += fhx (zeroF);

	const fhx hgain = fhx (fhx (gGain) * fhx (16.0));
	const fhx ch0 = fhx (inSample);
	const fhx h0 = fhx (sinl(ch0 * drve_rnd0) / sinl(ch0) * drve + (tanl(ch0) * drvo_rnd0) * drvo * fhx (0.3));
	dcFa = fhx (fhx (h0) - fhx (dcFfa = fhx (h0*kb+dcFfa*ka)));
	lim0 = (fhx) fminl(fmaxl(dcFa*hgain,fhx (-lim)),lim);
	m01 = fhx (m00);
	m03 = fhx (m02);
	fir0 = fhx (fhx (0.5) * fhx (fhx (m03) + fhx (m02 = (fhx) fhx (0.50) * fhx (m01 + fhx (m00 = (fhx) lim0)))));
	dcFb = fhx (fir0 - fhx (dcFfb = (fhx) fir0*kb+dcFfb*ka));

	inSample = fhx (fhx (fhx (inSample) + fhx (dcFb)) * fhx (trim));

	return fhx (fhx (inSample) * fhx (0.99750));
}
fsx DstTubeXb::applyDistortion(fsx inSample)
{
	if(fsx (inSample) > fsx (1.0)) inSample = fsx (1.0);
	if(fsx (inSample) < fsx (-1.0)) inSample = fsx (-1.0);
	if(fsx (inSample) == fsx (0.0)) inSample += fsx (zeroF);

	const fsx hgain = fsx (fsx (gGain) * fsx (16.0));
	const fsx ch0 = fsx (inSample);
	const fsx drve_rnd0 = fsx (1.0);
	const fsx drvo_rnd0 = fsx (1.0);
	const fsx h0 = fsx (sin(ch0 * drve_rnd0) / sin(ch0) * drve + (tan(ch0) * drvo_rnd0) * drvo * fsx (0.3));
	dcFa = fsx (fsx (h0) - fsx (dcFfa = fsx (h0*kb+dcFfa*ka)));
	lim0 = (fsx) fmin(fmax(dcFa*hgain,fsx (-lim)),lim);
	m01 = fsx (m00);
	m03 = fsx (m02);
	fir0 = fsx (fsx (0.5) * fsx (fsx (m03) + fsx (m02 = (fsx) fsx (0.50) * fsx (m01 + fsx (m00 = (fsx) lim0)))));
	dcFb = fsx (fir0 - fsx (dcFfb = (fsx) fir0*kb+dcFfb*ka));

	inSample = fsx (fsx (fsx (inSample) + fsx (dcFb)) * fsx (trim));

	return fsx (fsx (inSample) * fsx (0.99750));
}
flx DstTubeXb::applyDistortion(flx inSample)
{
	if(flx (inSample) > flx (1.0f)) inSample = flx (1.0f);
	if(flx (inSample) < flx (-1.0f)) inSample = flx (-1.0f);
	if(flx (inSample) == flx (0.0f)) inSample += flx (zeroF);

	const flx hgain = flx (flx (gGain) * flx (16.0f));
	const flx ch0 = flx (inSample);
	const flx drve_rnd0 = flx (1.0f);
	const flx drvo_rnd0 = flx (1.0f);
	const flx h0 = flx (sinf(ch0 * drve_rnd0) / sinf(ch0) * drve + (tanf(ch0) * drvo_rnd0) * drvo * flx (0.3f));
	dcFa = flx (flx (h0) - flx (dcFfa = flx (h0*kb+dcFfa*ka)));
	lim0 = (flx) fminf(fmaxf(dcFa*hgain,flx (-lim)),lim);
	m01 = flx (m00);
	m03 = flx (m02);
	fir0 = flx (flx (0.5f) * flx (flx (m03) + flx (m02 = (flx) flx (0.50f) * flx (m01 + flx (m00 = (flx) lim0)))));
	dcFb = flx (fir0 - flx (dcFfb = (flx) fir0*kb+dcFfb*ka));

	inSample = flx (flx (flx (inSample) + flx (dcFb)) * flx (trim));

	return flx (flx (inSample) * flx (0.99750f));
}
void DstTubeXb::RunBlockInternals(const int bkSize)
{
	// x
}
fhx DstTubeXb::DirtySine(fhx inp)
{
	while(fhx (inp) > fhx (4.0)) inp -= fhx (4.0);
	if(fhx (inp) < fhx (0.0)) return fhx (0.0);

	if(fhx (inp) <= fhx (1.0))
	{
		inp *= fhx (0.50);
		inp += fhx (0.50);
		inp *= fhx (fhx (2.0) - fhx (inp));
		inp -= fhx (0.750);
		inp *= fhx (4.0);
		return fhx (inp);
	}
	if(fhx (inp) <= fhx (2.0))
	{
		inp = fhx (fhx (2.0) - fhx (inp));
		inp *= fhx (0.50);
		inp += fhx (0.50);
		inp *= fhx (fhx (2.0) - fhx (inp));
		inp -= fhx (0.750);
		inp *= fhx (4.0);
		return fhx (inp);
	}
	if(fhx (inp) <= fhx (3.0))
	{
		inp -= fhx (2.0);
		inp *= fhx (0.50);
		inp += fhx (0.50);
		inp *= fhx (fhx (2.0) - fhx (inp));
		inp -= fhx (0.750);
		inp *= fhx (4.0);
		return fhx (-inp);
	}
	if(fhx (inp) <= fhx (4.0))
	{
		inp = fhx (fhx (4.0) - fhx (inp));
		inp *= fhx (0.50);
		inp += fhx (0.50);
		inp *= fhx (fhx (2.0) - fhx (inp));
		inp -= fhx (0.750);
		inp *= fhx (4.0);
		return fhx (-inp);
	}

	return fhx (0.0);
}
