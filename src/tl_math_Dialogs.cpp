//
//  tl_math_Dialogs.cpp
//  tlMath
//
//
//
#include "tl_math.h"
#include "script.h"


//detect if a string has a specified char
static PF_Boolean
hasString(std::string str, std::string expr)
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

static std::string evalMathExprStr (std::string redResultStr, std::string greenResultStr, std::string blueResultStr, std::string alphaResultStr)
{
    PF_Boolean returnExprErrB = false;
    std::string errReturn = "Error \n";
    MathInfoP miP;
    funcTransfertInfoP fiP;
    fiP.redExpr = parseExpr<PF_FpShort>((void*)&miP, &fiP, redResultStr);
    if (fiP.hasErrorB)
    {
        fiP.channelErrorstr = "red channel expression";
        returnExprErrB = true;
        errReturn.append(fiP.channelErrorstr).append(": ").append(fiP.errorstr).append("\n");

    }
    fiP.greenExpr = parseExpr<PF_FpShort>((void*)&miP, &fiP, greenResultStr);
    if (fiP.hasErrorB)
    {
        fiP.channelErrorstr = "green channel expression";
        returnExprErrB = true;
        errReturn.append(fiP.channelErrorstr).append(": ").append(fiP.errorstr).append("\n");

    }
    fiP.blueExpr = parseExpr<PF_FpShort>((void*)&miP, &fiP, blueResultStr);
    if (fiP.hasErrorB)
    {
        fiP.channelErrorstr = "blue channel expression";
        returnExprErrB = true;
        errReturn.append(fiP.channelErrorstr).append(": ").append(fiP.errorstr).append("\n");

    }
    fiP.alphaExpr = parseExpr<PF_FpShort>((void*)&miP, &fiP, alphaResultStr);
    if (fiP.hasErrorB)
    {
        fiP.channelErrorstr = "alpha channel expression";
        returnExprErrB = true;
        errReturn.append(fiP.channelErrorstr).append(": ").append(fiP.errorstr).append("\n");
    }
    if (!returnExprErrB) {
        errReturn = compile_success;
    }
    return errReturn;
}


static void jsonStrToArb (std::string resultStr,
                          m_ArbData    *arbOutP)
{


    //copy to flat ARB (keeping /n and other speical char from js
    #ifdef AE_OS_WIN
	strncpy_s(arbOutP->arbDataStr, resultStr.c_str(), resultStr.length() + 1);
    #else
	strncpy(arbOutP->arbDataStr, resultStr.c_str(), resultStr.length() + 1);
    #endif

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
			PF_OutData        *out_data,
              std::string     &taskId)
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
    //taskId = resultAC;
	ERR(suites.MemorySuite1()->AEGP_FreeMemHandle(resultMemH));

	return err;
}



PF_Err
SetupDialogSend(
                  PF_InData        *in_data,
                  PF_OutData        *out_data,
                  PF_ParamDef        *params[],
                  std::string   taskId,
                  PF_LayerDef        *output)
{
    PF_Err err = PF_Err_NONE;
    AEGP_SuiteHandler    suites(in_data->pica_basicP);
    my_global_dataP        globP = reinterpret_cast<my_global_dataP>(DH(out_data->global_data));

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
    std::string resultStr;
    std::string errReturn = "NONE";
    std::string jsonDump = "'''";
    jsonDump.append(arbInP->arbDataStr);
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




