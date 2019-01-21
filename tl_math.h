
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
#include "tl_math_Strings.h"

#include <cstdio>
#include <string>
#include <thread>
#include <mutex>

#include "lib/exprtk.hpp"
#include "lib/json.hpp"

/* Versioning information */

#define	MAJOR_VERSION	1
#define	MINOR_VERSION	1
#define	BUG_VERSION		3
#define	STAGE_VERSION	PF_Stage_ALPHA
#define	BUILD_VERSION	1

typedef exprtk::symbol_table<PF_FpShort> symbol_table_t;
typedef exprtk::expression<PF_FpShort>     expression_t;
typedef exprtk::parser<PF_FpShort>             parser_t;
  typedef exprtk::function_compositor<PF_FpShort> compositor_t;

#define ARB_REFCON			(void*)0xDEADBEEFDEADBEEF
typedef struct {

    A_char  redExAc[4096];
    A_char  greenExAc[4096];
    A_char  blueExAc[4096];
    A_char  alphaExAc[4096];
    A_char  functionOneAc[4096];
    A_char  functionTwoAc[4096];
    A_char  functionThreeAc[4096];
    A_char  Glsl_FragmentShAc[4096];
    A_char  descriptionAc[2048];
    A_char  presetNameAc[32];


     A_char MATH_INPONE_NameAC[32];
     A_char MATH_INPTWO_NameAC[32];
     A_char MATH_INPTHREE_NameAC[32];
     A_char MATH_INPFOUR_NameAC[32];
     A_char MATH_INP_POINT_ONE_NameAC[32];
     A_char MATH_INP_POINT_TWO_NameAC[32];
     A_char MATH_INP_COLOR_ONE_NameAC[32];
     A_char MATH_INP_COLOR_TWO_NameAC[32];

    
    //duplicate in order to keep the "/n" in javascript
    A_char  redExAcFlat[4096];
    A_char  greenExAcFlat[4096];
    A_char  blueExAcFlat[4096];
    A_char  alphaExAcFlat [4096];

    A_char  functionOneFlat[4096];
    A_char  functionTwoFlat[4096];
    A_char  functionThreeFlat[4096];
    A_char  Glsl_FragmentShFlat[4096];
    A_char  descriptionAcFlat [4096];
    
    //Mode
    PF_Boolean parserModeB;
	//Boolean information about chars
	PF_Boolean NeedsPixelAroundB;
	PF_Boolean PixelsCallExternalInputB;
	PF_Boolean NeedsLumaB;
	PF_Boolean PresetHasWideInputB;
    


} m_ArbData;

typedef struct {
    AEGP_PluginID	my_id;
} my_global_data, *my_global_dataP, **my_global_dataH;


/* Parameter defaults */

#define	MATH_VAR_MIN		0
#define	MATH_VAR_MAX		1000
#define	MATH_SLIDER_MAX		100
#define	MATH_VAR_DFLT       1

#define TIMEOFFSET_MIN      -1000
#define TIMEOFFSET_MAX       1000
#define TIMEOFFSET_DFLT      0


enum {
	MATH_INPUT = 0,
    MATH_ARB_DATA,
    MATH_TOPIC_SLIDER,
	MATH_INPONE_VAR,
    MATH_INPTWO_VAR,
    MATH_INPTHREE_VAR,
    MATH_INPFOUR_VAR,
    MATH_END_TOPIC_SLIDER,
    MATH_TOPIC_POINTS,
    MATH_INP_POINT_ONE,
    MATH_INP_POINT_TWO,
    MATH_END_TOPIC_POINTS,
    MATH_TOPIC_COLORS,
    MATH_INP_COLOR_ONE,
    MATH_INP_COLOR_TWO,
    MATH_END_TOPIC_COLORS,
    MATH_TOPIC_INPUTS,
    MATH_INP_LAYER_ONE,
    MATH_INP_TOFF_ONE,
    MATH_INP_POFF_ONE,
    MATH_END_TOPIC_INPUTS,
	MATH_NUM_PARAMS
};

enum {   MATH_ARB_DATA_DISK_ID =1,
    MATH_TOPIC_SLIDER_DISK_ID,
    MATH_INPONE_VAR_DISK_ID,
    MATH_INPTWO_VAR_DISK_ID,
    MATH_INPTHREE_VAR_DISK_ID,
    MATH_INPFOUR_VAR_DISK_ID,
    MATH_END_TOPIC_SLIDER_DISK_ID,
    MATH_TOPIC_POINTS_DISK_ID,
    MATH_INP_POINT_ONE_DISK_ID,
    MATH_INP_POINT_TWO_DISK_ID,
    MATH_END_TOPIC_POINTS_DISK_ID,
    MATH_TOPIC_COLORS_DISK_ID,
    MATH_INP_COLOR_ONE_DISK_ID,
    MATH_INP_COLOR_TWO_DISK_ID,
    MATH_END_TOPIC_COLORS_DISK_ID,
    MATH_TOPIC_INPUTS_DISK_ID,
    MATH_INP_LAYER_ONE_DISK_ID,
    MATH_INP_TOFF_ONE_DISK_ID,
    MATH_INP_POFF_ONE_DISK_ID,
    MATH_END_TOPIC_INPUTS_DISK_ID,
};

typedef struct FlagsInfo {
	PF_Boolean NeedsPixelAroundB;
	PF_Boolean PixelsCallExternalInputB;
	PF_Boolean NeedsLumaB;
	PF_Boolean PresetHasWideInput;
    PF_Boolean      CallsAEGP_CompB;
    PF_Boolean      CallsAEGP_layerB;
    PF_Boolean      hasFuncOneB;
    PF_Boolean      hasFuncTwoB;
    PF_Boolean      hasFuncThreeB;

}FlagsInfoP;

typedef struct funcTransfertInfo {
	std::function<PF_FpShort()> redExpr;
	std::function<PF_FpShort()> greenExpr;
	std::function<PF_FpShort()> blueExpr;
	std::function<PF_FpShort()> alphaExpr;
	PF_Boolean      hasErrorB;
	std::string     channelErrorstr;
	std::string     errorstr;
    std::string     func1str;
    std::string     func2str;
    std::string     func3str;

}funcTransfertInfoP;

typedef struct MathInfo{
    PF_EffectWorld  inW;
    PF_EffectWorld  outW;
    PF_FpShort		inRedF;
    PF_FpShort		inGreenF;
    PF_FpShort		inBlueF;
    PF_FpShort		inAlphaF;
    PF_FpShort		xLF;
    PF_FpShort		yLF;
    PF_EffectWorld  extLW;
    PF_FpShort      extL_red;
    PF_FpShort      extL_green;
    PF_FpShort      extL_blue;
    PF_FpShort      extL_alpha;
	PF_FpShort      inOneF;
    PF_FpShort      inTwoF;
    PF_FpShort      inThreeF;
    PF_FpShort      inFourF;
    PF_FpShort      scale_x;
    PF_FpShort      scale_y;
    PF_FpShort      layerWidthF;
    PF_FpShort      layerHeightF;
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
	PF_FpShort      compHeightF;
	PF_FpShort      compFpsF;
    PF_FpShort      pointOneX;
    PF_FpShort      pointOneY;
    PF_FpShort      pointTwoX;
    PF_FpShort      pointTwoY;
    PF_FpShort      colorOne[3];
    PF_FpShort      colorTwo[3];
	PF_FpShort		m9P_red[9];
	PF_FpShort		m9P_green[9];
	PF_FpShort		m9P_blue[9];
	PF_FpShort      m9P_alpha[9];
	PF_FpShort		luma;
	PF_PixelFloat   PixelOFfP; 
} MathInfoP, *MathinfoP, **MathinfoH;


typedef struct {
    PF_Fixed	x_offFi;
    PF_Fixed	y_offFi;
    PF_SampPB	samp_pb;
    PF_InData	in_data;
} OffInfo;

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
                    void *refconFunc,
					void *refconFlags,
                    A_long yL);
PF_Err
LineIteration16Func ( void *refconPV,
                      void *refconFunc,
					  void *refconFlags,
                      A_long yL);
//math parser's functions
static PF_FpShort
inline parseDrawRect(PF_FpShort xL,
                     PF_FpShort yL,
                     PF_FpShort center_x,
                     PF_FpShort center_y,
                     PF_FpShort lx,
                     PF_FpShort ly);

template <typename T=PF_FpShort> class parseExpr {
private:
    std::shared_ptr<exprtk::parser<T>> parser;
    exprtk::expression<T> expression;
    exprtk::symbol_table<T> symbol_table;
    std::shared_ptr<exprtk::function_compositor<T>> compositor;
public:
    parseExpr(void *mathRefcon, void *refconFunc, const std::string &exprstr) {
        MathInfo	*miP	= reinterpret_cast<MathInfo*>(mathRefcon);
		funcTransfertInfo *fiP = reinterpret_cast<funcTransfertInfo*>(refconFunc);
        std::string expression_string_Safe = "1";
        if (!parser){
            parser = std::make_shared<exprtk::parser<T>>();
        }
        fiP->hasErrorB = FALSE;
        symbol_table.clear();
        symbol_table.add_variable("xL",  miP->xLF);
        symbol_table.add_variable("yL",  miP->yLF);
        symbol_table.add_variable("in_red", miP->inRedF);
        symbol_table.add_variable("in_green", miP->inGreenF);
        symbol_table.add_variable("in_blue", miP->inBlueF);
        symbol_table.add_variable("in_alpha", miP->inAlphaF);
        symbol_table.add_variable("in_luma", miP->luma);
        symbol_table.add_vector("vec9_red", miP->m9P_red);
        symbol_table.add_vector("vec9_green",miP->m9P_green);
        symbol_table.add_vector("vec9_blue", miP->m9P_blue);
        symbol_table.add_vector("vec9_alpha", miP->m9P_alpha);

        symbol_table.add_variable("extL_red", miP->extL_red);
        symbol_table.add_variable("extL_green", miP->extL_green);
        symbol_table.add_variable("extL_blue", miP->extL_blue);
        symbol_table.add_variable("extL_alpha", miP->extL_alpha);
        
        symbol_table.add_constants();
        symbol_table.add_constant("var1",miP->inOneF);
        symbol_table.add_constant("var2",miP->inTwoF);
        symbol_table.add_constant("var3",miP->inThreeF);
        symbol_table.add_constant("var4",miP->inFourF);
        symbol_table.add_constant ("pt1_x",miP->pointOneX);
        symbol_table.add_constant ("pt1_y",miP->pointOneY);
        symbol_table.add_constant ("pt2_x",miP->pointTwoX);
        symbol_table.add_constant ("pt2_y",miP->pointTwoY);
        symbol_table.add_constant ("cl1_red",miP->colorOne[0]);
        symbol_table.add_constant ("cl1_green", miP->colorOne[1]);
        symbol_table.add_constant ("cl1_blue",miP->colorOne[2]);
        symbol_table.add_constant ("cl2_red",miP->colorTwo[0]);
        symbol_table.add_constant ("cl2_green",miP->colorTwo[1]);
        symbol_table.add_constant ("cl2_blue",miP->colorTwo[2]);
        symbol_table.add_constant("layerWidth",miP->layerWidthF);
        symbol_table.add_constant("layerHeight",miP->layerHeightF);
        symbol_table.add_constant("layerTime_sec",miP->layerTime_Sec);
        symbol_table.add_constant("layerTime_frame",miP->layerTime_Frame);
        symbol_table.add_constant("layerDuration",miP->layerDuration);
        symbol_table.add_constant("layerPosition_x", miP->layerPos_X);
        symbol_table.add_constant("layerPosition_y", miP->layerPos_Y);
        symbol_table.add_constant("layerPosition_z", miP->layerPos_Z);
        symbol_table.add_constant("layerScale_z", miP->layerScale_X);
        symbol_table.add_constant("layerScale_y", miP->layerScale_Y);
        symbol_table.add_constant("layerScale_z", miP->layerScale_Z);
        symbol_table.add_constant("compWidth", miP->compWidthF);
        symbol_table.add_constant("compHeight", miP->compHeightF);
        symbol_table.add_constant("compFps", miP->compFpsF);
        symbol_table.add_function("drawRect", parseDrawRect);
        expression.register_symbol_table(symbol_table);
        parser->compile(exprstr,expression);
        if (!parser->compile(exprstr,expression))
        {
            fiP->hasErrorB = TRUE;
            fiP->errorstr =parser->error();
            parser->compile(expression_string_Safe, expression);
        }
    }
    T operator()() { return expression.value(); }
};

class threaded_render
{
private:
    std::mutex mut;
    A_long curNumIter;
public:
    
    void render_8(void *refconPV, void *refconFunc, void *refconFlags, A_long thread_idxL, A_long numThreads, A_long numIter, A_long lastNumIter);

    void render_16(void *refconPV, void *refconFunc, void *refconFlags, A_long thread_idxL, A_long numThreads, A_long numIter, A_long lastNumIter);

    
};


#endif

#endif // TLMATH
