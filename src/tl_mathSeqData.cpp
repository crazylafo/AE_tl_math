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

    //std::string effect_name = arbDataJS["/effectInfo/effectName"_json_pointer];
    //std::string effect_presetName=(arbDataJS["/effectInfo/presetName"_json_pointer]);

    auto effect_presetName=arbDataJS["/effectInfo/presetName"_json_pointer].get<std::string>();
    effect_presetName.copy( seqDataP->presetNameAc, sizeof(seqDataP->presetNameAc));

    //strncpy(seqDataP->presetNameAc, effect_presetName.c_str(), effect_presetName.length() + 1);


    std::string effect_description=(arbDataJS["/effectInfo/description"_json_pointer]);

    bool mode_glB    = arbDataJS["/effectMode/gl_modeB"_json_pointer];
    bool mode_exprB     = arbDataJS["/effectMode/expr_modeB"_json_pointer];
    bool mode_evalB     = arbDataJS["/effectMode/evalModeB"_json_pointer];

    std::string gl_fragsh =  (arbDataJS["/gl_expression/gl_frag_sh"_json_pointer]);
    std::string gl_vertsh = (arbDataJS["/gl_expression/gl_vert_sh"_json_pointer]);

    std::string expr_red = (arbDataJS["/math_expression/redExpr"_json_pointer]);
    std::string expr_green = (arbDataJS["/math_expression/greenExpr"_json_pointer]);
    std::string expr_blue= (arbDataJS["/math_expression/blueExpr"_json_pointer]);
    std::string expr_rgb = (arbDataJS["/math_expression/rgbExpr"_json_pointer]);
    std::string expr_alpha = (arbDataJS["/math_expression/alphaExpr"_json_pointer]);

    scriptCorrectorStr(gl_fragsh);
    scriptCorrectorStr(gl_vertsh);
    ExprtkCorrectorStr(expr_red);
    ExprtkCorrectorStr(expr_green);
    ExprtkCorrectorStr(expr_blue);
    ExprtkCorrectorStr(expr_alpha);
    ExprtkCorrectorStr(expr_rgb);

    std::string expr_ColorChNameAc = (arbDataJS["/math_expression/expr_current_channel"_json_pointer]);
    std::string expr_pix = (arbDataJS["/math_expression/expr_pix"_json_pointer]);
    std::string expr_luma = (arbDataJS["/math_expression/expr_luma"_json_pointer]);
    std::string expr_red_off = (arbDataJS["/math_expression/expr_red_off"_json_pointer]);
    std::string expr_green_off = (arbDataJS["/math_expression/expr_green_off"_json_pointer]);
    std::string expr_blue_off = (arbDataJS["/math_expression/expr_blue_off"_json_pointer]);
    std::string expr_alpha_off = (arbDataJS["/math_expression/expr_alpha_off"_json_pointer]);

    bool  exprRGBModeB = (arbDataJS["/math_expression/exprRGBModeB"_json_pointer]);
    bool  param_pixelAroundB= (arbDataJS["/flags/needsPixelAroundB"_json_pointer]);
    bool param_ExternalInputB = (arbDataJS["/flags/pixelsCallExternalInputB"_json_pointer]);
    bool param_lumaB = (arbDataJS["/flags/needsLumaB"_json_pointer]);
    bool paramWideeInputB= (arbDataJS["/flags/presetHasWideInputB"_json_pointer]);
    bool  usesCameraB = (arbDataJS["/flags/usesCameraB"_json_pointer]);

    std::string camera_position = (arbDataJS["/composition/camera_position"_json_pointer]);
    std::string camera_target = (arbDataJS["/composition/camera_target"_json_pointer]);
    std::string camera_zoom = (arbDataJS["/composition/camera_zoom"_json_pointer]);
    std::string camera_rotation = (arbDataJS["/composition/camera_rotation"_json_pointer]);

    std::string setting_resolutionName = (arbDataJS["/composition/resolution"_json_pointer]);
    std::string setting_layerPosition = (arbDataJS["/composition/layerPosition"_json_pointer]);
    std::string setting_layerScale = (arbDataJS["/composition/layerScale"_json_pointer]);
    std::string setting_compResolution = (arbDataJS["/composition/compResolution"_json_pointer]);
    std::string setting_timeSecName= (arbDataJS["/composition/time_sec"_json_pointer]);
    std::string setting_timeFrameName= (arbDataJS["/composition/time_frame"_json_pointer]);
    std::string setting_frameRateName= (arbDataJS["/composition/frame_rate"_json_pointer]);

    bool slider_grpVisibleB = (arbDataJS["/gui_settings/sliderGrp/grpVisibleB"_json_pointer]);
    std::string slider_grpName = (arbDataJS["/gui_settings/sliderGrp/grpName"_json_pointer]);
    bool slider_01VisibleB =    (arbDataJS["/gui_settings/sliderGrp/slider_1/visibleB"_json_pointer]);
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
    strncpy_s(seqDataP->rgbExprExAc, expr_rgb.c_str(), expr_rgb.length() + 1);
    strncpy_s(seqDataP->expr_ColorChNameAc, expr_ColorChNameAc.c_str(), expr_ColorChNameAc.length()+1);
    strncpy_s(seqDataP->expr_pixNameAc,expr_pix.c_str(),expr_pix.length() + 1);
    strncpy_s(seqDataP->expr_lumaNameAc ,expr_luma.c_str(),expr_luma.length() + 1);
    strncpy_s(seqDataP->expr_red_offNameAc,expr_red_off.c_str(),expr_red_off.length() + 1);
    strncpy_s(seqDataP->expr_green_offNameAc,expr_green_off.c_str(),expr_green_off.length() + 1);
    strncpy_s(seqDataP->expr_blue_offNameAc,expr_blue_off.c_str(),expr_blue_off.length() + 1);
    strncpy_s(seqDataP->expr_alpha_offNameAc,expr_alpha_off.c_str(),expr_alpha_off.length() + 1);
    strncpy_s(seqDataP->cameraPosNameAc, camera_position.c_str(), camera_position.length() + 1);
    strncpy_s(seqDataP->cameraTargetNameAc , camera_target.c_str(), camera_target.length() + 1);
    strncpy_s(seqDataP->cameraZoomNameAc, camera_zoom.c_str(), camera_zoom.length() + 1);
    strncpy_s(seqDataP->cameraRotationNameAc, camera_rotation.c_str(), camera_rotation.length() + 1);
    strncpy_s(seqDataP->resolution,  setting_resolutionName.c_str(), setting_resolutionName.length() + 1);
    strncpy_s(seqDataP->layerPosition, setting_layerPosition.c_str(), setting_layerPosition.length()+1);
    strncpy_s(seqDataP->layerScale, setting_layerScale.c_str(), setting_layerScale.length() + 1);
    strncpy_s(seqDataP->compResolution, setting_compResolution.c_str(), setting_compResolution.length() + 1);
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
    //strncpy(seqDataP->presetNameAc, effect_presetName.c_str(), effect_presetName.length() + 1);
    strncpy(seqDataP->descriptionAc, effect_description.c_str(), effect_description.length() + 1);
    strncpy(seqDataP->Glsl_FragmentShAc, gl_fragsh.c_str(), gl_fragsh.length() + 1);
    strncpy(seqDataP->Glsl_VertexShAc, gl_vertsh.c_str(), gl_vertsh.length() + 1);
    strncpy(seqDataP->redExAc, expr_red.c_str(), expr_red.length() + 1);
    strncpy(seqDataP->greenExAc, expr_green.c_str(), expr_green.length() + 1);
    strncpy(seqDataP->blueExAc, expr_blue.c_str(), expr_blue.length() + 1);
    strncpy(seqDataP->alphaExAc, expr_alpha.c_str(), expr_alpha.length() + 1);
    strncpy(seqDataP->rgbExprExAc, expr_rgb.c_str(), expr_rgb.length() + 1);
    strncpy(seqDataP->expr_ColorChNameAc, expr_ColorChNameAc.c_str(), expr_ColorChNameAc.length()+1);
    strncpy(seqDataP->expr_pixNameAc, expr_pix.c_str(), expr_pix.length() + 1);
    strncpy(seqDataP->expr_lumaNameAc, expr_luma.c_str(), expr_luma.length() + 1);
    strncpy(seqDataP->expr_red_offNameAc, expr_red_off.c_str(), expr_red_off.length() + 1);
    strncpy(seqDataP->expr_green_offNameAc, expr_green_off.c_str(), expr_green_off.length() + 1);
    strncpy(seqDataP->expr_blue_offNameAc, expr_blue_off.c_str(), expr_blue_off.length() + 1);
    strncpy(seqDataP->expr_alpha_offNameAc, expr_alpha_off.c_str(), expr_alpha_off.length() + 1);
    strncpy(seqDataP->cameraPosNameAc, camera_position.c_str(), camera_position.length() + 1);
    strncpy(seqDataP->cameraTargetNameAc, camera_target.c_str(), camera_target.length() + 1);
    strncpy(seqDataP->cameraZoomNameAc, camera_zoom.c_str(), camera_zoom.length() + 1);
    strncpy(seqDataP->cameraRotationNameAc, camera_rotation.c_str(), camera_rotation.length() + 1);
    strncpy(seqDataP->resolution, setting_resolutionName.c_str(), setting_resolutionName.length() + 1);
    strncpy(seqDataP->layerPosition, setting_layerPosition.c_str(), setting_layerPosition.length() + 1);
    strncpy(seqDataP->layerScale, setting_layerScale.c_str(), setting_layerScale.length() + 1);
    strncpy(seqDataP->compResolution, setting_compResolution.c_str(), setting_compResolution.length() + 1);
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
    seqDataP->exprRGBModeB = exprRGBModeB;
    seqDataP->cameraB = usesCameraB;

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
