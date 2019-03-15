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
    A_char          scriptAC[70000] { '\0' };
    std::string Majvers = std::to_string(MAJOR_VERSION);
    std::string MinVers = std::to_string(MINOR_VERSION);
    std::string Bugvers = std::to_string(BUG_VERSION);

    //ARB
    PF_ParamDef arb_param;

    //strings to send expr to script
    std::string inputName;
    std::string inputDescription;
    std::string inputGlsl;
    std::string inputRedS;
    std::string inputGreenS;
    std::string inputBlueS;
    std::string inputAlphaS;

	PF_Boolean inputuiSliderGrpVisibleB;
	std::string inputuiSliderGrpNameS;
	PF_Boolean inputuiSlider1VisibleB;
	std::string inputuiSlider1NameS;
	PF_Boolean inputuiSlider2VisibleB;
	std::string inputuiSlider2NameS;
	PF_Boolean inputuiSlider3VisibleB;
	std::string   inputuiSlider3NameS;
	PF_Boolean inputuiSlider4VisibleB;
	std::string 	inputuiSlider4NameS;
	PF_Boolean inputuiPointGrpVisibleB;
	std::string 	inputuiPointGrpNameS;
	PF_Boolean inputuiPoint1VisibleB;
	std::string 	inputuiPoint1NameS;
	PF_Boolean inputuiPoint2VisibleB;
	std::string 	inputuiPoint2NameS;
	PF_Boolean inputuiColorGrpVisibleB;
	std::string 	inputuiColorGrpNameS;
	PF_Boolean inputuiColor1VisibleB;
	std::string 	inputuiColor1NameS;
	PF_Boolean inputuiColor2VisibleB;
	std::string inputuiColor2NameS;
	PF_Boolean inputextLGrpVisibleB;
	std::string inputextLGrpNameS;

	PF_Boolean		inputParserModeB;



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
			inputParserModeB =arbInP->parserModeB;
			inputRedS.append(arbInP->redExAcFlat);
			inputGreenS.append(arbInP->greenExAcFlat);
			inputBlueS.append(arbInP->blueExAcFlat);
			inputAlphaS.append(arbInP->alphaExAcFlat);
			inputName.append(arbInP->presetNameAc);
			inputGlsl.append(arbInP->Glsl_FragmentShFlat);
			inputDescription.append(arbInP->descriptionAcFlat);
			inputuiSliderGrpVisibleB=(arbInP->uiSliderGrpB);
			inputuiSliderGrpNameS.append(arbInP->uiSliderGrp_NameAC);
			inputuiSlider1VisibleB=arbInP->uiSliderOneB;
			inputuiSlider1NameS.append(arbInP->uiSliderOne_NameAC);
			inputuiSlider2VisibleB = arbInP->uiSliderTwoB;
			inputuiSlider2NameS.append(arbInP->uiSliderTwo_NameAC);
			inputuiSlider3VisibleB = arbInP->uiSliderThreeB;
			inputuiSlider3NameS.append(arbInP->uiSliderThree_NameAC);
			inputuiSlider4VisibleB= arbInP->uiSliderFourB;
			inputuiSlider4NameS.append(arbInP->uiSliderFour_NameAC);
			inputuiPointGrpVisibleB = arbInP->uiPointGrpB;
			inputuiPointGrpNameS.append(arbInP->uiPointGrp_NameAC);
			inputuiPoint1VisibleB= arbInP->uiPointOneB;
			inputuiPoint1NameS.append(arbInP->uiPointOne_NameAC);
			inputuiPoint2VisibleB= arbInP->uiPointTwoB;
			inputuiPoint2NameS.append(arbInP->uiPointTwo_NameAC);
			inputuiColorGrpVisibleB=arbInP->uiColorGrpB;
			inputuiColorGrpNameS.append(arbInP->uiColorGrp_NameAC);
			inputuiColor1VisibleB= arbInP->uiColorOneB;
			inputuiColor1NameS.append(arbInP->uiColorOne_NameAC);
			inputuiColor2VisibleB= arbInP->uiColorTwoB;
			inputuiColor2NameS.append(arbInP->uiColorTwo_NameAC);
			inputextLGrpVisibleB=arbInP->uiExtLGrpB;
			inputextLGrpNameS.append(arbInP->uiExtLGrp_NameAC);
        }

    }
    //to force the parser to keep \n before to send it to js
    strReplace(inputRedS, "\n", "\\n");
    strReplace(inputGreenS, "\n", "\\n");
    strReplace(inputBlueS, "\n", "\\n");
    strReplace(inputAlphaS, "\n", "\\n");
    strReplace(inputDescription, "\n", "\\n");
    strReplace(inputGlsl, "\n" , "\\n");
	if (inputGlsl == "fragSh") {
	inputGlsl = glfrag1str;
	strReplace(inputGlsl, "\n", "\\n");
	}

	

    nlohmann::json  jToJs;

	jToJs["parserModeB"] = inputParserModeB;
	jToJs["presetName"] = inputName;
	jToJs["description"] = inputDescription;
	jToJs["redExpr"] = inputRedS;
	jToJs["greenExpr"] = inputGreenS;
	jToJs["blueExpr"] = inputBlueS;
	jToJs["alphaExpr"] = inputAlphaS;
	jToJs["glslExpr"] = inputGlsl;
	jToJs["uiSliderGrpVisible"] = inputuiSliderGrpVisibleB;
	jToJs["uiSliderGrpName"] = inputuiSliderGrpNameS;
	jToJs["uiSlider1Visible"] = inputuiSlider1VisibleB;
	jToJs["uiSlider1Name"] = inputuiSlider1NameS;
	jToJs["uiSlider2Visible"] = inputuiSlider2VisibleB;
	jToJs["uiSlider2Name"] = inputuiSlider2NameS;
	jToJs["uiSlider3Visible"] = inputuiSlider3VisibleB;
	jToJs["uiSlider3Name"] = inputuiSlider3NameS;
	jToJs["uiSlider4Visible"] = inputuiSlider4VisibleB;
	jToJs["uiSlider4Name"] = inputuiSlider4NameS;
	jToJs["uiPointGrpVisible"] = inputuiPointGrpVisibleB;
	jToJs["uiPointGrpName"] = inputuiPointGrpNameS;
	jToJs["uiPoint1Visible"] = inputuiPoint1VisibleB;
	jToJs["uiPoint1Name"] = inputuiPoint1NameS;
	jToJs["uiPoint2Visible"] = inputuiPoint2VisibleB;
	jToJs["uiPoint2Name"] = inputuiPoint2NameS;
	jToJs["uiColorGrpVisible"] = inputuiColorGrpVisibleB;
	jToJs["uiColorGrpName"] = inputuiColorGrpNameS;
	jToJs["uiColor1Visible"] = inputuiColor1VisibleB;
	jToJs["uiColor1Name"] = inputuiColor1NameS;
	jToJs["uiColor2Visible"] = inputuiColor2VisibleB;
	jToJs["uiColor2Name"] = inputuiColor2NameS;
	jToJs["extLGrpVisible"] = inputextLGrpVisibleB;
	jToJs["extLGrpName"] = inputextLGrpNameS;

	std::string resultStr;
	bool scriptLoopEvalB = true;

	while (scriptLoopEvalB) {
		std::string jsonDump = "'''";
		jsonDump.append(jToJs.dump());
		jsonDump.append("'''");


		sprintf(scriptAC,
            script_ae.c_str(),
			jsonDump.c_str(),
			Majvers.c_str(),
			MinVers.c_str(),
			Bugvers.c_str());

		ERR(suites.UtilitySuite6()->AEGP_ExecuteScript(globP->my_id, scriptAC, FALSE, &resultMemH, NULL));
		//AEGP SETSTREAMVALUR TO ARB
		AEFX_CLR_STRUCT(resultAC);
		ERR(suites.MemorySuite1()->AEGP_LockMemHandle(resultMemH, reinterpret_cast<void**>(&resultAC)));
		if (resultAC) {
			resultStr = resultAC;
		}
		ERR(suites.MemorySuite1()->AEGP_FreeMemHandle(resultMemH));
		nlohmann::json jeval = nlohmann::json::parse(resultStr);
		bool evalB = jeval["/evalB"_json_pointer];
		if (evalB == false) {
			scriptLoopEvalB = false;
		}
		else {
			bool ParserModeB = jeval["/parserModeB"_json_pointer];
			if (ParserModeB == true) {
				std::string errReturn = "NONE";
                std::string glslEvalExpr= jeval["/glslExpr"_json_pointer];
				strReplace(glslEvalExpr, "\t", "    ");
				evalFragShader(glslEvalExpr, errReturn);
				strReplace(errReturn, "\n", "\\n");
				strReplace(glslEvalExpr, "\n", "\\n");
				jToJs["evalglslExp"] = errReturn;
				jToJs["glslExpr"] = glslEvalExpr;
				jToJs["parserModeB"] = ParserModeB;
			}
		}
	}
		AEFX_CLR_STRUCT(arbOutP);
		arbOutP = reinterpret_cast<m_ArbData*>(*arb_param.u.arb_d.value);

		//set result per channel
		

		nlohmann::json  jresult = nlohmann::json::parse(resultStr);
		bool ParserModeB, SLIDERGRPB, INPONEB, INPTWOB, INPTHREEB, INPFOURB,
			POINTGRPB, POINT_ONEB, POINT_TWOB, COLORGRPB, COLOR_ONEB, COLOR_TWOB, EXTLGRPB;

	

		ParserModeB = jresult["/parserModeB"_json_pointer];
		//AEGP SETSTREAMVALUE TO ARB
        AEFX_CLR_STRUCT(arbOutP);
        arbOutP = reinterpret_cast<m_ArbData*>(*arb_param.u.arb_d.value);
		SLIDERGRPB =jresult["/uiSliderGrpVisible"_json_pointer];
		INPONEB = jresult["/uiSlider1Visible"_json_pointer];
		INPTWOB= jresult["/uiSlider2Visible"_json_pointer];
		INPTHREEB = jresult["/uiSlider3Visible"_json_pointer];
		INPFOURB = jresult["/uiSlider4Visible"_json_pointer];
		POINTGRPB = jresult["/uiPointGrpVisible"_json_pointer];
		POINT_ONEB = jresult["/uiPoint1Visible"_json_pointer];
		POINT_TWOB = jresult["/uiPoint2Visible"_json_pointer];
		COLORGRPB = jresult["/uiColorGrpVisible"_json_pointer];
		COLOR_ONEB = jresult["/uiColor1Visible"_json_pointer];
		COLOR_TWOB = jresult["/uiColor2Visible"_json_pointer];
		EXTLGRPB = jresult["/extLGrpVisible"_json_pointer];

		std::string redResultStr = jresult["/redExpr"_json_pointer];
		ExprtkCorrectorStr(redResultStr);
		std::string greenResultStr = jresult["/greenExpr"_json_pointer];
		ExprtkCorrectorStr(greenResultStr);
		std::string blueResultStr = jresult["/blueExpr"_json_pointer];
		ExprtkCorrectorStr(blueResultStr);
		std::string alphaResultStr = jresult["/alphaExpr"_json_pointer];
		ExprtkCorrectorStr(alphaResultStr);
		std::string glslExpr = jresult["/glslExpr"_json_pointer];
		strReplace(glslExpr, "\t", "    ");

		
		//bool to PF_Boolean
		arbOutP->uiSliderGrpB = SLIDERGRPB;
		arbOutP->uiSliderOneB = INPONEB;
		arbOutP->uiSliderTwoB = INPTWOB;
		arbOutP->uiSliderThreeB = INPTHREEB;
		arbOutP->uiSliderFourB = INPFOURB;
		arbOutP->uiPointGrpB = POINTGRPB;
		arbOutP->uiPointOneB = POINT_ONEB;
		arbOutP->uiPointTwoB = POINT_TWOB;
		arbOutP->uiColorGrpB = COLORGRPB;
		arbOutP->uiColorOneB = COLOR_ONEB;
		arbOutP->uiColorTwoB = COLOR_TWOB;
		arbOutP->uiExtLGrpB = EXTLGRPB;
		arbOutP->parserModeB = ParserModeB;
		if (ParserModeB) {
			if ((hasString(resultStr, std::string("time"))) ||
				(hasString(resultStr, std::string("iTime"))) ||
				(hasString(resultStr, std::string("extLayerTex")))) {
				arbOutP->PresetHasWideInputB = true;
			}
			else {
				arbOutP->PresetHasWideInputB = false;
			}
			arbOutP->PixelsCallExternalInputB = hasString(resultStr, std::string("extLayerTex"));
		}
		else {
			if ((hasString(resultStr, std::string("extL")))|| (hasString(resultStr, std::string("layerTime")))) {
				arbOutP->PresetHasWideInputB = true;
			}
			else {
				arbOutP->PresetHasWideInputB = false;
			}
			arbOutP->PixelsCallExternalInputB = hasString(resultStr, std::string("extL"));
			arbOutP->NeedsPixelAroundB = hasString(resultStr, std::string("vec9_"));
			arbOutP->NeedsLumaB = hasString(resultStr, std::string("in_luma"));
		}
		arbOutP->CallsAEGP_CompB = hasString(resultStr, std::string("layer"));
		arbOutP->CallsAEGP_layerB = hasString(resultStr, std::string("comp"));


		
        std::string presetNameStr = jresult["/presetName"_json_pointer];
        std::string descriptionStr = jresult["/description"_json_pointer];

		std::string SLIDERGRPS= jresult["/uiSliderGrpName"_json_pointer];
		std::string INPONES  = jresult["/uiSlider1Name"_json_pointer];
		std::string INPTWOS  = jresult["/uiSlider2Name"_json_pointer];
		std::string INPTHREES = jresult["/uiSlider3Name"_json_pointer];
		std::string INPFOURS= jresult["/uiSlider4Name"_json_pointer];
		std::string POINTGRPS = jresult["/uiPointGrpName"_json_pointer];
		std::string  POINT_ONES = jresult["/uiPoint1Name"_json_pointer];
		std::string POINT_TWOS= jresult["/uiPoint2Name"_json_pointer];
		std::string COLORGRPS = jresult["/uiColorGrpName"_json_pointer];
		std::string COLOR_ONES = jresult["/uiColor1Name"_json_pointer];
		std::string COLOR_TWOS = jresult["/uiColor2Name"_json_pointer];
		std::string EXTLGRPS= jresult["/extLGrpName"_json_pointer];

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
        strncpy_s( arbOutP->Glsl_FragmentShAc, glslExpr.c_str(), glslExpr.length()+1);
		strncpy_s(arbOutP->Glsl_VertexShAc, glvertstr.c_str(), glvertstr.length() + 1);
        strncpy_s( arbOutP->descriptionAc, descriptionStr.c_str(), descriptionStr.length()+1);
		strncpy_s(arbOutP->presetNameAc, presetNameStr.c_str(), presetNameStr.length() + 1);
		strncpy_s(arbOutP->uiSliderGrp_NameAC, SLIDERGRPS.c_str(), SLIDERGRPS.length() + 1);
		strncpy_s(arbOutP->uiSliderOne_NameAC, INPONES.c_str(), INPONES.length() + 1);
		strncpy_s(arbOutP->uiSliderTwo_NameAC, INPTWOS.c_str(), INPTWOS.length() + 1);
		strncpy_s(arbOutP->uiSliderThree_NameAC, INPTHREES.c_str(), INPTHREES.length() + 1);
		strncpy_s(arbOutP->uiSliderFour_NameAC, INPFOURS.c_str(), INPFOURS.length() + 1);
		strncpy_s(arbOutP->uiPointGrp_NameAC, POINTGRPS.c_str(), POINTGRPS.length() + 1);
		strncpy_s(arbOutP->uiPointOne_NameAC, POINT_ONES.c_str(), POINT_ONES.length() + 1);
		strncpy_s(arbOutP->uiPointTwo_NameAC, POINT_TWOS.c_str(), POINT_TWOS.length() + 1);
		strncpy_s(arbOutP->uiColorGrp_NameAC, COLORGRPS.c_str(), COLORGRPS.length() + 1);
		strncpy_s(arbOutP->uiColorOne_NameAC, COLOR_ONES.c_str(), COLOR_ONES.length() + 1);
		strncpy_s(arbOutP->uiColorTwo_NameAC, COLOR_TWOS.c_str(), COLOR_TWOS.length() + 1);
		strncpy_s(arbOutP->uiExtLGrp_NameAC, EXTLGRPS.c_str(), EXTLGRPS.length() + 1);

#else
        strncpy( arbOutP->redExAc, redResultStr.c_str(), redResultStr.length()+1);
        strncpy( arbOutP->greenExAc, greenResultStr.c_str(), greenResultStr.length()+1);
        strncpy( arbOutP->blueExAc, blueResultStr.c_str(), blueResultStr.length()+1);
        strncpy( arbOutP->alphaExAc, alphaResultStr.c_str(), alphaResultStr.length()+1);
        strncpy( arbOutP->Glsl_FragmentShAc, glslExpr.c_str(), glslExpr.length()+1);
		strncpy(arbOutP->Glsl_VertexShAc, glvertstr.c_str(), glvertstr.length() + 1);
        strncpy( arbOutP->descriptionAc, descriptionStr.c_str(), descriptionStr.length()+1);
		strncpy(arbOutP->presetNameAc, presetNameStr.c_str(), presetNameStr.length() + 1);
		strncpy(arbOutP->uiSliderGrp_NameAC, SLIDERGRPS.c_str(), SLIDERGRPS.length() + 1);
		strncpy(arbOutP->uiSliderOne_NameAC, INPONES.c_str(), INPONES.length() + 1);
		strncpy(arbOutP->uiSliderTwo_NameAC, INPTWOS.c_str(), INPTWOS.length() + 1);
		strncpy(arbOutP->uiSliderThree_NameAC, INPTHREES.c_str(), INPTHREES.length() + 1);
		strncpy(arbOutP->uiSliderFour_NameAC, INPFOURS.c_str(), INPFOURS.length() + 1);
		strncpy(arbOutP->uiPointGrp_NameAC, POINTGRPS.c_str(), POINTGRPS.length() + 1);
		strncpy(arbOutP->uiPointOne_NameAC, POINT_ONES.c_str(), POINT_ONES.length() + 1);
		strncpy(arbOutP->uiPointTwo_NameAC, POINT_TWOS.c_str(), POINT_TWOS.length() + 1);
		strncpy(arbOutP->uiColorGrp_NameAC, COLORGRPS.c_str(), COLORGRPS.length() + 1);
		strncpy(arbOutP->uiColorOne_NameAC, COLOR_ONES.c_str(), COLOR_ONES.length() + 1);
		strncpy(arbOutP->uiColorTwo_NameAC, COLOR_TWOS.c_str(), COLOR_TWOS.length() + 1);
		strncpy(arbOutP->uiExtLGrp_NameAC, EXTLGRPS.c_str(), EXTLGRPS.length() + 1);

#endif
		if (redResultStr.compare(arbOutP->redExAc) !=0 ||
			greenResultStr.compare(arbOutP->greenExAc) != 0 ||
			blueResultStr.compare(arbOutP->blueExAc) != 0 ||
			alphaResultStr.compare(arbOutP->alphaExAc) != 0) {
			arbOutP->ExprResetB = true;
		}
		else { arbOutP->ExprResetB = false; }
        arbOutH = reinterpret_cast <PF_Handle>(arbOutP);
        ERR (AEGP_SetParamStreamValue(in_data, out_data, globP->my_id, MATH_ARB_DATA, &arbOutH));
        PF_UNLOCK_HANDLE(arbOutH);


    ERR(PF_CHECKIN_PARAM(in_data, &arb_param));
    
    out_data->out_flags |= PF_OutFlag_DISPLAY_ERROR_MESSAGE |
    PF_OutFlag_FORCE_RERENDER;
    return err;
}



