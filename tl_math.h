
/*
	tl_math.h
*/

#pragma once

#ifndef TLMATH_H
#define TLMATH_H



#define PF_TABLE_BITS	12
#define PF_TABLE_SZ_16	4096

#define PF_DEEP_COLOR_AWARE 1	// make sure we get 16bpc pixels; 
								// AE_Effect.h checks for this.

#include "AEConfig.h"

#ifdef AE_OS_WIN
	typedef unsigned short PixelType;
	#include <assert.h>
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
#define	STAGE_VERSION	PF_Stage_ALPHA
#define	BUILD_VERSION	1

typedef exprtk::symbol_table<PF_FpShort> symbol_table_t;
typedef exprtk::expression<PF_FpShort>     expression_t;
typedef exprtk::parser<PF_FpShort>             parser_t;

#define ARB_REFCON			(void*)0xDEADBEEFDEADBEEF
typedef struct {
    A_char  redExAc[4096];
    A_char  greenExAc[4096];
    A_char  blueExAc[4096];
    A_char  alphaExAc[4096];
    
    //duplicate in order to keep the "/n" in javascript
    A_char  redExAcFlat[4096];
    A_char  greenExAcFlat[4096];
    A_char  blueExAcFlat[4096];
    A_char  alphaExAcFlat [4096];
} m_ArbData;

typedef struct {
    AEGP_PluginID	my_id;
} my_global_data, *my_global_dataP, **my_global_dataH;


/* Parameter defaults */

#define	MATH_VAR_MIN		0
#define	MATH_VAR_MAX		100
#define	MATH_VAR_DFLT       1

enum {
	MATH_INPUT = 0,
	MATH_INPONE_VAR,
    MATH_INPTWO_VAR,
    MATH_INPTHREE_VAR,
    MATH_INPFOUR_VAR,
    MATH_ARB_DATA,
    MATH_INP_POINT_ONE,
    MATH_INP_POINT_TWO,
    MATH_INP_COLOR_ONE,
    MATH_INP_COLOR_TWO,
	MATH_NUM_PARAMS
};

enum {
	MATH_INPONE_VAR_DISK_ID = 1,
    MATH_INPTWO_VAR_DISK_ID,
    MATH_INPTHREE_VAR_DISK_ID,
    MATH_INPFOUR_VAR_DISK_ID,
    MATH_ARB_DATA_DISK_ID,
    MATH_INP_POINT_ONE_DISK_ID,
    MATH_INP_POINT_TWO_DISK_ID,
    MATH_INP_COLOR_ONE_DISK_ID,
    MATH_INP_COLOR_TWO_DISK_ID,

};

typedef struct MathInfo{

    std::function<PF_FpShort()> redExpr;
    std::function<PF_FpShort()> greenExpr;
    std::function<PF_FpShort()> blueExpr;
    std::function<PF_FpShort()> alphaExpr;
    
    PF_EffectWorld inW;
    PF_EffectWorld outW;
    
	PF_FpShort	inOneF;
    PF_FpShort	inTwoF;
    PF_FpShort	inThreeF;
    PF_FpShort	inFourF;
    
    PF_FpShort  scale_x;
    PF_FpShort  scale_y;
    PF_FpShort layerWidthF;
    PF_FpShort layerHeightF;


	PF_FpShort		inRedF;
	PF_FpShort		inGreenF;
	PF_FpShort		inBlueF;
	PF_FpShort		inAlphaF;
    PF_FpShort		xLF;
    PF_FpShort		yLF;
    PF_FpShort		layerTime_Sec;
    PF_FpShort		layerTime_Frame;
    PF_FpShort		layerDuration;

	PF_FpShort		layerPos_X;
	PF_FpShort		layerPos_Y;
	PF_FpShort		layerPos_Z;

	PF_FpShort		layerScale_X;
	PF_FpShort		layerScale_Y;
	PF_FpShort		layerScale_Z;

	PF_FpShort      compWidthF;
	PF_FpShort       compHeightF;
	PF_FpShort      compFpsF;
    
    PF_FpShort       pointOneX;
    PF_FpShort       pointOneY;
    PF_FpShort       pointTwoX;
    PF_FpShort       pointTwoY;
    
    PF_FpShort       colorOne_red;
    PF_FpShort       colorOne_green;
    PF_FpShort       colorOne_blue;
    PF_FpShort       colorTwo_red;
    PF_FpShort       colorTwo_green;
    PF_FpShort       colorTwo_blue;


	PF_FpShort		 m3P_red[9];
	PF_FpShort		 m3P_green[9];
	PF_FpShort		 m3P_blue[9];
	PF_FpShort       m3P_alpha[9];
    
	PF_FpShort		luma;
    
    
    PF_Boolean      hasErrorB;
    std::string     channelErrorstr;
    std::string     errorstr;

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





void
strReplace(std::string& str,
                const std::string& oldStr,
                const std::string& newStr);

PF_Err
LineIteration8Func ( void *refconPV,
                    A_long thread_idxL,
                    A_long iterIndex,
                    A_long numIter);

#endif

#endif // TLMATH