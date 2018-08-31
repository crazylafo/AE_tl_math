
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
                           PF_OutFlag_I_DO_DIALOG;
    
    
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
                seqP->exprMemoryAc.alphaExAc =0;
                seqP->exprMemoryAc.redExAc =0;
                seqP->exprMemoryAc.greenExAc = 0;
                seqP->exprMemoryAc.blueExAc =0;
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

template <typename T>
static double trig_function(
                            std::string expression_string 
    )
{
    typedef exprtk::symbol_table<T> symbol_table_t;
    typedef exprtk::expression<T>     expression_t;
    typedef exprtk::parser<T>             parser_t;
    
    
    
    T x;
    
    symbol_table_t symbol_table;
    symbol_table.add_variable("x",x);
    symbol_table.add_constants();
    
    expression_t expression;
    expression.register_symbol_table(symbol_table);
    
    parser_t parser;
    parser.compile(expression_string,expression);
    
    x =10;
    T y = expression.value();
    return y;

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

	if (miP){
        
        try {
            outP->alpha		=	inP->alpha;
            outP->red		=	MIN( A_u_char (miP->parseRed), PF_MAX_CHAN8);
            outP->green		=	inP->green;
            outP->blue	= inP->blue;
        }catch(int e){
            outP->alpha		=	0;
            outP->red		=	0;
            outP->green		=	0;
            outP->blue	=       0;
            
        }
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
    my_sequence_dataP	seqP				= reinterpret_cast<my_sequence_dataP>(DH(out_data->sequence_data));


    AEGP_MemHandle     resultMemH          =     NULL;
    A_char *resultAC =     NULL;
    A_char          scriptAC[4096] = {'\0'};

    A_char   SET_EXPR_SCRIPT [4096] = "function expr(redExpr,GreenExpr,BlueExpr,AlphaExpr) {\
    var w = new Window('dialog', 'Maths Expressions', undefined, {resizeable:true} );\
    w.sttxt= w.add ('statictext', undefined, 'Write here your math operations for each channels. Math operations must be written in C++.');\
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
    

    sprintf( scriptAC, SET_EXPR_SCRIPT, seqP->exprMemoryAc.redExAc,seqP->exprMemoryAc.greenExAc, seqP->exprMemoryAc.blueExAc, seqP->exprMemoryAc.alphaExAc);
    

    
    ERR(suites.UtilitySuite6()->AEGP_ExecuteScript(globP->my_id, scriptAC, TRUE, &resultMemH, NULL));
    AEFX_CLR_STRUCT(resultAC);
    ERR(suites.MemorySuite1()->AEGP_LockMemHandle(resultMemH, reinterpret_cast<void**>(&resultAC)));
    
    if  (atoi (resultAC) !=0){
        seqP->exprMemoryAc.redExAc = resultAC;
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
	PF_LayerDef		*output )
{
	PF_Err				err		= PF_Err_NONE;
	AEGP_SuiteHandler	suites(in_data->pica_basicP);
    my_sequence_dataP	seqP	= reinterpret_cast<my_sequence_dataP>(DH(in_data->sequence_data));

	/*	Put interesting code here. */
	MathInfo			miP;
	AEFX_CLR_STRUCT(miP);
	A_long				linesL	= 0;

	linesL 		= output->extent_hint.bottom - output->extent_hint.top;
	miP.RedIF	= params[MATH_RED_OPACITY]->u.fs_d.value;
    miP.GreenIF	= params[MATH_RED_OPACITY]->u.fs_d.value;
    miP.BlueIF	= params[MATH_RED_OPACITY]->u.fs_d.value;
    miP.AlphaIF	= params[MATH_RED_OPACITY]->u.fs_d.value;
    
    std::string expression_string = std::string (seqP->exprMemoryAc.redExAc);
    miP.parseRed = A_long (ABS (trig_function<double>(expression_string)));
	
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

