//
//  tl_mathSeqData.cpp
//  tlMath
//
// 
//


#include    "tl_math.h"

static void ExprtkCorrectorStr(std::string &str)
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
static void scriptCorrectorStr(std::string &str)
{
    strReplace(str, "\\n", "\n");
    strReplace(str, "\\r", "\r");
    strReplace(str, "\\t", "  ");
    strReplace(str, "\\'", "\'");
}


static std::string  copyStrFromJsonToSeqData(nlohmann::json arbDataJS,
								  std::string json_adress)
{
	nlohmann::json::json_pointer jpointer(json_adress);
	std::string dataStr = arbDataJS[jpointer].get<std::string>();
	return dataStr;
}

static bool getBoolFromJsonToSeqData(nlohmann::json arbDataJS,
	std::string json_adress)
{
	nlohmann::json::json_pointer jpointer(json_adress);
	bool dataBoolB = arbDataJS[jpointer].get<bool>();
	return dataBoolB;
}


static std::string  copyExprFromJsonToSeqData(nlohmann::json arbDataJS,
												std::string json_adress)
{
	nlohmann::json::json_pointer jpointer(json_adress);
	std::string dataStr = arbDataJS[jpointer].get<std::string>();
	ExprtkCorrectorStr(dataStr);
	return dataStr;
}
static std::string  copyShaderFromJsonToSeqData(nlohmann::json arbDataJS,
	std::string json_adress)
{
	nlohmann::json::json_pointer jpointer(json_adress);
	std::string dataStr = arbDataJS[jpointer].get<std::string>();
	scriptCorrectorStr(dataStr);
	return dataStr;
}

PF_Err
copyFromArbToSeqData(PF_InData        *in_data,
                     PF_OutData        *out_data,
                     std::string       arbStr,
                     seqData           *seqDataP)
{
    PF_Err err = PF_Err_NONE;
    nlohmann::json arbDataJS;
    try{
        arbDataJS = nlohmann::json::parse(arbStr);
    }
    catch ( nlohmann::json::exception& e)
    {
        std::string errMess =  e.what();
        AEGP_SuiteHandler    suites(in_data->pica_basicP);
        suites.ANSICallbacksSuite1()->sprintf(    out_data->return_msg,
                                              errMess.c_str());
        return PF_Err_INTERNAL_STRUCT_DAMAGED;
    }
    //copy expressions
	//rewrite the size delimiter 
	copyStrFromJsonToSeqData(arbDataJS, "/effectInfo/presetName").copy(seqDataP->presetNameAc, sizeof(seqDataP->presetNameAc));
	copyStrFromJsonToSeqData(arbDataJS, "/effectInfo/description").copy(seqDataP->descriptionAc, sizeof(seqDataP->descriptionAc));
	copyExprFromJsonToSeqData(arbDataJS, "/math_expression/redExpr").copy(seqDataP->redExAc, sizeof(seqDataP->redExAc));
	copyExprFromJsonToSeqData(arbDataJS, "/math_expression/greenExpr").copy(seqDataP->greenExAc, sizeof(seqDataP->greenExAc));
	copyExprFromJsonToSeqData(arbDataJS, "/math_expression/blueExpr").copy(seqDataP->blueExAc, sizeof(seqDataP->blueExAc));
	copyExprFromJsonToSeqData(arbDataJS, "/math_expression/alphaExpr").copy(seqDataP->alphaExAc, sizeof(seqDataP->alphaExAc));
	copyExprFromJsonToSeqData(arbDataJS, "/math_expression/rgbExpr").copy(seqDataP->rgbExprExAc, sizeof(seqDataP->rgbExprExAc));

    std::string curr_fragSh = seqDataP->Glsl33_FragmentShAc;
    std::string curr_vertSh = seqDataP->Glsl33_VertexShAc;
    std::string  new_frag = copyShaderFromJsonToSeqData(arbDataJS, "/gl_expression/gl33_frag_sh");
    std::string  new_vert = copyShaderFromJsonToSeqData (arbDataJS,"/gl_expression/gl33_vert_sh");
    if (curr_fragSh.compare(new_frag) != 0 || curr_vertSh.compare(new_vert) != 0)
    {
        seqDataP->resetShaderB = true;
        //copy shaders
         new_frag.copy(seqDataP->Glsl33_FragmentShAc, sizeof(seqDataP->Glsl33_FragmentShAc));
         new_vert.copy(seqDataP->Glsl33_VertexShAc, sizeof(seqDataP->Glsl33_VertexShAc));
    }
    else {
        seqDataP->resetShaderB = false;
    }


     //copy expressions params name
	copyStrFromJsonToSeqData(arbDataJS, "/math_expression/expr_current_channel").copy(seqDataP->expr_ColorChNameAc, sizeof(seqDataP->expr_ColorChNameAc));
	copyStrFromJsonToSeqData(arbDataJS, "/math_expression/expr_pix").copy(seqDataP->expr_pixNameAc, sizeof(seqDataP->expr_pixNameAc));
	copyStrFromJsonToSeqData(arbDataJS, "/math_expression/expr_luma").copy(seqDataP->expr_lumaNameAc, sizeof(seqDataP->expr_lumaNameAc));
	copyStrFromJsonToSeqData(arbDataJS, "/math_expression/expr_red_off").copy(seqDataP->expr_red_offNameAc, sizeof(seqDataP->expr_red_offNameAc));
	copyStrFromJsonToSeqData(arbDataJS, "/math_expression/expr_green_off").copy(seqDataP->expr_green_offNameAc, sizeof(seqDataP->expr_green_offNameAc));
	copyStrFromJsonToSeqData(arbDataJS, "/math_expression/expr_blue_off").copy(seqDataP->expr_blue_offNameAc, sizeof(seqDataP->expr_blue_offNameAc));
	copyStrFromJsonToSeqData(arbDataJS, "/math_expression/expr_alpha_off").copy(seqDataP->expr_alpha_offNameAc, sizeof(seqDataP->expr_alpha_offNameAc));
    // copy compositions params
	copyStrFromJsonToSeqData(arbDataJS, "/composition/camera_position").copy(seqDataP->cameraPosNameAc, sizeof(seqDataP->cameraPosNameAc));
	copyStrFromJsonToSeqData(arbDataJS, "/composition/camera_target").copy(seqDataP->cameraTargetNameAc, sizeof(seqDataP->cameraTargetNameAc));
	copyStrFromJsonToSeqData(arbDataJS, "/composition/camera_zoom").copy(seqDataP->cameraZoomNameAc, sizeof(seqDataP->cameraZoomNameAc));
	copyStrFromJsonToSeqData(arbDataJS, "/composition/camera_rotation").copy(seqDataP->cameraRotationNameAc, sizeof(seqDataP->cameraRotationNameAc));
	copyStrFromJsonToSeqData(arbDataJS, "/composition/resolution").copy(seqDataP->resolutionNameAc, sizeof(seqDataP->resolutionNameAc));
	copyStrFromJsonToSeqData(arbDataJS, "/composition/layerPosition").copy(seqDataP->layerPositionNameAc, sizeof(seqDataP->layerPositionNameAc));
	copyStrFromJsonToSeqData(arbDataJS, "/composition/layerScale").copy(seqDataP->layerGrpNameAc, sizeof(seqDataP->layerScaleNameAc));
	copyStrFromJsonToSeqData(arbDataJS, "/composition/compResolution").copy(seqDataP->compResolutionNameAc, sizeof(seqDataP->compResolutionNameAc));
	copyStrFromJsonToSeqData(arbDataJS, "/composition/time_sec").copy(seqDataP->time_secNameAc, sizeof(seqDataP->time_secNameAc));
	copyStrFromJsonToSeqData(arbDataJS, "/composition/time_frame").copy(seqDataP->time_frameNameAc, sizeof(seqDataP->time_frameNameAc));
	copyStrFromJsonToSeqData(arbDataJS, "/composition/frame_rate").copy(seqDataP->frame_rateNameAc, sizeof(seqDataP->frame_rateNameAc));
	 
    //copy flags
    seqDataP->glsl33ModeB =getBoolFromJsonToSeqData (arbDataJS, "/effectMode/gl33_modeB");
    seqDataP->exprModeB =  getBoolFromJsonToSeqData (arbDataJS, "/effectMode/expr_modeB");
    seqDataP->evalModeB = getBoolFromJsonToSeqData (arbDataJS, "/effectMode/evalModeB");
    seqDataP->exprRGBModeB = getBoolFromJsonToSeqData (arbDataJS, "/math_expression/exprRGBModeB");
    seqDataP->needsPixelAroundB =getBoolFromJsonToSeqData (arbDataJS,"/flags/needsPixelAroundB");
    seqDataP->pixelsCallExternalInputB = getBoolFromJsonToSeqData (arbDataJS, "/flags/pixelsCallExternalInputB");
    seqDataP->needsLumaB = getBoolFromJsonToSeqData (arbDataJS, "/flags/needsLumaB");
    seqDataP->presetHasWideInputB = getBoolFromJsonToSeqData (arbDataJS, "/flags/presetHasWideInputB");
    seqDataP->cameraB = getBoolFromJsonToSeqData (arbDataJS, "/flags/usesCameraB");
    //copy layers
    copyStrFromJsonToSeqData(arbDataJS, "/gui_settings/layerGrp/grpName").copy(seqDataP->layerGrpNameAc, sizeof(seqDataP->layerGrpNameAc));
    copyStrFromJsonToSeqData(arbDataJS, "/gui_settings/layerGrp/current_layer/name").copy(seqDataP->paramLayer00NameAc, sizeof(seqDataP->layerGrpNameAc));
    copyStrFromJsonToSeqData(arbDataJS, "/gui_settings/layerGrp/extLayer_1/name").copy(seqDataP->paramLayer01NameAc, sizeof(seqDataP->layerGrpNameAc));
    seqDataP->layerGrpVisibleB = getBoolFromJsonToSeqData (arbDataJS, "/gui_settings/layerGrp/grpVisibleB");
    seqDataP->paramLayer01VisibleB = getBoolFromJsonToSeqData (arbDataJS, "/gui_settings/layerGrp/extLayer_1/visibleB");
    //copy params
    seqDataP->sliderGrpVisibleB  = getBoolFromJsonToSeqData (arbDataJS, "/gui_settings/sliderGrp/grpVisibleB");
    copyStrFromJsonToSeqData(arbDataJS, "/gui_settings/sliderGrp/grpName").copy(seqDataP->sliderGrpNameAc, sizeof(seqDataP->sliderGrpNameAc));
    seqDataP->paramSlider01VisibleB = getBoolFromJsonToSeqData(arbDataJS,"/gui_settings/sliderGrp/params/0/visibleB");
    copyStrFromJsonToSeqData(arbDataJS, "/gui_settings/sliderGrp/params/0/name").copy(seqDataP->paramSlider01NameAc, sizeof(seqDataP->paramSlider01NameAc));
    seqDataP->paramSlider02VisibleB = getBoolFromJsonToSeqData(arbDataJS, "/gui_settings/sliderGrp/params/1/visibleB");
    copyStrFromJsonToSeqData(arbDataJS, "/gui_settings/sliderGrp/params/1/name").copy(seqDataP->paramSlider02NameAc, sizeof(seqDataP->paramSlider02NameAc));
    seqDataP->paramSlider03VisibleB = getBoolFromJsonToSeqData(arbDataJS, "/gui_settings/sliderGrp/params/2/visibleB");
    copyStrFromJsonToSeqData(arbDataJS, "/gui_settings/sliderGrp/params/2/name").copy(seqDataP->paramSlider03NameAc, sizeof(seqDataP->paramSlider03NameAc));
    seqDataP->paramSlider04VisibleB = getBoolFromJsonToSeqData(arbDataJS, "/gui_settings/sliderGrp/params/3/visibleB");
    copyStrFromJsonToSeqData(arbDataJS, "/gui_settings/sliderGrp/params/3/name").copy(seqDataP->paramSlider04NameAc, sizeof(seqDataP->paramSlider04NameAc));
    seqDataP->paramSlider05VisibleB = getBoolFromJsonToSeqData(arbDataJS, "/gui_settings/sliderGrp/params/4/visibleB");;
    copyStrFromJsonToSeqData(arbDataJS, "/gui_settings/sliderGrp/params/4/name").copy(seqDataP->paramSlider05NameAc, sizeof(seqDataP->paramSlider05NameAc));
    seqDataP->paramSlider06VisibleB = getBoolFromJsonToSeqData(arbDataJS, "/gui_settings/sliderGrp/params/5/visibleB");
    copyStrFromJsonToSeqData(arbDataJS, "/gui_settings/sliderGrp/params/5/name").copy(seqDataP->paramSlider06NameAc, sizeof(seqDataP->paramSlider06NameAc));
    seqDataP->paramSlider07VisibleB = getBoolFromJsonToSeqData(arbDataJS, "/gui_settings/sliderGrp/params/6/visibleB");
    copyStrFromJsonToSeqData(arbDataJS, "/gui_settings/sliderGrp/params/6/name").copy(seqDataP->paramSlider07NameAc, sizeof(seqDataP->paramSlider07NameAc));
    seqDataP->paramSlider08VisibleB = getBoolFromJsonToSeqData(arbDataJS, "/gui_settings/sliderGrp/params/7/visibleB");
    copyStrFromJsonToSeqData(arbDataJS, "/gui_settings/sliderGrp/params/7/name").copy(seqDataP->paramSlider08NameAc, sizeof(seqDataP->paramSlider08NameAc));
    seqDataP->paramSlider09VisibleB = getBoolFromJsonToSeqData(arbDataJS, "/gui_settings/sliderGrp/params/8/visibleB");
    copyStrFromJsonToSeqData(arbDataJS, "/gui_settings/sliderGrp/params/8/name").copy(seqDataP->paramSlider09NameAc, sizeof(seqDataP->paramSlider09NameAc));
    seqDataP->paramSlider10VisibleB = getBoolFromJsonToSeqData(arbDataJS, "/gui_settings/sliderGrp/params/9/visibleB");
    copyStrFromJsonToSeqData(arbDataJS, "/gui_settings/sliderGrp/params/9/name").copy(seqDataP->paramSlider10NameAc, sizeof(seqDataP->paramSlider10NameAc));

    seqDataP->pointGrpVisibleB  = getBoolFromJsonToSeqData (arbDataJS, "/gui_settings/pointGrp/grpVisibleB");
    copyStrFromJsonToSeqData(arbDataJS, "/gui_settings/pointGrp/grpName").copy(seqDataP->pointGrpNameAc, sizeof(seqDataP->pointGrpNameAc));
    seqDataP->paramPoint01VisibleB = getBoolFromJsonToSeqData(arbDataJS, "/gui_settings/pointGrp/params/0/visibleB");
    copyStrFromJsonToSeqData(arbDataJS, "/gui_settings/pointGrp/params/0/name").copy(seqDataP->paramPoint01NameAc, sizeof(seqDataP->paramPoint01NameAc));
    seqDataP->paramPoint02VisibleB =    getBoolFromJsonToSeqData(arbDataJS, "/gui_settings/pointGrp/params/1/visibleB");
    copyStrFromJsonToSeqData(arbDataJS, "/gui_settings/pointGrp/params/1/name").copy(seqDataP->paramPoint02NameAc, sizeof(seqDataP->paramPoint02NameAc));
    seqDataP->paramPoint03VisibleB =    getBoolFromJsonToSeqData(arbDataJS, "/gui_settings/pointGrp/params/2/visibleB");
    copyStrFromJsonToSeqData(arbDataJS, "/gui_settings/pointGrp/params/2/name").copy(seqDataP->paramPoint03NameAc, sizeof(seqDataP->paramPoint03NameAc));
    seqDataP->paramPoint04VisibleB =    getBoolFromJsonToSeqData(arbDataJS, "/gui_settings/pointGrp/params/3/visibleB");
    copyStrFromJsonToSeqData(arbDataJS, "/gui_settings/pointGrp/params/3/name").copy(seqDataP->paramPoint04NameAc, sizeof(seqDataP->paramPoint04NameAc));
    seqDataP->paramPoint05VisibleB =    getBoolFromJsonToSeqData(arbDataJS, "/gui_settings/pointGrp/params/4/visibleB");
    copyStrFromJsonToSeqData(arbDataJS, "/gui_settings/pointGrp/params/4/name").copy(seqDataP->paramPoint05NameAc, sizeof(seqDataP->paramPoint05NameAc));
    seqDataP->paramPoint06VisibleB =    getBoolFromJsonToSeqData(arbDataJS, "/gui_settings/pointGrp/params/5/visibleB");
    copyStrFromJsonToSeqData(arbDataJS, "/gui_settings/pointGrp/params/5/name").copy(seqDataP->paramPoint06NameAc, sizeof(seqDataP->paramPoint06NameAc));
    seqDataP->paramPoint07VisibleB =    getBoolFromJsonToSeqData(arbDataJS, "/gui_settings/pointGrp/params/6/visibleB");
    copyStrFromJsonToSeqData(arbDataJS, "/gui_settings/pointGrp/params/6/name").copy(seqDataP->paramPoint07NameAc, sizeof(seqDataP->paramPoint07NameAc));
    seqDataP->paramPoint08VisibleB =    getBoolFromJsonToSeqData(arbDataJS, "/gui_settings/pointGrp/params/7/visibleB");
    copyStrFromJsonToSeqData(arbDataJS, "/gui_settings/pointGrp/params/7/name").copy(seqDataP->paramPoint08NameAc, sizeof(seqDataP->paramPoint08NameAc));
    seqDataP->paramPoint09VisibleB =    getBoolFromJsonToSeqData(arbDataJS, "/gui_settings/pointGrp/params/8/visibleB");
    copyStrFromJsonToSeqData(arbDataJS, "/gui_settings/pointGrp/params/8/name").copy(seqDataP->paramPoint09NameAc, sizeof(seqDataP->paramPoint09NameAc));
    seqDataP->paramPoint10VisibleB =    getBoolFromJsonToSeqData(arbDataJS, "/gui_settings/pointGrp/params/9/visibleB");
    copyStrFromJsonToSeqData(arbDataJS, "/gui_settings/pointGrp/params/9/name").copy(seqDataP->paramPoint10NameAc, sizeof(seqDataP->paramPoint10NameAc));

    seqDataP->cbGrpVisibleB  = getBoolFromJsonToSeqData (arbDataJS, "/gui_settings/cboxGrp/grpVisibleB");
    copyStrFromJsonToSeqData(arbDataJS, "/gui_settings/cboxGrp/grpName").copy(seqDataP->cbGrpNameAc, sizeof(seqDataP->cbGrpNameAc));
    seqDataP->paramCb01VisibleB =    getBoolFromJsonToSeqData(arbDataJS, "/gui_settings/cboxGrp/params/0/visibleB");
    copyStrFromJsonToSeqData(arbDataJS, "/gui_settings/cboxGrp/params/0/name").copy(seqDataP->paramCb01NameAc, sizeof(seqDataP->paramCb01NameAc));
    seqDataP->paramCb02VisibleB =    getBoolFromJsonToSeqData(arbDataJS, "/gui_settings/cboxGrp/params/1/visibleB");
    copyStrFromJsonToSeqData(arbDataJS, "/gui_settings/cboxGrp/params/1/name").copy(seqDataP->paramCb02NameAc, sizeof(seqDataP->paramCb02NameAc));
    seqDataP->paramCb03VisibleB =    getBoolFromJsonToSeqData(arbDataJS, "/gui_settings/cboxGrp/params/2/visibleB");
    copyStrFromJsonToSeqData(arbDataJS, "/gui_settings/cboxGrp/params/2/name").copy(seqDataP->paramCb03NameAc, sizeof(seqDataP->paramCb03NameAc));
    seqDataP->paramCb04VisibleB =    getBoolFromJsonToSeqData(arbDataJS, "/gui_settings/cboxGrp/params/3/visibleB");
    copyStrFromJsonToSeqData(arbDataJS, "/gui_settings/cboxGrp/params/3/name").copy(seqDataP->paramCb04NameAc, sizeof(seqDataP->paramCb04NameAc));
    seqDataP->paramCb05VisibleB =    getBoolFromJsonToSeqData(arbDataJS, "/gui_settings/cboxGrp/params/4/visibleB");
    copyStrFromJsonToSeqData(arbDataJS, "/gui_settings/cboxGrp/params/4/name").copy(seqDataP->paramCb05NameAc, sizeof(seqDataP->paramCb05NameAc));
    seqDataP->paramCb06VisibleB =    getBoolFromJsonToSeqData(arbDataJS, "/gui_settings/cboxGrp/params/5/visibleB");
    copyStrFromJsonToSeqData(arbDataJS, "/gui_settings/cboxGrp/params/5/name").copy(seqDataP->paramCb06NameAc, sizeof(seqDataP->paramCb06NameAc));
    seqDataP->paramCb07VisibleB =    getBoolFromJsonToSeqData(arbDataJS, "/gui_settings/cboxGrp/params/6/visibleB");
    copyStrFromJsonToSeqData(arbDataJS, "/gui_settings/cboxGrp/params/6/name").copy(seqDataP->paramCb07NameAc, sizeof(seqDataP->paramCb07NameAc));
    seqDataP->paramCb08VisibleB =    getBoolFromJsonToSeqData(arbDataJS, "/gui_settings/cboxGrp/params/7/visibleB");
    copyStrFromJsonToSeqData(arbDataJS, "/gui_settings/cboxGrp/params/7/name").copy(seqDataP->paramCb08NameAc, sizeof(seqDataP->paramCb08NameAc));
    seqDataP->paramCb09VisibleB =    getBoolFromJsonToSeqData(arbDataJS, "/gui_settings/cboxGrp/params/8/visibleB");
    copyStrFromJsonToSeqData(arbDataJS, "/gui_settings/cboxGrp/params/8/name").copy(seqDataP->paramCb09NameAc, sizeof(seqDataP->paramCb09NameAc));
    seqDataP->paramCb10VisibleB =    getBoolFromJsonToSeqData(arbDataJS, "/gui_settings/cboxGrp/params/9/visibleB");
    copyStrFromJsonToSeqData(arbDataJS, "/gui_settings/cboxGrp/params/9/name").copy(seqDataP->paramCb10NameAc, sizeof(seqDataP->paramCb10NameAc));

    seqDataP->colorGrpVisibleB  = getBoolFromJsonToSeqData (arbDataJS, "/gui_settings/colorGrp/grpVisibleB");
    copyStrFromJsonToSeqData(arbDataJS, "/gui_settings/colorGrp/grpName").copy(seqDataP->colorGrpNameAc, sizeof(seqDataP->colorGrpNameAc));
    seqDataP->paramColor01VisibleB =    getBoolFromJsonToSeqData(arbDataJS, "/gui_settings/colorGrp/params/0/visibleB");
    copyStrFromJsonToSeqData(arbDataJS, "/gui_settings/colorGrp/params/0/name").copy(seqDataP->paramColor01NameAc, sizeof(seqDataP->paramColor01NameAc));
    seqDataP->paramColor02VisibleB =    getBoolFromJsonToSeqData(arbDataJS, "/gui_settings/colorGrp/params/1/visibleB");
    copyStrFromJsonToSeqData(arbDataJS, "/gui_settings/colorGrp/params/1/name").copy(seqDataP->paramColor02NameAc, sizeof(seqDataP->paramColor02NameAc));
    seqDataP->paramColor03VisibleB =    getBoolFromJsonToSeqData(arbDataJS, "/gui_settings/colorGrp/params/2/visibleB");
    copyStrFromJsonToSeqData(arbDataJS, "/gui_settings/colorGrp/params/2/name").copy(seqDataP->paramColor03NameAc, sizeof(seqDataP->paramColor03NameAc));
    seqDataP->paramColor04VisibleB =    getBoolFromJsonToSeqData(arbDataJS, "/gui_settings/colorGrp/params/3/visibleB");
    copyStrFromJsonToSeqData(arbDataJS, "/gui_settings/colorGrp/params/3/name").copy(seqDataP->paramColor04NameAc, sizeof(seqDataP->paramColor04NameAc));
    seqDataP->paramColor05VisibleB =    getBoolFromJsonToSeqData(arbDataJS, "/gui_settings/colorGrp/params/4/visibleB");
    copyStrFromJsonToSeqData(arbDataJS, "/gui_settings/colorGrp/params/4/name").copy(seqDataP->paramColor05NameAc, sizeof(seqDataP->paramColor05NameAc));
    seqDataP->paramColor06VisibleB =    getBoolFromJsonToSeqData(arbDataJS, "/gui_settings/colorGrp/params/5/visibleB");
    copyStrFromJsonToSeqData(arbDataJS, "/gui_settings/colorGrp/params/5/name").copy(seqDataP->paramColor06NameAc, sizeof(seqDataP->paramColor06NameAc));
    seqDataP->paramColor07VisibleB =    getBoolFromJsonToSeqData(arbDataJS, "/gui_settings/colorGrp/params/6/visibleB");
    copyStrFromJsonToSeqData(arbDataJS, "/gui_settings/colorGrp/params/6/name").copy(seqDataP->paramColor07NameAc, sizeof(seqDataP->paramColor07NameAc));
    seqDataP->paramColor08VisibleB =    getBoolFromJsonToSeqData(arbDataJS, "/gui_settings/colorGrp/params/7/visibleB");
    copyStrFromJsonToSeqData(arbDataJS, "/gui_settings/colorGrp/params/7/name").copy(seqDataP->paramColor08NameAc, sizeof(seqDataP->paramColor08NameAc));
    seqDataP->paramColor09VisibleB =    getBoolFromJsonToSeqData(arbDataJS, "/gui_settings/colorGrp/params/8/visibleB");
    copyStrFromJsonToSeqData(arbDataJS, "/gui_settings/colorGrp/params/8/name").copy(seqDataP->paramColor09NameAc, sizeof(seqDataP->paramColor09NameAc));
    seqDataP->paramColor10VisibleB =    getBoolFromJsonToSeqData(arbDataJS, "/gui_settings/colorGrp/params/9/visibleB");
    copyStrFromJsonToSeqData(arbDataJS, "/gui_settings/colorGrp/params/9/name").copy(seqDataP->paramColor10NameAc, sizeof(seqDataP->paramColor10NameAc));
    
    return err;
}

PF_Err
tlmath_updateSeqData(PF_InData            *in_data,
                     PF_OutData            *out_data,
                     PF_ParamDef            *params[])
{
    PF_Err                err = PF_Err_NONE;
    AEGP_SuiteHandler        suites(in_data->pica_basicP);
    PF_ParamDef arb_param;
    PF_Handle    seq_dataH = suites.HandleSuite1()->host_new_handle(sizeof(seqData));
    m_ArbData        *arbInP = NULL;
    std::string arbDataStr;
    AEFX_CLR_STRUCT(arb_param);
    ERR(PF_CHECKOUT_PARAM(in_data,
                          MATH_ARB_DATA,
                          in_data->current_time,
                          in_data->time_step,
                          in_data->time_scale,
                          &arb_param));

    AEFX_CLR_STRUCT(arbInP);
    arbInP = reinterpret_cast<m_ArbData*>(*arb_param.u.arb_d.value);
    if (!arbInP) {
        err = PF_Err_OUT_OF_MEMORY;
    }
    else {
        arbDataStr = arbInP->arbDataAc;
    }
    if (seq_dataH && !err) {
        seqData      *seqP = reinterpret_cast<seqData*>(suites.HandleSuite1()->host_lock_handle(seq_dataH));
        if (seqP->initializedB == false) {
            copyFromArbToSeqData(in_data, out_data,arbDataStr, seqP);
            seqP->initializedB = true;
            out_data->sequence_data = seq_dataH;

        }
        suites.HandleSuite1()->host_unlock_handle(seq_dataH);
    }
    else {    // whoa, we couldn't allocate sequence data; bail!
        err = PF_Err_OUT_OF_MEMORY;
    }
    return err;
}


PF_Err
tlMath_SequenceSetdown (
                        PF_InData        *in_data,
                        PF_OutData        *out_data)
{
    PF_Err err = PF_Err_NONE;

    if (in_data->sequence_data){
        AEGP_SuiteHandler suites(in_data->pica_basicP);
        suites.HandleSuite1()->host_dispose_handle(in_data->sequence_data);
    }
    return err;
}

PF_Err
tlMath_SequenceSetup (
                      PF_InData        *in_data,
                      PF_OutData        *out_data)
{
    PF_Err err = PF_Err_NONE;
    AEGP_SuiteHandler suites(in_data->pica_basicP);
    err = tlMath_SequenceSetdown(in_data, out_data);

    if (!err){
        PF_Handle    seq_dataH =    suites.HandleSuite1()->host_new_handle(sizeof(seqData));

        if (seq_dataH){
            seqData      *seqP = reinterpret_cast<seqData*>(suites.HandleSuite1()->host_lock_handle(seq_dataH));
            seqP->initializedB = false;
            copyFromArbToSeqData(in_data, out_data, defaultArb, seqP);
            out_data->sequence_data = seq_dataH;
            suites.HandleSuite1()->host_unlock_handle(seq_dataH);
        } else {    // whoa, we couldn't allocate sequence data; bail!
            err = PF_Err_OUT_OF_MEMORY;
        }
    }
    return err;
}
