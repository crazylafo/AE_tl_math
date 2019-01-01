#include "tl_math.h"
#include "script/script.h"


static PF_Err 
About (	
	PF_InData		*in_data,
	PF_OutData		*out_data,
	PF_ParamDef		*params[],
	PF_LayerDef		*output )
{
	AEGP_SuiteHandler suites(in_data->pica_basicP);
	
	suites.ANSICallbacksSuite1()->sprintf(	out_data->return_msg,
											"%s v%d.%d.%d\r%s",
											STR(StrID_Name), 
											MAJOR_VERSION, 
											MINOR_VERSION,
                                            BUG_VERSION,
											STR(StrID_Description));
	return PF_Err_NONE;
}

static PF_Err 
GlobalSetup (	
	PF_InData		*in_data,
	PF_OutData		*out_data,
	PF_ParamDef		*params[],
	PF_LayerDef		*output )
{
    PF_Err 				err 		= PF_Err_NONE;
    
    PF_Handle			globH		= NULL;
    my_global_dataP		globP		= NULL;
    AEGP_SuiteHandler		suites(in_data->pica_basicP);
    
	out_data->my_version = PF_VERSION(	MAJOR_VERSION, 
										MINOR_VERSION,
										BUG_VERSION, 
										STAGE_VERSION, 
										BUILD_VERSION);

    
	out_data->out_flags =  PF_OutFlag_CUSTOM_UI			|
                           PF_OutFlag_SEND_UPDATE_PARAMS_UI	|
                           PF_OutFlag_DEEP_COLOR_AWARE|	// just 16bpc, not 32bpc
                           PF_OutFlag_I_DO_DIALOG|
                           PF_OutFlag_WIDE_TIME_INPUT|
                           PF_OutFlag_NON_PARAM_VARY;
    

    
    globH	=	suites.HandleSuite1()->host_new_handle(sizeof(my_global_data));
    
    if (globH) {
        globP = reinterpret_cast<my_global_dataP>(suites.HandleSuite1()->host_lock_handle(globH));
        if (globP) {
            ERR(suites.UtilitySuite3()->AEGP_RegisterWithAEGP(NULL, STR(StrID_Name), &globP->my_id));
            
            if (!err){
                out_data->global_data 	= globH;
            }
        }
        suites.HandleSuite1()->host_unlock_handle(globH);
    } else	{
        err = PF_Err_INTERNAL_STRUCT_DAMAGED;
    }
	
	return 	err;
}

static PF_Err 
ParamsSetup (	
	PF_InData		*in_data,
	PF_OutData		*out_data,
	PF_ParamDef		*params[],
	PF_LayerDef		*output )
{
	PF_Err		err		= PF_Err_NONE;
	PF_ParamDef	def;
    
    AEFX_CLR_STRUCT(def);
    ERR(CreateDefaultArb(in_data,
                         out_data,
                         &def.u.arb_d.dephault));
    
    
    PF_ADD_ARBITRARY2(	"preset Name",
                      10,
                      10,
                      PF_ParamFlag_SUPERVISE| PF_ParamFlag_CANNOT_TIME_VARY,
                      PF_PUI_TOPIC,
                      def.u.arb_d.dephault,
                      MATH_ARB_DATA,
                      ARB_REFCON);
    
    if (!err) {
        PF_CustomUIInfo			ci;
        
        ci.events				= PF_CustomEFlag_EFFECT;
        
        ci.comp_ui_width		= ci.comp_ui_height = 0;
        ci.comp_ui_alignment	= PF_UIAlignment_NONE;
        
        ci.layer_ui_width		=
        ci.layer_ui_height		= 0;
        ci.layer_ui_alignment	= PF_UIAlignment_NONE;
        
        ci.preview_ui_width		=
        ci.preview_ui_height	= 0;
        ci.layer_ui_alignment	= PF_UIAlignment_NONE;
        
        err = (*(in_data->inter.register_ui))(in_data->effect_ref, &ci);
    }
    AEFX_CLR_STRUCT(def);

    PF_ADD_TOPIC(STR( StrID_TOPIC_SLIDER_Param_Name),  MATH_TOPIC_SLIDER_DISK_ID);
    
    AEFX_CLR_STRUCT(def);
    PF_ADD_FLOAT_SLIDERX(	STR(StrID_INPUTONE_Param_Name),
							MATH_VAR_MIN,
							MATH_VAR_MAX,
							MATH_VAR_MIN,
							MATH_SLIDER_MAX,
							MATH_VAR_DFLT,
							PF_Precision_THOUSANDTHS,
							0,
							0,
							MATH_INPONE_VAR_DISK_ID);

	AEFX_CLR_STRUCT(def);
    
    PF_ADD_FLOAT_SLIDERX(	STR(StrID_INPUTTWO_Param_Name),
                         MATH_VAR_MIN,
                         MATH_VAR_MAX,
                         MATH_VAR_MIN,
                         MATH_SLIDER_MAX,
                         MATH_VAR_DFLT,
                         PF_Precision_THOUSANDTHS,
                         0,
                         0,
                         MATH_INPTWO_VAR_DISK_ID);
    
    AEFX_CLR_STRUCT(def);
    
    PF_ADD_FLOAT_SLIDERX(	STR(StrID_INPUTTHREE_Param_Name),
                         MATH_VAR_MIN,
                         MATH_VAR_MAX,
                         MATH_VAR_MIN,
                         MATH_SLIDER_MAX,
                         MATH_VAR_DFLT,
                         PF_Precision_THOUSANDTHS,
                         0,
                         0,
                         MATH_INPTHREE_VAR_DISK_ID);
    
    AEFX_CLR_STRUCT(def);
    
    PF_ADD_FLOAT_SLIDERX(STR( StrID_INPUTFOUR_Param_Name),
                         MATH_VAR_MIN,
                         MATH_VAR_MAX,
                         MATH_VAR_MIN,
                         MATH_SLIDER_MAX,
                         MATH_VAR_DFLT,
                         PF_Precision_THOUSANDTHS,
                         0,
                         0,
                         MATH_INPFOUR_VAR_DISK_ID);
    
    AEFX_CLR_STRUCT(def);
    PF_END_TOPIC( MATH_TOPIC_SLIDER_DISK_ID);

    AEFX_CLR_STRUCT(def);
    

    PF_ADD_TOPIC(STR( StrID_TOPIC_POINTS_Param_Name), MATH_TOPIC_POINTS_DISK_ID);
	AEFX_CLR_STRUCT(def);
    PF_ADD_POINT(STR (strID_INPUTPOINTONE_Param_Name), 50, 50, FALSE, MATH_INP_POINT_ONE_DISK_ID);
    AEFX_CLR_STRUCT(def);
    PF_ADD_POINT(STR(strID_INPUTPOINTTWO_Param_Name), 50, 50, FALSE, MATH_INP_POINT_TWO_DISK_ID);
    AEFX_CLR_STRUCT(def);
    PF_END_TOPIC(MATH_TOPIC_POINTS_DISK_ID);
    AEFX_CLR_STRUCT(def);
    
    PF_ADD_TOPIC(STR( StrID_TOPIC_COLORS_Param_Name), MATH_TOPIC_COLORS_DISK_ID);
    AEFX_CLR_STRUCT(def);
    PF_ADD_COLOR(STR(strID_INPUTCOLORONE_Param_Name), PF_MAX_CHAN8, PF_MAX_CHAN8, PF_MAX_CHAN8, MATH_INP_COLOR_ONE_DISK_ID);
    AEFX_CLR_STRUCT(def);
    PF_ADD_COLOR(STR(strID_INPUTCOLORTWO_Param_Name), PF_MAX_CHAN8, PF_MAX_CHAN8, PF_MAX_CHAN8,MATH_INP_COLOR_TWO_DISK_ID);
    AEFX_CLR_STRUCT(def);
    PF_END_TOPIC (MATH_TOPIC_COLORS_DISK_ID);
    AEFX_CLR_STRUCT(def);
    
    PF_ADD_TOPIC(STR( StrID_TOPIC_INPUTS_Param_Name), MATH_TOPIC_INPUTS_DISK_ID);
    AEFX_CLR_STRUCT(def);

    PF_ADD_LAYER(STR(StrID_LAYER_ONE_Param_Name), PF_LayerDefault_MYSELF, MATH_INP_LAYER_ONE_DISK_ID);
    AEFX_CLR_STRUCT(def);
    PF_ADD_FLOAT_SLIDERX(STR( StrID_TOFF_ONE_Param_Name),
                         TIMEOFFSET_MIN,
                         TIMEOFFSET_MAX,
                         MATH_VAR_MIN,
                         MATH_SLIDER_MAX,
                         TIMEOFFSET_DFLT,
                         PF_Precision_INTEGER,
                         0,
                         0,
                         MATH_INP_TOFF_ONE_DISK_ID);
    
    PF_ADD_POINT(STR(StrID_POFF_ONE_Param_Name), 50, 50, FALSE, MATH_INP_POFF_ONE_DISK_ID);
    AEFX_CLR_STRUCT(def);

    PF_END_TOPIC(MATH_TOPIC_INPUTS_DISK_ID);
    AEFX_CLR_STRUCT(def);

    
    out_data->num_params = MATH_NUM_PARAMS;
    
    AEFX_SuiteScoper<PF_EffectUISuite1> effect_ui_suiteP = AEFX_SuiteScoper<PF_EffectUISuite1>(
                                                                                               in_data,
                                                                                               kPFEffectUISuite,
                                                                                               kPFEffectUISuiteVersion1,
                                                                                               out_data);
    
    ERR(effect_ui_suiteP->PF_SetOptionsButtonName(in_data->effect_ref, "Math Expression"));


	return err;
}

static PF_Err
MakeParamCopy(
              PF_ParamDef *actual[],	/* >> */
              PF_ParamDef copy[])		/* << */
{
    for (A_short iS = 0; iS < MATH_NUM_PARAMS; ++iS){
        AEFX_CLR_STRUCT(copy[iS]);	// clean params are important!
    }
    copy[MATH_INPUT]			= *actual[MATH_INPUT];
    copy[MATH_ARB_DATA]			= *actual[MATH_ARB_DATA];
    
    return PF_Err_NONE;
    
}
static PF_Err
UpdateParameterUI(
                  PF_InData			*in_data,
                  PF_OutData			*out_data,
                  PF_ParamDef			*params[],
                  PF_LayerDef			*outputP)
{
    PF_Err				err					= PF_Err_NONE;
    PF_Handle		arbH			= params[MATH_ARB_DATA]->u.arb_d.value;
    AEGP_SuiteHandler		suites(in_data->pica_basicP);
    m_ArbData		*arbP			= NULL;
    PF_ParamDef		param_copy[MATH_NUM_PARAMS];
    ERR(MakeParamCopy(params, param_copy));
    arbP = reinterpret_cast<m_ArbData*>(suites.HandleSuite1()->host_lock_handle(arbH));
    if (arbP) {
        m_ArbData *tempPointer = reinterpret_cast<m_ArbData*>(arbP);
        strcpy(param_copy[MATH_ARB_DATA].name, tempPointer->presetNameAc);
        ERR(suites.ParamUtilsSuite3()->PF_UpdateParamUI(in_data->effect_ref,
                                                        MATH_ARB_DATA,
                                                        &param_copy[MATH_ARB_DATA]));
        
    }
    
    
    PF_UNLOCK_HANDLE(arbH);
    return err;
}
static PF_Err
AEGP_SetParamStreamValue(PF_InData			*in_data,
						PF_OutData			*out_data,
						AEGP_PluginID		PlugId,
						PF_ParamIndex		param_index,
						PF_Handle           *ArbH)
{
	PF_Err  err = PF_Err_NONE,
			err2 = PF_Err_NONE;
	AEGP_LayerH		layerH;
	AEGP_StreamRefH effect_streamH = NULL;
	AEGP_EffectRefH   thisEffect_refH;
	AEGP_StreamValue2	val;
	AEGP_StreamValue2	*sample_valP = &val;
	A_Time currT;
	


	AEFX_SuiteScoper<AEGP_PFInterfaceSuite1> PFInterfaceSuite = AEFX_SuiteScoper<AEGP_PFInterfaceSuite1>(in_data,
		kAEGPPFInterfaceSuite,
		kAEGPPFInterfaceSuiteVersion1,
		out_data);
	AEFX_SuiteScoper<AEGP_LayerSuite8> layerSuite = AEFX_SuiteScoper<AEGP_LayerSuite8>(in_data,
		kAEGPLayerSuite,
		kAEGPLayerSuiteVersion8,
		out_data);

	AEFX_SuiteScoper<AEGP_StreamSuite4> StreamSuite = AEFX_SuiteScoper<AEGP_StreamSuite4>(in_data,
		kAEGPStreamSuite,
		kAEGPStreamSuiteVersion4,
		out_data);
	AEFX_SuiteScoper<AEGP_EffectSuite4> EffectSuite = AEFX_SuiteScoper<AEGP_EffectSuite4>(in_data,
		kAEGPEffectSuite,
		kAEGPEffectSuiteVersion4,
		out_data);


	PFInterfaceSuite->AEGP_GetEffectLayer(in_data->effect_ref, &layerH);
	layerSuite->AEGP_GetLayerCurrentTime(layerH, AEGP_LTimeMode_LayerTime, &currT);
	PFInterfaceSuite->AEGP_GetNewEffectForEffect(PlugId, in_data->effect_ref, &thisEffect_refH);
	StreamSuite->AEGP_GetNewEffectStreamByIndex(PlugId, thisEffect_refH, param_index, &effect_streamH);
    StreamSuite->AEGP_GetNewStreamValue(PlugId, effect_streamH, AEGP_LTimeMode_LayerTime, &currT, FALSE, sample_valP);
    val.val.arbH = ArbH;
	StreamSuite->AEGP_SetStreamValue(PlugId, effect_streamH, sample_valP);
    StreamSuite->AEGP_GetNewStreamValue(PlugId, effect_streamH, AEGP_LTimeMode_LayerTime, &currT, FALSE, sample_valP);
    
    if (sample_valP) {
        ERR2(StreamSuite->AEGP_DisposeStreamValue(sample_valP));
    }

	if (effect_streamH) {
		ERR2(StreamSuite->AEGP_DisposeStream(effect_streamH));
	}

	if (thisEffect_refH) {
		ERR2(EffectSuite->AEGP_DisposeEffect(thisEffect_refH));
	}

	return err;
}

//fast find/replace all method
void strReplace(std::string& str,
		const std::string& oldStr,
		const std::string& newStr)
{
	std::string::size_type pos = 0u;
	while ((pos = str.find(oldStr, pos)) != std::string::npos) {
		str.replace(pos, oldStr.length(), newStr);
		pos += newStr.length();
	}
}

static PF_Boolean 
hasString(std::string& str, std::string& expr)
{
	std::string::size_type pos = 0u;
	if ((pos = str.find(expr, pos)) != std::string::npos) {
		return true;
	}
	else
	{
		return false;
	}

}


//math parser's functions
static PF_FpShort
inline parseDrawRect(PF_FpShort xL, PF_FpShort yL, PF_FpShort center_x, PF_FpShort center_y, PF_FpShort lx,PF_FpShort ly)
{

	if (xL > (center_x - lx) &&
		xL <(center_x + lx) &&
		yL >(center_y - ly) &&
		yL < (center_y + ly))
	{
		return 1;
	}

	else{
		return 0;
	}
}

static PF_Err
PopDialog(
	PF_InData		*in_data,
	PF_OutData		*out_data,
	PF_ParamDef		*params[],
	PF_LayerDef		*output)
{
	PF_Err err = PF_Err_NONE;
	AEGP_SuiteHandler	suites(in_data->pica_basicP);
	my_global_dataP		globP = reinterpret_cast<my_global_dataP>(DH(out_data->global_data));

	AEGP_MemHandle     resultMemH = NULL;
	A_char *resultAC = NULL;
	A_char          scriptAC[12288] = { '\0' };
	std::string Majvers = std::to_string(MAJOR_VERSION);
	std::string MinVers = std::to_string(MINOR_VERSION);
    std::string Bugvers = std::to_string(BUG_VERSION);
    
    //ARB
    PF_ParamDef arb_param;
    
    //strings to send expr to script
    std::string tempRedS ="'";
    std::string tempGreenS ="'";
    std::string tempBlueS ="'";
    std::string tempAlphaS ="'";
    std::string tempName ="'";
    std::string tempDescription ="'";

	PF_Handle		arbOutH = NULL;
	m_ArbData		*arbInP = NULL;
    m_ArbData		*arbOutP= NULL;

    
    AEFX_CLR_STRUCT(arb_param);
    ERR(PF_CHECKOUT_PARAM(	in_data,
                          MATH_ARB_DATA,
                          in_data->current_time,
                          in_data->time_step,
                          in_data->time_scale, 
                          &arb_param));

    if (!err){
        AEFX_CLR_STRUCT(arbInP);
        arbInP = reinterpret_cast<m_ArbData*>(*arb_param.u.arb_d.value);
        if (arbInP){
            tempRedS.append(arbInP->redExAcFlat);
            tempGreenS.append(arbInP->greenExAcFlat);
            tempBlueS.append(arbInP->blueExAcFlat);
            tempAlphaS.append(arbInP->alphaExAcFlat);
            tempName.append(arbInP->presetNameAcFlat);
            tempDescription.append(arbInP->descriptionAcFlat);
        }
       
    }

    tempRedS.append("'");
    tempGreenS.append("'");
    tempBlueS.append("'");
    tempAlphaS.append("'");
    tempName.append("'");
    tempDescription.append("'");

	//to force the parser to keep \n before to send it to js
	strReplace(tempRedS, "\n", "\\n");
	strReplace(tempGreenS, "\n", "\\n");
	strReplace(tempBlueS, "\n", "\\n");
	strReplace(tempAlphaS, "\n", "\\n");
    strReplace(tempDescription, "\n", "\\n");
    sprintf( scriptAC,
            script_ae.c_str(),
            tempRedS.c_str(),
            tempGreenS.c_str(),
            tempBlueS.c_str() ,
            tempAlphaS.c_str(),
            tempName.c_str(),
            tempDescription.c_str(),
            Majvers.c_str(),
            MinVers .c_str(),
            Bugvers.c_str());
    
    ERR(suites.UtilitySuite6()->AEGP_ExecuteScript(globP->my_id, scriptAC, FALSE, &resultMemH, NULL));
    //AEGP SETSTREAMVALUR TO ARB
    AEFX_CLR_STRUCT(resultAC);
    ERR(suites.MemorySuite1()->AEGP_LockMemHandle(resultMemH, reinterpret_cast<void**>(&resultAC)));

    if  (resultAC){
        AEFX_CLR_STRUCT(arbOutP);
        arbOutP = reinterpret_cast<m_ArbData*>(*arb_param.u.arb_d.value);
        
        //set result per channel
        std::string resultStr = resultAC;
        
        //adapt exprtk language to AE expression's language.
        strReplace(resultStr, "&&",  "&");
        strReplace(resultStr, "||",  "|");
        strReplace(resultStr, "++",  "+=1");
        strReplace(resultStr, "--",  "-=1");
        strReplace(resultStr, " = ", " := ");
        
        nlohmann::json  j = nlohmann::json::parse(resultStr);

        std::string redResultStr =   j["/redExpr"_json_pointer];
        std::string greenResultStr = j["/greenExpr"_json_pointer];
        std::string blueResultStr =  j["/blueExpr"_json_pointer];
        std::string alphaResultStr = j["/alphaExpr"_json_pointer];
        
        
        std::string presetNameStr = j["/presetName"_json_pointer];
        std::string descriptionStr = j["/description"_json_pointer];
        
        presetNameStr.erase(std::remove(presetNameStr.begin(), presetNameStr.end(), '\n'), presetNameStr.end());

        //copy to flat ARB (keeping /n and other speical char from js
        #ifdef AE_OS_WIN
            strncpy_s( arbOutP->redExAcFlat, redResultStr.c_str(), redResultStr.length()+1);
            strncpy_s(arbOutP->greenExAcFlat, greenResultStr.c_str(), greenResultStr.length()+1);
            strncpy_s( arbOutP->blueExAcFlat, blueResultStr.c_str(), blueResultStr.length()+1);
            strncpy_s( arbOutP->alphaExAcFlat, alphaResultStr.c_str(), alphaResultStr.length()+1);
            strncpy_s( arbOutP->presetNameAcFlat, presetNameStr.c_str(), presetNameStr.length()+1);
            strncpy_s( arbOutP->descriptionAcFlat, descriptionStr.c_str(), descriptionStr.length()+1);
        #else
            strncpy( arbOutP->redExAcFlat, redResultStr.c_str(), redResultStr.length()+1);
            strncpy(arbOutP->greenExAcFlat, greenResultStr.c_str(), greenResultStr.length()+1);
            strncpy( arbOutP->blueExAcFlat, blueResultStr.c_str(), blueResultStr.length()+1);
            strncpy( arbOutP->alphaExAcFlat, alphaResultStr.c_str(), alphaResultStr.length()+1);
            strncpy( arbOutP->presetNameAcFlat, presetNameStr.c_str(), presetNameStr.length()+1);
            strncpy( arbOutP->descriptionAcFlat, descriptionStr.c_str(), descriptionStr.length()+1);
        #endif
        
        //delete \nfor execution expr
        redResultStr.erase(std::remove(redResultStr.begin(), redResultStr.end(), '\n'), redResultStr.end());
        greenResultStr.erase(std::remove(greenResultStr.begin(), greenResultStr.end(), '\n'), greenResultStr.end());
        blueResultStr.erase(std::remove(blueResultStr.begin(), blueResultStr.end(), '\n'), blueResultStr.end());
        alphaResultStr.erase(std::remove(alphaResultStr.begin(), alphaResultStr.end(), '\n'), alphaResultStr.end());
        descriptionStr.erase(std::remove(descriptionStr.begin(), descriptionStr.end(), '\n'), descriptionStr.end());
        
        #ifdef AE_OS_WIN
            strncpy_s( arbOutP->redExAc, redResultStr.c_str(), redResultStr.length()+1);
            strncpy_s(arbOutP->greenExAc, greenResultStr.c_str(), greenResultStr.length()+1);
            strncpy_s( arbOutP->blueExAc, blueResultStr.c_str(), blueResultStr.length()+1);
            strncpy_s( arbOutP->alphaExAc, alphaResultStr.c_str(), alphaResultStr.length()+1);
            strncpy_s( arbOutP->presetNameAc, presetNameStr.c_str(), presetNameStr.length()+1);
            strncpy_s( arbOutP->descriptionAc, descriptionStr.c_str(), descriptionStr.length()+1);
        #else
 			strncpy( arbOutP->redExAc, redResultStr.c_str(), redResultStr.length()+1);
            strncpy(arbOutP->greenExAc, greenResultStr.c_str(), greenResultStr.length()+1);
            strncpy( arbOutP->blueExAc, blueResultStr.c_str(), blueResultStr.length()+1);
            strncpy( arbOutP->alphaExAc, alphaResultStr.c_str(), alphaResultStr.length()+1);
            strncpy( arbOutP->presetNameAc, presetNameStr.c_str(), presetNameStr.length()+1);
            strncpy( arbOutP->descriptionAc, descriptionStr.c_str(), descriptionStr.length()+1);
        
        #endif
        arbOutH = reinterpret_cast <PF_Handle>(arbOutP);
        ERR (AEGP_SetParamStreamValue(in_data, out_data, globP->my_id, MATH_ARB_DATA, &arbOutH));
        PF_UNLOCK_HANDLE(arbOutH);
     }

    ERR(PF_CHECKIN_PARAM(in_data, &arb_param));
    ERR(suites.MemorySuite1()->AEGP_FreeMemHandle(resultMemH));
    out_data->out_flags |= PF_OutFlag_DISPLAY_ERROR_MESSAGE |
                            PF_OutFlag_FORCE_RERENDER;
    return err;
}

        static PF_Err
ShiftImage8 (
                     void 		*refcon,
                     A_long 		xL,
                     A_long 		yL,
                     PF_Pixel 	*inP,
                     PF_Pixel 	*outP)
    {
        OffInfo	*oiP		= (OffInfo*)refcon;
        PF_Err				err			= PF_Err_NONE;
        PF_Fixed			new_xFi		= 0,
                        new_yFi		= 0;
            
        // Resample original image at offset point.
            
        new_xFi = PF_Fixed (((A_long) xL<<16) + oiP->x_offFi);
        new_yFi = PF_Fixed (((A_long) yL<<16) + oiP->y_offFi);
            
        ERR(oiP->in_data.utils->subpixel_sample(oiP->in_data.effect_ref,
                                                    new_xFi,
                                                    new_yFi,
                                                    &oiP->samp_pb,
                                                    outP));

        return err;
    }
        
static PF_Err
ShiftImage16 (
                      void 		*refcon,
                      A_long 		xL,
                      A_long 		yL,
                      PF_Pixel16 	*inP,
                      PF_Pixel16 	*outP)
    {
        OffInfo	*oiP		= (OffInfo*)refcon;
        PF_InData			*in_data	= &(oiP->in_data);
        PF_Err				err			= PF_Err_NONE;
        PF_Fixed			new_xFi 		= 0, 
        new_yFi 		= 0;
        
        AEGP_SuiteHandler suites(in_data->pica_basicP);
        
        // Resample original image at offset point.
        
        new_xFi = PF_Fixed (((A_long) xL<<16) + oiP->x_offFi);
        new_yFi = PF_Fixed (((A_long) yL<<16) + oiP->y_offFi);
        
        ERR(suites.Sampling16Suite1()->subpixel_sample16(in_data->effect_ref,
                                                         new_xFi, 
                                                         new_yFi, 
                                                         &oiP->samp_pb,
                                                         outP));
        return err;
    }

PF_Err
LineIteration8Func ( void *refconPV,
					 void *refconFunc,
					 void *refconFlags,
                     A_long yL)
    {
        PF_Err err = PF_Err_NONE;
        MathInfo	*miP	= reinterpret_cast<MathInfo*>(refconPV);
		funcTransfertInfo *fiP = reinterpret_cast<funcTransfertInfo*>(refconFunc);
		FlagsInfoP *flagsP = reinterpret_cast<FlagsInfo*>(refconFlags);
        PF_EffectWorld inW = miP->inW;//cast input buffer here.
        PF_EffectWorld outW = miP->outW;//cast output buffer here.
		PF_EffectWorld extW;
		PF_Pixel8 *bop_extP;
		if (flagsP->PixelsCallExternalInputB){
			extW = miP->extLW;
			//external layer world
			bop_extP = reinterpret_cast<PF_Pixel8*>(extW.data) + (extW.rowbytes* yL / sizeof(PF_Pixel));
		}

        PF_FpShort  red_result, green_result, blue_result, alpha_result;
		PF_Pixel *in00;
		PF_Pixel *in10; 
		PF_Pixel *in20;
		PF_Pixel *in01;
		PF_Pixel *in21; 
		PF_Pixel *in02; 
		PF_Pixel *in12;
		PF_Pixel *in22;

        
        PF_Pixel8 *bop_outP = reinterpret_cast<PF_Pixel8*>(outW.data)+ (outW.rowbytes* yL / sizeof(PF_Pixel)),
        *bop_inP = reinterpret_cast<PF_Pixel8*>(inW.data)+ (inW.rowbytes* yL / sizeof(PF_Pixel));
        

        //3*3 matrix grp
		if (flagsP->NeedsPixelAroundB) {
			in00 = bop_inP - (inW.rowbytes / sizeof(PF_Pixel)) - 1;//top left pixel in 3X3.
			in10 = in00 + 1;//top middle pixel in 3X3.
			in20 = in10 + 1;//top right pixel in 3X3.
			in01 = bop_inP - 1;//mid left pixel in 3X3.
			in21 = bop_inP + 1;//top right pixel in 3X3.
			in02 = bop_inP + (inW.rowbytes / sizeof(PF_Pixel)) - 1;//bottom left pixel in 3X3.
			in12 = in02 + 1;//bottom middle pixel in 3X3.
			in22 = in12 + 1;//bottom right pixel in 3X3.
		}

		AEFX_CLR_STRUCT(miP->yLF);
		miP->yLF = PF_FpShort(yL);

        for ( A_long xL =0; xL < inW.width; xL++) {
			if (flagsP->NeedsPixelAroundB) {
				if (yL - 1 >= 0) {
					miP->m3P_red[0] = PF_FpShort(in00->red) / (PF_FpShort)PF_MAX_CHAN8;
					miP->m3P_green[0] = PF_FpShort(in00->green) / (PF_FpShort)PF_MAX_CHAN8;
					miP->m3P_blue[0] = PF_FpShort(in00->blue) / (PF_FpShort)PF_MAX_CHAN8;
					miP->m3P_alpha[0] = PF_FpShort(in00->alpha) / (PF_FpShort)PF_MAX_CHAN8;

					miP->m3P_red[1] = PF_FpShort(in10->red) / (PF_FpShort)PF_MAX_CHAN8;
					miP->m3P_green[1] = PF_FpShort(in10->green) / (PF_FpShort)PF_MAX_CHAN8;
					miP->m3P_blue[1] = PF_FpShort(in10->blue) / (PF_FpShort)PF_MAX_CHAN8;
					miP->m3P_alpha[1] = PF_FpShort(in10->alpha) / (PF_FpShort)PF_MAX_CHAN8;
					if (xL + 1 <= inW.width) {
						miP->m3P_red[2] = PF_FpShort(in20->red) / (PF_FpShort)PF_MAX_CHAN8;
						miP->m3P_green[2] = PF_FpShort(in20->green) / (PF_FpShort)PF_MAX_CHAN8;
						miP->m3P_blue[2] = PF_FpShort(in20->blue) / (PF_FpShort)PF_MAX_CHAN8;
						miP->m3P_alpha[2] = PF_FpShort(in20->alpha) / (PF_FpShort)PF_MAX_CHAN8;
					}
					else {
						miP->m3P_red[2] = 0;
						miP->m3P_green[2] = 0;
						miP->m3P_blue[2] = 0;
						miP->m3P_alpha[2] = 0;
					}
				}
				miP->m3P_red[3] = PF_FpShort(in01->red) / (PF_FpShort)PF_MAX_CHAN8;
				miP->m3P_green[3] = PF_FpShort(in01->green) / (PF_FpShort)PF_MAX_CHAN8;
				miP->m3P_blue[3] = PF_FpShort(in01->blue) / (PF_FpShort)PF_MAX_CHAN8;
				miP->m3P_alpha[3] = PF_FpShort(in01->alpha) / (PF_FpShort)PF_MAX_CHAN8;

				miP->m3P_red[4] = PF_FpShort(bop_inP->red) / (PF_FpShort)PF_MAX_CHAN8;
				miP->m3P_green[4] = PF_FpShort(bop_inP->green) / (PF_FpShort)PF_MAX_CHAN8;
				miP->m3P_blue[4] = PF_FpShort(bop_inP->blue) / (PF_FpShort)PF_MAX_CHAN8;
				miP->m3P_alpha[4] = PF_FpShort(bop_inP->alpha) / (PF_FpShort)PF_MAX_CHAN8;
				if (xL + 1 <= inW.height) {
					miP->m3P_red[5] = PF_FpShort(in21->red) / (PF_FpShort)PF_MAX_CHAN8;
					miP->m3P_green[5] = PF_FpShort(in21->green) / (PF_FpShort)PF_MAX_CHAN8;
					miP->m3P_blue[5] = PF_FpShort(in21->blue) / (PF_FpShort)PF_MAX_CHAN8;
					miP->m3P_alpha[5] = PF_FpShort(in21->alpha) / (PF_FpShort)PF_MAX_CHAN8;
				}
				else {
					miP->m3P_red[5] = 0;
					miP->m3P_green[5] = 0;
					miP->m3P_blue[5] = 0;
					miP->m3P_alpha[5] = 0;
				}
				if (yL + 1 <= inW.height) {
					miP->m3P_red[6] = PF_FpShort(in02->red) / (PF_FpShort)PF_MAX_CHAN8;
					miP->m3P_green[6] = PF_FpShort(in02->green) / (PF_FpShort)PF_MAX_CHAN8;
					miP->m3P_blue[6] = PF_FpShort(in02->blue) / (PF_FpShort)PF_MAX_CHAN8;
					miP->m3P_alpha[6] = PF_FpShort(in02->alpha) / (PF_FpShort)PF_MAX_CHAN8;

					miP->m3P_red[7] = PF_FpShort(in12->red) / (PF_FpShort)PF_MAX_CHAN8;
					miP->m3P_green[7] = PF_FpShort(in12->green) / (PF_FpShort)PF_MAX_CHAN8;
					miP->m3P_blue[7] = PF_FpShort(in12->blue) / (PF_FpShort)PF_MAX_CHAN8;
					miP->m3P_alpha[7] = PF_FpShort(in12->alpha) / (PF_FpShort)PF_MAX_CHAN8;

					if (xL + 1 <= inW.width) {
						miP->m3P_red[8] = PF_FpShort(in22->red) / (PF_FpShort)PF_MAX_CHAN8;
						miP->m3P_green[8] = PF_FpShort(in22->green) / (PF_FpShort)PF_MAX_CHAN8;
						miP->m3P_blue[8] = PF_FpShort(in22->blue) / (PF_FpShort)PF_MAX_CHAN8;
						miP->m3P_alpha[8] = PF_FpShort(in22->alpha) / (PF_FpShort)PF_MAX_CHAN8;
					}
					else {
						miP->m3P_red[8] = 0;
						miP->m3P_green[8] = 0;
						miP->m3P_blue[8] = 0;
						miP->m3P_alpha[8] = 0;
					}
				}

			}
                AEFX_CLR_STRUCT(miP->xLF);
                miP->xLF = PF_FpShort(xL);                
                AEFX_CLR_STRUCT(miP->inAlphaF);
                miP->inAlphaF = (PF_FpShort)bop_inP->alpha / PF_MAX_CHAN8;

				if (miP->inAlphaF != 0) {
					if (flagsP->NeedsLumaB) {
						AEFX_CLR_STRUCT(miP->luma);
						miP->luma = (0.2126*bop_inP->red + 0.7152*bop_inP->green + 0.0722*bop_inP->blue) / (PF_FpShort)PF_MAX_CHAN8;
					}
					AEFX_CLR_STRUCT(miP->inRedF);
					miP->inRedF = (PF_FpShort)bop_inP->red / PF_MAX_CHAN8;
					AEFX_CLR_STRUCT(miP->inGreenF);
					miP->inGreenF = (PF_FpShort)bop_inP->green / PF_MAX_CHAN8;
					AEFX_CLR_STRUCT(miP->inBlueF);
					miP->inBlueF = (PF_FpShort)bop_inP->blue / PF_MAX_CHAN8;
					if (flagsP->PixelsCallExternalInputB) {
						AEFX_CLR_STRUCT(miP->extL_red);
						miP->extL_red = (PF_FpShort)bop_extP->red / PF_MAX_CHAN8;
						AEFX_CLR_STRUCT(miP->extL_green);
						miP->extL_green = (PF_FpShort)bop_extP->green / PF_MAX_CHAN8;
						AEFX_CLR_STRUCT(miP->extL_blue);
						miP->extL_blue = (PF_FpShort)bop_extP->blue / PF_MAX_CHAN8;
						AEFX_CLR_STRUCT(miP->extL_alpha);
						miP->extL_alpha = (PF_FpShort)bop_extP->alpha / PF_MAX_CHAN8;

					}
					
					AEFX_CLR_STRUCT(red_result);
					red_result = MIN(fiP->redExpr(), 1);
					AEFX_CLR_STRUCT(green_result);
					green_result = MIN(fiP->greenExpr(), 1);
					AEFX_CLR_STRUCT(blue_result);
					blue_result = MIN(fiP->blueExpr(), 1);
					AEFX_CLR_STRUCT(alpha_result);
					alpha_result = MIN(fiP->alphaExpr(), 1);

					bop_outP->alpha = A_u_char(alpha_result *PF_MAX_CHAN8);
					bop_outP->red = A_u_char(red_result *PF_MAX_CHAN8);
					bop_outP->green = A_u_char(green_result *PF_MAX_CHAN8);
					bop_outP->blue = A_u_char(blue_result *PF_MAX_CHAN8);

				}
				else {
					bop_outP->alpha = 0;
					bop_outP->red   = 0;
					bop_outP->green = 0;
					bop_outP->blue  = 0;
				}

                bop_outP++;
                bop_inP++;
                bop_extP++;
				if (flagsP->NeedsPixelAroundB) {
					in00++;
					in10++;
					in20++;
					in01++;
					in21++;
					in02++;
					in12++;
					in22++;
				}
            }
            return err;
        }
PF_Err
	LineIteration16Func(void *refconPV,
		                void *refconFunc,
						void *refconFlags,
                            A_long yL)
{
	PF_Err err = PF_Err_NONE;
	MathInfo	*miP = reinterpret_cast<MathInfo*>(refconPV);
	funcTransfertInfo *fiP = reinterpret_cast<funcTransfertInfo*>(refconFunc);
	FlagsInfoP *flagsP = reinterpret_cast<FlagsInfo*>(refconFlags);
	PF_EffectWorld inW = miP->inW;//cast input buffer here.
	PF_EffectWorld outW = miP->outW;//cast output buffer here.
	PF_EffectWorld extW;
	PF_Pixel16 *bop_extP;
	if (flagsP->PixelsCallExternalInputB) {
		extW = miP->extLW;
		//external layer world
		bop_extP = reinterpret_cast<PF_Pixel16*>(extW.data) + (extW.rowbytes* yL / sizeof(PF_Pixel16));
	}

	PF_FpShort  red_result, green_result, blue_result, alpha_result;
	PF_Pixel16 *in00;
	PF_Pixel16 *in10;
	PF_Pixel16 *in20;
	PF_Pixel16 *in01;
	PF_Pixel16 *in21;
	PF_Pixel16 *in02;
	PF_Pixel16 *in12;
	PF_Pixel16 *in22;


	PF_Pixel16 *bop_outP = reinterpret_cast<PF_Pixel16*>(outW.data) + (outW.rowbytes* yL / sizeof(PF_Pixel16)),
				*bop_inP = reinterpret_cast<PF_Pixel16*>(inW.data) +  (inW.rowbytes* yL / sizeof(PF_Pixel16));
    
    
	//3*3 matrix grp
	if (flagsP->NeedsPixelAroundB) {
		in00 = bop_inP - (inW.rowbytes / sizeof(PF_Pixel16)) - 1;//top left pixel in 3X3.
		in10 = in00 + 1;//top middle pixel in 3X3.
		in20 = in10 + 1;//top right pixel in 3X3.
		in01 = bop_inP - 1;//mid left pixel in 3X3.
		in21 = bop_inP + 1;//top right pixel in 3X3.
		in02 = bop_inP + (inW.rowbytes / sizeof(PF_Pixel16)) - 1;//bottom left pixel in 3X3.
		in12 = in02 + 1;//bottom middle pixel in 3X3.
		in22 = in12 + 1;//bottom right pixel in 3X3.
	}
	AEFX_CLR_STRUCT(miP->yLF);
	miP->yLF = PF_FpShort(yL);

	for (A_long xL = 0; xL < inW.width; xL++) {
		if (flagsP->NeedsPixelAroundB) {
			if (yL - 1 >= 0) {
				miP->m3P_red[0] = PF_FpShort(in00->red) / (PF_FpShort)PF_MAX_CHAN16;
				miP->m3P_green[0] = PF_FpShort(in00->green) / (PF_FpShort)PF_MAX_CHAN16;
				miP->m3P_blue[0] = PF_FpShort(in00->blue) / (PF_FpShort)PF_MAX_CHAN16;
				miP->m3P_alpha[0] = PF_FpShort(in00->alpha) / (PF_FpShort)PF_MAX_CHAN16;

				miP->m3P_red[1] = PF_FpShort(in10->red) / (PF_FpShort)PF_MAX_CHAN16;
				miP->m3P_green[1] = PF_FpShort(in10->green) / (PF_FpShort)PF_MAX_CHAN16;
				miP->m3P_blue[1] = PF_FpShort(in10->blue) / (PF_FpShort)PF_MAX_CHAN16;
				miP->m3P_alpha[1] = PF_FpShort(in10->alpha) / (PF_FpShort)PF_MAX_CHAN16;
				if (xL + 1 <= inW.width) {
					miP->m3P_red[2] = PF_FpShort(in20->red) / (PF_FpShort)PF_MAX_CHAN16;
					miP->m3P_green[2] = PF_FpShort(in20->green) / (PF_FpShort)PF_MAX_CHAN16;
					miP->m3P_blue[2] = PF_FpShort(in20->blue) / (PF_FpShort)PF_MAX_CHAN16;
					miP->m3P_alpha[2] = PF_FpShort(in20->alpha) / (PF_FpShort)PF_MAX_CHAN16;
				}
				else {
					miP->m3P_red[2] = 0;
					miP->m3P_green[2] = 0;
					miP->m3P_blue[2] = 0;
					miP->m3P_alpha[2] = 0;
				}
			}
			miP->m3P_red[3] = PF_FpShort(in01->red) / (PF_FpShort)PF_MAX_CHAN16;
			miP->m3P_green[3] = PF_FpShort(in01->green) / (PF_FpShort)PF_MAX_CHAN16;
			miP->m3P_blue[3] = PF_FpShort(in01->blue) / (PF_FpShort)PF_MAX_CHAN16;
			miP->m3P_alpha[3] = PF_FpShort(in01->alpha) / (PF_FpShort)PF_MAX_CHAN16;

			miP->m3P_red[4] = PF_FpShort(bop_inP->red) / (PF_FpShort)PF_MAX_CHAN16;
			miP->m3P_green[4] = PF_FpShort(bop_inP->green) / (PF_FpShort)PF_MAX_CHAN16;
			miP->m3P_blue[4] = PF_FpShort(bop_inP->blue) / (PF_FpShort)PF_MAX_CHAN16;
			miP->m3P_alpha[4] = PF_FpShort(bop_inP->alpha) / (PF_FpShort)PF_MAX_CHAN16;
			if (xL + 1 <= inW.height) {
				miP->m3P_red[5] = PF_FpShort(in21->red) / (PF_FpShort)PF_MAX_CHAN16;
				miP->m3P_green[5] = PF_FpShort(in21->green) / (PF_FpShort)PF_MAX_CHAN16;
				miP->m3P_blue[5] = PF_FpShort(in21->blue) / (PF_FpShort)PF_MAX_CHAN16;
				miP->m3P_alpha[5] = PF_FpShort(in21->alpha) / (PF_FpShort)PF_MAX_CHAN16;
			}
			else {
				miP->m3P_red[5] = 0;
				miP->m3P_green[5] = 0;
				miP->m3P_blue[5] = 0;
				miP->m3P_alpha[5] = 0;
			}
			if (yL + 1 <= inW.height) {
				miP->m3P_red[6] = PF_FpShort(in02->red) / (PF_FpShort)PF_MAX_CHAN16;
				miP->m3P_green[6] = PF_FpShort(in02->green) / (PF_FpShort)PF_MAX_CHAN16;
				miP->m3P_blue[6] = PF_FpShort(in02->blue) / (PF_FpShort)PF_MAX_CHAN16;
				miP->m3P_alpha[6] = PF_FpShort(in02->alpha) / (PF_FpShort)PF_MAX_CHAN16;

				miP->m3P_red[7] = PF_FpShort(in12->red) / (PF_FpShort)PF_MAX_CHAN16;
				miP->m3P_green[7] = PF_FpShort(in12->green) / (PF_FpShort)PF_MAX_CHAN16;
				miP->m3P_blue[7] = PF_FpShort(in12->blue) / (PF_FpShort)PF_MAX_CHAN16;
				miP->m3P_alpha[7] = PF_FpShort(in12->alpha) / (PF_FpShort)PF_MAX_CHAN16;

				if (xL + 1 <= inW.width) {
					miP->m3P_red[8] = PF_FpShort(in22->red) / (PF_FpShort)PF_MAX_CHAN16;
					miP->m3P_green[8] = PF_FpShort(in22->green) / (PF_FpShort)PF_MAX_CHAN16;
					miP->m3P_blue[8] = PF_FpShort(in22->blue) / (PF_FpShort)PF_MAX_CHAN16;
					miP->m3P_alpha[8] = PF_FpShort(in22->alpha) / (PF_FpShort)PF_MAX_CHAN16;
				}
				else {
					miP->m3P_red[8] = 0;
					miP->m3P_green[8] = 0;
					miP->m3P_blue[8] = 0;
					miP->m3P_alpha[8] = 0;
				}
			}
		}

		AEFX_CLR_STRUCT(miP->xLF);
		miP->xLF = PF_FpShort(xL);
		
		AEFX_CLR_STRUCT(miP->inAlphaF);
		miP->inAlphaF = (PF_FpShort)bop_inP->alpha / PF_MAX_CHAN16;


		if (miP->inAlphaF != 0) {
			if (flagsP->NeedsLumaB) {
				AEFX_CLR_STRUCT(miP->luma);
				miP->luma = (0.2126*bop_inP->red + 0.7152*bop_inP->green + 0.0722*bop_inP->blue) / (PF_FpShort)PF_MAX_CHAN16;
			}

			AEFX_CLR_STRUCT(miP->inRedF);
			miP->inRedF = (PF_FpShort)bop_inP->red / PF_MAX_CHAN16;
			AEFX_CLR_STRUCT(miP->inGreenF);
			miP->inGreenF = (PF_FpShort)bop_inP->green / PF_MAX_CHAN16;
			AEFX_CLR_STRUCT(miP->inBlueF);
			miP->inBlueF = (PF_FpShort)bop_inP->blue / PF_MAX_CHAN16;
			if (flagsP->PixelsCallExternalInputB) {
				AEFX_CLR_STRUCT(miP->extL_alpha);
				miP->extL_alpha = (PF_FpShort)bop_extP->alpha / PF_MAX_CHAN16;
				AEFX_CLR_STRUCT(miP->extL_red);
				miP->extL_red = (PF_FpShort)bop_extP->red / PF_MAX_CHAN16;
				AEFX_CLR_STRUCT(miP->extL_green);
				miP->extL_green = (PF_FpShort)bop_extP->green / PF_MAX_CHAN16;
				AEFX_CLR_STRUCT(miP->extL_blue);
				miP->extL_blue = (PF_FpShort)bop_extP->blue / PF_MAX_CHAN16;
			}
			AEFX_CLR_STRUCT(red_result);
			red_result = MIN(fiP->redExpr(), 1);
			AEFX_CLR_STRUCT(green_result);
			green_result = MIN(fiP->greenExpr(), 1);
			AEFX_CLR_STRUCT(blue_result);
			blue_result = MIN(fiP->blueExpr(), 1);
			AEFX_CLR_STRUCT(alpha_result);
			alpha_result = MIN(fiP->alphaExpr(), 1);

			bop_outP->alpha = A_u_short(alpha_result *PF_MAX_CHAN16);
			bop_outP->red = A_u_short(red_result *PF_MAX_CHAN16);
			bop_outP->green = A_u_short(green_result *PF_MAX_CHAN16);
			bop_outP->blue = A_u_short(blue_result *PF_MAX_CHAN16);

		}
		else {
			bop_outP->alpha = 0;
			bop_outP->red = 0;
			bop_outP->green = 0;
			bop_outP->blue = 0;
		}

		bop_outP++;
		bop_inP++;
        bop_extP++;

		if (flagsP->NeedsPixelAroundB) {
			in00++;
			in10++;
			in20++;
			in01++;
			in21++;
			in02++;
			in12++;
			in22++;
		}
	}
	return err;
}

void threaded_render::render_8(void *refconPV, void *refconFunc, void *refconFlags, A_long thread_idxL, A_long numThreads, A_long numIter, A_long lastNumIter)
{
        curNumIter =numIter;
        if ( thread_idxL == (numThreads-1)){
            curNumIter =lastNumIter;
        }
	
        std::lock_guard<std::mutex> guard(mut);
        for (A_long iterIndex = 0; iterIndex < curNumIter; ++iterIndex)
        {
            A_long yL =  thread_idxL*numIter+iterIndex;
            LineIteration8Func(refconPV, refconFunc, refconFlags, yL);
        }
}

void threaded_render::render_16(void *refconPV, void *refconFunc, void *refconFlags, A_long thread_idxL, A_long numThreads, A_long numIter, A_long lastNumIter)
{
    curNumIter =numIter;
    if ( thread_idxL == (numThreads-1)){
        curNumIter =lastNumIter;
        }
    std::lock_guard<std::mutex> guard(mut);
    for (A_long iterIndex = 0; iterIndex < curNumIter; ++iterIndex)
    {
        A_long yL =  thread_idxL*numIter+iterIndex;
        LineIteration16Func(refconPV,refconFunc, refconFlags, yL);
    }
}

static PF_Err
Render (
                PF_InData		*in_data,
                PF_OutData		*out_data,
                PF_ParamDef		*params[],
                PF_LayerDef		*outputP )
{
	PF_Err				err = PF_Err_NONE, err2= PF_Err_NONE;
	AEGP_SuiteHandler	suites(in_data->pica_basicP);
    PF_EffectWorld		*inputP = &params[0]->u.ld;
    PF_Point			origin;
    
    PF_Handle		arbH			= params[MATH_ARB_DATA]->u.arb_d.value;
    m_ArbData		*arbP			= NULL;
    AEGP_LayerH		layerH;
    AEGP_CompH		compH;
    AEGP_ItemH      itemH;
    OffInfo         oiP;
    AEFX_CLR_STRUCT(oiP);
	funcTransfertInfo fiP;
	AEFX_CLR_STRUCT(fiP);
    MathInfo		miP;
	AEFX_CLR_STRUCT(miP);
	FlagsInfo      flagsP;
	AEFX_CLR_STRUCT(flagsP);
    

    
    PF_ParamDef checkoutLayerOne;
    AEFX_CLR_STRUCT(checkoutLayerOne);
    PF_EffectWorld Externalworld;
    PF_Pixel16 empty16 = {0,0,0,0};
    PF_Pixel empty8 = {0,0,0,0};

	std::string expression_string_red = "1";
	std::string expression_string_green = "1";
	std::string expression_string_blue = "1";
	std::string expression_string_alpha = "1";
	std::string expression_merge;

	if (!err) {
		arbP = reinterpret_cast<m_ArbData*>(suites.HandleSuite1()->host_lock_handle(arbH));
		if (arbP) {
			m_ArbData *tempPointer = reinterpret_cast<m_ArbData*>(arbP);
			expression_string_red = tempPointer->redExAc;
			expression_string_green = tempPointer->greenExAc;
			expression_string_blue = tempPointer->blueExAc;
			expression_string_alpha = tempPointer->alphaExAc;
			expression_merge.append(expression_string_red);
			expression_merge.append (expression_string_green);
			expression_merge.append(expression_string_blue);
			expression_merge.append(expression_string_alpha);
		}
	}
    
	flagsP.PixelsCallExternalInputB = hasString(expression_merge, std::string ("extL"));
	flagsP.PresetHasWideInput = hasString(expression_merge, std::string("extL"));
	flagsP.NeedsPixelAroundB = hasString(expression_merge, std::string("vec3_"));
	flagsP.NeedsLumaB = hasString(expression_merge, std::string("in_luma"));


	if (flagsP.PixelsCallExternalInputB) {
		ERR(PF_CHECKOUT_PARAM(in_data,
			MATH_INP_LAYER_ONE,
			(in_data->current_time + A_long(params[MATH_INP_TOFF_ONE]->u.fs_d.value) * in_data->time_step),
			in_data->time_step,
			in_data->time_scale,
			&checkoutLayerOne));

		Externalworld = *outputP;
		miP.extLW = *outputP;

		if (PF_WORLD_IS_DEEP(outputP)) {
			ERR(suites.FillMatteSuite2()->fill16(in_data->effect_ref,
				&empty16,
				NULL,
				&Externalworld));
		}
		else {
			ERR(suites.FillMatteSuite2()->fill(in_data->effect_ref,
				&empty8,
				NULL,
				&Externalworld));
		}

		if (checkoutLayerOne.u.ld.data) {
			if (PF_Quality_HI == in_data->quality) {
				ERR(suites.WorldTransformSuite1()->copy_hq(in_data->effect_ref,
					&checkoutLayerOne.u.ld,
					&Externalworld,
					&checkoutLayerOne.u.ld.extent_hint,
					&Externalworld.extent_hint));
			}
			else {
				ERR(suites.WorldTransformSuite1()->copy(in_data->effect_ref,
					&checkoutLayerOne.u.ld,
					&Externalworld,
					&checkoutLayerOne.u.ld.extent_hint,
					&Externalworld.extent_hint));
			}

		}
		oiP.x_offFi = ((long)inputP->width << 15) - params[MATH_INP_POFF_ONE]->u.td.x_value;
		oiP.y_offFi = ((long)inputP->height << 15) - params[MATH_INP_POFF_ONE]->u.td.y_value;

		if (oiP.x_offFi != 0 || oiP.y_offFi != 0) {
			oiP.in_data = *in_data;
			oiP.samp_pb.src = inputP;
			origin.h = (A_short)(in_data->output_origin_x);
			origin.v = (A_short)(in_data->output_origin_y);
			if (PF_WORLD_IS_DEEP(outputP)) {
				ERR(suites.Iterate16Suite1()->iterate_origin(in_data,
					0,
					Externalworld.height,
					&Externalworld,
					&Externalworld.extent_hint,
					&origin,
					(void*)(&oiP),
					ShiftImage16,
					&miP.extLW));
			}
			else {
				ERR(suites.Iterate8Suite1()->iterate_origin(in_data,
					0,
					Externalworld.height,
					&Externalworld,
					&Externalworld.extent_hint,
					&origin,
					(void*)(&oiP),
					ShiftImage8,
					&miP.extLW));
			}

		}
		else {
			miP.extLW = Externalworld;
		}

	}
    //LOADING SUITES TO ACCESS COMP PARAMS
	AEFX_SuiteScoper<AEGP_PFInterfaceSuite1> PFInterfaceSuite = AEFX_SuiteScoper<AEGP_PFInterfaceSuite1>(in_data,
		kAEGPPFInterfaceSuite,
		kAEGPPFInterfaceSuiteVersion1,
		out_data);
	AEFX_SuiteScoper<AEGP_LayerSuite8> layerSuite = AEFX_SuiteScoper<AEGP_LayerSuite8>(in_data,
		kAEGPLayerSuite,
		kAEGPLayerSuiteVersion8,
		out_data);
	AEFX_SuiteScoper<AEGP_CompSuite10> compSuite = AEFX_SuiteScoper<AEGP_CompSuite10>(in_data,
		kAEGPCompSuite,
		kAEGPCompSuiteVersion10,
		out_data);

	AEFX_SuiteScoper<AEGP_ItemSuite8> itemSuite = AEFX_SuiteScoper<AEGP_ItemSuite8>(in_data,
		kAEGPItemSuite,
		kAEGPItemSuiteVersion8,
		out_data);

	AEFX_SuiteScoper<AEGP_StreamSuite4> StreamSuite = AEFX_SuiteScoper<AEGP_StreamSuite4>(in_data,
		kAEGPStreamSuite,
		kAEGPStreamSuiteVersion4,
		out_data);

	PFInterfaceSuite->AEGP_GetEffectLayer(in_data->effect_ref, &layerH);
	layerSuite->AEGP_GetLayerParentComp(layerH, &compH);
	compSuite->AEGP_GetItemFromComp(compH, &itemH);
	A_long width, height;
	A_Time currTime;
	AEGP_StreamVal2 strValP, strValSP;
	AEGP_StreamType  strTypeP;
	AEFX_CLR_STRUCT(width);
	AEFX_CLR_STRUCT(height);
	itemSuite->AEGP_GetItemDimensions(itemH, &width, &height);
	miP.compWidthF = PF_FpLong(width);
	miP.compHeightF = PF_FpLong(height);
	AEGP_DownsampleFactor dsp;
	compSuite->AEGP_GetCompDownsampleFactor(compH, &dsp);
	miP.compWidthF *= dsp.xS;
	miP.compHeightF *= dsp.yS;
    PF_FpLong fpsF;
	compSuite->AEGP_GetCompFramerate(compH,&fpsF);
    miP.compFpsF = static_cast<float>( fpsF);

	layerSuite->AEGP_GetLayerCurrentTime(layerH, AEGP_LTimeMode_LayerTime, &currTime);
	StreamSuite->AEGP_GetLayerStreamValue(layerH, AEGP_LayerStream_POSITION, AEGP_LTimeMode_LayerTime, &currTime, NULL, &strValP, &strTypeP);
	miP.layerPos_X = strValP.three_d.x;
	miP.layerPos_Y = strValP.three_d.y;
	miP.layerPos_Z = strValP.three_d.z;
	StreamSuite->AEGP_GetLayerStreamValue(layerH, AEGP_LayerStream_SCALE, AEGP_LTimeMode_LayerTime, &currTime, NULL, &strValSP, &strTypeP);
	miP.layerScale_X= strValSP.three_d.x;
	miP.layerScale_Y = strValSP.three_d.z;
	miP.layerScale_Z = strValSP.three_d.z;
    
	miP.inOneF	= params[MATH_INPONE_VAR]->u.fs_d.value;
    miP.inTwoF	= params[MATH_INPTWO_VAR]->u.fs_d.value;
    miP.inThreeF= params[MATH_INPTHREE_VAR]->u.fs_d.value;
    miP.inFourF	= params[MATH_INPFOUR_VAR]->u.fs_d.value;
   
    //layer size
    miP.scale_x = in_data->downsample_x.num*in_data->pixel_aspect_ratio.num/ (float)in_data->downsample_x.den,
    miP.scale_y = in_data->downsample_y.num*in_data->pixel_aspect_ratio.den/ (float)in_data->downsample_y.den;
    miP.layerWidthF = PF_FpShort (in_data->width*miP.scale_x) ;
    miP.layerHeightF = PF_FpShort(in_data->height* miP.scale_y);
    
    //time params
    miP.layerTime_Sec = PF_FpShort(in_data->current_time)/PF_FpShort(in_data->time_scale);
    miP.layerTime_Frame = PF_FpShort(in_data->current_time/ (float)in_data->time_step);
    miP.layerDuration =PF_FpShort( in_data->total_time / in_data->time_scale);
    
    
    //user param points
    miP.pointOneX = static_cast<PF_FpShort>(round(FIX_2_FLOAT(params[MATH_INP_POINT_ONE]->u.td.x_value)));
    miP.pointOneY =static_cast<PF_FpShort>(round(FIX_2_FLOAT(params[MATH_INP_POINT_ONE]->u.td.y_value)));
    miP.pointTwoX = static_cast<PF_FpShort>(round(FIX_2_FLOAT(params[MATH_INP_POINT_TWO]->u.td.x_value)));
    miP.pointTwoY =static_cast<PF_FpShort>(round(FIX_2_FLOAT(params[MATH_INP_POINT_TWO]->u.td.y_value)));
    
    //user param color
    miP.colorOne[0]=  PF_FpShort (params[MATH_INP_COLOR_ONE]->u.cd.value.red)/ PF_FpShort (PF_MAX_CHAN8);
    miP.colorOne[1]=PF_FpShort (params[MATH_INP_COLOR_ONE]->u.cd.value.green)/ PF_FpShort (PF_MAX_CHAN8);
    miP.colorOne[2] = PF_FpShort (params[MATH_INP_COLOR_ONE]->u.cd.value.blue)/ PF_FpShort (PF_MAX_CHAN8);
    
    miP.colorTwo[0] = PF_FpShort (params[MATH_INP_COLOR_TWO]->u.cd.value.red)/ PF_FpShort (PF_MAX_CHAN8);
    miP.colorTwo[1] = PF_FpShort (params[MATH_INP_COLOR_TWO]->u.cd.value.green)/ PF_FpShort (PF_MAX_CHAN8);
    miP.colorTwo[2] = PF_FpShort (params[MATH_INP_COLOR_TWO]->u.cd.value.blue)/ PF_FpShort (PF_MAX_CHAN8);
	miP.xLF = 0;
	miP.yLF = 0;


    fiP.redExpr = parseExpr<PF_FpShort>((void*)&miP, &fiP, expression_string_red);
    if (fiP.hasErrorB)
    {   fiP.channelErrorstr = "red channel expression";
        suites.ANSICallbacksSuite1()->sprintf(out_data->return_msg,
                                              "Error in %s : %s",
                                              fiP.channelErrorstr.c_str(),
                                              fiP.errorstr.c_str());
    }
    fiP.greenExpr = parseExpr<PF_FpShort>((void*)&miP, &fiP, expression_string_green);
    if (fiP.hasErrorB)
    {   fiP.channelErrorstr = "green channel expression";
        suites.ANSICallbacksSuite1()->sprintf(out_data->return_msg,
                                              "Error in %s : %s",
                                              fiP.channelErrorstr.c_str(),
                                              fiP.errorstr.c_str());
    }
    fiP.blueExpr = parseExpr<PF_FpShort>((void*)&miP, &fiP, expression_string_blue);
    if (fiP.hasErrorB)
    {   fiP.channelErrorstr = "blue channel expression";
        suites.ANSICallbacksSuite1()->sprintf(out_data->return_msg,
                                              "Error in %s : %s",
                                              fiP.channelErrorstr.c_str(),
                                              fiP.errorstr.c_str());
    }
    fiP.alphaExpr = parseExpr<PF_FpShort>((void*)&miP, &fiP, expression_string_alpha);
    if (fiP.hasErrorB)
    {   fiP.channelErrorstr = "alpha channel expression";
        suites.ANSICallbacksSuite1()->sprintf(out_data->return_msg,
                                              "Error in %s : %s",
                                              fiP.channelErrorstr.c_str(),
                                              fiP.errorstr.c_str());
    }

    
    
    
    std::vector<std::thread> workers_thrds;
    A_long part_length, lastPart_length, num_thrd;
	AEFX_CLR_STRUCT(part_length);
	AEFX_CLR_STRUCT(num_thrd);
    AEFX_CLR_STRUCT(lastPart_length);
    miP.inW = *inputP;
    miP.outW = *outputP;
        

    ERR(suites.IterateSuite1()->AEGP_GetNumThreads(&num_thrd));
    part_length = A_long ((outputP->height/(float)num_thrd));
    lastPart_length =  part_length + (outputP->height -  (part_length*num_thrd));
   
    threaded_render* thRenderPtr = new threaded_render();
	if (PF_WORLD_IS_DEEP(outputP)) {
		AEFX_CLR_STRUCT(workers_thrds);
		for (A_long thrd_id = 0; thrd_id < num_thrd; ++thrd_id) {
			workers_thrds.emplace_back(std::thread(&threaded_render::render_16, thRenderPtr, (void*)&miP, (void*)&flagsP, thrd_id,num_thrd, part_length,lastPart_length));
		}
	}
	else {
		AEFX_CLR_STRUCT(workers_thrds);
		for (A_long thrd_id = 0; thrd_id < num_thrd; ++thrd_id) {
			workers_thrds.emplace_back(std::thread(&threaded_render::render_8, thRenderPtr, (void*)&miP, (void*)&flagsP, thrd_id,num_thrd, part_length,lastPart_length));
		}

	}

    for (auto&  t :  workers_thrds){
        t.join();
    }
    delete thRenderPtr;
	outputP = &miP.outW;
    PF_UNLOCK_HANDLE(arbH);
    ERR2( PF_CHECKIN_PARAM(in_data, &checkoutLayerOne));
	return err;
}





static PF_Err
GlobalSetdown(
              PF_InData		*in_data)
{
    AEGP_SuiteHandler	suites(in_data->pica_basicP);
    
    if (in_data->global_data) {
        suites.HandleSuite1()->host_dispose_handle(in_data->global_data);
    }
    
    return PF_Err_NONE;
}


static PF_Err
HandleArbitrary(
                PF_InData			*in_data,
                PF_OutData			*out_data,
                PF_ParamDef			*params[],
                PF_LayerDef			*output,
                PF_ArbParamsExtra	*extra)
{
    PF_Err 	err 	= PF_Err_NONE;
    void 	*srcP	= NULL,
		 	*dstP	= NULL;
    
    switch (extra->which_function) {
            
        case PF_Arbitrary_NEW_FUNC:
            if (extra->u.new_func_params.refconPV != ARB_REFCON) {
                err = PF_Err_INTERNAL_STRUCT_DAMAGED;
            } else {
                err = CreateDefaultArb(	in_data,
                                       out_data,
                                       extra->u.new_func_params.arbPH);
            }
            break;
          
        case PF_Arbitrary_DISPOSE_FUNC:
            if (extra->u.dispose_func_params.refconPV != ARB_REFCON) {
                err = PF_Err_INTERNAL_STRUCT_DAMAGED;
            } else {
                PF_DISPOSE_HANDLE(extra->u.dispose_func_params.arbH);
            }
            break;
            
        case PF_Arbitrary_COPY_FUNC:
            if(extra->u.copy_func_params.refconPV == ARB_REFCON) {
                ERR(CreateDefaultArb(	in_data,
                                     out_data,
                                     extra->u.copy_func_params.dst_arbPH));
                
                ERR(Arb_Copy(in_data,
                             out_data,
                             &extra->u.copy_func_params.src_arbH,
                             extra->u.copy_func_params.dst_arbPH));
            }
            break;
           
        case PF_Arbitrary_FLAT_SIZE_FUNC:
            *(extra->u.flat_size_func_params.flat_data_sizePLu) = sizeof(m_ArbData);
            break;
            
        case PF_Arbitrary_FLATTEN_FUNC:

            if(extra->u.flatten_func_params.buf_sizeLu == sizeof(m_ArbData)){
                srcP = (m_ArbData*)PF_LOCK_HANDLE(extra->u.flatten_func_params.arbH);
                dstP = extra->u.flatten_func_params.flat_dataPV;
                if (srcP){
                    memcpy(dstP,srcP,sizeof(m_ArbData));
                }
                PF_UNLOCK_HANDLE(extra->u.flatten_func_params.arbH);
            }
            break;
            
        case PF_Arbitrary_UNFLATTEN_FUNC:
            if(extra->u.unflatten_func_params.buf_sizeLu == sizeof(m_ArbData)){
                PF_Handle	handle = PF_NEW_HANDLE(sizeof(m_ArbData));
                dstP = (m_ArbData*)PF_LOCK_HANDLE(handle);
                srcP = (void*)extra->u.unflatten_func_params.flat_dataPV;
                if (srcP){
                    memcpy(dstP,srcP,sizeof(m_ArbData));
                }
                *(extra->u.unflatten_func_params.arbPH) = handle;
                PF_UNLOCK_HANDLE(handle);
            }
            break;
            
        case PF_Arbitrary_INTERP_FUNC:
            if(extra->u.interp_func_params.refconPV == ARB_REFCON) {
                ERR(CreateDefaultArb(	in_data,
                                     out_data,
                                     extra->u.interp_func_params.interpPH));
                
                ERR(Arb_Interpolate(	in_data,
                                    out_data,
                                    extra->u.interp_func_params.tF,
                                    &extra->u.interp_func_params.left_arbH,
                                    &extra->u.interp_func_params.right_arbH,
                                    extra->u.interp_func_params.interpPH));
            }
            break;
            
        case PF_Arbitrary_COMPARE_FUNC:
            ERR(Arb_Compare(	in_data,
                            out_data,
                            &extra->u.compare_func_params.a_arbH,
                            &extra->u.compare_func_params.b_arbH,
                            extra->u.compare_func_params.compareP));
            break;
            
             
        case PF_Arbitrary_PRINT_SIZE_FUNC:
            err = PF_Err_UNRECOGNIZED_PARAM_TYPE;
            break;
                case PF_Arbitrary_PRINT_FUNC:
            
            if (extra->u.print_func_params.refconPV == ARB_REFCON) {
                ERR(Arb_Print(in_data,
                              out_data,
                              extra->u.print_func_params.print_flags,
                              extra->u.print_func_params.arbH,
                              extra->u.print_func_params.print_sizeLu,
                              extra->u.print_func_params.print_bufferPC));
            } else {
                err = PF_Err_UNRECOGNIZED_PARAM_TYPE;
            }
            break;
            
        case PF_Arbitrary_SCAN_FUNC:
            if (extra->u.scan_func_params.refconPV == ARB_REFCON) {
                ERR(Arb_Scan(	in_data,
                             out_data,
                             extra->u.scan_func_params.refconPV,
                             extra->u.scan_func_params.bufPC,
                             extra->u.scan_func_params.bytes_to_scanLu,
                             extra->u.scan_func_params.arbPH));
            } else {
                err = PF_Err_UNRECOGNIZED_PARAM_TYPE;
            }
            break;
    }
    return err;
}

static PF_Err
RespondtoAEGP (
               PF_InData		*in_data,
               PF_OutData		*out_data,
               PF_ParamDef		*params[],
               PF_LayerDef		*output,
               void*			extraP)
{
    PF_Err			err = PF_Err_NONE;
    
    AEGP_SuiteHandler suites(in_data->pica_basicP);
    
    suites.ANSICallbacksSuite1()->sprintf(	out_data->return_msg,
                                          "%s",
                                          reinterpret_cast<A_char*>(extraP));
    
    return err;
}



DllExport	
PF_Err 
EntryPointFunc (
	PF_Cmd			cmd,
	PF_InData		*in_data,
	PF_OutData		*out_data,
	PF_ParamDef		*params[],
	PF_LayerDef		*output,
	void			*extra)
{
	PF_Err		err = PF_Err_NONE;
	
	try {
		switch (cmd) {
			case PF_Cmd_ABOUT:

				err = About(in_data,
							out_data,
							params,
							output);
				break;
				
			case PF_Cmd_GLOBAL_SETUP:

				err = GlobalSetup(	in_data,
									out_data,
									params,
									output);
				break;
                
            case PF_Cmd_GLOBAL_SETDOWN:
                err = GlobalSetdown(in_data);
                break;
                
            case PF_Cmd_PARAMS_SETUP:
                
                err = ParamsSetup(	in_data,
                                  out_data,
                                  params,
                                  output);
                break;

            case PF_Cmd_ARBITRARY_CALLBACK:
                err = HandleArbitrary(	in_data,
                                      out_data,
                                      params,
                                      output,
                                      reinterpret_cast<PF_ArbParamsExtra*>(extra));
                break;
            
            case PF_Cmd_COMPLETELY_GENERAL:
                err = RespondtoAEGP(in_data,
                                    out_data,
                                    params,
                                    output,
                                    extra);
                break;
            
                
            case PF_Cmd_DO_DIALOG:
                    err = PopDialog(in_data,out_data,params,output);
                break;
				
			case PF_Cmd_RENDER:

				err = Render(	in_data,
								out_data,
								params,
								output);
				break;
            case PF_Cmd_UPDATE_PARAMS_UI:
                err = UpdateParameterUI(	in_data,
                                        out_data,
                                        params,
                                        output);
                break;


            
		}
	}
	catch(PF_Err &thrown_err){
		err = thrown_err;
	}
	return err;
}

