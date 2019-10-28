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
	AEGP_LayerH        layerH = NULL;
	AEGP_CompH        compH = NULL;
	AEGP_ItemH      itemH = NULL;
	AEGP_StreamRefH streamH = NULL;
	AEGP_EffectRefH effectH = NULL;
	AEGP_StreamRefH parentStreamH = NULL;
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
    AEGP_LayerH        layerH = nullptr;
    AEGP_StreamRefH effect_streamH = nullptr;
    AEGP_EffectRefH   thisEffect_refH = nullptr;
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
tlmath::CallCepDialog(PF_InData        *in_data,
			PF_OutData        *out_data)
{
	AEGP_SuiteHandler    suites(in_data->pica_basicP);
	PF_Err err = PF_Err_NONE;

	AEGP_MemHandle     resultMemH = NULL;
	A_char *resultAC = NULL;
	A_char          scriptAC[1500]{ '\0' };
	my_global_dataP        globP = reinterpret_cast<my_global_dataP>(DH(out_data->global_data));

	A_long compId = NULL, layerIndex = NULL, effectIndex = NULL;
	ERR(GetLayerData(in_data, out_data, &compId, &layerIndex, &effectIndex));
    layerIndex +=1; //from AEsdk to AEjs
    effectIndex +=1; //from AEsdk to AEjs

	std::string plugIdObjJsonStr = parseLayerDataToJsonStr(compId, layerIndex, effectIndex);
	//script_callMathCEP_to_Execute.append(script_jsontojs);

	//slit script: first call cep
	AEFX_CLR_STRUCT(scriptAC);
	sprintf(scriptAC,
		 script_callMathCEP.c_str(),
         plugIdObjJsonStr.c_str(),
         compId, layerIndex, effectIndex);
	
	ERR(suites.UtilitySuite6()->AEGP_ExecuteScript(globP->my_id, scriptAC, FALSE, &resultMemH, NULL));
	AEFX_CLR_STRUCT(resultAC);
	ERR(suites.MemorySuite1()->AEGP_LockMemHandle(resultMemH, reinterpret_cast<void**>(&resultAC)));
	ERR(suites.MemorySuite1()->AEGP_FreeMemHandle(resultMemH));

	return err;
}

PF_Err
tlmath::SetupDialogSend( PF_InData        *in_data,
                  PF_OutData        *out_data,
                  PF_ParamDef        *params[])
{
    PF_Err err = PF_Err_NONE;
    AEGP_SuiteHandler    suites(in_data->pica_basicP);
    my_global_dataP        globP = reinterpret_cast<my_global_dataP>(DH(out_data->global_data));
    seqDataP seqP = reinterpret_cast<seqDataP>(DH(out_data->sequence_data));
    AEGP_MemHandle     resultMemH = NULL;
    A_char *resultAC = NULL;
    A_char          scriptAC[120000] { '\0' };
    std::string Majvers = std::to_string(MAJOR_VERSION);
    std::string MinVers = std::to_string(MINOR_VERSION);
    std::string Bugvers = std::to_string(BUG_VERSION);

    std::string plugVersionStr =Majvers.append(MinVers)
                                .append(Bugvers);
    A_long plugVersionA = std::atoi( plugVersionStr.c_str());

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

    std::string fragErr, vertErr ,redErr, greenErr , blueErr, alphaErr, rgbErr;
    AEFX_CLR_STRUCT(fragErr);
    fragErr = seqP->Glsl33_fragError;
    AEFX_CLR_STRUCT(vertErr);
    vertErr = seqP-> Glsl33_VertError;
	/*
    AEFX_CLR_STRUCT(redErr);
    redErr = seqP->redError;
    AEFX_CLR_STRUCT(greenErr);
    greenErr = seqP->greenError;
    AEFX_CLR_STRUCT(blueErr);
    blueErr = seqP->blueError;
    AEFX_CLR_STRUCT(alphaErr);
    alphaErr =seqP->alphaError;
    AEFX_CLR_STRUCT (rgbErr);
    rgbErr = seqP->rgbError;*/

    tlmath::jsonCorrectorStr(fragErr);
    tlmath::jsonCorrectorStr(vertErr);

    tlmath::jsonCorrectorStr(redErr);
    tlmath::jsonCorrectorStr(greenErr);
    tlmath::jsonCorrectorStr(blueErr);
    tlmath::jsonCorrectorStr(alphaErr);
	tlmath::jsonCorrectorStr(rgbErr);


    arbDataJS["effectInfo"]["pluginVersion"] = plugVersionA;
    arbDataJS["gl_expression"]["gl33_frag_error"] = fragErr;
	arbDataJS["gl_expression"]["gl33_vert_error"] = vertErr;
	arbDataJS["math_expression"]["red_error"] = compile_success; // redErr;
    arbDataJS["math_expression"]["green_error"] = compile_success; //greenErr;
    arbDataJS["math_expression"]["blue_error"] = compile_success; //blueErr;
    arbDataJS["math_expression"]["alpha_error"] = compile_success; // alphaErr;
	arbDataJS["math_expression"]["rgb_error"] = compile_success; //rgbErr;
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
tlmath::SetupGetDataBack(
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
		ERR(tlmath::copyFromArbToSeqData(in_data, out_data,resultStr, seqP));
        if (seqP->exprModeB){
            ERR(tlmath::embedExprInShaders(seqP));
        }
        ERR(tlmath::evalScripts(seqP));
		out_data->sequence_data = seq_dataH;
		suites.HandleSuite1()->host_unlock_handle(seq_dataH);
		ERR(tlmath::updateParamsValue(in_data, params, resultStr));

	}
	else {    // whoa, we couldn't allocate sequence data; bail!
		err = PF_Err_OUT_OF_MEMORY;
	}
    PF_UNLOCK_HANDLE(arbOutH);

    return err;

}


static void
AppendGlslInput2dText(std::string& newSh,
                     std::string  expr,
                     std::string varStr,
					 bool*  hasTexture){
    const std::size_t found = expr.find(varStr);
    if (found!=std::string::npos){
        newSh.append(import2dTextGlStr+ varStr+ endLineStr);
		*hasTexture = true;
    }
}

static void
AppendGlslInputFloat(std::string& newSh,
                     std::string  expr,
                std::string varStr){
    const std::size_t found = expr.find(varStr);
    if (found!=std::string::npos){
        newSh.append(importFloatGlStr+ varStr+ endLineStr);
    }
}
static void
AppendGlslInputVec2d(std::string& newSh,
                     std::string  expr,
                     std::string varStr){
    const std::size_t found = expr.find(varStr);
    if (found!=std::string::npos){
        newSh.append(importVec2GlStr+varStr+endLineStr );
    }
}
static void
AppendGlslInputVec3d(std::string& newSh,
                     std::string  expr,
                     std::string varStr){
    const std::size_t found = expr.find(varStr);
    if (found!=std::string::npos){
        newSh.append(importVec3GlStr+ varStr+ endLineStr);
    }
}
static void
AppendGlslInputBool (std::string& newSh,
                     std::string  expr,
                     std::string varStr){
    const std::size_t found = expr.find(varStr);
    if (found!=std::string::npos){
        newSh.append(importBoolGlStr+ varStr+ endLineStr);
    }
}


PF_Err
tlmath::embedExprInShaders (seqData  *seqP){
    PF_Err err = PF_Err_NONE;

	//float rgbExpr(seqP->expr_pixNameAc,  seqP->expr_ColorChNameAc,vec4 seqP->paramLayer00NameAc,  vec4 seqP->paramLayer01NameAc, vec4 seqP-> paramLayer02NameAc, vec4 seqP-> paramLayer03NameAc, vec4 seqP-> paramLayer04NameAc){

    std::string redExprStr = redFunctionStr+seqP->redExAc+endFunctionStr ;
    std::string greenExprStr = greenFunctionStr+seqP->greenExAc+endFunctionStr;
    std::string blueExprStr = blueFunctionStr+seqP->blueExAc+endFunctionStr;
    std::string alphaExprStr = alphaFunctionStr+seqP->alphaExAc+endFunctionStr;
    std::string rgbExprStr = rgbFunctionStr+ seqP->rgbExprExAc +endFunctionStr;



    std::string exprGrpStr =redExprStr+ greenExprStr+blueExprStr+alphaExprStr;
    if (seqP->exprRGBModeB){
        AEFX_CLR_STRUCT(exprGrpStr);
        exprGrpStr =rgbExprStr;
    }

    //start new Shader as string
    std::string fragShStr = gl33GeneriqueShInput; //get the classic input

    //bool
    AppendGlslInputBool(fragShStr,exprGrpStr, seqP-> paramCb01NameAc);
    AppendGlslInputBool(fragShStr,exprGrpStr, seqP-> paramCb02NameAc);
    AppendGlslInputBool(fragShStr,exprGrpStr, seqP-> paramCb03NameAc);
    AppendGlslInputBool(fragShStr,exprGrpStr, seqP-> paramCb04NameAc);
    AppendGlslInputBool(fragShStr,exprGrpStr, seqP-> paramCb05NameAc);
    AppendGlslInputBool(fragShStr,exprGrpStr, seqP-> paramCb06NameAc);
    AppendGlslInputBool(fragShStr,exprGrpStr, seqP-> paramCb07NameAc);
    AppendGlslInputBool(fragShStr,exprGrpStr, seqP-> paramCb08NameAc);
    AppendGlslInputBool(fragShStr,exprGrpStr, seqP-> paramCb09NameAc);
    AppendGlslInputBool(fragShStr,exprGrpStr, seqP-> paramCb10NameAc);

    //1d
    AppendGlslInputFloat(fragShStr,exprGrpStr, seqP->time_secNameAc);
    AppendGlslInputFloat(fragShStr,exprGrpStr, seqP->time_frameNameAc);
    AppendGlslInputFloat(fragShStr,exprGrpStr, seqP->frame_rateNameAc);
    AppendGlslInputFloat(fragShStr,exprGrpStr, seqP->paramSlider01NameAc);
    AppendGlslInputFloat(fragShStr,exprGrpStr, seqP->paramSlider02NameAc);
    AppendGlslInputFloat(fragShStr,exprGrpStr, seqP->paramSlider03NameAc);
    AppendGlslInputFloat(fragShStr,exprGrpStr, seqP->paramSlider04NameAc);
    AppendGlslInputFloat(fragShStr,exprGrpStr, seqP->paramSlider05NameAc);
    AppendGlslInputFloat(fragShStr,exprGrpStr, seqP->paramSlider06NameAc);
    AppendGlslInputFloat(fragShStr,exprGrpStr, seqP->paramSlider07NameAc);
    AppendGlslInputFloat(fragShStr,exprGrpStr, seqP->paramSlider08NameAc);
    AppendGlslInputFloat(fragShStr,exprGrpStr, seqP->paramSlider09NameAc);
    AppendGlslInputFloat(fragShStr,exprGrpStr, seqP->paramRot01NameAc);
    AppendGlslInputFloat(fragShStr,exprGrpStr, seqP->paramRot02NameAc);
    AppendGlslInputFloat(fragShStr,exprGrpStr, seqP->paramRot03NameAc);
    AppendGlslInputFloat(fragShStr,exprGrpStr, seqP->paramRot04NameAc);
    AppendGlslInputFloat(fragShStr,exprGrpStr, seqP->paramRot05NameAc);
    AppendGlslInputFloat(fragShStr,exprGrpStr, seqP->paramRot06NameAc);
    AppendGlslInputFloat(fragShStr,exprGrpStr, seqP->paramRot07NameAc);
    AppendGlslInputFloat(fragShStr,exprGrpStr, seqP->paramRot08NameAc);
    AppendGlslInputFloat(fragShStr,exprGrpStr, seqP->paramRot09NameAc);
    AppendGlslInputFloat(fragShStr,exprGrpStr, seqP->paramRot10NameAc);

    //2d
    AppendGlslInputVec2d(fragShStr,exprGrpStr, seqP->resolutionNameAc);
    AppendGlslInputVec2d(fragShStr,exprGrpStr, seqP->compResolutionNameAc);

    //3d
    AppendGlslInputVec3d(fragShStr,exprGrpStr, seqP->layerPositionNameAc);
    AppendGlslInputVec3d(fragShStr,exprGrpStr, seqP->layerScaleNameAc);
    AppendGlslInputVec3d(fragShStr,exprGrpStr, seqP->cameraPosNameAc);
    AppendGlslInputVec3d(fragShStr,exprGrpStr, seqP->cameraTargetNameAc);
    AppendGlslInputVec3d(fragShStr,exprGrpStr, seqP->cameraRotationNameAc);
    AppendGlslInputVec3d(fragShStr,exprGrpStr, seqP->cameraZoomNameAc);
    AppendGlslInputVec3d(fragShStr,exprGrpStr, seqP->paramPoint01NameAc);
    AppendGlslInputVec3d(fragShStr,exprGrpStr, seqP->paramPoint02NameAc);
    AppendGlslInputVec3d(fragShStr,exprGrpStr, seqP->paramPoint03NameAc);
    AppendGlslInputVec3d(fragShStr,exprGrpStr, seqP->paramPoint04NameAc);
    AppendGlslInputVec3d(fragShStr,exprGrpStr, seqP-> paramPoint05NameAc);
    AppendGlslInputVec3d(fragShStr,exprGrpStr, seqP->paramPoint06NameAc);
    AppendGlslInputVec3d(fragShStr,exprGrpStr, seqP->paramPoint07NameAc);
    AppendGlslInputVec3d(fragShStr,exprGrpStr, seqP->paramPoint08NameAc);
    AppendGlslInputVec3d(fragShStr,exprGrpStr, seqP->paramPoint09NameAc);
    AppendGlslInputVec3d(fragShStr,exprGrpStr, seqP->paramPoint10NameAc);

    AppendGlslInputVec3d(fragShStr,exprGrpStr, seqP->paramColor01NameAc);
    AppendGlslInputVec3d(fragShStr,exprGrpStr, seqP->paramColor02NameAc);
    AppendGlslInputVec3d(fragShStr,exprGrpStr, seqP->paramColor03NameAc);
    AppendGlslInputVec3d(fragShStr,exprGrpStr, seqP->paramColor04NameAc);
    AppendGlslInputVec3d(fragShStr,exprGrpStr, seqP-> paramColor05NameAc);
    AppendGlslInputVec3d(fragShStr,exprGrpStr, seqP->paramColor06NameAc);
    AppendGlslInputVec3d(fragShStr,exprGrpStr, seqP->paramColor07NameAc);
    AppendGlslInputVec3d(fragShStr,exprGrpStr, seqP->paramColor08NameAc);
    AppendGlslInputVec3d(fragShStr,exprGrpStr, seqP->paramColor09NameAc);
    AppendGlslInputVec3d(fragShStr,exprGrpStr, seqP->paramColor10NameAc);

    //for texttures
	bool hasTexture0B, hasTexture1B, hasTexture2B, hasTexture3B, hasTexture4B = false;
    AppendGlslInput2dText (fragShStr,exprGrpStr, "text0", &hasTexture0B);
    AppendGlslInput2dText (fragShStr,exprGrpStr, "text1", &hasTexture1B);
    AppendGlslInput2dText (fragShStr,exprGrpStr, "text2", &hasTexture2B);
    AppendGlslInput2dText (fragShStr,exprGrpStr, "text3", &hasTexture3B);
    AppendGlslInput2dText (fragShStr,exprGrpStr, "text4", &hasTexture4B);
    if (hasTexture0B || hasTexture1B, hasTexture2B, hasTexture3B, hasTexture4B){
        //add the function to load texture
		exprGrpStr.append(gl33InputTexture);
    }

    /*
	float seqP->expr_lumaNameAc (vec4 text) {
	return 0.3*text.r+0.59*text.g+0.11*text.b;
	}
	//indexer le numero de ligne de chaque expression.

	}

void main(void)
{
	vec4 text0= loadTextureFromAE(texture1, out_uvs.xy);
	vec4 text1, text2, text3, text4;
	fragColorOut.r = rgbExpr(gl_FragCoord.xy, text0.r, text0, text1, text2, text3, text4);
	fragColorOut.g = rgbExpr(gl_FragCoord.xy, text0.g, text0, text1, text2, text3, text4);
	fragColorOut.b = rgbExpr(gl_FragCoord.xy, text0.b, text0, text1, text2, text3, text4);
	fragColorOut.a = 1; //rgbExpr(gl_FragCoord.xy, text0.a, text0, text1, text2, text3, text4);
}

	
	
     //for expressions
    );
     );
     );
     seqP->expr_pix_offNameAc);
     */

    //embed expressions in shaders and add uniforms in shaders depending of expression.
    //gl33InputMainGrp
    //gl33InputMainSplit

    return err;

}

PF_Err
tlmath::evalScripts(seqData  *seqDataP)
{
    PF_Err err = PF_Err_NONE;
    std::string evalVertSh, evalFragSh;


    tlmath::evalVertShader (seqDataP->Glsl33_VertexShAc, evalVertSh);
    if (evalVertSh != compile_success){
        #ifdef AE_OS_WIN
                strncpy_s(seqDataP->Glsl33_VertexShAc, glvertstr.c_str(),  glvertstr.length() + 1);
        #else
        strncpy(seqDataP->Glsl33_VertexShAc, glvertstr.c_str(),  glvertstr.length() + 1);
        #endif
    }

    tlmath::evalFragShader (seqDataP->Glsl33_FragmentShAc, evalFragSh);
    if (evalFragSh != compile_success){
        std::string setting_resolutionName = "resolution";
        #ifdef AE_OS_WIN
                 strncpy_s(seqDataP->resolutionNameAc, setting_resolutionName.c_str(),  setting_resolutionName.length() + 1);
                 strncpy_s(seqDataP->Glsl33_FragmentShAc, glErrorMessageStr.c_str(),  glErrorMessageStr .length() + 1);
        #else
                strncpy(seqDataP->resolutionNameAc, setting_resolutionName.c_str(),  setting_resolutionName.length() + 1);
        strncpy(seqDataP->Glsl33_FragmentShAc, glErrorMessageStr.c_str(),  glErrorMessageStr .length() + 1);
        #endif

    }
     #ifdef AE_OS_WIN
        strncpy_s(seqDataP->Glsl33_fragError , evalFragSh.c_str(),  evalFragSh.length() + 1);
        strncpy_s(seqDataP->Glsl33_VertError , evalVertSh.c_str(),   evalVertSh.length() + 1);
     #else

    strncpy(seqDataP->Glsl33_fragError , evalFragSh.c_str(),  evalFragSh.length() + 1);
    strncpy(seqDataP->Glsl33_VertError , evalVertSh.c_str(),   evalVertSh.length() + 1);
     #endif


    return err;
}


