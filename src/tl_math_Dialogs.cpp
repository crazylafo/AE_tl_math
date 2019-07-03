//
//  tl_math_Dialogs.cpp
//  tlMath
//
//
//
#include "tl_math.h"
#include "script.h"


static PF_Err
GetLayerData(PF_InData     *in_data,
			PF_OutData     *out_data,
				A_long *compId,
				 A_long *layerIndex,
				A_long *effectIndex)
{
	PF_Err				err = PF_Err_NONE, err2 = PF_Err_NONE;
	AEGP_LayerH        layerH;
	AEGP_CompH        compH;
	AEGP_ItemH      itemH;
	AEGP_StreamRefH streamH;
	AEGP_EffectRefH effectH;
	AEGP_StreamRefH parentStreamH;
	my_global_dataP		globP = reinterpret_cast<my_global_dataP>(DH(out_data->global_data));

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

	AEFX_SuiteScoper<AEGP_StreamSuite4> streamSuite = AEFX_SuiteScoper<AEGP_StreamSuite4>(in_data,
		kAEGPStreamSuite,
		kAEGPStreamSuiteVersion4,
		out_data);
	AEFX_SuiteScoper<AEGP_DynamicStreamSuite4> DynamicStreamSuite = AEFX_SuiteScoper<AEGP_DynamicStreamSuite4>(in_data,
		kAEGPDynamicStreamSuite,
		kAEGPDynamicStreamSuiteVersion4,
		out_data);
	AEFX_SuiteScoper<AEGP_EffectSuite4> EffectSuite = AEFX_SuiteScoper<AEGP_EffectSuite4>(in_data,
		kAEGPEffectSuite,
		kAEGPEffectSuiteVersion4,
		out_data);


	ERR(PFInterfaceSuite->AEGP_GetEffectLayer(in_data->effect_ref, &layerH));
	ERR(PFInterfaceSuite->AEGP_GetNewEffectForEffect(globP->my_id, in_data->effect_ref, &effectH));

	ERR(streamSuite->AEGP_GetNewEffectStreamByIndex(globP->my_id, effectH,1, &streamH)); //get access to a stream of the effect

	ERR(DynamicStreamSuite->AEGP_GetNewParentStreamRef(globP->my_id, streamH, &parentStreamH));

	ERR(DynamicStreamSuite->AEGP_GetStreamIndexInParent(parentStreamH, effectIndex));


	ERR2(layerSuite->AEGP_GetLayerParentComp(layerH, &compH));
	ERR2(layerSuite->AEGP_GetLayerIndex(layerH, layerIndex)); //layer index
	ERR2(compSuite->AEGP_GetItemFromComp(compH, &itemH));
	ERR2(itemSuite->AEGP_GetItemID(itemH, compId)); //compId

	if (streamH) (streamSuite->AEGP_DisposeStream(streamH));
	if(effectH) (EffectSuite->AEGP_DisposeEffect(effectH));
	if(parentStreamH) (streamSuite->AEGP_DisposeStream(parentStreamH));

	return err;

}


static void jsonStrToArb (std::string resultStr,
                          m_ArbData    *arbOutP)
{


    //copy to flat ARB (keeping /n and other speical char from js
    #ifdef AE_OS_WIN
	strncpy_s(arbOutP->arbDataAc, resultStr.c_str(), resultStr.length() + 1);
    #else
	strncpy(arbOutP->arbDataAc, resultStr.c_str(), resultStr.length() + 1);
    #endif

}

static PF_Err
AEGP_SetParamStreamValue(PF_InData            *in_data,
                         PF_OutData            *out_data,
                         AEGP_PluginID        PlugId,
                         PF_ParamIndex        param_index,
                         PF_Handle           *ArbH)
{
    PF_Err  err = PF_Err_NONE,
    err2 = PF_Err_NONE;
    AEGP_LayerH        layerH;
    AEGP_StreamRefH effect_streamH = NULL;
    AEGP_EffectRefH   thisEffect_refH;
    AEGP_StreamValue2    val;
    AEGP_StreamValue2    *sample_valP = &val;
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


//fast copy/find/replace all method
std::string strCopyAndReplace(std::string str,
                              const std::string& oldStr,
                              const std::string& newStr)
{
    std::string copyStr =str;
    std::string::size_type pos = 0u;
    while ((pos = copyStr.find(oldStr, pos)) != std::string::npos) {
        copyStr.replace(pos, oldStr.length(), newStr);
        pos += newStr.length();
    }
    return copyStr;

}

//quick find/replace all method
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

void ExprtkCorrectorStr(std::string &str)
{
	//convert some AE javascript operator to exprtk operators
	strReplace(str, "&&", "&");
	strReplace(str, "||", "|");
	strReplace(str, "++", "+=1");
	strReplace(str, "--", "-=1");
	strReplace(str, " = ", " := ");
	strReplace(str, "\t", "    ");
	strReplace(str, "\"", " '");
}

PF_Boolean
strToBoolean( std::string str)
{
    if (str== "1" || str =="true"){
        return true;
    }
    else{
        return false;
    }
}

static std::string
parseLayerDataToJsonStr(A_long compId,
	A_long layerIndex,
	A_long effectIndex)
{
	nlohmann::json  plugIdObjJson;
	plugIdObjJson["compId"] = compId;
	plugIdObjJson["layerIndex"] = (layerIndex + 1); //plugin indexes layers from 0 and script from 1
	plugIdObjJson["effectIndex"] = (effectIndex + 1);


	std::string plugIdObjJsonStr = "'''";
	plugIdObjJsonStr.append(plugIdObjJson.dump());
	plugIdObjJsonStr.append("'''");
	return plugIdObjJsonStr;

}



PF_Err
CallCepDialog(PF_InData        *in_data,
			PF_OutData        *out_data)
{
	AEGP_SuiteHandler    suites(in_data->pica_basicP);
	PF_Err err = PF_Err_NONE;

	AEGP_MemHandle     resultMemH = NULL;
	A_char *resultAC = NULL;
	A_char          scriptAC[1500]{ '\0' };
	my_global_dataP        globP = reinterpret_cast<my_global_dataP>(DH(out_data->global_data));

	A_long compId, layerIndex, effectIndex;
	ERR(GetLayerData(in_data, out_data, &compId, &layerIndex, &effectIndex));
	std::string plugIdObjJsonStr = parseLayerDataToJsonStr(compId, layerIndex, effectIndex);
	//script_callMathCEP_to_Execute.append(script_jsontojs);

	//slit script: first call cep
	AEFX_CLR_STRUCT(scriptAC);
	sprintf(scriptAC,
		 script_callMathCEP.c_str(),
		plugIdObjJsonStr.c_str());
	
	ERR(suites.UtilitySuite6()->AEGP_ExecuteScript(globP->my_id, scriptAC, FALSE, &resultMemH, NULL));
	AEFX_CLR_STRUCT(resultAC);
	ERR(suites.MemorySuite1()->AEGP_LockMemHandle(resultMemH, reinterpret_cast<void**>(&resultAC)));
	ERR(suites.MemorySuite1()->AEGP_FreeMemHandle(resultMemH));

	return err;
}

PF_Err
SetupDialogSend( PF_InData        *in_data,
                  PF_OutData        *out_data,
                  PF_ParamDef        *params[])
{
    PF_Err err = PF_Err_NONE;
    AEGP_SuiteHandler    suites(in_data->pica_basicP);
    my_global_dataP        globP = reinterpret_cast<my_global_dataP>(DH(out_data->global_data));
    seqDataP seqP = reinterpret_cast<seqDataP>(DH(out_data->sequence_data));
    AEGP_MemHandle     resultMemH = NULL;
    A_char *resultAC = NULL;
    A_char          scriptAC[100000] { '\0' };
    std::string Majvers = std::to_string(MAJOR_VERSION);
    std::string MinVers = std::to_string(MINOR_VERSION);
    std::string Bugvers = std::to_string(BUG_VERSION);

    //ARB
    PF_ParamDef arb_param;
    m_ArbData        *arbInP = NULL;
    AEFX_CLR_STRUCT(arb_param);
    ERR(PF_CHECKOUT_PARAM(    in_data,
                          MATH_ARB_DATA,
                          in_data->current_time,
                          in_data->time_step,
                          in_data->time_scale,
                          &arb_param));
        AEFX_CLR_STRUCT(arbInP);
        arbInP = reinterpret_cast<m_ArbData*>(*arb_param.u.arb_d.value);
        if (!arbInP){
            err = PF_Err_OUT_OF_MEMORY;
        }
    auto  arbDataJS = nlohmann::json::parse(arbInP->arbDataAc);

    std::string     fragErr = seqP->Glsl_fragError,
                    vertErr = seqP-> Glsl_VertError,
                    redErr = seqP->redError,
                    greenErr = seqP->greenError,
                    blueErr = seqP->blueError,
                    alphaErr =seqP->alphaError;

    strReplace(  fragErr, "\n","\\n");
    strReplace( vertErr, "\n","\\n");
    strReplace( redErr, "\n","\\n");
    strReplace( greenErr, "\n","\\n");
    strReplace(  blueErr, "\n","\\n");
    strReplace( alphaErr, "\n","\\n");

    arbDataJS["gl_expression"]["gl_frag_error"] =  fragErr;
    arbDataJS["gl_expression"]["gl_vert_error"] =  vertErr;

    arbDataJS["math_expression"]["red_error"] =      redErr;
    arbDataJS["math_expression"]["green_error"] =    greenErr;
    arbDataJS["math_expression"]["blue_error"] =     blueErr;
    arbDataJS["math_expression"]["alpha_error"] =    alphaErr;
    std::string resultStr;
   std::string jsonDump = "'''";
   jsonDump.append(arbDataJS.dump());
   jsonDump.append("'''");


	AEFX_CLR_STRUCT(scriptAC);
        sprintf(scriptAC,
		script_sendToMathCEP.c_str(),
		jsonDump.c_str());
	ERR(suites.UtilitySuite6()->AEGP_ExecuteScript(globP->my_id, scriptAC, FALSE, &resultMemH, NULL));
	AEFX_CLR_STRUCT(resultAC);
	ERR(suites.MemorySuite1()->AEGP_LockMemHandle(resultMemH, reinterpret_cast<void**>(&resultAC)));
	ERR(suites.MemorySuite1()->AEGP_FreeMemHandle(resultMemH));
	return err;
}

PF_Err
SetupGetDataBack(
                 PF_InData        *in_data,
                 PF_OutData        *out_data,
                 PF_ParamDef        *params[])
{
    PF_Err err = PF_Err_NONE;
     AEGP_SuiteHandler    suites(in_data->pica_basicP);
    my_global_dataP globP = reinterpret_cast<my_global_dataP>(DH(out_data->global_data));
    PF_Handle        arbOutH = NULL;
    PF_ParamDef arb_param;
	AEGP_MemHandle     resultMemH = NULL;
	A_char *resultAC = NULL;
	std::string resultStr;
    m_ArbData *arbOutP= NULL;

	AEFX_CLR_STRUCT(arb_param);
	ERR(PF_CHECKOUT_PARAM(in_data,
		MATH_ARB_DATA,
		in_data->current_time,
		in_data->time_step,
		in_data->time_scale,
		&arb_param));
	arbOutP = reinterpret_cast<m_ArbData*>(*arb_param.u.arb_d.value);
	ERR(suites.UtilitySuite6()->AEGP_ExecuteScript(globP->my_id, script_getDataBackFromMathCEP.c_str(), FALSE, &resultMemH, NULL));
	AEFX_CLR_STRUCT(resultAC);
	ERR(suites.MemorySuite1()->AEGP_LockMemHandle(resultMemH, reinterpret_cast<void**>(&resultAC)));
    
    if (resultAC){  
        resultStr = resultAC;
        jsonStrToArb(resultStr, arbOutP);
    }
	ERR(suites.MemorySuite1()->AEGP_FreeMemHandle(resultMemH));
	arbOutH = reinterpret_cast <PF_Handle>(arbOutP);
    //AEGP SETSTREAMVALUE TO ARB
    ERR (AEGP_SetParamStreamValue(in_data, out_data, globP->my_id, MATH_ARB_DATA, &arbOutH));

	PF_Handle    seq_dataH = suites.HandleSuite1()->host_new_handle(sizeof(seqData));

	if (seq_dataH) {
		seqData  	*seqP = reinterpret_cast<seqData*>(suites.HandleSuite1()->host_lock_handle(seq_dataH));
		ERR(copyFromArbToSeqData(resultStr, seqP));
        ERR(evalScripts  (seqP));
		out_data->sequence_data = seq_dataH;
		suites.HandleSuite1()->host_unlock_handle(seq_dataH);

	}
	else {    // whoa, we couldn't allocate sequence data; bail!
		err = PF_Err_OUT_OF_MEMORY;
	}
    PF_UNLOCK_HANDLE(arbOutH);
    return err;

}

PF_Err
copyFromArbToSeqData( std::string       arbStr,
                      seqData    *seqDataP)
{
    PF_Err err = PF_Err_NONE;
    nlohmann::json  arbDataJS = nlohmann::json::parse(arbStr);
	std::string effect_name = arbDataJS["/effectInfo/effectName"_json_pointer];
	std::string effect_pluginVersion = arbDataJS["/effectInfo/pluginVesion"_json_pointer];
	std::string effect_minimalPluginversion = arbDataJS["/effectInfo/minimalPluginVersion"_json_pointer];
	//std::string effect_tags=(arbDataJS["/effectInfo/tags"_json_pointer]);
	std::string effect_presetName=(arbDataJS["/effectInfo/presetName"_json_pointer]);
	std::string effect_description=(arbDataJS["/effectInfo/description"_json_pointer]);
		
    bool mode_glB		 = arbDataJS["/effectMode/gl_modeB"_json_pointer];
	bool mode_exprB	 = arbDataJS["/effectMode/expr_modeB"_json_pointer];
	bool mode_evalB	 = arbDataJS["/effectMode/evalModeB"_json_pointer];
	
	std::string gl_fragsh =  (arbDataJS["/gl_expression/gl_frag_sh"_json_pointer]);
	std::string gl_vertsh = (arbDataJS["/gl_expression/gl_vert_sh"_json_pointer]);
					
	std::string expr_red = (arbDataJS["/math_expression/redExpr"_json_pointer]);
	std::string expr_green = (arbDataJS["/math_expression/greenExpr"_json_pointer]);
	std::string expr_blue= (arbDataJS["/math_expression/blueExpr"_json_pointer]);
	std::string expr_alpha = (arbDataJS["/math_expression/alphaExpr"_json_pointer]);



    strReplace( gl_fragsh, "\\n","\n");
    strReplace( gl_vertsh, "\\n","\n");
    strReplace( expr_red, "\\n","\n");
    strReplace( expr_green, "\\n","\n");
    strReplace( expr_blue, "\\n","\n");
    strReplace( expr_alpha, "\\n","\n");


	bool  param_pixelAroundB= (arbDataJS["/flags/needsPixelAroundB"_json_pointer]);
	bool param_ExternalInputB = (arbDataJS["/flags/pixelsCallExternalInputB"_json_pointer]);
	bool param_lumaB = (arbDataJS["/flags/needsLumaB"_json_pointer]);
	bool paramWideeInputB= (arbDataJS["/flags/presetHasWideInputB"_json_pointer]);

	std::string setting_resolutionName = (arbDataJS["/composition/resolution"_json_pointer]);
	std::string setting_timeSecName= (arbDataJS["/composition/time_sec"_json_pointer]);
	std::string setting_timeFrameName= (arbDataJS["/composition/time_frame"_json_pointer]);
	std::string setting_frameRateName= (arbDataJS["/composition/frame_rate"_json_pointer]);

	bool slider_grpVisibleB = (arbDataJS["/gui_settings/sliderGrp/grpVisibleB"_json_pointer]);
	std::string slider_grpName = (arbDataJS["/gui_settings/sliderGrp/grpName"_json_pointer]);
	bool slider_01VisibleB =	(arbDataJS["/gui_settings/sliderGrp/slider_1/visibleB"_json_pointer]);
	std::string slider_01Name = (arbDataJS["/gui_settings/sliderGrp/slider_1/name"_json_pointer]);
	bool slider_02VisibleB = (arbDataJS["/gui_settings/sliderGrp/slider_2/visibleB"_json_pointer]);
	std::string slider_02Name = (arbDataJS["/gui_settings/sliderGrp/slider_2/name"_json_pointer]);
	bool slider_03VisibleB = (arbDataJS["/gui_settings/sliderGrp/slider_3/visibleB"_json_pointer]);
	std::string slider_03Name = (arbDataJS["/gui_settings/sliderGrp/slider_3/name"_json_pointer]);
	bool slider_04VisibleB =(arbDataJS["/gui_settings/sliderGrp/slider_4/visibleB"_json_pointer]);
	std::string slider_04Name = (arbDataJS["/gui_settings/sliderGrp/slider_4/name"_json_pointer]);
	bool slider_05VisibleB = (arbDataJS["/gui_settings/sliderGrp/slider_5/visibleB"_json_pointer]);
	std::string slider_05Name = (arbDataJS["/gui_settings/sliderGrp/slider_5/name"_json_pointer]);
	bool slider_06VisibleB = (arbDataJS["/gui_settings/sliderGrp/slider_6/visibleB"_json_pointer]);
	std::string slider_06Name = (arbDataJS["/gui_settings/sliderGrp/slider_6/name"_json_pointer]);
	bool slider_07VisibleB = (arbDataJS["/gui_settings/sliderGrp/slider_7/visibleB"_json_pointer]);
	std::string slider_07Name = (arbDataJS["/gui_settings/sliderGrp/slider_7/name"_json_pointer]);
	bool slider_08VisibleB = (arbDataJS["/gui_settings/sliderGrp/slider_8/visibleB"_json_pointer]);
	std::string slider_08Name = (arbDataJS["/gui_settings/sliderGrp/slider_8/name"_json_pointer]);
	bool slider_09VisibleB = (arbDataJS["/gui_settings/sliderGrp/slider_9/visibleB"_json_pointer]);
	std::string slider_09Name=(arbDataJS["/gui_settings/sliderGrp/slider_9/name"_json_pointer]);
	bool slider_10VisibleB = (arbDataJS["/gui_settings/sliderGrp/slider_10/visibleB"_json_pointer]);
	std::string slider_10Name =(arbDataJS["/gui_settings/sliderGrp/slider_10/name"_json_pointer]);

	bool point_grpVisibleB = (arbDataJS["/gui_settings/pointGrp/grpVisibleB"_json_pointer]);
	std::string point_grpName = (arbDataJS["/gui_settings/pointGrp/grpName"_json_pointer]);
	bool point_01VisibleB = (arbDataJS["/gui_settings/pointGrp/point_1/visibleB"_json_pointer]);
	std::string point_01Name = (arbDataJS["/gui_settings/pointGrp/point_1/name"_json_pointer]);
	bool point_02VisibleB = (arbDataJS["/gui_settings/pointGrp/point_2/visibleB"_json_pointer]);
	std::string point_02Name = (arbDataJS["/gui_settings/pointGrp/point_2/name"_json_pointer]);
	bool point_03VisibleB = (arbDataJS["/gui_settings/pointGrp/point_3/visibleB"_json_pointer]);
	std::string point_03Name = (arbDataJS["/gui_settings/pointGrp/point_3/name"_json_pointer]);
	bool point_04VisibleB = (arbDataJS["/gui_settings/pointGrp/point_4/visibleB"_json_pointer]);
	std::string point_04Name = (arbDataJS["/gui_settings/pointGrp/point_4/name"_json_pointer]);
	bool point_05VisibleB = (arbDataJS["/gui_settings/pointGrp/point_5/visibleB"_json_pointer]);
	std::string point_05Name = (arbDataJS["/gui_settings/pointGrp/point_5/name"_json_pointer]);
	bool point_06VisibleB = (arbDataJS["/gui_settings/pointGrp/point_6/visibleB"_json_pointer]);
	std::string point_06Name = (arbDataJS["/gui_settings/pointGrp/point_6/name"_json_pointer]);
	bool point_07VisibleB = (arbDataJS["/gui_settings/pointGrp/point_7/visibleB"_json_pointer]);
	std::string point_07Name = (arbDataJS["/gui_settings/pointGrp/point_7/name"_json_pointer]);
	bool point_08VisibleB = (arbDataJS["/gui_settings/pointGrp/point_8/visibleB"_json_pointer]);
	std::string point_08Name = (arbDataJS["/gui_settings/pointGrp/point_8/name"_json_pointer]);
	bool point_09VisibleB = (arbDataJS["/gui_settings/pointGrp/point_9/visibleB"_json_pointer]);
	std::string point_09Name = (arbDataJS["/gui_settings/pointGrp/point_9/name"_json_pointer]);
	bool point_10VisibleB = (arbDataJS["/gui_settings/pointGrp/point_10/visibleB"_json_pointer]);
	std::string point_10Name = (arbDataJS["/gui_settings/pointGrp/point_10/name"_json_pointer]);



	bool cbox_grpVisibleB = (arbDataJS["/gui_settings/cboxGrp/grpVisibleB"_json_pointer]);
	std::string cbox_grpName = (arbDataJS["/gui_settings/cboxGrp/grpName"_json_pointer]);
	bool cbox_01VisibleB = (arbDataJS["/gui_settings/cboxGrp/cbox_1/visibleB"_json_pointer]);
	std::string cbox_01Name = (arbDataJS["/gui_settings/cboxGrp/cbox_1/name"_json_pointer]);
	bool cbox_02VisibleB = (arbDataJS["/gui_settings/cboxGrp/cbox_2/visibleB"_json_pointer]);
	std::string cbox_02Name = (arbDataJS["/gui_settings/cboxGrp/cbox_2/name"_json_pointer]);
	bool cbox_03VisibleB = (arbDataJS["/gui_settings/cboxGrp/cbox_3/visibleB"_json_pointer]);
	std::string cbox_03Name = (arbDataJS["/gui_settings/cboxGrp/cbox_3/name"_json_pointer]);
	bool cbox_04VisibleB = (arbDataJS["/gui_settings/cboxGrp/cbox_4/visibleB"_json_pointer]);
	std::string cbox_04Name = (arbDataJS["/gui_settings/cboxGrp/cbox_4/name"_json_pointer]);
	bool cbox_05VisibleB = (arbDataJS["/gui_settings/cboxGrp/cbox_5/visibleB"_json_pointer]);
	std::string cbox_05Name = (arbDataJS["/gui_settings/cboxGrp/cbox_5/name"_json_pointer]);
	bool cbox_06VisibleB = (arbDataJS["/gui_settings/cboxGrp/cbox_6/visibleB"_json_pointer]);
	std::string cbox_06Name = (arbDataJS["/gui_settings/cboxGrp/cbox_6/name"_json_pointer]);
	bool cbox_07VisibleB = (arbDataJS["/gui_settings/cboxGrp/cbox_7/visibleB"_json_pointer]);
	std::string cbox_07Name = (arbDataJS["/gui_settings/cboxGrp/cbox_7/name"_json_pointer]);
	bool cbox_08VisibleB = (arbDataJS["/gui_settings/cboxGrp/cbox_8/visibleB"_json_pointer]);
	std::string cbox_08Name = (arbDataJS["/gui_settings/cboxGrp/cbox_8/name"_json_pointer]);
	bool cbox_09VisibleB = (arbDataJS["/gui_settings/cboxGrp/cbox_9/visibleB"_json_pointer]);
	std::string cbox_09Name = (arbDataJS["/gui_settings/cboxGrp/cbox_9/name"_json_pointer]);
	bool cbox_10VisibleB = (arbDataJS["/gui_settings/cboxGrp/cbox_10/visibleB"_json_pointer]);
	std::string cbox_10Name = (arbDataJS["/gui_settings/cboxGrp/cbox_10/name"_json_pointer]);


	bool color_grpVisibleB = (arbDataJS["/gui_settings/colorGrp/grpVisibleB"_json_pointer]);
	std::string color_grpName = (arbDataJS["/gui_settings/colorGrp/grpName"_json_pointer]);
	bool color_01VisibleB = (arbDataJS["/gui_settings/colorGrp/color_1/visibleB"_json_pointer]);
	std::string color_01Name = (arbDataJS["/gui_settings/colorGrp/color_1/name"_json_pointer]);
	bool color_02VisibleB = (arbDataJS["/gui_settings/colorGrp/color_2/visibleB"_json_pointer]);
	std::string color_02Name = (arbDataJS["/gui_settings/colorGrp/color_2/name"_json_pointer]);
	bool color_03VisibleB = (arbDataJS["/gui_settings/colorGrp/color_3/visibleB"_json_pointer]);
	std::string color_03Name = (arbDataJS["/gui_settings/colorGrp/color_3/name"_json_pointer]);
	bool color_04VisibleB = (arbDataJS["/gui_settings/colorGrp/color_4/visibleB"_json_pointer]);
	std::string color_04Name = (arbDataJS["/gui_settings/colorGrp/color_4/name"_json_pointer]);
	bool color_05VisibleB = (arbDataJS["/gui_settings/colorGrp/color_5/visibleB"_json_pointer]);
	std::string color_05Name = (arbDataJS["/gui_settings/colorGrp/color_5/name"_json_pointer]);
	bool color_06VisibleB = (arbDataJS["/gui_settings/colorGrp/color_6/visibleB"_json_pointer]);
	std::string color_06Name = (arbDataJS["/gui_settings/colorGrp/color_6/name"_json_pointer]);
	bool color_07VisibleB = (arbDataJS["/gui_settings/colorGrp/color_7/visibleB"_json_pointer]);
	std::string color_07Name = (arbDataJS["/gui_settings/colorGrp/color_7/name"_json_pointer]);
	bool color_08VisibleB = (arbDataJS["/gui_settings/colorGrp/color_8/visibleB"_json_pointer]);
	std::string color_08Name = (arbDataJS["/gui_settings/colorGrp/color_8/name"_json_pointer]);
	bool color_09VisibleB = (arbDataJS["/gui_settings/colorGrp/color_9/visibleB"_json_pointer]);
	std::string color_09Name = (arbDataJS["/gui_settings/colorGrp/color_9/name"_json_pointer]);
	bool color_10VisibleB = (arbDataJS["/gui_settings/colorGrp/color_10/visibleB"_json_pointer]);
	std::string color_10Name = (arbDataJS["/gui_settings/colorGrp/color_10/name"_json_pointer]);
    bool layer_grpVisibleB= (arbDataJS["/gui_settings/layerGrp/grpVisibleB"_json_pointer]);
    std::string layerGrpName = (arbDataJS["/gui_settings/layerGrp/grpName"_json_pointer]);
	std::string layer_currLayerName= (arbDataJS["/gui_settings/layerGrp/current_layer/name"_json_pointer]);

	bool layer_01VisibleB = (arbDataJS["/gui_settings/layerGrp/extLayer_1/visibleB"_json_pointer]);
	std::string layer_01Name = (arbDataJS["/gui_settings/layerGrp/extLayer_1/name"_json_pointer]);
	
	std::string curr_fragSh = seqDataP->Glsl_FragmentShAc;
	std::string curr_vertSh = seqDataP->Glsl_VertexShAc;

	if (curr_fragSh.compare(gl_fragsh) != 0 || curr_vertSh.compare(gl_vertsh) != 0 )
	{
		seqDataP->resetShaderB = true;
	}
	else {
		seqDataP->resetShaderB = false;
	}

	//copy str to A_char
#ifdef AE_OS_WIN
	strncpy_s(seqDataP->presetNameAc, effect_presetName.c_str(), effect_presetName.length() + 1);
	strncpy_s(seqDataP->descriptionAc, effect_description.c_str(), effect_description.length() + 1);
	strncpy_s(seqDataP->Glsl_FragmentShAc, gl_fragsh.c_str(), gl_fragsh.length() + 1);
	strncpy_s(seqDataP->Glsl_VertexShAc , gl_vertsh.c_str(), gl_vertsh.length() + 1);
	strncpy_s(seqDataP->redExAc, expr_red.c_str(), expr_red.length() + 1);
	strncpy_s(seqDataP->greenExAc, expr_green.c_str(), expr_green.length() + 1);
	strncpy_s(seqDataP->blueExAc, expr_blue.c_str(), expr_blue.length() + 1);
	strncpy_s(seqDataP->alphaExAc, expr_alpha.c_str(), expr_alpha.length() + 1);

	strncpy_s(seqDataP->resolution,  setting_resolutionName.c_str(), setting_resolutionName.length() + 1);
	strncpy_s(seqDataP->time_sec, setting_timeSecName.c_str(), setting_timeSecName.length() + 1);
	strncpy_s(seqDataP->time_frame, setting_timeFrameName.c_str(), setting_timeFrameName.length() + 1);
	strncpy_s(seqDataP->frame_rate, setting_frameRateName.c_str(), setting_frameRateName.length() + 1);
	strncpy_s(seqDataP->sliderGrpNameAc, slider_grpName.c_str(), slider_grpName.length() + 1);
	strncpy_s(seqDataP->paramSlider01NameAc, slider_01Name.c_str(), slider_01Name.length() + 1);
	strncpy_s(seqDataP->paramSlider02NameAc, slider_02Name.c_str(), slider_02Name.length() + 1);
	strncpy_s(seqDataP->paramSlider03NameAc, slider_03Name.c_str(), slider_03Name.length() + 1);
	strncpy_s(seqDataP->paramSlider04NameAc, slider_04Name.c_str(), slider_04Name.length() + 1);
	strncpy_s(seqDataP->paramSlider05NameAc, slider_05Name.c_str(), slider_05Name.length() + 1);
	strncpy_s(seqDataP->paramSlider06NameAc, slider_06Name.c_str(), slider_06Name.length() + 1);
	strncpy_s(seqDataP->paramSlider07NameAc, slider_07Name.c_str(), slider_07Name.length() + 1);
	strncpy_s(seqDataP->paramSlider08NameAc, slider_08Name.c_str(), slider_08Name.length() + 1);
	strncpy_s(seqDataP->paramSlider09NameAc, slider_09Name.c_str(), slider_09Name.length() + 1);
	strncpy_s(seqDataP->paramSlider10NameAc, slider_10Name.c_str(), slider_10Name.length() + 1);
	strncpy_s(seqDataP->pointGrpNameAc, point_grpName.c_str(), point_grpName.length() + 1);
	strncpy_s(seqDataP->paramPoint01NameAc, point_01Name.c_str(), point_01Name.length() + 1);
	strncpy_s(seqDataP->paramPoint02NameAc, point_02Name.c_str(), point_02Name.length() + 1);
	strncpy_s(seqDataP->paramPoint03NameAc, point_03Name.c_str(), point_03Name.length() + 1);
	strncpy_s(seqDataP->paramPoint04NameAc, point_04Name.c_str(), point_04Name.length() + 1);
	strncpy_s(seqDataP->paramPoint05NameAc, point_05Name.c_str(), point_05Name.length() + 1);
	strncpy_s(seqDataP->paramPoint06NameAc, point_06Name.c_str(), point_06Name.length() + 1);
	strncpy_s(seqDataP->paramPoint07NameAc, point_07Name.c_str(), point_07Name.length() + 1);
	strncpy_s(seqDataP->paramPoint08NameAc, point_08Name.c_str(), point_08Name.length() + 1);
	strncpy_s(seqDataP->paramPoint09NameAc, point_09Name.c_str(), point_09Name.length() + 1);
	strncpy_s(seqDataP->paramPoint10NameAc, point_10Name.c_str(), point_10Name.length() + 1);
	strncpy_s(seqDataP->cbGrpNameAc, cbox_grpName.c_str(), cbox_grpName.length() + 1);
	strncpy_s(seqDataP->paramCb01NameAc, cbox_01Name.c_str(), cbox_01Name.length() + 1);
	strncpy_s(seqDataP->paramCb02NameAc, cbox_02Name.c_str(), cbox_02Name.length() + 1);
	strncpy_s(seqDataP->paramCb03NameAc, cbox_03Name.c_str(), cbox_03Name.length() + 1);
	strncpy_s(seqDataP->paramCb04NameAc, cbox_04Name.c_str(), cbox_04Name.length() + 1);
	strncpy_s(seqDataP->paramCb05NameAc, cbox_05Name.c_str(), cbox_05Name.length() + 1);
	strncpy_s(seqDataP->paramCb06NameAc, cbox_06Name.c_str(), cbox_06Name.length() + 1);
	strncpy_s(seqDataP->paramCb07NameAc, cbox_07Name.c_str(), cbox_07Name.length() + 1);
	strncpy_s(seqDataP->paramCb08NameAc, cbox_08Name.c_str(), cbox_08Name.length() + 1);
	strncpy_s(seqDataP->paramCb09NameAc, cbox_09Name.c_str(), cbox_09Name.length() + 1);
	strncpy_s(seqDataP->paramCb10NameAc, cbox_10Name.c_str(), cbox_10Name.length() + 1);
	strncpy_s(seqDataP->colorGrpNameAc, color_grpName.c_str(), color_grpName.length() + 1);
	strncpy_s(seqDataP->paramColor01NameAc, color_01Name.c_str(), color_01Name.length() + 1);
	strncpy_s(seqDataP->paramColor02NameAc, color_02Name.c_str(), color_02Name.length() + 1);
	strncpy_s(seqDataP->paramColor03NameAc, color_03Name.c_str(), color_03Name.length() + 1);
	strncpy_s(seqDataP->paramColor04NameAc, color_04Name.c_str(), color_04Name.length() + 1);
	strncpy_s(seqDataP->paramColor05NameAc, color_05Name.c_str(), color_05Name.length() + 1);
	strncpy_s(seqDataP->paramColor06NameAc, color_06Name.c_str(), color_06Name.length() + 1);
	strncpy_s(seqDataP->paramColor07NameAc, color_07Name.c_str(), color_07Name.length() + 1);
	strncpy_s(seqDataP->paramColor08NameAc, color_08Name.c_str(), color_08Name.length() + 1);
	strncpy_s(seqDataP->paramColor09NameAc, color_09Name.c_str(), color_09Name.length() + 1);
	strncpy_s(seqDataP->paramColor10NameAc , color_10Name.c_str(), color_10Name.length() + 1);
	strncpy_s(seqDataP->paramLayer00NameAc, layer_currLayerName.c_str(), layer_currLayerName.length() + 1);
	strncpy_s(seqDataP->layerGrpNameAc,layerGrpName.c_str(), layerGrpName.length() + 1);
	strncpy_s(seqDataP->paramLayer01NameAc, layer_01Name.c_str(), layer_01Name.length() + 1);
#else
	strncpy(seqDataP->presetNameAc, effect_presetName.c_str(), effect_presetName.length() + 1);
	strncpy(seqDataP->descriptionAc, effect_description.c_str(), effect_description.length() + 1);
	strncpy(seqDataP->Glsl_FragmentShAc, gl_fragsh.c_str(), gl_fragsh.length() + 1);
	strncpy(seqDataP->Glsl_VertexShAc, gl_vertsh.c_str(), gl_vertsh.length() + 1);
	strncpy(seqDataP->redExAc, expr_red.c_str(), expr_red.length() + 1);
	strncpy(seqDataP->greenExAc, expr_green.c_str(), expr_green.length() + 1);
	strncpy(seqDataP->blueExAc, expr_blue.c_str(), expr_blue.length() + 1);
	strncpy(seqDataP->alphaExAc, expr_alpha.c_str(), expr_alpha.length() + 1);

	strncpy(seqDataP->resolution, setting_resolutionName.c_str(), setting_resolutionName.length() + 1);
	strncpy(seqDataP->time_sec, setting_timeSecName.c_str(), setting_timeSecName.length() + 1);
	strncpy(seqDataP->time_frame, setting_timeFrameName.c_str(), setting_timeFrameName.length() + 1);
	strncpy(seqDataP->frame_rate, setting_frameRateName.c_str(), setting_frameRateName.length() + 1);
	strncpy(seqDataP->sliderGrpNameAc, slider_grpName.c_str(), slider_grpName.length() + 1);
	strncpy(seqDataP->paramSlider01NameAc, slider_01Name.c_str(), slider_01Name.length() + 1);
	strncpy(seqDataP->paramSlider02NameAc, slider_02Name.c_str(), slider_02Name.length() + 1);
	strncpy(seqDataP->paramSlider03NameAc, slider_03Name.c_str(), slider_03Name.length() + 1);
	strncpy(seqDataP->paramSlider04NameAc, slider_04Name.c_str(), slider_04Name.length() + 1);
	strncpy(seqDataP->paramSlider05NameAc, slider_05Name.c_str(), slider_05Name.length() + 1);
	strncpy(seqDataP->paramSlider06NameAc, slider_06Name.c_str(), slider_06Name.length() + 1);
	strncpy(seqDataP->paramSlider07NameAc, slider_07Name.c_str(), slider_07Name.length() + 1);
	strncpy(seqDataP->paramSlider08NameAc, slider_08Name.c_str(), slider_08Name.length() + 1);
	strncpy(seqDataP->paramSlider09NameAc, slider_09Name.c_str(), slider_09Name.length() + 1);
	strncpy(seqDataP->paramSlider10NameAc, slider_10Name.c_str(), slider_10Name.length() + 1);
	strncpy(seqDataP->pointGrpNameAc, point_grpName.c_str(), point_grpName.length() + 1);
	strncpy(seqDataP->paramPoint01NameAc, point_01Name.c_str(), point_01Name.length() + 1);
	strncpy(seqDataP->paramPoint02NameAc, point_02Name.c_str(), point_02Name.length() + 1);
	strncpy(seqDataP->paramPoint03NameAc, point_03Name.c_str(), point_03Name.length() + 1);
	strncpy(seqDataP->paramPoint04NameAc, point_04Name.c_str(), point_04Name.length() + 1);
	strncpy(seqDataP->paramPoint05NameAc, point_05Name.c_str(), point_05Name.length() + 1);
	strncpy(seqDataP->paramPoint06NameAc, point_06Name.c_str(), point_06Name.length() + 1);
	strncpy(seqDataP->paramPoint07NameAc, point_07Name.c_str(), point_07Name.length() + 1);
	strncpy(seqDataP->paramPoint08NameAc, point_08Name.c_str(), point_08Name.length() + 1);
	strncpy(seqDataP->paramPoint09NameAc, point_09Name.c_str(), point_09Name.length() + 1);
	strncpy(seqDataP->paramPoint10NameAc, point_10Name.c_str(), point_10Name.length() + 1);
	strncpy(seqDataP->cbGrpNameAc, cbox_grpName.c_str(), cbox_grpName.length() + 1);
	strncpy(seqDataP->paramCb01NameAc, cbox_01Name.c_str(), cbox_01Name.length() + 1);
	strncpy(seqDataP->paramCb02NameAc, cbox_02Name.c_str(), cbox_02Name.length() + 1);
	strncpy(seqDataP->paramCb03NameAc, cbox_03Name.c_str(), cbox_03Name.length() + 1);
	strncpy(seqDataP->paramCb04NameAc, cbox_04Name.c_str(), cbox_04Name.length() + 1);
	strncpy(seqDataP->paramCb05NameAc, cbox_05Name.c_str(), cbox_05Name.length() + 1);
	strncpy(seqDataP->paramCb06NameAc, cbox_06Name.c_str(), cbox_06Name.length() + 1);
	strncpy(seqDataP->paramCb07NameAc, cbox_07Name.c_str(), cbox_07Name.length() + 1);
	strncpy(seqDataP->paramCb08NameAc, cbox_08Name.c_str(), cbox_08Name.length() + 1);
	strncpy(seqDataP->paramCb09NameAc, cbox_09Name.c_str(), cbox_09Name.length() + 1);
	strncpy(seqDataP->paramCb10NameAc, cbox_10Name.c_str(), cbox_10Name.length() + 1);
	strncpy(seqDataP->colorGrpNameAc, color_grpName.c_str(), color_grpName.length() + 1);
	strncpy(seqDataP->paramColor01NameAc, color_01Name.c_str(), color_01Name.length() + 1);
	strncpy(seqDataP->paramColor02NameAc, color_02Name.c_str(), color_02Name.length() + 1);
	strncpy(seqDataP->paramColor03NameAc, color_03Name.c_str(), color_03Name.length() + 1);
	strncpy(seqDataP->paramColor04NameAc, color_04Name.c_str(), color_04Name.length() + 1);
	strncpy(seqDataP->paramColor05NameAc, color_05Name.c_str(), color_05Name.length() + 1);
	strncpy(seqDataP->paramColor06NameAc, color_06Name.c_str(), color_06Name.length() + 1);
	strncpy(seqDataP->paramColor07NameAc, color_07Name.c_str(), color_07Name.length() + 1);
	strncpy(seqDataP->paramColor08NameAc, color_08Name.c_str(), color_08Name.length() + 1);
	strncpy(seqDataP->paramColor09NameAc, color_09Name.c_str(), color_09Name.length() + 1);
	strncpy(seqDataP->paramColor10NameAc, color_10Name.c_str(), color_10Name.length() + 1);
	strncpy(seqDataP->layerGrpNameAc, layerGrpName.c_str(), layerGrpName.length() + 1);
	strncpy(seqDataP->paramLayer00NameAc, layer_currLayerName.c_str(), layer_currLayerName.length() + 1);
	strncpy(seqDataP->paramLayer01NameAc, layer_01Name.c_str(), layer_01Name.length() + 1);
#endif


	//copy from arb to seqP
	// copy bool
	seqDataP->glslModeB = mode_glB;
	seqDataP->exprModeB =  mode_exprB;
	seqDataP->evalModeB = mode_evalB;
	seqDataP->needsPixelAroundB =param_pixelAroundB;
	seqDataP->pixelsCallExternalInputB = param_ExternalInputB;
	seqDataP->needsLumaB = param_lumaB;
	seqDataP->presetHasWideInputB = paramWideeInputB;
	seqDataP->sliderGrpVisibleB = slider_grpVisibleB;
	seqDataP->paramSlider01VisibleB = slider_01VisibleB;
	seqDataP->paramSlider02VisibleB = slider_02VisibleB;
	seqDataP->paramSlider03VisibleB = slider_03VisibleB;
	seqDataP->paramSlider04VisibleB = slider_04VisibleB;
	seqDataP->paramSlider05VisibleB = slider_05VisibleB;
	seqDataP->paramSlider06VisibleB = slider_06VisibleB;
	seqDataP->paramSlider07VisibleB = slider_07VisibleB;
	seqDataP->paramSlider08VisibleB = slider_08VisibleB;
	seqDataP->paramSlider09VisibleB = slider_09VisibleB;
	seqDataP->paramSlider10VisibleB = slider_10VisibleB;

	seqDataP->pointGrpVisibleB = point_grpVisibleB;
	seqDataP->paramPoint01VisibleB = point_01VisibleB;
	seqDataP->paramPoint02VisibleB = point_02VisibleB;
	seqDataP->paramPoint03VisibleB = point_03VisibleB;
	seqDataP->paramPoint04VisibleB = point_04VisibleB;
	seqDataP->paramPoint05VisibleB = point_05VisibleB;
	seqDataP->paramPoint06VisibleB = point_06VisibleB;
	seqDataP->paramPoint07VisibleB = point_07VisibleB;
	seqDataP->paramPoint08VisibleB = point_08VisibleB;
	seqDataP->paramPoint09VisibleB = point_09VisibleB;
	seqDataP->paramPoint10VisibleB = point_10VisibleB;

	seqDataP->cbGrpVisibleB = cbox_grpVisibleB;
	seqDataP->paramCb01VisibleB = cbox_01VisibleB;
	seqDataP->paramCb02VisibleB = cbox_02VisibleB;
	seqDataP->paramCb03VisibleB = cbox_03VisibleB;
	seqDataP->paramCb04VisibleB = cbox_04VisibleB;
	seqDataP->paramCb05VisibleB = cbox_05VisibleB;
	seqDataP->paramCb06VisibleB = cbox_06VisibleB;
	seqDataP->paramCb07VisibleB = cbox_07VisibleB;
	seqDataP->paramCb08VisibleB = cbox_08VisibleB;
	seqDataP->paramCb09VisibleB = cbox_09VisibleB;
	seqDataP->paramCb10VisibleB = cbox_10VisibleB;

	seqDataP->colorGrpVisibleB = color_grpVisibleB;
	seqDataP->paramColor01VisibleB = color_01VisibleB;
	seqDataP->paramColor02VisibleB = color_02VisibleB;
	seqDataP->paramColor03VisibleB = color_03VisibleB;
	seqDataP->paramColor04VisibleB = color_04VisibleB;
	seqDataP->paramColor05VisibleB = color_05VisibleB;
	seqDataP->paramColor06VisibleB = color_06VisibleB;
	seqDataP->paramColor07VisibleB = color_07VisibleB;
	seqDataP->paramColor08VisibleB = color_08VisibleB;
	seqDataP->paramColor09VisibleB = color_09VisibleB;
	seqDataP->paramColor10VisibleB = color_10VisibleB;


	seqDataP->layerGrpVisibleB = layer_grpVisibleB;
	seqDataP->paramLayer01VisibleB = layer_01VisibleB;


    return err;
}

PF_Err
evalScripts(seqData  *seqDataP)
{
    PF_Err err = PF_Err_NONE;
    std::string evalRedExpr, evalGreenExpr,evalBlueExpr, evalAlphaExpr, evalVertSh, evalFragSh;

    evalRedExpr = evalMathExprStr (seqDataP->redExAc, &seqDataP);
    if (evalRedExpr != compile_success){
        #ifdef AE_OS_WIN
         strncpy_s(seqDataP->redExAc, safeExpr.c_str(),  safeExpr.length() + 1);
        #else
         strncpy(seqDataP->redExAc, safeExpr.c_str(),  safeExpr.length() + 1);
        #endif

    }

    evalGreenExpr = evalMathExprStr (seqDataP->greenExAc, &seqDataP);
    if (evalGreenExpr != compile_success){
        #ifdef AE_OS_WIN
                strncpy_s(seqDataP->greenExAc, safeExpr.c_str(),  safeExpr.length() + 1);
        #else
                strncpy(seqDataP->greenExAc, safeExpr.c_str(),  safeExpr.length() + 1);
        #endif
    }

    evalBlueExpr = evalMathExprStr (seqDataP->blueExAc, &seqDataP);
    if (evalBlueExpr != compile_success){
        #ifdef AE_OS_WIN
                strncpy_s(seqDataP->blueExAc, safeExpr.c_str(),  safeExpr.length() + 1);
        #else
                strncpy(seqDataP->blueExAc, safeExpr.c_str(),  safeExpr.length() + 1);
        #endif
    }

    evalAlphaExpr = evalMathExprStr (seqDataP->alphaExAc, &seqDataP);
    if ( evalAlphaExpr != compile_success){
        #ifdef AE_OS_WIN
                strncpy_s(seqDataP->alphaExAc, safeExpr.c_str(),  safeExpr.length() + 1);
        #else
                strncpy(seqDataP->alphaExAc, safeExpr.c_str(),  safeExpr.length() + 1);
        #endif
    }

    evalVertShader (seqDataP->Glsl_VertexShAc, evalVertSh);
    if (evalVertSh != compile_success){
        #ifdef AE_OS_WIN
                strncpy_s(seqDataP->Glsl_VertexShAc, glvertstr.c_str(),  glvertstr.length() + 1);
        #else
                strncpy(seqDataP->Glsl_VertexShAc, glvertstr.c_str(),  glvertstr.length() + 1);
        #endif
    }

    evalFragShader (seqDataP->Glsl_FragmentShAc, evalFragSh);
    if (evalFragSh != compile_success){
        std::string setting_resolutionName = "resolution";
        #ifdef AE_OS_WIN
                 strncpy_s(seqDataP->resolution, setting_resolutionName.c_str(),  setting_resolutionName.length() + 1);
                 strncpy_s(seqDataP->Glsl_FragmentShAc, glErrorMessageStr.c_str(),  glErrorMessageStr .length() + 1);
        #else
                strncpy(seqDataP->resolution, setting_resolutionName.c_str(),  setting_resolutionName.length() + 1);
                strncpy(seqDataP->Glsl_FragmentShAc, glErrorMessageStr.c_str(),  glErrorMessageStr .length() + 1);
        #endif

    }
     #ifdef AE_OS_WIN
        strncpy_s(seqDataP->Glsl_fragError , evalFragSh.c_str(),  evalFragSh.length() + 1);
        strncpy_s(seqDataP->Glsl_VertError , evalVertSh.c_str(),   evalVertSh.length() + 1);
        strncpy_s(seqDataP->redError, evalRedExpr.c_str(),  evalRedExpr.length() + 1);
        strncpy_s(seqDataP->greenError, evalGreenExpr.c_str(),  evalGreenExpr.length() + 1);
        strncpy_s(seqDataP->blueError,evalBlueExpr.c_str(), evalBlueExpr.length() + 1);
        strncpy_s(seqDataP->alphaError,evalAlphaExpr.c_str(), evalAlphaExpr.length() + 1);
     #else
        strncpy(seqDataP->redError, evalRedExpr.c_str(),  evalRedExpr.length() + 1);
        strncpy(seqDataP->greenError, evalGreenExpr.c_str(),  evalGreenExpr.length() + 1);
        strncpy(seqDataP->blueError,evalBlueExpr.c_str(), evalBlueExpr.length() + 1);
        strncpy(seqDataP->alphaError,evalAlphaExpr.c_str(), evalAlphaExpr.length() + 1);
        strncpy(seqDataP->Glsl_fragError , evalFragSh.c_str(),  evalFragSh.length() + 1);
        strncpy(seqDataP->Glsl_VertError , evalVertSh.c_str(),   evalVertSh.length() + 1);
     #endif


    return err;
}


