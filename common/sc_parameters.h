#ifndef __ScorchCrafter_Global_PlugIn_Params__
#define __ScorchCrafter_Global_PlugIn_Params__

//! ------------------------------------------------------------------
//! Global Parameter List
//! ------------------------------------------------------------------
//! All ScorchCrafter plug-ins use some, but usually not all of
//! the parameters from these lists.  Which ones are used depends
//! on what specific plugin(s) is/are included.
//!
//! The first list is for parameters set with floating-point
//! (32-bit "float") values, which are valid from 0.0f to 1.0f.
//!
//! The second list is for parameters set usually by integer values,
//! with a minimum and maximum value that can differ depending on what
//! the parameter is and what the plug-in accepts.
//!
//! The second list is sometimes also used for boolean values, with
//! 0 == false and 1 == true.
//!
//! Both lists can also be used for getting values returned by functions
//! of a plugin.  (For example, you may notice the "Error_Status" entry
//! in the second list.  Usually a value of 0 (zero) for this means
//! that no error is detected and all is well.)
//! ------------------------------------------------------------------

enum SC_Parameters_FP
{
	ScPrmF_Master_Volume,
	ScPrmF_Mix_Wet,
	ScPrmF_Mix_Dry,
	ScPrmF_Effect_Amount,
	ScPrmF_Drive_Gain,
	ScPrmF_EQ_Low,
	ScPrmF_EQ_Mid,
	ScPrmF_EQ_High,
	ScPrmF_EQ_Contour,
	ScPrmF_EQ_Floor,
	ScPrmF_EQ_Presence,
	ScPrmF_MAX_COUNT
};
enum SC_Parameters_INT
{
	ScPrmI_Active_And_Ready,
	ScPrmI_Error_Status,
	ScPrmI_Drive_Channel,
	ScPrmI_Bright,
	ScPrmI_OverSampling_Rate,
	ScPrmI_Phase_Control,
	ScPrmI_FP_Precision_Bit_Depth,
	ScPrmI_Stereo,
	ScPrmI_CabMic_Active,
	ScPrmI_TStack_Shape,
	ScPrmI_MAX_COUNT
};
enum SCv_FP_Bit_Depth_Rates
{
	SCv_Bit_Depth_32 = 0,
	SCv_Bit_Depth_64 = 1,
	SCv_Bit_Depth_128 = 2,
	SCv_Bit_Depth_MAX_COUNT = 3
};
enum SCv_OverSampling_Rates
{
	SCv_OverSampling_1x,
	SCv_OverSampling_2x,
	SCv_OverSampling_4x,
	SCv_OverSampling_8x,
	SCv_OverSampling_16x,
	SCv_OverSampling_OFF,
	SCv_OverSampling_MAX_COUNT
};

#endif
