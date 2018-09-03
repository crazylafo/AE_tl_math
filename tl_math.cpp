
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

	out_data->out_flags =  PF_OutFlag_DEEP_COLOR_AWARE|	// just 16bpc, not 32bpc
                           PF_OutFlag_I_DO_DIALOG|
                           PF_OutFlag_PIX_INDEPENDENT|
                           PF_OutFlag_NON_PARAM_VARY|
                           PF_OutFlag_SEQUENCE_DATA_NEEDS_FLATTENING;
    
    out_data->out_flags2 =  PF_OutFlag2_SUPPORTS_GET_FLATTENED_SEQUENCE_DATA;
    
    
    
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
							MATH_OPACITY_MIN,
							MATH_OPACITY_MAX,
							MATH_OPACITY_MIN,
							MATH_OPACITY_MAX,
							MATH_OPACITY_DFLT,
							PF_Precision_HUNDREDTHS,
							0,
							0,
							MATH_RED_OPACITY_DISK_ID);

	AEFX_CLR_STRUCT(def);
    
    PF_ADD_FLOAT_SLIDERX(	STR(StrID_MGreen_Param_Name),
                         MATH_OPACITY_MIN,
                         MATH_OPACITY_MAX,
                         MATH_OPACITY_MIN,
                         MATH_OPACITY_MAX,
                         MATH_OPACITY_DFLT,
                         PF_Precision_HUNDREDTHS,
                         0,
                         0,
                         MATH_GREEN_OPACITY_DISK_ID);
    
    AEFX_CLR_STRUCT(def);
    
    PF_ADD_FLOAT_SLIDERX(	STR(StrID_MBlue_Param_Name),
                         MATH_OPACITY_MIN,
                         MATH_OPACITY_MAX,
                         MATH_OPACITY_MIN,
                         MATH_OPACITY_MAX,
                         MATH_OPACITY_DFLT,
                         PF_Precision_HUNDREDTHS,
                         0,
                         0,
                         MATH_BLUE_OPACITY_DISK_ID);
    
    AEFX_CLR_STRUCT(def);
    
    PF_ADD_FLOAT_SLIDERX(	STR( StrID_MAlpha_Param_Name),
                         MATH_OPACITY_MIN,
                         MATH_OPACITY_MAX,
                         MATH_OPACITY_MIN,
                         MATH_OPACITY_MAX,
                         MATH_OPACITY_DFLT,
                         PF_Precision_HUNDREDTHS,
                         0,
                         0,
                         MATH_ALPHA_OPACITY_DISK_ID);
    
    AEFX_CLR_STRUCT(def);
    
    PF_ADD_BUTTON( STR(StrID_MExecute_Param_Name),
                   STR(StrID_MExecute_Param_Name),
                  NULL,
                  NULL,
                  MATH_EXECUTE_DISK_ID);

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
                seqP->alphaExAc = new A_char[strlen(STR(StrID_Default_expr)) + 1];
                seqP->redExAc = new A_char[strlen(STR(StrID_Default_expr)) + 1];
                seqP->greenExAc = new A_char[strlen(STR(StrID_Default_expr)) + 1];
                seqP->blueExAc = new A_char[strlen(STR(StrID_Default_expr)) + 1];
                
                
                if (seqP->alphaExAc){
                    suites.ANSICallbacksSuite1()->strcpy(seqP->alphaExAc, STR(StrID_Default_expr));
                }
                if (seqP->redExAc ){
                    suites.ANSICallbacksSuite1()->strcpy(seqP->redExAc, STR(StrID_Default_expr));
                }
                if (seqP->greenExAc){
                    suites.ANSICallbacksSuite1()->strcpy(seqP->greenExAc, STR(StrID_Default_expr));
                }
                if (seqP->blueExAc){
                    suites.ANSICallbacksSuite1()->strcpy(seqP->blueExAc, STR(StrID_Default_expr));
                }
               
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
            delete [] seqP->redExAc;
            delete [] seqP->greenExAc;
            delete [] seqP->blueExAc;
            delete [] seqP->alphaExAc;
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
                    strncpy_s(flat_seq_dataP->alphaExAc ,unflat_seq_dataP->alphaExAc, PF_MAX_EFFECT_MSG_LEN);
                    strncpy_s(flat_seq_dataP->redExAc,   unflat_seq_dataP->redExAc, PF_MAX_EFFECT_MSG_LEN);
                    strncpy_s(flat_seq_dataP->greenExAc , unflat_seq_dataP->greenExAc, PF_MAX_EFFECT_MSG_LEN);
                    strncpy_s(flat_seq_dataP->blueExAc , unflat_seq_dataP->blueExAc, PF_MAX_EFFECT_MSG_LEN);
                    #else
                    strncpy(flat_seq_dataP->alphaExAc ,unflat_seq_dataP->alphaExAc, PF_MAX_EFFECT_MSG_LEN);
                    strncpy(flat_seq_dataP->redExAc,   unflat_seq_dataP->redExAc, PF_MAX_EFFECT_MSG_LEN);
                    strncpy(flat_seq_dataP->greenExAc , unflat_seq_dataP->greenExAc, PF_MAX_EFFECT_MSG_LEN);
                    strncpy(flat_seq_dataP->blueExAc , unflat_seq_dataP->blueExAc, PF_MAX_EFFECT_MSG_LEN);
                    #endif
                    
                    // In SequenceSetdown we toss out the unflat data
                    delete [] unflat_seq_dataP->redExAc;
                    delete []unflat_seq_dataP->greenExAc;
                    delete []unflat_seq_dataP->blueExAc;
                    delete [] unflat_seq_dataP->alphaExAc;
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
                    strncpy_s(flat_seq_dataP->alphaExAc ,unflat_seq_dataP->alphaExAc, PF_MAX_EFFECT_MSG_LEN);
                    strncpy_s(flat_seq_dataP->redExAc,   unflat_seq_dataP->redExAc, PF_MAX_EFFECT_MSG_LEN);
                    strncpy_s(flat_seq_dataP->greenExAc , unflat_seq_dataP->greenExAc, PF_MAX_EFFECT_MSG_LEN);
                    strncpy_s(flat_seq_dataP->blueExAc , unflat_seq_dataP->blueExAc, PF_MAX_EFFECT_MSG_LEN);
                    #else
                    strncpy(flat_seq_dataP->alphaExAc ,unflat_seq_dataP->alphaExAc, PF_MAX_EFFECT_MSG_LEN);
                    strncpy(flat_seq_dataP->redExAc,   unflat_seq_dataP->redExAc, PF_MAX_EFFECT_MSG_LEN);
                    strncpy(flat_seq_dataP->greenExAc , unflat_seq_dataP->greenExAc, PF_MAX_EFFECT_MSG_LEN);
                    strncpy(flat_seq_dataP->blueExAc , unflat_seq_dataP->blueExAc, PF_MAX_EFFECT_MSG_LEN);
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
                    A_short	redExprlengthS		= strlen(flatP->redExAc);
                    A_short	greenExprlengthS	= strlen(flatP->greenExAc);
                    A_short	blueExprlengthS		= strlen(flatP->blueExAc);
                    A_short	alphaExprlengthS	= strlen(flatP->alphaExAc);
                    
                    unflatP->redExAc	= new A_char[redExprlengthS+ 1];
                    unflatP->greenExAc	= new A_char[greenExprlengthS+ 1];
                    unflatP->blueExAc	= new A_char[blueExprlengthS+ 1];
                    unflatP->alphaExAc	= new A_char[alphaExprlengthS+ 1];
                    
					if (unflatP->redExAc) {
						suites.ANSICallbacksSuite1()->strcpy(unflatP->redExAc, flatP->redExAc);
					}
					if (unflatP->greenExAc) {
						suites.ANSICallbacksSuite1()->strcpy(unflatP->greenExAc, flatP->greenExAc);
					}
					if (unflatP->blueExAc) {
						suites.ANSICallbacksSuite1()->strcpy(unflatP->blueExAc, flatP->blueExAc);
					}
					if (unflatP->alphaExAc){
						suites.ANSICallbacksSuite1()->strcpy(unflatP->alphaExAc, flatP->alphaExAc);
					}
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
    Flat_Seq_Data	seqP				= *reinterpret_cast<Flat_Seq_Data*>(DH(in_data->sequence_data));


    AEGP_MemHandle     resultMemH          =     NULL;
    A_char *resultAC =     NULL;
    A_char          scriptAC[4096] = {'\0'};

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
    expr(%d,%d,%d,%d);";
    

    sprintf( scriptAC, SET_EXPR_SCRIPT, seqP.redExAc,seqP.greenExAc, seqP.blueExAc, seqP.alphaExAc);
    

    
    ERR(suites.UtilitySuite6()->AEGP_ExecuteScript(globP->my_id, scriptAC, TRUE, &resultMemH, NULL));
    AEFX_CLR_STRUCT(resultAC);
    ERR(suites.MemorySuite1()->AEGP_LockMemHandle(resultMemH, reinterpret_cast<void**>(&resultAC)));
    
    if  (atoi (resultAC) !=0){
        suites.ANSICallbacksSuite1()->strcpy(seqP.redExAc, resultAC);
    }
    ERR(suites.MemorySuite1()->AEGP_FreeMemHandle(resultMemH));
    out_data->out_flags |= PF_OutFlag_DISPLAY_ERROR_MESSAGE |
                            PF_OutFlag_FORCE_RERENDER;
    return err;
}

static PF_Err
MathFunc8 (
           void		*refcon,
           A_long		xL,
           A_long		yL,
           PF_Pixel8	*inP,
           PF_Pixel8	*outP)
{
    PF_Err		err = PF_Err_NONE;
    
    MathInfo	*miP	= reinterpret_cast<MathInfo*>(refcon);
    PF_FpLong  red_result;
    

    miP->xLF *= xL;
    miP->yLF *= yL;
    

    AEFX_CLR_STRUCT(red_result);
    red_result = MIN (miP-> expression_t_red.value(), 1);
    
    if (miP){

        outP->alpha		=	inP->alpha;
        outP->red		=	A_u_char (red_result *PF_MAX_CHAN8);
        outP->green		=	inP->green;
        outP->blue      =   inP->blue;

    }
    
    return err;
}


static PF_Err 
Render (
	PF_InData		*in_data,
	PF_OutData		*out_data,
	PF_ParamDef		*params[],
	PF_LayerDef		*output )
{
	PF_Err				err		= PF_Err_NONE;
	AEGP_SuiteHandler	suites(in_data->pica_basicP);
    Flat_Seq_Data	seqP				= *reinterpret_cast<Flat_Seq_Data*>(DH(out_data->sequence_data));

	/*	Put interesting code here. */
	MathInfo			miP;
	AEFX_CLR_STRUCT(miP);
	A_long				linesL	= 0;
	linesL 		= output->extent_hint.bottom - output->extent_hint.top;
    
	miP.RedIF	= params[MATH_RED_OPACITY]->u.fs_d.value;
    miP.GreenIF	= params[MATH_RED_OPACITY]->u.fs_d.value;
    miP.BlueIF	= params[MATH_RED_OPACITY]->u.fs_d.value;
    miP.AlphaIF	= params[MATH_RED_OPACITY]->u.fs_d.value;
   
    //layer size
    miP.scale_x = in_data->downsample_x.num*in_data->pixel_aspect_ratio.num/ (float)in_data->downsample_x.den,
    miP.scale_y = in_data->downsample_y.num*in_data->pixel_aspect_ratio.den/ (float)in_data->downsample_y.den;
    miP.layerWidthF = PF_FpLong (in_data->width*miP.scale_x) ;
    miP.layerHeightF =PF_FpLong (in_data->height* miP.scale_y);
    
    //time params
    miP.layerTime_sec = PF_FpLong(in_data->current_time/in_data->time_scale);
    miP.layerTime_frame = PF_FpLong(in_data->current_time/in_data->time_step);
    miP.layerDuration =PF_FpLong( in_data->total_time / in_data->time_scale);
    
    

    miP.xLF = 1;
    miP.yLF = 1;

    std::string expression_string_Safe = "1";
    std::string expression_string(seqP.redExAc);  //= "if (xLF<layerWidth/2){layerTime_frame/(layerDuration*25)}";
    

    
    symbol_table_t symbol_table;
  
    symbol_table.add_variable("xLF",  miP.xLF);
    symbol_table.add_variable("yLF", miP.yLF);
    symbol_table.add_constants();
    symbol_table.add_constant("layerWidth",miP.layerWidthF);
    symbol_table.add_constant("layerHeight",miP.layerHeightF);
    symbol_table.add_constant("layerTime_sec",miP.layerTime_sec);
    symbol_table.add_constant("layerTime_frame",miP.layerTime_frame);
    symbol_table.add_constant("layerDuration",miP.layerDuration);
    miP.expression_t_red.register_symbol_table(symbol_table);
    parser_t parser;
    parser.compile(expression_string, miP.expression_t_red);
    //if error in expression
    
    if (!parser.compile(expression_string, miP.expression_t_red))
    {
        parser.compile(expression_string_Safe, miP.expression_t_red);
    }
   

	
	if (PF_WORLD_IS_DEEP(output)){
		ERR(suites.Iterate16Suite1()->iterate(	in_data,
												0,								// progress base
												linesL,							// progress final
												&params[MATH_INPUT]->u.ld,	// src
												NULL,							// area - null for all pixels
												(void*)&miP,					// refcon - your custom data pointer
												MySimpleGainFunc16,				// pixel function pointer
												output));
	} else {
		ERR(suites.Iterate8Suite1()->iterate(	in_data,
												0,								// progress base
												linesL,							// progress final
												&params[MATH_INPUT]->u.ld,	// src
												NULL,							// area - null for all pixels
												(void*)&miP,					// refcon - your custom data pointer
												MathFunc8,				// pixel function pointer
												output));	
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

                
            case PF_Cmd_PARAMS_SETUP:
                
                err = ParamsSetup(	in_data,
                                  out_data,
                                  params,
                                  output);
                break;
                

				
			case PF_Cmd_RENDER:

				err = Render(	in_data,
								out_data,
								params,
								output);
				break;
            case PF_Cmd_DO_DIALOG:
                err = PopDialog(in_data,out_data,params,output);
                break;

		}
	}
	catch(PF_Err &thrown_err){
		err = thrown_err;
	}
	return err;
}

