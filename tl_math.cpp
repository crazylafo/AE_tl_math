
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

	PF_ADD_FLOAT_SLIDERX(	STR(StrID_MRed_Param_Name),
							MATH_VAR_MIN,
							MATH_VAR_MAX,
							MATH_VAR_MIN,
							MATH_VAR_MAX,
							MATH_VAR_DFLT,
							PF_Precision_THOUSANDTHS,
							0,
							0,
							MATH_RED_VAR_DISK_ID);

	AEFX_CLR_STRUCT(def);
    
    PF_ADD_FLOAT_SLIDERX(	STR(StrID_MGreen_Param_Name),
                         MATH_VAR_MIN,
                         MATH_VAR_MAX,
                         MATH_VAR_MIN,
                         MATH_VAR_MAX,
                         MATH_VAR_DFLT,
                         PF_Precision_THOUSANDTHS,
                         0,
                         0,
                         MATH_GREEN_VAR_DISK_ID);
    
    AEFX_CLR_STRUCT(def);
    
    PF_ADD_FLOAT_SLIDERX(	STR(StrID_MBlue_Param_Name),
                         MATH_VAR_MIN,
                         MATH_VAR_MAX,
                         MATH_VAR_MIN,
                         MATH_VAR_MAX,
                         MATH_VAR_DFLT,
                         PF_Precision_THOUSANDTHS,
                         0,
                         0,
                         MATH_BLUE_VAR_DISK_ID);
    
    AEFX_CLR_STRUCT(def);
    
    PF_ADD_FLOAT_SLIDERX(	STR( StrID_MAlpha_Param_Name),
                         MATH_VAR_MIN,
                         MATH_VAR_MAX,
                         MATH_VAR_MIN,
                         MATH_VAR_MAX,
                         MATH_VAR_DFLT,
                         PF_Precision_THOUSANDTHS,
                         0,
                         0,
                         MATH_ALPHA_VAR_DISK_ID);
    
    AEFX_CLR_STRUCT(def);
    
    
    ERR(CreateDefaultArb(	in_data,
                         out_data,
                         &def.u.arb_d.dephault));
    
    PF_ADD_ARBITRARY2(	"data transfert",
                      10,
                      10,
                      0,
                      PF_PUI_CONTROL | PF_PUI_DONT_ERASE_CONTROL,
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
SequenceSetup (
               PF_InData		*in_data,
               PF_OutData		*out_data)
{
    PF_Err err = PF_Err_NONE;
    AEGP_SuiteHandler suites(in_data->pica_basicP);
    

    if (!err){
        PF_Handle	seq_dataH =	suites.HandleSuite1()->host_new_handle(sizeof(Unflat_Seq_Data));
        
        if (seq_dataH){
           Unflat_Seq_Data	*seqP = reinterpret_cast<Unflat_Seq_Data*>(suites.HandleSuite1()->host_lock_handle(seq_dataH));
            if (seqP){
				AEFX_CLR_STRUCT(*seqP);
				seqP->flatB = FALSE;
                seqP->redExAcP = STR(StrID_Default_expr);
                seqP->greenExAcP =STR(StrID_Default_expr);
                seqP->blueExAcP =STR(StrID_Default_expr);
                seqP->alphaExAcP =STR(StrID_Default_expr);
                seqP->initB = TRUE;
                

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
SequenceSetdown (
                 PF_InData		*in_data,
                 PF_OutData		*out_data)
{
    PF_Err err = PF_Err_NONE;
     AEGP_SuiteHandler suites(in_data->pica_basicP);
    
    if (in_data->sequence_data){
        Unflat_Seq_Data	*seqP = reinterpret_cast<Unflat_Seq_Data*>(DH(in_data->sequence_data));
        if (seqP->flatB){
            seqP->redExAcP.clear();
            seqP->greenExAcP.clear();
            seqP->blueExAcP.clear();
            seqP->alphaExAcP.clear();
        }
       
        suites.HandleSuite1()->host_dispose_handle(in_data->sequence_data);
    }
    return err;
}

static PF_Err
SequenceFlatten(
                PF_InData		*in_data,
                PF_OutData		*out_data)
{
    PF_Err err = PF_Err_NONE;
    AEGP_SuiteHandler suites(in_data->pica_basicP);
    
    // Make a flat copy of whatever is in the unflat seq data handed to us.
    
    if (in_data->sequence_data){
        Unflat_Seq_Data* unflat_seq_dataP = reinterpret_cast<Unflat_Seq_Data*>(DH(in_data->sequence_data));
        
        if (unflat_seq_dataP){
            PF_Handle flat_seq_dataH = suites.HandleSuite1()->host_new_handle(sizeof(Flat_Seq_Data));
            
            if (flat_seq_dataH){
                Flat_Seq_Data*	flat_seq_dataP = reinterpret_cast<Flat_Seq_Data*>(suites.HandleSuite1()->host_lock_handle(flat_seq_dataH));
                
                if (flat_seq_dataP){
                    AEFX_CLR_STRUCT(*flat_seq_dataP);
                    flat_seq_dataP->flatB		= TRUE;
                    
                    
                    #ifdef AE_OS_WIN
                    strncpy_s(flat_seq_dataP->alphaExAc ,unflat_seq_dataP->alphaExAcP.c_str(), PF_MAX_EFFECT_MSG_LEN);
                    strncpy_s(flat_seq_dataP->redExAc,   unflat_seq_dataP->redExAcP.c_str(), PF_MAX_EFFECT_MSG_LEN);
                    strncpy_s(flat_seq_dataP->greenExAc , unflat_seq_dataP->greenExAcP.c_str(), PF_MAX_EFFECT_MSG_LEN);
                    strncpy_s(flat_seq_dataP->blueExAc , unflat_seq_dataP->blueExAcP.c_str(), PF_MAX_EFFECT_MSG_LEN);
                    #else
                    strncpy(flat_seq_dataP->alphaExAc ,unflat_seq_dataP->alphaExAcP.c_str(), PF_MAX_EFFECT_MSG_LEN);
                    strncpy(flat_seq_dataP->redExAc,   unflat_seq_dataP->redExAcP.c_str(), PF_MAX_EFFECT_MSG_LEN);
                    strncpy(flat_seq_dataP->greenExAc , unflat_seq_dataP->greenExAcP.c_str(), PF_MAX_EFFECT_MSG_LEN);
                    strncpy(flat_seq_dataP->blueExAc , unflat_seq_dataP->blueExAcP.c_str(), PF_MAX_EFFECT_MSG_LEN);
                    #endif
                    
                    // In SequenceSetdown we toss out the unflat data
                    unflat_seq_dataP->redExAcP.clear();
                    unflat_seq_dataP->greenExAcP.clear();
                    unflat_seq_dataP->blueExAcP.clear();
                    unflat_seq_dataP->alphaExAcP.clear();
                    suites.HandleSuite1()->host_dispose_handle(in_data->sequence_data);
                    
                    out_data->sequence_data = flat_seq_dataH;
                    suites.HandleSuite1()->host_unlock_handle(flat_seq_dataH);
                }
            } else {
                err = PF_Err_INTERNAL_STRUCT_DAMAGED;
            }
        }
    } else {
        err = PF_Err_INTERNAL_STRUCT_DAMAGED;
    }
    return err;
}

static PF_Err
GetFlattenedSequenceData(
                         PF_InData		*in_data,
                         PF_OutData		*out_data)
{
    PF_Err err = PF_Err_NONE;
    AEGP_SuiteHandler suites(in_data->pica_basicP);
    
    // Make a flat copy of whatever is in the unflat seq data handed to us.
    
    if (in_data->sequence_data){
        Unflat_Seq_Data* unflat_seq_dataP = reinterpret_cast<Unflat_Seq_Data*>(DH(in_data->sequence_data));
        
        if (unflat_seq_dataP){
            PF_Handle flat_seq_dataH = suites.HandleSuite1()->host_new_handle(sizeof(Flat_Seq_Data));
            
            if (flat_seq_dataH){
                Flat_Seq_Data*	flat_seq_dataP = reinterpret_cast<Flat_Seq_Data*>(suites.HandleSuite1()->host_lock_handle(flat_seq_dataH));
                
                if (flat_seq_dataP){
                    AEFX_CLR_STRUCT(*flat_seq_dataP);
                    flat_seq_dataP->flatB		= TRUE;

                    
                    
                    #ifdef AE_OS_WIN

                    strncpy_s(flat_seq_dataP->alphaExAc, unflat_seq_dataP->redExAcP.c_str(), PF_MAX_EFFECT_MSG_LEN);
                    strncpy_s(flat_seq_dataP->redExAc,   unflat_seq_dataP->redExAcP.c_str(), PF_MAX_EFFECT_MSG_LEN);
                    strncpy_s(flat_seq_dataP->greenExAc , unflat_seq_dataP->greenExAcP.c_str(), PF_MAX_EFFECT_MSG_LEN);
                    strncpy_s(flat_seq_dataP->blueExAc , unflat_seq_dataP->blueExAcP.c_str(), PF_MAX_EFFECT_MSG_LEN);
                    #else
                    strncpy(flat_seq_dataP->alphaExAc ,unflat_seq_dataP->alphaExAcP.c_str(), PF_MAX_EFFECT_MSG_LEN);
                    strncpy(flat_seq_dataP->redExAc,   unflat_seq_dataP->redExAcP.c_str(), PF_MAX_EFFECT_MSG_LEN);
                    strncpy(flat_seq_dataP->greenExAc , unflat_seq_dataP->greenExAcP.c_str(), PF_MAX_EFFECT_MSG_LEN);
                    strncpy(flat_seq_dataP->blueExAc , unflat_seq_dataP->blueExAcP.c_str(), PF_MAX_EFFECT_MSG_LEN);
                    #endif
                    
                    // The whole point of this function is that we don't dispose of the unflat data!
                    // delete [] unflat_seq_dataP->stringP;
                    out_data->sequence_data = flat_seq_dataH;
                    suites.HandleSuite1()->host_unlock_handle(flat_seq_dataH);
                }
            } else {
                err = PF_Err_INTERNAL_STRUCT_DAMAGED;
            }
        }
    } else {
        err = PF_Err_INTERNAL_STRUCT_DAMAGED;
    }
    return err;
}

static PF_Err
SequenceResetup (
                 PF_InData		*in_data,
                 PF_OutData		*out_data)
{
    PF_Err err = PF_Err_NONE;
    AEGP_SuiteHandler suites(in_data->pica_basicP);
    
    // We got here because we're either opening a project w/saved (flat) sequence data,
    // or we've just been asked to flatten our sequence data (for a save) and now
    // we're blowing it back up.
    
    if (in_data->sequence_data){
        Flat_Seq_Data*	flatP = reinterpret_cast<Flat_Seq_Data*>(DH(in_data->sequence_data));
        
        if (flatP && flatP->flatB){
            PF_Handle unflat_seq_dataH = suites.HandleSuite1()->host_new_handle(sizeof(Unflat_Seq_Data));
            
            if (unflat_seq_dataH){
                Unflat_Seq_Data* unflatP = reinterpret_cast<Unflat_Seq_Data*>(suites.HandleSuite1()->host_lock_handle(unflat_seq_dataH));
                
                if (unflatP){
                    AEFX_CLR_STRUCT(*unflatP);

                    unflatP->flatB		= FALSE;
                    unflatP->redExAcP = flatP->redExAc;
                    unflatP->greenExAcP =flatP->greenExAc;
                    unflatP->blueExAcP = flatP->blueExAc;
                    unflatP->alphaExAcP = flatP->alphaExAc;
                    
                
                        // if it all went well, set the sequence data to our new, inflated seq data.
                        out_data->sequence_data = unflat_seq_dataH;
                    } else {
                        err = PF_Err_INTERNAL_STRUCT_DAMAGED;
                    }
                    suites.HandleSuite1()->host_unlock_handle(unflat_seq_dataH);
                }
            }
		    else {
				out_data->sequence_data = in_data->sequence_data;
		    }

    }
	else {
		err = SequenceSetup(in_data, out_data);
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
PopDialog (
           PF_InData		*in_data,
           PF_OutData		*out_data,
           PF_ParamDef		*params[],
           PF_LayerDef		*output )
{
    PF_Err err = PF_Err_NONE;
    AEGP_SuiteHandler	suites(in_data->pica_basicP);
    my_global_dataP		globP				= reinterpret_cast<my_global_dataP>(DH(out_data->global_data));
	Unflat_Seq_Data seqP = *reinterpret_cast<Unflat_Seq_Data*>(DH(out_data->sequence_data));

    AEGP_MemHandle     resultMemH          =     NULL;
    A_char *resultAC =     NULL;
    A_char          scriptAC[4096] = {'\0'};
    
    //strings to send expr to script
    std::string tempRedS ="'";
    std::string tempGreenS ="'";
    std::string tempBlueS ="'";
    std::string tempAlphaS ="'";
    
    AEGP_LayerH		layerH;
    AEGP_StreamRefH effect_streamH				= NULL;
    AEGP_EffectRefH   thisEffect_refH;
    AEGP_StreamValue2   valP;
    
    const A_Time currT = {0,100};
 

        
    AEFX_SuiteScoper<AEGP_PFInterfaceSuite1> PFInterfaceSuite = AEFX_SuiteScoper<AEGP_PFInterfaceSuite1>(	in_data,
                                                                                                             kAEGPPFInterfaceSuite,
                                                                                                             kAEGPPFInterfaceSuiteVersion1,
                                                                                                             out_data);
    AEFX_SuiteScoper<AEGP_LayerSuite8> layerSuite = AEFX_SuiteScoper<AEGP_LayerSuite8>(	in_data,
                                                                                           kAEGPLayerSuite,
                                                                                           kAEGPLayerSuiteVersion8,
                                                                                           out_data);
    
    AEFX_SuiteScoper<AEGP_StreamSuite4> StreamSuite = AEFX_SuiteScoper<AEGP_StreamSuite4>(	in_data,
                                                                                       kAEGPStreamSuite,
                                                                                       kAEGPStreamSuiteVersion4,
                                                                                       out_data);
    
    
    PFInterfaceSuite->AEGP_GetEffectLayer(in_data->effect_ref, &layerH);
    PFInterfaceSuite->AEGP_GetNewEffectForEffect (globP->my_id, in_data->effect_ref,  &thisEffect_refH);
    StreamSuite->AEGP_GetNewEffectStreamByIndex (globP->my_id, thisEffect_refH,  MATH_ARB_DATA,&effect_streamH);
    StreamSuite->AEGP_GetNewStreamValue(globP->my_id, effect_streamH, AEGP_LTimeMode_LayerTime, &currT,FALSE, &valP );
    
     PF_Handle		arbH = reinterpret_cast <PF_Handle>(valP.val.arbH);
    m_ArbData		*arbP			= NULL;
    
   
    
    
    
    if (!err){
        arbP = reinterpret_cast<m_ArbData*>(suites.HandleSuite1()->host_lock_handle(arbH));
        if (arbP){
            m_ArbData *tempPointer = reinterpret_cast<m_ArbData*>(arbP);
             tempRedS.append(tempPointer->redExAcP.c_str());
            tempGreenS.append(tempPointer->greenExAcP.c_str());
            tempBlueS.append(tempPointer->blueExAcP.c_str());
            tempAlphaS.append(tempPointer->alphaExAcP.c_str());
        }
    }
    PF_UNLOCK_HANDLE(arbH);
    
    
    if (effect_streamH){
        StreamSuite->AEGP_DisposeStream(effect_streamH);
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
        var arrayExpr =w.grp.redC.redet.text;\
        w.close();\
        result = arrayExpr;\
    }\
    w.grp.btnGrp.Cancel.onClick = function(){\
        var ret = parseInt(0);\
        w.close();\
        result = ret;\
    }\
    w.onResizing = w.onResize = function(){this.layout.resize();}\
    w.show();\
    return result\
    }\
    expr(%s,%s,%s,%s);";

    sprintf( scriptAC, SET_EXPR_SCRIPT,tempRedS.c_str() , tempGreenS.c_str() , tempBlueS.c_str() , tempAlphaS.c_str() );
    seqP.initB = TRUE;
    ERR(suites.UtilitySuite6()->AEGP_ExecuteScript(globP->my_id, scriptAC, FALSE, &resultMemH, NULL));

    //AEGP SETSTREAMVALUR TO ARB
    AEFX_CLR_STRUCT(resultAC);
    ERR(suites.MemorySuite1()->AEGP_LockMemHandle(resultMemH, reinterpret_cast<void**>(&resultAC)));


    
    if  (atoi (resultAC) !=0){
        seqP.redExAcP = resultAC;

    }
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

    
	miP.RedIF	= params[MATH_RED_VAR]->u.fs_d.value;
    miP.GreenIF	= params[MATH_RED_VAR]->u.fs_d.value;
    miP.BlueIF	= params[MATH_RED_VAR]->u.fs_d.value;
    miP.AlphaIF	= params[MATH_RED_VAR]->u.fs_d.value;
   
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

    // TO ADD TO GET THE COMP FPS ERR(suites.CompSuite10()->AEGP_GetCompFramerate(compPH, &fpsPF));

    std::string expression_string_Safe = "1";
    std::string expression_string_red= "1";
    std::string expression_string_green= "1";
    std::string expression_string_blue= "1";
    std::string expression_string_alpha= "1";
    

    if (!err){
        arbP = reinterpret_cast<m_ArbData*>(suites.HandleSuite1()->host_lock_handle(arbH));
        if (arbP){
            m_ArbData *tempPointer = reinterpret_cast<m_ArbData*>(arbP);
            expression_string_red = (tempPointer->redExAcP);
            expression_string_green = tempPointer->greenExAcP;
            expression_string_blue = tempPointer->blueExAcP;
            expression_string_alpha  = tempPointer->alphaExAcP;
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
                
                ERR(Arb_Copy(	in_data,
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
				
            case PF_Cmd_SEQUENCE_SETUP:
                err = SequenceSetup(in_data,out_data);
                break;
            case PF_Cmd_SEQUENCE_SETDOWN:
                err = SequenceSetdown(in_data,out_data);
                break;
            case PF_Cmd_GET_FLATTENED_SEQUENCE_DATA:
                err = GetFlattenedSequenceData(in_data,out_data);
                break;
            case PF_Cmd_SEQUENCE_FLATTEN:
                err = SequenceFlatten(in_data,out_data);
                break;
            case PF_Cmd_SEQUENCE_RESETUP:
                err = SequenceResetup(in_data,out_data);
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
                
                
            case PF_Cmd_DO_DIALOG:
                err = PopDialog(in_data,out_data,params,output);
                break;

				
			case PF_Cmd_RENDER:

				err = Render(	in_data,
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

