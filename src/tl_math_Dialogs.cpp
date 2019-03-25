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


static void arbToJson (m_ArbData        *arbInP,
                       nlohmann::json& Jsondata)
{
    std::string inputName;
    std::string inputDescription;
    std::string inputGlsl;
    std::string inputRedS;
    std::string inputGreenS;
    std::string inputBlueS;
    std::string inputAlphaS;

    std::string inputuiSliderGrpNameS;
    std::string inputuiSlider1NameS;
    std::string inputuiSlider2NameS;
    std::string   inputuiSlider3NameS;
    std::string     inputuiSlider4NameS;
    std::string     inputuiPointGrpNameS;
    std::string     inputuiPoint1NameS;
    std::string     inputuiPoint2NameS;
    std::string     inputuiColorGrpNameS;
    std::string     inputuiColor1NameS;
    std::string inputuiColor2NameS;
    std::string inputextLGrpNameS;

    PF_Boolean inputuiSliderGrpVisibleB;
    PF_Boolean inputuiSlider1VisibleB;
    PF_Boolean inputuiSlider2VisibleB;
    PF_Boolean inputuiSlider3VisibleB;
    PF_Boolean inputuiSlider4VisibleB;
    PF_Boolean inputuiPointGrpVisibleB;
    PF_Boolean inputuiPoint1VisibleB;
    PF_Boolean inputuiPoint2VisibleB;
    PF_Boolean inputuiColorGrpVisibleB;
    PF_Boolean inputuiColor1VisibleB;
    PF_Boolean inputuiColor2VisibleB;
    PF_Boolean inputextLGrpVisibleB;
    PF_Boolean inputParserModeB;

    inputParserModeB =arbInP->parserModeB;
    inputRedS.append(arbInP->redExAcFlat);
    inputGreenS.append(arbInP->greenExAcFlat);
    inputBlueS.append(arbInP->blueExAcFlat);
    inputAlphaS.append(arbInP->alphaExAcFlat);
    inputGlsl.append(arbInP->Glsl_FragmentShFlat);
    inputDescription.append(arbInP->descriptionAcFlat);

     inputName.append(arbInP->presetNameAc);

    inputuiSliderGrpNameS.append(arbInP->uiSliderGrp_NameAC);
    inputuiSlider1NameS.append(arbInP->uiSliderOne_NameAC);
    inputuiSlider2NameS.append(arbInP->uiSliderTwo_NameAC);
    inputuiSlider3NameS.append(arbInP->uiSliderThree_NameAC);
    inputuiSlider4NameS.append(arbInP->uiSliderFour_NameAC);
    inputuiPointGrpNameS.append(arbInP->uiPointGrp_NameAC);
    inputuiPoint1NameS.append(arbInP->uiPointOne_NameAC);
    inputuiPoint2NameS.append(arbInP->uiPointTwo_NameAC);
    inputuiColorGrpNameS.append(arbInP->uiColorGrp_NameAC);
    inputuiColor1NameS.append(arbInP->uiColorOne_NameAC);
    inputuiColor2NameS.append(arbInP->uiColorTwo_NameAC);
    inputuiSliderGrpVisibleB=(arbInP->uiSliderGrpB);
    inputuiSlider1VisibleB=arbInP->uiSliderOneB;
    inputuiSlider2VisibleB = arbInP->uiSliderTwoB;
    inputuiSlider3VisibleB = arbInP->uiSliderThreeB;
    inputuiSlider4VisibleB= arbInP->uiSliderFourB;
    inputuiPointGrpVisibleB = arbInP->uiPointGrpB;
    inputuiPoint1VisibleB= arbInP->uiPointOneB;
    inputuiPoint2VisibleB= arbInP->uiPointTwoB;
    inputuiColorGrpVisibleB=arbInP->uiColorGrpB;
    inputuiColor1VisibleB= arbInP->uiColorOneB;
    inputuiColor2VisibleB= arbInP->uiColorTwoB;
    inputextLGrpVisibleB=arbInP->uiExtLGrpB;
    inputextLGrpNameS.append(arbInP->uiExtLGrp_NameAC);


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
    Jsondata["parserModeB"] = inputParserModeB;
    Jsondata["presetName"] = inputName;
    Jsondata["description"] = inputDescription;
    Jsondata["redExpr"] = inputRedS;
    Jsondata["greenExpr"] = inputGreenS;
    Jsondata["blueExpr"] = inputBlueS;
    Jsondata["alphaExpr"] = inputAlphaS;
    Jsondata["glslExpr"] = inputGlsl;
    Jsondata["uiSliderGrpVisible"] = inputuiSliderGrpVisibleB;
    Jsondata["uiSliderGrpName"] = inputuiSliderGrpNameS;
    Jsondata["uiSlider1Visible"] = inputuiSlider1VisibleB;
    Jsondata["uiSlider1Name"] = inputuiSlider1NameS;
    Jsondata["uiSlider2Visible"] = inputuiSlider2VisibleB;
    Jsondata["uiSlider2Name"] = inputuiSlider2NameS;
    Jsondata["uiSlider3Visible"] = inputuiSlider3VisibleB;
    Jsondata["uiSlider3Name"] = inputuiSlider3NameS;
    Jsondata["uiSlider4Visible"] = inputuiSlider4VisibleB;
    Jsondata["uiSlider4Name"] = inputuiSlider4NameS;
    Jsondata["uiPointGrpVisible"] = inputuiPointGrpVisibleB;
    Jsondata["uiPointGrpName"] = inputuiPointGrpNameS;
    Jsondata["uiPoint1Visible"] = inputuiPoint1VisibleB;
    Jsondata["uiPoint1Name"] = inputuiPoint1NameS;
    Jsondata["uiPoint2Visible"] = inputuiPoint2VisibleB;
    Jsondata["uiPoint2Name"] = inputuiPoint2NameS;
    Jsondata["uiColorGrpVisible"] = inputuiColorGrpVisibleB;
    Jsondata["uiColorGrpName"] = inputuiColorGrpNameS;
    Jsondata["uiColor1Visible"] = inputuiColor1VisibleB;
    Jsondata["uiColor1Name"] = inputuiColor1NameS;
    Jsondata["uiColor2Visible"] = inputuiColor2VisibleB;
    Jsondata["uiColor2Name"] = inputuiColor2NameS;
    Jsondata["extLGrpVisible"] = inputextLGrpVisibleB;
    Jsondata["extLGrpName"] = inputextLGrpNameS;

}

static void jsonStrToArb (std::string resultStr,
                         m_ArbData    *arbOutP)
{
    nlohmann::json  jresult = nlohmann::json::parse(resultStr);
    bool ParserModeB, SLIDERGRPB, INPONEB, INPTWOB, INPTHREEB, INPFOURB,
    POINTGRPB, POINT_ONEB, POINT_TWOB, COLORGRPB, COLOR_ONEB, COLOR_TWOB, EXTLGRPB;



    ParserModeB = jresult["/parserModeB"_json_pointer];

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
    std::string greenResultStr = jresult["/greenExpr"_json_pointer];
    std::string blueResultStr = jresult["/blueExpr"_json_pointer];
    std::string alphaResultStr = jresult["/alphaExpr"_json_pointer];


    ExprtkCorrectorStr(redResultStr);
    ExprtkCorrectorStr(greenResultStr);
    ExprtkCorrectorStr(blueResultStr);
    ExprtkCorrectorStr(alphaResultStr);


    std::string glslExpr = jresult["/glslExpr"_json_pointer];
    strReplace(glslExpr, "\"", " '");
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


    //check the flags to change
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
        arbOutP->NeedsPixelAroundB = hasString(resultStr, std::string("_off"));
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
}

static void jsonSendError (std::string  errorMessage,
                           m_ArbData    *arbOutP){

    if (arbOutP->parserModeB){
        #ifdef AE_OS_WIN
        strncpy_s(arbOutP->Glsl_FragmentShAc, glErrorMessageStr.c_str(), glErrorMessageStr.length()+1);
        #else
        strncpy(arbOutP->Glsl_FragmentShAc, glErrorMessageStr.c_str(), glErrorMessageStr.length()+1);
        #endif
    }else{
         #ifdef AE_OS_WIN
         strncpy_s( arbOutP->alphaExAc,  exprErrorMessageStr.c_str(), exprErrorMessageStr.length()+1);
         strncpy_s(arbOutP->redExAc,  exprErrorMessageStr.c_str(), exprErrorMessageStr.length()+1);
         strncpy_s(arbOutP->greenExAc,  exprErrorMessageStr.c_str(), exprErrorMessageStr.length()+1);
         strncpy_s(arbOutP->blueExAc,    exprErrorMessageStr.c_str(), exprErrorMessageStr.length()+1);
        #else
        strncpy( arbOutP->alphaExAc,  exprErrorMessageStr.c_str(), exprErrorMessageStr.length()+1);
        strncpy(arbOutP->redExAc,  exprErrorMessageStr.c_str(), exprErrorMessageStr.length()+1);
        strncpy(arbOutP->greenExAc,  exprErrorMessageStr.c_str(), exprErrorMessageStr.length()+1);
        strncpy(arbOutP->blueExAc,    exprErrorMessageStr.c_str(), exprErrorMessageStr.length()+1);

        #endif
    }

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
void ExprtkCorrectorStr(std::string &str) {
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
    nlohmann::json  jToJs;

    if (!err){
        AEFX_CLR_STRUCT(arbInP);
        arbInP = reinterpret_cast<m_ArbData*>(*arb_param.u.arb_d.value);
        if (arbInP){
            arbToJson (arbInP,jToJs);
        }

    }

	std::string resultStr;
	bool scriptLoopEvalB = true;
	bool hasErrorB = false;
    std::string errReturn = "NONE";

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

			std::string glslEvalExpr = jeval["/glslExpr"_json_pointer];
			bool ParserModeB = jeval["/parserModeB"_json_pointer];
			std::string redResultStr = jeval["/redExpr"_json_pointer];
			std::string greenResultStr = jeval["/greenExpr"_json_pointer];
			std::string blueResultStr = jeval["/blueExpr"_json_pointer];
			std::string alphaResultStr = jeval["/alphaExpr"_json_pointer];


			if (ParserModeB == true) {
				
				strReplace(glslEvalExpr, "\t", "    ");
				strReplace(glslEvalExpr, "\"", " '");
				evalFragShader(glslEvalExpr, errReturn);
				strReplace(errReturn, "\n", "\\n");
				strReplace(glslEvalExpr, "\n", "\\n");
                
				jToJs["evalglslExp"] = errReturn;
				jToJs["glslExpr"] = glslEvalExpr;
				jToJs["parserModeB"] = ParserModeB;

                strReplace(redResultStr, "\n", "\\n");
                strReplace(greenResultStr, "\n", "\\n");
                strReplace(blueResultStr, "\n", "\\n");
                strReplace(alphaResultStr, "\n", "\\n");
                jToJs["redExpr"] = redResultStr;
                jToJs["greenExpr"] = greenResultStr;
                jToJs["blueExpr"] = blueResultStr;
                jToJs["alphaExpr"] = alphaResultStr;

			}
			else {

				ExprtkCorrectorStr(redResultStr);
				ExprtkCorrectorStr(greenResultStr);
				ExprtkCorrectorStr(blueResultStr);
				ExprtkCorrectorStr(alphaResultStr);

                errReturn= evalMathExprStr (redResultStr,greenResultStr,blueResultStr,alphaResultStr);

				strReplace(redResultStr, "\n", "\\n");
				strReplace(greenResultStr, "\n", "\\n");
				strReplace(blueResultStr, "\n", "\\n");
				strReplace(alphaResultStr, "\n", "\\n");
				strReplace(errReturn, "\n", "\\n");
				strReplace(glslEvalExpr, "\n", "\\n");
				jToJs["glslExpr"] = glslEvalExpr;
				jToJs["parserModeB"] = ParserModeB;
				jToJs["redExpr"] = redResultStr;
				jToJs["greenExpr"] = greenResultStr;
				jToJs["blueExpr"] = blueResultStr;
				jToJs["alphaExpr"] = alphaResultStr;
				jToJs["evalmathExp"] = errReturn;


			}
			if (errReturn != compile_success){
				hasErrorB = true;
            }else{
                hasErrorB = false;
            }
		}
	}
		AEFX_CLR_STRUCT(arbOutP);
		arbOutP = reinterpret_cast<m_ArbData*>(*arb_param.u.arb_d.value);
        jsonStrToArb (resultStr, arbOutP);
    if (hasErrorB){
        jsonSendError (errReturn,arbOutP);
        suites.ANSICallbacksSuite1()->sprintf(out_data->return_msg,
                                              errReturn.c_str());

        }


        if (std::string(arbOutP->Glsl_FragmentShFlat).compare(std::string(arbInP->Glsl_FragmentShFlat)) !=0)  {
        arbOutP->ShaderResetB = true;
        }

        arbOutH = reinterpret_cast <PF_Handle>(arbOutP);
        //AEGP SETSTREAMVALUE TO ARB
        ERR (AEGP_SetParamStreamValue(in_data, out_data, globP->my_id, MATH_ARB_DATA, &arbOutH));
        PF_UNLOCK_HANDLE(arbOutH);


    ERR(PF_CHECKIN_PARAM(in_data, &arb_param));
    
    out_data->out_flags |= PF_OutFlag_DISPLAY_ERROR_MESSAGE |
    PF_OutFlag_FORCE_RERENDER;
    return err;
}



