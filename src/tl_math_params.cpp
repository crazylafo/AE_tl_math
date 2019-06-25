#include "tl_math.h"


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
	copy[MATH_SLIDER_ONE_VAR] = *actual[MATH_SLIDER_ONE_VAR];
	copy[MATH_SLIDER_TWO_VAR] = *actual[MATH_SLIDER_TWO_VAR];
	copy[MATH_SLIDER_THREE_VAR] = *actual[MATH_SLIDER_THREE_VAR];
	copy[MATH_SLIDER_FOUR_VAR] = *actual[MATH_SLIDER_FOUR_VAR];
	copy[MATH_TOPIC_POINTS] = *actual[MATH_TOPIC_POINTS];
	copy[MATH_POINT_ONE] = *actual[MATH_POINT_ONE];
	copy[MATH_POINT_TWO] = *actual[MATH_POINT_TWO];
	copy[MATH_TOPIC_COLORS] = *actual[MATH_TOPIC_COLORS];
	copy[MATH_COLOR_ONE] = *actual[MATH_COLOR_ONE];
	copy[MATH_COLOR_TWO] = *actual[MATH_COLOR_TWO];
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
		MATH_SLIDER_ONE_VAR_streamH = NULL,
		MATH_SLIDER_TWO_VAR_streamH = NULL,
		MATH_SLIDER_THREE_VAR_streamH = NULL,
		MATH_SLIDER_FOUR_VAR_streamH = NULL,
		MATH_TOPIC_POINTS_streamH = NULL,
		MATH_POINT_ONE_streamH = NULL,
		MATH_POINT_TWO_streamH = NULL,
		MATH_TOPIC_COLORS_streamH = NULL,
		MATH_COLOR_ONE_streamH = NULL,
		MATH_COLOR_TWO_streamH = NULL,
		MATH_TOPIC_INPUTS_streamH = NULL;


	if (seqP) {

		strcpy(param_copy[MATH_ARB_DATA].name, seqP->presetNameAc);
		ERR(suites.ParamUtilsSuite3()->PF_UpdateParamUI(in_data->effect_ref,
			MATH_ARB_DATA,
			&param_copy[MATH_ARB_DATA]));

		strcpy(param_copy[MATH_TOPIC_SLIDER].name, seqP->sliderGrpNameAc);
		ERR(suites.ParamUtilsSuite3()->PF_UpdateParamUI(in_data->effect_ref,
			MATH_TOPIC_SLIDER,
			&param_copy[MATH_TOPIC_SLIDER]));

		strcpy(param_copy[MATH_SLIDER_ONE_VAR].name, seqP->paramSlider01NameAc);
		ERR(suites.ParamUtilsSuite3()->PF_UpdateParamUI(in_data->effect_ref,
			MATH_SLIDER_ONE_VAR,
			&param_copy[MATH_SLIDER_ONE_VAR]));

		strcpy(param_copy[MATH_SLIDER_TWO_VAR].name, seqP->paramSlider02NameAc);
		ERR(suites.ParamUtilsSuite3()->PF_UpdateParamUI(in_data->effect_ref,
			MATH_SLIDER_TWO_VAR,
			&param_copy[MATH_SLIDER_TWO_VAR]));

		strcpy(param_copy[MATH_SLIDER_THREE_VAR].name, seqP->paramSlider03NameAc);
		ERR(suites.ParamUtilsSuite3()->PF_UpdateParamUI(in_data->effect_ref,
			MATH_SLIDER_THREE_VAR,
			&param_copy[MATH_SLIDER_THREE_VAR]));

		strcpy(param_copy[MATH_SLIDER_FOUR_VAR].name, seqP->paramSlider04NameAc);
		ERR(suites.ParamUtilsSuite3()->PF_UpdateParamUI(in_data->effect_ref,
			MATH_SLIDER_FOUR_VAR,
			&param_copy[MATH_SLIDER_FOUR_VAR]));


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


		strcpy(param_copy[MATH_TOPIC_INPUTS].name, seqP->layerGrpNameAc);
		ERR(suites.ParamUtilsSuite3()->PF_UpdateParamUI(in_data->effect_ref,
			MATH_TOPIC_INPUTS,
			&param_copy[MATH_TOPIC_INPUTS]));

		ERR(suites.PFInterfaceSuite1()->AEGP_GetNewEffectForEffect(globP->my_id, in_data->effect_ref, &meH));
		ERR(suites.StreamSuite2()->AEGP_GetNewEffectStreamByIndex(globP->my_id, meH, MATH_TOPIC_SLIDER, &MATH_TOPIC_SLIDER_streamH));
		ERR(suites.StreamSuite2()->AEGP_GetNewEffectStreamByIndex(globP->my_id, meH, MATH_SLIDER_ONE_VAR, &MATH_SLIDER_ONE_VAR_streamH));
		ERR(suites.StreamSuite2()->AEGP_GetNewEffectStreamByIndex(globP->my_id, meH, MATH_SLIDER_TWO_VAR, &MATH_SLIDER_TWO_VAR_streamH));
		ERR(suites.StreamSuite2()->AEGP_GetNewEffectStreamByIndex(globP->my_id, meH, MATH_SLIDER_THREE_VAR, &MATH_SLIDER_THREE_VAR_streamH));
		ERR(suites.StreamSuite2()->AEGP_GetNewEffectStreamByIndex(globP->my_id, meH, MATH_SLIDER_FOUR_VAR, &MATH_SLIDER_FOUR_VAR_streamH));
		ERR(suites.StreamSuite2()->AEGP_GetNewEffectStreamByIndex(globP->my_id, meH, MATH_TOPIC_POINTS, &MATH_TOPIC_POINTS_streamH));
		ERR(suites.StreamSuite2()->AEGP_GetNewEffectStreamByIndex(globP->my_id, meH, MATH_POINT_ONE, &MATH_POINT_ONE_streamH));
		ERR(suites.StreamSuite2()->AEGP_GetNewEffectStreamByIndex(globP->my_id, meH, MATH_POINT_TWO, &MATH_POINT_TWO_streamH));
		ERR(suites.StreamSuite2()->AEGP_GetNewEffectStreamByIndex(globP->my_id, meH, MATH_TOPIC_COLORS, &MATH_TOPIC_COLORS_streamH));
		ERR(suites.StreamSuite2()->AEGP_GetNewEffectStreamByIndex(globP->my_id, meH, MATH_COLOR_ONE, &MATH_COLOR_ONE_streamH));
		ERR(suites.StreamSuite2()->AEGP_GetNewEffectStreamByIndex(globP->my_id, meH, MATH_COLOR_TWO, &MATH_COLOR_TWO_streamH));
		ERR(suites.StreamSuite2()->AEGP_GetNewEffectStreamByIndex(globP->my_id, meH, MATH_TOPIC_INPUTS, &MATH_TOPIC_INPUTS_streamH));

x²		ERR(suites.DynamicStreamSuite2()->AEGP_SetDynamicStreamFlag(MATH_TOPIC_SLIDER_streamH, AEGP_DynStreamFlag_HIDDEN, FALSE, !seqP->sliderGrpVisibleB));
		ERR(suites.DynamicStreamSuite2()->AEGP_SetDynamicStreamFlag(MATH_SLIDER_ONE_VAR_streamH, AEGP_DynStreamFlag_HIDDEN, FALSE, !seqP->paramSlider01VisibleB));
		ERR(suites.DynamicStreamSuite2()->AEGP_SetDynamicStreamFlag(MATH_SLIDER_TWO_VAR_streamH, AEGP_DynStreamFlag_HIDDEN, FALSE, !seqP->paramSlider02VisibleB));
		ERR(suites.DynamicStreamSuite2()->AEGP_SetDynamicStreamFlag(MATH_SLIDER_THREE_VAR_streamH, AEGP_DynStreamFlag_HIDDEN, FALSE, !seqP->paramSlider03VisibleB));
		ERR(suites.DynamicStreamSuite2()->AEGP_SetDynamicStreamFlag(MATH_SLIDER_FOUR_VAR_streamH, AEGP_DynStreamFlag_HIDDEN, FALSE, !seqP->paramSlider04VisibleB));

		ERR(suites.DynamicStreamSuite2()->AEGP_SetDynamicStreamFlag(MATH_TOPIC_POINTS_streamH, AEGP_DynStreamFlag_HIDDEN, FALSE, !seqP->pointGrpVisibleB));
		ERR(suites.DynamicStreamSuite2()->AEGP_SetDynamicStreamFlag(MATH_POINT_ONE_streamH, AEGP_DynStreamFlag_HIDDEN, FALSE, !seqP->paramPoint01VisibleB));
		ERR(suites.DynamicStreamSuite2()->AEGP_SetDynamicStreamFlag(MATH_POINT_TWO_streamH, AEGP_DynStreamFlag_HIDDEN, FALSE, !seqP->paramPoint02VisibleB));

		ERR(suites.DynamicStreamSuite2()->AEGP_SetDynamicStreamFlag(MATH_TOPIC_COLORS_streamH, AEGP_DynStreamFlag_HIDDEN, FALSE, !seqP->colorGrpVisibleB));
		ERR(suites.DynamicStreamSuite2()->AEGP_SetDynamicStreamFlag(MATH_COLOR_ONE_streamH, AEGP_DynStreamFlag_HIDDEN, FALSE, !seqP->paramColor01VisibleB));
		ERR(suites.DynamicStreamSuite2()->AEGP_SetDynamicStreamFlag(MATH_COLOR_TWO_streamH, AEGP_DynStreamFlag_HIDDEN, FALSE, !seqP->paramColor02VisibleB));

		ERR(suites.DynamicStreamSuite2()->AEGP_SetDynamicStreamFlag(MATH_TOPIC_INPUTS_streamH, AEGP_DynStreamFlag_HIDDEN, FALSE, !seqP->layerGrpVisibleB));

		if (meH) {
			ERR2(suites.EffectSuite2()->AEGP_DisposeEffect(meH));
		}
		if (MATH_TOPIC_SLIDER_streamH) {
			ERR2(suites.StreamSuite2()->AEGP_DisposeStream(MATH_TOPIC_SLIDER_streamH));
		}
		if (MATH_SLIDER_ONE_VAR_streamH) {
			ERR2(suites.StreamSuite2()->AEGP_DisposeStream(MATH_SLIDER_ONE_VAR_streamH));
		}
		if (MATH_SLIDER_TWO_VAR_streamH) {
			ERR2(suites.StreamSuite2()->AEGP_DisposeStream(MATH_SLIDER_TWO_VAR_streamH));
		}
		if (MATH_SLIDER_THREE_VAR_streamH) {
			ERR2(suites.StreamSuite2()->AEGP_DisposeStream(MATH_SLIDER_THREE_VAR_streamH));
		}
		if (MATH_SLIDER_FOUR_VAR_streamH) {
			ERR2(suites.StreamSuite2()->AEGP_DisposeStream(MATH_SLIDER_FOUR_VAR_streamH));
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
		if (MATH_TOPIC_COLORS_streamH) {
			ERR2(suites.StreamSuite2()->AEGP_DisposeStream(MATH_TOPIC_COLORS_streamH));
		}
		if (MATH_COLOR_ONE_streamH) {
			ERR2(suites.StreamSuite2()->AEGP_DisposeStream(MATH_COLOR_ONE_streamH));
		}
		if (MATH_COLOR_TWO_streamH) {
			ERR2(suites.StreamSuite2()->AEGP_DisposeStream(MATH_COLOR_TWO_streamH));
		}
		if (MATH_TOPIC_INPUTS_streamH) {
			ERR2(suites.StreamSuite2()->AEGP_DisposeStream(MATH_TOPIC_INPUTS_streamH));
		}
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
		}

	}

	return err;
}