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

static bool getBoolFromJsonToSeqData(nlohmann::json arbDataJS,
									std::string json_adress)
{
	nlohmann::json::json_pointer jpointer(json_adress);
	bool dataBoolB = arbDataJS[jpointer].get<bool>();
	return dataBoolB;
}

static std::string getStringFromJsonAdress(nlohmann::json arbDataJS,
                                           std::string json_adress,
                                           A_char* target)
{
     //we input the target. in case of error in json pointer
	std::string dataStr;
	AEFX_CLR_STRUCT(dataStr);
	nlohmann::json::json_pointer jpointer(json_adress);
    try{
        dataStr = arbDataJS[jpointer].get<std::string>();
        }catch (nlohmann::json::exception& e){
            dataStr = target;
        }
	return dataStr;
}

static PF_FpLong getFloatFromJsonAdress(nlohmann::json arbDataJS,
                                    std::string json_adress,
                                    PF_FpLong  target)
{
    //we input the target. in case of error in json pointer
    PF_FpLong  dataF;
    nlohmann::json::json_pointer jpointer(json_adress);
    try{
        dataF = (PF_FpLong) arbDataJS[jpointer].get<float>();
    }catch (nlohmann::json::exception& e){
         dataF= target;
    }
    return  dataF;
}

static void copyStrFromJsonToSeqData(nlohmann::json arbDataJS,
								  std::string json_adress,
								  char* target)
{
	std::string dataStr = getStringFromJsonAdress(arbDataJS, json_adress, target);
	std::size_t length = dataStr.copy(target, dataStr.size());
	target[length] = '\0';
}


static void   copyExprFromJsonToSeqData(nlohmann::json arbDataJS,
												std::string json_adress,
												A_char* target)
{
	std::string dataStr = getStringFromJsonAdress(arbDataJS, json_adress, target);
	ExprtkCorrectorStr(dataStr);
	std::size_t length = dataStr.copy(target, dataStr.size());
	target[length] = '\0';
}

PF_Err
tlmath_updateParamsValue(PF_ParamDef     *params[],
                         std::string     arbStr)
{
    PF_Err err = PF_Err_NONE;
    nlohmann::json arbDataJS;
    try {
        arbDataJS = nlohmann::json::parse(arbStr);
    }
    catch (nlohmann::json::exception& e)
    {
        return PF_Err_INTERNAL_STRUCT_DAMAGED;
    }
    params[MATH_SLIDER_ONE]->u.fs_d.value = getFloatFromJsonAdress(arbDataJS, "/gui_settings/sliderGrp/params/0/defaultVal/0", params[MATH_SLIDER_ONE]->u.fs_d.value);
    params[MATH_SLIDER_ONE]->uu.change_flags = PF_ChangeFlag_CHANGED_VALUE;
    params[MATH_SLIDER_TWO]->u.fs_d.value = getFloatFromJsonAdress(arbDataJS, "/gui_settings/sliderGrp/params/1/defaultVal/0", params[MATH_SLIDER_TWO]->u.fs_d.value);
    params[MATH_SLIDER_TWO]->uu.change_flags = PF_ChangeFlag_CHANGED_VALUE;
    params[MATH_SLIDER_THREE]->u.fs_d.value = getFloatFromJsonAdress(arbDataJS, "/gui_settings/sliderGrp/params/2/defaultVal/0", params[MATH_SLIDER_THREE]->u.fs_d.value);
    params[MATH_SLIDER_THREE]->uu.change_flags = PF_ChangeFlag_CHANGED_VALUE;
    params[MATH_SLIDER_FOUR]->u.fs_d.value = getFloatFromJsonAdress(arbDataJS, "/gui_settings/sliderGrp/params/3/defaultVal/0", params[MATH_SLIDER_FOUR]->u.fs_d.value);
    params[MATH_SLIDER_FOUR]->uu.change_flags = PF_ChangeFlag_CHANGED_VALUE;
    params[MATH_SLIDER_FIVE]->u.fs_d.value = getFloatFromJsonAdress(arbDataJS, "/gui_settings/sliderGrp/params/4/defaultVal/0", params[MATH_SLIDER_FIVE]->u.fs_d.value);
    params[MATH_SLIDER_FIVE]->uu.change_flags = PF_ChangeFlag_CHANGED_VALUE;
    params[MATH_SLIDER_SIX]->u.fs_d.value = getFloatFromJsonAdress(arbDataJS, "/gui_settings/sliderGrp/params/5/defaultVal/0", params[MATH_SLIDER_SIX]->u.fs_d.value);
    params[MATH_SLIDER_SIX]->uu.change_flags = PF_ChangeFlag_CHANGED_VALUE;
    params[MATH_SLIDER_SEVEN]->u.fs_d.value = getFloatFromJsonAdress(arbDataJS, "/gui_settings/sliderGrp/params/6/defaultVal/0", params[MATH_SLIDER_SEVEN]->u.fs_d.value);
    params[MATH_SLIDER_SEVEN]->uu.change_flags = PF_ChangeFlag_CHANGED_VALUE;
    params[MATH_SLIDER_HEIGHT]->u.fs_d.value = getFloatFromJsonAdress(arbDataJS, "/gui_settings/sliderGrp/params/7/defaultVal/0", params[MATH_SLIDER_HEIGHT]->u.fs_d.value);
    params[MATH_SLIDER_HEIGHT]->uu.change_flags = PF_ChangeFlag_CHANGED_VALUE;
    params[MATH_SLIDER_NINE]->u.fs_d.value = getFloatFromJsonAdress(arbDataJS, "/gui_settings/sliderGrp/params/8/defaultVal/0", params[MATH_SLIDER_NINE]->u.fs_d.value);
    params[MATH_SLIDER_NINE]->uu.change_flags = PF_ChangeFlag_CHANGED_VALUE;
    params[MATH_SLIDER_TEN]->u.fs_d.value = getFloatFromJsonAdress(arbDataJS, "/gui_settings/sliderGrp/params/9/defaultVal/0", params[MATH_SLIDER_TEN]->u.fs_d.value);
    params[MATH_SLIDER_TEN]->uu.change_flags = PF_ChangeFlag_CHANGED_VALUE;
    
    return err;

}

PF_Err
copyFromArbToSeqData(PF_InData* in_data,
	PF_OutData* out_data,
	std::string       arbStr,
	seqData* seqDataP)
{
	PF_Err err = PF_Err_NONE;
	nlohmann::json arbDataJS;
	try {
		arbDataJS = nlohmann::json::parse(arbStr);
	}
	catch (nlohmann::json::exception& e)
	{
		std::string errMess = e.what();
		AEGP_SuiteHandler    suites(in_data->pica_basicP);
		suites.ANSICallbacksSuite1()->sprintf(out_data->return_msg,
			errMess.c_str());
		return PF_Err_INTERNAL_STRUCT_DAMAGED;
	}
	//copy expressions
	//rewrite the size delimiter 
	copyStrFromJsonToSeqData(arbDataJS, "/effectInfo/presetName", seqDataP->presetNameAc);
	copyStrFromJsonToSeqData(arbDataJS, "/effectInfo/description", seqDataP->descriptionAc);

	copyExprFromJsonToSeqData(arbDataJS, "/math_expression/redExpr", seqDataP->redExAc);
	copyExprFromJsonToSeqData(arbDataJS, "/math_expression/greenExpr", seqDataP->greenExAc);
	copyExprFromJsonToSeqData(arbDataJS, "/math_expression/blueExpr", seqDataP->blueExAc);
	copyExprFromJsonToSeqData(arbDataJS, "/math_expression/alphaExpr", seqDataP->alphaExAc);
	copyExprFromJsonToSeqData(arbDataJS, "/math_expression/rgbExpr", seqDataP->rgbExprExAc);

	std::string curr_fragSh = seqDataP->Glsl33_FragmentShAc;
	std::string curr_vertSh = seqDataP->Glsl33_VertexShAc;
	std::string  new_frag = getStringFromJsonAdress(arbDataJS, "/gl_expression/gl33_frag_sh", seqDataP->Glsl33_FragmentShAc);
	std::string  new_vert = getStringFromJsonAdress(arbDataJS, "/gl_expression/gl33_vert_sh",seqDataP->Glsl33_VertexShAc);
	scriptCorrectorStr(new_frag);
	scriptCorrectorStr(new_vert);
    if (curr_fragSh.compare(new_frag) != 0 || curr_vertSh.compare(new_vert) != 0)
    {
        seqDataP->resetShaderB = true;
        //copy shaders
		std::size_t  fragLength = new_frag.copy(seqDataP->Glsl33_FragmentShAc, new_frag.length());
		seqDataP->Glsl33_FragmentShAc[fragLength] = '\0';
		std::size_t  vertLength = new_vert.copy(seqDataP->Glsl33_VertexShAc, new_vert.length());
		 seqDataP->Glsl33_VertexShAc[vertLength] = '\0';
    }
    else {
        seqDataP->resetShaderB = false;
    }
     //copy expressions params name
	copyStrFromJsonToSeqData(arbDataJS, "/math_expression/expr_current_channel",seqDataP->expr_ColorChNameAc);
	copyStrFromJsonToSeqData(arbDataJS, "/math_expression/expr_pix",seqDataP->expr_pixNameAc);
	copyStrFromJsonToSeqData(arbDataJS, "/math_expression/expr_luma",seqDataP->expr_lumaNameAc);
	copyStrFromJsonToSeqData(arbDataJS, "/math_expression/expr_red_off",seqDataP->expr_red_offNameAc);
	copyStrFromJsonToSeqData(arbDataJS, "/math_expression/expr_green_off",seqDataP->expr_green_offNameAc);
	copyStrFromJsonToSeqData(arbDataJS, "/math_expression/expr_blue_off",seqDataP->expr_blue_offNameAc);
	copyStrFromJsonToSeqData(arbDataJS, "/math_expression/expr_alpha_off",seqDataP->expr_alpha_offNameAc);
    // copy compositions params
	copyStrFromJsonToSeqData(arbDataJS, "/composition/camera_position",seqDataP->cameraPosNameAc);
	copyStrFromJsonToSeqData(arbDataJS, "/composition/camera_target",seqDataP->cameraTargetNameAc);
	copyStrFromJsonToSeqData(arbDataJS, "/composition/camera_zoom",seqDataP->cameraZoomNameAc);
	copyStrFromJsonToSeqData(arbDataJS, "/composition/camera_rotation",seqDataP->cameraRotationNameAc);
	copyStrFromJsonToSeqData(arbDataJS, "/composition/resolution",seqDataP->resolutionNameAc);
	copyStrFromJsonToSeqData(arbDataJS, "/composition/layerPosition",seqDataP->layerPositionNameAc);
	copyStrFromJsonToSeqData(arbDataJS, "/composition/layerScale",seqDataP->layerGrpNameAc);
	copyStrFromJsonToSeqData(arbDataJS, "/composition/compResolution",seqDataP->compResolutionNameAc);
	copyStrFromJsonToSeqData(arbDataJS, "/composition/time_sec",seqDataP->time_secNameAc);
	copyStrFromJsonToSeqData(arbDataJS, "/composition/time_frame",seqDataP->time_frameNameAc);
	copyStrFromJsonToSeqData(arbDataJS, "/composition/frame_rate",seqDataP->frame_rateNameAc);
	 
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
    copyStrFromJsonToSeqData(arbDataJS, "/gui_settings/layerGrp/grpName",seqDataP->layerGrpNameAc);
    copyStrFromJsonToSeqData(arbDataJS, "/gui_settings/layerGrp/current_layer/name",seqDataP->paramLayer00NameAc);
    copyStrFromJsonToSeqData(arbDataJS, "/gui_settings/layerGrp/extLayer_1/name",seqDataP->paramLayer01NameAc);
    seqDataP->layerGrpVisibleB = getBoolFromJsonToSeqData (arbDataJS, "/gui_settings/layerGrp/grpVisibleB");
    seqDataP->paramLayer01VisibleB = getBoolFromJsonToSeqData (arbDataJS, "/gui_settings/layerGrp/extLayer_1/visibleB");
    //copy params
    seqDataP->sliderGrpVisibleB  = getBoolFromJsonToSeqData (arbDataJS, "/gui_settings/sliderGrp/grpVisibleB");
    copyStrFromJsonToSeqData(arbDataJS, "/gui_settings/sliderGrp/grpName",seqDataP->sliderGrpNameAc);
    seqDataP->paramSlider01VisibleB = getBoolFromJsonToSeqData(arbDataJS,"/gui_settings/sliderGrp/params/0/visibleB");
    copyStrFromJsonToSeqData(arbDataJS, "/gui_settings/sliderGrp/params/0/name",seqDataP->paramSlider01NameAc);
    seqDataP->paramSlider02VisibleB = getBoolFromJsonToSeqData(arbDataJS, "/gui_settings/sliderGrp/params/1/visibleB");
    copyStrFromJsonToSeqData(arbDataJS, "/gui_settings/sliderGrp/params/1/name",seqDataP->paramSlider02NameAc);
    seqDataP->paramSlider03VisibleB = getBoolFromJsonToSeqData(arbDataJS, "/gui_settings/sliderGrp/params/2/visibleB");
    copyStrFromJsonToSeqData(arbDataJS, "/gui_settings/sliderGrp/params/2/name",seqDataP->paramSlider03NameAc);
    seqDataP->paramSlider04VisibleB = getBoolFromJsonToSeqData(arbDataJS, "/gui_settings/sliderGrp/params/3/visibleB");
    copyStrFromJsonToSeqData(arbDataJS, "/gui_settings/sliderGrp/params/3/name",seqDataP->paramSlider04NameAc);
    seqDataP->paramSlider05VisibleB = getBoolFromJsonToSeqData(arbDataJS, "/gui_settings/sliderGrp/params/4/visibleB");;
    copyStrFromJsonToSeqData(arbDataJS, "/gui_settings/sliderGrp/params/4/name",seqDataP->paramSlider05NameAc);
    seqDataP->paramSlider06VisibleB = getBoolFromJsonToSeqData(arbDataJS, "/gui_settings/sliderGrp/params/5/visibleB");
    copyStrFromJsonToSeqData(arbDataJS, "/gui_settings/sliderGrp/params/5/name",seqDataP->paramSlider06NameAc);
    seqDataP->paramSlider07VisibleB = getBoolFromJsonToSeqData(arbDataJS, "/gui_settings/sliderGrp/params/6/visibleB");
    copyStrFromJsonToSeqData(arbDataJS, "/gui_settings/sliderGrp/params/6/name",seqDataP->paramSlider07NameAc);
    seqDataP->paramSlider08VisibleB = getBoolFromJsonToSeqData(arbDataJS, "/gui_settings/sliderGrp/params/7/visibleB");
    copyStrFromJsonToSeqData(arbDataJS, "/gui_settings/sliderGrp/params/7/name",seqDataP->paramSlider08NameAc);
    seqDataP->paramSlider09VisibleB = getBoolFromJsonToSeqData(arbDataJS, "/gui_settings/sliderGrp/params/8/visibleB");
    copyStrFromJsonToSeqData(arbDataJS, "/gui_settings/sliderGrp/params/8/name",seqDataP->paramSlider09NameAc);
    seqDataP->paramSlider10VisibleB = getBoolFromJsonToSeqData(arbDataJS, "/gui_settings/sliderGrp/params/9/visibleB");
    copyStrFromJsonToSeqData(arbDataJS, "/gui_settings/sliderGrp/params/9/name",seqDataP->paramSlider10NameAc);

    seqDataP->pointGrpVisibleB  = getBoolFromJsonToSeqData (arbDataJS, "/gui_settings/pointGrp/grpVisibleB");
    copyStrFromJsonToSeqData(arbDataJS, "/gui_settings/pointGrp/grpName",seqDataP->pointGrpNameAc);
    seqDataP->paramPoint01VisibleB = getBoolFromJsonToSeqData(arbDataJS, "/gui_settings/pointGrp/params/0/visibleB");
    copyStrFromJsonToSeqData(arbDataJS, "/gui_settings/pointGrp/params/0/name",seqDataP->paramPoint01NameAc);
    seqDataP->paramPoint02VisibleB =    getBoolFromJsonToSeqData(arbDataJS, "/gui_settings/pointGrp/params/1/visibleB");
    copyStrFromJsonToSeqData(arbDataJS, "/gui_settings/pointGrp/params/1/name",seqDataP->paramPoint02NameAc);
    seqDataP->paramPoint03VisibleB =    getBoolFromJsonToSeqData(arbDataJS, "/gui_settings/pointGrp/params/2/visibleB");
    copyStrFromJsonToSeqData(arbDataJS, "/gui_settings/pointGrp/params/2/name",seqDataP->paramPoint03NameAc);
    seqDataP->paramPoint04VisibleB =    getBoolFromJsonToSeqData(arbDataJS, "/gui_settings/pointGrp/params/3/visibleB");
    copyStrFromJsonToSeqData(arbDataJS, "/gui_settings/pointGrp/params/3/name",seqDataP->paramPoint04NameAc);
    seqDataP->paramPoint05VisibleB =    getBoolFromJsonToSeqData(arbDataJS, "/gui_settings/pointGrp/params/4/visibleB");
    copyStrFromJsonToSeqData(arbDataJS, "/gui_settings/pointGrp/params/4/name",seqDataP->paramPoint05NameAc);
    seqDataP->paramPoint06VisibleB =    getBoolFromJsonToSeqData(arbDataJS, "/gui_settings/pointGrp/params/5/visibleB");
    copyStrFromJsonToSeqData(arbDataJS, "/gui_settings/pointGrp/params/5/name",seqDataP->paramPoint06NameAc);
    seqDataP->paramPoint07VisibleB =    getBoolFromJsonToSeqData(arbDataJS, "/gui_settings/pointGrp/params/6/visibleB");
    copyStrFromJsonToSeqData(arbDataJS, "/gui_settings/pointGrp/params/6/name",seqDataP->paramPoint07NameAc);
    seqDataP->paramPoint08VisibleB =    getBoolFromJsonToSeqData(arbDataJS, "/gui_settings/pointGrp/params/7/visibleB");
    copyStrFromJsonToSeqData(arbDataJS, "/gui_settings/pointGrp/params/7/name",seqDataP->paramPoint08NameAc);
    seqDataP->paramPoint09VisibleB =    getBoolFromJsonToSeqData(arbDataJS, "/gui_settings/pointGrp/params/8/visibleB");
    copyStrFromJsonToSeqData(arbDataJS, "/gui_settings/pointGrp/params/8/name",seqDataP->paramPoint09NameAc);
    seqDataP->paramPoint10VisibleB =    getBoolFromJsonToSeqData(arbDataJS, "/gui_settings/pointGrp/params/9/visibleB");
    copyStrFromJsonToSeqData(arbDataJS, "/gui_settings/pointGrp/params/9/name",seqDataP->paramPoint10NameAc);

    seqDataP->cbGrpVisibleB  = getBoolFromJsonToSeqData (arbDataJS, "/gui_settings/cboxGrp/grpVisibleB");
    copyStrFromJsonToSeqData(arbDataJS, "/gui_settings/cboxGrp/grpName",seqDataP->cbGrpNameAc);
    seqDataP->paramCb01VisibleB =    getBoolFromJsonToSeqData(arbDataJS, "/gui_settings/cboxGrp/params/0/visibleB");
    copyStrFromJsonToSeqData(arbDataJS, "/gui_settings/cboxGrp/params/0/name",seqDataP->paramCb01NameAc);
    seqDataP->paramCb02VisibleB =    getBoolFromJsonToSeqData(arbDataJS, "/gui_settings/cboxGrp/params/1/visibleB");
    copyStrFromJsonToSeqData(arbDataJS, "/gui_settings/cboxGrp/params/1/name",seqDataP->paramCb02NameAc);
    seqDataP->paramCb03VisibleB =    getBoolFromJsonToSeqData(arbDataJS, "/gui_settings/cboxGrp/params/2/visibleB");
    copyStrFromJsonToSeqData(arbDataJS, "/gui_settings/cboxGrp/params/2/name",seqDataP->paramCb03NameAc);
    seqDataP->paramCb04VisibleB =    getBoolFromJsonToSeqData(arbDataJS, "/gui_settings/cboxGrp/params/3/visibleB");
    copyStrFromJsonToSeqData(arbDataJS, "/gui_settings/cboxGrp/params/3/name",seqDataP->paramCb04NameAc);
    seqDataP->paramCb05VisibleB =    getBoolFromJsonToSeqData(arbDataJS, "/gui_settings/cboxGrp/params/4/visibleB");
    copyStrFromJsonToSeqData(arbDataJS, "/gui_settings/cboxGrp/params/4/name",seqDataP->paramCb05NameAc);
    seqDataP->paramCb06VisibleB =    getBoolFromJsonToSeqData(arbDataJS, "/gui_settings/cboxGrp/params/5/visibleB");
    copyStrFromJsonToSeqData(arbDataJS, "/gui_settings/cboxGrp/params/5/name",seqDataP->paramCb06NameAc);
    seqDataP->paramCb07VisibleB =    getBoolFromJsonToSeqData(arbDataJS, "/gui_settings/cboxGrp/params/6/visibleB");
    copyStrFromJsonToSeqData(arbDataJS, "/gui_settings/cboxGrp/params/6/name",seqDataP->paramCb07NameAc);
    seqDataP->paramCb08VisibleB =    getBoolFromJsonToSeqData(arbDataJS, "/gui_settings/cboxGrp/params/7/visibleB");
    copyStrFromJsonToSeqData(arbDataJS, "/gui_settings/cboxGrp/params/7/name",seqDataP->paramCb08NameAc);
    seqDataP->paramCb09VisibleB =    getBoolFromJsonToSeqData(arbDataJS, "/gui_settings/cboxGrp/params/8/visibleB");
    copyStrFromJsonToSeqData(arbDataJS, "/gui_settings/cboxGrp/params/8/name",seqDataP->paramCb09NameAc);
    seqDataP->paramCb10VisibleB =    getBoolFromJsonToSeqData(arbDataJS, "/gui_settings/cboxGrp/params/9/visibleB");
    copyStrFromJsonToSeqData(arbDataJS, "/gui_settings/cboxGrp/params/9/name",seqDataP->paramCb10NameAc);

    seqDataP->colorGrpVisibleB  = getBoolFromJsonToSeqData (arbDataJS, "/gui_settings/colorGrp/grpVisibleB");
    copyStrFromJsonToSeqData(arbDataJS, "/gui_settings/colorGrp/grpName",seqDataP->colorGrpNameAc);
    seqDataP->paramColor01VisibleB =    getBoolFromJsonToSeqData(arbDataJS, "/gui_settings/colorGrp/params/0/visibleB");
    copyStrFromJsonToSeqData(arbDataJS, "/gui_settings/colorGrp/params/0/name",seqDataP->paramColor01NameAc);
    seqDataP->paramColor02VisibleB =    getBoolFromJsonToSeqData(arbDataJS, "/gui_settings/colorGrp/params/1/visibleB");
    copyStrFromJsonToSeqData(arbDataJS, "/gui_settings/colorGrp/params/1/name",seqDataP->paramColor02NameAc);
    seqDataP->paramColor03VisibleB =    getBoolFromJsonToSeqData(arbDataJS, "/gui_settings/colorGrp/params/2/visibleB");
    copyStrFromJsonToSeqData(arbDataJS, "/gui_settings/colorGrp/params/2/name",seqDataP->paramColor03NameAc);
    seqDataP->paramColor04VisibleB =    getBoolFromJsonToSeqData(arbDataJS, "/gui_settings/colorGrp/params/3/visibleB");
    copyStrFromJsonToSeqData(arbDataJS, "/gui_settings/colorGrp/params/3/name",seqDataP->paramColor04NameAc);
    seqDataP->paramColor05VisibleB =    getBoolFromJsonToSeqData(arbDataJS, "/gui_settings/colorGrp/params/4/visibleB");
    copyStrFromJsonToSeqData(arbDataJS, "/gui_settings/colorGrp/params/4/name",seqDataP->paramColor05NameAc);
    seqDataP->paramColor06VisibleB =    getBoolFromJsonToSeqData(arbDataJS, "/gui_settings/colorGrp/params/5/visibleB");
    copyStrFromJsonToSeqData(arbDataJS, "/gui_settings/colorGrp/params/5/name",seqDataP->paramColor06NameAc);
    seqDataP->paramColor07VisibleB =    getBoolFromJsonToSeqData(arbDataJS, "/gui_settings/colorGrp/params/6/visibleB");
    copyStrFromJsonToSeqData(arbDataJS, "/gui_settings/colorGrp/params/6/name",seqDataP->paramColor07NameAc);
    seqDataP->paramColor08VisibleB =    getBoolFromJsonToSeqData(arbDataJS, "/gui_settings/colorGrp/params/7/visibleB");
    copyStrFromJsonToSeqData(arbDataJS, "/gui_settings/colorGrp/params/7/name",seqDataP->paramColor08NameAc);
    seqDataP->paramColor09VisibleB =    getBoolFromJsonToSeqData(arbDataJS, "/gui_settings/colorGrp/params/8/visibleB");
    copyStrFromJsonToSeqData(arbDataJS, "/gui_settings/colorGrp/params/8/name",seqDataP->paramColor09NameAc);
    seqDataP->paramColor10VisibleB =    getBoolFromJsonToSeqData(arbDataJS, "/gui_settings/colorGrp/params/9/visibleB");
    copyStrFromJsonToSeqData(arbDataJS, "/gui_settings/colorGrp/params/9/name",seqDataP->paramColor10NameAc);

	seqDataP->rotGrpVisibleB = getBoolFromJsonToSeqData(arbDataJS, "/gui_settings/rotationGrp/grpVisibleB");
	copyStrFromJsonToSeqData(arbDataJS, "/gui_settings/rotationGrp/grpName", seqDataP->rotGrpNameAc);
	seqDataP->paramRot01VisibleB = getBoolFromJsonToSeqData(arbDataJS, "/gui_settings/rotationGrp/params/0/visibleB");
	copyStrFromJsonToSeqData(arbDataJS, "/gui_settings/rotationGrp/params/0/name", seqDataP->paramRot01NameAc);
	seqDataP->paramRot02VisibleB = getBoolFromJsonToSeqData(arbDataJS, "/gui_settings/rotationGrp/params/1/visibleB");
	copyStrFromJsonToSeqData(arbDataJS, "/gui_settings/rotationGrp/params/1/name", seqDataP->paramRot02NameAc);
	seqDataP->paramRot03VisibleB = getBoolFromJsonToSeqData(arbDataJS, "/gui_settings/rotationGrp/params/2/visibleB");
	copyStrFromJsonToSeqData(arbDataJS, "/gui_settings/rotationGrp/params/2/name", seqDataP->paramRot03NameAc);
	seqDataP->paramRot04VisibleB = getBoolFromJsonToSeqData(arbDataJS, "/gui_settings/rotationGrp/params/3/visibleB");
	copyStrFromJsonToSeqData(arbDataJS, "/gui_settings/rotationGrp/params/3/name", seqDataP->paramRot04NameAc);
	seqDataP->paramRot05VisibleB = getBoolFromJsonToSeqData(arbDataJS, "/gui_settings/rotationGrp/params/4/visibleB");
	copyStrFromJsonToSeqData(arbDataJS, "/gui_settings/rotationGrp/params/4/name", seqDataP->paramRot05NameAc);
	seqDataP->paramRot06VisibleB = getBoolFromJsonToSeqData(arbDataJS, "/gui_settings/rotationGrp/params/5/visibleB");
	copyStrFromJsonToSeqData(arbDataJS, "/gui_settings/rotationGrp/params/5/name", seqDataP->paramRot06NameAc);
	seqDataP->paramRot07VisibleB = getBoolFromJsonToSeqData(arbDataJS, "/gui_settings/rotationGrp/params/6/visibleB");
	copyStrFromJsonToSeqData(arbDataJS, "/gui_settings/rotationGrp/params/6/name", seqDataP->paramRot07NameAc);
	seqDataP->paramRot08VisibleB = getBoolFromJsonToSeqData(arbDataJS, "/gui_settings/rotationGrp/params/7/visibleB");
	copyStrFromJsonToSeqData(arbDataJS, "/gui_settings/rotationGrp/params/7/name", seqDataP->paramRot08NameAc);
	seqDataP->paramRot09VisibleB = getBoolFromJsonToSeqData(arbDataJS, "/gui_settings/rotationGrp/params/8/visibleB");
	copyStrFromJsonToSeqData(arbDataJS, "/gui_settings/rotationGrp/params/8/name", seqDataP->paramRot09NameAc);
	seqDataP->paramRot10VisibleB = getBoolFromJsonToSeqData(arbDataJS, "/gui_settings/rotationGrp/params/9/visibleB");
	copyStrFromJsonToSeqData(arbDataJS, "/gui_settings/rotationGrp/params/9/name", seqDataP->paramRot10NameAc);
    
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
            ERR(evalScripts(seqP));
            out_data->sequence_data = seq_dataH;
            suites.HandleSuite1()->host_unlock_handle(seq_dataH);
        } else {    // whoa, we couldn't allocate sequence data; bail!
            err = PF_Err_OUT_OF_MEMORY;
        }
    }
    return err;
}
