 /*
	rtt.h
	

*/

#pragma once

#ifndef RTT_H
#define RTT_H

#include "AEConfig.h"
#ifdef AE_OS_WIN
	#include <windows.h>
	#include <wingdi.h>
	#include "stdlib.h"
#endif

#define PF_DEEP_COLOR_AWARE 1

#include "rtt_Strings.h"

#include "entry.h"
#include "AE_Effect.h"
#include "AE_EffectUI.h"
#include "AE_EffectCB.h"
#include "AE_Macros.h"
#include "AE_EffectSuites.h"
#include "AE_EffectCBSuites.h"
#include "AE_AdvEffectSuites.h"
#include "AEFX_SuiteHelper.h"
#include "Smart_Utils.h"
#include "Param_Utils.h"
#include "String_Utils.h"

#include "AEGP_SuiteHandler.h"

#define	MAJOR_VERSION		1
#define	MINOR_VERSION		0
#define	BUG_VERSION			0
#define	STAGE_VERSION		PF_Stage_DEVELOP
#define	BUILD_VERSION		1

typedef enum {
	MODE_HORIZONTAL = 1,
	MODE_VERTICAL,
	MODE_SIZE = MODE_HORIZONTAL
} current_mode;



typedef struct {
	PF_Boolean		initializedB;
	AEGP_PluginID	my_id;
} my_global_data, *my_global_dataP, **my_global_dataH;

typedef struct {
    
	PF_State		   state;
   

} my_sequence_data, *my_sequence_dataP, **my_sequence_dataH;

typedef struct {
    PF_InData	  in_data;
    PF_SampPB	samp_pb;
    PF_FpLong   scale_x;
    PF_FpLong   scale_y;
    
    
    A_long     fos;
    A_long     timeIn;
    A_long     timeOut;
    
    A_long     horYPosA;
    A_long     horXposInA;
    A_long     horXposOutA;
    PF_Boolean  horFixXposInB;
    PF_Boolean  horFixXposOutB;
    
    PF_Boolean   modeB;
    A_long       layerWidthA;
    A_long       layerHeightA;
    A_long       compWidthA;
    A_long       compHeightA;
    
    AEGP_ThreeDVal positionTD;
    AEGP_ThreeDVal scaleTD;
    AEGP_ThreeDVal acPointTD;
    
    PF_FpLong     fpsF;
    A_Time        compTimeT;
    
    
    PF_Boolean   showSafeB;
    PF_FpLong    xSafety;
    PF_FpLong    ySafety;
    
    A_long    safeWidthInA;
    A_long    safeWidthOutA;
    A_long    safeHeightInA;
    A_long    safeHeightOutA;
    AEGP_ThreeDVal    SafePixelTD;
    
   
    
    AEGP_TwoDVal loffTD;
    AEGP_TwoDVal compoffTD;
    

} prerender_rtt, *pre_render_rttP, **pre_render_rttH;


void
copyWorld ( PF_InData		*in_data,
           PF_EffectWorld  *world_src,
           PF_EffectWorld  *world_dest,
           PF_LRect        *mask_src,
           PF_LRect        *mask_dest);

enum {
	RTT_INPUT = 0,
    RTT_LAYER_SOURCE,
    RTT_SPEED,
    RTT_CBSHOWSAFE,
	RTT_MODE,
    RTT_TOPIC_GR1,
    RTT_FIXEDY,
    RTT_POSX_IN,
    RTT_CB_XIN_SAFE,
    RTT_POSX_OUT,
    RTT_CB_XOUT_SAFE,
    END_TOPIC_GR1,
    RTT_TOPIC_GR2,
    RTT_FIXEDX,
    RTT_POSY_IN,
    RTT_CB_YIN_SAFE,
    RTT_POSY_OUT,
    RTT_CB_YOUT_SAFE,
    END_TOPIC_GR2,
    RTT_TOPIC_GR3,
    RTT_TIME_IN,
    RTT_TIME_OUT,
    END_TOPIC_GR3,
    RTT_TOPIC_GR4,
    RTT_ENABLE_SETTINGS,
    RTT_SCREEN_PRCT_SAFE_X,
    RTT_SCREEN_PRCT_SAFE_Y,
    RTT_TXT_PRCT_SAFE_X,
    RTT_TXT_PRCT_SAFE_Y,
    RTT_SAFE_PIXELS,
    END_TOPIC_GR4,
	RTT_NUM_PARAMS
};

enum {
    RTT_LAYER_SOURCE_DISK_ID =1,
    RTT_SPEED_DISK_ID,
    RTT_CB_SHOWSAFEDISK_ID,
	RTT_MODE_DISK_ID,
    TOPIC_GR1_DISK_ID,
    RTT_FIXEDY_DISK_ID,
    RTT_POSX_IN_DISK_ID,
    RTT_CB_XIN_SAFE_DISK_ID,
    RTT_POSX_OUT_DISK_ID,
    RTT_CB_XOUT_SAFE_DISK_ID,
    END_TOPIC_GR1_DISK_ID,
    TOPIC_GR2_DISK_ID,
    RTT_FIXEDX_DISK_ID,
    RTT_POSY_IN_DISK_ID,
    RTT_CB_YIN_SAFE_DISK_ID,
    RTT_POSY_OUT_DISK_ID,
    RTT_CB_YOUT_SAFE_DISK_ID,
    END_TOPIC_GR2_DISK_ID,
    TOPIC_GR3_DISK_ID,
    RTT_TIME_IN_DISK_ID,
    RTT_TIME_OUT_DISK_ID,
    END_TOPIC_GR3_DISK_ID,
    TOPIC_GR4_DISK_ID,
    RTT_ENABLE_SETTINGS_DISK_ID,
    RTT_SCREEN_PRCT_SAFE_X_DISK_ID,
    RTT_SCREEN_PRCT_SAFE_Y_DISK_ID,
    RTT_TXT_PRCT_SAFE_X_DISK_ID,
    RTT_TXT_PRCT_SAFE_Y_DISK_ID,
    RTT_SAFE_PIXELS_DISK_ID,
    END_TOPIC_GR4_DISK_ID,
};

extern "C" {
DllExport	PF_Err 
EntryPointFunc(	
	PF_Cmd			cmd,
	PF_InData		*in_data,
	PF_OutData		*out_data,
	PF_ParamDef		*params[],
	PF_LayerDef		*output,
	void			*extra );
}	
#endif  // SUPERVISOR_H
