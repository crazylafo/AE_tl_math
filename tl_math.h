
/*
	tl_math.h
*/

#pragma once

#ifndef TLMATH_H
#define TLMATH_H

typedef unsigned char		u_char;
typedef unsigned short		u_short;
typedef unsigned short		u_int16;
typedef unsigned long		u_long;
typedef short int			int16;
#define PF_TABLE_BITS	12
#define PF_TABLE_SZ_16	4096

#define PF_DEEP_COLOR_AWARE 1	// make sure we get 16bpc pixels; 
								// AE_Effect.h checks for this.

#include "AEConfig.h"

#ifdef AE_OS_WIN
	typedef unsigned short PixelType;
	#include <Windows.h>
#endif

#include "entry.h"
#include "AE_Effect.h"
#include "AE_EffectCB.h"
#include "AE_Macros.h"
#include "Param_Utils.h"
#include "AE_EffectCBSuites.h"
#include "String_Utils.h"
#include "AE_GeneralPlug.h"
#include "AEFX_ChannelDepthTpl.h"
#include "AEGP_SuiteHandler.h"
#include "AEFX_SuiteHelper.h"
#include "AE_EffectSuites.h"


#include <cstdio>
#include <string>
#include "exprtk.hpp"

#include "tl_math_Strings.h"

/* Versioning information */

#define	MAJOR_VERSION	1
#define	MINOR_VERSION	0
#define	BUG_VERSION		0
#define	STAGE_VERSION	PF_Stage_DEVELOP
#define	BUILD_VERSION	1

typedef struct{
    A_char redExAc ;
    A_char greenExAc;
    A_char blueExAc;
    A_char alphaExAc;
}my_expr;


typedef struct {
    AEGP_PluginID	my_id;
} my_global_data, *my_global_dataP, **my_global_dataH;

typedef struct {
    my_expr		exprMemoryAc;
} my_sequence_data, *my_sequence_dataP, **my_sequence_dataH;

/* Parameter defaults */

#define	MATH_OPACITY_MIN		0
#define	MATH_OPACITY_MAX		100
#define	MATH_OPACITY_DFLT		100

enum {
	MATH_INPUT = 0,
	MATH_RED_OPACITY,
    MATH_GREEN_OPACITY,
    MATH_BLUE_OPACITY,
    MATH_ALPHA_OPACITY,
    MATH_EXECUTE,
	MATH_NUM_PARAMS
};

enum {
	MATH_RED_OPACITY_DISK_ID = 1,
    MATH_GREEN_OPACITY_DISK_ID,
    MATH_BLUE_OPACITY_DISK_ID,
    MATH_ALPHA_OPACITY_DISK_ID,
    MATH_EXECUTE_DISK_ID,

};

typedef struct MathInfo{
	PF_FpLong	RedIF;
    PF_FpLong	GreenIF;
    PF_FpLong	BlueIF;
    PF_FpLong	AlphaIF;
    
} MathInfoP, *MathinfoP, **MathinfoH;

#ifdef __cplusplus
	extern "C" {
#endif
	
DllExport	PF_Err 
EntryPointFunc(	
	PF_Cmd			cmd,
	PF_InData		*in_data,
	PF_OutData		*out_data,
	PF_ParamDef		*params[],
	PF_LayerDef		*output,
	void			*extra) ;

#ifdef __cplusplus
}
#endif

#endif // TLMATH