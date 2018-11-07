 

#include "rtt.h"

void
copyWorld ( PF_InData		*in_data,
           PF_EffectWorld  *world_src,
           PF_EffectWorld  *world_dest,
           PF_LRect        *mask_src,
           PF_LRect        *mask_dest)
{
    AEGP_SuiteHandler suites(in_data->pica_basicP);
    PF_Err 				err = PF_Err_NONE;
    
    if (PF_Quality_HI == in_data->quality)
    {
        ERR(suites.WorldTransformSuite1()->copy_hq(in_data->effect_ref,world_src,world_dest,mask_src,mask_dest));
    }
    else
    {
        ERR(suites.WorldTransformSuite1()->copy(in_data->effect_ref,world_src,world_dest,mask_src,mask_dest));
    }
}


static PF_Err
About (
	PF_InData		*in_data,
	PF_OutData		*out_data,
	PF_ParamDef		*params[],
	PF_LayerDef		*output )
{
	
	PF_Err	err			=	PF_Err_NONE;
	AEGP_SuiteHandler		suites(in_data->pica_basicP);
	PF_AppPersonalTextInfo	personal_info;

	AEFX_CLR_STRUCT(personal_info);

	ERR(suites.AppSuite4()->PF_GetPersonalInfo(&personal_info));
		
	if (!err) {
		suites.ANSICallbacksSuite1()->sprintf(	out_data->return_msg,
										"%s\r%s\r%s, v%d.%d\r\r%s",
										STR(StrID_Name), 
										personal_info.name,
										personal_info.serial_str,
										MAJOR_VERSION, 
										MINOR_VERSION, 
										STR(StrID_Description));
	}
	return err;
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

	out_data->out_flags		=	PF_OutFlag_PIX_INDEPENDENT 			| 
								PF_OutFlag_SEND_UPDATE_PARAMS_UI	|
								PF_OutFlag_USE_OUTPUT_EXTENT		|
								PF_OutFlag_DEEP_COLOR_AWARE	;


	
	out_data->out_flags2	=	PF_OutFlag2_PARAM_GROUP_START_COLLAPSED_FLAG	|
                                PF_OutFlag2_FLOAT_COLOR_AWARE					|
                                PF_OutFlag2_SUPPORTS_SMART_RENDER				|
                                PF_OutFlag2_I_MIX_GUID_DEPENDENCIES             ;


	globH	=	suites.HandleSuite1()->host_new_handle(sizeof(my_global_data));
	
	if (globH) {
		globP = reinterpret_cast<my_global_dataP>(suites.HandleSuite1()->host_lock_handle(globH));
		if (globP) {
			globP->initializedB 	= TRUE;
            ERR(suites.UtilitySuite3()->AEGP_RegisterWithAEGP(NULL, STR(StrID_Name), &globP->my_id));

				if (!err){
					out_data->global_data 	= globH;
			}
		}
		suites.HandleSuite1()->host_unlock_handle(globH);
	} else	{
		err = PF_Err_INTERNAL_STRUCT_DAMAGED;
	}
	return err;
}

static PF_Err 
SequenceSetdown (	
					PF_InData		*in_data,
					PF_OutData		*out_data)
{
	PF_Err err = PF_Err_NONE;
	
	if (in_data->sequence_data){
		AEGP_SuiteHandler suites(in_data->pica_basicP);
		suites.HandleSuite1()->host_dispose_handle(in_data->sequence_data);
	}
	return err;
}

static PF_Err 
SequenceSetup (	
	PF_InData		*in_data,
	PF_OutData		*out_data)
{
	PF_Err err = PF_Err_NONE;
	AEGP_SuiteHandler suites(in_data->pica_basicP);
	
	err = SequenceSetdown(in_data, out_data);
	
	if (!err){
		PF_Handle	seq_dataH =	suites.HandleSuite1()->host_new_handle(sizeof(my_sequence_data));
		
		if (seq_dataH){
			my_sequence_dataP seqP = static_cast<my_sequence_dataP>(suites.HandleSuite1()->host_lock_handle(seq_dataH));
			if (seqP){

				out_data->sequence_data = seq_dataH;
				suites.HandleSuite1()->host_unlock_handle(seq_dataH);
			}
		} else {	// whoa, we couldn't allocate sequence data; bail!
			err = PF_Err_OUT_OF_MEMORY;
		}
	}
	return err;
}

static PF_Err	
ParamsSetup(
	PF_InData		*in_data,
	PF_OutData		*out_data,
	PF_ParamDef		*params[],
	PF_LayerDef		*output)
{
	PF_Err				err					= PF_Err_NONE;
	PF_ParamDef			def;
    

    
    
    AEFX_CLR_STRUCT(def);
    
    
    def.flags		|=	PF_ParamFlag_SUPERVISE |
    PF_ParamFlag_CANNOT_TIME_VARY;
    
    PF_ADD_LAYER (STR(StrID_Layer_source),
                  PF_LayerDefault_MYSELF,
                  RTT_LAYER_SOURCE_DISK_ID);
    
    
    AEFX_CLR_STRUCT(def);
    def.flags		=	PF_ParamFlag_SUPERVISE;

    PF_ADD_FLOAT_SLIDERX(STR(StrID_RTT_SPEED),
                         0,
                         1000,
                         0,
                         1000,
                         0,
                         PF_Precision_TENTHS,
                         0,
                        PF_ParamFlag_CANNOT_TIME_VARY,
                         RTT_SPEED_DISK_ID);
    AEFX_CLR_STRUCT(def);
    
    def.flags		=	PF_ParamFlag_SUPERVISE			|
                        PF_ParamFlag_CANNOT_TIME_VARY;

    
    PF_ADD_CHECKBOX(STR(StrID_RTT_CB_SHOW_SAFE),
                    "",
                    FALSE,
                    0,
                    RTT_CB_SHOWSAFEDISK_ID);
	AEFX_CLR_STRUCT(def);
    
	def.flags		=	PF_ParamFlag_SUPERVISE			|
						PF_ParamFlag_CANNOT_TIME_VARY	|
						PF_ParamFlag_CANNOT_INTERP;
    



	PF_ADD_POPUP(		STR(StrID_ModeName), 
						MODE_SIZE,
						MODE_HORIZONTAL,
						STR(StrID_ModeChoices),
						RTT_MODE_DISK_ID);
	AEFX_CLR_STRUCT(def);
    
	//FIRST TOPIC : HONRIZONTAL PARAMS
    
	def.flags		=	PF_ParamFlag_SUPERVISE |
						PF_ParamFlag_CANNOT_INTERP;
    PF_ADD_TOPIC (STR (StrID_RTT_TOPIC_GR1),
                  TOPIC_GR1_DISK_ID);
    AEFX_CLR_STRUCT(def);
    
    def.ui_flags	=	PF_PUI_STD_CONTROL_ONLY;
    PF_ADD_FLOAT_SLIDERX(STR(StrID_RTT_FIXEDY),
                         0,
                         1500,
                         0,
                         1500,
                         0,
                         0,
                         0,
                         PF_ParamFlag_CANNOT_TIME_VARY,
                         RTT_FIXEDY_DISK_ID);
    AEFX_CLR_STRUCT(def);
    
    def.flags		=	PF_ParamFlag_SUPERVISE			|
                        PF_ParamFlag_CANNOT_TIME_VARY	|
                        PF_ParamFlag_CANNOT_INTERP;

    
    PF_ADD_FLOAT_SLIDERX(STR(StrID_RTT_POSX_IN),
                         0,
                         1500,
                         0,
                         1500,
                         0,
                         0,
                         0,
                         PF_ParamFlag_CANNOT_TIME_VARY,
                         RTT_POSX_IN_DISK_ID);
    AEFX_CLR_STRUCT(def);
    
    
    
    def.flags		=	PF_ParamFlag_SUPERVISE			|
    PF_ParamFlag_CANNOT_TIME_VARY;

    
    PF_ADD_CHECKBOX(STR(StrID_RTT_CB_XIN_SAFE),
                    "",
                    FALSE,
                    0,
                    RTT_CB_XIN_SAFE_DISK_ID);
    AEFX_CLR_STRUCT(def);
    
    def.flags		=	PF_ParamFlag_SUPERVISE			|
                        PF_ParamFlag_CANNOT_TIME_VARY;
    

    PF_ADD_FLOAT_SLIDERX(STR(StrID_RTT_POSX_OUT),
                         0,
                         1500,
                         0,
                         1500,
                         0,
                         0,
                         0,
                         PF_ParamFlag_CANNOT_TIME_VARY,
                         RTT_POSX_OUT_DISK_ID);
    AEFX_CLR_STRUCT(def);
    
    def.flags		=	PF_ParamFlag_SUPERVISE			|
    PF_ParamFlag_CANNOT_TIME_VARY;
    def.ui_flags	=	PF_PUI_STD_CONTROL_ONLY;
    
    PF_ADD_CHECKBOX(STR(StrID_RTT_CB_XOUT_SAFE),
                    "",
                    FALSE,
                    0,
                    RTT_CB_XOUT_SAFE_DISK_ID);
    AEFX_CLR_STRUCT(def);
    
    PF_END_TOPIC(END_TOPIC_GR1_DISK_ID);
    AEFX_CLR_STRUCT(def);
    
    //SECOND TOPIC : VERTRICAL PARAMS
    
    def.flags		=	PF_ParamFlag_SUPERVISE |
    PF_ParamFlag_CANNOT_INTERP;
    PF_ADD_TOPIC (STR (StrID_RTT_TOPIC_GR2),
                  TOPIC_GR2_DISK_ID);
    
    AEFX_CLR_STRUCT(def);
    
    def.ui_flags	=	PF_PUI_STD_CONTROL_ONLY;
    PF_ADD_FLOAT_SLIDERX(STR(StrID_RTT_FIXEDX),
                         0,
                         1500,
                         0,
                         1500,
                         0,
                         0,
                         0,
                         PF_ParamFlag_CANNOT_TIME_VARY,
                         RTT_FIXEDX_DISK_ID);
    AEFX_CLR_STRUCT(def);
    
    def.flags		=	PF_ParamFlag_SUPERVISE			|
    PF_ParamFlag_CANNOT_TIME_VARY	|
    PF_ParamFlag_CANNOT_INTERP;

    
    PF_ADD_FLOAT_SLIDERX(STR(StrID_RTT_POSY_IN),
                         0,
                         5000,
                         0,
                         5000,
                         0,
                         0,
                         0,
                         PF_ParamFlag_CANNOT_TIME_VARY,
                         RTT_POSY_IN_DISK_ID);
    AEFX_CLR_STRUCT(def);
    
    
    
    def.flags		=	PF_ParamFlag_SUPERVISE			|
    PF_ParamFlag_CANNOT_TIME_VARY;

    
    PF_ADD_CHECKBOX("",
                    STR(StrID_RTT_CB_YIN_SAFE),
                    FALSE,
                    0,
                    RTT_CB_YIN_SAFE_DISK_ID);
    AEFX_CLR_STRUCT(def);
    
    def.flags		=	PF_ParamFlag_SUPERVISE			|
    PF_ParamFlag_CANNOT_TIME_VARY;

    PF_ADD_FLOAT_SLIDERX(STR(StrID_RTT_POSY_OUT),
                         0,
                         1500,
                         0,
                         1500,
                         0,
                         0,
                         0,
                         PF_ParamFlag_CANNOT_TIME_VARY,
                         RTT_POSY_OUT_DISK_ID);
    AEFX_CLR_STRUCT(def);
    
    def.flags		=	PF_ParamFlag_SUPERVISE			|
    PF_ParamFlag_CANNOT_TIME_VARY;

    
    PF_ADD_CHECKBOX("",
                    STR(StrID_RTT_CB_YOUT_SAFE),
                    FALSE,
                    0,
                    RTT_CB_YOUT_SAFE_DISK_ID);
    AEFX_CLR_STRUCT(def);
    
    PF_END_TOPIC(END_TOPIC_GR2_DISK_ID);
    AEFX_CLR_STRUCT(def);
    
    

    
    //THIRD TOPIC : TIME PARAMS
    def.flags		=	PF_ParamFlag_SUPERVISE |
    PF_ParamFlag_CANNOT_INTERP;
    PF_ADD_TOPIC (STR (StrID_RTT_TOPIC_GR3),
                  TOPIC_GR3_DISK_ID);
    
    AEFX_CLR_STRUCT(def);
    
    def.ui_flags	=	PF_PUI_STD_CONTROL_ONLY;
    PF_ADD_FLOAT_SLIDERX(STR(StrID_RTT_TIME_IN),
                         0,
                         1500,
                         0,
                         1500,
                         0,
                         0,
                         0,
                         PF_ParamFlag_CANNOT_TIME_VARY,
                         RTT_TIME_IN_DISK_ID);
    AEFX_CLR_STRUCT(def);
    
    def.flags		=	PF_ParamFlag_SUPERVISE			|
    PF_ParamFlag_CANNOT_TIME_VARY	|
    PF_ParamFlag_CANNOT_INTERP;

    
    PF_ADD_FLOAT_SLIDERX(STR(StrID_RTT_TIME_OUT),
                         0,
                         1500,
                         0,
                         1500,
                         0,
                         0,
                         0,
                         PF_ParamFlag_CANNOT_TIME_VARY,
                         RTT_TIME_OUT_DISK_ID);
    AEFX_CLR_STRUCT(def);
    PF_END_TOPIC(END_TOPIC_GR3_DISK_ID);
    AEFX_CLR_STRUCT(def);
    
    //FOURTH TOPIC : SETTINGS PARAMS
    def.flags		=	PF_ParamFlag_SUPERVISE |
    PF_ParamFlag_CANNOT_INTERP;
    PF_ADD_TOPIC (STR (StrID_RTT_TOPIC_GR4),
                  TOPIC_GR4_DISK_ID);
    
    AEFX_CLR_STRUCT(def);
    
    
    
    def.flags		=	PF_ParamFlag_SUPERVISE			|
    PF_ParamFlag_CANNOT_TIME_VARY;
    
    
    PF_ADD_CHECKBOX("",
                    STR(StrID_RTT_ENABLE_SETTINGS),
                    TRUE,
                    0,
                    RTT_ENABLE_SETTINGS_DISK_ID);
    AEFX_CLR_STRUCT(def);

    def.flags		=	PF_ParamFlag_SUPERVISE;
    
    PF_ADD_FLOAT_SLIDERX(STR(StrID_RTT_SCREEN_PRCT_SAFE_X),
                         0,
                         100,
                         0,
                         100,
                         80,
                         0,
                         0,
                         PF_ParamFlag_CANNOT_TIME_VARY,
                         RTT_SCREEN_PRCT_SAFE_X_DISK_ID);
    AEFX_CLR_STRUCT(def);
    
    def.flags		=	PF_ParamFlag_SUPERVISE;
    
    PF_ADD_FLOAT_SLIDERX(STR(StrID_RTT_SCREEN_PRCT_SAFE_Y),
                         0,
                         100,
                         0,
                         100,
                         90,
                         0,
                         0,
                         PF_ParamFlag_CANNOT_TIME_VARY,
                        RTT_SCREEN_PRCT_SAFE_Y_DISK_ID);
    
    AEFX_CLR_STRUCT(def);
    
    def.flags		=	PF_ParamFlag_SUPERVISE;
    PF_ADD_FLOAT_SLIDERX(STR(StrID_RTT_TXT_PRCT_SAFE_X),
                         0,
                         100,
                         0,
                         100,
                         33,
                         0,
                         0,
                         PF_ParamFlag_CANNOT_TIME_VARY,
                          RTT_TXT_PRCT_SAFE_X_DISK_ID);
    AEFX_CLR_STRUCT(def);
    
    def.flags		=	PF_ParamFlag_SUPERVISE;
    
    PF_ADD_FLOAT_SLIDERX(STR(StrID_RTT_TXT_PRCT_SAFE_Y),
                         0,
                         100,
                         0,
                         100,
                         33,
                         0,
                         0,
                         PF_ParamFlag_CANNOT_TIME_VARY,
                          RTT_TXT_PRCT_SAFE_Y_DISK_ID);
    AEFX_CLR_STRUCT(def);
    
    def.flags		=	PF_ParamFlag_SUPERVISE;
    
    PF_ADD_FLOAT_SLIDERX(STR(StrID_RTT_SAFE_PIXELS),
                         0,
                         50,
                         0,
                         10,
                         5,
                         PF_Precision_INTEGER,
                         0,
                         PF_ParamFlag_CANNOT_TIME_VARY,
                         RTT_SAFE_PIXELS_DISK_ID);
    AEFX_CLR_STRUCT(def);
    
    PF_END_TOPIC(END_TOPIC_GR4_DISK_ID);
    AEFX_CLR_STRUCT(def);


	out_data->num_params = RTT_NUM_PARAMS;

	return err;
}


static PF_Err  
MakeParamCopy(
	PF_ParamDef *actual[],	/* >> */ 
	PF_ParamDef copy[])		/* << */
{
	for (A_short iS = 0; iS < RTT_NUM_PARAMS; ++iS)	{
		AEFX_CLR_STRUCT(copy[iS]);	// clean params are important!
	}
	copy[RTT_INPUT]			= *actual[RTT_INPUT];
    copy[RTT_CBSHOWSAFE] =*actual[RTT_CBSHOWSAFE];
	copy[RTT_MODE]			= *actual[RTT_MODE];
    
    copy[RTT_TOPIC_GR1]			= *actual[RTT_TOPIC_GR1];
    copy[RTT_FIXEDY]			= *actual[RTT_FIXEDY];
    copy[RTT_POSX_IN]			= *actual[RTT_POSX_IN];
    copy[RTT_CB_XIN_SAFE]		= *actual[RTT_CB_XIN_SAFE];
    copy[RTT_POSX_OUT]			= *actual[RTT_POSX_OUT];
    copy[RTT_CB_XOUT_SAFE]		= *actual[RTT_CB_XOUT_SAFE];
    
    copy[RTT_TOPIC_GR2]			= *actual[RTT_TOPIC_GR2];
    copy[RTT_FIXEDX]			= *actual[RTT_FIXEDX];
    copy[RTT_POSY_IN]			= *actual[RTT_POSY_IN];
    copy[RTT_CB_YIN_SAFE]		= *actual[RTT_CB_YIN_SAFE];
    copy[RTT_POSY_OUT]			= *actual[RTT_POSY_OUT];
    copy[RTT_CB_YOUT_SAFE]		= *actual[RTT_CB_YOUT_SAFE];
    
    
	return PF_Err_NONE;

}

static PF_Err
UserChangedParam(
	PF_InData						*in_data,
	PF_OutData						*out_data,
	PF_ParamDef						*params[],
	PF_LayerDef						*outputP,
	const PF_UserChangedParamExtra	*which_hitP)
{
	PF_Err				err					= PF_Err_NONE;

	if (which_hitP->param_index == RTT_SPEED)
	{
		// If checkbox is checked, change slider value to 50 and flip checkbox back off
			params[RTT_SPEED]->u.sd.value = INT2FIX(50);
			params[RTT_SPEED]->uu.change_flags = PF_ChangeFlag_CHANGED_VALUE;
	}
	return err;
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
	AEGP_StreamRefH		topicGr1_streamH		= NULL,
						fixedy_streamH		 = NULL,
						posxin_streamH		 = NULL,
						cbxin_streamH        = NULL,
                        posxout_streamH      = NULL,
                        cbxout_streamH       = NULL,
    
                    topicGr2_streamH		= NULL,
                    fixedx_streamH		 = NULL,
                    posyin_streamH		 = NULL,
                    cbyin_streamH        = NULL,
                    posyout_streamH      = NULL,
                    cbyout_streamH       = NULL;


	PF_ParamType		param_type;
	PF_ParamDefUnion	param_union;
	
	A_Boolean			hide_themVertcB			= FALSE,
                        hide_themHorB           =FALSE;
						
	AEGP_EffectRefH			meH				= NULL;
	AEGP_SuiteHandler		suites(in_data->pica_basicP);


	PF_ParamDef		param_copy[RTT_NUM_PARAMS];
	ERR(MakeParamCopy(params, param_copy));


    if (params[RTT_MODE]->u.pd.value == MODE_VERTICAL) {
            hide_themVertcB			= FALSE,
            hide_themHorB           =TRUE;

		} else {
            hide_themVertcB			= TRUE,
            hide_themHorB           =FALSE;

		}


		// Changing visibility of params in AE is handled through stream suites
		ERR(suites.PFInterfaceSuite1()->AEGP_GetNewEffectForEffect(globP->my_id, in_data->effect_ref, &meH));

		ERR(suites.StreamSuite2()->AEGP_GetNewEffectStreamByIndex(globP->my_id, meH, RTT_TOPIC_GR1, 	&topicGr1_streamH));
		ERR(suites.StreamSuite2()->AEGP_GetNewEffectStreamByIndex(globP->my_id, meH, RTT_FIXEDY,	&fixedy_streamH));
		ERR(suites.StreamSuite2()->AEGP_GetNewEffectStreamByIndex(globP->my_id, meH, RTT_POSX_IN, &posxin_streamH));
        ERR(suites.StreamSuite2()->AEGP_GetNewEffectStreamByIndex(globP->my_id, meH, RTT_CB_XIN_SAFE, 	&cbxin_streamH));
        ERR(suites.StreamSuite2()->AEGP_GetNewEffectStreamByIndex(globP->my_id, meH, RTT_POSX_OUT,	&posxout_streamH));
        ERR(suites.StreamSuite2()->AEGP_GetNewEffectStreamByIndex(globP->my_id, meH, RTT_CB_XOUT_SAFE, &cbxout_streamH));
        
        ERR(suites.StreamSuite2()->AEGP_GetNewEffectStreamByIndex(globP->my_id, meH, RTT_TOPIC_GR2, 	&topicGr2_streamH));
        ERR(suites.StreamSuite2()->AEGP_GetNewEffectStreamByIndex(globP->my_id, meH, RTT_FIXEDX,	&fixedx_streamH));
        ERR(suites.StreamSuite2()->AEGP_GetNewEffectStreamByIndex(globP->my_id, meH, RTT_POSY_IN, &posyin_streamH));
        ERR(suites.StreamSuite2()->AEGP_GetNewEffectStreamByIndex(globP->my_id, meH, RTT_CB_YIN_SAFE, 	&cbyin_streamH));
        ERR(suites.StreamSuite2()->AEGP_GetNewEffectStreamByIndex(globP->my_id, meH, RTT_POSY_OUT,	&posyout_streamH));
        ERR(suites.StreamSuite2()->AEGP_GetNewEffectStreamByIndex(globP->my_id, meH, RTT_CB_YOUT_SAFE, &cbyout_streamH));
        
        

		// Toggle visibility of parameters
		ERR(suites.DynamicStreamSuite2()->AEGP_SetDynamicStreamFlag(topicGr1_streamH, 	AEGP_DynStreamFlag_HIDDEN, FALSE,     hide_themHorB));
		ERR(suites.DynamicStreamSuite2()->AEGP_SetDynamicStreamFlag(fixedy_streamH, 	AEGP_DynStreamFlag_HIDDEN, FALSE,     hide_themHorB));
		ERR(suites.DynamicStreamSuite2()->AEGP_SetDynamicStreamFlag(posxin_streamH, 	AEGP_DynStreamFlag_HIDDEN, FALSE,     hide_themHorB));
        ERR(suites.DynamicStreamSuite2()->AEGP_SetDynamicStreamFlag(cbxin_streamH, 	AEGP_DynStreamFlag_HIDDEN, FALSE,         hide_themHorB));
        ERR(suites.DynamicStreamSuite2()->AEGP_SetDynamicStreamFlag(posxout_streamH, 	AEGP_DynStreamFlag_HIDDEN, FALSE,     hide_themHorB));
        ERR(suites.DynamicStreamSuite2()->AEGP_SetDynamicStreamFlag(cbxout_streamH, 	AEGP_DynStreamFlag_HIDDEN, FALSE,     hide_themHorB));
        
        ERR(suites.DynamicStreamSuite2()->AEGP_SetDynamicStreamFlag(topicGr2_streamH, 	AEGP_DynStreamFlag_HIDDEN, FALSE,     hide_themVertcB));
        ERR(suites.DynamicStreamSuite2()->AEGP_SetDynamicStreamFlag(fixedx_streamH, 	AEGP_DynStreamFlag_HIDDEN, FALSE,     hide_themVertcB));
        ERR(suites.DynamicStreamSuite2()->AEGP_SetDynamicStreamFlag(posyin_streamH, 	AEGP_DynStreamFlag_HIDDEN, FALSE,     hide_themVertcB));
        ERR(suites.DynamicStreamSuite2()->AEGP_SetDynamicStreamFlag(cbyin_streamH, 	AEGP_DynStreamFlag_HIDDEN, FALSE,         hide_themVertcB));
        ERR(suites.DynamicStreamSuite2()->AEGP_SetDynamicStreamFlag(posyout_streamH, 	AEGP_DynStreamFlag_HIDDEN, FALSE,     hide_themVertcB));
        ERR(suites.DynamicStreamSuite2()->AEGP_SetDynamicStreamFlag(cbyout_streamH, 	AEGP_DynStreamFlag_HIDDEN, FALSE,     hide_themVertcB));


		// Change popup menu items
		ERR(suites.EffectSuite3()->AEGP_GetEffectParamUnionByIndex(globP->my_id, meH,  TOPIC_GR1_DISK_ID, &param_type, &param_union));

		if (meH){
			ERR2(suites.EffectSuite2()->AEGP_DisposeEffect(meH));
		}
		if (topicGr1_streamH){
			ERR2(suites.StreamSuite2()->AEGP_DisposeStream(topicGr1_streamH));
		}
		if (fixedy_streamH){
			ERR2(suites.StreamSuite2()->AEGP_DisposeStream(fixedy_streamH));
		}
		if (posxin_streamH){
			ERR2(suites.StreamSuite2()->AEGP_DisposeStream(posxin_streamH));
		}
		if (cbxin_streamH){
			ERR2(suites.StreamSuite2()->AEGP_DisposeStream(cbxin_streamH));
		}
        if (posxout_streamH){
            ERR2(suites.StreamSuite2()->AEGP_DisposeStream(posxout_streamH));
        }
        if (cbxout_streamH){
            ERR2(suites.StreamSuite2()->AEGP_DisposeStream(cbxout_streamH));
        }
        
        if (topicGr2_streamH){
            ERR2(suites.StreamSuite2()->AEGP_DisposeStream(topicGr2_streamH));
        }
        if (fixedx_streamH){
            ERR2(suites.StreamSuite2()->AEGP_DisposeStream(fixedx_streamH));
        }
        if (posyin_streamH){
            ERR2(suites.StreamSuite2()->AEGP_DisposeStream(posyin_streamH));
        }
        if (cbyin_streamH){
            ERR2(suites.StreamSuite2()->AEGP_DisposeStream(cbyin_streamH));
        }
        if (posyout_streamH){
            ERR2(suites.StreamSuite2()->AEGP_DisposeStream(posyout_streamH));
        }
        if (cbyout_streamH){
            ERR2(suites.StreamSuite2()->AEGP_DisposeStream(cbyout_streamH));
        }

        
        
		if (!err){
			out_data->out_flags |= PF_OutFlag_FORCE_RERENDER;
		}


	return err;
}



static PF_Err
ShiftXFunc8(
            void 		*refcon,
            A_long 		xL,
            A_long 		yL,
            PF_Pixel8 	*inP,
            PF_Pixel8 	*outP)
{
    prerender_rtt*	rttP = reinterpret_cast<prerender_rtt*>(refcon);
    PF_Err				err			= PF_Err_NONE;
    PF_Fixed			new_xFi		= 0,
                        new_yFi		= 0;

    if (rttP)
    {
        new_xFi = INT2FIX(xL) + (rttP->in_data.current_time*rttP->in_data.time_step)/rttP->fpsF;
        new_yFi = INT2FIX(yL);
        AEGP_SuiteHandler suites(rttP->in_data.pica_basicP);
        ERR(rttP->in_data.utils->subpixel_sample(rttP->in_data.effect_ref,
                                                new_xFi,
                                                new_yFi,
                                                &rttP->samp_pb,
                                                outP));
        
    }
     return err;
}


static PF_Err
SafetyFunc8 (
            void 		*refcon,
            A_long 		xL,
            A_long 		yL,
            PF_Pixel8 	*inP,
            PF_Pixel8 	*outP)
{
    prerender_rtt*	rttP = reinterpret_cast<prerender_rtt*>(refcon);
    PF_Err				err			= PF_Err_NONE;
    if (rttP)
    {
        outP->alpha =   inP->alpha;
        outP->red =     inP->red;
        outP->green =   inP->green;
        outP->blue =    inP->blue;
        // if out of safe --> add red
        if (    ((yL < rttP->safeHeightInA) ||
                 (yL > rttP->safeHeightOutA)||
                 (xL < rttP->safeWidthInA)||
                 (xL > rttP->safeWidthOutA)))
        {
            outP->alpha =  PF_MAX_CHAN8;
            outP->red = PF_MAX_CHAN8;
            outP->green =  0;
            outP->blue =  0;
        }

    }
    return err;
}

static PF_Err
SafetyFunc16 (
             void 		*refcon,
             A_long 		xL,
             A_long 		yL,
             PF_Pixel16 	*inP,
             PF_Pixel16 	*outP)
{
    prerender_rtt*	rttP = reinterpret_cast<prerender_rtt*>(refcon);
    PF_Err				err			= PF_Err_NONE;
    if (rttP)
    {
        
        
        outP->alpha =   inP->alpha;
        outP->red =     inP->red;
        outP->green =   inP->green;
        outP->blue =    inP->blue;
        
        // if out of safe --> add red
        if (    ((yL < rttP->safeHeightInA) ||
                 (yL > rttP->safeHeightOutA)||
                 (xL < rttP->safeWidthInA)||
                 (xL > rttP->safeWidthOutA)))
        {
            outP->alpha =  PF_MAX_CHAN16;
            outP->red = PF_MAX_CHAN16;
            outP->green =  0;
            outP->blue =  0;
        }
        
    }
    return err;
}
    
static PF_Err
SafetyFuncFloat (
              void 		*refcon,
              A_long 		xL,
              A_long 		yL,
              PF_PixelFloat 	*inP,
              PF_PixelFloat 	*outP)
{
    prerender_rtt*	rttP = reinterpret_cast<prerender_rtt*>(refcon);
    PF_Err				err			= PF_Err_NONE;
    if (rttP)
    {
        
        
        outP->alpha =   inP->alpha;
        outP->red =     inP->red;
        outP->green =   inP->green;
        outP->blue =    inP->blue;
        
        // if out of safe --> add red
        if (    ((yL < rttP->safeHeightInA) ||
                 (yL > rttP->safeHeightOutA)||
                 (xL < rttP->safeWidthInA)||
                 (xL > rttP->safeWidthOutA)))
        {
            outP->alpha =  1;
            outP->red =    1;
            outP->green =  0;
            outP->blue =  0;
        }
        
    }
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
PreRender(
          PF_InData				*in_data,
          PF_OutData				*out_data,
          PF_PreRenderExtra		*extraP)
{
    PF_Err	err				= PF_Err_NONE;
    
    PF_RenderRequest req	= extraP->input->output_request;
    
    PF_CheckoutResult		in_result;
    AEGP_SuiteHandler		suites(in_data->pica_basicP);
    
    PF_Handle	infoH		=	suites.HandleSuite1()->host_new_handle(sizeof(prerender_rtt));
    
    prerender_rtt		*rttP = NULL;
    if (infoH){
        rttP = reinterpret_cast<prerender_rtt*>(suites.HandleSuite1()->host_lock_handle(infoH));
        if (rttP){
            AEFX_CLR_STRUCT(*rttP);
            
            AEGP_LayerH		layerH;
            AEGP_CompH		compH;
            AEGP_ItemH      itemH;
            AEFX_SuiteScoper<AEGP_PFInterfaceSuite1> PFInterfaceSuite = AEFX_SuiteScoper<AEGP_PFInterfaceSuite1>(	in_data,
                                                                                                                 kAEGPPFInterfaceSuite,
                                                                                                                 kAEGPPFInterfaceSuiteVersion1,
                                                                                                                 out_data);
            AEFX_SuiteScoper<AEGP_LayerSuite8> layerSuite = AEFX_SuiteScoper<AEGP_LayerSuite8>(	in_data,
                                                                                               kAEGPLayerSuite,
                                                                                               kAEGPLayerSuiteVersion8,
                                                                                               out_data);
            AEFX_SuiteScoper<AEGP_CompSuite10> compSuite = AEFX_SuiteScoper<AEGP_CompSuite10>(  in_data,
                                                                                              kAEGPCompSuite,
                                                                                              kAEGPCompSuiteVersion10,
                                                                                              out_data);
            
            AEFX_SuiteScoper<AEGP_ItemSuite8> itemSuite = AEFX_SuiteScoper<AEGP_ItemSuite8>(  in_data,
                                                                                            kAEGPItemSuite,
                                                                                            kAEGPItemSuiteVersion8,
                                                                                            out_data);
            
            AEFX_SuiteScoper<AEGP_StreamSuite4> StreamSuite = AEFX_SuiteScoper<AEGP_StreamSuite4>(  in_data,
                                                                                                  kAEGPStreamSuite,
                                                                                                  kAEGPStreamSuiteVersion4,
                                                                                                  out_data);
            
            
            
            PFInterfaceSuite->AEGP_GetEffectLayer(in_data->effect_ref, &layerH);
            layerSuite->AEGP_GetLayerParentComp(layerH, &compH);
            compSuite->AEGP_GetItemFromComp (compH, &itemH);
            A_long width, height;
            A_Time currTime;
            AEGP_StreamVal2 strValP, strValSP, strValAP;
            AEGP_StreamType  strTypeP;
            AEFX_CLR_STRUCT(width);
            AEFX_CLR_STRUCT(height);
            itemSuite->AEGP_GetItemDimensions(itemH, &width, &height);
            rttP->compWidthA =  PF_FpLong (width);
            rttP->compHeightA = PF_FpLong (height);
            ERR(suites.CompSuite10()->AEGP_GetCompFramerate(compH, &rttP->fpsF));
            
            layerSuite->AEGP_GetLayerCurrentTime (layerH, AEGP_LTimeMode_LayerTime, &currTime);
            StreamSuite->AEGP_GetLayerStreamValue(layerH, AEGP_LayerStream_POSITION, AEGP_LTimeMode_LayerTime, &currTime, NULL, &strValP, &strTypeP);
            AEFX_CLR_STRUCT(rttP->positionTD);
            rttP->positionTD.x= strValP.three_d.x;
            rttP->positionTD.y = strValP.three_d.y;
            
            AEFX_CLR_STRUCT(rttP->scaleTD);
            StreamSuite->AEGP_GetLayerStreamValue(layerH, AEGP_LayerStream_SCALE, AEGP_LTimeMode_LayerTime, &currTime, NULL, &strValSP, &strTypeP);
            rttP->scaleTD.x = strValSP.three_d.x;
            rttP->scaleTD.y = strValSP.three_d.y;
            
            AEFX_CLR_STRUCT(rttP->acPointTD);
            StreamSuite->AEGP_GetLayerStreamValue(layerH, AEGP_LayerStream_ANCHORPOINT, AEGP_LTimeMode_LayerTime, &currTime, NULL, &strValAP, &strTypeP);
            rttP->acPointTD.x = strValAP.three_d.x;
            rttP->acPointTD.y = strValAP.three_d.y;

            
            //Comunicate with AEGP to force rerender if one of those params has changed
            if (extraP->cb->GuidMixInPtr) {
                extraP->cb->GuidMixInPtr(in_data->effect_ref, sizeof(width), reinterpret_cast<void *>(&width));
                extraP->cb->GuidMixInPtr(in_data->effect_ref, sizeof(height), reinterpret_cast<void *>(&height));
                extraP->cb->GuidMixInPtr(in_data->effect_ref, sizeof(strValP), reinterpret_cast<void *>(&strValP));
                extraP->cb->GuidMixInPtr(in_data->effect_ref, sizeof(strValSP), reinterpret_cast<void *>(&strValSP));
                extraP->cb->GuidMixInPtr(in_data->effect_ref, sizeof(strValAP), reinterpret_cast<void *>(&strValAP));
            }
            
            extraP->output->pre_render_data = infoH;
            AEFX_CLR_STRUCT(in_result);
            
            if (!err){
                req.preserve_rgb_of_zero_alpha = TRUE;
                req.field = PF_Field_FRAME;
                
                PF_LRect fullLayer = {0, 0, in_data->width, in_data->height};
                UnionLRect(&fullLayer, &req.rect);

                ERR(extraP->cb->checkout_layer(	in_data->effect_ref,
                                               RTT_INPUT,
                                               RTT_INPUT,
                                               &req,
                                               in_data->current_time,
                                               in_data->time_step,
                                               in_data->time_scale,
                                               &in_result));
                extraP->output->flags = PF_RenderOutputFlag_RETURNS_EXTRA_PIXELS;
              
                if (!err){
                    rttP->layerWidthA =in_data->width;
                    rttP->layerHeightA = in_data->height;
                    
                    //PREPARE FOR DOWNSAMPLING
                    rttP->scale_x = in_data->downsample_x.num*in_data->pixel_aspect_ratio.num/ (float)in_data->downsample_x.den,
                    rttP->scale_y = in_data->downsample_y.num*in_data->pixel_aspect_ratio.den/ (float)in_data->downsample_y.den;
                    rttP->compWidthA    *= rttP->scale_x;
                    rttP->compHeightA   *= rttP->scale_y;
                    rttP->layerWidthA   *= rttP->scale_x;
                    rttP->layerHeightA  *= rttP->scale_y;
                    rttP->acPointTD.x   *= rttP->scale_x;
                    rttP->acPointTD.y   *= rttP->scale_y;
                    rttP->positionTD.x  *= rttP->scale_x;
                    rttP->positionTD.y  *= rttP->scale_y;
                    rttP->SafePixelTD.x *= rttP->scale_x;
                    rttP->SafePixelTD.y *= rttP->scale_y;

                    
                    UnionLRect(&in_result.result_rect, 		&extraP->output->result_rect);
                    UnionLRect(&in_result.max_result_rect, 	&extraP->output->max_result_rect);
                }
            }
            suites.HandleSuite1()->host_unlock_handle(infoH);
        }
    }
    return err;
}

static PF_Err
SmartRender(
            PF_InData				*in_data,
            PF_OutData				*out_data,
            PF_SmartRenderExtra		*extraP)
{
    
    PF_Err			err			= PF_Err_NONE,
                    err2		= PF_Err_NONE;
    
    PF_EffectWorld	*inputP         = NULL,
                    *outputP        = NULL;
    
    
    

    
    
    PF_PixelFormat	format		=	PF_PixelFormat_INVALID;

    my_sequence_dataP	seqP	= reinterpret_cast<my_sequence_dataP>(DH(in_data->sequence_data));
    
    AEGP_SuiteHandler suites(in_data->pica_basicP);
    
    AEFX_SuiteScoper<PF_WorldSuite2> wsP = AEFX_SuiteScoper<PF_WorldSuite2>(in_data,
                                                                            kPFWorldSuite,
                                                                            kPFWorldSuiteVersion2,
                                                                            out_data);
    if (!err && seqP){
        
        prerender_rtt *rttP = reinterpret_cast<prerender_rtt*>(suites.HandleSuite1()->host_lock_handle(reinterpret_cast<PF_Handle>(extraP->input->pre_render_data)));
        if (rttP){
            

            PF_EffectWorld sourceLayerW;
            // checkout input & output buffers.
            ERR((extraP->cb->checkout_layer_pixels(	in_data->effect_ref, RTT_INPUT, &inputP)));
            ERR(extraP->cb->checkout_output(in_data->effect_ref, &outputP));
            // determine requested output depth
            ERR(wsP->PF_GetPixelFormat(outputP, &format));


            PF_ParamDef sourceLayer;
            AEFX_CLR_STRUCT(sourceLayer);
            ERR(PF_CHECKOUT_PARAM(in_data,
                                  RTT_LAYER_SOURCE,
                                  in_data->time_step,
                                  in_data->time_step,
                                  in_data->time_scale,
                                  &sourceLayer));

           
            const PF_PixelFloat fillFloat = {0,0,0,0};
            const PF_Pixel16 fillDeep = {0,0,0,0};
            const PF_Pixel fill8 = {0,0,0,0};
            
            if (sourceLayer.u.ld.data){
                 wsP->PF_NewWorld (in_data->effect_ref,
                                   sourceLayer.u.ld.width,
                                   sourceLayer.u.ld.height,
                                   TRUE,
                                   format,
                                   &sourceLayerW);
                 copyWorld(in_data, &sourceLayer.u.ld, &sourceLayerW, NULL, NULL);
                 ERR(PF_ABORT(in_data));
            }
            else {
                wsP->PF_NewWorld    (in_data->effect_ref,
                                           inputP->width,
                                           inputP->height,
                                           TRUE,
                                           format,
                                           &sourceLayerW);
                 ERR(PF_ABORT(in_data));

                switch (format){
                        
                         case PF_PixelFormat_ARGB128:
                        
                             ERR(suites.FillMatteSuite2()->fill_float (in_data->effect_ref, &fillFloat, NULL, &sourceLayerW));

                         case PF_PixelFormat_ARGB64:
                        
                             ERR(suites.FillMatteSuite2()->fill16 (in_data->effect_ref, &fillDeep, NULL, &sourceLayerW));

                        
                        case PF_PixelFormat_ARGB32:
                             ERR(suites.FillMatteSuite2()->fill (in_data->effect_ref, &fill8, NULL, &sourceLayerW));

                        
                        break;
                        
                    default:
                        err = PF_Err_INTERNAL_STRUCT_DAMAGED;
                        break;
                }
                
               
            }
            
            
            PF_ParamDef fos;
            AEFX_CLR_STRUCT(fos)
            ERR(PF_CHECKOUT_PARAM(	in_data,
                                  RTT_SPEED,
                                  in_data->current_time,
                                  in_data->time_step,
                                  in_data->time_scale,
                                  &fos));
            
            rttP->fos = A_long(fos.u.fs_d.value);
            
            
            PF_ParamDef fixedY;
            AEFX_CLR_STRUCT(fixedY)
            ERR(PF_CHECKOUT_PARAM(	in_data,
                                  RTT_FIXEDY,
                                  in_data->current_time,
                                  in_data->time_step,
                                  in_data->time_scale,
                                  &fixedY));
            
            rttP->horYPosA = A_long(fixedY.u.fs_d.value);
            

            PF_ParamDef posxIn;
            AEFX_CLR_STRUCT(posxIn)
            ERR(PF_CHECKOUT_PARAM(	in_data,
                                  RTT_POSX_IN,
                                  in_data->current_time,
                                  in_data->time_step,
                                  in_data->time_scale,
                                  &posxIn));
            
            rttP->horXposInA = A_long(posxIn.u.fs_d.value);
            
            
            PF_ParamDef posxOut;
            AEFX_CLR_STRUCT(posxOut)
            ERR(PF_CHECKOUT_PARAM(in_data,
                                  RTT_POSX_OUT,
                                  in_data->current_time,
                                  in_data->time_step,
                                  in_data->time_scale,
                                  &posxOut));
            
            rttP->horXposOutA = A_long(posxOut.u.fs_d.value);
            

            PF_ParamDef safeXInB;
            AEFX_CLR_STRUCT(safeXInB)
            ERR(PF_CHECKOUT_PARAM(in_data,
                                  RTT_CB_XIN_SAFE,
                                  in_data->current_time,
                                  in_data->time_step,
                                  in_data->time_scale,
                                  &safeXInB));
            
            rttP->horFixXposInB = A_long(safeXInB.u.bd.value);
            
          
            PF_ParamDef safeXOutB;
            AEFX_CLR_STRUCT(safeXOutB)
            ERR(PF_CHECKOUT_PARAM(in_data,
                                  RTT_CB_XOUT_SAFE,
                                  in_data->current_time,
                                  in_data->time_step,
                                  in_data->time_scale,
                                  &safeXOutB));
            
            rttP->horFixXposOutB = A_long(safeXOutB.u.bd.value);


            PF_ParamDef timeIn;
            AEFX_CLR_STRUCT(timeIn)
            ERR(PF_CHECKOUT_PARAM(in_data,
                                  RTT_TIME_IN,
                                  in_data->current_time,
                                  in_data->time_step,
                                  in_data->time_scale,
                                  &timeIn));
            
            rttP->timeIn = A_long(posxOut.u.fs_d.value);
            
            PF_ParamDef timeOut;
            AEFX_CLR_STRUCT(timeOut)
            ERR(PF_CHECKOUT_PARAM(in_data,
                                  RTT_TIME_OUT,
                                  in_data->current_time,
                                  in_data->time_step,
                                  in_data->time_scale,
                                  &timeOut));
             rttP->timeOut = A_long(posxOut.u.fs_d.value);
            


            //checkout params
            PF_ParamDef mode;
            AEFX_CLR_STRUCT(mode)
            ERR(PF_CHECKOUT_PARAM(	in_data,
                                  RTT_MODE,
                                  in_data->current_time,
                                  in_data->time_step,
                                  in_data->time_scale,
                                  &mode));
            if (mode.u.pd.value == MODE_HORIZONTAL)
            {
                rttP->modeB =1;
            }
            else
            {
                rttP->modeB =2;
            }
            

            
            //CHECKOUT SHOW SAFETY
            PF_ParamDef showsafe;
            AEFX_CLR_STRUCT(showsafe)
            ERR(PF_CHECKOUT_PARAM(	in_data,
                                  RTT_CBSHOWSAFE,
                                  in_data->current_time,
                                  in_data->time_step,
                                  in_data->time_scale,
                                  &showsafe));
            
            if (showsafe.u.bd.value == TRUE)
            {
                rttP->showSafeB = TRUE;
            }
            else
            {
                rttP->showSafeB = FALSE;
            }
            
            
            //SETTINGS VALUES
            
            PF_ParamDef titleLimitX;
            AEFX_CLR_STRUCT(titleLimitX);
            AEFX_CLR_STRUCT(rttP->xSafety)
            ERR(PF_CHECKOUT_PARAM(	in_data,
                                  RTT_SCREEN_PRCT_SAFE_X,
                                  in_data->current_time,
                                  in_data->time_step,
                                  in_data->time_scale,
                                  &titleLimitX));
            
            rttP->xSafety = titleLimitX.u.fs_d.value/100;
            
            
            
            
            PF_ParamDef titleLimitY;
            AEFX_CLR_STRUCT(titleLimitY);
            AEFX_CLR_STRUCT(rttP->ySafety)
            ERR(PF_CHECKOUT_PARAM(	in_data,
                                  RTT_SCREEN_PRCT_SAFE_Y,
                                  in_data->current_time,
                                  in_data->time_step,
                                  in_data->time_scale,
                                  &titleLimitY));
            
            rttP->ySafety = titleLimitY.u.fs_d.value/100;

            
            PF_ParamDef safePixel;
            AEFX_CLR_STRUCT(safePixel)
            ERR(PF_CHECKOUT_PARAM(	in_data,
                                  RTT_SAFE_PIXELS,
                                  in_data->current_time,
                                  in_data->time_step,
                                  in_data->time_scale,
                                  &safePixel));
            
            rttP->SafePixelTD.x = A_long (safePixel.u.fs_d.value);
            rttP->SafePixelTD.y = A_long (safePixel.u.fs_d.value);
            
            ERR2(PF_CHECKIN_PARAM(in_data, &sourceLayer));
            ERR2(PF_CHECKIN_PARAM(in_data, &safePixel));
            ERR2(PF_CHECKIN_PARAM(in_data, &showsafe));
            ERR2(PF_CHECKIN_PARAM(in_data, &fos));
            ERR2(PF_CHECKIN_PARAM(in_data, &mode));
            ERR2(PF_CHECKIN_PARAM(in_data, &fixedY));
            ERR2(PF_CHECKIN_PARAM(in_data, &posxIn));
            ERR2(PF_CHECKIN_PARAM(in_data, &posxOut));
            ERR2(PF_CHECKIN_PARAM(in_data, &safeXInB));
            ERR2(PF_CHECKIN_PARAM(in_data, &safeXOutB));
            ERR2(PF_CHECKIN_PARAM(in_data, &timeIn));
            ERR2(PF_CHECKIN_PARAM(in_data, &timeOut));
            ERR2(PF_CHECKIN_PARAM(in_data, &titleLimitX));
            ERR2(PF_CHECKIN_PARAM(in_data, &titleLimitY));
            
            rttP->in_data = *in_data;
            rttP->samp_pb.src = inputP;
            
            rttP->scaleTD.x = rttP->scaleTD.x/100;
            rttP->scaleTD.y = rttP->scaleTD.y/100;
            
            rttP->layerWidthA =
            rttP->loffTD.x =  (rttP->compWidthA - rttP->layerWidthA);
            rttP->loffTD.y =    (rttP->compHeightA- rttP->layerHeightA);
            rttP->compoffTD.x =(0.5*rttP->compWidthA)- rttP->positionTD.x - (0.5*rttP->compWidthA -rttP->acPointTD.x);
            rttP->compoffTD.y =(0.5*rttP->compHeightA)- rttP->positionTD.y- (0.5*rttP->compHeightA -rttP->acPointTD.y);


            //DELIMIT SAFE LIMITS
            A_long widthSecuLength = A_long (rttP->compWidthA *(1-rttP->xSafety)*0.5);
            A_long heightSecuLength = A_long (rttP->compHeightA *(1-rttP->ySafety)*0.5);
            
            rttP->safeWidthInA =  widthSecuLength+rttP->SafePixelTD.x;
            rttP->safeWidthOutA = rttP->compWidthA -rttP->safeWidthInA;
            rttP->safeWidthInA  += -(0.5*rttP->loffTD.x) + rttP->compoffTD.x -1;
            rttP->safeWidthOutA += (0.5*rttP->loffTD.x) + rttP->compoffTD.x -1;
            
            
            rttP->safeHeightInA = heightSecuLength+rttP->SafePixelTD.y;
            rttP->safeHeightOutA =rttP->compHeightA- rttP->safeHeightInA;
            rttP->safeHeightInA +=-(0.5*rttP->loffTD.y) + rttP->compoffTD.y -1;
            rttP->safeHeightOutA +=(0.5*rttP->loffTD.y) + rttP->compoffTD.y -1;


            // Transform SourceWorld and merge
            PF_EffectWorld horTransformSourceP;
            AEFX_CLR_STRUCT(horTransformSourceP);
            wsP->PF_NewWorld(in_data->effect_ref,
                             outputP->width,
                             outputP->height,
                             TRUE,
                             format,
                             &horTransformSourceP);

            ERR(PF_ABORT(in_data));

            //&sourceLayerW
            switch (format){
                    
                case PF_PixelFormat_ARGB128:
                    
                    ERR(suites.FillMatteSuite2()->fill_float (in_data->effect_ref, &fillFloat, NULL, &horTransformSourceP));
                    break;
                    
                case PF_PixelFormat_ARGB64:
                    
                    ERR(suites.FillMatteSuite2()->fill16 (in_data->effect_ref, &fillDeep, NULL, &horTransformSourceP));
                    break;
                    
                    
                case PF_PixelFormat_ARGB32:
                    ERR(suites.FillMatteSuite2()->fill (in_data->effect_ref, &fill8, NULL,&horTransformSourceP));
                    
                    
                    break;
                    
                default:
                    err = PF_Err_INTERNAL_STRUCT_DAMAGED;
                    break;
            }


            
            if (rttP->modeB ==1 && horTransformSourceP.data ){
             ERR(suites.WorldTransformSuite1()->blend (in_data->effect_ref,  inputP, &horTransformSourceP,  1 ,outputP));
            }
            
            // SafeWorld
            PF_EffectWorld outputSafeP, safeP;
            AEFX_CLR_STRUCT(outputSafeP);
            wsP->PF_NewWorld(in_data->effect_ref,
                             rttP->compWidthA,
                             rttP->compHeightA,
                             TRUE,
                             format,
                             &outputSafeP);
            ERR(PF_ABORT(in_data));
            safeP =outputSafeP;
            //&safeworld
            switch (format){
                    
                case PF_PixelFormat_ARGB128:
                    ERR(suites.FillMatteSuite2()->fill_float (in_data->effect_ref, &fillFloat, NULL,  &safeP));
                    break;
                    
                case PF_PixelFormat_ARGB64:
                    
                    ERR(suites.FillMatteSuite2()->fill16 (in_data->effect_ref, &fillDeep, NULL,  &safeP));
                    break;
                    
                    
                case PF_PixelFormat_ARGB32:
                    ERR(suites.FillMatteSuite2()->fill (in_data->effect_ref, &fill8, NULL, &safeP));
                    
                    
                    break;
                    
                default:
                    err = PF_Err_INTERNAL_STRUCT_DAMAGED;
                    break;
            }

            if (rttP->showSafeB)
            {
                switch (format) {
                        
                    case PF_PixelFormat_ARGB128:
                        ERR(suites.IterateFloatSuite1()->iterate(	in_data,
                                                                 0,
                                                                 (outputSafeP.extent_hint.bottom - outputSafeP.extent_hint.top),
                                                                 &safeP,
                                                                 &outputSafeP.extent_hint,
                                                                 (void*)rttP,
                                                                 SafetyFuncFloat,
                                                                 &outputSafeP));
                        break;
                        
                    case PF_PixelFormat_ARGB64:
                        
                        ERR(suites.Iterate16Suite1()->iterate(	in_data,
                                                              0,
                                                              (outputSafeP.extent_hint.bottom - outputSafeP.extent_hint.top),
                                                              &safeP,
                                                              &outputSafeP.extent_hint,
                                                              (void*)rttP,
                                                              SafetyFunc16,
                                                             &outputSafeP));
                        break;
                        
                    case PF_PixelFormat_ARGB32:
                        
                        ERR(suites.Iterate8Suite1()->iterate (	in_data,
                                                              0,
                                                              (outputSafeP.extent_hint.bottom - outputSafeP.extent_hint.top),
                                                              &safeP,
                                                              &outputSafeP.extent_hint,
                                                              (void*)rttP,
                                                              SafetyFunc8,
                                                              &outputSafeP));
                        
                        
                        break;
                        
                    default:
                        err = PF_Err_INTERNAL_STRUCT_DAMAGED;
                        break;
                }
                PF_Rect compR;
                compR.left=0;
                compR.top= 0;
                compR.right= rttP->compWidthA;
                compR.bottom = rttP->compHeightA;
                ERR(suites.WorldTransformSuite1()->composite_rect (in_data->effect_ref, &compR,100,&outputSafeP, 0,0, PF_Field_FRAME, PF_Xfer_COPY, outputP));
            }
            

            
            if (outputSafeP.data){
                 wsP->PF_DisposeWorld(in_data->effect_ref, &outputSafeP);
            }
            
            if (horTransformSourceP.data){
                wsP->PF_DisposeWorld(in_data->effect_ref, &horTransformSourceP);
            }
            if (sourceLayerW.data){
                 wsP->PF_DisposeWorld(in_data->effect_ref, &sourceLayerW);
            }
            
            ERR2(extraP->cb->checkin_layer_pixels(in_data->effect_ref, RTT_INPUT));
        }
        suites.HandleSuite1()->host_unlock_handle(reinterpret_cast<PF_Handle>(extraP->input->pre_render_data));
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

DllExport	PF_Err 
EntryPointFunc(	
	PF_Cmd			cmd,
	PF_InData		*in_data,
	PF_OutData		*out_data,
	PF_ParamDef		*params[],
	PF_LayerDef		*output,
	void			*extra )
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

			case PF_Cmd_SEQUENCE_SETUP:
				err = SequenceSetup(in_data,out_data);
				break;

			case PF_Cmd_SEQUENCE_SETDOWN:
				err = SequenceSetdown(in_data,out_data);
				break;

			case PF_Cmd_SEQUENCE_RESETUP:
				err = SequenceSetup(in_data,out_data);
				break;
	
            case PF_Cmd_SMART_RENDER:
                err = SmartRender(	in_data,
                                  out_data,
                                  reinterpret_cast<PF_SmartRenderExtra*>(extra));
                break;
                
            case PF_Cmd_SMART_PRE_RENDER:
                err = PreRender(	in_data,
                                out_data,
                                reinterpret_cast<PF_PreRenderExtra*>(extra));
                break;
                
            case PF_Cmd_COMPLETELY_GENERAL:
                err = RespondtoAEGP(in_data,
                                    out_data,
                                    params,
                                    output, 
                                    extra);
                break;

				
			case PF_Cmd_USER_CHANGED_PARAM:
				err = UserChangedParam(	in_data,
										out_data,
										params,
										output,
										reinterpret_cast<const PF_UserChangedParamExtra *>(extra));
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
		err = PF_Err_INTERNAL_STRUCT_DAMAGED;
	}
	return err;
}
