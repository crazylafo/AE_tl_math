
#include "tl_math.h"




static PF_Err 
About (	
	PF_InData		*in_data,
	PF_OutData		*out_data,
	PF_ParamDef		*params[],
	PF_LayerDef		*output )
{
	AEGP_SuiteHandler suites(in_data->pica_basicP);
	
	suites.ANSICallbacksSuite1()->sprintf(	out_data->return_msg,
											"%s v%d.%d\r%s",
											STR(StrID_Name), 
											MAJOR_VERSION, 
											MINOR_VERSION, 
											STR(StrID_Description));
	return PF_Err_NONE;
}

static PF_Err 
GlobalSetup (	
	PF_InData		*in_data,
	PF_OutData		*out_data,
	PF_ParamDef		*params[],
	PF_LayerDef		*output )
{
    PF_Err 				err 		= PF_Err_NONE;
    
    PF_Handle			globH		= NULL;
    my_global_dataP		globP		= NULL;
    AEGP_SuiteHandler		suites(in_data->pica_basicP);
    
	out_data->my_version = PF_VERSION(	MAJOR_VERSION, 
										MINOR_VERSION,
										BUG_VERSION, 
										STAGE_VERSION, 
										BUILD_VERSION);

    
	out_data->out_flags =  PF_OutFlag_CUSTOM_UI			|
                           PF_OutFlag_SEND_UPDATE_PARAMS_UI|
                           PF_OutFlag_DEEP_COLOR_AWARE|	// just 16bpc, not 32bpc
                           PF_OutFlag_I_DO_DIALOG|
                           PF_OutFlag_PIX_INDEPENDENT|
                           PF_OutFlag_NON_PARAM_VARY;
    

    
    globH	=	suites.HandleSuite1()->host_new_handle(sizeof(my_global_data));
    
    if (globH) {
        globP = reinterpret_cast<my_global_dataP>(suites.HandleSuite1()->host_lock_handle(globH));
        if (globP) {
            ERR(suites.UtilitySuite3()->AEGP_RegisterWithAEGP(NULL, STR(StrID_Name), &globP->my_id));
            
            if (!err){
                out_data->global_data 	= globH;
            }
        }
        suites.HandleSuite1()->host_unlock_handle(globH);
    } else	{
        err = PF_Err_INTERNAL_STRUCT_DAMAGED;
    }
	
	return 	err;
}

static PF_Err 
ParamsSetup (	
	PF_InData		*in_data,
	PF_OutData		*out_data,
	PF_ParamDef		*params[],
	PF_LayerDef		*output )
{
	PF_Err		err		= PF_Err_NONE;
	PF_ParamDef	def;	

	AEFX_CLR_STRUCT(def);

	PF_ADD_FLOAT_SLIDERX(	STR(StrID_INPUTONE_Param_Name),
							MATH_VAR_MIN,
							MATH_VAR_MAX,
							MATH_VAR_MIN,
							MATH_VAR_MAX,
							MATH_VAR_DFLT,
							PF_Precision_THOUSANDTHS,
							0,
							0,
							MATH_INPONE_VAR_DISK_ID);

	AEFX_CLR_STRUCT(def);
    
    PF_ADD_FLOAT_SLIDERX(	STR(StrID_INPUTTWO_Param_Name),
                         MATH_VAR_MIN,
                         MATH_VAR_MAX,
                         MATH_VAR_MIN,
                         MATH_VAR_MAX,
                         MATH_VAR_DFLT,
                         PF_Precision_THOUSANDTHS,
                         0,
                         0,
                         MATH_INPTWO_VAR_DISK_ID);
    
    AEFX_CLR_STRUCT(def);
    
    PF_ADD_FLOAT_SLIDERX(	STR(StrID_INPUTTHREE_Param_Name),
                         MATH_VAR_MIN,
                         MATH_VAR_MAX,
                         MATH_VAR_MIN,
                         MATH_VAR_MAX,
                         MATH_VAR_DFLT,
                         PF_Precision_THOUSANDTHS,
                         0,
                         0,
                         MATH_INPTHREE_VAR_DISK_ID);
    
    AEFX_CLR_STRUCT(def);
    
    PF_ADD_FLOAT_SLIDERX(	STR( StrID_INPUTFOUR_Param_Name),
                         MATH_VAR_MIN,
                         MATH_VAR_MAX,
                         MATH_VAR_MIN,
                         MATH_VAR_MAX,
                         MATH_VAR_DFLT,
                         PF_Precision_THOUSANDTHS,
                         0,
                         0,
                         MATH_INPFOUR_VAR_DISK_ID);
    
    AEFX_CLR_STRUCT(def);

    ERR(CreateDefaultArb(	in_data,
                         out_data,
                         &def.u.arb_d.dephault));

    
    
    PF_ADD_ARBITRARY2(	"data transfert", 
                      10,
                      10,
                      PF_ParamFlag_CANNOT_TIME_VARY| PF_ParamFlag_SUPERVISE,
                      PF_PUI_NO_ECW_UI,
                      def.u.arb_d.dephault,
                      MATH_ARB_DATA,
                      ARB_REFCON);
    
    if (!err) {
        PF_CustomUIInfo			ci;
        
        ci.events				= PF_CustomEFlag_EFFECT;
        
        ci.comp_ui_width		= ci.comp_ui_height = 0;
        ci.comp_ui_alignment	= PF_UIAlignment_NONE;
        
        ci.layer_ui_width		=
        ci.layer_ui_height		= 0;
        ci.layer_ui_alignment	= PF_UIAlignment_NONE;
        
        ci.preview_ui_width		=
        ci.preview_ui_height	= 0;
        ci.layer_ui_alignment	= PF_UIAlignment_NONE;
        
        err = (*(in_data->inter.register_ui))(in_data->effect_ref, &ci);
    }
    
	AEFX_CLR_STRUCT(def);
    PF_ADD_POINT(STR (strID_INPUTPOINTONE_Param_Name), 50, 50, FALSE, MATH_INP_POINT_ONE_DISK_ID);
    
    AEFX_CLR_STRUCT(def);
    PF_ADD_POINT(STR(strID_INPUTPOINTTWO_Param_Name), 50, 50, FALSE, MATH_INP_POINT_TWO_DISK_ID);
    
    
    
    AEFX_CLR_STRUCT(def);
    PF_ADD_COLOR(STR(strID_INPUTCOLORONE_Param_Name), PF_MAX_CHAN8, PF_MAX_CHAN8, PF_MAX_CHAN8, MATH_INP_COLOR_ONE_DISK_ID);
    
    AEFX_CLR_STRUCT(def);
    PF_ADD_COLOR(STR(strID_INPUTCOLORTWO_Param_Name), PF_MAX_CHAN8, PF_MAX_CHAN8, PF_MAX_CHAN8,MATH_INP_COLOR_TWO_DISK_ID);
    
    AEFX_CLR_STRUCT(def);
	out_data->num_params = MATH_NUM_PARAMS;
    
    AEFX_SuiteScoper<PF_EffectUISuite1> effect_ui_suiteP = AEFX_SuiteScoper<PF_EffectUISuite1>(
                                                                                               in_data,
                                                                                               kPFEffectUISuite,
                                                                                               kPFEffectUISuiteVersion1,
                                                                                               out_data);
    
    ERR(effect_ui_suiteP->PF_SetOptionsButtonName(in_data->effect_ref, "Expresion Math"));


	return err;
}

static PF_Err
MakeParamCopy(
              PF_ParamDef *actual[],	/* >> */
              PF_ParamDef copy[])		/* << */
{
    for (A_short iS = 0; iS < MATH_NUM_PARAMS; ++iS)	{
        AEFX_CLR_STRUCT(copy[iS]);	// clean params are important!
    }
    copy[MATH_INPUT]			=*actual[MATH_INPUT];
    copy[MATH_INPONE_VAR]       =*actual[MATH_INPONE_VAR];
    copy[MATH_INPTWO_VAR]       =*actual[MATH_INPTWO_VAR];
    copy[MATH_INPTHREE_VAR]     =*actual[MATH_INPTHREE_VAR];
    copy[MATH_INPFOUR_VAR]      =*actual[MATH_INPFOUR_VAR];
    copy[MATH_ARB_DATA]         =*actual[MATH_ARB_DATA];
    copy[MATH_INP_POINT_ONE]    =*actual [MATH_INP_POINT_ONE];
    copy[MATH_INP_POINT_TWO]    =*actual [MATH_INP_POINT_TWO];
    copy[MATH_INP_COLOR_ONE]    =*actual [MATH_INP_COLOR_ONE];
    copy[MATH_INP_COLOR_TWO]    =*actual [MATH_INP_COLOR_TWO];
    
    return PF_Err_NONE;
    
}

static PF_Err
UpdateParameterUI(
                  PF_InData			*in_data,
                  PF_OutData			*out_data,
                  PF_ParamDef			*params[],
                  PF_LayerDef			*outputP)
{
    PF_Err				err					= PF_Err_NONE,
    err2				= PF_Err_NONE;
    my_global_dataP		globP				= reinterpret_cast<my_global_dataP>(DH(out_data->global_data));

    AEGP_EffectRefH			meH				= NULL;
    AEGP_SuiteHandler		suites(in_data->pica_basicP);
    PF_ParamDef		param_copy[MATH_NUM_PARAMS];
    ERR(MakeParamCopy(params, param_copy));
    
    AEGP_StreamRefH		ARB_DATA_streamH		= NULL;
    

    
    ERR(suites.PFInterfaceSuite1()->AEGP_GetNewEffectForEffect(globP->my_id, in_data->effect_ref, &meH));
    ERR(suites.StreamSuite2()->AEGP_GetNewEffectStreamByIndex(globP->my_id, meH, MATH_ARB_DATA, 	&ARB_DATA_streamH));
    ERR(suites.DynamicStreamSuite2()->AEGP_SetDynamicStreamFlag(ARB_DATA_streamH, AEGP_DynStreamFlag_HIDDEN, FALSE, TRUE));
    
    if (meH){
        ERR2(suites.EffectSuite2()->AEGP_DisposeEffect(meH));
    }
    if (ARB_DATA_streamH){
        ERR2(suites.StreamSuite2()->AEGP_DisposeStream(ARB_DATA_streamH));
    }
    
    if (!err){
        out_data->out_flags |= PF_OutFlag_FORCE_RERENDER;
    }

    return err;
}




static PF_Err
MySimpleGainFunc16 (
                    void		*refcon,
	A_long		xL, 
	A_long		yL, 
	PF_Pixel16	*inP, 
	PF_Pixel16	*outP)
{
	PF_Err		err = PF_Err_NONE;

	MathInfo	*miP	= reinterpret_cast<MathInfo*>(refcon);

					
	if (miP){

		outP->alpha		=	inP->alpha;
		outP->red		=	MIN((inP->red), PF_MAX_CHAN16);
		outP->green		=	MIN((inP->green), PF_MAX_CHAN16);
		outP->blue		=	MIN((inP->blue), PF_MAX_CHAN16);
	}

	return err;
}


static PF_Err
AEGP_SetParamStreamValue(PF_InData			*in_data,
						PF_OutData			*out_data,
						AEGP_PluginID		PlugId,
						PF_ParamIndex		param_index,
						PF_Handle           *ArbH)
{
	PF_Err  err = PF_Err_NONE,
			err2 = PF_Err_NONE;
	AEGP_LayerH		layerH;
	AEGP_StreamRefH effect_streamH = NULL;
	AEGP_EffectRefH   thisEffect_refH;
	AEGP_StreamValue2	val;
	AEGP_StreamValue2	*sample_valP = &val;
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




//math parser's functions
static PF_FpShort
inline parseDrawRect(PF_FpShort xL, PF_FpShort yL, PF_FpShort center_x, PF_FpShort center_y, PF_FpShort lx,PF_FpShort ly)
{

	if (xL > (center_x - lx) &&
		xL <(center_x + lx) &&
		yL >(center_y - ly) &&
		yL < (center_y + ly))
	{
		return 1;
	}

	else{
		return 0;
	}
}

static PF_Err
PopDialog(
	PF_InData		*in_data,
	PF_OutData		*out_data,
	PF_ParamDef		*params[],
	PF_LayerDef		*output)
{
	PF_Err err = PF_Err_NONE;
	AEGP_SuiteHandler	suites(in_data->pica_basicP);
	my_global_dataP		globP = reinterpret_cast<my_global_dataP>(DH(out_data->global_data));

	AEGP_MemHandle     resultMemH = NULL;
	A_char *resultAC = NULL;
	A_char          scriptAC[6000] = { '\0' };
	std::string Majvers = std::to_string(MAJOR_VERSION);
	std::string MinVers = std::to_string(MINOR_VERSION);
    
    
    A_char   SET_EXPR_SCRIPT[6000] = "function expr(redExpr,greenExpr,blueExpr,alphaExpr, pluginMAJORV, pluginMINORV){ \n\
    var pluginVersion = pluginMAJORV+'.'+pluginMINORV;\n\
    var w = new Window('dialog', 'Maths Expressions', undefined, {resizeable:true} );\n\
    w.sttxt= w.add ('statictext', undefined, 'Write here your math operations for each channels. Math operations are based on Mathematical Expression Toolkit Library');\n\
    w.grp = w.add('group');\n\
    w.grp.orientation='column';\n\
    w.grp.alignment = ['fill', 'fill'];\n\
    w.grp.alignChildren = ['fill', 'fill'];\n\
    w.grp.extButtGrp = w.grp.add('group');\n\
    w.grp.extButtGrp.alignment = ['fill', 'fill'];\n\
    w.grp.extButtGrp.alignChildren = ['fill', 'fill'];\n\
    w.grp.extButtGrp.loadBtn = w.grp.extButtGrp.add ('button', undefined, 'Load');\n\
    w.grp.extButtGrp.saveBtn =w.grp.extButtGrp.add('button', undefined, 'Save');\n\
    w.grp.redC = w.grp.add('group');\n\
    w.grp.redC.orientation = 'row';\n\
    w.grp.redC.alignment = ['fill', 'fill'];\n\
    w.grp.redC.alignChildren = ['fill', 'fill'];\n\
    w.grp.redC.redst = w.grp.redC.add ('statictext', undefined,'Red Expr    :');\n\
    w.grp.redC.redet = w.grp.redC.add ('edittext', undefined, redExpr,{multiline:true});\n\
    w.grp.greenC = w. grp.add('group');\n\
    w.grp.greenC.orientation = 'row';\n\
    w.grp.greenC.alignChildren = ['fill', 'fill'];\n\
    w.grp.greenC.greenst = w.grp.greenC.add ('statictext', undefined,'Green Expr :');\n\
    w.grp.greenC.greenet = w.grp.greenC.add ('edittext', undefined, greenExpr,{multiline:true});\n\
    w.grp.blueC = w.grp.add('group');\n\
    w.grp.blueC.orientation = 'row';\n\
    w.grp.blueC.alignChildren = ['fill', 'fill'];\n\
    w.grp.blueC.bluest = w.grp.blueC.add ('statictext', undefined, 'Blue Expr   :');\n\
    w.grp.blueC.blueet = w.grp.blueC.add ('edittext', undefined, blueExpr,{multiline:true});\n\
    w.grp.alphaC = w.grp.add('group');\n\
    w.grp.alphaC.orientation = 'row';\n\
    w.grp.alphaC.alignChildren = ['fill', 'fill'];\n\
    w.grp.alphaC.alphast = w.grp.alphaC.add ('statictext', undefined, 'Alpha Expr :');\n\
    w.grp.alphaC.alphaet = w.grp.alphaC.add ('edittext', undefined, alphaExpr,{multiline:true});\n\
    w.grp.btnGrp = w.grp.add('Group');\n\
    w.grp.btnGrp.orientation = 'row';\n\
    w.grp.btnGrp.Ok =w.grp.btnGrp.add ('button', undefined, 'ok');\n\
    w.grp.btnGrp.Cancel =w.grp.btnGrp.add ('button', undefined, 'Cancel');\n\
    var result = '';\n\
    w.grp.extButtGrp.loadBtn.onClick = function (){\n\
    var exprObj = readJson();\n\
    if (exprObj.error === \"none\"){\n\
    w.grp.redC.redet.text =		exprObj.redExpr;\n\
    w.grp.greenC.greenet.text=	exprObj.greenExpr;\n\
    w.grp.blueC.blueet.text =	exprObj.blueExpr;\n\
    w.grp.alphaC.alphaet.text=	exprObj.alphaExpr;\n\
    alert (exprObj.error);\n\
    }\n\
    }\n\
    w.grp.extButtGrp.saveBtn.onClick = function (){\n\
    saveAsJson (w.grp.redC.redet.text, w.grp.greenC.greenet.text, w.grp.blueC.blueet.text, w.grp.alphaC.alphaet.text, pluginVersion);\n\
    }\n\
    w.grp.btnGrp.Ok.onClick = function(){\n\
    var strExpr ='rfromJS'+w.grp.redC.redet.text+'gfromJS'+w.grp.greenC.greenet.text+'bfromJS'+w.grp.blueC.blueet.text+'afromJS'+w.grp.alphaC.alphaet.text;\n\
    w.close();\n\
    result = strExpr;\n\
    }\n\
    w.grp.btnGrp.Cancel.onClick = function(){\n\
    var ret ='rfromJS'+redExpr+'gfromJS'+greenExpr+'bfromJS'+blueExpr+'afromJS'+alphaExpr;\n\
    w.close();\n\
    result = ret;\n\
    }\n\
    w.onResizing = w.onResize = function(){this.layout.resize();}\n\
    w.show();\n\
    return result\n\
    }\n\
    function createJson(redExpr,greenExpr,blueExpr,alphaExpr, pluginVersion){\n\
    ExprObj = {\n\
    effectName   : \"tlMath\",\n\
    exprLang :  \"Exprtk\",\n\
    category :  \"Custom\",\n\
    pluginVesion :  pluginVersion,\n\
    notCompatibleVersion : 0,\n\
    redExpr   : redExpr,\n\
    greenExpr : greenExpr,\n\
    blueExpr  : blueExpr,\n\
    alphaExpr : alphaExpr,\n\
    };\n\
    return ExprObj;\n\
    }\n\
    function saveAsJson(redExpr,greenExpr,blueExpr,alphaExpr, pluginVersion){\n\
    ExprObj = createJson(redExpr,greenExpr,blueExpr,alphaExpr, pluginVersion);\n\
    var presetFile =File.saveDialog('save your preset as a json');\n\
    if (presetFile && presetFile.open('w')){\n\
    presetFile.encoding ='UTF-8';\n\
    presetFile.write(JSON.stringify(ExprObj, undefined, '\\r\\n'));\n\
    presetFile.close();\n\
    }\n\
    };\n\
    function readJson(){\n\
    var ExprObj ={};\n\
    var loadFile =File.openDialog('load your preset json');\n\
    if (loadFile && loadFile.open('r')){\n\
    loadFile.encoding ='UTF-8';\n\
    var jsonFile = loadFile.read();\n\
    var testObj = JSON.parse(jsonFile);\n\
    if (testObj.effectName === \"tlMath\")\n\
    try{\n\
    ExprObj.exprLang = testObj.exprLang;\n\
    ExprObj.category = testObj.category;\n\
    ExprObj.pluginVesion = testObj.pluginVesion;\n\
    ExprObj.notCompatibleVersion = testObj.notCompatibleVersion;\n\
    ExprObj.redExpr = testObj.redExpr;\n\
    ExprObj.greenExpr = testObj.greenExpr;\n\
    ExprObj.blueExpr = testObj.blueExpr;\n\
    ExprObj.alphaExpr = testObj.alphaExpr;\n\
    ExprObj.error = \"none\";\n\
    }catch (e) {\n\
    alert(e)\n\
    ExprObj.error = \"err\";\n\
    }\n\
    loadFile.close();\n\
    }\n\
    return ExprObj;\n\
    };\n\
    expr(%s,%s,%s,%s,%s,%s);";
    
    

    
    //ARB
    PF_ParamDef arb_param;
    
    //strings to send expr to script
    std::string tempRedS ="'";
    std::string tempGreenS ="'";
    std::string tempBlueS ="'";
    std::string tempAlphaS ="'";
    

	PF_Handle		arbOutH = NULL;
	m_ArbData		*arbInP = NULL;
    m_ArbData		*arbOutP= NULL;

    
    AEFX_CLR_STRUCT(arb_param);
    ERR(PF_CHECKOUT_PARAM(	in_data,
                          MATH_ARB_DATA,
                          in_data->current_time,
                          in_data->time_step,
                          in_data->time_scale, 
                          &arb_param));

    if (!err){
        AEFX_CLR_STRUCT(arbInP);
        arbInP = reinterpret_cast<m_ArbData*>(*arb_param.u.arb_d.value);
        if (arbInP){
            tempRedS.append(arbInP->redExAcFlat);
            tempGreenS.append(arbInP->greenExAcFlat);
            tempBlueS.append(arbInP->blueExAcFlat);
            tempAlphaS.append(arbInP->alphaExAcFlat);
        }
       
    }

    tempRedS.append("'");
    tempGreenS.append("'");
    tempBlueS.append("'");
    tempAlphaS.append("'");

	//to force the parser to keep \n before to send it to js
	strReplace(tempRedS, "\n", "\\n");
	strReplace(tempGreenS, "\n", "\\n");
	strReplace(tempBlueS, "\n", "\\n");
	strReplace(tempAlphaS, "\n", "\\n");


    sprintf( scriptAC, SET_EXPR_SCRIPT,tempRedS.c_str(), tempGreenS.c_str() , tempBlueS.c_str() , tempAlphaS.c_str(),Majvers.c_str() , MinVers .c_str() );
    ERR(suites.UtilitySuite6()->AEGP_ExecuteScript(globP->my_id, scriptAC, FALSE, &resultMemH, NULL));

    //AEGP SETSTREAMVALUR TO ARB
    AEFX_CLR_STRUCT(resultAC);
    ERR(suites.MemorySuite1()->AEGP_LockMemHandle(resultMemH, reinterpret_cast<void**>(&resultAC)));

    if  (resultAC){
        AEFX_CLR_STRUCT(arbOutP);
        arbOutP = reinterpret_cast<m_ArbData*>(*arb_param.u.arb_d.value);
        //set result per channel
        std::string resultStr = resultAC;
        std::size_t redPos = resultStr.find("rfromJS");
        std::size_t greenPos = resultStr.find("gfromJS");
        std::size_t bluePos = resultStr.find("bfromJS");
        std::size_t alphaPos = resultStr.find("afromJS");
	
        
        std::string redResultStr =resultStr.substr(redPos+7, greenPos -redPos-7); // extract red channel from script return
        std::string greenResultStr = resultStr.substr(greenPos+7, bluePos-greenPos-7);
        std::string blueResultStr = resultStr.substr(bluePos+7, alphaPos-bluePos-7);
        std::string alphaResultStr = resultStr.substr(alphaPos+7);
        //copy to flat ARB (keeping /n and other speical char from js
        

        #ifdef AE_OS_WIN
            strncpy_s( arbOutP->redExAcFlat, redResultStr.c_str(), redResultStr.length()+1);
            strncpy_s(arbOutP->greenExAcFlat, greenResultStr.c_str(), greenResultStr.length()+1);
            strncpy_s( arbOutP->blueExAcFlat, blueResultStr.c_str(), blueResultStr.length()+1);
            strncpy_s( arbOutP->alphaExAcFlat, alphaResultStr.c_str(), alphaResultStr.length()+1);
        #else
            strncpy( arbOutP->redExAcFlat, redResultStr.c_str(), redResultStr.length()+1);
            strncpy(arbOutP->greenExAcFlat, greenResultStr.c_str(), greenResultStr.length()+1);
            strncpy( arbOutP->blueExAcFlat, blueResultStr.c_str(), blueResultStr.length()+1);
            strncpy( arbOutP->alphaExAcFlat, alphaResultStr.c_str(), alphaResultStr.length()+1);
        #endif
        
        //delete \nfor execution expr
        redResultStr.erase(std::remove(redResultStr.begin(), redResultStr.end(), '\n'), redResultStr.end());
        greenResultStr.erase(std::remove(greenResultStr.begin(), greenResultStr.end(), '\n'), greenResultStr.end());
        blueResultStr.erase(std::remove(blueResultStr.begin(), blueResultStr.end(), '\n'), blueResultStr.end());
        alphaResultStr.erase(std::remove(alphaResultStr.begin(), alphaResultStr.end(), '\n'), alphaResultStr.end());
        
        #ifdef AE_OS_WIN
            strncpy_s( arbOutP->redExAc, redResultStr.c_str(), redResultStr.length()+1);
            strncpy_s(arbOutP->greenExAc, greenResultStr.c_str(), greenResultStr.length()+1);
            strncpy_s( arbOutP->blueExAc, blueResultStr.c_str(), blueResultStr.length()+1);
            strncpy_s( arbOutP->alphaExAc, alphaResultStr.c_str(), alphaResultStr.length()+1);
        #else
 			strncpy( arbOutP->redExAc, redResultStr.c_str(), redResultStr.length()+1);
            strncpy(arbOutP->greenExAc, greenResultStr.c_str(), greenResultStr.length()+1);
            strncpy( arbOutP->blueExAc, blueResultStr.c_str(), blueResultStr.length()+1);
            strncpy( arbOutP->alphaExAc, alphaResultStr.c_str(), alphaResultStr.length()+1);
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



template <typename T=PF_FpShort> class parseExpr {
        private:
            std::shared_ptr<exprtk::parser<T>> parser;
            exprtk::expression<T> expression;
            exprtk::symbol_table<T> symbol_table;
        public:
            parseExpr(void *refcon, const std::string &exprstr) {
                MathInfo	*miP	= reinterpret_cast<MathInfo*>(refcon);
                std::string expression_string_Safe = "1";
                if (!parser){
                    parser = std::make_shared<exprtk::parser<T>>();
                }
                miP->hasErrorB = FALSE;
                symbol_table.clear();
                symbol_table.add_variable("xL",  miP->xLF);
                symbol_table.add_variable("yL",  miP->yLF);
                symbol_table.add_variable("inP_red", miP->inRedF);
                symbol_table.add_variable("inP_green", miP->inGreenF);
                symbol_table.add_variable("inP_blue", miP->inBlueF);
                symbol_table.add_variable("inP_alpha", miP->inAlphaF);
                symbol_table.add_variable("inP_luma", miP->luma);
                symbol_table.add_vector("vec3_red", miP->m3P_red);
                symbol_table.add_vector("vec3_green",miP->m3P_green);
                symbol_table.add_vector("vec3_blue", miP->m3P_blue);
                symbol_table.add_vector("vec3_alpha", miP->m3P_alpha);
                symbol_table.add_constants();
                symbol_table.add_constant("var1",miP->inOneF);
                symbol_table.add_constant("var2",miP->inTwoF);
                symbol_table.add_constant("var3",miP->inThreeF);
                symbol_table.add_constant("var4",miP->inFourF);
                symbol_table.add_constant ("pt1_x",miP->pointOneX);
                symbol_table.add_constant ("pt1_y",miP->pointOneY);
                symbol_table.add_constant ("pt2_x",miP->pointTwoX);
                symbol_table.add_constant ("pt2_x",miP->pointTwoY);
                symbol_table.add_constant ("cl1_red",miP->colorOne_red);
                symbol_table.add_constant ("cl1_green", miP->colorOne_green);
                symbol_table.add_constant ("cl1_blue",miP->colorOne_blue);
                symbol_table.add_constant ("cl2_red",miP->colorTwo_red);
                symbol_table.add_constant ("cl2_green",miP->colorTwo_green);
                symbol_table.add_constant ("cl2_blue",miP->colorTwo_blue);
                symbol_table.add_constant("layerWidth",miP->layerWidthF);
                symbol_table.add_constant("layerHeight",miP->layerHeightF);
                symbol_table.add_constant("layerTime_sec",miP->layerTime_Sec);
                symbol_table.add_constant("layerTime_frame",miP->layerTime_Frame);
                symbol_table.add_constant("layerDuration",miP->layerDuration);
                symbol_table.add_constant("layerPosition_x", miP->layerPos_X);
                symbol_table.add_constant("layerPosition_y", miP->layerPos_Y);
                symbol_table.add_constant("layerPosition_z", miP->layerPos_Z);
                symbol_table.add_constant("layerScale_z", miP->layerScale_X);
                symbol_table.add_constant("layerScale_y", miP->layerScale_Y);
                symbol_table.add_constant("layerScale_z", miP->layerScale_Z);
                symbol_table.add_constant("compWidth", miP->compWidthF);
                symbol_table.add_constant("compHeight", miP->compHeightF);
                symbol_table.add_constant("compFps", miP->compFpsF);
                symbol_table.add_function("drawRect", parseDrawRect);
                expression.register_symbol_table(symbol_table);
                parser->compile(exprstr,expression);
                if (!parser->compile(exprstr,expression))
                {
                    miP->hasErrorB = TRUE;
                    miP->errorstr =parser->error();
                    parser->compile(expression_string_Safe, expression);
                }
            }
            T operator()() { return expression.value(); }
        };


static PF_Err
Render (
        PF_InData		*in_data,
        PF_OutData		*out_data,
        PF_ParamDef		*params[],
        PF_LayerDef		*outputP )
{
	PF_Err				err		= PF_Err_NONE;
	AEGP_SuiteHandler	suites(in_data->pica_basicP);
	

	MathInfo			miP;
	AEFX_CLR_STRUCT(miP);
	A_long				linesL	= 0;
	linesL 		= outputP->extent_hint.bottom - outputP->extent_hint.top;
	PF_EffectWorld		*inputP = &params[0]->u.ld;
    
    PF_Handle		arbH			= params[MATH_ARB_DATA]->u.arb_d.value;
    m_ArbData		*arbP			= NULL;

	AEGP_LayerH		layerH;
	AEGP_CompH		compH;
	AEGP_ItemH      itemH;
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

	AEFX_SuiteScoper<AEGP_StreamSuite4> StreamSuite = AEFX_SuiteScoper<AEGP_StreamSuite4>(in_data,
		kAEGPStreamSuite,
		kAEGPStreamSuiteVersion4,
		out_data);

	PFInterfaceSuite->AEGP_GetEffectLayer(in_data->effect_ref, &layerH);
	layerSuite->AEGP_GetLayerParentComp(layerH, &compH);
	compSuite->AEGP_GetItemFromComp(compH, &itemH);
	A_long width, height;
	A_Time currTime;
	AEGP_StreamVal2 strValP, strValSP;
	AEGP_StreamType  strTypeP;
	AEFX_CLR_STRUCT(width);
	AEFX_CLR_STRUCT(height);
	itemSuite->AEGP_GetItemDimensions(itemH, &width, &height);
	miP.compWidthF = PF_FpLong(width);
	miP.compHeightF = PF_FpLong(height);
	AEGP_DownsampleFactor dsp;
	compSuite->AEGP_GetCompDownsampleFactor(compH, &dsp);
	miP.compWidthF *= dsp.xS;
	miP.compHeightF *= dsp.yS;
    PF_FpLong fpsF;
	compSuite->AEGP_GetCompFramerate(compH,&fpsF);
    miP.compFpsF = static_cast<float>( fpsF);

	layerSuite->AEGP_GetLayerCurrentTime(layerH, AEGP_LTimeMode_LayerTime, &currTime);
	StreamSuite->AEGP_GetLayerStreamValue(layerH, AEGP_LayerStream_POSITION, AEGP_LTimeMode_LayerTime, &currTime, NULL, &strValP, &strTypeP);
	miP.layerPos_X = strValP.three_d.x;
	miP.layerPos_Y = strValP.three_d.y;
	miP.layerPos_Z = strValP.three_d.z;
	StreamSuite->AEGP_GetLayerStreamValue(layerH, AEGP_LayerStream_SCALE, AEGP_LTimeMode_LayerTime, &currTime, NULL, &strValSP, &strTypeP);
	miP.layerScale_X= strValSP.three_d.x;
	miP.layerScale_Y = strValSP.three_d.z;
	miP.layerScale_Z = strValSP.three_d.z;
    
	miP.inOneF	= params[MATH_INPONE_VAR]->u.fs_d.value;
    miP.inTwoF	= params[MATH_INPTWO_VAR]->u.fs_d.value;
    miP.inThreeF= params[MATH_INPTHREE_VAR]->u.fs_d.value;
    miP.inFourF	= params[MATH_INPFOUR_VAR]->u.fs_d.value;
   
    //layer size
    miP.scale_x = in_data->downsample_x.num*in_data->pixel_aspect_ratio.num/ (float)in_data->downsample_x.den,
    miP.scale_y = in_data->downsample_y.num*in_data->pixel_aspect_ratio.den/ (float)in_data->downsample_y.den;
    miP.layerWidthF = PF_FpShort (in_data->width*miP.scale_x) ;
    miP.layerHeightF = PF_FpShort(in_data->height* miP.scale_y);
    
    //time params
    miP.layerTime_Sec = PF_FpShort(in_data->current_time)/PF_FpShort(in_data->time_scale);
    miP.layerTime_Frame = PF_FpShort(in_data->current_time/in_data->time_step);
    miP.layerDuration =PF_FpShort( in_data->total_time / in_data->time_scale);
    
    
    //user param points
    miP.pointOneX = static_cast<PF_FpShort>(round(FIX_2_FLOAT(params[MATH_INP_POINT_ONE]->u.td.x_value)));
    miP.pointOneY =static_cast<PF_FpShort>(round(FIX_2_FLOAT(params[MATH_INP_POINT_ONE]->u.td.y_value)));
    miP.pointTwoX = static_cast<PF_FpShort>(round(FIX_2_FLOAT(params[MATH_INP_POINT_TWO]->u.td.x_value)));
    miP.pointTwoY =static_cast<PF_FpShort>(round(FIX_2_FLOAT(params[MATH_INP_POINT_TWO]->u.td.y_value)));
    
    //user param color
    miP.colorOne_red =  PF_FpShort (params[MATH_INP_COLOR_ONE]->u.cd.value.red)/ PF_FpShort (PF_MAX_CHAN8);
    miP.colorOne_green =PF_FpShort (params[MATH_INP_COLOR_ONE]->u.cd.value.green)/ PF_FpShort (PF_MAX_CHAN8);
    miP.colorOne_blue = PF_FpShort (params[MATH_INP_COLOR_ONE]->u.cd.value.blue)/ PF_FpShort (PF_MAX_CHAN8);
    
    miP.colorTwo_red = PF_FpShort (params[MATH_INP_COLOR_TWO]->u.cd.value.red)/ PF_FpShort (PF_MAX_CHAN8);
    miP.colorTwo_green = PF_FpShort (params[MATH_INP_COLOR_TWO]->u.cd.value.green)/ PF_FpShort (PF_MAX_CHAN8);
    miP.colorTwo_blue = PF_FpShort (params[MATH_INP_COLOR_TWO]->u.cd.value.blue)/ PF_FpShort (PF_MAX_CHAN8);
	miP.xLF = 0;
	miP.yLF = 0;


    std::string expression_string_red= "1";
    std::string expression_string_green= "1";
    std::string expression_string_blue= "1";
    std::string expression_string_alpha= "1";
    

    if (!err){
        arbP = reinterpret_cast<m_ArbData*>(suites.HandleSuite1()->host_lock_handle(arbH));
        if (arbP){
            m_ArbData *tempPointer = reinterpret_cast<m_ArbData*>(arbP);
            expression_string_red = tempPointer->redExAc;
            expression_string_green = tempPointer->greenExAc;
            expression_string_blue = tempPointer->blueExAc;
            expression_string_alpha  = tempPointer->alphaExAc;
            }
        }
    miP.redExpr = parseExpr<PF_FpShort>((void*)&miP, expression_string_red);
    if (miP.hasErrorB)
    {   miP.channelErrorstr = "red channel expression";
        suites.ANSICallbacksSuite1()->sprintf(out_data->return_msg,
                                              "Error in %s : %s",
                                              miP.channelErrorstr.c_str(),
                                              miP.errorstr.c_str());
    }
     miP.greenExpr = parseExpr<PF_FpShort>((void*)&miP, expression_string_green);
    if (miP.hasErrorB)
    {   miP.channelErrorstr = "green channel expression";
        suites.ANSICallbacksSuite1()->sprintf(out_data->return_msg,
                                              "Error in %s : %s",
                                              miP.channelErrorstr.c_str(),
                                              miP.errorstr.c_str());
    }
    miP.blueExpr = parseExpr<PF_FpShort>((void*)&miP, expression_string_blue);
    if (miP.hasErrorB)
    {   miP.channelErrorstr = "blue channel expression";
        suites.ANSICallbacksSuite1()->sprintf(out_data->return_msg,
                                              "Error in %s : %s",
                                              miP.channelErrorstr.c_str(),
                                              miP.errorstr.c_str());
    }
    miP.alphaExpr = parseExpr<PF_FpShort>((void*)&miP, expression_string_alpha);
    if (miP.hasErrorB)
    {   miP.channelErrorstr = "alpha channel expression";
        suites.ANSICallbacksSuite1()->sprintf(out_data->return_msg,
                                              "Error in %s : %s",
                                              miP.channelErrorstr.c_str(),
                                              miP.errorstr.c_str());
    }
    
	
	if (PF_WORLD_IS_DEEP(outputP)){
		ERR(suites.Iterate16Suite1()->iterate(	in_data,
												0,								// progress base
												linesL,							// progress final
												&params[MATH_INPUT]->u.ld,	// src
												NULL,							// area - null for all pixels
												(void*)&miP,					// refcon - your custom data pointer
												MySimpleGainFunc16,				// pixel function pointer
												outputP));
	}
	else {
        
        
		PF_Pixel8			*bop_outP = reinterpret_cast<PF_Pixel8*>(outputP->data), //main
			*bop_inP = reinterpret_cast<PF_Pixel8*>(inputP->data);
		A_long  in_gutterL = (inputP->rowbytes / sizeof(PF_Pixel8)) - inputP->width,
			out_gutterL = (outputP->rowbytes / sizeof(PF_Pixel8)) - outputP->width;

		PF_FpShort  red_result, green_result, blue_result, alpha_result;

		for ( A_long yL = 0; yL < outputP->height; yL++) {
			//3*3 matrix grp

			PF_Pixel *in00 = bop_inP - (inputP->rowbytes / sizeof(PF_Pixel)) - 1;//top left pixel in 3X3.
			PF_Pixel *in10 = in00 + 1;//top middle pixel in 3X3.
			PF_Pixel *in20 = in10 + 1;//top right pixel in 3X3.
			PF_Pixel *in01 = bop_inP - 1;//mid left pixel in 3X3.
			PF_Pixel *in21 = bop_inP + 1;//top right pixel in 3X3.
			PF_Pixel *in02 = bop_inP + (inputP->rowbytes / sizeof(PF_Pixel)) - 1;//bottom left pixel in 3X3.
			PF_Pixel *in12 = in02 + 1;//bottom middle pixel in 3X3.
			PF_Pixel *in22 = in12 + 1;//bottom right pixel in 3X3.


			for (A_long xL = 0; xL < inputP->width; xL++) {


				if (yL - 1 >= 0) {
					miP.m3P_red[0] = PF_FpShort(in00->red) / (PF_FpShort)PF_MAX_CHAN8;
					miP.m3P_green[0] = PF_FpShort(in00->green) / (PF_FpShort)PF_MAX_CHAN8;
					miP.m3P_blue[0] = PF_FpShort(in00->blue) / (PF_FpShort)PF_MAX_CHAN8;
					miP.m3P_alpha[0] = PF_FpShort(in00->alpha) / (PF_FpShort)PF_MAX_CHAN8;

					miP.m3P_red[1] = PF_FpShort(in10->red) / (PF_FpShort)PF_MAX_CHAN8;
					miP.m3P_green[1] = PF_FpShort(in10->green) / (PF_FpShort)PF_MAX_CHAN8;
					miP.m3P_blue[1] = PF_FpShort(in10->blue) / (PF_FpShort)PF_MAX_CHAN8;
					miP.m3P_alpha[1] = PF_FpShort(in10->alpha) / (PF_FpShort)PF_MAX_CHAN8;
					if (xL + 1 <= inputP->width) {
						miP.m3P_red[2] = PF_FpShort(in20->red) / (PF_FpShort)PF_MAX_CHAN8;
						miP.m3P_green[2] = PF_FpShort(in20->green) / (PF_FpShort)PF_MAX_CHAN8;
						miP.m3P_blue[2] = PF_FpShort(in20->blue) / (PF_FpShort)PF_MAX_CHAN8;
						miP.m3P_alpha[2] = PF_FpShort(in20->alpha) / (PF_FpShort)PF_MAX_CHAN8;
					}
					else {
						miP.m3P_red[2] = 0;
						miP.m3P_green[2] = 0;
						miP.m3P_blue[2] = 0;
						miP.m3P_alpha[2] = 0;
					}
				}
				miP.m3P_red[3] = PF_FpShort(in01->red) / (PF_FpShort)PF_MAX_CHAN8;
				miP.m3P_green[3] = PF_FpShort(in01->green) / (PF_FpShort)PF_MAX_CHAN8;
				miP.m3P_blue[3] = PF_FpShort(in01->blue) / (PF_FpShort)PF_MAX_CHAN8;
				miP.m3P_alpha[3] = PF_FpShort(in01->alpha) / (PF_FpShort)PF_MAX_CHAN8;

				miP.m3P_red[4] = PF_FpShort(bop_inP->red) / (PF_FpShort)PF_MAX_CHAN8;
				miP.m3P_green[4] = PF_FpShort(bop_inP->green) / (PF_FpShort)PF_MAX_CHAN8;
				miP.m3P_blue[4] = PF_FpShort(bop_inP->blue) / (PF_FpShort)PF_MAX_CHAN8;
				miP.m3P_alpha[4] = PF_FpShort(bop_inP->alpha) / (PF_FpShort)PF_MAX_CHAN8;
				if (xL + 1 <= inputP->height) {
					miP.m3P_red[5] = PF_FpShort(in21->red) / (PF_FpShort)PF_MAX_CHAN8;
					miP.m3P_green[5] = PF_FpShort(in21->green) / (PF_FpShort)PF_MAX_CHAN8;
					miP.m3P_blue[5] = PF_FpShort(in21->blue) / (PF_FpShort)PF_MAX_CHAN8;
					miP.m3P_alpha[5] = PF_FpShort(in21->alpha) / (PF_FpShort)PF_MAX_CHAN8;
				}
				else {
					miP.m3P_red[5] = 0;
					miP.m3P_green[5] = 0;
					miP.m3P_blue[5] = 0;
					miP.m3P_alpha[5] = 0;
				}
				if (yL + 1 <= inputP->height) {
					miP.m3P_red[6] = PF_FpShort(in02->red) / (PF_FpShort)PF_MAX_CHAN8;
					miP.m3P_green[6] = PF_FpShort(in02->green) / (PF_FpShort)PF_MAX_CHAN8;
					miP.m3P_blue[6] = PF_FpShort(in02->blue) / (PF_FpShort)PF_MAX_CHAN8;
					miP.m3P_alpha[6] = PF_FpShort(in02->alpha) / (PF_FpShort)PF_MAX_CHAN8;

					miP.m3P_red[7] = PF_FpShort(in12->red) / (PF_FpShort)PF_MAX_CHAN8;
					miP.m3P_green[7] = PF_FpShort(in12->green) / (PF_FpShort)PF_MAX_CHAN8;
					miP.m3P_blue[7] = PF_FpShort(in12->blue) / (PF_FpShort)PF_MAX_CHAN8;
					miP.m3P_alpha[7] = PF_FpShort(in12->alpha) / (PF_FpShort)PF_MAX_CHAN8;

					if (xL + 1 <= inputP->width) {
						miP.m3P_red[8] = PF_FpShort(in22->red) / (PF_FpShort)PF_MAX_CHAN8;
						miP.m3P_green[8] = PF_FpShort(in22->green) / (PF_FpShort)PF_MAX_CHAN8;
						miP.m3P_blue[8] = PF_FpShort(in22->blue) / (PF_FpShort)PF_MAX_CHAN8;
						miP.m3P_alpha[8] = PF_FpShort(in22->alpha) / (PF_FpShort)PF_MAX_CHAN8;
					}
					else {
						miP.m3P_red[8] = 0;
						miP.m3P_green[8] = 0;
						miP.m3P_blue[8] = 0;
						miP.m3P_alpha[8] = 0;
					}
				}

				AEFX_CLR_STRUCT(miP.xLF);
				miP.xLF = PF_FpShort(xL);
				AEFX_CLR_STRUCT(miP.yLF);
				miP.yLF = PF_FpShort(yL);
				AEFX_CLR_STRUCT(miP.luma);
				miP.luma = (0.2126*bop_inP->red + 0.7152*bop_inP->green + 0.0722*bop_inP->blue) / (PF_FpShort)PF_MAX_CHAN8;
				AEFX_CLR_STRUCT(miP.inAlphaF);
				miP.inAlphaF = (PF_FpShort)bop_inP->alpha / PF_MAX_CHAN8;
				AEFX_CLR_STRUCT(miP.inRedF);
				miP.inRedF = (PF_FpShort)bop_inP->red / PF_MAX_CHAN8;
				AEFX_CLR_STRUCT(miP.inGreenF);
				miP.inGreenF = (PF_FpShort)bop_inP->green / PF_MAX_CHAN8;
				AEFX_CLR_STRUCT(miP.inBlueF);
				miP.inBlueF = (PF_FpShort)bop_inP->blue / PF_MAX_CHAN8;
				AEFX_CLR_STRUCT(red_result);
				red_result = MIN(miP.redExpr(), 1);
				AEFX_CLR_STRUCT(green_result);
                green_result = MIN(miP.greenExpr(), 1);
				AEFX_CLR_STRUCT(blue_result);
                blue_result = MIN(miP.blueExpr(), 1);
				AEFX_CLR_STRUCT(alpha_result);
                alpha_result = MIN(miP.alphaExpr(), 1);

				bop_outP->alpha = A_u_char(alpha_result *PF_MAX_CHAN8);;
				bop_outP->red = A_u_char(red_result *PF_MAX_CHAN8);
				bop_outP->green = A_u_char(green_result *PF_MAX_CHAN8);
				bop_outP->blue = A_u_char(blue_result *PF_MAX_CHAN8);
				bop_outP++;
				bop_inP++;


				in00++;
				in10++;
				in20++;
				in01++;
				in21++;
				in02++;
				in12++;
				in22++;
			}
			if (yL >= 0 && yL < inputP->height) {
				bop_inP += in_gutterL;
			}

			bop_outP += out_gutterL;

			// Check for interrupt!
			if ((yL) && (err = PF_PROGRESS(in_data, yL + 1, outputP->height))) {
				return err;
			}
		}
    }
    PF_UNLOCK_HANDLE(arbH);
	return err;
}





static PF_Err
GlobalSetdown(
              PF_InData		*in_data)
{
    AEGP_SuiteHandler	suites(in_data->pica_basicP);
    
    if (in_data->global_data) {
        suites.HandleSuite1()->host_dispose_handle(in_data->global_data);
    }
    
    return PF_Err_NONE;
}


static PF_Err
HandleArbitrary(
                PF_InData			*in_data,
                PF_OutData			*out_data,
                PF_ParamDef			*params[],
                PF_LayerDef			*output,
                PF_ArbParamsExtra	*extra)
{
    PF_Err 	err 	= PF_Err_NONE;
    void 	*srcP	= NULL,
		 	*dstP	= NULL;
    
    switch (extra->which_function) {
            
        case PF_Arbitrary_NEW_FUNC:
            if (extra->u.new_func_params.refconPV != ARB_REFCON) {
                err = PF_Err_INTERNAL_STRUCT_DAMAGED;
            } else {
                err = CreateDefaultArb(	in_data,
                                       out_data,
                                       extra->u.new_func_params.arbPH);
            }
            break;
          
        case PF_Arbitrary_DISPOSE_FUNC:
            if (extra->u.dispose_func_params.refconPV != ARB_REFCON) {
                err = PF_Err_INTERNAL_STRUCT_DAMAGED;
            } else {
                PF_DISPOSE_HANDLE(extra->u.dispose_func_params.arbH);
            }
            break;
            
        case PF_Arbitrary_COPY_FUNC:
            if(extra->u.copy_func_params.refconPV == ARB_REFCON) {
                ERR(CreateDefaultArb(	in_data,
                                     out_data,
                                     extra->u.copy_func_params.dst_arbPH));
                
                ERR(Arb_Copy(in_data,
                             out_data,
                             &extra->u.copy_func_params.src_arbH,
                             extra->u.copy_func_params.dst_arbPH));
            }
            break;
           
        case PF_Arbitrary_FLAT_SIZE_FUNC:
            *(extra->u.flat_size_func_params.flat_data_sizePLu) = sizeof(m_ArbData);
            break;
            
        case PF_Arbitrary_FLATTEN_FUNC:

            if(extra->u.flatten_func_params.buf_sizeLu == sizeof(m_ArbData)){
                srcP = (m_ArbData*)PF_LOCK_HANDLE(extra->u.flatten_func_params.arbH);
                dstP = extra->u.flatten_func_params.flat_dataPV;
                if (srcP){
                    memcpy(dstP,srcP,sizeof(m_ArbData));
                }
                PF_UNLOCK_HANDLE(extra->u.flatten_func_params.arbH);
            }
            break;
            
        case PF_Arbitrary_UNFLATTEN_FUNC:
            if(extra->u.unflatten_func_params.buf_sizeLu == sizeof(m_ArbData)){
                PF_Handle	handle = PF_NEW_HANDLE(sizeof(m_ArbData));
                dstP = (m_ArbData*)PF_LOCK_HANDLE(handle);
                srcP = (void*)extra->u.unflatten_func_params.flat_dataPV;
                if (srcP){
                    memcpy(dstP,srcP,sizeof(m_ArbData));
                }
                *(extra->u.unflatten_func_params.arbPH) = handle;
                PF_UNLOCK_HANDLE(handle);
            }
            break;
            
        case PF_Arbitrary_INTERP_FUNC:
            if(extra->u.interp_func_params.refconPV == ARB_REFCON) {
                ERR(CreateDefaultArb(	in_data,
                                     out_data,
                                     extra->u.interp_func_params.interpPH));
                
                ERR(Arb_Interpolate(	in_data,
                                    out_data,
                                    extra->u.interp_func_params.tF,
                                    &extra->u.interp_func_params.left_arbH,
                                    &extra->u.interp_func_params.right_arbH,
                                    extra->u.interp_func_params.interpPH));
            }
            break;
            
        case PF_Arbitrary_COMPARE_FUNC:
            ERR(Arb_Compare(	in_data,
                            out_data,
                            &extra->u.compare_func_params.a_arbH,
                            &extra->u.compare_func_params.b_arbH,
                            extra->u.compare_func_params.compareP));
            break;
            
             
        case PF_Arbitrary_PRINT_SIZE_FUNC:
            err = PF_Err_UNRECOGNIZED_PARAM_TYPE;
            break;
                case PF_Arbitrary_PRINT_FUNC:
            
            if (extra->u.print_func_params.refconPV == ARB_REFCON) {
                ERR(Arb_Print(in_data,
                              out_data,
                              extra->u.print_func_params.print_flags,
                              extra->u.print_func_params.arbH,
                              extra->u.print_func_params.print_sizeLu,
                              extra->u.print_func_params.print_bufferPC));
            } else {
                err = PF_Err_UNRECOGNIZED_PARAM_TYPE;
            }
            break;
            
        case PF_Arbitrary_SCAN_FUNC:
            if (extra->u.scan_func_params.refconPV == ARB_REFCON) {
                ERR(Arb_Scan(	in_data,
                             out_data,
                             extra->u.scan_func_params.refconPV,
                             extra->u.scan_func_params.bufPC,
                             extra->u.scan_func_params.bytes_to_scanLu,
                             extra->u.scan_func_params.arbPH));
            } else {
                err = PF_Err_UNRECOGNIZED_PARAM_TYPE;
            }
            break;
    }
    return err;
}

static PF_Err
RespondtoAEGP (
               PF_InData		*in_data,
               PF_OutData		*out_data,
               PF_ParamDef		*params[],
               PF_LayerDef		*output,
               void*			extraP)
{
    PF_Err			err = PF_Err_NONE;
    
    AEGP_SuiteHandler suites(in_data->pica_basicP);
    
    suites.ANSICallbacksSuite1()->sprintf(	out_data->return_msg,
                                          "%s",
                                          reinterpret_cast<A_char*>(extraP));
    
    return err;
}



DllExport	
PF_Err 
EntryPointFunc (
	PF_Cmd			cmd,
	PF_InData		*in_data,
	PF_OutData		*out_data,
	PF_ParamDef		*params[],
	PF_LayerDef		*output,
	void			*extra)
{
	PF_Err		err = PF_Err_NONE;
	
	try {
		switch (cmd) {
			case PF_Cmd_ABOUT:

				err = About(in_data,
							out_data,
							params,
							output);
				break;
				
			case PF_Cmd_GLOBAL_SETUP:

				err = GlobalSetup(	in_data,
									out_data,
									params,
									output);
				break;
                
            case PF_Cmd_GLOBAL_SETDOWN:
                err = GlobalSetdown(in_data);
                break;
                
            case PF_Cmd_PARAMS_SETUP:
                
                err = ParamsSetup(	in_data,
                                  out_data,
                                  params,
                                  output);
                break;
                
            case PF_Cmd_ARBITRARY_CALLBACK:
                err = HandleArbitrary(	in_data, out_data, params, output, reinterpret_cast<PF_ArbParamsExtra*>(extra));
                break;
            
            case PF_Cmd_COMPLETELY_GENERAL:
                err = RespondtoAEGP(in_data,
                                    out_data,
                                    params,
                                    output,
                                    extra);
                break;
                
                
            case PF_Cmd_DO_DIALOG:
                    err = PopDialog(in_data,out_data,params,output);
                break;

				
			case PF_Cmd_RENDER:

				err = Render(	in_data,
								out_data,
								params,
								output);
				break;
            case PF_Cmd_UPDATE_PARAMS_UI:
                err = UpdateParameterUI(	in_data,
                                        out_data,
                                        params,
                                        output);
                break;
            
		}
	}
	catch(PF_Err &thrown_err){
		err = thrown_err;
	}
	return err;
}

