//
//  tl_mathSeqData.cpp
//  tlMath
//
// 
//
#include    "tl_math.h"


static PF_Boolean convertIntToBool(A_long input) {
	if (input > 0) {
		return true;
	}
	else return false;
}
static bool getBoolFromJsonToSeqData(nlohmann::json arbDataJS,std::string json_adress){
	nlohmann::json::json_pointer jpointer(json_adress);
	bool dataBoolB = arbDataJS[jpointer].get<bool>();
	return dataBoolB;
}
static std::string getStringFromJsonAdress(nlohmann::json arbDataJS,std::string json_adress, A_char* target){
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
static PF_FpLong getFloatFromJsonAdress(nlohmann::json arbDataJS,std::string json_adress, PF_FpLong  target){
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
static void getMiddleVal(PF_InData* in_data,  PF_Point3DDef *input){
	if (input->x_value == 50.0) {
		input->x_value = (PF_FpLong)in_data->width / 2;
		}
    else if(input->x_value == 100.0) {
        input->x_value = (PF_FpLong)in_data->width;
        }
	if (input->y_value == 50.0) {
		input->y_value = (PF_FpLong)in_data->height / 2;
		}
    else if (input->y_value == 100.0) {
        input->y_value = (PF_FpLong)in_data->height;
        }

	if (input->z_value == 50.0) {
		input->z_value = (PF_FpLong)in_data->height / 2;
		}
    else if (input->z_value == 100.0) {
        input->z_value = (PF_FpLong)in_data->height;
        }
}
static PF_Point3DDef getPointsFromJsonAdress(PF_InData* in_data, nlohmann::json arbDataJS, std::string json_adress, PF_Point3DDef  target){
	//we input the target. in case of error in json pointer
    PF_Point3DDef   data3dF = target;
    std::string json_adress_X = json_adress+"0";
    std::string json_adress_Y= json_adress+"1";
    std::string json_adress_Z= json_adress+"2";
	try {
        nlohmann::json::json_pointer jpointerX(json_adress_X);
        data3dF.x_value= (PF_FpLong)arbDataJS[jpointerX].get<float>();

        nlohmann::json::json_pointer jpointerY(json_adress_Y);
        data3dF.y_value= (PF_FpLong)arbDataJS[jpointerY].get<float>();

        nlohmann::json::json_pointer jpointerZ( json_adress_Z);
        data3dF.z_value= (PF_FpLong)arbDataJS[jpointerZ].get<float>();
	}
	catch (nlohmann::json::exception& e) {
		data3dF = target;
	}
	getMiddleVal(in_data, &data3dF);
	return   data3dF;
}
static A_long getIntFromJsonAdress(nlohmann::json arbDataJS, std::string json_adress, A_long  target){
	//we input the target. in case of error in json pointer
	A_long  dataA;
	nlohmann::json::json_pointer jpointer(json_adress);
	try {
		dataA = (A_long)arbDataJS[jpointer].get<int>();
	}
	catch (nlohmann::json::exception& e) {
		dataA = target;
	}
	return  dataA;
}
static PF_Pixel getColorFromJsonAdress(nlohmann::json arbDataJS, std::string json_adress, PF_Pixel  target) {
	PF_Pixel dataColorCh = target;
	PF_Pixel32 dataSourceF;
	std::string json_adress_R = json_adress + "0";
	std::string json_adress_G = json_adress + "1";
	std::string json_adress_B = json_adress + "2";

	try {
		nlohmann::json::json_pointer jpointerR(json_adress_R);
		dataSourceF.red = (PF_FpShort)arbDataJS[jpointerR].get<float>();

		nlohmann::json::json_pointer jpointerG(json_adress_G);
		dataSourceF.green= (PF_FpShort)arbDataJS[jpointerG].get<float>();

		nlohmann::json::json_pointer jpointerB(json_adress_B);
		dataSourceF.blue = (PF_FpShort)arbDataJS[jpointerB].get<float>();
		dataColorCh.red = (A_u_char)MIN(MAX(0, (float) dataSourceF.red * PF_MAX_CHAN8), PF_MAX_CHAN8);
		dataColorCh.green = (A_u_char)MIN(MAX(0, (float)dataSourceF.green * PF_MAX_CHAN8), PF_MAX_CHAN8);
		dataColorCh.blue = (A_u_char)MIN(MAX(0, (float)dataSourceF.blue * PF_MAX_CHAN8), PF_MAX_CHAN8);
		dataColorCh.alpha = PF_MAX_CHAN8; // palettes don't access to alpha channel
	}
	catch (nlohmann::json::exception& e) {
		dataColorCh = target;
	}
	return dataColorCh;
}
static void copyStrFromJsonToSeqData(nlohmann::json arbDataJS, std::string json_adress, char* target) {
	std::string dataStr = getStringFromJsonAdress(arbDataJS, json_adress, target);
	std::size_t length = dataStr.copy(target, dataStr.size());
	target[length] = '\0';
}
 void   tlmath::copyExprFromJsonToSeqData(nlohmann::json arbDataJS,std::string json_adress,A_char* target){
	std::string dataStr =  getStringFromJsonAdress(arbDataJS, json_adress, target);
	tlmath::scriptCorrectorStr(dataStr);
	std::size_t length = dataStr.copy(target, dataStr.size());
	target[length] = '\0';
}
static void updateSliderParams(PF_ParamDef* params[], nlohmann::json arbDataJS, int indexOffsetI, int numParamsI){
	for (int index = 0; index < numParamsI; index++) {
		int paramIndex = index + indexOffsetI;
		params[paramIndex]->u.fs_d.value = getFloatFromJsonAdress(arbDataJS, "/gui_settings/sliderGrp/params/"+ std::to_string(index) +"/defaultVal/0", params[paramIndex]->u.fs_d.value);
		params[paramIndex]->uu.change_flags = PF_ChangeFlag_CHANGED_VALUE;
	}
}
static void updatePointParams(PF_InData* in_data, PF_ParamDef* params[], nlohmann::json arbDataJS, int indexOffsetI, int numParamsI){
	for (int index = 0; index < numParamsI; index++) {
		int paramIndex = index + indexOffsetI;
		params[paramIndex]->u.point3d_d = getPointsFromJsonAdress(in_data, arbDataJS, "/gui_settings/pointGrp/params/" + std::to_string(index) + "/defaultVal/", params[paramIndex]->u.point3d_d);
		params[paramIndex]->uu.change_flags = PF_ChangeFlag_CHANGED_VALUE;
		}
	}
static void updateCbParams(PF_ParamDef * params[],nlohmann::json arbDataJS, int indexOffsetI, int numParamsI){
	for (int index = 0; index < numParamsI; index++) {
		int paramIndex = index + indexOffsetI;
		params[paramIndex]->u.bd.value = convertIntToBool(getIntFromJsonAdress(arbDataJS, "/gui_settings/cboxGrp/params/" + std::to_string(index) + "/defaultVal/0", params[paramIndex]->u.bd.value));
		params[paramIndex]->uu.change_flags = PF_ChangeFlag_CHANGED_VALUE;
	}
}
static void updateRotParams(PF_ParamDef* params[], nlohmann::json arbDataJS, int indexOffsetI, int numParamsI){
	for (int index = 0; index < numParamsI; index++) {
		int paramIndex = index + indexOffsetI;
		params[paramIndex]->u.ad.value = INT2FIX(getFloatFromJsonAdress(arbDataJS, "/gui_settings/rotationGrp/params/" + std::to_string(index) + "/defaultVal/0", params[paramIndex]->u.ad.value));
		params[paramIndex]->uu.change_flags = PF_ChangeFlag_CHANGED_VALUE;
	}
}
static void updateColorParams(PF_ParamDef* params[], nlohmann::json arbDataJS, int indexOffsetI, int numParamsI){
	for (int index = 0; index < numParamsI; index++) {
		int paramIndex = index + indexOffsetI;
		params[paramIndex]->u.cd.value = (getColorFromJsonAdress(arbDataJS, "/gui_settings/colorGrp/params/" + std::to_string(index) + "/defaultVal/", params[paramIndex]->u.cd.value));
		params[paramIndex]->uu.change_flags = PF_ChangeFlag_CHANGED_VALUE;
	}
}
PF_Err tlmath::updateParamsValue(PF_InData* in_data, PF_ParamDef     *params[],std::string     arbStr){
    PF_Err err = PF_Err_NONE;
    nlohmann::json arbDataJS;
    try {
        arbDataJS = nlohmann::json::parse(arbStr);
    }
    catch (nlohmann::json::exception& e)
    {
        return PF_Err_INTERNAL_STRUCT_DAMAGED;
    }

	//Assuming each params of the  same type are written in group without strangers
	updateSliderParams(params, arbDataJS, MATH_SLIDER_ONE, 10);
	updatePointParams(in_data, params, arbDataJS, MATH_POINT_ONE, 10);
	updateCbParams(params, arbDataJS, MATH_CB_ONE, 10);
	updateRotParams(params, arbDataJS, MATH_ROT_ONE, 10);
	updateColorParams(params, arbDataJS, MATH_COLOR_ONE, 10);
    
    return err;

}
PF_Err tlmath::copyFromArbToSeqData(PF_InData* in_data, PF_OutData* out_data, std::string       arbStr, seqData* seqDataP){
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

	//copy flags
	seqDataP->glsl33ModeB = getBoolFromJsonToSeqData(arbDataJS, "/effectMode/gl33_modeB");
	seqDataP->exprModeB = getBoolFromJsonToSeqData(arbDataJS, "/effectMode/expr_modeB");
	seqDataP->evalModeB = getBoolFromJsonToSeqData(arbDataJS, "/effectMode/evalModeB");
	seqDataP->exprRGBModeB = getBoolFromJsonToSeqData(arbDataJS, "/math_expression/exprRGBModeB");
	seqDataP->pixelsCallExternalInputB[0] = convertIntToBool(getIntFromJsonAdress(arbDataJS, "/flags/pixelsCallExternalInputB/0", 0));
	seqDataP->pixelsCallExternalInputB[1] = convertIntToBool(getIntFromJsonAdress(arbDataJS, "/flags/pixelsCallExternalInputB/1", 0));
	seqDataP->pixelsCallExternalInputB[2] = convertIntToBool(getIntFromJsonAdress(arbDataJS, "/flags/pixelsCallExternalInputB/2", 0));
	seqDataP->pixelsCallExternalInputB[3] = convertIntToBool(getIntFromJsonAdress(arbDataJS, "/flags/pixelsCallExternalInputB/3", 0));
	seqDataP->presetHasWideInputB = getBoolFromJsonToSeqData(arbDataJS, "/flags/presetHasWideInputB");
	seqDataP->cameraB = getBoolFromJsonToSeqData(arbDataJS, "/flags/usesCameraB");

	tlmath::copyExprFromJsonToSeqData(arbDataJS, "/math_expression/redExpr", seqDataP->redExAc);
	tlmath::copyExprFromJsonToSeqData(arbDataJS, "/math_expression/greenExpr", seqDataP->greenExAc);
	tlmath::copyExprFromJsonToSeqData(arbDataJS, "/math_expression/blueExpr", seqDataP->blueExAc);
	tlmath::copyExprFromJsonToSeqData(arbDataJS, "/math_expression/alphaExpr", seqDataP->alphaExAc);
	tlmath::copyExprFromJsonToSeqData(arbDataJS, "/math_expression/rgbExpr", seqDataP->rgbExprExAc);
	tlmath::copyExprFromJsonToSeqData(arbDataJS, "/math_expression/commonExpr", seqDataP->commonExpr);

	tlmath::copyExprFromJsonToSeqData(arbDataJS, "/math_expression/red_error", seqDataP->redError);
	tlmath::copyExprFromJsonToSeqData(arbDataJS, "/math_expression/green_error", seqDataP->greenError);
	tlmath::copyExprFromJsonToSeqData(arbDataJS, "/math_expression/blue_error", seqDataP->blueError);
	tlmath::copyExprFromJsonToSeqData(arbDataJS, "/math_expression/alpha_error", seqDataP->alphaError);
	tlmath::copyExprFromJsonToSeqData(arbDataJS, "/math_expression/rgb_error", seqDataP->rgbError);
	tlmath::copyExprFromJsonToSeqData(arbDataJS, "/math_expression/common_error", seqDataP->commonError);
	
	if (!seqDataP->exprModeB) {
		std::string curr_fragSh = seqDataP->Glsl33_FragmentShAc;
		std::string curr_vertSh = seqDataP->Glsl33_VertexShAc;
		std::string  new_frag = getStringFromJsonAdress(arbDataJS, "/gl_expression/gl33_frag_sh", seqDataP->Glsl33_FragmentShAc);
		std::string  new_vert = getStringFromJsonAdress(arbDataJS, "/gl_expression/gl33_vert_sh", seqDataP->Glsl33_VertexShAc);
		tlmath::scriptCorrectorStr(new_frag);
		tlmath::scriptCorrectorStr(new_vert);
		if (curr_fragSh.compare(new_frag) != 0 || curr_vertSh.compare(new_vert) != 0)
		{
			//copy shaders
			std::size_t  fragLength = new_frag.copy(seqDataP->Glsl33_FragmentShAc, new_frag.length());
			seqDataP->Glsl33_FragmentShAc[fragLength] = '\0';
			std::size_t  vertLength = new_vert.copy(seqDataP->Glsl33_VertexShAc, new_vert.length());
			seqDataP->Glsl33_VertexShAc[vertLength] = '\0';
		}

	}
	tlmath::copyExprFromJsonToSeqData(arbDataJS, "/gl_expression/gl33_frag_error", seqDataP->Glsl33_fragError);
	tlmath::copyExprFromJsonToSeqData(arbDataJS, "/gl_expression/gl33_vert_error", seqDataP->Glsl33_VertError);

     //copy expressions params name
	copyStrFromJsonToSeqData(arbDataJS, "/math_expression/expr_current_channel",seqDataP->expr_ColorChNameAc);
	copyStrFromJsonToSeqData(arbDataJS, "/math_expression/expr_rgb_channel", seqDataP->expr_rgbChNameAc);
	copyStrFromJsonToSeqData(arbDataJS, "/math_expression/expr_pix",seqDataP->expr_pixNameAc);
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
	 

    //copy layers

    copyStrFromJsonToSeqData(arbDataJS, "/gui_settings/layerGrp/current_layer/name",seqDataP->paramLayer00NameAc);
    copyStrFromJsonToSeqData(arbDataJS, "/gui_settings/layerGrp/grpName",seqDataP->layerGrpNameAc);
    seqDataP->layerGrpVisibleB = getBoolFromJsonToSeqData (arbDataJS, "/gui_settings/layerGrp/grpVisibleB");

    copyStrFromJsonToSeqData(arbDataJS, "/gui_settings/layerGrp/extLayer_1/name",seqDataP->paramLayer01NameAc);
    seqDataP->paramLayer01VisibleB = getBoolFromJsonToSeqData (arbDataJS, "/gui_settings/layerGrp/extLayer_1/visibleB");

    copyStrFromJsonToSeqData(arbDataJS, "/gui_settings/layerGrp/extLayer_2/name",seqDataP->paramLayer02NameAc);
    seqDataP->paramLayer02VisibleB = getBoolFromJsonToSeqData (arbDataJS, "/gui_settings/layerGrp/extLayer_2/visibleB");

    copyStrFromJsonToSeqData(arbDataJS, "/gui_settings/layerGrp/extLayer_3/name",seqDataP->paramLayer03NameAc);
    seqDataP->paramLayer03VisibleB = getBoolFromJsonToSeqData (arbDataJS, "/gui_settings/layerGrp/extLayer_3/visibleB");

    copyStrFromJsonToSeqData(arbDataJS, "/gui_settings/layerGrp/extLayer_4/name",seqDataP->paramLayer04NameAc);
    seqDataP->paramLayer04VisibleB = getBoolFromJsonToSeqData (arbDataJS, "/gui_settings/layerGrp/extLayer_4/visibleB");

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
PF_Err tlmath::updateSeqData(PF_InData *in_data,  PF_OutData  *out_data,  PF_ParamDef *params[]){
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
		seqP->initializedB = false;
        tlmath::copyFromArbToSeqData(in_data, out_data,arbDataStr, seqP);
		if (seqP->exprModeB) {
			ERR(tlmath::embedExprInShaders(seqP));
		}
        seqP->initializedB = true;
        out_data->sequence_data = seq_dataH;
        suites.HandleSuite1()->host_unlock_handle(seq_dataH);
    }
    else {    // whoa, we couldn't allocate sequence data; bail!
        err = PF_Err_OUT_OF_MEMORY;
    }
    return err;
}
PF_Err tlmath::SequenceSetdown ( PF_InData        *in_data, PF_OutData        *out_data){
    PF_Err err = PF_Err_NONE;

    if (in_data->sequence_data){
        AEGP_SuiteHandler suites(in_data->pica_basicP);
        suites.HandleSuite1()->host_dispose_handle(in_data->sequence_data);
    }
    return err;
}
PF_Err tlmath::SequenceSetup (PF_InData        *in_data,  PF_OutData        *out_data){
    PF_Err err = PF_Err_NONE;
    AEGP_SuiteHandler suites(in_data->pica_basicP);
    err = SequenceSetdown(in_data, out_data);

    if (!err){
        PF_Handle    seq_dataH =    suites.HandleSuite1()->host_new_handle(sizeof(seqData));

        if (seq_dataH){
            seqData      *seqP = static_cast<seqData*>(suites.HandleSuite1()->host_lock_handle(seq_dataH));
            seqP->initializedB = false;
            tlmath::copyFromArbToSeqData(in_data, out_data, defaultArb, seqP);
			if (seqP->exprModeB) {
				ERR(tlmath::embedExprInShaders(seqP));
			}
            evalScripts(seqP);
            out_data->sequence_data = seq_dataH;
            suites.HandleSuite1()->host_unlock_handle(seq_dataH);
        } else {    // whoa, we couldn't allocate sequence data; bail!
            err = PF_Err_OUT_OF_MEMORY;
        }
    }
    return err;
}
