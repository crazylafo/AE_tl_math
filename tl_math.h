
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
#define	STAGE_VERSION	PF_Stage_DEVELOP
#define	BUILD_VERSION	1

typedef exprtk::symbol_table<PF_FpLong> symbol_table_t;
typedef exprtk::expression<PF_FpLong>     expression_t;
typedef exprtk::parser<PF_FpLong>             parser_t;

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
#define	MATH_VAR_MAX		1
#define	MATH_VAR_DFLT       1

enum {
	MATH_INPUT = 0,
	MATH_INPONE_VAR,
    MATH_INPTWO_VAR,
    MATH_INPTHREE_VAR,
    MATH_INPFOUR_VAR,
    MATH_ARB_DATA,
	MATH_NUM_PARAMS
};

enum {
	MATH_INPONE_VAR_DISK_ID = 1,
    MATH_INPTWO_VAR_DISK_ID,
    MATH_INPTHREE_VAR_DISK_ID,
    MATH_INPFOUR_VAR_DISK_ID,
    MATH_ARB_DATA_DISK_ID,

};

typedef struct MathInfo{
	PF_FpLong	inOneF;
    PF_FpLong	inTwoF;
    PF_FpLong	inThreeF;
    PF_FpLong	inFourF;
    
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
    PF_FpLong		layerTime_Sec;
    PF_FpLong		layerTime_Frame;
    PF_FpLong		layerDuration;

	PF_FpLong		layerPos_X;
	PF_FpLong		layerPos_Y;
	PF_FpLong		layerPos_Z;

	PF_FpLong		layerScale_X;
	PF_FpLong		layerScale_Y;
	PF_FpLong		layerScale_Z;

	PF_FpLong       compWidthF;
	PF_FpLong       compHeightF;
	PF_FpLong       compFpsF;
    
    
    PF_Boolean      has3MatrixB;

	/*
	PF_FpLong		luma;
	PF_FpLong       saturation_Red;
	PF_FpLong		saturation_Green;
	PF_FpLong		saturation_blue;*/



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

PF_Pixel
*sampleIntegral32(PF_EffectWorld &def,
                  int x,
                  int y);

//same in 16bits
PF_Pixel16
*sampleIntegral64(PF_EffectWorld &def,
                  int x,
                  int y);

//same in 32 bits
PF_PixelFloat
*sampleIntegral128(PF_EffectWorld &def,
                   int x,
                   int y);



void
GetPixelValue(
              PF_EffectWorld  *WorldP,
              PF_PixelFormat  pxFormat,
              int x,
              int y,
              PF_PixelFloat		*pixvalueF);



#endif

#endif // TLMATH