//
//  tl_math_Dialogs.cpp
//  tlMath
//
//
//

#include "tl_math.h"
#include "script.h"

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
void ExprtkCorrectorStr(std::string &str) {
	//adapt exprtk language to AE expression's language.
	strReplace(str, "&&", "&");
	strReplace(str, "||", "|");
	strReplace(str, "++", "+=1");
	strReplace(str, "--", "-=1");
	strReplace(str, " = ", " := ");
	strReplace(str, "\t", "    ");
}
//detect if a string has a specified string
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

PF_Err
SetupDialog(
          PF_InData        *in_data,
          PF_OutData        *out_data,
          PF_ParamDef        *params[],
          PF_LayerDef        *output)
{
    PF_Err err = PF_Err_NONE;
    AEGP_SuiteHandler    suites(in_data->pica_basicP);
    my_global_dataP        globP = reinterpret_cast<my_global_dataP>(DH(out_data->global_data));

    AEGP_MemHandle     resultMemH = NULL;
    A_char *resultAC = NULL;
    A_char          scriptAC[40000] { '\0' };
    std::string Majvers = std::to_string(MAJOR_VERSION);
    std::string MinVers = std::to_string(MINOR_VERSION);
    std::string Bugvers = std::to_string(BUG_VERSION);

    //ARB
    PF_ParamDef arb_param;

    //strings to send expr to script
    std::string tempName;
    std::string tempDescription;


    std::string tempGlsl;

    std::string tempRedS;
    std::string tempGreenS;
    std::string tempBlueS;
    std::string tempAlphaS;



	PF_Boolean		tempParserModeB;



    PF_Handle        arbOutH = NULL;
    m_ArbData        *arbInP = NULL;
    m_ArbData        *arbOutP= NULL;


    AEFX_CLR_STRUCT(arb_param);
    ERR(PF_CHECKOUT_PARAM(    in_data,
                          MATH_ARB_DATA,
                          in_data->current_time,
                          in_data->time_step,
                          in_data->time_scale,
                          &arb_param));

    if (!err){
        AEFX_CLR_STRUCT(arbInP);
        arbInP = reinterpret_cast<m_ArbData*>(*arb_param.u.arb_d.value);
        if (arbInP){
            tempParserModeB =arbInP->parserModeB;
            tempRedS.append(arbInP->redExAcFlat);
            tempGreenS.append(arbInP->greenExAcFlat);
            tempBlueS.append(arbInP->blueExAcFlat);
            tempAlphaS.append(arbInP->alphaExAcFlat);
            tempName.append(arbInP->presetNameAc);
			tempGlsl.append(arbInP->Glsl_FragmentShFlat);
            tempDescription.append(arbInP->descriptionAcFlat);
        }

    }
    //to force the parser to keep \n before to send it to js
    strReplace(tempRedS, "\n", "\\n");
    strReplace(tempGreenS, "\n", "\\n");
    strReplace(tempBlueS, "\n", "\\n");
    strReplace(tempAlphaS, "\n", "\\n");
    strReplace(tempDescription, "\n", "\\n");
    strReplace(tempGlsl, "\n" , "\\n");

	
	
	if (tempGlsl == "fragSh") {
	tempGlsl = glfrag1str;
	strReplace(tempGlsl, "\n", "\\n");
	}

	

    nlohmann::json  jToJs;
    jToJs["parserModeB"] = tempParserModeB;
    jToJs["presetName"] =tempName;
    jToJs["description"]=tempDescription;
    jToJs["redExpr"]=tempRedS;
    jToJs["greenExpr"]=tempGreenS;
    jToJs["blueExpr"]=tempBlueS;
    jToJs["alphaExpr"]=tempAlphaS;
    jToJs["glslExpr"]=tempGlsl;

	
    std::string jsonDump = "'''";
    jsonDump .append(jToJs.dump());
    jsonDump.append("'''");

    sprintf( scriptAC,
            script_ui.c_str(),
            jsonDump.c_str(),
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




        arbOutP->PixelsCallExternalInputB = hasString(resultStr, std::string("extL"));
		arbOutP->PixelsCallExternalInputB = hasString(resultStr, std::string("extLayerTex"));
        arbOutP->PresetHasWideInputB = hasString(resultStr, std::string("layerTime"));
		arbOutP->PresetHasWideInputB = hasString(resultStr, std::string("time"));
        arbOutP->NeedsPixelAroundB = hasString(resultStr, std::string("vec9_"));
        arbOutP->NeedsLumaB = hasString(resultStr, std::string("in_luma"));
        arbOutP->CallsAEGP_CompB =hasString(resultStr, std::string("layer"));
        arbOutP->CallsAEGP_layerB =hasString(resultStr, std::string("comp"));


        nlohmann::json  jresult = nlohmann::json::parse(resultStr);

		bool ParserModeB;
		
		ParserModeB = jresult["/parserModeB"_json_pointer];
		arbOutP->parserModeB = ParserModeB;
        std::string redResultStr =   jresult["/redExpr"_json_pointer];
		ExprtkCorrectorStr(redResultStr);
        std::string greenResultStr = jresult["/greenExpr"_json_pointer];
		ExprtkCorrectorStr(greenResultStr);
        std::string blueResultStr =  jresult["/blueExpr"_json_pointer];
		ExprtkCorrectorStr(blueResultStr);
        std::string alphaResultStr = jresult["/alphaExpr"_json_pointer];
		ExprtkCorrectorStr(alphaResultStr);
        std::string glslExpr = jresult["/glslExpr"_json_pointer];
		strReplace(glslExpr, "\t", "    ");

        std::string presetNameStr = jresult["/presetName"_json_pointer];
        std::string descriptionStr = jresult["/description"_json_pointer];

		if (glslExpr.compare(arbInP->Glsl_FragmentShFlat) !=0)  {
			arbOutP->ShaderResetB = true;
		}

        presetNameStr.erase(std::remove(presetNameStr.begin(), presetNameStr.end(), '\n'), presetNameStr.end());

        //copy to flat ARB (keeping /n and other speical char from js
#ifdef AE_OS_WIN
        strncpy_s( arbOutP->redExAcFlat, redResultStr.c_str(), redResultStr.length()+1);
        strncpy_s(arbOutP->greenExAcFlat, greenResultStr.c_str(), greenResultStr.length()+1);
        strncpy_s( arbOutP->blueExAcFlat, blueResultStr.c_str(), blueResultStr.length()+1);
        strncpy_s( arbOutP->alphaExAcFlat, alphaResultStr.c_str(), alphaResultStr.length()+1);

        strncpy_s( arbOutP->Glsl_FragmentShFlat, glslExpr.c_str(), glslExpr.length()+1);
		strncpy_s(arbOutP->Glsl_VertexShFlat, glvertstr.c_str(), glvertstr.length() + 1);
        strncpy_s( arbOutP->descriptionAcFlat, descriptionStr.c_str(), descriptionStr.length()+1);
#else
        strncpy( arbOutP->redExAcFlat, redResultStr.c_str(), redResultStr.length()+1);
        strncpy(arbOutP->greenExAcFlat, greenResultStr.c_str(), greenResultStr.length()+1);
        strncpy( arbOutP->blueExAcFlat, blueResultStr.c_str(), blueResultStr.length()+1);
        strncpy( arbOutP->alphaExAcFlat, alphaResultStr.c_str(), alphaResultStr.length()+1);
        strncpy( arbOutP->descriptionAcFlat, descriptionStr.c_str(), descriptionStr.length()+1);
        strncpy( arbOutP->Glsl_FragmentShFlat, glslExpr.c_str(), glslExpr.length()+1);
		strncpy(arbOutP->Glsl_VertexShFlat, glvertstr.c_str(), glvertstr.length() + 1);
        strncpy( arbOutP->descriptionAcFlat, descriptionStr.c_str(), descriptionStr.length()+1);
#endif

        //delete \nfor execution expr
        redResultStr.erase(std::remove(redResultStr.begin(), redResultStr.end(), '\n'), redResultStr.end());
        greenResultStr.erase(std::remove(greenResultStr.begin(), greenResultStr.end(), '\n'), greenResultStr.end());
        blueResultStr.erase(std::remove(blueResultStr.begin(), blueResultStr.end(), '\n'), blueResultStr.end());
        alphaResultStr.erase(std::remove(alphaResultStr.begin(), alphaResultStr.end(), '\n'), alphaResultStr.end());
		glslExpr.erase(std::remove(glslExpr.begin(), glslExpr.end(), '\t'), glslExpr.end());
        

        descriptionStr.erase(std::remove(descriptionStr.begin(), descriptionStr.end(), '\n'), descriptionStr.end());

#ifdef AE_OS_WIN
        strncpy_s( arbOutP->redExAc, redResultStr.c_str(), redResultStr.length()+1);
        strncpy_s(arbOutP->greenExAc, greenResultStr.c_str(), greenResultStr.length()+1);
        strncpy_s( arbOutP->blueExAc, blueResultStr.c_str(), blueResultStr.length()+1);
        strncpy_s( arbOutP->alphaExAc, alphaResultStr.c_str(), alphaResultStr.length()+1);
        strncpy_s( arbOutP->presetNameAc, presetNameStr.c_str(), presetNameStr.length()+1);
        strncpy_s( arbOutP->Glsl_FragmentShAc, glslExpr.c_str(), glslExpr.length()+1);
		strncpy_s(arbOutP->Glsl_VertexShAc, glvertstr.c_str(), glvertstr.length() + 1);
        strncpy_s( arbOutP->descriptionAc, descriptionStr.c_str(), descriptionStr.length()+1);
#else
        strncpy( arbOutP->redExAc, redResultStr.c_str(), redResultStr.length()+1);
        strncpy( arbOutP->greenExAc, greenResultStr.c_str(), greenResultStr.length()+1);
        strncpy( arbOutP->blueExAc, blueResultStr.c_str(), blueResultStr.length()+1);
        strncpy( arbOutP->alphaExAc, alphaResultStr.c_str(), alphaResultStr.length()+1);
        strncpy( arbOutP->presetNameAc, presetNameStr.c_str(), presetNameStr.length()+1);
        strncpy( arbOutP->Glsl_FragmentShAc, glslExpr.c_str(), glslExpr.length()+1);
		strncpy(arbOutP->Glsl_VertexShAc, glvertstr.c_str(), glvertstr.length() + 1);
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



