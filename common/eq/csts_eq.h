#ifndef __sc_shared_eq_custom_S__
#define __sc_shared_eq_custom_S__

class RateFC;

#include <cmath>
#include "custdata_types.h"

class Cst_EQ
{
public:
	Cst_EQ();
	~Cst_EQ() {delete rateV;};

	void SplInL(const flx inp);
	void SplInS(const fsx inp);
	void SplInH(const fhx inp);
	void SetBands(fsx lo, fsx hi);
	void SetSmpRate(const fsx nRate);
	void ClearBuffers();

	void RunQL();
	void RunQS();
	void RunQH();
	void RunLPL();
	void RunLPS();
	void RunLPH();
	void RunHPL();
	void RunHPS();
	void RunHPH();

	flx GetOutputL();
	fsx GetOutputS();
	fhx GetOutputH();

private:
	RateFC *rateV;

	flx sampL;
	fsx sampS;
	fhx sampH;

	flx sRateL;
	fsx sRateS;
	fhx sRateH;

	flx qPaL[2][3];
	fsx qPaS[2][3];
	fhx qPaH[2][3];

	flx pValLowL;
	flx pValHiL;
	fsx pValLowS;
	fsx pValHiS;
	fhx pValLowH;
	fhx pValHiH;

	flx bValLowL;
	flx bValHiL;
	fsx bValLowS;
	fsx bValHiS;
	fhx bValLowH;
	fhx bValHiH;
};

#endif
