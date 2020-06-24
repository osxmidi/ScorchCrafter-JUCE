#ifndef __ScorchCrafter_C120__
#define __ScorchCrafter_C120__

//! ----------------------------------------------------------------------------------------------------------------
//! ScorchCrafter Plug-in General Class Header
//!     Revision 2.1
//! ----------------------------------------------------------------------------------------------------------------

//! Note: Please look at "custdata_types.h" for details on the typdefs used throughout the software (such as "fsx" or "fhx" etc.).
#include <cmath>
#include "../common/custdata_types.h"

//! ----------------------------------------------------------------------------------------------------------------
//! These check for Global, or "Master" #define(d) values (In case of combination plug-ins made up of more than one SC plug-in)
//! ----------------------------------------------------------------------------------------------------------------
#ifndef SCr_Global_Fade_Factor
#define SCr_Global_Fade_Factor (double) 0.0
#endif

//! ----------------------------------------------------------------------------------------------------------------
//! These #define(s) can vary by plug-in
//! ----------------------------------------------------------------------------------------------------------------

//! Sets tiny bit of silence after clearing buffers or changing sample rate and such, to avoid pops, clicks, and spikes in the audio
#define SCr_C120_AmpHead_Fade_Factor (double) (fmax(double 		(256.0), double (SCr_Global_Fade_Factor)))

//! These control input filter settings.
//! (Frequencies in Hz.)
#define SCr_C120_AmpHead_InputQ_Low_Standard (double)			420.0
#define SCr_C120_AmpHead_InputQ_Low_Bright (double)				625.0
#define SCr_C120_AmpHead_InputQ_Ceiling (double)				7200.0

//! These settings affect how a stereo effect is created in stereo mode.
//! Please only change them if you really know what you are doing!
#define SCr_C120_AmpHead_QSpatialFactor_Mono (long double)		70.0
#define SCr_C120_AmpHead_QSpatialFactor_Left (long double) 		60.0
#define SCr_C120_AmpHead_QSpatialFactor_Right (long double) 	80.0
#define SCr_C120_AmpHead_CnvPhase_Left (double) 				20.0
#define SCr_C120_AmpHead_CnvPhase_Right (double) 				80.0
#define SCr_C120_AmpHead_CnvSkew_Left (double) 					0.750
#define SCr_C120_AmpHead_CnvSkew_Right (double) 				-0.750

//! These Control the overdrive/distortion
//! "Stages" are the number of simulated "tubes" or runs (loops) through
//! 	distortion, listed for each channel.  If you change these, don't forget to
//! 	also update the "Max Stages" value to the highest of the four or else you can
//! 	BREAK STUFF and possibly cause a CRASH in your DAW or host.
//!
//! "T Speed" is part of how fast (in Hz) the "grit" or "grind" of the sound is,
//! 	and is combined with "T Ratio" in part of distorting the sound.  The two
//! 	values multiplied together should not be too high, unless you enjoy "fizzy"-
//! 		sounding guitar amps.
//!
//! "Tube Frequency" A and B affect the tone going in and out of each stage.  Please only
//! 	adjust them if you really know what you are doing.
//!
//! "Tube Frequency Stages" affect how many times the frequency effect is applied to each
//! 	stage.  You can drop it down to 1 to save CPU, and probably wouldn't notice much improvement
//! 	in the smoothness of tone going above 4.
//!
//! "Number (of) Harmonics" controls the number of split harmonic bands (overtones) that get distorted.
//! 	Normally this is set to 5, please only change it if you really know what you are doing.
//! 	If you don't want overtones or want non-harmonic distortion (yawn) you can set it to 1.
//! 	(The first band is the base tone, so it must be set to at least 1, NOT zero.)
#define DIST_TBD_MAX_STAGES (int) 								6

#define DIST_TBD_STAGES_CLEAN (int) 							2
#define DIST_TBD_STAGES_CRUNCH (int) 							2
#define DIST_TBD_STAGES_HIGH (int) 								6
#define DIST_TBD_STAGES_MEGA (int) 								6

#define Dist_TBD_T_Speed (long double) 							150.0
#define Dist_TBD_T_Ratio (long double) 							0.10
#define Dist_TBD_Gain_Mult_Factor (long double)					1.0
#define Dist_TBD_TubeFreqA (double) 							3600.0
#define Dist_TBD_TubeFreqB (double)								7200.0
#define Dist_TBD_TubeFreqStages (int) 							3
#define Dist_TBD_PreAmp_Stages_Max (int)						6
#define Dist_TBD_PreAmp_Stages_BoostOff (int)					3

//! These are for the output tone stack controls
//! They are frequencies in Hz for the control bands, and determine where
//!		the bands start and end (approx.).
#define SCr_C120_TStack_EqBand_Start_Low (long double)			30.0
#define SCr_C120_TStack_EqBand_Start_Mid (long double)			300.0
#define SCr_C120_TStack_EqBand_Start_High (long double)			2000.0
#define SCr_C120_TStack_EqBand_Start_CntrA (long double)		100.0
#define SCr_C120_TStack_EqBand_Start_CntrB (long double)		1000.0
#define SCr_C120_TStack_EqBand_Start_Pres (long double)			3000.0

#define SCr_C120_TStack_EqBand_End_Low (long double)			300.0
#define SCr_C120_TStack_EqBand_End_Mid (long double)			2000.0
#define SCr_C120_TStack_EqBand_End_High (long double)			20000.0
#define SCr_C120_TStack_EqBand_End_CntrA (long double)			300.0
#define SCr_C120_TStack_EqBand_End_CntrB (long double)			3000.0
#define SCr_C120_TStack_EqBand_End_Pres (long double)			20000.0

//! Base Length of internal convolution impulses for tone stack and built-in cab/mic
//! 	(The acutal impulse length used at run-time can vary, and depends on the host sample rate)
//! Note:  During run-time usage the actual length will automatically be re-calculated when
//!		the host changes sample rates, and then the internal impulses will be re-generated (calculated)
//!		and stored in dynamic memory.  Almost any arbitrary sample rate at or above 44.100 Hz is supported,
//!		including non-standard or unusual rates.  Yes, both impulse length values MUST be the SAME!
//! 		(Except one is an integer, the other floating=point.)
//!
//! The "Sync Rate" values are used for re-calculating the impulses in memory when the sample rate changes.
//! 	Higher values are more accurate but going too big can cause issues with high sample rates.
//!		Please only adjust them if you really know what you are doing.  Yes, the two sync values MUST be the SAME!
//! 		(Except one is an integer, the other floating=point.)
#define SC_TSTACK_D_IMPULSE_MAX_I (long) 						300
#define SC_TSTACK_D_IMPULSE_MAX_F (long double) 				300.0
#define SC_TSTACK_D_IMPULSE_SyncRate_Integer (long)				64
#define SC_TSTACK_D_IMPULSE_SyncRate_FloatingPnt (long double)	64.0

//! ----------------------------------------------------------------------------------------------------------------
class Tube_BankD;
class ToneStack_D;
class Cst_EQ;

#include "sc_parameters.h"
#include "lp_btw.h"
#include "sample_bank_custom.h"
#include "tube_bank_d.h"
#include "tonestack_d.h"
#include "csts_eq.h"

class SC_C120_Amp_Head
{
//! ----------------------------------------------------------------------------------------------------------------
//! NOTE: "Public" functions of the plug-in are usually the same from (ScorchCrafter) plug-in to plug-in.
//!     If a plug-in doesn't use a specific function or parameter, it will usually return a zero as int (0) or a float/double (0.0).
//! ----------------------------------------------------------------------------------------------------------------
public:
	SC_C120_Amp_Head(double sRt = 44100.0);							//! Create an instance of the plug-in, setting a starting sample rate.  (Default is 44100 kHz.)
	~SC_C120_Amp_Head();

	void SetNewSampleRate(const double inp);							//! Use this whenever changing the standard sampling rate (NOT oversampling).
	void ClearAllBuffers();												//! Should be used when starting or stopping playback, or to silence effect "tail" tones.
																		//! Note:  This is automatically called when chagning certain other settings, such as sample rate, etc.,
																		//! So you do NOT need to call it when setting parameters, the plug-in will clear buffers as needed automatically in such cases.
	void SetFpParam(const int prm, float newValue);					//! Sets internal parameter (from the sc_parameters.h list) to a new 32-bit float value from 0.0f to 1.0f.
	void SetIntParam(const int prm, int newValue);					//! Sets internal parameter to new integer or boolean value, valid values can differ.
	void InputSample(const double inp);								//! Send an audio sample to the plugin (64-bit floating point).  Usually (but not always) should be from -1.0 to +1.0
	void InputSampleStereo(const double inpL, const double inpR);	//! Just like InputSample(), except for sending stereo sound to the plugin.
	void RunPlugin();													//! Always run this ONCE in the program loop for the plugin, to run all internals.  JUST ONCE, even when oversampling!
	double GetMonoOut();												//! Returns processed and/or generated audio of the plugin, in one mono channel.
	double GetStereoOutLeft();											//! Gets the LEFT channel audio output proecessed and/or generated by the plugin.
	double GetStereoOutRight();										//! Gets the RIGHT channel audio output proecessed and/or generated by the plugin.
	float GetFpParam(const int prm);									//! Returns 32-bit float internal value.  If the plugin doesn't have such a paramter, it returns 0.0f.
	int GetIntParam(const int prm);									//! Returns internal integer value.  If it is an invalid paramteter, it returns zero.
	long long GetLongParam(const int prm);							//! Returns internal integer value of great size.  If it is an invalid paramteter, it returns zero.  Not all plug-ins use this.
	long GetPluginInterfaceVersion() { return long (2100); }			//! Returns Version of Plug-in General Class Header (this file).  Major revisions are x1000, so rev. 2.0 is "2000"

//! ----------------------------------------------------------------------------------------------------------------
//! NOTE: "Private" functions, variables, objects, etc. can vary by plug-in.
//! ----------------------------------------------------------------------------------------------------------------
private:
	long fadeCount;					// Counts down and must be zero before audio is sent to the output.  Used to avoid pops and clicks.  Jumps up to the max when buffers are cleared or the plugin is reset.
	long fadeMax;					// This is the maximum for the fadecount, and is changed when the sample rate is updated.
	const double fadeFac;			// Determines how long the fade should be when buffers are cleared, this value can be different in various plugins but is fixed and does not change.
	double smpRate;				// Current sample rate that the plugin recognizes and runs at.
	double splInputMono;			// The input value the plug-in uses to preocess any audio from, 64-bit floating point.  Use the InputSample() function to pass audio in a stream to it.
	double splInputLeft;			// Same as splInputMono, except as the left channel of stereo input.
	double splInputRight;			// Same as splInputMono, except as the right channel of stereo input.
	double splOutputMono;			// Output audio sample after any processing, 64-bit floating point.  Use GetMonoOut() to retrieve its value.
	double splOutputLeft;			// Output audio sample after any processing, 64-bit floating point.  Use GetMonoOut() to retrieve its value.
	double splOutputRight;			// Output audio sample after any processing, 64-bit floating point.  Use GetMonoOut() to retrieve its value.

	flx l_smp[3];					// Internal sample data, 32 bit (float)
	fsx s_smp[3];					// Internal sample data, 64 bit (double)
	fhx h_smp[3];					// Internal sample data, 128 bit (long double)

	float FpParam[ScPrmF_MAX_COUNT];
	int IntParam[ScPrmI_MAX_COUNT];

	Tube_BankD *PluGin[3];
	ToneStack_D *Qstack[3];

	Cst_EQ *InpFilter;
	Cst_EQ *InpFilterB;

	const flx l_PI;
	const fsx s_PI;
	const fhx h_PI;

	flx l_deNormSuppress;
	fsx s_deNormSuppress;
	fhx h_deNormSuppress;
	const flx l_dNrmThresh;
	const fsx s_dNrmThresh;
	const fhx h_dNrmThresh;
	flx l_dNrm;
	fsx s_dNrm;
	fhx h_dNrm;
};

#endif
