#include "custdata_types.h"
#include <cmath>

const int RateFC::vi[7] = {1,1,2,4,8,16,1};
const flx RateFC::vl[] = {1.0, 1.0, 2.0, 4.0, 8.0, 16.0, 1.0};
const fsx RateFC::vs[] = {1.0, 1.0, 2.0, 4.0, 8.0, 16.0, 1.0};
const fhx RateFC::vh[] = {1.0, 1.0, 2.0, 4.0, 8.0, 16.0, 1.0};

RateFC::RateFC() :
zl(flx (powf(10.0f,-30.f))),
zs(fsx (pow(10.0,-32.0))),
zh(fhx (powl(10.0,-36.0)))
{}
