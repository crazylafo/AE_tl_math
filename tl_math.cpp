
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
                           PF_OutFlag_NON_PARAM_VARY|
                           PF_OutFlag_SEQUENCE_DATA_NEEDS_FLATTENING;
    
    out_data->out_flags2 =  PF_OutFlag2_AE13_5_THREADSAFE	|
                            PF_OutFlag2_SUPPORTS_GET_FLATTENED_SEQUENCE_DATA;
    
    
    
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
	const A_Time currT = { 0,100 };
	


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



static PF_Err
PopDialog (
           PF_InData		*in_data,
           PF_OutData		*out_data,
           PF_ParamDef		*params[],
           PF_LayerDef		*output )
{
    PF_Err err = PF_Err_NONE;
    AEGP_SuiteHandler	suites(in_data->pica_basicP);
    my_global_dataP		globP				= reinterpret_cast<my_global_dataP>(DH(out_data->global_data));

    AEGP_MemHandle     resultMemH          =     NULL;
    A_char *resultAC =     NULL;
    A_char          scriptAC[4096] = {'\0'};
    
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
            tempRedS.append(arbInP->redExAc);
            tempGreenS.append(arbInP->greenExAc);
            tempBlueS.append(arbInP->blueExAc);
            tempAlphaS.append(arbInP->alphaExAc);
        }
       
    }

    tempRedS.append("'");
    tempGreenS.append("'");
    tempBlueS.append("'");
    tempAlphaS.append("'");


    A_char   SET_EXPR_SCRIPT [4096] = "function expr(redExpr,GreenExpr,BlueExpr,AlphaExpr) {\
    var w = new Window('dialog', 'Maths Expressions', undefined, {resizeable:true} );\
    w.sttxt= w.add ('statictext', undefined, 'Write here your math operations for each channels. Math operations are based on C++ Mathematical Expression Toolkit Library');\
    w.grp = w.add('group');\
    w.grp.orientation='column';\
    w.grp.alignment = ['fill', 'fill'];\
    w.grp.alignChildren = ['fill', 'fill'];\
    w.grp.redC = w.grp.add('group');\
    w.grp.redC.orientation = 'row';\
    w.grp.redC.alignment = ['fill', 'fill'];\
    w.grp.redC.alignChildren = ['fill', 'fill'];\
    w.grp.redC.redst = w.grp.redC.add ('statictext', undefined,'Red Expr    :');\
    w.grp.redC.redet = w.grp.redC.add ('edittext', undefined, redExpr);\
    w.grp.greenC = w. grp.add('group');\
    w.grp.greenC.orientation = 'row';\
    w.grp.greenC.alignChildren = ['fill', 'fill'];\
    w.grp.greenC.greenst = w.grp.greenC.add ('statictext', undefined,'Green Expr :');\
    w.grp.greenC.greenet = w.grp.greenC.add ('edittext', undefined, GreenExpr);\
    w.grp.blueC = w.grp.add('group');\
    w.grp.blueC.orientation = 'row';\
    w.grp.blueC.alignChildren = ['fill', 'fill'];\
    w.grp.blueC.bluest = w.grp.blueC.add ('statictext', undefined, 'Blue Expr   :');\
    w.grp.blueC.blueet = w.grp.blueC.add ('edittext', undefined, BlueExpr);\
    w.grp.alphaC = w.grp.add('group');\
    w.grp.alphaC.orientation = 'row';\
    w.grp.alphaC.alignChildren = ['fill', 'fill'];\
    w.grp.alphaC.alphast = w.grp.alphaC.add ('statictext', undefined, 'Alpha Expr :');\
    w.grp.alphaC.alphaet = w.grp.alphaC.add ('edittext', undefined, AlphaExpr);\
    w.grp.btnGrp = w.grp.add('Group');\
    w.grp.btnGrp.orientation = 'row';\
    w.grp.btnGrp.Ok =w.grp.btnGrp.add ('button', undefined, 'ok');\
    w.grp.btnGrp.Cancel =w.grp.btnGrp.add ('button', undefined, 'Cancel');\
    var result = '';\
    w.grp.btnGrp.Ok.onClick = function(){\
        var strExpr ='rfromJS'+w.grp.redC.redet.text+'gfromJS'+w.grp.greenC.greenet.text+'bfromJS'+w.grp.blueC.blueet.text+'afromJS'+w.grp.alphaC.alphaet.text ;\
        w.close();\
        result = strExpr;\
    }\
    w.grp.btnGrp.Cancel.onClick = function(){\
        var ret ='rfromJS'+redExpr+'gfromJS'+GreenExpr+'bfromJS'+BlueExpr+'afromJS'+AlphaExpr;\
        w.close();\
        result = ret;\
    }\
    w.onResizing = w.onResize = function(){this.layout.resize();}\
    w.show();\
    return result\
    }\
    expr(%s,%s,%s,%s);";

    sprintf( scriptAC, SET_EXPR_SCRIPT,tempRedS.c_str() , tempGreenS.c_str() , tempBlueS.c_str() , tempAlphaS.c_str() );
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
        
            std::string redResultStr =resultStr.substr(redPos+7, greenPos -redPos-7);
            std::string greenResultStr = resultStr.substr(greenPos+7, bluePos-greenPos-7);
            std::string blueResultStr = resultStr.substr(bluePos+7, alphaPos-bluePos-7);
            std::string alphaResultStr; resultStr.substr(alphaPos+7);
        
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



static PF_Err 
Render (
	PF_InData		*in_data,
	PF_OutData		*out_data,
	PF_ParamDef		*params[],
	PF_LayerDef		*outputP )
{
	PF_Err				err		= PF_Err_NONE;
	AEGP_SuiteHandler	suites(in_data->pica_basicP);
	
    //Unflat_Seq_Data seqP = *reinterpret_cast<Unflat_Seq_Data*>(DH(in_data->sequence_data));
	/*	Put interesting code here. */
	MathInfo			miP;
	AEFX_CLR_STRUCT(miP);
	A_long				linesL	= 0;
	linesL 		= outputP->extent_hint.bottom - outputP->extent_hint.top;
	PF_EffectWorld		*inputP = &params[0]->u.ld;
    
    PF_Handle		arbH			= params[MATH_ARB_DATA]->u.arb_d.value;
    m_ArbData		*arbP			= NULL;

    
	miP.RedIF	= params[MATH_INPONE_VAR]->u.fs_d.value;
    miP.GreenIF	= params[MATH_INPTWO_VAR]->u.fs_d.value;
    miP.BlueIF	= params[MATH_INPTHREE_VAR]->u.fs_d.value;
    miP.AlphaIF	= params[MATH_INPFOUR_VAR]->u.fs_d.value;
   
    //layer size
    miP.scale_x = in_data->downsample_x.num*in_data->pixel_aspect_ratio.num/ (float)in_data->downsample_x.den,
    miP.scale_y = in_data->downsample_y.num*in_data->pixel_aspect_ratio.den/ (float)in_data->downsample_y.den;
    miP.layerWidthF = PF_FpLong (in_data->width*miP.scale_x) ;
    miP.layerHeightF =PF_FpLong (in_data->height* miP.scale_y);
    
    //time params
    miP.layerTime_sec = PF_FpLong(in_data->current_time/in_data->time_scale);
    miP.layerTime_frame = PF_FpLong(in_data->current_time/in_data->time_step);
    miP.layerDuration =PF_FpLong( in_data->total_time / in_data->time_scale);
    
	miP.xLF = 0;
	miP.yLF = 0;

    // TO DO: ADD TO GET THE COMP FPS ERR(suites.CompSuite10()->AEGP_GetCompFramerate(compPH, &fpsPF));

    std::string expression_string_Safe = "1";
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
    

    symbol_table_t symbol_table;
  
    symbol_table.add_variable("xLF",  miP.xLF);
    symbol_table.add_variable("yLF",  miP.yLF);
	symbol_table.add_variable("inP_Red", miP.inRedF);
	symbol_table.add_variable("inP_Green", miP.inGreenF);
	symbol_table.add_variable("inP_Blue", miP.inBlueF);
	symbol_table.add_variable("inP_Alpha", miP.inAlphaF);
    symbol_table.add_constants();
    symbol_table.add_constant("layerWidth",miP.layerWidthF);
    symbol_table.add_constant("layerHeight",miP.layerHeightF);
    symbol_table.add_constant("layerTime_sec",miP.layerTime_sec);
    symbol_table.add_constant("layerTime_frame",miP.layerTime_frame);
    symbol_table.add_constant("layerDuration",miP.layerDuration);
    
    parser_t parser;
	expression_t    expression_t_red;
	expression_t    expression_t_green;
	expression_t    expression_t_blue;
	expression_t    expression_t_alpha;

	expression_t_red.register_symbol_table(symbol_table);
	expression_t_green.register_symbol_table(symbol_table);
	expression_t_blue.register_symbol_table(symbol_table);
	expression_t_alpha.register_symbol_table(symbol_table);

    parser.compile(expression_string_red, expression_t_red);
	parser.compile(expression_string_green, expression_t_green);
	parser.compile(expression_string_blue, expression_t_blue);
	parser.compile(expression_string_alpha, expression_t_alpha);
    //if error in expression
    if (!parser.compile(expression_string_red, expression_t_red))
    {
        parser.compile(expression_string_Safe, expression_t_red);
    }
	if (!parser.compile(expression_string_green, expression_t_green))
	{
		parser.compile(expression_string_Safe, expression_t_green);
	}
	if (!parser.compile(expression_string_blue, expression_t_blue))
	{
		parser.compile(expression_string_Safe, expression_t_blue);
	}
	if (!parser.compile(expression_string_alpha, expression_t_alpha))
	{
		parser.compile(expression_string_Safe, expression_t_alpha);
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
		// rewrite the itiration to safe access to math iteration with xL and yL values.
		PF_Pixel8			*bop_outP = reinterpret_cast<PF_Pixel8*>(outputP->data),
							*bop_inP = reinterpret_cast<PF_Pixel8*>(inputP->data);
		A_long  in_gutterL = (inputP->rowbytes / sizeof(PF_Pixel8)) - inputP->width,
				out_gutterL = (outputP->rowbytes / sizeof(PF_Pixel8)) - outputP->width;
		PF_FpLong  red_result, green_result, blue_result, alpha_result;

		for (register A_long yL = 0; yL < outputP->height; yL++) {
			for (register A_long xL = 0; xL < inputP->width; xL++) {
				AEFX_CLR_STRUCT(miP.xLF);
				miP.xLF = xL;
				AEFX_CLR_STRUCT(miP.yLF);
				miP.yLF = yL;
				AEFX_CLR_STRUCT(miP.inAlphaF);
				miP.inAlphaF = (PF_FpLong)bop_inP->alpha / PF_MAX_CHAN8;
				AEFX_CLR_STRUCT(miP.inRedF);
				miP.inRedF = (PF_FpLong)bop_inP->red / PF_MAX_CHAN8;
				AEFX_CLR_STRUCT(miP.inGreenF);
				miP.inGreenF = (PF_FpLong)bop_inP->green / PF_MAX_CHAN8;
				AEFX_CLR_STRUCT(miP.inBlueF);
				miP.inBlueF = (PF_FpLong)bop_inP->blue / PF_MAX_CHAN8;
				AEFX_CLR_STRUCT(red_result);
				red_result = MIN(expression_t_red.value(), 1);
				AEFX_CLR_STRUCT(green_result);
				green_result = MIN(expression_t_green.value(), 1);
				AEFX_CLR_STRUCT(blue_result);
				blue_result = MIN(expression_t_blue.value(), 1);
				AEFX_CLR_STRUCT(alpha_result);
				alpha_result = MIN(expression_t_alpha.value(), 1);
				bop_outP->alpha = A_u_char(alpha_result *PF_MAX_CHAN8);;
				bop_outP->red = A_u_char(red_result *PF_MAX_CHAN8);
				bop_outP->green = A_u_char(green_result *PF_MAX_CHAN8);
				bop_outP->blue = A_u_char(blue_result *PF_MAX_CHAN8);
				bop_outP++;
				bop_inP++;
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

