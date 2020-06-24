#ifndef __dist_tube_Xb__
#define __dist_tube_Xb__

#include <cmath>
#include <cstdlib>
#include "custdata_types.h"
#include "lp_bworth.h"

class DstTubeXb
{
public:
	DstTubeXb(fsx inSRate, const fhx kFF);
	~DstTubeXb();

	void setSmpRate(fsx nRate);
	void setGain(const fhx nGain);
	void clearBuffs();
	fhx applyDistortion(fhx inSample);
	fsx applyDistortion(fsx inSample);
	flx applyDistortion(flx inSample);
	void setOverSamplingRate(int inp);
	void RunBlockInternals(const int bkSize);

private:
	const fhx zeroF;
	const fhx PI;
	const fhx hPI;
	const fhx PId;
	const fhx kFMult;
	fhx DirtySine(fhx inp);

	fhx gGain;
	fhx drySamp;
	fhx sRate;
	fhx rateFac;
	int oSampling;

	fhx lim;
	fhx tgt_drve;
	fhx drvo;
	fhx drve;
	fhx kr;
	fhx kabs;
	fhx trim;
	fhx kb;
	fhx ka;
	fhx m00,m01,m02,m03;
	fhx dcFa, dcFb, dcFfa, dcFfb, fir0;
	fhx lim0;

	fhx drve_rnd0, drvo_rnd0;
	fhx seed0, seed1;
};

#endif
