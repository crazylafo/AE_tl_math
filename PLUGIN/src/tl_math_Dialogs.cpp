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
	AEGP_LayerH        layerH = nullptr;
	AEGP_CompH        compH = nullptr;
	AEGP_ItemH      itemH = nullptr;
	AEGP_StreamRefH streamH = nullptr;
	AEGP_EffectRefH effectH = nullptr;
	AEGP_StreamRefH parentStreamH = nullptr;
	const my_global_dataP		globP = static_cast<my_global_dataP>(DH(out_data->global_data));

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

 PF_Err
tlmath::AEGP_SetParamStreamValue(PF_InData            *in_data,
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

	AEGP_MemHandle     resultMemH = nullptr;
	A_char *resultAC = nullptr;
	A_char          scriptAC[1500]{ '\0' };
	const my_global_dataP        globP = static_cast<my_global_dataP>(DH(out_data->global_data));

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
    PF_Err err = PF_Err_NONE, err2 = PF_Err_NONE;
	
    AEGP_SuiteHandler    suites(in_data->pica_basicP);
    const my_global_dataP        globP = static_cast<my_global_dataP>(DH(out_data->global_data));
    const  seqDataP seqP = reinterpret_cast<seqDataP>(DH(out_data->sequence_data));
    AEGP_MemHandle     resultMemH =nullptr;
    A_char *resultAC = nullptr;
    A_char          scriptAC[120000] { '\0' };
    std::string Majvers = std::to_string(MAJOR_VERSION);
    std::string MinVers = std::to_string(MINOR_VERSION);
    std::string Bugvers = std::to_string(BUG_VERSION);

    std::string plugVersionStr =Majvers.append(MinVers)
                                .append(Bugvers);
    A_long plugVersionA = std::atoi( plugVersionStr.c_str());

    //ARB
    PF_ParamDef arb_param;
    m_ArbData        *arbInP = nullptr;
    AEFX_CLR_STRUCT(arb_param);
    ERR(PF_CHECKOUT_PARAM(    in_data,
                          MATH_ARB_DATA,
                          in_data->current_time,
                          in_data->time_step,
                          in_data->time_scale,
                          &arb_param));
        AEFX_CLR_STRUCT(arbInP);
        arbInP = static_cast<m_ArbData*>(*arb_param.u.arb_d.value);
        if (!arbInP){
            return err = PF_Err_OUT_OF_MEMORY;
        }
    auto  arbDataJS = nlohmann::json::parse(arbInP->arbDataAc);


    std::string fragErr, vertErr ,redErr, greenErr , blueErr, alphaErr, rgbErr;

    AEFX_CLR_STRUCT(fragErr);
    fragErr = seqP->Glsl33_fragError;
    AEFX_CLR_STRUCT(vertErr);
    vertErr = seqP-> Glsl33_VertError;
	arbDataJS["effectInfo"]["pluginVersion"] = plugVersionA;


	if (seqP->glsl33ModeB) {
		tlmath::jsonCorrectorStr(fragErr);
		strReplace(fragErr, "ERROR:", "\\nERROR:");
		tlmath::jsonCorrectorStr(vertErr);
		strReplace(vertErr, "ERROR:", "\\nERROR:");
		arbDataJS["gl_expression"]["gl33_frag_error"] = fragErr;
		arbDataJS["gl_expression"]["gl33_vert_error"] = vertErr;
	}
	else {
		if (seqP->exprRGBModeB) {
			std::string exprRGBErrStr = seqP->rgbError;
			tlmath::jsonCorrectorStr(exprRGBErrStr);
			strReplace(exprRGBErrStr, "ERROR:", "\\nERROR:");
			arbDataJS["math_expression"]["rgb_error"] = exprRGBErrStr;
		}
		else {
			std::string exprRedErrStr = seqP->redError;
			std::string exprGreenErrStr = seqP->greenError;
			std::string exprBlueErrStr = seqP->blueError;
			tlmath::jsonCorrectorStr(exprRedErrStr);
			tlmath::jsonCorrectorStr(exprGreenErrStr);
			tlmath::jsonCorrectorStr(exprBlueErrStr);
			strReplace(exprRedErrStr, "ERROR:", "\\nERROR:");
			strReplace(exprGreenErrStr, "ERROR:", "\\nERROR:");
			strReplace(exprBlueErrStr, "ERROR:", "\\nERROR:");
			arbDataJS["math_expression"]["red_error"] = exprRedErrStr;
			arbDataJS["math_expression"]["green_error"] = exprGreenErrStr;
			arbDataJS["math_expression"]["blue_error"] = exprBlueErrStr;
			}
		std::string commonexprErrStr = seqP->commonError;
		std::string exprAlphaErrStr = seqP->alphaError;
		tlmath::jsonCorrectorStr(commonexprErrStr);
		tlmath::jsonCorrectorStr(exprAlphaErrStr);
		strReplace(commonexprErrStr, "ERROR:", "\\nERROR:");
		strReplace(exprAlphaErrStr, "ERROR:", "\\nERROR:");
		arbDataJS["math_expression"]["common_error"] = commonexprErrStr;
		arbDataJS["math_expression"]["alpha_error"] = exprAlphaErrStr;
		}


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

	ERR2(PF_CHECKIN_PARAM(in_data, &arb_param));
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
    const my_global_dataP globP = static_cast<my_global_dataP>(DH(out_data->global_data));
    PF_Handle        arbOutH = nullptr;
    PF_ParamDef arb_param;
	AEGP_MemHandle     resultMemH = nullptr;
	A_char *resultAC = nullptr;
	std::string resultStr;
    m_ArbData *arbOutP = nullptr;

	AEFX_CLR_STRUCT(arb_param);
	ERR(PF_CHECKOUT_PARAM(in_data,
		MATH_ARB_DATA,
		in_data->current_time,
		in_data->time_step,
		in_data->time_scale,
		&arb_param));
	arbOutP = static_cast<m_ArbData*>(*arb_param.u.arb_d.value);
	ERR(suites.UtilitySuite6()->AEGP_ExecuteScript(globP->my_id, script_getDataBackFromMathCEP.c_str(), FALSE, &resultMemH, NULL));
	AEFX_CLR_STRUCT(resultAC);
	ERR(suites.MemorySuite1()->AEGP_LockMemHandle(resultMemH, reinterpret_cast<void**>(&resultAC)));
    
    if (resultAC){  
        resultStr = resultAC;
        jsonStrToArb(resultStr, arbOutP);
		arbOutP->hasChangedB = true;
    }
	ERR(suites.MemorySuite1()->AEGP_FreeMemHandle(resultMemH));
	arbOutH = reinterpret_cast <PF_Handle>(arbOutP);
    //AEGP SETSTREAMVALUE TO ARB
    ERR (AEGP_SetParamStreamValue(in_data, out_data, globP->my_id, MATH_ARB_DATA, &arbOutH));

	PF_Handle    seq_dataH = suites.HandleSuite1()->host_new_handle(sizeof(seqData));

	if (seq_dataH) {
		seqData  	*seqP = static_cast<seqData*>(suites.HandleSuite1()->host_lock_handle(seq_dataH));
		ERR(tlmath::copyFromArbToSeqData(in_data, out_data,resultStr, seqP));
		if (seqP->exprModeB) {
			ERR(tlmath::embedExprInShaders(seqP));
		}
        evalScripts(seqP);
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
	tlmath_shaders tlms;
    if (found!=std::string::npos){
        newSh.append(tlms.import2dTextGlStr+ varStr+ tlms.endLineStr);
		*hasTexture = true;
    }
}

static void
AppendGlslInputFloat(std::string& newSh,
                     std::string  expr,
                std::string varStr){
    const std::size_t found = expr.find(varStr);
	tlmath_shaders tlms;
    if (found!=std::string::npos){
        newSh.append(tlms.importFloatGlStr+ varStr+ tlms.endLineStr);
    }
}
static void
AppendGlslInputVec2d(std::string& newSh,
                     std::string  expr,
                     std::string varStr){
    const std::size_t found = expr.find(varStr);
    if (found!=std::string::npos){
		tlmath_shaders tlms;
        newSh.append(tlms.importVec2GlStr+varStr+ tlms.endLineStr );
    }
}
static void
AppendGlslInputVec3d(std::string& newSh,
                     std::string  expr,
                     std::string varStr){
    const std::size_t found = expr.find(varStr);
    if (found!=std::string::npos){
		tlmath_shaders tlms;
        newSh.append(tlms.importVec3GlStr+ varStr+ tlms.endLineStr);
    }
}
static void
AppendGlslInputBool (std::string& newSh,
                     std::string  expr,
                     std::string varStr){
    const std::size_t found = expr.find(varStr);
    if (found!=std::string::npos){
		tlmath_shaders tlms;
        newSh.append(tlms.importBoolGlStr+ varStr+ tlms.endLineStr);
    }
}


PF_Err
tlmath::embedExprInShaders (seqData  *seqP){
    PF_Err err = PF_Err_NONE;

	tlmath_shaders tlms;
    std::string redExprStr = tlms.redFunctionStr+seqP->redExAc+ tlms.endFunctionStr ;
    std::string greenExprStr = tlms.greenFunctionStr+seqP->greenExAc+ tlms.endFunctionStr;
    std::string blueExprStr = tlms.blueFunctionStr+seqP->blueExAc+ tlms.endFunctionStr;
    std::string rgbExprStr = tlms.rgbFunctionStr+ seqP->rgbExprExAc + tlms.endFunctionStr;
	std::string alphaExprStr = tlms.alphaFunctionStr + seqP->alphaExAc + tlms.endFunctionStr;

    std::string exprGrpStr =redExprStr+ greenExprStr+blueExprStr+alphaExprStr;
    if (seqP->exprRGBModeB){
        AEFX_CLR_STRUCT(exprGrpStr);
        exprGrpStr =rgbExprStr + alphaExprStr;
    }
	strReplace(exprGrpStr, "fragCoord", seqP->expr_pixNameAc);
	strReplace(exprGrpStr, "colorCh", seqP->expr_ColorChNameAc);
	strReplace(exprGrpStr, "inputLayer0", seqP->paramLayer00NameAc);

    //start new Shader as string
    std::string fragShStr = tlms.gl33GeneriqueShInput; //get the classic input

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
	AppendGlslInputFloat(fragShStr, exprGrpStr, seqP->paramSlider10NameAc);
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
	//we force to import texture0, alias current layer
	fragShStr.append(tlms.import2dTextGlStr + seqP->paramLayer00NameAc + tlms.endLineStr);
	//the other only if they are called
	bool  hasTexture1B = false, hasTexture2B = false , hasTexture3B = false, hasTexture4B  = false;
    AppendGlslInput2dText (fragShStr,exprGrpStr, seqP->paramLayer01NameAc, &hasTexture1B);
    AppendGlslInput2dText (fragShStr,exprGrpStr, seqP-> paramLayer02NameAc, &hasTexture2B);
    AppendGlslInput2dText (fragShStr,exprGrpStr, seqP-> paramLayer03NameAc, &hasTexture3B);
    AppendGlslInput2dText (fragShStr,exprGrpStr,seqP-> paramLayer04NameAc, &hasTexture4B);

	fragShStr.append("//BEGIN IMPORT COMMON EXPR \n");
	fragShStr.append(seqP->commonExpr);
	fragShStr.append("//END OF IMPORT COMMON EXPR \n");


     //embed expressions in shaders and add uniforms in shaders depending of expression.
	fragShStr.append(exprGrpStr);
    if (seqP->exprRGBModeB){
        strReplace(tlms.gl33InputMainGrp,"inputLayer0", seqP->paramLayer00NameAc);
        fragShStr.append (tlms.gl33InputMainGrp);
    }else{
		strReplace(tlms.gl33InputMainSplit, "inputLayer0", seqP->paramLayer00NameAc);
		fragShStr.append(tlms.gl33InputMainSplit);

    }
	std::size_t length = fragShStr.copy(seqP->Glsl33_FragmentShAc, fragShStr.size());
	seqP->Glsl33_FragmentShAc[length] = '\0';
	//keep the default vertex shader
	std::size_t  vertLength = tlms.glvertstr.copy(seqP->Glsl33_VertexShAc, tlms.glvertstr.length());
	seqP->Glsl33_VertexShAc[vertLength] = '\0';
    return err;

}
void 
tlmath::evalScripts(seqData  *seqP)
{
    std::string evalVertSh, evalFragSh;

	tlmath_shaders tlms;
    tlmath::evalVertShader (seqP->Glsl33_VertexShAc, evalVertSh);
    if (evalVertSh != tlms.compile_success){
        #ifdef AE_OS_WIN
                strncpy_s(seqP->Glsl33_VertexShAc, tlms.glvertstr.c_str(), tlms.glvertstr.length() + 1);
        #else
        strncpy(seqP->Glsl33_VertexShAc, glvertstr.c_str(),  glvertstr.length() + 1);
        #endif
    }

    tlmath::evalFragShader (seqP->Glsl33_FragmentShAc, evalFragSh);
	std::string originalfragSh = seqP->Glsl33_FragmentShAc;
    if (evalFragSh != tlms.compile_success){
        std::string setting_resolutionName = "resolution";
        #ifdef AE_OS_WIN
                 strncpy_s(seqP->resolutionNameAc, setting_resolutionName.c_str(),  setting_resolutionName.length() + 1);
                 strncpy_s(seqP->Glsl33_FragmentShAc, tlms.glErrorMessageStr.c_str(), tlms.glErrorMessageStr .length() + 1);
        #else
                strncpy(seqP->resolutionNameAc, setting_resolutionName.c_str(),  setting_resolutionName.length() + 1);
        strncpy(seqP->Glsl33_FragmentShAc, tlms.glErrorMessageStr.c_str(), tlms.glErrorMessageStr .length() + 1);
        #endif

    }
         #ifdef AE_OS_WIN
            strncpy_s(seqP->Glsl33_fragError , evalFragSh.c_str(),  evalFragSh.length() + 1);
            strncpy_s(seqP->Glsl33_VertError , evalVertSh.c_str(),   evalVertSh.length() + 1);
         #else

        strncpy(seqP->Glsl33_fragError , evalFragSh.c_str(),  evalFragSh.length() + 1);
        strncpy(seqP->Glsl33_VertError , evalVertSh.c_str(),   evalVertSh.length() + 1);
     #endif

	if (seqP->exprModeB) {
		std::string errIndex = "ERROR: 0:";
		size_t alphaExprIndex = originalfragSh.find("float alphaExpr");
		if (seqP->exprRGBModeB) {
			size_t rgbExprIndex = originalfragSh.find("vec3 rgbExpr(");
			std::string exprRGBErrStr = tlmath::ReIndexErrorInExpr(originalfragSh, evalFragSh, errIndex, rgbExprIndex, alphaExprIndex);

			#ifdef AE_OS_WIN
			strncpy_s(seqP->rgbError, exprRGBErrStr.c_str(), exprRGBErrStr.length() + 1);
			#else
			strncpy(seqP->rgbError, exprRGBErrStr.c_str(), exprRGBErrStr.length() + 1);
			#endif
		}
		else {
			const size_t redExprIndex = originalfragSh.find("float redExpr(");
			const size_t greenExprIndex = originalfragSh.find("float greenExpr(");
			const size_t blueExprIndex = originalfragSh.find("float blueExpr(");
			//redExprStr
			std::string exprRedErrStr = tlmath::ReIndexErrorInExpr(originalfragSh, evalFragSh, errIndex, redExprIndex, greenExprIndex);
			//greenExprStr
			std::string exprGreenErrStr = tlmath::ReIndexErrorInExpr(originalfragSh, evalFragSh, errIndex, greenExprIndex, blueExprIndex);
			// blueExprStr
			std::string exprBlueErrStr = tlmath::ReIndexErrorInExpr(originalfragSh, evalFragSh, errIndex, blueExprIndex, alphaExprIndex);
			#ifdef AE_OS_WIN
					strncpy_s(seqP->redError, exprRedErrStr.c_str(), exprRedErrStr.length() + 1);
					strncpy_s(seqP->greenError, exprGreenErrStr.c_str(), exprGreenErrStr.length() + 1);
					strncpy_s(seqP->blueError, exprBlueErrStr.c_str(), exprBlueErrStr.length() + 1);
			#else
					strncpy(seqP->redError, exprRedErrStr.c_str(), exprRedErrStr.length() + 1);
					strncpy(seqP->greenError, exprGreenErrStr.c_str(), exprGreenErrStr.length() + 1);
					strncpy(seqP->blueError, exprBlueErrStr.c_str(), exprBlueErrStr.length() + 1);
			#endif
		}
		//alphaExprStr
		size_t commonExprBeginIndex = originalfragSh.find("//BEGIN IMPORT COMMON EXPR");
		size_t commonExprEndIndex = originalfragSh.find("//END OF IMPORT COMMON EXPR");
		std::string commonErrStr = tlmath::ReIndexErrorInExpr(originalfragSh, evalFragSh, errIndex, commonExprBeginIndex, commonExprEndIndex);
		const size_t mainFunIndex = originalfragSh.find("main(void)"); //to delimit the end of the alpha expr
		std::string exprAlphaErrStr = tlmath::ReIndexErrorInExpr(originalfragSh, evalFragSh, errIndex, alphaExprIndex, mainFunIndex);
		#ifdef AE_OS_WIN
			strncpy_s(seqP->commonError, commonErrStr.c_str(), commonErrStr.length() + 1);
			strncpy_s(seqP->alphaError, exprAlphaErrStr.c_str(), exprAlphaErrStr.length() + 1);
		#else
			strncpy(seqP->commonError, commonErrStr.c_str(), commonErrStr.length() + 1);
			strncpy(seqP->alphaError, exprAlphaErrStr.c_str(), exprAlphaErrStr.length() + 1);
		#endif
	 }
}


