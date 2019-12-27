
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

#include "json.hpp"
#include  "tl_defaultArb.h"
#include  "tl_defaultShaders.h"
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
#define	STAGE_VERSION	PF_Stage_BETA
#define	BUILD_VERSION	5

#define ARB_REFCON            (void*)0xDEADBEEFDEADBEEF
typedef struct {
    A_char arbDataAc[250000];
	bool hasChangedB;
} m_ArbData;
typedef struct {
	//PF_State		state;
	bool    initializedB;
	A_char  descriptionAc[2048];
	A_char  presetNameAc[32];

    A_char  redExAc[4096];
    A_char  greenExAc[4096];
    A_char  blueExAc[4096];
    A_char  alphaExAc[4096];
	A_char  rgbExprExAc[4096];
	A_char  commonExpr[8192];
    A_char  Glsl33_VertexShAc[25000];
    A_char  Glsl33_FragmentShAc[50000];

	A_char redError[2048];
    A_char greenError[4096];
    A_char blueError[4096];
    A_char alphaError[4096];
	A_char rgbError[4096];
	A_char commonError[4096];

    A_char Glsl33_VertError[25000];
    A_char Glsl33_fragError[25000];

	A_char  resolutionNameAc[32];
	A_char  layerPositionNameAc[32];
	A_char  layerScaleNameAc[32];
	A_char  compResolutionNameAc[32];
	A_char  time_secNameAc[32];
	A_char  time_frameNameAc[32];
	A_char  frame_rateNameAc[32];

    A_char expr_ColorChNameAc[32];
	A_char	expr_pixNameAc[32];


    A_char   cameraPosNameAc[32];
    A_char   cameraTargetNameAc[32];
    A_char   cameraRotationNameAc[32];
    A_char   cameraZoomNameAc[32];

    A_char sliderGrpNameAc[32];
    PF_Boolean sliderGrpVisibleB;

    A_char       pointGrpNameAc[32];
    PF_Boolean   pointGrpVisibleB;

    A_char       cbGrpNameAc[32];
    PF_Boolean  cbGrpVisibleB;

    A_char       colorGrpNameAc[32];
    PF_Boolean   colorGrpVisibleB;

	A_char       rotGrpNameAc[32];
	PF_Boolean   rotGrpVisibleB;

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

	A_char   paramRot01NameAc[32];
	A_char   paramRot02NameAc[32];
	A_char   paramRot03NameAc[32];
	A_char   paramRot04NameAc[32];
	A_char   paramRot05NameAc[32];
	A_char   paramRot06NameAc[32];
	A_char   paramRot07NameAc[32];
	A_char   paramRot08NameAc[32];
	A_char   paramRot09NameAc[32];
	A_char   paramRot10NameAc[32];
	PF_Boolean  paramRot01VisibleB;
	PF_Boolean  paramRot02VisibleB;
	PF_Boolean  paramRot03VisibleB;
	PF_Boolean  paramRot04VisibleB;
	PF_Boolean  paramRot05VisibleB;
	PF_Boolean  paramRot06VisibleB;
	PF_Boolean  paramRot07VisibleB;
	PF_Boolean  paramRot08VisibleB;
	PF_Boolean  paramRot09VisibleB;
	PF_Boolean  paramRot10VisibleB;


	A_char   paramLayer00NameAc[32];
    A_char   paramLayer01NameAc[32];
    PF_Boolean  paramLayer01VisibleB;
    A_char   paramLayer02NameAc[32];
    PF_Boolean  paramLayer02VisibleB;
    A_char   paramLayer03NameAc[32];
    PF_Boolean  paramLayer03VisibleB;
    A_char   paramLayer04NameAc[32];
    PF_Boolean  paramLayer04VisibleB;

    //Mode
    PF_Boolean  cameraB;
    PF_Boolean  exprRGBModeB;
    PF_Boolean  glsl33ModeB;
	PF_Boolean  exprModeB;
	PF_Boolean  evalModeB;
    PF_Boolean  presetHasWideInputB;
    bool pixelsCallExternalInputB[4];
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
    MATH_EFFECT_DESCRIPTION,
	MATH_RESET,
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
	MATH_TOPIC_ROTS,
	MATH_ROT_ONE,
	MATH_ROT_TWO,
	MATH_ROT_THREE,
	MATH_ROT_FOUR,
	MATH_ROT_FIVE,
	MATH_ROT_SIX,
	MATH_ROT_SEVEN,
	MATH_ROT_HEIGHT,
	MATH_ROT_NINE,
	MATH_ROT_TEN,
	MATH_END_TOPIC_ROTS,
    MATH_TOPIC_INPUTS,
    MATH_INP_LAYER_ONE,
    MATH_INP_TOFF_ONE,
	MATH_INP_LAYER_TWO,
	MATH_INP_TOFF_TWO,
	MATH_INP_LAYER_THREE,
	MATH_INP_TOFF_THREE,
	MATH_INP_LAYER_FOUR,
	MATH_INP_TOFF_FOUR,
    MATH_END_TOPIC_INPUTS,
	MATH_CEP_GET_ARB_DATA,
    MATH_CEP_RETURN_MESSAGE,
	MATH_NUM_PARAMS
};
enum {
	MATH_SETUP_DISK_ID =1,
	MATH_ARB_DATA_DISK_ID,
    MATH_EFFECT_DESCRIPTION_DISK_ID,
	MATH_RESET_DISK_ID,
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
	MATH_TOPIC_ROTS_DISK_ID,
	MATH_ROT_ONE_DISK_ID,
	MATH_ROT_TWO_DISK_ID,
	MATH_ROT_THREE_DISK_ID,
	MATH_ROT_FOUR_DISK_ID,
	MATH_ROT_FIVE_DISK_ID,
	MATH_ROT_SIX_DISK_ID,
	MATH_ROT_SEVEN_DISK_ID,
	MATH_ROT_HEIGHT_DISK_ID,
	MATH_ROT_NINE_DISK_ID,
	MATH_ROT_TEN_DISK_ID,
	MATH_END_TOPIC_ROTS_DISK_ID,
    MATH_TOPIC_INPUTS_DISK_ID,
    MATH_INP_LAYER_ONE_DISK_ID,
    MATH_INP_TOFF_ONE_DISK_ID,
	MATH_INP_LAYER_TWO_DISK_ID,
	MATH_INP_TOFF_TWO_DISK_ID,
	MATH_INP_LAYER_THREE_DISK_ID,
	MATH_INP_TOFF_THREE_DISK_ID,
	MATH_INP_LAYER_FOUR_DISK_ID,
	MATH_INP_TOFF_FOUR_DISK_ID,
    MATH_END_TOPIC_INPUTS_DISK_ID,
    MATH_CEP_GET_ARB_DATA_DISK_ID,
    MATH_CEP_RETURN_MESSAGE_DISK_ID,
};
typedef struct  FlagsInfo {
        PF_Boolean PixelsCallExternalInputB[4];
        PF_Boolean PresetHasWideInput;
		PF_Boolean parserModeB;
        PF_Boolean exprRGBModeB; //for expression mode only
}FlagsInfoP;
typedef struct {
	std::string  *redstr;
	std::string  *greenstr;
	std::string  *bluestr;
	std::string  *alphastr;
    std::string   *rgbstr;
	std::string  *frag1str;
	std::string  *frag2str;
	std::string  *vertexstr;
}ExprInfoP;
typedef struct WorldTransfertInfo {
    PF_EffectWorld  inW;
    PF_EffectWorld  outW;
    PF_EffectWorld  extLW;
}WorldTransfertInfoP;
struct point_3d {
    PF_FpShort point[3];
};
struct color_3d {
    PF_FpShort color[3];
};

typedef struct MathInfo{
    PF_FpShort      scale_x;
    PF_FpShort      scale_y;
    PF_FpShort      layerSizeF[2];
    PF_FpShort      compSizeF[2];
    PF_FpShort		layerTime_Sec;
    PF_FpShort		layerTime_Frame;
    PF_FpShort		layerDuration;
	PF_FpShort      compFpsF;
    point_3d       layerPos;
    point_3d       layerScale;
    point_3d       cameraPos;
    point_3d       cameraTarget;
    point_3d        cameraRotation;
    PF_FpShort      cameraZoom;
    PF_FpShort      inSliderF[10];
	point_3d        inPoints[10];
    bool            inCboxF[10];
	PF_FpShort      inRotF[10];
    color_3d        inColors[10];
	PF_FpShort		luma;
	PF_PixelFloat   PixelOFfP;
	GLfloat          camMat[4][4];
    //PF_Fixed    x_offFi[4];
    //PF_Fixed    y_offFi[4];

} MathInfoP, *MathinfoP, **MathinfoH;
/*
typedef struct {
    PF_Fixed    x_offFi;
    PF_Fixed    y_offFi;
    PF_SampPB    samp_pb;
    PF_InData    in_data;
} OffInfo;*/
typedef struct {
	AEGP_PluginID	my_id;
} my_global_data, *my_global_dataP, **my_global_dataH;

//PF_Err ShiftImage32 (void *refcon, A_long xL, A_long yL, PF_Pixel32 *inP, PF_Pixel32 *outP);
//PF_Err ShiftImage16 ( void *refcon,A_long xL, A_long yL, PF_Pixel16 *inP, PF_Pixel16 *outP);
//PF_Err ShiftImage8 ( void *refcon, A_long xL, A_long yL, PF_Pixel *inP, PF_Pixel *outP);

class tlmath{
private:
    //std::string evalMathExprStr(std::string expr, seqDataP    *seqP);
    void evalFragShader(std::string inFragmentShaderStr, std::string& errReturn);
    void evalVertShader(std::string inVertShaderStr, std::string& errReturn);

    std::string strCopyAndReplace(std::string str,const std::string& oldStr,const std::string& newStr);
    PF_Boolean strToBoolean( std::string str);
    void strReplace(std::string& str,const std::string& oldStr,const std::string& newStr);
    void jsonCorrectorStr(std::string& str); //to json
    void scriptCorrectorStr(std::string& str); //from json
    void descriptionCorrectorStr (std::string& str);
	void copyExprFromJsonToSeqData(nlohmann::json arbDataJS, std::string json_adress, A_char* target);
	void evalScripts (seqData  *seqDataP);
    PF_Err CallCepDialog(PF_InData  *in_data, PF_OutData  *out_data);
    PF_Err SetupGetDataBack(PF_InData *in_data, PF_OutData  *out_data, PF_ParamDef *params[]);
    PF_Err copyFromArbToSeqData(PF_InData *in_data, PF_OutData   *out_data, std::string arbStr, seqData   *seqDataP);
    PF_Err MakeParamCopy( PF_ParamDef *actual[], PF_ParamDef copy[]);
    PF_Err updateSeqData(PF_InData *in_data, PF_OutData *out_data,  PF_ParamDef *params[]);
    PF_Err SetupDialogSend( PF_InData *in_data, PF_OutData *out_data, PF_ParamDef *params[]);
    PF_Err updateParamsValue(PF_InData* in_data, PF_ParamDef     *params[], std::string     arbStr);
    PF_Err embedExprInShaders  (seqData  *seqP);
	PF_Err AEGP_SetParamStreamValue(PF_InData* in_data, PF_OutData* out_data, AEGP_PluginID   PlugId, PF_ParamIndex   param_index, PF_Handle* ArbH);
	PF_Err updateArbState(PF_InData* in_data, PF_OutData* out_data);
	std::vector<int> getOccurenceFromStr(std::string src, std::string target);
	std::string ReIndexErrorInExpr(std::string originalfragSh, std::string  evalFragSh, std::string errIndex, size_t index, size_t delimiter);


    PF_Err Render_GLSL(PF_InData  *in_data,
                PF_OutData *out_data,
                PF_EffectWorld *inputP,
                PF_EffectWorld *outputP,
                PF_EffectWorld *extL1W,
                PF_EffectWorld *extL2W,
                PF_EffectWorld *extL3W,
                PF_EffectWorld *extL4W,
                PF_PixelFormat format,
                AEGP_SuiteHandler &suites,
                void  *refcon,
                const std::string& vertexShstr,
                const std::string& fragSh1str,
                const std::string&        fragSh2str);

    PF_Err
    ExtLayerInput(PF_InData       *in_data,
                  PF_EffectWorld *inputP,
                  PF_EffectWorld *extLP,
                  PF_EffectWorld *extLW,
                  AEGP_SuiteHandler &suites,
                  PF_PixelFormat   format);

    PF_Err Arb_Print_Size();
    PF_Err CreateDefaultArb( PF_InData  *in_data, PF_OutData *out_data, PF_ArbitraryH *dephault);
    PF_Err AEFX_AppendText(A_char  *srcAC, const A_u_long dest_sizeLu, A_char *destAC, A_u_long *current_indexPLu);
    PF_Err Arb_Copy( PF_InData *in_data, PF_OutData *out_data, const PF_ArbitraryH *srcP,PF_ArbitraryH *dstP);
    PF_Err Arb_Interpolate( PF_InData *in_data, PF_OutData *out_data, double  itrp_amtF, const PF_ArbitraryH *l_arbP, const PF_ArbitraryH *r_arbP, PF_ArbitraryH  *result_arbP);
    PF_Err Arb_Compare( PF_InData   *in_data, PF_OutData *out_data, const PF_ArbitraryH   *a_arbP,const PF_ArbitraryH  *b_arbP, PF_ArbCompareResult  *resultP);
    PF_Err Arb_Print( PF_InData *in_data, PF_OutData *out_data, PF_ArbPrintFlags print_flags, PF_ArbitraryH arbH, A_u_long print_sizeLu, A_char *print_bufferPC);
    PF_Err Arb_Scan( PF_InData *in_data, PF_OutData *out_data, void *refconPV, const char *bufPC, unsigned long bytes_to_scanLu, PF_ArbitraryH *arbPH);

public:
	PF_Err ExprRender(PF_OutData* out_data,
			PF_PixelFormat format,
			PF_EffectWorld* inputP,
			PF_EffectWorld* outputP,
			PF_EffectWorld* extLW,
			AEGP_SuiteHandler& suites,
			void* refcon,
			void* refconFlags,
			void* refconExpr);
    PF_Err UpdateParameterUI(PF_InData *in_data, PF_OutData *out_data, PF_ParamDef *params[], PF_LayerDef *outputP);
    PF_Err UserChangedParam( PF_InData  *in_data, PF_OutData *out_data,PF_ParamDef  *params[],PF_LayerDef *outputP, const PF_UserChangedParamExtra    *which_hitP);
    PF_Err ParamsSetup (PF_InData        *in_data, PF_OutData        *out_data, PF_ParamDef        *params[], PF_LayerDef        *output );
    PF_Err SmartRender(PF_InData *in_data, PF_OutData  *out_data,  PF_SmartRenderExtra *extraP);
    PF_Err PreRender(PF_InData  *in_data,  PF_OutData *out_data,PF_PreRenderExtra *extraP);
    PF_Err SequenceSetdown (PF_InData *in_data, PF_OutData *out_data);
    PF_Err SequenceSetup (PF_InData  *in_data, PF_OutData *out_data);
    PF_Err  HandleArbitrary( PF_InData *in_data, PF_OutData *out_data, PF_ParamDef *params[], PF_LayerDef *output, PF_ArbParamsExtra    *extra);
};

//glsl helper func
inline u_char AlphaLookup(u_int16 inValSu, u_int16 inMaxSu)
{
    fpshort normValFp = 1.0f - (inValSu) / static_cast<fpshort>(inMaxSu);
    return static_cast<u_char>(normValFp*normValFp*0.8f * 255);
}

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
#endif // TLMATH
