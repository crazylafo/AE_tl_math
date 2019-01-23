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
											"%s v%d.%d.%d\r%s",
											STR(StrID_Name), 
											MAJOR_VERSION, 
											MINOR_VERSION,
                                            BUG_VERSION,
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
                           PF_OutFlag_SEND_UPDATE_PARAMS_UI	|
                           PF_OutFlag_DEEP_COLOR_AWARE|	// just 16bpc, not 32bpc
                           PF_OutFlag_I_DO_DIALOG|
                           PF_OutFlag_NON_PARAM_VARY;


	out_data->out_flags2 = PF_OutFlag2_SUPPORTS_QUERY_DYNAMIC_FLAGS;
    

    
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
    ERR(CreateDefaultArb(in_data,
                         out_data,
                         &def.u.arb_d.dephault));
    
    
    PF_ADD_ARBITRARY2(	"preset Name",
                      10,
                      10,
                      PF_ParamFlag_SUPERVISE| PF_ParamFlag_CANNOT_TIME_VARY,
                      PF_PUI_TOPIC,
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

    PF_ADD_TOPIC(STR( StrID_TOPIC_SLIDER_Param_Name),  MATH_TOPIC_SLIDER_DISK_ID);
    
    AEFX_CLR_STRUCT(def);
    PF_ADD_FLOAT_SLIDERX(	STR(StrID_INPUTONE_Param_Name),
							MATH_VAR_MIN,
							MATH_VAR_MAX,
							MATH_VAR_MIN,
							MATH_SLIDER_MAX,
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
                         MATH_SLIDER_MAX,
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
                         MATH_SLIDER_MAX,
                         MATH_VAR_DFLT,
                         PF_Precision_THOUSANDTHS,
                         0,
                         0,
                         MATH_INPTHREE_VAR_DISK_ID);
    
    AEFX_CLR_STRUCT(def);
    
    PF_ADD_FLOAT_SLIDERX(STR( StrID_INPUTFOUR_Param_Name),
                         MATH_VAR_MIN,
                         MATH_VAR_MAX,
                         MATH_VAR_MIN,
                         MATH_SLIDER_MAX,
                         MATH_VAR_DFLT,
                         PF_Precision_THOUSANDTHS,
                         0,
                         0,
                         MATH_INPFOUR_VAR_DISK_ID);
    
    AEFX_CLR_STRUCT(def);
    PF_END_TOPIC( MATH_TOPIC_SLIDER_DISK_ID);

    AEFX_CLR_STRUCT(def);
    

    PF_ADD_TOPIC(STR( StrID_TOPIC_POINTS_Param_Name), MATH_TOPIC_POINTS_DISK_ID);
	AEFX_CLR_STRUCT(def);
    PF_ADD_POINT(STR (strID_INPUTPOINTONE_Param_Name), 50, 50, FALSE, MATH_INP_POINT_ONE_DISK_ID);
    AEFX_CLR_STRUCT(def);
    PF_ADD_POINT(STR(strID_INPUTPOINTTWO_Param_Name), 50, 50, FALSE, MATH_INP_POINT_TWO_DISK_ID);
    AEFX_CLR_STRUCT(def);
    PF_END_TOPIC(MATH_TOPIC_POINTS_DISK_ID);
    AEFX_CLR_STRUCT(def);
    
    PF_ADD_TOPIC(STR( StrID_TOPIC_COLORS_Param_Name), MATH_TOPIC_COLORS_DISK_ID);
    AEFX_CLR_STRUCT(def);
    PF_ADD_COLOR(STR(strID_INPUTCOLORONE_Param_Name), PF_MAX_CHAN8, PF_MAX_CHAN8, PF_MAX_CHAN8, MATH_INP_COLOR_ONE_DISK_ID);
    AEFX_CLR_STRUCT(def);
    PF_ADD_COLOR(STR(strID_INPUTCOLORTWO_Param_Name), PF_MAX_CHAN8, PF_MAX_CHAN8, PF_MAX_CHAN8,MATH_INP_COLOR_TWO_DISK_ID);
    AEFX_CLR_STRUCT(def);
    PF_END_TOPIC (MATH_TOPIC_COLORS_DISK_ID);
    AEFX_CLR_STRUCT(def);
    
    PF_ADD_TOPIC(STR( StrID_TOPIC_INPUTS_Param_Name), MATH_TOPIC_INPUTS_DISK_ID);
    AEFX_CLR_STRUCT(def);

    PF_ADD_LAYER(STR(StrID_LAYER_ONE_Param_Name), PF_LayerDefault_MYSELF, MATH_INP_LAYER_ONE_DISK_ID);
    AEFX_CLR_STRUCT(def);
    PF_ADD_FLOAT_SLIDERX(STR( StrID_TOFF_ONE_Param_Name),
                         TIMEOFFSET_MIN,
                         TIMEOFFSET_MAX,
                         MATH_VAR_MIN,
                         MATH_SLIDER_MAX,
                         TIMEOFFSET_DFLT,
                         PF_Precision_INTEGER,
                         0,
                         0,
                         MATH_INP_TOFF_ONE_DISK_ID);
    
    PF_ADD_POINT(STR(StrID_POFF_ONE_Param_Name), 50, 50, FALSE, MATH_INP_POFF_ONE_DISK_ID);
    AEFX_CLR_STRUCT(def);

    PF_END_TOPIC(MATH_TOPIC_INPUTS_DISK_ID);
    AEFX_CLR_STRUCT(def);

    
    out_data->num_params = MATH_NUM_PARAMS;
    
    AEFX_SuiteScoper<PF_EffectUISuite1> effect_ui_suiteP = AEFX_SuiteScoper<PF_EffectUISuite1>(
                                                                                               in_data,
                                                                                               kPFEffectUISuite,
                                                                                               kPFEffectUISuiteVersion1,
                                                                                               out_data);
    
    ERR(effect_ui_suiteP->PF_SetOptionsButtonName(in_data->effect_ref, "Expressions"));


	return err;
}

static PF_Err
MakeParamCopy(
              PF_ParamDef *actual[],	/* >> */
              PF_ParamDef copy[])		/* << */
{
    for (A_short iS = 0; iS < MATH_NUM_PARAMS; ++iS){
        AEFX_CLR_STRUCT(copy[iS]);	// clean params are important!
    }
    copy[MATH_INPUT]			= *actual[MATH_INPUT];
    copy[MATH_ARB_DATA]			= *actual[MATH_ARB_DATA];
    
    return PF_Err_NONE;
    
}
static PF_Err
UpdateParameterUI(
                  PF_InData			*in_data,
                  PF_OutData			*out_data,
                  PF_ParamDef			*params[],
                  PF_LayerDef			*outputP)
{
    PF_Err				err					= PF_Err_NONE;
    PF_Handle		arbH			= params[MATH_ARB_DATA]->u.arb_d.value;
    AEGP_SuiteHandler		suites(in_data->pica_basicP);
    m_ArbData		*arbP			= NULL;
    PF_ParamDef		param_copy[MATH_NUM_PARAMS];
    ERR(MakeParamCopy(params, param_copy));
    arbP = reinterpret_cast<m_ArbData*>(suites.HandleSuite1()->host_lock_handle(arbH));
    if (arbP) {
        m_ArbData *tempPointer = reinterpret_cast<m_ArbData*>(arbP);
        strcpy(param_copy[MATH_ARB_DATA].name, tempPointer->presetNameAc);
        ERR(suites.ParamUtilsSuite3()->PF_UpdateParamUI(in_data->effect_ref,
                                                        MATH_ARB_DATA,
                                                        &param_copy[MATH_ARB_DATA]));
        
    }
    
    
    PF_UNLOCK_HANDLE(arbH);
    return err;
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
Render (
                PF_InData		*in_data,
                PF_OutData		*out_data,
                PF_ParamDef		*params[],
                PF_LayerDef		*outputP )
{
	PF_Err				err = PF_Err_NONE, err2= PF_Err_NONE;
	AEGP_SuiteHandler	suites(in_data->pica_basicP);
    PF_EffectWorld		*inputP = &params[0]->u.ld;
    PF_Point			origin;
    
    PF_Handle		arbH			= params[MATH_ARB_DATA]->u.arb_d.value;
    m_ArbData		*arbP			= NULL;
    AEGP_LayerH		layerH;
    AEGP_CompH		compH;
    AEGP_ItemH      itemH;
    OffInfo         oiP;
    AEFX_CLR_STRUCT(oiP);
	funcTransfertInfo fiP;
	AEFX_CLR_STRUCT(fiP);
    MathInfo		miP;
	AEFX_CLR_STRUCT(miP);
	FlagsInfo      flagsP;
	AEFX_CLR_STRUCT(flagsP);
    

    
    PF_ParamDef checkoutLayerOne;
    AEFX_CLR_STRUCT(checkoutLayerOne);

    PF_Pixel16 empty16 = {0,0,0,0};
    PF_Pixel empty8 = {0,0,0,0};

	std::string expression_string_red = "1";
	std::string expression_string_green = "1";
	std::string expression_string_blue = "1";
	std::string expression_string_alpha = "1";
    std::string expression_string_funcOne = "1";
    std::string expression_string_funcTwo = "1";
     std::string expression_string_funcThree = "1";
	if (!err) {
		arbP = reinterpret_cast<m_ArbData*>(suites.HandleSuite1()->host_lock_handle(arbH));
		if (arbP) {
			m_ArbData *tempPointer = reinterpret_cast<m_ArbData*>(arbP);
			expression_string_red = tempPointer->redExAc;
			expression_string_green = tempPointer->greenExAc;
			expression_string_blue = tempPointer->blueExAc;
            expression_string_alpha = tempPointer->alphaExAc;
            expression_string_funcOne = tempPointer-> functionOneAc;
            expression_string_funcTwo =  tempPointer->functionTwoAc;
            expression_string_funcThree =  tempPointer->functionThreeAc;

			flagsP.PixelsCallExternalInputB = tempPointer->PixelsCallExternalInputB;
			flagsP.PresetHasWideInput = tempPointer->PresetHasWideInputB;
			flagsP.NeedsPixelAroundB = tempPointer->NeedsPixelAroundB;
			flagsP.NeedsLumaB = tempPointer->NeedsLumaB;
            flagsP.CallsAEGP_CompB = tempPointer-> CallsAEGP_CompB;
            flagsP.CallsAEGP_layerB = tempPointer->CallsAEGP_layerB;
            flagsP.UsesFunctionsB = tempPointer->UsesFunctionsB;

		}
	}

    ERR(PF_CHECKOUT_PARAM(in_data,
                          MATH_INP_LAYER_ONE,
                          (in_data->current_time + A_long(params[MATH_INP_TOFF_ONE]->u.fs_d.value) * in_data->time_step),
                          in_data->time_step,
                          in_data->time_scale,
                          &checkoutLayerOne));
    
	if (flagsP.PixelsCallExternalInputB) {

        
        PF_EffectWorld Externalworld;
		Externalworld = *outputP;
		miP.extLW = *outputP;

		if (PF_WORLD_IS_DEEP(outputP)) {
			ERR(suites.FillMatteSuite2()->fill16(in_data->effect_ref,
				&empty16,
				NULL,
				&Externalworld));
		}
		else {
			ERR(suites.FillMatteSuite2()->fill(in_data->effect_ref,
				&empty8,
				NULL,
				&Externalworld));
		}

		if (checkoutLayerOne.u.ld.data) {
			if (PF_Quality_HI == in_data->quality) {
				ERR(suites.WorldTransformSuite1()->copy_hq(in_data->effect_ref,
					&checkoutLayerOne.u.ld,
					&Externalworld,
					&checkoutLayerOne.u.ld.extent_hint,
					&Externalworld.extent_hint));
			}
			else {
				ERR(suites.WorldTransformSuite1()->copy(in_data->effect_ref,
					&checkoutLayerOne.u.ld,
					&Externalworld,
					&checkoutLayerOne.u.ld.extent_hint,
					&Externalworld.extent_hint));
			}

		}
		oiP.x_offFi = PF_Fixed (((long)inputP->width << 15) - params[MATH_INP_POFF_ONE]->u.td.x_value);
		oiP.y_offFi = PF_Fixed (((long)inputP->height << 15) - params[MATH_INP_POFF_ONE]->u.td.y_value);

		if (oiP.x_offFi != 0 || oiP.y_offFi != 0) {
			oiP.in_data = *in_data;
			oiP.samp_pb.src = inputP;
			origin.h = (A_short)(in_data->output_origin_x);
			origin.v = (A_short)(in_data->output_origin_y);
			if (PF_WORLD_IS_DEEP(outputP)) {
				ERR(suites.Iterate16Suite1()->iterate_origin(in_data,
					0,
					Externalworld.height,
					&Externalworld,
					&Externalworld.extent_hint,
					&origin,
					(void*)(&oiP),
					ShiftImage16,
					&miP.extLW));
			}
			else {
				ERR(suites.Iterate8Suite1()->iterate_origin(in_data,
					0,
					Externalworld.height,
					&Externalworld,
					&Externalworld.extent_hint,
					&origin,
					(void*)(&oiP),
					ShiftImage8,
					&miP.extLW));
			}

		}
		else {
			miP.extLW = Externalworld;
		}

	}
    //LOADING SUITES TO ACCESS COMP PARAMS
    if ( flagsP.CallsAEGP_CompB ||flagsP.CallsAEGP_layerB  ){
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

    }
    //layer size
    miP.scale_x = in_data->downsample_x.num*in_data->pixel_aspect_ratio.num/ (float)in_data->downsample_x.den;
    miP.scale_y = in_data->downsample_y.num*in_data->pixel_aspect_ratio.den/ (float)in_data->downsample_y.den;
    miP.layerWidthF = PF_FpShort (in_data->width*miP.scale_x) ;
    miP.layerHeightF = PF_FpShort(in_data->height* miP.scale_y);

    //time params
    miP.layerTime_Sec = PF_FpShort(in_data->current_time)/PF_FpShort(in_data->time_scale);
    miP.layerTime_Frame = PF_FpShort(in_data->current_time/ (float)in_data->time_step);
    miP.layerDuration =PF_FpShort( in_data->total_time / in_data->time_scale);

    
	miP.inOneF	= params[MATH_INPONE_VAR]->u.fs_d.value;
    miP.inTwoF	= params[MATH_INPTWO_VAR]->u.fs_d.value;
    miP.inThreeF= params[MATH_INPTHREE_VAR]->u.fs_d.value;
    miP.inFourF	= params[MATH_INPFOUR_VAR]->u.fs_d.value;
   

    
    //user param points
    miP.pointOneX = static_cast<PF_FpShort>(round(FIX_2_FLOAT(params[MATH_INP_POINT_ONE]->u.td.x_value)));
    miP.pointOneY =static_cast<PF_FpShort>(round(FIX_2_FLOAT(params[MATH_INP_POINT_ONE]->u.td.y_value)));
    miP.pointTwoX = static_cast<PF_FpShort>(round(FIX_2_FLOAT(params[MATH_INP_POINT_TWO]->u.td.x_value)));
    miP.pointTwoY =static_cast<PF_FpShort>(round(FIX_2_FLOAT(params[MATH_INP_POINT_TWO]->u.td.y_value)));
    
    //user param color
    miP.colorOne[0] = PF_FpShort (params[MATH_INP_COLOR_ONE]->u.cd.value.red)/ PF_FpShort (PF_MAX_CHAN8);
    miP.colorOne[1] = PF_FpShort (params[MATH_INP_COLOR_ONE]->u.cd.value.green)/ PF_FpShort (PF_MAX_CHAN8);
    miP.colorOne[2] = PF_FpShort (params[MATH_INP_COLOR_ONE]->u.cd.value.blue)/ PF_FpShort (PF_MAX_CHAN8);
    
    miP.colorTwo[0] = PF_FpShort (params[MATH_INP_COLOR_TWO]->u.cd.value.red)/ PF_FpShort (PF_MAX_CHAN8);
    miP.colorTwo[1] = PF_FpShort (params[MATH_INP_COLOR_TWO]->u.cd.value.green)/ PF_FpShort (PF_MAX_CHAN8);
    miP.colorTwo[2] = PF_FpShort (params[MATH_INP_COLOR_TWO]->u.cd.value.blue)/ PF_FpShort (PF_MAX_CHAN8);
	miP.xLF = 0;
	miP.yLF = 0;


    fiP.UsesFunctionsB = false;
    if (flagsP.UsesFunctionsB){
        fiP.UsesFunctionsB = true;
        fiP.func1str=expression_string_funcOne;
        fiP.func2str=expression_string_funcTwo;
        fiP.func3str=expression_string_funcThree;
    }

    fiP.redExpr = parseExpr<PF_FpShort>((void*)&miP, &fiP, expression_string_red);
    if (fiP.hasErrorB)
    {   fiP.channelErrorstr = "red channel expression";
        suites.ANSICallbacksSuite1()->sprintf(out_data->return_msg,
                                              "Error in %s : %s",
                                              fiP.channelErrorstr.c_str(),
                                              fiP.errorstr.c_str());
    }
    fiP.greenExpr = parseExpr<PF_FpShort>((void*)&miP, &fiP, expression_string_green);
    if (fiP.hasErrorB)
    {   fiP.channelErrorstr = "green channel expression";
        suites.ANSICallbacksSuite1()->sprintf(out_data->return_msg,
                                              "Error in %s : %s",
                                              fiP.channelErrorstr.c_str(),
                                              fiP.errorstr.c_str());
    }
    fiP.blueExpr = parseExpr<PF_FpShort>((void*)&miP, &fiP, expression_string_blue);
    if (fiP.hasErrorB)
    {   fiP.channelErrorstr = "blue channel expression";
        suites.ANSICallbacksSuite1()->sprintf(out_data->return_msg,
                                              "Error in %s : %s",
                                              fiP.channelErrorstr.c_str(),
                                              fiP.errorstr.c_str());
    }
    fiP.alphaExpr = parseExpr<PF_FpShort>((void*)&miP, &fiP, expression_string_alpha);
    if (fiP.hasErrorB)
    {   fiP.channelErrorstr = "alpha channel expression";
        suites.ANSICallbacksSuite1()->sprintf(out_data->return_msg,
                                              "Error in %s : %s",
                                              fiP.channelErrorstr.c_str(),
                                              fiP.errorstr.c_str());
    }

    
    
    
    std::vector<std::thread> workers_thrds;
    A_long part_length, lastPart_length, num_thrd;
	AEFX_CLR_STRUCT(part_length);
	AEFX_CLR_STRUCT(num_thrd);
    AEFX_CLR_STRUCT(lastPart_length);
    miP.inW = *inputP;
    miP.outW = *outputP;
        

    ERR(suites.IterateSuite1()->AEGP_GetNumThreads(&num_thrd));
    part_length = A_long ((outputP->height/(float)num_thrd));
    lastPart_length =  part_length + (outputP->height -  (part_length*num_thrd));
   
    threaded_render* thRenderPtr = new threaded_render();
	if (PF_WORLD_IS_DEEP(outputP)) {
		AEFX_CLR_STRUCT(workers_thrds);
		for (A_long thrd_id = 0; thrd_id < num_thrd; ++thrd_id) {
			workers_thrds.emplace_back(std::thread(&threaded_render::render_16, thRenderPtr, (void*)&miP, (void*)&fiP, (void*)&flagsP, thrd_id,num_thrd, part_length,lastPart_length));
		}
	}
	else {
		AEFX_CLR_STRUCT(workers_thrds);
		for (A_long thrd_id = 0; thrd_id < num_thrd; ++thrd_id) {
			workers_thrds.emplace_back(std::thread(&threaded_render::render_8, thRenderPtr, (void*)&miP, (void*)&fiP, (void*)&flagsP, thrd_id,num_thrd, part_length,lastPart_length));
		}

	}

    for (auto&  t :  workers_thrds){
        t.join();
    }
    delete thRenderPtr;
	outputP = &miP.outW;
    PF_UNLOCK_HANDLE(arbH);
    ERR2( PF_CHECKIN_PARAM(in_data, &checkoutLayerOne));
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

static PF_Err
QueryDynamicFlags(
	PF_InData		*in_data,
	PF_OutData		*out_data,
	PF_ParamDef		*params[],
	void			*extra)
{
	AEGP_SuiteHandler	suites(in_data->pica_basicP);
	PF_Err 	err  = PF_Err_NONE,
            err2  = PF_Err_NONE;
    
    PF_ParamDef arb_param;
    AEFX_CLR_STRUCT(arb_param);
    ERR(PF_CHECKOUT_PARAM(    in_data,
                          MATH_ARB_DATA,
                          in_data->current_time,
                          in_data->time_step,
                          in_data->time_scale,
                          &arb_param));
    
	
	PF_Handle		arbH = arb_param.u.arb_d.value;
	m_ArbData		*arbP = NULL;
	arbP = reinterpret_cast<m_ArbData*>(suites.HandleSuite1()->host_lock_handle(arbH));
	if (arbP && !err) {
		if (arbP->PresetHasWideInputB) {
            out_data->out_flags  &=    PF_OutFlag_WIDE_TIME_INPUT;
            //PF_OutFlag2_AUTOMATIC_WIDE_- TIME_INPUT
        }
        if (arbP->NeedsPixelAroundB){
            out_data->out_flags &= PF_OutFlag_PIX_INDEPENDENT;
        }
		

	}
	PF_UNLOCK_HANDLE(arbH);
    ERR2(PF_CHECKIN_PARAM(in_data, &arb_param));

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
                err = HandleArbitrary(	in_data,
                                      out_data,
                                      params,
                                      output,
                                      reinterpret_cast<PF_ArbParamsExtra*>(extra));
                break;
            
            case PF_Cmd_COMPLETELY_GENERAL:
                err = RespondtoAEGP(in_data,
                                    out_data,
                                    params,
                                    output,
                                    extra);
                break;
			case PF_Cmd_QUERY_DYNAMIC_FLAGS:
				err = QueryDynamicFlags(in_data,
										out_data,
										params,
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

