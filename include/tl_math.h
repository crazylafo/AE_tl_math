
/*
	tl_math.h
*/

#pragma once

#ifndef TLMATH_H
#define TLMATH_H

#define PF_TABLE_BITS	12
#define PF_TABLE_SZ_16	4096
typedef unsigned char		u_char;
typedef unsigned short		u_short;
typedef unsigned short		u_int16;
typedef unsigned long		u_long;
typedef short int			int16;
typedef float				fpshort;
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
#include "AE_effect.h"
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
#include "Smart_Utils.h" // for smartfx

#include "exprtk.hpp"
#include "json.hpp"
#include    "tl_defaultArb.h"

#include <cstdio>
#include <string>
#include <thread>
#include <map>
#include <mutex>
#include <assert.h>
#include <atomic>

#include "GL_base.h"
#include "vmath.hpp"




using namespace AESDK_OpenGL;
using namespace gl33core;





/* Versioning information */

#define	MAJOR_VERSION	1
#define	MINOR_VERSION	1
#define	BUG_VERSION		5
#define	STAGE_VERSION	PF_Stage_ALPHA
#define	BUILD_VERSION	5

#define ARB_REFCON            (void*)0xDEADBEEFDEADBEEF
typedef struct {
    A_char arbDataAc[100000];
} m_ArbData;
typedef struct {
	bool    initializedB;
	A_char  descriptionAc[2048];
	A_char  presetNameAc[32];
    A_char  redExAc[4096];
    A_char  greenExAc[4096];
    A_char  blueExAc[4096];
    A_char  alphaExAc[4096];
    A_char  Glsl_VertexShAc[25000];
    A_char  Glsl_FragmentShAc[50000];

    A_char redError[2048];
    A_char greenError[2048];
    A_char blueError[2048];
    A_char alphaError[2048];

    A_char Glsl_VertError[2048];
    A_char Glsl_fragError[2048];


	A_char   resolution[32];
	A_char   time_sec[32];
	A_char   time_frame[32];
	A_char   frame_rate[32];

    A_char sliderGrpNameAc[32];
    PF_Boolean sliderGrpVisibleB;

    A_char       pointGrpNameAc[32];
    PF_Boolean   pointGrpVisibleB;

    A_char       cbGrpNameAc[32];
    PF_Boolean  cbGrpVisibleB;

    A_char       colorGrpNameAc[32];
    PF_Boolean   colorGrpVisibleB;

    A_char       layerGrpNameAc[32];
    PF_Boolean  layerGrpVisibleB;

    A_char   paramSlider01NameAc[32];
    A_char   paramSlider02NameAc[32];
    A_char   paramSlider03NameAc[32];
    A_char   paramSlider04NameAc[32];
    A_char   paramSlider05NameAc[32];
    A_char   paramSlider06NameAc[32];
    A_char   paramSlider07NameAc[32];
    A_char   paramSlider08NameAc[32];
    A_char   paramSlider09NameAc[32];
    A_char   paramSlider10NameAc[32];
    PF_Boolean  paramSlider01VisibleB;
    PF_Boolean  paramSlider02VisibleB;
    PF_Boolean  paramSlider03VisibleB;
    PF_Boolean  paramSlider04VisibleB;
    PF_Boolean  paramSlider05VisibleB;
    PF_Boolean  paramSlider06VisibleB;
    PF_Boolean  paramSlider07VisibleB;
    PF_Boolean  paramSlider08VisibleB;
    PF_Boolean  paramSlider09VisibleB;
    PF_Boolean  paramSlider10VisibleB;


    A_char   paramPoint01NameAc[32];
    A_char   paramPoint02NameAc[32];
    A_char   paramPoint03NameAc[32];
    A_char   paramPoint04NameAc[32];
    A_char   paramPoint05NameAc[32];
    A_char   paramPoint06NameAc[32];
    A_char   paramPoint07NameAc[32];
    A_char   paramPoint08NameAc[32];
    A_char   paramPoint09NameAc[32];
    A_char   paramPoint10NameAc[32];
    PF_Boolean  paramPoint01VisibleB;
    PF_Boolean  paramPoint02VisibleB;
    PF_Boolean  paramPoint03VisibleB;
    PF_Boolean  paramPoint04VisibleB;
    PF_Boolean  paramPoint05VisibleB;
    PF_Boolean  paramPoint06VisibleB;
    PF_Boolean  paramPoint07VisibleB;
    PF_Boolean  paramPoint08VisibleB;
    PF_Boolean  paramPoint09VisibleB;
    PF_Boolean  paramPoint10VisibleB;


    A_char   paramCb01NameAc[32];
    A_char   paramCb02NameAc[32];
    A_char   paramCb03NameAc[32];
    A_char   paramCb04NameAc[32];
    A_char   paramCb05NameAc[32];
    A_char   paramCb06NameAc[32];
    A_char   paramCb07NameAc[32];
    A_char   paramCb08NameAc[32];
    A_char   paramCb09NameAc[32];
    A_char   paramCb10NameAc[32];
    PF_Boolean  paramCb01VisibleB;
    PF_Boolean  paramCb02VisibleB;
    PF_Boolean  paramCb03VisibleB;
    PF_Boolean  paramCb04VisibleB;
    PF_Boolean  paramCb05VisibleB;
    PF_Boolean  paramCb06VisibleB;
    PF_Boolean  paramCb07VisibleB;
    PF_Boolean  paramCb08VisibleB;
    PF_Boolean  paramCb09VisibleB;
    PF_Boolean  paramCb10VisibleB;

    A_char   paramColor01NameAc[32];
    A_char   paramColor02NameAc[32];
    A_char   paramColor03NameAc[32];
    A_char   paramColor04NameAc[32];
    A_char   paramColor05NameAc[32];
    A_char   paramColor06NameAc[32];
    A_char   paramColor07NameAc[32];
    A_char   paramColor08NameAc[32];
    A_char   paramColor09NameAc[32];
    A_char   paramColor10NameAc[32];
    PF_Boolean  paramColor01VisibleB;
    PF_Boolean  paramColor02VisibleB;
    PF_Boolean  paramColor03VisibleB;
    PF_Boolean  paramColor04VisibleB;
    PF_Boolean  paramColor05VisibleB;
    PF_Boolean  paramColor06VisibleB;
    PF_Boolean  paramColor07VisibleB;
    PF_Boolean  paramColor08VisibleB;
    PF_Boolean  paramColor09VisibleB;
    PF_Boolean  paramColor10VisibleB;
	A_char   paramLayer00NameAc[32];
    A_char   paramLayer01NameAc[32];
    PF_Boolean  paramLayer01VisibleB;


    //Mode
    PF_Boolean  glslModeB;
	PF_Boolean  exprModeB;
	PF_Boolean  evalModeB;
    PF_Boolean needsPixelAroundB;
    PF_Boolean pixelsCallExternalInputB;
    PF_Boolean needsLumaB;
    PF_Boolean presetHasWideInputB;
	PF_Boolean resetShaderB;

} seqData, *seqDataP, **seqDataH;


/* Parameter defaults */

#define	MATH_VAR_MIN		-1000
#define	MATH_SLIDER_MIN  	0
#define	MATH_VAR_MAX		1000
#define	MATH_SLIDER_MAX		100
#define	MATH_VAR_DFLT       1

#define TIMEOFFSET_MIN      -1000
#define TIMEOFFSET_MAX       1000
#define TIMEOFFSET_DFLT      0


enum {
	MATH_INPUT = 0,
	MATH_SETUP,
    MATH_ARB_DATA,
    MATH_TOPIC_SLIDER,
	MATH_SLIDER_ONE,
    MATH_SLIDER_TWO,
    MATH_SLIDER_THREE,
    MATH_SLIDER_FOUR,
    MATH_SLIDER_FIVE,
    MATH_SLIDER_SIX,
    MATH_SLIDER_SEVEN,
    MATH_SLIDER_HEIGHT,
    MATH_SLIDER_NINE,
    MATH_SLIDER_TEN,
    MATH_END_TOPIC_SLIDER,
    MATH_TOPIC_POINTS,
    MATH_POINT_ONE,
    MATH_POINT_TWO,
    MATH_POINT_THREE,
    MATH_POINT_FOUR,
    MATH_POINT_FIVE,
    MATH_POINT_SIX,
    MATH_POINT_SEVEN,
    MATH_POINT_HEIGHT,
    MATH_POINT_NINE,
    MATH_POINT_TEN,
    MATH_END_TOPIC_POINTS,
    MATH_TOPIC_CB,
    MATH_CB_ONE,
    MATH_CB_TWO,
    MATH_CB_THREE,
    MATH_CB_FOUR,
    MATH_CB_FIVE,
    MATH_CB_SIX,
    MATH_CB_SEVEN,
    MATH_CB_HEIGHT,
    MATH_CB_NINE,
    MATH_CB_TEN,
    MATH_END_TOPIC_CB,
    MATH_TOPIC_COLORS,
    MATH_COLOR_ONE,
    MATH_COLOR_TWO,
    MATH_COLOR_THREE,
    MATH_COLOR_FOUR,
    MATH_COLOR_FIVE,
    MATH_COLOR_SIX,
    MATH_COLOR_SEVEN,
    MATH_COLOR_HEIGHT,
    MATH_COLOR_NINE,
    MATH_COLOR_TEN,
    MATH_END_TOPIC_COLORS,
    MATH_TOPIC_INPUTS,
    MATH_INP_LAYER_ONE,
    MATH_INP_TOFF_ONE,
    MATH_INP_POFF_ONE,
    MATH_END_TOPIC_INPUTS,
	MATH_CEP_GET_ARB_DATA,
	MATH_NUM_PARAMS
};

enum {
	MATH_SETUP_DISK_ID =1,
	MATH_ARB_DATA_DISK_ID,
    MATH_TOPIC_SLIDER_DISK_ID,
    MATH_SLIDER_ONE_DISK_ID,
    MATH_SLIDER_TWO_DISK_ID,
    MATH_SLIDER_THREE_DISK_ID,
    MATH_SLIDER_FOUR_DISK_ID,
    MATH_SLIDER_FIVE_DISK_ID,
    MATH_SLIDER_SIX_DISK_ID,
    MATH_SLIDER_SEVEN_DISK_ID,
    MATH_SLIDER_HEIGHT_DISK_ID,
    MATH_SLIDER_NINE_DISK_ID,
    MATH_SLIDER_TEN_DISK_ID,
    MATH_END_TOPIC_SLIDER_DISK_ID,
    MATH_TOPIC_POINTS_DISK_ID,
    MATH_POINT_ONE_DISK_ID,
    MATH_POINT_TWO_DISK_ID,
    MATH_POINT_THREE_DISK_ID,
    MATH_POINT_FOUR_DISK_ID,
    MATH_POINT_FIVE_DISK_ID,
    MATH_POINT_SIX_DISK_ID,
    MATH_POINT_SEVEN_DISK_ID,
    MATH_POINT_HEIGHT_DISK_ID,
    MATH_POINT_NINE_DISK_ID,
    MATH_POINT_TEN_DISK_ID,
    MATH_END_TOPIC_POINTS_DISK_ID,
    MATH_TOPIC_CB_DISK_ID,
    MATH_CB_ONE_DISK_ID,
    MATH_CB_TWO_DISK_ID,
    MATH_CB_THREE_DISK_ID,
    MATH_CB_FOUR_DISK_ID,
    MATH_CB_FIVE_DISK_ID,
    MATH_CB_SIX_DISK_ID,
    MATH_CB_SEVEN_DISK_ID,
    MATH_CB_HEIGHT_DISK_ID,
    MATH_CB_NINE_DISK_ID,
    MATH_CB_TEN_DISK_ID,
    MATH_END_TOPIC_CB_DISK_ID,
    MATH_TOPIC_COLORS_DISK_ID,
    MATH_COLOR_ONE_DISK_ID,
    MATH_COLOR_TWO_DISK_ID,
    MATH_COLOR_THREE_DISK_ID,
    MATH_COLOR_FOUR_DISK_ID,
    MATH_COLOR_FIVE_DISK_ID,
    MATH_COLOR_SIX_DISK_ID,
    MATH_COLOR_SEVEN_DISK_ID,
    MATH_COLOR_HEIGHT_DISK_ID,
    MATH_COLOR_NINE_DISK_ID,
    MATH_COLOR_TEN_DISK_ID,
    MATH_END_TOPIC_COLORS_DISK_ID,
    MATH_TOPIC_INPUTS_DISK_ID,
    MATH_INP_LAYER_ONE_DISK_ID,
    MATH_INP_TOFF_ONE_DISK_ID,
    MATH_INP_POFF_ONE_DISK_ID,
    MATH_END_TOPIC_INPUTS_DISK_ID,
	MATH_CEP_GET_ARB_DATA_DISK_ID
};

typedef struct  FlagsInfo {
        PF_Boolean NeedsPixelAroundB;
        PF_Boolean PixelsCallExternalInputB;
        PF_Boolean NeedsLumaB;
        PF_Boolean PresetHasWideInput;
		PF_Boolean parserModeB;
}FlagsInfoP;

typedef struct {
	std::string  *redstr;
	std::string  *greenstr;
	std::string  *bluestr;
	std::string  *alphastr;
	std::string  *frag1str;
	std::string  *frag2str;
	std::string  *vertexstr;
}ExprInfoP;

typedef struct funcTransfertInfo {
	
	std::function<PF_FpShort()> redExpr;
	std::function<PF_FpShort()> greenExpr;
	std::function<PF_FpShort()> blueExpr;
	std::function<PF_FpShort()> alphaExpr;
    std::function<PF_FpShort()> evalExpr;
	PF_Boolean      hasErrorB;
	std::string     channelErrorstr;
	std::string     errorstr;
}funcTransfertInfoP;

typedef struct WorldTransfertInfo {
    PF_EffectWorld  inW;
    PF_EffectWorld  outW;
    PF_EffectWorld  extLW;
}WorldTransfertInfoP;


typedef struct MathInfo{
    PF_FpShort		inRedF;
    PF_FpShort		inGreenF;
    PF_FpShort		inBlueF;
    PF_FpShort		inAlphaF;
    PF_FpShort		xLF;
    PF_FpShort		yLF;
    PF_FpShort      extL_red;
    PF_FpShort      extL_green;
    PF_FpShort      extL_blue;
    PF_FpShort      extL_alpha;
	PF_FpShort      inSliderF[10];
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
	std::map<int, point_3d>   points;

    PF_FpShort      colorOne[3];
    PF_FpShort      colorTwo[3];
	PF_FpShort      m9P_red[9];
	PF_FpShort		m9P_green[9];
	PF_FpShort		m9P_blue[9];
	PF_FpShort      m9P_alpha[9];
	PF_FpShort		luma;
	PF_PixelFloat   PixelOFfP;
    PF_Fixed    x_offFi;
    PF_Fixed    y_offFi;

} MathInfoP, *MathinfoP, **MathinfoH;


typedef struct {
    PF_Fixed    x_offFi;
    PF_Fixed    y_offFi;
    PF_SampPB    samp_pb;
    PF_InData    in_data;
} OffInfo;

typedef struct {
	AEGP_PluginID	my_id;
} my_global_data, *my_global_dataP, **my_global_dataH;


struct point_3d {
	PF_FpShort points[3];
};



template <typename T=PF_FpShort> class parseExpr {
private:
    std::shared_ptr<exprtk::parser<T>> parser;
    exprtk::expression<T> expression;
    exprtk::symbol_table<T> symbol_table;
public:
    parseExpr(void *mathRefcon, void *refconFunc, const std::string &exprstr,seqDataP seqP) {
        MathInfo	*miP	= reinterpret_cast<MathInfo*>(mathRefcon);
		funcTransfertInfo *fiP = reinterpret_cast<funcTransfertInfo*>(refconFunc);

        std::string  expression_string_Safe = "1";
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
        symbol_table.add_vector("red_off", miP->m9P_red);
        symbol_table.add_vector("green_off",miP->m9P_green);
        symbol_table.add_vector("blue_off", miP->m9P_blue);
        symbol_table.add_vector("alpha_off", miP->m9P_alpha);

		symbol_table.add_vector(seqP->paramPoint01NameAC, miP->points[0]);
		symbol_table.add_vector(seqP->paramPoint02NameAC, miP->points[1]);
		symbol_table.add_vector(seqP->paramPoint03NameAC, miP->points[2]);
		symbol_table.add_vector(seqP->paramPoint04NameAC, miP->points[3]);
		symbol_table.add_vector(seqP->paramPoint05NameAC, miP->points[4]);
		symbol_table.add_vector(seqP->paramPoint06NameAC, miP->points[5]);
		symbol_table.add_vector(seqP->paramPoint07NameAC, miP->points[6]);
		symbol_table.add_vector(seqP->paramPoint08NameAC, miP->points[7]);
		symbol_table.add_vector(seqP->paramPoint09NameAC, miP->points[8]);
		symbol_table.add_vector(seqP->paramPoint10NameAC, miP->points[9]);

        symbol_table.add_variable("extL_red", miP->extL_red);
        symbol_table.add_variable("extL_green", miP->extL_green);
        symbol_table.add_variable("extL_blue", miP->extL_blue);
        symbol_table.add_variable("extL_alpha", miP->extL_alpha);
        
        symbol_table.add_constants();
        symbol_table.add_constant(seqP->paramSlider01NameAc, miP->inSliderF[0]);
        symbol_table.add_constant(seqP->paramSlider02NameAc, miP->inSliderF[1]);
        symbol_table.add_constant(seqP->paramSlider03NameAc,miP->inSliderF[2]);
        symbol_table.add_constant(seqP->paramSlider04NameAc,miP->inSliderF[3]);
		symbol_table.add_constant(seqP->paramSlider05NameAc, miP->inSliderF[4]);
		symbol_table.add_constant(seqP->paramSlider06NameAc, miP->inSliderF[5]);
		symbol_table.add_constant(seqP->paramSlider07NameAc, miP->inSliderF[6]);
		symbol_table.add_constant(seqP->paramSlider08NameAc, miP->inSliderF[7]);
		symbol_table.add_constant(seqP->paramSlider09NameAc, miP->inSliderF[8]);
		symbol_table.add_constant(seqP->paramSlider10NameAc, miP->inSliderF[9]);
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
        symbol_table.add_constant("layerScale_x", miP->layerScale_X);
        symbol_table.add_constant("layerScale_y", miP->layerScale_Y);
        symbol_table.add_constant("layerScale_z", miP->layerScale_Z);
        symbol_table.add_constant("compWidth", miP->compWidthF);
        symbol_table.add_constant("compHeight", miP->compHeightF);
        symbol_table.add_constant("compFps", miP->compFpsF);
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
    
    void render_8(void *refconPV, void *refconFunc, void *refconFlags,void *refconWorld,  A_long thread_idxL, A_long numThreads, A_long numIter, A_long lastNumIter);

    void render_16(void *refconPV, void *refconFunc, void *refconFlags,void *refconWorld, A_long thread_idxL, A_long numThreads, A_long numIter, A_long lastNumIter);

	void render_32(void *refconPV, void *refconFunc, void *refconFlags,void *refconWorld, A_long thread_idxL, A_long numThreads, A_long numIter, A_long lastNumIter);

    
};




//GLSL SCRIPTS
static std::string glvertstr = "#version 330 \n\
in vec4 Position;\n\
in vec2 UVs;\n\
out vec4 out_pos;\n\
out vec2 out_uvs;\n\
uniform mat4 ModelviewProjection;\n\
void main(void)\n\
{\n\
	out_pos = ModelviewProjection * Position; \n\
	gl_Position = out_pos; \n\
	out_uvs = UVs;\n\
}";



static std::string glfrag2str = "#version 330\n\
uniform sampler2D layerTex;\n\
uniform float multiplier16bit;\n\
in vec4 out_pos;\n\
in vec2 out_uvs;\n\
out vec4 colourOut;\n\
void main(void)\n\
{\n\
	colourOut = texture( layerTex, out_uvs.xy ); \n\
	if (colourOut.a == 0) {\n\
		colourOut = vec4(0, 0, 0, 0);\n\
	} else {\n\
		colourOut = vec4(colourOut.a, colourOut.r / colourOut.a, colourOut.g / colourOut.a, colourOut.b / colourOut.a); \n\
	}\n\
	colourOut = colourOut / multiplier16bit;\n\
}";
//helper func
inline u_char AlphaLookup(u_int16 inValSu, u_int16 inMaxSu)
{
	fpshort normValFp = 1.0f - (inValSu) / static_cast<fpshort>(inMaxSu);
	return static_cast<u_char>(normValFp*normValFp*0.8f * 255);
}

static std::string glErrorMessageStr = R"=====(
    // based on http://glslsandbox.com/e#53346.0
    #version 330 // glsls version for opengl 3.3
    uniform float multiplier16bit; //proper to AE 16 bits depth.

    out vec4 fragColorOut;
    vec2 uv;
    uniform vec2 resolution;

    const vec2 ch_size  = vec2(1.0, 2.0) * 0.6;              // character size (X,Y)
    const vec2 ch_space = ch_size + vec2(1.0, 1.0);    // character distance Vector(X,Y)
    const vec2 ch_start = vec2 (ch_space.x * -5., 1.); // start position
    vec2 ch_pos   = vec2 (0.0, 0.0);             // character position(X,Y)


    #define REPEAT_SIGN false // True/False; True=Multiple, False=Single

    #define S ddigit(0x8866);
    #define H ddigit(0x1199);
    #define A ddigit(0x119F);
    #define D ddigit(0x44E7);
    #define E ddigit(0x107E);
    #define R ddigit(0x911F);
    #define _ ch_pos.x += ch_space.x;
    #define O ddigit(0x00FF);
    #define nl1 ch_pos = ch_start;  ch_pos.y -= 3.0;


    float dseg(vec2 p0, vec2 p1)
    {
        vec2 dir = normalize(p1 - p0);
        vec2 cp = (uv - ch_pos - p0) * mat2(dir.x, dir.y,-dir.y, dir.x);
        return distance(cp, clamp(cp, vec2(0), vec2(distance(p0, p1), 0)));
    }

    bool bit(int n, int b)
    {
        return mod(floor(float(n) / exp2(floor(float(b)))), 2.0) != 0.0;
    }

    float d = 1e6;

    void ddigit(int n)
    {
        float v = 1e6;
        vec2 cp = uv - ch_pos;
        if (n == 0)     v = min(v, dseg(vec2(-0.405, -1.000), vec2(-0.500, -1.000)));
        if (bit(n,  0)) v = min(v, dseg(vec2( 0.500,  0.063), vec2( 0.500,  0.937)));
        if (bit(n,  1)) v = min(v, dseg(vec2( 0.438,  1.000), vec2( 0.063,  1.000000000)));
        if (bit(n,  2)) v = min(v, dseg(vec2(-0.063,  1.000), vec2(-0.438,  1.000)));
        if (bit(n,  3)) v = min(v, dseg(vec2(-0.500,  0.937), vec2(-0.500,  0.062)));
        if (bit(n,  4)) v = min(v, dseg(vec2(-0.500, -0.063), vec2(-0.500, -0.938)));
        if (bit(n,  5)) v = min(v, dseg(vec2(-0.438, -1.000), vec2(-0.063, -1.000)));
        if (bit(n,  6)) v = min(v, dseg(vec2( 0.063, -1.000), vec2( 0.438, -1.000)));
        if (bit(n,  7)) v = min(v, dseg(vec2( 0.500, -0.938), vec2( 0.500, -0.063)));
        if (bit(n,  8)) v = min(v, dseg(vec2( 0.063,  0.000), vec2( 0.438, -0.000)));
        if (bit(n,  9)) v = min(v, dseg(vec2( 0.063,  0.063), vec2( 0.438,  0.938)));
        if (bit(n, 10)) v = min(v, dseg(vec2( 0.000,  0.063), vec2( 0.000,  0.937)));
        if (bit(n, 11)) v = min(v, dseg(vec2(-0.063,  0.063), vec2(-0.438,  0.938)));
        if (bit(n, 12)) v = min(v, dseg(vec2(-0.438,  0.000), vec2(-0.063, -0.000)));
        if (bit(n, 13)) v = min(v, dseg(vec2(-0.063, -0.063), vec2(-0.438, -0.938)));
        if (bit(n, 14)) v = min(v, dseg(vec2( 0.000, -0.938), vec2( 0.000, -0.063)));
        if (bit(n, 15)) v = min(v, dseg(vec2( 0.063, -0.063), vec2( 0.438, -0.938)));
        ch_pos.x += ch_space.x;
        d = min(d, v);
    }
    mat2 rotate(float a)
    {
        float c = cos(a);
        float s = sin(a);
        return mat2(c, s, -s, c);
    }
    vec3 hsv2rgb_smooth( in vec3 c )
    {
        vec3 rgb = clamp( abs(mod(c.x*6.0+vec3(0.0,4.0,2.0),6.0)-3.0)-1.0, 0.0, 1.0 );
        rgb = rgb*rgb*(3.0-2.0*rgb); // cubic smoothing
        return c.z * mix( vec3(1.0), rgb, c.y);
    }
    void main( void )
    {
        vec2 aspect = resolution.xy / resolution.y;
        vec2 fragCoord = gl_FragCoord.xy;
        fragCoord.y =  resolution.y -gl_FragCoord.y;
        uv =  ( fragCoord.xy / resolution.y ) - aspect / 2.0;
        float _d =  1.0-length(uv);
        uv *= 18.0 ;
        uv.y -= 3.;
        vec3 ch_color = hsv2rgb_smooth(vec3(1*-.04+uv.y*0.1,0.5,1.0));
        vec3 bg_color = vec3(_d*0.4, _d*0.2, _d*0.1);
        ch_pos = ch_start;
        nl1  S H A D E R _ E R R O R
        vec3 color = mix(ch_color, bg_color, 1.0- (0.08 / d*2.0));  // shading
        fragColorOut = vec4(1.-pow(length(color)-0.25, 4.)*vec3(.5,.5,.5), 1.0);
    }
)=====";
static std::string exprErrorMessageStr ="0";

//error checking macro
#define CHECK(err) {PF_Err err1 = err; if (err1 != PF_Err_NONE ){ throw PF_Err(err1);}}

#endif


#ifdef __cplusplus
extern "C" {
#endif

    DllExport    PF_Err
    EntryPointFunc(
                   PF_Cmd            cmd,
                   PF_InData        *in_data,
                   PF_OutData        *out_data,
                   PF_ParamDef        *params[],
                   PF_LayerDef        *output,
                   void            *extra) ;

#ifdef __cplusplus
}

std::string evalMathExprStr(std::string expr, seqDataP    *seqP);
void evalFragShader(std::string inFragmentShaderStr, std::string& errReturn);
void evalVertShader(std::string inVertShaderStr, std::string& errReturn);

PF_Err
CreateDefaultArb(
                 PF_InData            *in_data,
                 PF_OutData            *out_data,
                 PF_ArbitraryH        *dephault);


PF_Err
Arb_Copy(
         PF_InData                *in_data,
         PF_OutData                *out_data,
         const PF_ArbitraryH        *srcP,
         PF_ArbitraryH            *dstP);

PF_Err
Arb_Interpolate(
                PF_InData                *in_data,
                PF_OutData                *out_data,
                double                    itrp_amtF,
                const PF_ArbitraryH        *l_arbP,
                const PF_ArbitraryH        *r_arbP,
                PF_ArbitraryH            *result_arbP);

PF_Err
Arb_Compare(
            PF_InData                *in_data,
            PF_OutData                *out_data,
            const PF_ArbitraryH        *a_arbP,
            const PF_ArbitraryH        *b_arbP,
            PF_ArbCompareResult        *resultP);

PF_Err
Arb_Print_Size();

PF_Err
Arb_Print(
          PF_InData            *in_data,
          PF_OutData            *out_data,
          PF_ArbPrintFlags    print_flags,
          PF_ArbitraryH        arbH,
          A_u_long            print_sizeLu,
          A_char                *print_bufferPC);
PF_Err
Arb_Scan(
         PF_InData            *in_data,
         PF_OutData            *out_data,
         void                 *refconPV,
         const char            *bufPC,
         unsigned long        bytes_to_scanLu,
         PF_ArbitraryH        *arbPH);


static std::string compile_success = "compiled successfully";
static std::string safeExpr ="0";

std::string strCopyAndReplace(std::string str,
                              const std::string& oldStr,
                              const std::string& newStr);
PF_Boolean
strToBoolean( std::string str);
void
strReplace(std::string& str,
           const std::string& oldStr,
           const std::string& newStr);

void ExprtkCorrectorStr(std::string &str);

PF_Err
LineIteration8Func ( void *refconPV,
                    void *refconFunc,
                    void *refconFlags,
                    void *refconWorld,
                    A_long yL);
PF_Err
LineIteration16Func ( void *refconPV,
                     void *refconFunc,
                     void *refconFlags,
                     void *refconWorld,
                     A_long yL);

PF_Err
LineIteration32Func(void *refconPV,
                    void *refconFunc,
                    void *refconFlags,
                    void *refconWorld,
                    A_long yL);
PF_Err
CallCepDialog(PF_InData        *in_data,
              PF_OutData        *out_data);
PF_Err
SetupDialogSend(
	PF_InData        *in_data,
	PF_OutData        *out_data,
	PF_ParamDef        *params[]);

PF_Err
AEGP_GetParamStreamValue(PF_InData            *in_data,
	PF_OutData            *out_data,
	AEGP_PluginID        PlugId,
	PF_ParamIndex        param_index,
	PF_Handle           *ArbH);

PF_Err
SetupGetDataBack(
                PF_InData        *in_data,
                PF_OutData        *out_data,
                PF_ParamDef        *params[]);

PF_Err
copyFromArbToSeqData(std::string       arbStr,
                     seqData    *seqData);
PF_Err
evalScripts  (seqData  *seqDataP);
PF_Err
ShiftImage32 (
              void         *refcon,
              A_long         xL,
              A_long         yL,
              PF_Pixel32     *inP,
              PF_Pixel32     *outP);
PF_Err
ShiftImage16 (
              void         *refcon,
              A_long         xL,
              A_long         yL,
              PF_Pixel16     *inP,
              PF_Pixel16     *outP);
PF_Err
ShiftImage8 (
             void         *refcon,
             A_long         xL,
             A_long         yL,
             PF_Pixel     *inP,
             PF_Pixel     *outP);

PF_Err
MakeParamCopy(
	PF_ParamDef *actual[],
	PF_ParamDef copy[]);
PF_Err
tlmath_updateSeqData(PF_InData			*in_data,
	PF_OutData			*out_data,
	PF_ParamDef			*params[]);

PF_Err
tlmath_UpdateParameterUI(
	PF_InData			*in_data,
	PF_OutData			*out_data,
	PF_ParamDef			*params[],
	PF_LayerDef			*outputP);
PF_Err
tlmath_UserChangedParam(
	PF_InData						*in_data,
	PF_OutData						*out_data,
	PF_ParamDef						*params[],
	PF_LayerDef						*outputP,
	const PF_UserChangedParamExtra	*which_hitP);

PF_Err
tlmath_ParamsSetup (
             PF_InData        *in_data,
             PF_OutData        *out_data,
             PF_ParamDef        *params[],
              PF_LayerDef        *output );

PF_Err
tl_math_SmartRender(
	PF_InData                *in_data,
	PF_OutData                *out_data,
	PF_SmartRenderExtra        *extraP);

PF_Err
tl_math_PreRender(PF_InData                *in_data,
	PF_OutData                *out_data,
	PF_PreRenderExtra        *extraP);

PF_Err
Render_GLSL(PF_InData                *in_data,
	PF_OutData               *out_data,
	PF_EffectWorld           *inputP,
	PF_EffectWorld           *outputP,
	PF_EffectWorld           *extLW,
	PF_PixelFormat           format,
	AEGP_SuiteHandler        &suites,
	void                    *refcon,
	PF_Boolean              ShaderResetB,
	const std::string&		vertexShstr,
	const std::string&		fragSh1str,
	const std::string&		fragSh2str);


#endif // TLMATH
