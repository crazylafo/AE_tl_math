#include "tl_math.h"

PF_Err
tlmath_ParamsSetup  (
                     PF_InData        *in_data,
                     PF_OutData        *out_data,
                     PF_ParamDef        *params[],
                     PF_LayerDef        *output )
{
    PF_Err        err        = PF_Err_NONE;
    PF_ParamDef    def;


    AEFX_CLR_STRUCT(def);
    PF_ADD_BUTTON(STR(STR_ID_BUTTON_SETUP_Param_Name),
                  STR(STR_ID_BUTTON_SETUP_Param_Name),
                  0,
                  PF_ParamFlag_SUPERVISE,
                  MATH_SETUP_DISK_ID);

    AEFX_CLR_STRUCT(def);
    ERR(CreateDefaultArb(in_data,
                         out_data,
                         &def.u.arb_d.dephault));


    PF_ADD_ARBITRARY2("preset Name",
                      10,
                      10,
                      PF_ParamFlag_SUPERVISE | PF_ParamFlag_CANNOT_TIME_VARY,
                      PF_PUI_TOPIC,
                      def.u.arb_d.dephault,
                      MATH_ARB_DATA,
                      ARB_REFCON);

    if (!err) {
        PF_CustomUIInfo            ci;

        ci.events                = PF_CustomEFlag_EFFECT;

        ci.comp_ui_width        = ci.comp_ui_height = 0;
        ci.comp_ui_alignment    = PF_UIAlignment_NONE;

        ci.layer_ui_width        =
        ci.layer_ui_height        = 0;
        ci.layer_ui_alignment    = PF_UIAlignment_NONE;

        ci.preview_ui_width        =
        ci.preview_ui_height    = 0;
        ci.layer_ui_alignment    = PF_UIAlignment_NONE;

        err = (*(in_data->inter.register_ui))(in_data->effect_ref, &ci);
    }
    AEFX_CLR_STRUCT(def);
    def.flags= PF_ParamFlag_SUPERVISE;
    PF_ADD_TOPIC(STR( StrID_TOPIC_INPUTS_Param_Name),  MATH_TOPIC_SLIDER_DISK_ID);

    AEFX_CLR_STRUCT(def);
    PF_ADD_FLOAT_SLIDERX(    STR(STR_ID_MATH_SLIDER_ONE_Param_Name),
                         MATH_VAR_MIN,
                         MATH_VAR_MAX,
                         MATH_SLIDER_MIN,
                         MATH_SLIDER_MAX,
                         MATH_VAR_DFLT,
                         PF_Precision_THOUSANDTHS,
                         0,
                         PF_ParamFlag_SUPERVISE,
                         MATH_SLIDER_ONE_DISK_ID);

    AEFX_CLR_STRUCT(def);

    PF_ADD_FLOAT_SLIDERX(    STR(STR_ID_MATH_SLIDER_TWO_Param_Name),
                         MATH_VAR_MIN,
                         MATH_VAR_MAX,
                         MATH_SLIDER_MIN,
                         MATH_SLIDER_MAX,
                         MATH_VAR_DFLT,
                         PF_Precision_THOUSANDTHS,
                         0,
                         PF_ParamFlag_SUPERVISE,
                         MATH_SLIDER_TWO_DISK_ID);

    AEFX_CLR_STRUCT(def);

    PF_ADD_FLOAT_SLIDERX(    STR(STR_ID_MATH_SLIDER_THREE_Param_Name),
                         MATH_VAR_MIN,
                         MATH_VAR_MAX,
                         MATH_SLIDER_MIN,
                         MATH_SLIDER_MAX,
                         MATH_VAR_DFLT,
                         PF_Precision_THOUSANDTHS,
                         0,
                         PF_ParamFlag_SUPERVISE,
                         MATH_SLIDER_THREE_DISK_ID);

    AEFX_CLR_STRUCT(def);

    PF_ADD_FLOAT_SLIDERX(STR( STR_ID_MATH_SLIDER_FOUR_Param_Name),
                         MATH_VAR_MIN,
                         MATH_VAR_MAX,
                         MATH_SLIDER_MIN,
                         MATH_SLIDER_MAX,
                         MATH_VAR_DFLT,
                         PF_Precision_THOUSANDTHS,
                         0,
                         PF_ParamFlag_SUPERVISE,
                         MATH_SLIDER_FOUR_DISK_ID);

    AEFX_CLR_STRUCT(def);

    PF_ADD_FLOAT_SLIDERX(STR( STR_ID_MATH_SLIDER_FIVE_Param_Name),
                         MATH_VAR_MIN,
                         MATH_VAR_MAX,
                         MATH_SLIDER_MIN,
                         MATH_SLIDER_MAX,
                         MATH_VAR_DFLT,
                         PF_Precision_THOUSANDTHS,
                         0,
                         PF_ParamFlag_SUPERVISE,
                         MATH_SLIDER_FIVE_DISK_ID);

    AEFX_CLR_STRUCT(def);

    PF_ADD_FLOAT_SLIDERX(STR( STR_ID_MATH_SLIDER_SIX_Param_Name),
                         MATH_VAR_MIN,
                         MATH_VAR_MAX,
                         MATH_SLIDER_MIN,
                         MATH_SLIDER_MAX,
                         MATH_VAR_DFLT,
                         PF_Precision_THOUSANDTHS,
                         0,
                         PF_ParamFlag_SUPERVISE,
                         MATH_SLIDER_SIX_DISK_ID);

    AEFX_CLR_STRUCT(def);

    PF_ADD_FLOAT_SLIDERX(STR( STR_ID_MATH_SLIDER_SEVEN_Param_Name),
                         MATH_VAR_MIN,
                         MATH_VAR_MAX,
                         MATH_SLIDER_MIN,
                         MATH_SLIDER_MAX,
                         MATH_VAR_DFLT,
                         PF_Precision_THOUSANDTHS,
                         0,
                         PF_ParamFlag_SUPERVISE,
                         MATH_SLIDER_SEVEN_DISK_ID);

    AEFX_CLR_STRUCT(def);

    PF_ADD_FLOAT_SLIDERX(STR( STR_ID_MATH_SLIDER_HEIGHT_Param_Name),
                         MATH_VAR_MIN,
                         MATH_VAR_MAX,
                         MATH_SLIDER_MIN,
                         MATH_SLIDER_MAX,
                         MATH_VAR_DFLT,
                         PF_Precision_THOUSANDTHS,
                         0,
                         PF_ParamFlag_SUPERVISE,
                         MATH_SLIDER_HEIGHT_DISK_ID);

    AEFX_CLR_STRUCT(def);

    PF_ADD_FLOAT_SLIDERX(STR( STR_ID_MATH_SLIDER_NINE_Param_Name),
                         MATH_VAR_MIN,
                         MATH_VAR_MAX,
                         MATH_SLIDER_MIN,
                         MATH_SLIDER_MAX,
                         MATH_VAR_DFLT,
                         PF_Precision_THOUSANDTHS,
                         0,
                         PF_ParamFlag_SUPERVISE,
                         MATH_SLIDER_NINE_DISK_ID);

    AEFX_CLR_STRUCT(def);

    PF_ADD_FLOAT_SLIDERX(STR( STR_ID_MATH_SLIDER_TEN_Param_Name),
                         MATH_VAR_MIN,
                         MATH_VAR_MAX,
                         MATH_SLIDER_MIN,
                         MATH_SLIDER_MAX,
                         MATH_VAR_DFLT,
                         PF_Precision_THOUSANDTHS,
                         0,
                         PF_ParamFlag_SUPERVISE,
                         MATH_SLIDER_TEN_DISK_ID);

    AEFX_CLR_STRUCT(def);

    PF_END_TOPIC( MATH_TOPIC_SLIDER_DISK_ID);

    AEFX_CLR_STRUCT(def);

    def.flags = PF_ParamFlag_SUPERVISE;
    PF_ADD_TOPIC(STR( StrID_TOPIC_INPUTS_Param_Name), MATH_TOPIC_POINTS_DISK_ID);
    AEFX_CLR_STRUCT(def);
    def.flags = PF_ParamFlag_SUPERVISE;
    PF_ADD_POINT_3D(STR (STR_ID_MATH_POINT_ONE_Param_Name), 50, 50, 0,  MATH_POINT_ONE_DISK_ID);
    AEFX_CLR_STRUCT(def);
    def.flags = PF_ParamFlag_SUPERVISE;
    PF_ADD_POINT_3D(STR(STR_ID_MATH_POINT_TWO_Param_Name), 50, 50, 0, MATH_POINT_TWO_DISK_ID);
    AEFX_CLR_STRUCT(def);

    def.flags = PF_ParamFlag_SUPERVISE;
    PF_ADD_POINT_3D(STR(STR_ID_MATH_POINT_THREE_Param_Name), 50, 50, 0, MATH_POINT_THREE_DISK_ID);
    AEFX_CLR_STRUCT(def);

    def.flags = PF_ParamFlag_SUPERVISE;
    PF_ADD_POINT_3D(STR(STR_ID_MATH_POINT_FOUR_Param_Name), 50, 50, 0, MATH_POINT_FOUR_DISK_ID);
    AEFX_CLR_STRUCT(def);

    def.flags = PF_ParamFlag_SUPERVISE;
    PF_ADD_POINT_3D(STR(STR_ID_MATH_POINT_FIVE_Param_Name), 50, 50, 0, MATH_POINT_FIVE_DISK_ID);
    AEFX_CLR_STRUCT(def);

    def.flags = PF_ParamFlag_SUPERVISE;
    PF_ADD_POINT_3D(STR(STR_ID_MATH_POINT_SIX_Param_Name), 50, 50, 0, MATH_POINT_SIX_DISK_ID);
    AEFX_CLR_STRUCT(def);

    def.flags = PF_ParamFlag_SUPERVISE;
    PF_ADD_POINT_3D(STR(STR_ID_MATH_POINT_SEVEN_Param_Name), 50, 50, 0, MATH_POINT_SEVEN_DISK_ID);
    AEFX_CLR_STRUCT(def);

    def.flags = PF_ParamFlag_SUPERVISE;
    PF_ADD_POINT_3D(STR(STR_ID_MATH_POINT_HEIGHT_Param_Name), 50, 50, 0, MATH_POINT_HEIGHT_DISK_ID);
    AEFX_CLR_STRUCT(def);

    def.flags = PF_ParamFlag_SUPERVISE;
    PF_ADD_POINT_3D(STR(STR_ID_MATH_POINT_NINE_Param_Name), 50, 50, 0, MATH_POINT_NINE_DISK_ID);
    AEFX_CLR_STRUCT(def);

    def.flags = PF_ParamFlag_SUPERVISE;
    PF_ADD_POINT_3D(STR(STR_ID_MATH_POINT_TEN_Param_Name), 50, 50, 0, MATH_POINT_TEN_DISK_ID);
    AEFX_CLR_STRUCT(def);


    PF_END_TOPIC(MATH_TOPIC_POINTS_DISK_ID);
    AEFX_CLR_STRUCT(def);

    def.flags = PF_ParamFlag_SUPERVISE;
    PF_ADD_TOPIC(STR(STR_ID_MATH_TOPIC_CB_Param_Name), MATH_TOPIC_CB_DISK_ID);
    AEFX_CLR_STRUCT(def);
    def.flags = PF_ParamFlag_SUPERVISE;
    PF_ADD_CHECKBOXX(STR(STR_ID_MATH_CB_ONE_Param_Name), FALSE, 0, MATH_CB_ONE_DISK_ID);
     AEFX_CLR_STRUCT(def);

    def.flags = PF_ParamFlag_SUPERVISE;
    PF_ADD_CHECKBOXX(STR(STR_ID_MATH_CB_TWO_Param_Name), FALSE, 0, MATH_CB_TWO_DISK_ID);
    AEFX_CLR_STRUCT(def);

    def.flags = PF_ParamFlag_SUPERVISE;
    PF_ADD_CHECKBOXX(STR(STR_ID_MATH_CB_THREE_Param_Name), FALSE, 0, MATH_CB_THREE_DISK_ID);
    AEFX_CLR_STRUCT(def);

    def.flags = PF_ParamFlag_SUPERVISE;
    PF_ADD_CHECKBOXX(STR(STR_ID_MATH_CB_FOUR_Param_Name), FALSE, 0, MATH_CB_FOUR_DISK_ID);
    AEFX_CLR_STRUCT(def);

    def.flags = PF_ParamFlag_SUPERVISE;
    PF_ADD_CHECKBOXX(STR(STR_ID_MATH_CB_FIVE_Param_Name), FALSE, 0, MATH_CB_FIVE_DISK_ID);
    AEFX_CLR_STRUCT(def);

    def.flags = PF_ParamFlag_SUPERVISE;
    PF_ADD_CHECKBOXX(STR(STR_ID_MATH_CB_SIX_Param_Name), FALSE, 0, MATH_CB_SIX_DISK_ID);
    AEFX_CLR_STRUCT(def);

    def.flags = PF_ParamFlag_SUPERVISE;
    PF_ADD_CHECKBOXX(STR(STR_ID_MATH_CB_SEVEN_Param_Name), FALSE, 0, MATH_CB_SEVEN_DISK_ID);
    AEFX_CLR_STRUCT(def);

    def.flags = PF_ParamFlag_SUPERVISE;
    PF_ADD_CHECKBOXX(STR(STR_ID_MATH_CB_HEIGHT_Param_Name), FALSE, 0, MATH_CB_HEIGHT_DISK_ID);
    AEFX_CLR_STRUCT(def);

    def.flags = PF_ParamFlag_SUPERVISE;
    PF_ADD_CHECKBOXX(STR(STR_ID_MATH_CB_NINE_Param_Name), FALSE, 0, MATH_CB_NINE_DISK_ID);
    AEFX_CLR_STRUCT(def);

    def.flags = PF_ParamFlag_SUPERVISE;
    PF_ADD_CHECKBOXX(STR(STR_ID_MATH_CB_TEN_Param_Name), FALSE, 0, MATH_CB_TEN_DISK_ID);
    AEFX_CLR_STRUCT(def);

    PF_END_TOPIC ( MATH_TOPIC_CB_DISK_ID);
    AEFX_CLR_STRUCT(def);

    def.flags = PF_ParamFlag_SUPERVISE;
    PF_ADD_TOPIC(STR( StrID_TOPIC_INPUTS_Param_Name), MATH_TOPIC_COLORS_DISK_ID);
    AEFX_CLR_STRUCT(def);


    def.flags = PF_ParamFlag_SUPERVISE;
    PF_ADD_COLOR(STR(STR_ID_MATH_COLOR_ONE_Param_Name), PF_MAX_CHAN8, PF_MAX_CHAN8, PF_MAX_CHAN8, MATH_COLOR_ONE_DISK_ID);
    AEFX_CLR_STRUCT(def);
    def.flags = PF_ParamFlag_SUPERVISE;
    PF_ADD_COLOR(STR(STR_ID_MATH_COLOR_TWO_Param_Name), PF_MAX_CHAN8, PF_MAX_CHAN8, PF_MAX_CHAN8,MATH_COLOR_TWO_DISK_ID);
    AEFX_CLR_STRUCT(def);
    PF_ADD_COLOR(STR(STR_ID_MATH_COLOR_THREE_Param_Name), PF_MAX_CHAN8, PF_MAX_CHAN8, PF_MAX_CHAN8,MATH_COLOR_THREE_DISK_ID);
    AEFX_CLR_STRUCT(def);
    PF_ADD_COLOR(STR(STR_ID_MATH_COLOR_FOUR_Param_Name), PF_MAX_CHAN8, PF_MAX_CHAN8, PF_MAX_CHAN8,MATH_COLOR_FOUR_DISK_ID);
    AEFX_CLR_STRUCT(def);
    PF_ADD_COLOR(STR(STR_ID_MATH_COLOR_FIVE_Param_Name), PF_MAX_CHAN8, PF_MAX_CHAN8, PF_MAX_CHAN8,MATH_COLOR_FIVE_DISK_ID);
    AEFX_CLR_STRUCT(def);
    PF_ADD_COLOR(STR(STR_ID_MATH_COLOR_SIX_Param_Name), PF_MAX_CHAN8, PF_MAX_CHAN8, PF_MAX_CHAN8,MATH_COLOR_SIX_DISK_ID);
    AEFX_CLR_STRUCT(def);
    PF_ADD_COLOR(STR(STR_ID_MATH_COLOR_SEVEN_Param_Name), PF_MAX_CHAN8, PF_MAX_CHAN8, PF_MAX_CHAN8,MATH_COLOR_SEVEN_DISK_ID);
    AEFX_CLR_STRUCT(def);
    PF_ADD_COLOR(STR(STR_ID_MATH_COLOR_HEIGHT_Param_Name), PF_MAX_CHAN8, PF_MAX_CHAN8, PF_MAX_CHAN8,MATH_COLOR_HEIGHT_DISK_ID);
    AEFX_CLR_STRUCT(def);
    PF_ADD_COLOR(STR(STR_ID_MATH_COLOR_NINE_Param_Name), PF_MAX_CHAN8, PF_MAX_CHAN8, PF_MAX_CHAN8,MATH_COLOR_NINE_DISK_ID);
    AEFX_CLR_STRUCT(def);
    PF_ADD_COLOR(STR(STR_ID_MATH_COLOR_TEN_Param_Name), PF_MAX_CHAN8, PF_MAX_CHAN8, PF_MAX_CHAN8,MATH_COLOR_TEN_DISK_ID);
    AEFX_CLR_STRUCT(def);

    PF_END_TOPIC (MATH_TOPIC_COLORS_DISK_ID);
    AEFX_CLR_STRUCT(def);
    def.flags = PF_ParamFlag_SUPERVISE;
    PF_ADD_TOPIC(STR( StrID_TOPIC_INPUTS_Param_Name), MATH_TOPIC_INPUTS_DISK_ID);
    AEFX_CLR_STRUCT(def);
    def.flags = PF_ParamFlag_SUPERVISE;
    PF_ADD_LAYER(STR(StrID_LAYER_ONE_Param_Name), PF_LayerDefault_MYSELF, MATH_INP_LAYER_ONE_DISK_ID);
    AEFX_CLR_STRUCT(def);
    PF_ADD_FLOAT_SLIDERX(STR( StrID_TOFF_ONE_Param_Name),
                         TIMEOFFSET_MIN,
                         TIMEOFFSET_MAX,
                         MATH_SLIDER_MIN,
                         MATH_SLIDER_MAX,
                         TIMEOFFSET_DFLT,
                         PF_Precision_INTEGER,
                         0,
                         PF_ParamFlag_SUPERVISE ,
                         MATH_INP_TOFF_ONE_DISK_ID);

    PF_ADD_POINT(STR(StrID_POFF_ONE_Param_Name), 50, 50, FALSE, MATH_INP_POFF_ONE_DISK_ID);
    AEFX_CLR_STRUCT(def);

    PF_END_TOPIC(MATH_TOPIC_INPUTS_DISK_ID);
    AEFX_CLR_STRUCT(def);

    // INVISIBLE PARAM TO EXCHANGE WITH CEP
    def.flags        |=    PF_ParamFlag_SUPERVISE |
    PF_ParamFlag_CANNOT_TIME_VARY;

    def.ui_flags    = PF_PUI_STD_CONTROL_ONLY|PF_PUI_INVISIBLE |PF_PUI_NO_ECW_UI;

    PF_ADD_CHECKBOX(STR(StrID_CEP_GETARB_Param_Name),
                    STR(StrID_CEP_GETARB_Param_Name),
                    FALSE,
                    0,
                    MATH_CEP_GET_ARB_DATA_DISK_ID);

    // PF_ADD_CHECKBOXX(STR(StrID_CEP_GETARB_Param_Name), FALSE, PF_ParamFlag_SUPERVISE | PF_ParamFlag_CANNOT_TIME_VARY, MATH_CEP_GET_ARB_DATA_DISK_ID);
    AEFX_CLR_STRUCT(def);

    out_data->num_params = MATH_NUM_PARAMS;
    return err;
}



PF_Err
MakeParamCopy(
	PF_ParamDef *actual[],	/* >> */
	PF_ParamDef copy[])		/* << */
{
	for (A_short iS = 0; iS < MATH_NUM_PARAMS; ++iS) {
		AEFX_CLR_STRUCT(copy[iS]);	// clean params are important!
	}
	copy[MATH_INPUT] = *actual[MATH_INPUT];
	copy[MATH_ARB_DATA] = *actual[MATH_ARB_DATA];
	copy[MATH_TOPIC_SLIDER] = *actual[MATH_TOPIC_SLIDER];
	copy[MATH_SLIDER_ONE] = *actual[MATH_SLIDER_ONE];
	copy[MATH_SLIDER_TWO] = *actual[MATH_SLIDER_TWO];
	copy[MATH_SLIDER_THREE] = *actual[MATH_SLIDER_THREE];
	copy[MATH_SLIDER_FOUR] = *actual[MATH_SLIDER_FOUR];
    copy[MATH_SLIDER_FIVE] = *actual[MATH_SLIDER_FIVE];
    copy[MATH_SLIDER_SIX] = *actual[MATH_SLIDER_SIX];
    copy[MATH_SLIDER_SEVEN] = *actual[MATH_SLIDER_SEVEN];
    copy[MATH_SLIDER_HEIGHT] = *actual[MATH_SLIDER_HEIGHT];
    copy[MATH_SLIDER_NINE] = *actual[MATH_SLIDER_NINE];
    copy[MATH_SLIDER_TEN] = *actual[MATH_SLIDER_TEN];
	copy[MATH_TOPIC_POINTS] = *actual[MATH_TOPIC_POINTS];
    copy[MATH_POINT_ONE] = *actual[MATH_POINT_ONE];
    copy[MATH_POINT_TWO] = *actual[MATH_POINT_TWO];
    copy[MATH_POINT_THREE] = *actual[MATH_POINT_THREE];
    copy[MATH_POINT_FOUR] = *actual[MATH_POINT_FOUR];
    copy[MATH_POINT_FIVE] = *actual[MATH_POINT_FIVE];
    copy[MATH_POINT_SIX] = *actual[MATH_POINT_SIX];
    copy[MATH_POINT_SEVEN] = *actual[MATH_POINT_SEVEN];
    copy[MATH_POINT_HEIGHT] = *actual[MATH_POINT_HEIGHT];
    copy[MATH_POINT_NINE] = *actual[MATH_POINT_NINE];
    copy[MATH_POINT_TEN] = *actual[MATH_POINT_TEN];
    copy[MATH_TOPIC_CB] = *actual[MATH_TOPIC_CB];
    copy[MATH_CB_ONE] = *actual[MATH_CB_ONE];
    copy[MATH_CB_TWO] = *actual[MATH_CB_TWO];
    copy[MATH_CB_THREE] = *actual[MATH_CB_THREE];
    copy[MATH_CB_FOUR] = *actual[MATH_CB_FOUR];
    copy[MATH_CB_FIVE] = *actual[MATH_CB_FIVE];
    copy[MATH_CB_SIX] = *actual[MATH_CB_SIX];
    copy[MATH_CB_SEVEN] = *actual[MATH_CB_SEVEN];
    copy[MATH_CB_HEIGHT] = *actual[MATH_CB_HEIGHT];
    copy[MATH_CB_NINE] = *actual[MATH_CB_NINE];
    copy[MATH_CB_TEN] = *actual[MATH_CB_TEN];

	copy[MATH_TOPIC_COLORS] = *actual[MATH_TOPIC_COLORS];
	copy[MATH_COLOR_ONE] = *actual[MATH_COLOR_ONE];
	copy[MATH_COLOR_TWO] = *actual[MATH_COLOR_TWO];
    copy[MATH_COLOR_THREE] = *actual[MATH_COLOR_THREE];
    copy[MATH_COLOR_FOUR] = *actual[MATH_COLOR_FOUR];
    copy[MATH_COLOR_FIVE] = *actual[MATH_COLOR_FIVE];
    copy[MATH_COLOR_SIX] = *actual[MATH_COLOR_SIX];
    copy[MATH_COLOR_SEVEN] = *actual[MATH_COLOR_SEVEN];
    copy[MATH_COLOR_HEIGHT] = *actual[MATH_COLOR_HEIGHT];
    copy[MATH_COLOR_NINE] = *actual[MATH_COLOR_NINE];
    copy[MATH_COLOR_TEN] = *actual[MATH_COLOR_TEN];

	copy[MATH_TOPIC_INPUTS] = *actual[MATH_TOPIC_INPUTS];
	copy[MATH_INP_LAYER_ONE] = *actual[MATH_INP_LAYER_ONE];
	copy[MATH_INP_TOFF_ONE] = *actual[MATH_INP_TOFF_ONE];
	copy[MATH_INP_POFF_ONE] = *actual[MATH_INP_POFF_ONE];
	copy[MATH_CEP_GET_ARB_DATA] = *actual[MATH_CEP_GET_ARB_DATA];

	return PF_Err_NONE;

}
PF_Err
tlmath_updateSeqData(PF_InData			*in_data,
	PF_OutData			*out_data,
	PF_ParamDef			*params[])
{
	PF_Err				err = PF_Err_NONE;
	AEGP_SuiteHandler		suites(in_data->pica_basicP);
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
		seqData  	*seqP = reinterpret_cast<seqData*>(suites.HandleSuite1()->host_lock_handle(seq_dataH));
		if (seqP->initializedB == false) {
			copyFromArbToSeqData(arbDataStr, seqP);
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
tlmath_UpdateParameterUI(
	PF_InData			*in_data,
	PF_OutData			*out_data,
	PF_ParamDef			*params[],
	PF_LayerDef			*outputP)
{
	PF_Err				err = PF_Err_NONE, err2 = PF_Err_NONE;
	AEGP_SuiteHandler		suites(in_data->pica_basicP);
	AEGP_EffectRefH			meH = NULL;
	PF_ParamDef		param_copy[MATH_NUM_PARAMS];
	ERR(MakeParamCopy(params, param_copy));
	ERR(tlmath_updateSeqData(in_data, out_data, params));
	my_global_dataP		globP = reinterpret_cast<my_global_dataP>(DH(out_data->global_data));
	seqDataP seqP = reinterpret_cast<seqDataP>(DH(out_data->sequence_data));

	AEGP_StreamRefH		MATH_TOPIC_SLIDER_streamH = NULL,
		MATH_SLIDER_ONE_streamH = NULL,
		MATH_SLIDER_TWO_streamH = NULL,
		MATH_SLIDER_THREE_streamH = NULL,
		MATH_SLIDER_FOUR_streamH = NULL,
        MATH_SLIDER_FIVE_streamH = NULL,
        MATH_SLIDER_SIX_streamH = NULL,
        MATH_SLIDER_SEVEN_streamH = NULL,
        MATH_SLIDER_HEIGHT_streamH = NULL,
        MATH_SLIDER_NINE_streamH = NULL,
        MATH_SLIDER_TEN_streamH = NULL,

		MATH_TOPIC_POINTS_streamH = NULL,
        MATH_POINT_ONE_streamH = NULL,
        MATH_POINT_TWO_streamH = NULL,
        MATH_POINT_THREE_streamH = NULL,
        MATH_POINT_FOUR_streamH = NULL,
        MATH_POINT_FIVE_streamH = NULL,
        MATH_POINT_SIX_streamH = NULL,
        MATH_POINT_SEVEN_streamH = NULL,
        MATH_POINT_HEIGHT_streamH = NULL,
        MATH_POINT_NINE_streamH = NULL,
        MATH_POINT_TEN_streamH = NULL,

        MATH_TOPIC_CB_streamH = NULL,
        MATH_CB_ONE_streamH = NULL,
        MATH_CB_TWO_streamH = NULL,
        MATH_CB_THREE_streamH = NULL,
        MATH_CB_FOUR_streamH = NULL,
        MATH_CB_FIVE_streamH = NULL,
        MATH_CB_SIX_streamH = NULL,
        MATH_CB_SEVEN_streamH = NULL,
        MATH_CB_HEIGHT_streamH = NULL,
        MATH_CB_NINE_streamH = NULL,
        MATH_CB_TEN_streamH = NULL,

		MATH_TOPIC_COLORS_streamH = NULL,
		MATH_COLOR_ONE_streamH = NULL,
		MATH_COLOR_TWO_streamH = NULL,
        MATH_COLOR_THREE_streamH = NULL,
        MATH_COLOR_FOUR_streamH = NULL,
        MATH_COLOR_FIVE_streamH = NULL,
        MATH_COLOR_SIX_streamH = NULL,
        MATH_COLOR_SEVEN_streamH = NULL,
        MATH_COLOR_HEIGHT_streamH = NULL,
        MATH_COLOR_NINE_streamH = NULL,
        MATH_COLOR_TEN_streamH = NULL,
        MATH_TOPIC_INPUTS_streamH = NULL;
       //MATH_CEP_GET_ARB_DATA_streamH = NULL;


	if (seqP) {

		strcpy(param_copy[MATH_ARB_DATA].name, seqP->presetNameAc);
		ERR(suites.ParamUtilsSuite3()->PF_UpdateParamUI(in_data->effect_ref,
			MATH_ARB_DATA,
			&param_copy[MATH_ARB_DATA]));

		strcpy(param_copy[MATH_TOPIC_SLIDER].name, seqP->sliderGrpNameAc);
		ERR(suites.ParamUtilsSuite3()->PF_UpdateParamUI(in_data->effect_ref,
			MATH_TOPIC_SLIDER,
			&param_copy[MATH_TOPIC_SLIDER]));

        strcpy(param_copy[MATH_SLIDER_ONE].name, seqP->paramSlider01NameAc);
		ERR(suites.ParamUtilsSuite3()->PF_UpdateParamUI(in_data->effect_ref,
                                                        MATH_SLIDER_ONE,
                                                        &param_copy[MATH_SLIDER_ONE]));

        strcpy(param_copy[MATH_SLIDER_TWO].name, seqP->paramSlider02NameAc);
		ERR(suites.ParamUtilsSuite3()->PF_UpdateParamUI(in_data->effect_ref,
                                                        MATH_SLIDER_TWO,
                                                        &param_copy[MATH_SLIDER_TWO]));

        strcpy(param_copy[MATH_SLIDER_THREE].name, seqP->paramSlider03NameAc);
		ERR(suites.ParamUtilsSuite3()->PF_UpdateParamUI(in_data->effect_ref,
                                                        MATH_SLIDER_THREE,
                                                        &param_copy[MATH_SLIDER_THREE]));

        strcpy(param_copy[MATH_SLIDER_FOUR].name, seqP->paramSlider04NameAc);
		ERR(suites.ParamUtilsSuite3()->PF_UpdateParamUI(in_data->effect_ref,
                                                        MATH_SLIDER_FOUR,
                                                        &param_copy[MATH_SLIDER_FOUR]));

        strcpy(param_copy[MATH_SLIDER_FIVE].name, seqP->paramSlider05NameAc);
        ERR(suites.ParamUtilsSuite3()->PF_UpdateParamUI(in_data->effect_ref,
                                                        MATH_SLIDER_FIVE,
                                                        &param_copy[MATH_SLIDER_FIVE]));

        strcpy(param_copy[MATH_SLIDER_SIX].name, seqP->paramSlider06NameAc);
        ERR(suites.ParamUtilsSuite3()->PF_UpdateParamUI(in_data->effect_ref,
                                                        MATH_SLIDER_SIX,
                                                        &param_copy[MATH_SLIDER_SIX]));

        strcpy(param_copy[MATH_SLIDER_SEVEN].name, seqP->paramSlider07NameAc);
        ERR(suites.ParamUtilsSuite3()->PF_UpdateParamUI(in_data->effect_ref,
                                                        MATH_SLIDER_SEVEN,
                                                        &param_copy[MATH_SLIDER_SEVEN]));

        strcpy(param_copy[MATH_SLIDER_HEIGHT].name, seqP->paramSlider08NameAc);
        ERR(suites.ParamUtilsSuite3()->PF_UpdateParamUI(in_data->effect_ref,
                                                        MATH_SLIDER_HEIGHT,
                                                        &param_copy[MATH_SLIDER_HEIGHT]));

        strcpy(param_copy[MATH_SLIDER_NINE].name, seqP->paramSlider09NameAc);
        ERR(suites.ParamUtilsSuite3()->PF_UpdateParamUI(in_data->effect_ref,
                                                        MATH_SLIDER_NINE,
                                                        &param_copy[MATH_SLIDER_NINE]));

        strcpy(param_copy[MATH_SLIDER_TEN].name, seqP->paramSlider10NameAc);
        ERR(suites.ParamUtilsSuite3()->PF_UpdateParamUI(in_data->effect_ref,
                                                        MATH_SLIDER_TEN,
                                                        &param_copy[MATH_SLIDER_TEN]));


		strcpy(param_copy[MATH_TOPIC_POINTS].name, seqP->pointGrpNameAc);
		ERR(suites.ParamUtilsSuite3()->PF_UpdateParamUI(in_data->effect_ref,
			MATH_TOPIC_POINTS,
			&param_copy[MATH_TOPIC_POINTS]));
		strcpy(param_copy[MATH_POINT_ONE].name, seqP->paramPoint01NameAc);
		ERR(suites.ParamUtilsSuite3()->PF_UpdateParamUI(in_data->effect_ref,
			MATH_POINT_ONE,
			&param_copy[MATH_POINT_ONE]));
		strcpy(param_copy[MATH_POINT_TWO].name, seqP->paramPoint02NameAc);
		ERR(suites.ParamUtilsSuite3()->PF_UpdateParamUI(in_data->effect_ref,
			MATH_POINT_TWO,
			&param_copy[MATH_POINT_TWO]));

        strcpy(param_copy[MATH_POINT_THREE].name, seqP->paramPoint03NameAc);
        ERR(suites.ParamUtilsSuite3()->PF_UpdateParamUI(in_data->effect_ref,
                                                        MATH_POINT_THREE,
                                                        &param_copy[MATH_POINT_THREE]));

        strcpy(param_copy[MATH_POINT_FOUR].name, seqP->paramPoint04NameAc);
        ERR(suites.ParamUtilsSuite3()->PF_UpdateParamUI(in_data->effect_ref,
                                                        MATH_POINT_FOUR,
                                                        &param_copy[MATH_POINT_FOUR]));

        strcpy(param_copy[MATH_POINT_FIVE].name, seqP->paramPoint05NameAc);
        ERR(suites.ParamUtilsSuite3()->PF_UpdateParamUI(in_data->effect_ref,
                                                        MATH_POINT_FIVE,
                                                        &param_copy[MATH_POINT_FIVE]));

        strcpy(param_copy[MATH_POINT_SIX].name, seqP->paramPoint06NameAc);
        ERR(suites.ParamUtilsSuite3()->PF_UpdateParamUI(in_data->effect_ref,
                                                        MATH_POINT_SIX,
                                                        &param_copy[MATH_POINT_SIX]));

        strcpy(param_copy[MATH_POINT_SEVEN].name, seqP->paramPoint07NameAc);
        ERR(suites.ParamUtilsSuite3()->PF_UpdateParamUI(in_data->effect_ref,
                                                        MATH_POINT_SEVEN,
                                                        &param_copy[MATH_POINT_SEVEN]));

        strcpy(param_copy[MATH_POINT_HEIGHT].name, seqP->paramPoint08NameAc);
        ERR(suites.ParamUtilsSuite3()->PF_UpdateParamUI(in_data->effect_ref,
                                                        MATH_POINT_HEIGHT,
                                                        &param_copy[MATH_POINT_HEIGHT]));

        strcpy(param_copy[MATH_POINT_NINE].name, seqP->paramPoint09NameAc);
        ERR(suites.ParamUtilsSuite3()->PF_UpdateParamUI(in_data->effect_ref,
                                                        MATH_POINT_NINE,
                                                        &param_copy[MATH_POINT_NINE]));

        strcpy(param_copy[MATH_POINT_TEN].name, seqP->paramPoint10NameAc);
        ERR(suites.ParamUtilsSuite3()->PF_UpdateParamUI(in_data->effect_ref,
                                                        MATH_POINT_TEN,
                                                        &param_copy[MATH_POINT_TEN]));


        strcpy(param_copy[MATH_TOPIC_CB].name, seqP->cbGrpNameAc);
        ERR(suites.ParamUtilsSuite3()->PF_UpdateParamUI(in_data->effect_ref,
                                                        MATH_TOPIC_CB,
                                                        &param_copy[MATH_TOPIC_CB]));

        strcpy(param_copy[MATH_CB_ONE].name, seqP->paramCb01NameAc);
        ERR(suites.ParamUtilsSuite3()->PF_UpdateParamUI(in_data->effect_ref,
                                                        MATH_CB_ONE,
                                                        &param_copy[MATH_CB_ONE]));
        strcpy(param_copy[MATH_CB_TWO].name, seqP->paramCb02NameAc);
        ERR(suites.ParamUtilsSuite3()->PF_UpdateParamUI(in_data->effect_ref,
                                                        MATH_CB_TWO,
                                                        &param_copy[MATH_CB_TWO]));

        strcpy(param_copy[MATH_CB_THREE].name, seqP->paramCb03NameAc);
        ERR(suites.ParamUtilsSuite3()->PF_UpdateParamUI(in_data->effect_ref,
                                                        MATH_CB_THREE,
                                                        &param_copy[MATH_CB_THREE]));

        strcpy(param_copy[MATH_CB_FOUR].name, seqP->paramCb04NameAc);
        ERR(suites.ParamUtilsSuite3()->PF_UpdateParamUI(in_data->effect_ref,
                                                        MATH_CB_FOUR,
                                                        &param_copy[MATH_CB_FOUR]));

        strcpy(param_copy[MATH_CB_FIVE].name, seqP->paramCb05NameAc);
        ERR(suites.ParamUtilsSuite3()->PF_UpdateParamUI(in_data->effect_ref,
                                                        MATH_CB_FIVE,
                                                        &param_copy[MATH_CB_FIVE]));

        strcpy(param_copy[MATH_CB_SIX].name, seqP->paramCb06NameAc);
        ERR(suites.ParamUtilsSuite3()->PF_UpdateParamUI(in_data->effect_ref,
                                                        MATH_CB_SIX,
                                                        &param_copy[MATH_CB_SIX]));

        strcpy(param_copy[MATH_CB_SEVEN].name, seqP->paramCb07NameAc);
        ERR(suites.ParamUtilsSuite3()->PF_UpdateParamUI(in_data->effect_ref,
                                                        MATH_CB_SEVEN,
                                                        &param_copy[MATH_CB_SEVEN]));

        strcpy(param_copy[MATH_CB_HEIGHT].name, seqP->paramCb08NameAc);
        ERR(suites.ParamUtilsSuite3()->PF_UpdateParamUI(in_data->effect_ref,
                                                        MATH_CB_HEIGHT,
                                                        &param_copy[MATH_CB_HEIGHT]));

        strcpy(param_copy[MATH_CB_NINE].name, seqP->paramCb09NameAc);
        ERR(suites.ParamUtilsSuite3()->PF_UpdateParamUI(in_data->effect_ref,
                                                        MATH_CB_NINE,
                                                        &param_copy[MATH_CB_NINE]));

        strcpy(param_copy[MATH_CB_TEN].name, seqP->paramCb10NameAc);
        ERR(suites.ParamUtilsSuite3()->PF_UpdateParamUI(in_data->effect_ref,
                                                        MATH_CB_TEN,
                                                        &param_copy[MATH_CB_TEN]));


		strcpy(param_copy[MATH_TOPIC_COLORS].name, seqP->colorGrpNameAc);
		ERR(suites.ParamUtilsSuite3()->PF_UpdateParamUI(in_data->effect_ref,
			MATH_TOPIC_COLORS,
			&param_copy[MATH_TOPIC_COLORS]));

		strcpy(param_copy[MATH_COLOR_ONE].name, seqP->paramColor01NameAc);
		ERR(suites.ParamUtilsSuite3()->PF_UpdateParamUI(in_data->effect_ref,
			MATH_COLOR_ONE,
			&param_copy[MATH_COLOR_ONE]));

		strcpy(param_copy[MATH_COLOR_TWO].name, seqP->paramColor02NameAc);
		ERR(suites.ParamUtilsSuite3()->PF_UpdateParamUI(in_data->effect_ref,
			MATH_COLOR_TWO,
			&param_copy[MATH_COLOR_TWO]));


        strcpy(param_copy[MATH_COLOR_THREE].name, seqP->paramColor03NameAc);
        ERR(suites.ParamUtilsSuite3()->PF_UpdateParamUI(in_data->effect_ref,
                                                        MATH_COLOR_THREE,
                                                        &param_copy[MATH_COLOR_THREE]));

        strcpy(param_copy[MATH_COLOR_FOUR].name, seqP->paramColor04NameAc);
        ERR(suites.ParamUtilsSuite3()->PF_UpdateParamUI(in_data->effect_ref,
                                                        MATH_COLOR_FOUR,
                                                        &param_copy[MATH_COLOR_FOUR]));

        strcpy(param_copy[MATH_COLOR_FIVE].name, seqP->paramColor05NameAc);
        ERR(suites.ParamUtilsSuite3()->PF_UpdateParamUI(in_data->effect_ref,
                                                        MATH_COLOR_FIVE,
                                                        &param_copy[MATH_COLOR_FIVE]));

        strcpy(param_copy[MATH_COLOR_SIX].name, seqP->paramColor06NameAc);
        ERR(suites.ParamUtilsSuite3()->PF_UpdateParamUI(in_data->effect_ref,
                                                        MATH_COLOR_SIX,
                                                        &param_copy[MATH_COLOR_SIX]));

        strcpy(param_copy[MATH_COLOR_SEVEN].name, seqP->paramColor07NameAc);
        ERR(suites.ParamUtilsSuite3()->PF_UpdateParamUI(in_data->effect_ref,
                                                        MATH_COLOR_SEVEN,
                                                        &param_copy[MATH_COLOR_SEVEN]));

        strcpy(param_copy[MATH_COLOR_HEIGHT].name, seqP->paramColor08NameAc);
        ERR(suites.ParamUtilsSuite3()->PF_UpdateParamUI(in_data->effect_ref,
                                                        MATH_COLOR_HEIGHT,
                                                        &param_copy[MATH_COLOR_HEIGHT]));

        strcpy(param_copy[MATH_COLOR_NINE].name, seqP->paramColor09NameAc);
        ERR(suites.ParamUtilsSuite3()->PF_UpdateParamUI(in_data->effect_ref,
                                                        MATH_COLOR_NINE,
                                                        &param_copy[MATH_COLOR_NINE]));

        strcpy(param_copy[MATH_COLOR_TEN].name, seqP->paramColor10NameAc);
        ERR(suites.ParamUtilsSuite3()->PF_UpdateParamUI(in_data->effect_ref,
                                                        MATH_COLOR_TEN,
                                                        &param_copy[MATH_COLOR_TEN]));




		strcpy(param_copy[MATH_TOPIC_INPUTS].name, seqP->layerGrpNameAc);
		ERR(suites.ParamUtilsSuite3()->PF_UpdateParamUI(in_data->effect_ref,
			MATH_TOPIC_INPUTS,
			&param_copy[MATH_TOPIC_INPUTS]));

		ERR(suites.PFInterfaceSuite1()->AEGP_GetNewEffectForEffect(globP->my_id, in_data->effect_ref, &meH));
		ERR(suites.StreamSuite2()->AEGP_GetNewEffectStreamByIndex(globP->my_id, meH, MATH_TOPIC_SLIDER, &MATH_TOPIC_SLIDER_streamH));
        ERR(suites.StreamSuite2()->AEGP_GetNewEffectStreamByIndex(globP->my_id, meH, MATH_SLIDER_ONE, &MATH_SLIDER_ONE_streamH));
        ERR(suites.StreamSuite2()->AEGP_GetNewEffectStreamByIndex(globP->my_id, meH, MATH_SLIDER_TWO, &MATH_SLIDER_TWO_streamH));
        ERR(suites.StreamSuite2()->AEGP_GetNewEffectStreamByIndex(globP->my_id, meH, MATH_SLIDER_THREE, &MATH_SLIDER_THREE_streamH));
        ERR(suites.StreamSuite2()->AEGP_GetNewEffectStreamByIndex(globP->my_id, meH, MATH_SLIDER_FOUR, &MATH_SLIDER_FOUR_streamH));
        ERR(suites.StreamSuite2()->AEGP_GetNewEffectStreamByIndex(globP->my_id, meH, MATH_SLIDER_ONE, &MATH_SLIDER_FIVE_streamH));
        ERR(suites.StreamSuite2()->AEGP_GetNewEffectStreamByIndex(globP->my_id, meH, MATH_SLIDER_TWO, &MATH_SLIDER_SIX_streamH));
        ERR(suites.StreamSuite2()->AEGP_GetNewEffectStreamByIndex(globP->my_id, meH, MATH_SLIDER_THREE, &MATH_SLIDER_SEVEN_streamH));
        ERR(suites.StreamSuite2()->AEGP_GetNewEffectStreamByIndex(globP->my_id, meH, MATH_SLIDER_FOUR, &MATH_SLIDER_HEIGHT_streamH));
        ERR(suites.StreamSuite2()->AEGP_GetNewEffectStreamByIndex(globP->my_id, meH, MATH_SLIDER_FOUR, &MATH_SLIDER_NINE_streamH));
        ERR(suites.StreamSuite2()->AEGP_GetNewEffectStreamByIndex(globP->my_id, meH, MATH_SLIDER_FOUR, &MATH_SLIDER_TEN_streamH));

		ERR(suites.StreamSuite2()->AEGP_GetNewEffectStreamByIndex(globP->my_id, meH, MATH_TOPIC_POINTS, &MATH_TOPIC_POINTS_streamH));
		ERR(suites.StreamSuite2()->AEGP_GetNewEffectStreamByIndex(globP->my_id, meH, MATH_POINT_ONE, &MATH_POINT_ONE_streamH));
		ERR(suites.StreamSuite2()->AEGP_GetNewEffectStreamByIndex(globP->my_id, meH, MATH_POINT_TWO, &MATH_POINT_TWO_streamH));
        ERR(suites.StreamSuite2()->AEGP_GetNewEffectStreamByIndex(globP->my_id, meH, MATH_POINT_THREE, &MATH_POINT_THREE_streamH));
        ERR(suites.StreamSuite2()->AEGP_GetNewEffectStreamByIndex(globP->my_id, meH, MATH_POINT_FOUR, &MATH_POINT_FOUR_streamH));
        ERR(suites.StreamSuite2()->AEGP_GetNewEffectStreamByIndex(globP->my_id, meH, MATH_POINT_ONE, &MATH_POINT_FIVE_streamH));
        ERR(suites.StreamSuite2()->AEGP_GetNewEffectStreamByIndex(globP->my_id, meH, MATH_POINT_TWO, &MATH_POINT_SIX_streamH));
        ERR(suites.StreamSuite2()->AEGP_GetNewEffectStreamByIndex(globP->my_id, meH, MATH_POINT_THREE, &MATH_POINT_SEVEN_streamH));
        ERR(suites.StreamSuite2()->AEGP_GetNewEffectStreamByIndex(globP->my_id, meH, MATH_POINT_FOUR, &MATH_POINT_HEIGHT_streamH));
        ERR(suites.StreamSuite2()->AEGP_GetNewEffectStreamByIndex(globP->my_id, meH, MATH_POINT_FOUR, &MATH_POINT_NINE_streamH));
        ERR(suites.StreamSuite2()->AEGP_GetNewEffectStreamByIndex(globP->my_id, meH, MATH_POINT_FOUR, &MATH_POINT_TEN_streamH));

        ERR(suites.StreamSuite2()->AEGP_GetNewEffectStreamByIndex(globP->my_id, meH, MATH_TOPIC_CB, &MATH_TOPIC_CB_streamH));
        ERR(suites.StreamSuite2()->AEGP_GetNewEffectStreamByIndex(globP->my_id, meH, MATH_CB_ONE, &MATH_CB_ONE_streamH));
        ERR(suites.StreamSuite2()->AEGP_GetNewEffectStreamByIndex(globP->my_id, meH, MATH_CB_TWO, &MATH_CB_TWO_streamH));
        ERR(suites.StreamSuite2()->AEGP_GetNewEffectStreamByIndex(globP->my_id, meH, MATH_CB_THREE, &MATH_CB_THREE_streamH));
        ERR(suites.StreamSuite2()->AEGP_GetNewEffectStreamByIndex(globP->my_id, meH, MATH_CB_FOUR, &MATH_CB_FOUR_streamH));
        ERR(suites.StreamSuite2()->AEGP_GetNewEffectStreamByIndex(globP->my_id, meH, MATH_CB_ONE, &MATH_CB_FIVE_streamH));
        ERR(suites.StreamSuite2()->AEGP_GetNewEffectStreamByIndex(globP->my_id, meH, MATH_CB_TWO, &MATH_CB_SIX_streamH));
        ERR(suites.StreamSuite2()->AEGP_GetNewEffectStreamByIndex(globP->my_id, meH, MATH_CB_THREE, &MATH_CB_SEVEN_streamH));
        ERR(suites.StreamSuite2()->AEGP_GetNewEffectStreamByIndex(globP->my_id, meH, MATH_CB_FOUR, &MATH_CB_HEIGHT_streamH));
        ERR(suites.StreamSuite2()->AEGP_GetNewEffectStreamByIndex(globP->my_id, meH, MATH_CB_FOUR, &MATH_CB_NINE_streamH));
        ERR(suites.StreamSuite2()->AEGP_GetNewEffectStreamByIndex(globP->my_id, meH, MATH_CB_FOUR, &MATH_CB_TEN_streamH));


		ERR(suites.StreamSuite2()->AEGP_GetNewEffectStreamByIndex(globP->my_id, meH, MATH_TOPIC_COLORS, &MATH_TOPIC_COLORS_streamH));
		ERR(suites.StreamSuite2()->AEGP_GetNewEffectStreamByIndex(globP->my_id, meH, MATH_COLOR_ONE, &MATH_COLOR_ONE_streamH));
		ERR(suites.StreamSuite2()->AEGP_GetNewEffectStreamByIndex(globP->my_id, meH, MATH_COLOR_TWO, &MATH_COLOR_TWO_streamH));
        ERR(suites.StreamSuite2()->AEGP_GetNewEffectStreamByIndex(globP->my_id, meH, MATH_COLOR_THREE, &MATH_COLOR_THREE_streamH));
        ERR(suites.StreamSuite2()->AEGP_GetNewEffectStreamByIndex(globP->my_id, meH, MATH_COLOR_FOUR, &MATH_COLOR_FOUR_streamH));
        ERR(suites.StreamSuite2()->AEGP_GetNewEffectStreamByIndex(globP->my_id, meH, MATH_COLOR_ONE, &MATH_COLOR_FIVE_streamH));
        ERR(suites.StreamSuite2()->AEGP_GetNewEffectStreamByIndex(globP->my_id, meH, MATH_COLOR_TWO, &MATH_COLOR_SIX_streamH));
        ERR(suites.StreamSuite2()->AEGP_GetNewEffectStreamByIndex(globP->my_id, meH, MATH_COLOR_THREE, &MATH_COLOR_SEVEN_streamH));
        ERR(suites.StreamSuite2()->AEGP_GetNewEffectStreamByIndex(globP->my_id, meH, MATH_COLOR_FOUR, &MATH_COLOR_HEIGHT_streamH));
        ERR(suites.StreamSuite2()->AEGP_GetNewEffectStreamByIndex(globP->my_id, meH, MATH_COLOR_FOUR, &MATH_COLOR_NINE_streamH));
        ERR(suites.StreamSuite2()->AEGP_GetNewEffectStreamByIndex(globP->my_id, meH, MATH_COLOR_FOUR, &MATH_COLOR_TEN_streamH));
		ERR(suites.StreamSuite2()->AEGP_GetNewEffectStreamByIndex(globP->my_id, meH, MATH_TOPIC_INPUTS, &MATH_TOPIC_INPUTS_streamH));
        //ERR(suites.StreamSuite2()->AEGP_GetNewEffectStreamByIndex(globP->my_id, meH,MATH_CEP_GET_ARB_DATA, &MATH_CEP_GET_ARB_DATA_streamH));

		ERR(suites.DynamicStreamSuite2()->AEGP_SetDynamicStreamFlag(MATH_TOPIC_SLIDER_streamH, AEGP_DynStreamFlag_HIDDEN, FALSE, !seqP->sliderGrpVisibleB));
		ERR(suites.DynamicStreamSuite2()->AEGP_SetDynamicStreamFlag(MATH_SLIDER_ONE_streamH, AEGP_DynStreamFlag_HIDDEN, FALSE, !seqP->paramSlider01VisibleB));
		ERR(suites.DynamicStreamSuite2()->AEGP_SetDynamicStreamFlag(MATH_SLIDER_TWO_streamH, AEGP_DynStreamFlag_HIDDEN, FALSE, !seqP->paramSlider02VisibleB));
		ERR(suites.DynamicStreamSuite2()->AEGP_SetDynamicStreamFlag(MATH_SLIDER_THREE_streamH, AEGP_DynStreamFlag_HIDDEN, FALSE, !seqP->paramSlider03VisibleB));
		ERR(suites.DynamicStreamSuite2()->AEGP_SetDynamicStreamFlag(MATH_SLIDER_FOUR_streamH, AEGP_DynStreamFlag_HIDDEN, FALSE, !seqP->paramSlider04VisibleB));
        ERR(suites.DynamicStreamSuite2()->AEGP_SetDynamicStreamFlag(MATH_SLIDER_FIVE_streamH, AEGP_DynStreamFlag_HIDDEN, FALSE, !seqP->paramSlider05VisibleB));
        ERR(suites.DynamicStreamSuite2()->AEGP_SetDynamicStreamFlag(MATH_SLIDER_SIX_streamH, AEGP_DynStreamFlag_HIDDEN, FALSE, !seqP->paramSlider06VisibleB));
        ERR(suites.DynamicStreamSuite2()->AEGP_SetDynamicStreamFlag(MATH_SLIDER_SEVEN_streamH, AEGP_DynStreamFlag_HIDDEN, FALSE, !seqP->paramSlider07VisibleB));
        ERR(suites.DynamicStreamSuite2()->AEGP_SetDynamicStreamFlag(MATH_SLIDER_HEIGHT_streamH, AEGP_DynStreamFlag_HIDDEN, FALSE, !seqP->paramSlider08VisibleB));
        ERR(suites.DynamicStreamSuite2()->AEGP_SetDynamicStreamFlag(MATH_SLIDER_NINE_streamH, AEGP_DynStreamFlag_HIDDEN, FALSE, !seqP->paramSlider09VisibleB));
        ERR(suites.DynamicStreamSuite2()->AEGP_SetDynamicStreamFlag(MATH_SLIDER_TEN_streamH, AEGP_DynStreamFlag_HIDDEN, FALSE, !seqP->paramSlider10VisibleB));

		ERR(suites.DynamicStreamSuite2()->AEGP_SetDynamicStreamFlag(MATH_TOPIC_POINTS_streamH, AEGP_DynStreamFlag_HIDDEN, FALSE, !seqP->pointGrpVisibleB));
		ERR(suites.DynamicStreamSuite2()->AEGP_SetDynamicStreamFlag(MATH_POINT_ONE_streamH, AEGP_DynStreamFlag_HIDDEN, FALSE, !seqP->paramPoint01VisibleB));
		ERR(suites.DynamicStreamSuite2()->AEGP_SetDynamicStreamFlag(MATH_POINT_TWO_streamH, AEGP_DynStreamFlag_HIDDEN, FALSE, !seqP->paramPoint02VisibleB));
        ERR(suites.DynamicStreamSuite2()->AEGP_SetDynamicStreamFlag(MATH_POINT_THREE_streamH, AEGP_DynStreamFlag_HIDDEN, FALSE, !seqP->paramPoint03VisibleB));
        ERR(suites.DynamicStreamSuite2()->AEGP_SetDynamicStreamFlag(MATH_POINT_FOUR_streamH, AEGP_DynStreamFlag_HIDDEN, FALSE, !seqP->paramPoint04VisibleB));
        ERR(suites.DynamicStreamSuite2()->AEGP_SetDynamicStreamFlag(MATH_POINT_FIVE_streamH, AEGP_DynStreamFlag_HIDDEN, FALSE, !seqP->paramPoint05VisibleB));
        ERR(suites.DynamicStreamSuite2()->AEGP_SetDynamicStreamFlag(MATH_POINT_SIX_streamH, AEGP_DynStreamFlag_HIDDEN, FALSE, !seqP->paramPoint06VisibleB));
        ERR(suites.DynamicStreamSuite2()->AEGP_SetDynamicStreamFlag(MATH_POINT_SEVEN_streamH, AEGP_DynStreamFlag_HIDDEN, FALSE, !seqP->paramPoint07VisibleB));
        ERR(suites.DynamicStreamSuite2()->AEGP_SetDynamicStreamFlag(MATH_POINT_HEIGHT_streamH, AEGP_DynStreamFlag_HIDDEN, FALSE, !seqP->paramPoint08VisibleB));
        ERR(suites.DynamicStreamSuite2()->AEGP_SetDynamicStreamFlag(MATH_POINT_NINE_streamH, AEGP_DynStreamFlag_HIDDEN, FALSE, !seqP->paramPoint09VisibleB));
        ERR(suites.DynamicStreamSuite2()->AEGP_SetDynamicStreamFlag(MATH_POINT_TEN_streamH, AEGP_DynStreamFlag_HIDDEN, FALSE, !seqP->paramPoint10VisibleB));

        ERR(suites.DynamicStreamSuite2()->AEGP_SetDynamicStreamFlag(MATH_TOPIC_CB_streamH, AEGP_DynStreamFlag_HIDDEN, FALSE, !seqP->cbGrpVisibleB));
        ERR(suites.DynamicStreamSuite2()->AEGP_SetDynamicStreamFlag(MATH_CB_ONE_streamH, AEGP_DynStreamFlag_HIDDEN, FALSE, !seqP->paramCb01VisibleB));
        ERR(suites.DynamicStreamSuite2()->AEGP_SetDynamicStreamFlag(MATH_CB_TWO_streamH, AEGP_DynStreamFlag_HIDDEN, FALSE, !seqP->paramCb02VisibleB));
        ERR(suites.DynamicStreamSuite2()->AEGP_SetDynamicStreamFlag(MATH_CB_THREE_streamH, AEGP_DynStreamFlag_HIDDEN, FALSE, !seqP->paramCb03VisibleB));
        ERR(suites.DynamicStreamSuite2()->AEGP_SetDynamicStreamFlag(MATH_CB_FOUR_streamH, AEGP_DynStreamFlag_HIDDEN, FALSE, !seqP->paramCb04VisibleB));
        ERR(suites.DynamicStreamSuite2()->AEGP_SetDynamicStreamFlag(MATH_CB_FIVE_streamH, AEGP_DynStreamFlag_HIDDEN, FALSE, !seqP->paramCb05VisibleB));
        ERR(suites.DynamicStreamSuite2()->AEGP_SetDynamicStreamFlag(MATH_CB_SIX_streamH, AEGP_DynStreamFlag_HIDDEN, FALSE, !seqP->paramCb06VisibleB));
        ERR(suites.DynamicStreamSuite2()->AEGP_SetDynamicStreamFlag(MATH_CB_SEVEN_streamH, AEGP_DynStreamFlag_HIDDEN, FALSE, !seqP->paramCb07VisibleB));
        ERR(suites.DynamicStreamSuite2()->AEGP_SetDynamicStreamFlag(MATH_CB_HEIGHT_streamH, AEGP_DynStreamFlag_HIDDEN, FALSE, !seqP->paramCb08VisibleB));
        ERR(suites.DynamicStreamSuite2()->AEGP_SetDynamicStreamFlag(MATH_CB_NINE_streamH, AEGP_DynStreamFlag_HIDDEN, FALSE, !seqP->paramCb09VisibleB));
        ERR(suites.DynamicStreamSuite2()->AEGP_SetDynamicStreamFlag(MATH_CB_TEN_streamH, AEGP_DynStreamFlag_HIDDEN, FALSE, !seqP->paramCb10VisibleB));

		ERR(suites.DynamicStreamSuite2()->AEGP_SetDynamicStreamFlag(MATH_TOPIC_COLORS_streamH, AEGP_DynStreamFlag_HIDDEN, FALSE, !seqP->colorGrpVisibleB));
		ERR(suites.DynamicStreamSuite2()->AEGP_SetDynamicStreamFlag(MATH_COLOR_ONE_streamH, AEGP_DynStreamFlag_HIDDEN, FALSE, !seqP->paramColor01VisibleB));
		ERR(suites.DynamicStreamSuite2()->AEGP_SetDynamicStreamFlag(MATH_COLOR_TWO_streamH, AEGP_DynStreamFlag_HIDDEN, FALSE, !seqP->paramColor02VisibleB));
        ERR(suites.DynamicStreamSuite2()->AEGP_SetDynamicStreamFlag(MATH_COLOR_THREE_streamH, AEGP_DynStreamFlag_HIDDEN, FALSE, !seqP->paramColor03VisibleB));
        ERR(suites.DynamicStreamSuite2()->AEGP_SetDynamicStreamFlag(MATH_COLOR_FOUR_streamH, AEGP_DynStreamFlag_HIDDEN, FALSE, !seqP->paramColor04VisibleB));
        ERR(suites.DynamicStreamSuite2()->AEGP_SetDynamicStreamFlag(MATH_COLOR_FIVE_streamH, AEGP_DynStreamFlag_HIDDEN, FALSE, !seqP->paramColor05VisibleB));
        ERR(suites.DynamicStreamSuite2()->AEGP_SetDynamicStreamFlag(MATH_COLOR_SIX_streamH, AEGP_DynStreamFlag_HIDDEN, FALSE, !seqP->paramColor06VisibleB));
        ERR(suites.DynamicStreamSuite2()->AEGP_SetDynamicStreamFlag(MATH_COLOR_SEVEN_streamH, AEGP_DynStreamFlag_HIDDEN, FALSE, !seqP->paramColor07VisibleB));
        ERR(suites.DynamicStreamSuite2()->AEGP_SetDynamicStreamFlag(MATH_COLOR_HEIGHT_streamH, AEGP_DynStreamFlag_HIDDEN, FALSE, !seqP->paramColor08VisibleB));
        ERR(suites.DynamicStreamSuite2()->AEGP_SetDynamicStreamFlag(MATH_COLOR_NINE_streamH, AEGP_DynStreamFlag_HIDDEN, FALSE, !seqP->paramColor09VisibleB));
        ERR(suites.DynamicStreamSuite2()->AEGP_SetDynamicStreamFlag(MATH_COLOR_TEN_streamH, AEGP_DynStreamFlag_HIDDEN, FALSE, !seqP->paramColor10VisibleB));



		ERR(suites.DynamicStreamSuite2()->AEGP_SetDynamicStreamFlag(MATH_TOPIC_INPUTS_streamH, AEGP_DynStreamFlag_HIDDEN, FALSE, !seqP->layerGrpVisibleB));

        //ERR(suites.DynamicStreamSuite2()->AEGP_SetDynamicStreamFlag(MATH_CEP_GET_ARB_DATA_streamH,AEGP_DynStreamFlag_HIDDEN, FALSE, TRUE));


		if (meH) {
			ERR2(suites.EffectSuite2()->AEGP_DisposeEffect(meH));
		}
		if (MATH_TOPIC_SLIDER_streamH) {
			ERR2(suites.StreamSuite2()->AEGP_DisposeStream(MATH_TOPIC_SLIDER_streamH));
		}
		if (MATH_SLIDER_ONE_streamH) {
			ERR2(suites.StreamSuite2()->AEGP_DisposeStream(MATH_SLIDER_ONE_streamH));
		}
		if (MATH_SLIDER_TWO_streamH) {
			ERR2(suites.StreamSuite2()->AEGP_DisposeStream(MATH_SLIDER_TWO_streamH));
		}
		if (MATH_SLIDER_THREE_streamH) {
			ERR2(suites.StreamSuite2()->AEGP_DisposeStream(MATH_SLIDER_THREE_streamH));
		}
		if (MATH_SLIDER_FOUR_streamH) {
			ERR2(suites.StreamSuite2()->AEGP_DisposeStream(MATH_SLIDER_FOUR_streamH));
		}
        if (MATH_SLIDER_FIVE_streamH) {
            ERR2(suites.StreamSuite2()->AEGP_DisposeStream(MATH_SLIDER_FIVE_streamH));
        }
        if (MATH_SLIDER_SIX_streamH) {
            ERR2(suites.StreamSuite2()->AEGP_DisposeStream(MATH_SLIDER_SIX_streamH));
        }
        if (MATH_SLIDER_SEVEN_streamH) {
            ERR2(suites.StreamSuite2()->AEGP_DisposeStream(MATH_SLIDER_SEVEN_streamH));
        }
        if (MATH_SLIDER_HEIGHT_streamH) {
            ERR2(suites.StreamSuite2()->AEGP_DisposeStream(MATH_SLIDER_HEIGHT_streamH));
        }
        if (MATH_SLIDER_NINE_streamH) {
            ERR2(suites.StreamSuite2()->AEGP_DisposeStream(MATH_SLIDER_NINE_streamH));
        }
        if (MATH_SLIDER_TEN_streamH) {
            ERR2(suites.StreamSuite2()->AEGP_DisposeStream(MATH_SLIDER_TEN_streamH));
        }

		if (MATH_TOPIC_POINTS_streamH) {
			ERR2(suites.StreamSuite2()->AEGP_DisposeStream(MATH_TOPIC_POINTS_streamH));
		}
        if (MATH_POINT_ONE_streamH) {
            ERR2(suites.StreamSuite2()->AEGP_DisposeStream(MATH_POINT_ONE_streamH));
        }
        if (MATH_POINT_TWO_streamH) {
            ERR2(suites.StreamSuite2()->AEGP_DisposeStream(MATH_POINT_TWO_streamH));
        }
        if (MATH_POINT_THREE_streamH) {
            ERR2(suites.StreamSuite2()->AEGP_DisposeStream(MATH_POINT_THREE_streamH));
        }
        if (MATH_POINT_FOUR_streamH) {
            ERR2(suites.StreamSuite2()->AEGP_DisposeStream(MATH_POINT_FOUR_streamH));
        }
        if (MATH_POINT_FIVE_streamH) {
            ERR2(suites.StreamSuite2()->AEGP_DisposeStream(MATH_POINT_FIVE_streamH));
        }
        if (MATH_POINT_SIX_streamH) {
            ERR2(suites.StreamSuite2()->AEGP_DisposeStream(MATH_POINT_SIX_streamH));
        }
        if (MATH_POINT_SEVEN_streamH) {
            ERR2(suites.StreamSuite2()->AEGP_DisposeStream(MATH_POINT_SEVEN_streamH));
        }
        if (MATH_POINT_HEIGHT_streamH) {
            ERR2(suites.StreamSuite2()->AEGP_DisposeStream(MATH_POINT_HEIGHT_streamH));
        }
        if (MATH_POINT_NINE_streamH) {
            ERR2(suites.StreamSuite2()->AEGP_DisposeStream(MATH_POINT_NINE_streamH));
        }
        if (MATH_POINT_TEN_streamH) {
            ERR2(suites.StreamSuite2()->AEGP_DisposeStream(MATH_POINT_TEN_streamH));
        }

        if (MATH_TOPIC_CB_streamH) {
            ERR2(suites.StreamSuite2()->AEGP_DisposeStream(MATH_TOPIC_CB_streamH));
        }
        if (MATH_CB_ONE_streamH) {
            ERR2(suites.StreamSuite2()->AEGP_DisposeStream(MATH_CB_ONE_streamH));
        }
        if (MATH_CB_TWO_streamH) {
            ERR2(suites.StreamSuite2()->AEGP_DisposeStream(MATH_CB_TWO_streamH));
        }
        if (MATH_CB_THREE_streamH) {
            ERR2(suites.StreamSuite2()->AEGP_DisposeStream(MATH_CB_THREE_streamH));
        }
        if (MATH_CB_FOUR_streamH) {
            ERR2(suites.StreamSuite2()->AEGP_DisposeStream(MATH_CB_FOUR_streamH));
        }
        if (MATH_CB_FIVE_streamH) {
            ERR2(suites.StreamSuite2()->AEGP_DisposeStream(MATH_CB_FIVE_streamH));
        }
        if (MATH_CB_SIX_streamH) {
            ERR2(suites.StreamSuite2()->AEGP_DisposeStream(MATH_CB_SIX_streamH));
        }
        if (MATH_CB_SEVEN_streamH) {
            ERR2(suites.StreamSuite2()->AEGP_DisposeStream(MATH_CB_SEVEN_streamH));
        }
        if (MATH_CB_HEIGHT_streamH) {
            ERR2(suites.StreamSuite2()->AEGP_DisposeStream(MATH_CB_HEIGHT_streamH));
        }
        if (MATH_CB_NINE_streamH) {
            ERR2(suites.StreamSuite2()->AEGP_DisposeStream(MATH_CB_NINE_streamH));
        }
        if (MATH_CB_TEN_streamH) {
            ERR2(suites.StreamSuite2()->AEGP_DisposeStream(MATH_CB_TEN_streamH));
        }

		if (MATH_TOPIC_COLORS_streamH) {
			ERR2(suites.StreamSuite2()->AEGP_DisposeStream(MATH_TOPIC_COLORS_streamH));
		}
        if (MATH_COLOR_ONE_streamH) {
            ERR2(suites.StreamSuite2()->AEGP_DisposeStream(MATH_COLOR_ONE_streamH));
        }
        if (MATH_COLOR_TWO_streamH) {
            ERR2(suites.StreamSuite2()->AEGP_DisposeStream(MATH_COLOR_TWO_streamH));
        }
        if (MATH_COLOR_THREE_streamH) {
            ERR2(suites.StreamSuite2()->AEGP_DisposeStream(MATH_COLOR_THREE_streamH));
        }
        if (MATH_COLOR_FOUR_streamH) {
            ERR2(suites.StreamSuite2()->AEGP_DisposeStream(MATH_COLOR_FOUR_streamH));
        }
        if (MATH_COLOR_FIVE_streamH) {
            ERR2(suites.StreamSuite2()->AEGP_DisposeStream(MATH_COLOR_FIVE_streamH));
        }
        if (MATH_COLOR_SIX_streamH) {
            ERR2(suites.StreamSuite2()->AEGP_DisposeStream(MATH_COLOR_SIX_streamH));
        }
        if (MATH_COLOR_SEVEN_streamH) {
            ERR2(suites.StreamSuite2()->AEGP_DisposeStream(MATH_COLOR_SEVEN_streamH));
        }
        if (MATH_COLOR_HEIGHT_streamH) {
            ERR2(suites.StreamSuite2()->AEGP_DisposeStream(MATH_COLOR_HEIGHT_streamH));
        }
        if (MATH_COLOR_NINE_streamH) {
            ERR2(suites.StreamSuite2()->AEGP_DisposeStream(MATH_COLOR_NINE_streamH));
        }
        if (MATH_COLOR_TEN_streamH) {
            ERR2(suites.StreamSuite2()->AEGP_DisposeStream(MATH_COLOR_TEN_streamH));
        }

		if (MATH_TOPIC_INPUTS_streamH) {
			ERR2(suites.StreamSuite2()->AEGP_DisposeStream(MATH_TOPIC_INPUTS_streamH));
		}
        /*
        if (MATH_CEP_GET_ARB_DATA_streamH){
            ERR2(suites.StreamSuite2()->AEGP_DisposeStream(MATH_CEP_GET_ARB_DATA_streamH));
        }*/

		if (!err) {
			out_data->out_flags |= PF_OutFlag_FORCE_RERENDER;
		}

	}

	return err;
}

PF_Err
tlmath_UserChangedParam(
	PF_InData						*in_data,
	PF_OutData						*out_data,
	PF_ParamDef						*params[],
	PF_LayerDef						*outputP,
	const PF_UserChangedParamExtra	*which_hitP)
{
	PF_Err				err = PF_Err_NONE;
	AEGP_SuiteHandler    suites(in_data->pica_basicP);

	if (which_hitP->param_index == MATH_SETUP)
	{
		ERR(CallCepDialog(in_data, out_data));
		ERR(suites.AdvAppSuite2()->PF_RefreshAllWindows());
		//eval shader or math expr
		ERR(SetupDialogSend(in_data, out_data, params));
	}
	if (which_hitP->param_index == MATH_CEP_GET_ARB_DATA)
	{
		if (params[MATH_CEP_GET_ARB_DATA]->u.bd.value == TRUE) {

			ERR(SetupGetDataBack(in_data, out_data, params));

			//deselect checkbox
            params[MATH_CEP_GET_ARB_DATA]->u.bd.value = FALSE;
			ERR(suites.ParamUtilsSuite3()->PF_UpdateParamUI(in_data->effect_ref,
                                                                MATH_CEP_GET_ARB_DATA,
                                                                params[MATH_CEP_GET_ARB_DATA]));
            seqDataP seqP = reinterpret_cast<seqDataP>(DH(out_data->sequence_data));
            if (seqP->evalModeB){
                ERR(SetupDialogSend(in_data, out_data, params));
            }
		}

	}

	return err;
}
