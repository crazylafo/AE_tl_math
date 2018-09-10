
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

//#include "AE_EffectUI.h"
#include "AEFX_ArbParseHelper.h"
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

typedef exprtk::symbol_table<PF_FpLong> symbol_table_t;
typedef exprtk::expression<PF_FpLong>     expression_t;
typedef exprtk::parser<PF_FpLong>             parser_t;

#define ARB_REFCON			(void*)0xDEADBEEFDEADBEEF
typedef struct {
    A_char *redExAcP;
	A_char *greenExAcP;
	A_char *blueExAcP;
	A_char *alphaExAcP;
} m_ArbData;

typedef struct {
    AEGP_PluginID	my_id;
} my_global_data, *my_global_dataP, **my_global_dataH;



/* Parameter defaults */

#define	MATH_VAR_MIN		0
#define	MATH_VAR_MAX		1
#define	MATH_VAR_DFLT       1

enum {
	MATH_INPUT = 0,
	MATH_RED_VAR,
    MATH_GREEN_VAR,
    MATH_BLUE_VAR,
    MATH_ALPHA_VAR,
    MATH_ARB_DATA,
	MATH_NUM_PARAMS
};

enum {
	MATH_RED_VAR_DISK_ID = 1,
    MATH_GREEN_VAR_DISK_ID,
    MATH_BLUE_VAR_DISK_ID,
    MATH_ALPHA_VAR_DISK_ID,
    MATH_ARB_DATA_DISK_ID,

};

typedef struct MathInfo{
	PF_FpLong	RedIF;
    PF_FpLong	GreenIF;
    PF_FpLong	BlueIF;
    PF_FpLong	AlphaIF;
    
    PF_FpLong  scale_x;
    PF_FpLong  scale_y;
    PF_FpLong layerWidthF;
    PF_FpLong layerHeightF;


	PF_FpLong		inRedF;
	PF_FpLong		inGreenF;
	PF_FpLong		inBlueF;
	PF_FpLong		inAlphaF;
    PF_FpLong		xLF;
    PF_FpLong		yLF;
    PF_FpLong		layerTime_sec;
    PF_FpLong		layerTime_frame;
    PF_FpLong		layerDuration;
    
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


PF_Err
CreateDefaultArb(
                 PF_InData			*in_data,
                 PF_OutData			*out_data,
                 PF_ArbitraryH		*dephault);


PF_Err
Arb_Copy(
         PF_InData				*in_data,
         PF_OutData				*out_data,
         const PF_ArbitraryH		*srcP,
         PF_ArbitraryH			*dstP);

PF_Err
Arb_Interpolate(
                PF_InData				*in_data,
                PF_OutData				*out_data,
                double					itrp_amtF,
                const PF_ArbitraryH		*l_arbP,
                const PF_ArbitraryH		*r_arbP,
                PF_ArbitraryH			*result_arbP);

PF_Err
Arb_Compare(
            PF_InData				*in_data,
            PF_OutData				*out_data,
            const PF_ArbitraryH		*a_arbP,
            const PF_ArbitraryH		*b_arbP,
            PF_ArbCompareResult		*resultP);

PF_Err
Arb_Print_Size();

PF_Err
Arb_Print(
          PF_InData			*in_data,
          PF_OutData			*out_data,
          PF_ArbPrintFlags	print_flags,
          PF_ArbitraryH		arbH,
          A_u_long			print_sizeLu,
          A_char				*print_bufferPC);
PF_Err
Arb_Scan(
         PF_InData			*in_data,
         PF_OutData			*out_data,
         void 				*refconPV,
         const char			*bufPC,
         unsigned long		bytes_to_scanLu,
         PF_ArbitraryH		*arbPH);

#endif

#endif // TLMATH