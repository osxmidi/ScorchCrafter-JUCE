#ifndef __scorch_cdata_types__
#define __scorch_cdata_types__

#include "cfloat"

//! Determine floating-point precision for running audio
//! dependending on compiler DEFINE flags.

#ifdef PRECISION_LOW
#define SCPREC 1
#endif
#ifdef PRECISION_NORMAL
#define SCPREC 2
#endif
#ifdef PRECISION_MAX
#define SCPREC 3
#endif

//! If unspecified, assume NORMAL precision.
#ifndef SCPREC
#define SCPREC 2
#endif

#if (SCPREC==1)
#define __FLT_EVAL_METHOD 0
typedef float f9x;
#endif
#if (SCPREC==2)
#define __FLT_EVAL_METHOD 2
typedef double f9x;
#endif
#if (SCPREC==3)
#define __FLT_EVAL_METHOD 2
typedef long double f9x;
#endif

typedef unsigned long long u9x;
typedef signed long long s9x;
typedef unsigned short u7x;
typedef signed short s7x;
typedef unsigned int u8x;
typedef int s8x;

typedef float flx;
typedef double fsx;
typedef long double fhx;

class RateFC
{
public:
	RateFC();
	static const int vi[7];
	static const flx vl[7];
	static const fsx vs[7];
	static const fhx vh[7];
	const flx zl;
	const fsx zs;
	const fhx zh;
};

#endif
