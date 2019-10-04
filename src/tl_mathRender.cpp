#include "tl_math.h"

static  void
copyPointsParam(PF_ParamDef  point_param,
                point_3d     *pt)
{

    //user param points
    pt->point[0] =  static_cast<PF_FpShort>(point_param.u.point3d_d.x_value);
    pt->point[1] =   static_cast<PF_FpShort>(point_param.u.point3d_d.y_value);
    pt->point[2] =  static_cast<PF_FpShort>(point_param.u.point3d_d.z_value);



}

PF_Err
tlmath::ExtLayerInput(void *refcon,
	PF_InData       *in_data,
	PF_EffectWorld *inputP,
	PF_EffectWorld *extLP,
	PF_EffectWorld *extLW,
	AEGP_SuiteHandler &suites,
	PF_PixelFormat   format)
{
	PF_Err err = PF_Err_NONE, err2 = PF_Err_NONE;
	OffInfo    *oiP = (OffInfo*)refcon;
	PF_Point            origin;
	PF_PixelFloat empty32 = { 0,0,0,0 };
	PF_Pixel16 empty16 = { 0,0,0,0 };
	PF_Pixel empty8 = { 0,0,0,0 };

	PF_EffectWorld Externalworld, ExternalWorldTransform;

	ERR(suites.WorldSuite1()->new_world(in_data->effect_ref, inputP->width, inputP->height, format, &Externalworld));
	ERR(suites.WorldSuite1()->new_world(in_data->effect_ref, inputP->width, inputP->height, format, &ExternalWorldTransform));
	//Externalworld.world_flags = extLW->world_flags;
	switch (format) {

	case PF_PixelFormat_ARGB128:
		ERR(suites.FillMatteSuite2()->fill_float(in_data->effect_ref,
			&empty32,
			NULL,
			&Externalworld));

		break;

	case PF_PixelFormat_ARGB64:
		ERR(suites.FillMatteSuite2()->fill16(in_data->effect_ref,
			&empty16,
			NULL,
			&Externalworld));


		break;

	case PF_PixelFormat_ARGB32:
		ERR(suites.FillMatteSuite2()->fill(in_data->effect_ref,
			&empty8,
			NULL,
			&Externalworld));
		break;

	default:
		err = PF_Err_INTERNAL_STRUCT_DAMAGED;
		break;
	}


	if (extLP && extLP->data) {
		if (PF_Quality_HI == in_data->quality) {
			ERR(suites.WorldTransformSuite1()->copy_hq(in_data->effect_ref,
				extLP,
				&Externalworld,
				&extLP->extent_hint,
				&extLP->extent_hint));
		}
		else {
			ERR(suites.WorldTransformSuite1()->copy(in_data->effect_ref,
				extLP,
				&Externalworld,
				&extLP->extent_hint,
				&extLP->extent_hint));
		}

	}
	if (!err && (oiP->x_offFi != 0 || oiP->y_offFi != 0)) {
		oiP->in_data = *in_data;
		oiP->samp_pb.src = &Externalworld;
		origin.h = (A_short)(in_data->output_origin_x);
		origin.v = (A_short)(in_data->output_origin_y);
		switch (format) {
		case PF_PixelFormat_ARGB128:
			ERR(suites.IterateFloatSuite1()->iterate_origin(in_data,
				0,
				Externalworld.height,
				&Externalworld,
				NULL,
				&origin,
				(void*)(oiP),
                ShiftImage32,
				&ExternalWorldTransform));
			break;

		case PF_PixelFormat_ARGB64:

			ERR(suites.Iterate16Suite1()->iterate_origin(in_data,
				0,
				Externalworld.height,
				&Externalworld,
				NULL,
				&origin,
				(void*)(oiP),
                ShiftImage16,
				&ExternalWorldTransform));
			break;

		case PF_PixelFormat_ARGB32:

			ERR(suites.Iterate8Suite1()->iterate_origin(in_data,
				0,
				Externalworld.height,
				&Externalworld,
				NULL,
				&origin,
				(void*)(oiP),
                ShiftImage8,
				&ExternalWorldTransform));

			break;

		default:
			err = PF_Err_INTERNAL_STRUCT_DAMAGED;
			break;
		}


		if (PF_Quality_HI == in_data->quality) {
			ERR(suites.WorldTransformSuite1()->copy_hq(in_data->effect_ref,
				&ExternalWorldTransform,
				extLW,
				NULL,
				NULL));
		}
		else {
			ERR(suites.WorldTransformSuite1()->copy(in_data->effect_ref,
				&ExternalWorldTransform,
				extLW,
				NULL,
				NULL));
		}
	}
	else {
		if (PF_Quality_HI == in_data->quality) {
			ERR(suites.WorldTransformSuite1()->copy_hq(in_data->effect_ref,
				&Externalworld,
				extLW,
				NULL,
				NULL));
		}
		else {
			ERR(suites.WorldTransformSuite1()->copy(in_data->effect_ref,
				&Externalworld,
				extLW,
				NULL,
				NULL));
		}
	}

	if (Externalworld.data) {
		ERR2(suites.WorldSuite1()->dispose_world(in_data->effect_ref, &Externalworld));
	}
	if (ExternalWorldTransform.data) {
		ERR2(suites.WorldSuite1()->dispose_world(in_data->effect_ref, &ExternalWorldTransform));
	}
	return err;
}

PF_Err
tlmath::ExprRender(PF_OutData     *out_data,
	PF_PixelFormat format,
	PF_EffectWorld *inputP,
	PF_EffectWorld *outputP,
	PF_EffectWorld *extLW,
	AEGP_SuiteHandler &suites,
	void    *refcon,
	void    *refconFlags,
	void    *refconExpr)
{
	PF_Err err = PF_Err_NONE;
	MathInfo           *miP = reinterpret_cast<MathInfo*>(refcon);
	MathInfo    miPP;
	AEFX_CLR_STRUCT(miPP); //new pointer wich can be modified in iterations
	miPP = *miP;
	FlagsInfoP         *flagsP = reinterpret_cast<FlagsInfo*>(refconFlags);
	FlagsInfoP flagsPP;
	AEFX_CLR_STRUCT(flagsPP);
	flagsPP = *flagsP;
	ExprInfoP          *exprP = reinterpret_cast<ExprInfoP*>(refconExpr);
	funcTransfertInfo fiP;
	AEFX_CLR_STRUCT(fiP);
	seqDataP            seqP = reinterpret_cast<seqDataP>(DH(out_data->sequence_data));


	WorldTransfertInfo   wtiP;
	AEFX_CLR_STRUCT(wtiP);
	wtiP.inW = *inputP;
	wtiP.outW = *outputP;
	if (flagsPP.PixelsCallExternalInputB) { //&extLW->data &&
		wtiP.extLW = *extLW;
	}

	std::string exprErrStr = "Error \n";
	PF_Boolean returnExprErrB = false;

    if (flagsP->exprRGBModeB){
        fiP.rgbExpr = parseExpr<PF_FpShort>((void*)&miPP, &fiP, *exprP->rgbstr, seqP);
        if (fiP.hasErrorB)
        {
            fiP.channelErrorstr = "RGB errors";
            returnExprErrB = true;
            exprErrStr.append(fiP.channelErrorstr).append(": ").append(fiP.errorstr).append("\n");
        }

    }
    else{
        fiP.redExpr = parseExpr<PF_FpShort>((void*)&miPP, &fiP, *exprP->redstr, seqP);
        if (fiP.hasErrorB)
        {
            fiP.channelErrorstr = "red channel expression";
            returnExprErrB = true;
            exprErrStr.append(fiP.channelErrorstr).append(": ").append(fiP.errorstr).append("\n");
        }
        fiP.greenExpr = parseExpr<PF_FpShort>((void*)&miPP, &fiP, *exprP->greenstr, seqP);
        if (fiP.hasErrorB)
        {
            fiP.channelErrorstr = "green channel expression";
            returnExprErrB = true;
            exprErrStr.append(fiP.channelErrorstr).append(": ").append(fiP.errorstr).append("\n");

        }
        fiP.blueExpr = parseExpr<PF_FpShort>((void*)&miPP, &fiP, *exprP->bluestr, seqP);
        if (fiP.hasErrorB)
        {
            fiP.channelErrorstr = "blue channel expression";
            returnExprErrB = true;
            exprErrStr.append(fiP.channelErrorstr).append(": ").append(fiP.errorstr).append("\n");

        }
    }
	fiP.alphaExpr = parseExpr<PF_FpShort>((void*)&miPP, &fiP, *exprP->alphastr, seqP);
	if (fiP.hasErrorB)
	{
		fiP.channelErrorstr = "alpha channel expression";
		returnExprErrB = true;
		exprErrStr.append(fiP.channelErrorstr).append(": ").append(fiP.errorstr).append("\n");
	}

	if (returnExprErrB) {
		suites.ANSICallbacksSuite1()->sprintf(out_data->return_msg,
			exprErrStr.c_str());
	}

	std::vector<std::thread> workers_thrds;
	A_long part_length, lastPart_length, num_thrd;
	AEFX_CLR_STRUCT(part_length);
	AEFX_CLR_STRUCT(num_thrd);
	AEFX_CLR_STRUCT(lastPart_length);
	ERR(suites.IterateSuite1()->AEGP_GetNumThreads(&num_thrd));
	part_length = A_long((outputP->height / (float)num_thrd));
	lastPart_length = part_length + (outputP->height - (part_length*num_thrd));
	thSafeExpr_render* thRenderPtr = new thSafeExpr_render();
	switch (format) {

	case PF_PixelFormat_ARGB128:
		AEFX_CLR_STRUCT(workers_thrds);
		for (A_long thrd_id = 0; thrd_id < num_thrd; ++thrd_id) {
			workers_thrds.emplace_back(std::thread(&thSafeExpr_render::render_32,
				thRenderPtr,
				(void*)&miPP,
				(void*)&fiP,
				(void*)&flagsPP,
				(void*)&wtiP,
				thrd_id,
				num_thrd,
				part_length,
				lastPart_length));
		}
		for (auto& t : workers_thrds) {
			t.join();
		}
		delete thRenderPtr;
		outputP = &wtiP.outW;
		break;

	case PF_PixelFormat_ARGB64:
		AEFX_CLR_STRUCT(workers_thrds);
		for (A_long thrd_id = 0; thrd_id < num_thrd; ++thrd_id) {
			workers_thrds.emplace_back(std::thread(&thSafeExpr_render::render_16,
				thRenderPtr,
				(void*)&miPP,
				(void*)&fiP,
				(void*)&flagsPP,
				(void*)&wtiP,
				thrd_id,
				num_thrd,
				part_length,
				lastPart_length));
		}
		for (auto& t : workers_thrds) {
			t.join();
		}
		delete thRenderPtr;
		outputP = &wtiP.outW;
		break;

	case PF_PixelFormat_ARGB32:
		AEFX_CLR_STRUCT(workers_thrds);
		for (A_long thrd_id = 0; thrd_id < num_thrd; ++thrd_id) {
			workers_thrds.emplace_back(std::thread(&thSafeExpr_render::render_8,
				thRenderPtr,
				(void*)&miPP,
				(void*)&fiP,
				(void*)&flagsPP,
				(void*)&wtiP,
				thrd_id,
				num_thrd,
				part_length,
				lastPart_length));
		}
		for (auto& t : workers_thrds) {
			t.join();
		}
		delete thRenderPtr;
		outputP = &wtiP.outW;

		break;

	default:
		err = PF_Err_INTERNAL_STRUCT_DAMAGED;
		break;
	}
	return err;
}


PF_Err
tlmath::PreRender(PF_InData                *in_data,
	PF_OutData                *out_data,
	PF_PreRenderExtra        *extraP)
{
	PF_Err    err = PF_Err_NONE;

	PF_RenderRequest req = extraP->input->output_request;

	PF_CheckoutResult        in_result, extL_result;
	AEGP_SuiteHandler        suites(in_data->pica_basicP);
	PF_Handle   infoH = suites.HandleSuite1()->host_new_handle(sizeof(MathInfoP));
	AEGP_LayerH        layerH;
	AEGP_CompH        compH;
	AEGP_ItemH      itemH;
	MathInfo        * miP = NULL;
    PF_Boolean     cameraModeB = false;

	if (infoH) {



		miP = reinterpret_cast< MathInfo*>(suites.HandleSuite1()->host_lock_handle(infoH));
		if (miP) {
			extraP->output->pre_render_data = infoH;

			PF_ParamDef  arb_param;
			AEFX_CLR_STRUCT(arb_param);
			ERR(PF_CHECKOUT_PARAM(in_data,
				MATH_ARB_DATA,
				in_data->current_time,
				in_data->time_step,
				in_data->time_scale,
				&arb_param));


			PF_ParamDef extlayer_toff_param;
			AEFX_CLR_STRUCT(extlayer_toff_param);
			ERR(PF_CHECKOUT_PARAM(in_data,
				MATH_INP_TOFF_ONE,
				in_data->current_time,
				in_data->time_step,
				in_data->time_scale,
				&extlayer_toff_param));
			PF_ParamDef extlayer_poff_param;
			AEFX_CLR_STRUCT(extlayer_poff_param);
			ERR(PF_CHECKOUT_PARAM(in_data,
				MATH_INP_POFF_ONE,
				in_data->current_time,
				in_data->time_step,
				in_data->time_scale,
				&extlayer_poff_param));

            PF_Handle    seq_dataH = suites.HandleSuite1()->host_new_handle(sizeof(seqData));
            PF_Boolean   initB = true;

            if (seq_dataH) {
                seqData      *seqP = reinterpret_cast<seqData*>(suites.HandleSuite1()->host_lock_handle(seq_dataH));
                if (seqP->initializedB == false){
                    initB = false;
                    m_ArbData *arbOutP = reinterpret_cast<m_ArbData*>(*arb_param.u.arb_d.value);
                    ERR(tlmath::copyFromArbToSeqData( in_data, out_data, arbOutP->arbDataAc , seqP));
                    seqP->initializedB = true;
                    out_data->sequence_data = seq_dataH;
                }
                suites.HandleSuite1()->host_unlock_handle(seq_dataH);
                if (!initB){
                      ERR(tlmath::evalScripts  (seqP));
                }
                if (seqP->cameraB ==true){
                    cameraModeB = true;
                }
            }


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
			miP->compSizeF[0] = PF_FpLong(width);
			miP->compSizeF[1] = PF_FpLong(height);
			AEGP_DownsampleFactor dsp;
			compSuite->AEGP_GetCompDownsampleFactor(compH, &dsp);
			miP->compSizeF[0] *= dsp.xS;
			miP->compSizeF[1] *= dsp.yS;
			PF_FpLong fpsF;
			compSuite->AEGP_GetCompFramerate(compH, &fpsF);
			miP->compFpsF = static_cast<float>(fpsF);

			layerSuite->AEGP_GetLayerCurrentTime(layerH, AEGP_LTimeMode_LayerTime, &currTime);
			StreamSuite->AEGP_GetLayerStreamValue(layerH, AEGP_LayerStream_POSITION, AEGP_LTimeMode_LayerTime, &currTime, NULL, &strValP, &strTypeP);

			miP->layerPos.point[0] = strValP.three_d.x;
            miP->layerPos.point[1] = strValP.three_d.y;
            miP->layerPos.point[2] = strValP.three_d.z;

			StreamSuite->AEGP_GetLayerStreamValue(layerH, AEGP_LayerStream_SCALE, AEGP_LTimeMode_LayerTime, &currTime, NULL, &strValSP, &strTypeP);
			miP->layerScale.point[0] = strValSP.three_d.x;
			miP->layerScale.point[1] = strValSP.three_d.z;
			miP->layerScale.point[2] = strValSP.three_d.z;

            if (cameraModeB){
                A_Time                comp_timeT        =    {0,1};
                AEGP_LayerH camera_layerH    =    NULL;
                ERR(suites.PFInterfaceSuite1()->AEGP_ConvertEffectToCompTime(in_data->effect_ref,
                                                                             in_data->current_time,
                                                                             in_data->time_scale,
                                                                             &comp_timeT));

                ERR(suites.PFInterfaceSuite1()->AEGP_GetEffectCamera(in_data->effect_ref,
                                                                     &comp_timeT,
                                                                     &camera_layerH));

                    if (!err && camera_layerH){
                        AEGP_StreamVal    stream_valZoom, stream_valPos, stream_valTarg, stream_valRot;
                        AEFX_CLR_STRUCT(stream_valZoom);
                        // camera zoom
                        ERR(suites.StreamSuite2()->AEGP_GetLayerStreamValue(     camera_layerH,
                                                                            AEGP_LayerStream_ZOOM,
                                                                            AEGP_LTimeMode_CompTime,
                                                                            &comp_timeT,
                                                                            FALSE,
                                                                            &stream_valZoom,
                                                                            NULL));
                        //camera position
                        AEFX_CLR_STRUCT(stream_valPos);
                        ERR(suites.StreamSuite2()->AEGP_GetLayerStreamValue(     camera_layerH,
                                                                            AEGP_LayerStream_POSITION ,
                                                                            AEGP_LTimeMode_CompTime,
                                                                            &comp_timeT,
                                                                            FALSE,
                                                                            &stream_valPos,
                                                                            NULL));

                        //camera target
                        AEFX_CLR_STRUCT(stream_valTarg);
                        ERR(suites.StreamSuite2()->AEGP_GetLayerStreamValue(     camera_layerH,
                                                                            AEGP_LayerStream_ANCHORPOINT,
                                                                            AEGP_LTimeMode_CompTime,
                                                                            &comp_timeT,
                                                                            FALSE,
                                                                            &stream_valTarg,
                                                                            NULL));
                        //camera rotation
                        AEFX_CLR_STRUCT(stream_valRot);
                        ERR(suites.StreamSuite2()->AEGP_GetLayerStreamValue(     camera_layerH,
                                                                            AEGP_LayerStream_ROTATION,
                                                                            AEGP_LTimeMode_CompTime,
                                                                            &comp_timeT,
                                                                            FALSE,
                                                                            &stream_valRot,
                                                                            NULL));



                        if (!err) {
                            miP->cameraZoom = stream_valZoom.one_d;

                            miP->cameraPos.point[0] = stream_valPos.three_d.x;
                            miP->cameraPos.point[1] = stream_valPos.three_d.y;
                            miP->cameraPos.point[2] = stream_valPos.three_d.z;

                            miP->cameraTarget.point[0] =stream_valTarg.three_d.x;
                            miP->cameraTarget.point[1] =stream_valTarg.three_d.y;
                            miP->cameraTarget.point[2] = stream_valRot.three_d.z;

                            miP->cameraRotation.point[0] =stream_valRot.three_d.x;
                            miP->cameraRotation.point[1] =stream_valRot.three_d.y;
                            miP->cameraRotation.point[2] = stream_valRot.three_d.z;

                        }
                    }
                }

			AEFX_CLR_STRUCT(in_result);
			AEFX_CLR_STRUCT(extL_result);

			if (!err) {
				req.preserve_rgb_of_zero_alpha = FALSE;
				ERR(extraP->cb->checkout_layer(in_data->effect_ref,
					MATH_INPUT,
					MATH_INPUT,
					&req,
					in_data->current_time,
					in_data->time_step,
					in_data->time_scale,
					&in_result));






				if (!err) {

					UnionLRect(&in_result.result_rect, &extraP->output->result_rect);
					UnionLRect(&in_result.max_result_rect, &extraP->output->max_result_rect);


					ERR(extraP->cb->checkout_layer(in_data->effect_ref,
						MATH_INP_LAYER_ONE,
						MATH_INP_LAYER_ONE,
						&req,
						(in_data->current_time + A_long(extlayer_toff_param.u.fs_d.value) * in_data->time_step),
						in_data->time_step,
						in_data->time_scale,
						&extL_result));


					PF_Fixed     widthFi = INT2FIX(ABS(extL_result.max_result_rect.right - extL_result.max_result_rect.left)),
						heightFi = INT2FIX(ABS(extL_result.max_result_rect.bottom - extL_result.max_result_rect.top));

					miP->x_offFi = PF_Fixed(widthFi / 2 - extlayer_poff_param.u.td.x_value);
					miP->y_offFi = PF_Fixed(heightFi / 2 - extlayer_poff_param.u.td.y_value);



				}
			}
			ERR(PF_CHECKIN_PARAM(in_data, &arb_param));
			suites.HandleSuite1()->host_unlock_handle(infoH);
		}
	}
	return err;
}

PF_Err
tlmath::SmartRender(
	PF_InData                *in_data,
	PF_OutData                *out_data,
	PF_SmartRenderExtra        *extraP)
{

	PF_Err            err = PF_Err_NONE,
		err2 = PF_Err_NONE;

	PF_EffectWorld  *inputP = NULL,
		*outputP = NULL,
		*extLP = NULL;
	PF_PixelFormat    format = PF_PixelFormat_INVALID;
	AEGP_SuiteHandler suites(in_data->pica_basicP);
	seqDataP            seqP = reinterpret_cast<seqDataP>(DH(out_data->sequence_data));
	AEFX_SuiteScoper<PF_WorldSuite2> wsP = AEFX_SuiteScoper<PF_WorldSuite2>(in_data,
		kPFWorldSuite,
		kPFWorldSuiteVersion2,
		out_data);

	if (!err && seqP) {

		MathInfo *miP = reinterpret_cast< MathInfo*>(suites.HandleSuite1()->host_lock_handle(reinterpret_cast<PF_Handle>(extraP->input->pre_render_data)));
		//flagInfo
		if (miP) {
			OffInfo         oiP;
			AEFX_CLR_STRUCT(oiP);
			FlagsInfo      flagsP;
			AEFX_CLR_STRUCT(flagsP);
			PF_Handle arbH = NULL;
			PF_EffectWorld extLW;
			ExprInfoP       ExprP;
			AEFX_CLR_STRUCT(ExprP);



			// checkout input & output buffers.
			ERR((extraP->cb->checkout_layer_pixels(in_data->effect_ref, MATH_INPUT, &inputP)));
			ERR((extraP->cb->checkout_layer_pixels(in_data->effect_ref, MATH_INP_LAYER_ONE, &extLP)));

			ERR(extraP->cb->checkout_output(in_data->effect_ref, &outputP));

			// determine requested output depth
			ERR(wsP->PF_GetPixelFormat(outputP, &format));
			ERR(wsP->PF_NewWorld(in_data->effect_ref, inputP->width, inputP->height, FALSE, format, &extLW));


			//CHECKOUT PARAMS
			PF_ParamDef  setup_param,
                description_param,
				arb_param,
				slider_param[10],
				point_param[10],
				cb_param[10],
				color_param[10],
				rot_param[10],
				cb_getarb_param;


			AEFX_CLR_STRUCT(setup_param);
			ERR(PF_CHECKOUT_PARAM(in_data,
				MATH_SETUP,
				in_data->current_time,
				in_data->time_step,
				in_data->time_scale,
				&setup_param));

            AEFX_CLR_STRUCT(description_param);
            ERR(PF_CHECKOUT_PARAM(in_data,
                  MATH_SETUP,
                  in_data->current_time,
                  in_data->time_step,
                  in_data->time_scale,
                  &description_param));

			AEFX_CLR_STRUCT(arb_param);
			ERR(PF_CHECKOUT_PARAM(in_data,
				MATH_ARB_DATA,
				in_data->current_time,
				in_data->time_step,
				in_data->time_scale,
				&arb_param));

			AEFX_CLR_STRUCT(slider_param[0]);
			ERR(PF_CHECKOUT_PARAM(in_data,
				MATH_SLIDER_ONE,
				in_data->current_time,
				in_data->time_step,
				in_data->time_scale,
				&slider_param[0]));

			AEFX_CLR_STRUCT(slider_param[1]);
			ERR(PF_CHECKOUT_PARAM(in_data,
				MATH_SLIDER_TWO,
				in_data->current_time,
				in_data->time_step,
				in_data->time_scale,
				&slider_param[1]));

			AEFX_CLR_STRUCT(slider_param[2]);
			ERR(PF_CHECKOUT_PARAM(in_data,
				MATH_SLIDER_THREE,
				in_data->current_time,
				in_data->time_step,
				in_data->time_scale,
				&slider_param[2]));

			AEFX_CLR_STRUCT(slider_param[3]);
			ERR(PF_CHECKOUT_PARAM(in_data,
				MATH_SLIDER_FOUR,
				in_data->current_time,
				in_data->time_step,
				in_data->time_scale,
				&slider_param[3]));

			AEFX_CLR_STRUCT(slider_param[4]);
			ERR(PF_CHECKOUT_PARAM(in_data,
				MATH_SLIDER_FIVE,
				in_data->current_time,
				in_data->time_step,
				in_data->time_scale,
				&slider_param[4]));

			AEFX_CLR_STRUCT(slider_param[5]);
			ERR(PF_CHECKOUT_PARAM(in_data,
				MATH_SLIDER_SIX,
				in_data->current_time,
				in_data->time_step,
				in_data->time_scale,
				&slider_param[5]));

			AEFX_CLR_STRUCT(slider_param[6]);
			ERR(PF_CHECKOUT_PARAM(in_data,
				MATH_SLIDER_SEVEN,
				in_data->current_time,
				in_data->time_step,
				in_data->time_scale,
				&slider_param[6]));

			AEFX_CLR_STRUCT(slider_param[7]);
			ERR(PF_CHECKOUT_PARAM(in_data,
				MATH_SLIDER_HEIGHT,
				in_data->current_time,
				in_data->time_step,
				in_data->time_scale,
				&slider_param[7]));

			AEFX_CLR_STRUCT(slider_param[8]);
			ERR(PF_CHECKOUT_PARAM(in_data,
				MATH_SLIDER_NINE,
				in_data->current_time,
				in_data->time_step,
				in_data->time_scale,
				&slider_param[8]));

			AEFX_CLR_STRUCT(slider_param[9]);
			ERR(PF_CHECKOUT_PARAM(in_data,
				MATH_SLIDER_TEN,
				in_data->current_time,
				in_data->time_step,
				in_data->time_scale,
				&slider_param[9]));




			AEFX_CLR_STRUCT(point_param[0]);
			ERR(PF_CHECKOUT_PARAM(in_data,
				MATH_POINT_ONE,
				in_data->current_time,
				in_data->time_step,
				in_data->time_scale,
				&point_param[0]));

			AEFX_CLR_STRUCT(point_param[1]);
			ERR(PF_CHECKOUT_PARAM(in_data,
				MATH_POINT_TWO,
				in_data->current_time,
				in_data->time_step,
				in_data->time_scale,
				&point_param[1]));

			AEFX_CLR_STRUCT(point_param[2]);
			ERR(PF_CHECKOUT_PARAM(in_data,
				MATH_POINT_THREE,
				in_data->current_time,
				in_data->time_step,
				in_data->time_scale,
				&point_param[2]));

			AEFX_CLR_STRUCT(point_param[3]);
			ERR(PF_CHECKOUT_PARAM(in_data,
				MATH_POINT_FOUR,
				in_data->current_time,
				in_data->time_step,
				in_data->time_scale,
				&point_param[3]));

			AEFX_CLR_STRUCT(point_param[4]);
			ERR(PF_CHECKOUT_PARAM(in_data,
				MATH_POINT_FIVE,
				in_data->current_time,
				in_data->time_step,
				in_data->time_scale,
				&point_param[4]));

			AEFX_CLR_STRUCT(point_param[5]);
			ERR(PF_CHECKOUT_PARAM(in_data,
				MATH_POINT_SIX,
				in_data->current_time,
				in_data->time_step,
				in_data->time_scale,
				&point_param[5]));

			AEFX_CLR_STRUCT(point_param[6]);
			ERR(PF_CHECKOUT_PARAM(in_data,
				MATH_POINT_SEVEN,
				in_data->current_time,
				in_data->time_step,
				in_data->time_scale,
				&point_param[6]));

			AEFX_CLR_STRUCT(point_param[7]);
			ERR(PF_CHECKOUT_PARAM(in_data,
				MATH_POINT_HEIGHT,
				in_data->current_time,
				in_data->time_step,
				in_data->time_scale,
				&point_param[7]));

			AEFX_CLR_STRUCT(point_param[8]);
			ERR(PF_CHECKOUT_PARAM(in_data,
				MATH_POINT_NINE,
				in_data->current_time,
				in_data->time_step,
				in_data->time_scale,
				&point_param[8]));

			AEFX_CLR_STRUCT(point_param[9]);
			ERR(PF_CHECKOUT_PARAM(in_data,
				MATH_POINT_TEN,
				in_data->current_time,
				in_data->time_step,
				in_data->time_scale,
				&point_param[9]));



			AEFX_CLR_STRUCT(cb_param[0]);
			ERR(PF_CHECKOUT_PARAM(in_data,
				MATH_CB_ONE,
				in_data->current_time,
				in_data->time_step,
				in_data->time_scale,
				&cb_param[0]));

			AEFX_CLR_STRUCT(cb_param[1]);
			ERR(PF_CHECKOUT_PARAM(in_data,
				MATH_CB_TWO,
				in_data->current_time,
				in_data->time_step,
				in_data->time_scale,
				&cb_param[1]));

			AEFX_CLR_STRUCT(cb_param[2]);
			ERR(PF_CHECKOUT_PARAM(in_data,
				MATH_CB_THREE,
				in_data->current_time,
				in_data->time_step,
				in_data->time_scale,
				&cb_param[2]));

			AEFX_CLR_STRUCT(cb_param[3]);
			ERR(PF_CHECKOUT_PARAM(in_data,
				MATH_CB_FOUR,
				in_data->current_time,
				in_data->time_step,
				in_data->time_scale,
				&cb_param[3]));

			AEFX_CLR_STRUCT(cb_param[4]);
			ERR(PF_CHECKOUT_PARAM(in_data,
				MATH_CB_FIVE,
				in_data->current_time,
				in_data->time_step,
				in_data->time_scale,
				&cb_param[4]));

			AEFX_CLR_STRUCT(cb_param[5]);
			ERR(PF_CHECKOUT_PARAM(in_data,
				MATH_CB_SIX,
				in_data->current_time,
				in_data->time_step,
				in_data->time_scale,
				&cb_param[5]));

			AEFX_CLR_STRUCT(cb_param[6]);
			ERR(PF_CHECKOUT_PARAM(in_data,
				MATH_CB_SEVEN,
				in_data->current_time,
				in_data->time_step,
				in_data->time_scale,
				&cb_param[6]));

			AEFX_CLR_STRUCT(cb_param[7]);
			ERR(PF_CHECKOUT_PARAM(in_data,
				MATH_CB_HEIGHT,
				in_data->current_time,
				in_data->time_step,
				in_data->time_scale,
				&cb_param[7]));

			AEFX_CLR_STRUCT(cb_param[8]);
			ERR(PF_CHECKOUT_PARAM(in_data,
				MATH_CB_NINE,
				in_data->current_time,
				in_data->time_step,
				in_data->time_scale,
				&cb_param[8]));

			AEFX_CLR_STRUCT(cb_param[9]);
			ERR(PF_CHECKOUT_PARAM(in_data,
				MATH_CB_TEN,
				in_data->current_time,
				in_data->time_step,
				in_data->time_scale,
				&cb_param[9]));


			AEFX_CLR_STRUCT(color_param[0]);
			ERR(PF_CHECKOUT_PARAM(in_data,
				MATH_COLOR_ONE,
				in_data->current_time,
				in_data->time_step,
				in_data->time_scale,
				&color_param[0]));

			AEFX_CLR_STRUCT(color_param[1]);
			ERR(PF_CHECKOUT_PARAM(in_data,
				MATH_COLOR_TWO,
				in_data->current_time,
				in_data->time_step,
				in_data->time_scale,
				&color_param[1]));

			AEFX_CLR_STRUCT(color_param[2]);
			ERR(PF_CHECKOUT_PARAM(in_data,
				MATH_COLOR_THREE,
				in_data->current_time,
				in_data->time_step,
				in_data->time_scale,
				&color_param[2]));

			AEFX_CLR_STRUCT(color_param[3]);
			ERR(PF_CHECKOUT_PARAM(in_data,
				MATH_COLOR_FOUR,
				in_data->current_time,
				in_data->time_step,
				in_data->time_scale,
				&color_param[3]));

			AEFX_CLR_STRUCT(color_param[4]);
			ERR(PF_CHECKOUT_PARAM(in_data,
				MATH_COLOR_FIVE,
				in_data->current_time,
				in_data->time_step,
				in_data->time_scale,
				&color_param[4]));

			AEFX_CLR_STRUCT(color_param[5]);
			ERR(PF_CHECKOUT_PARAM(in_data,
				MATH_COLOR_SIX,
				in_data->current_time,
				in_data->time_step,
				in_data->time_scale,
				&color_param[5]));

			AEFX_CLR_STRUCT(color_param[6]);
			ERR(PF_CHECKOUT_PARAM(in_data,
				MATH_COLOR_SEVEN,
				in_data->current_time,
				in_data->time_step,
				in_data->time_scale,
				&color_param[6]));

			AEFX_CLR_STRUCT(color_param[7]);
			ERR(PF_CHECKOUT_PARAM(in_data,
				MATH_COLOR_HEIGHT,
				in_data->current_time,
				in_data->time_step,
				in_data->time_scale,
				&color_param[7]));

			AEFX_CLR_STRUCT(color_param[8]);
			ERR(PF_CHECKOUT_PARAM(in_data,
				MATH_COLOR_NINE,
				in_data->current_time,
				in_data->time_step,
				in_data->time_scale,
				&color_param[8]));

			AEFX_CLR_STRUCT(color_param[9]);
			ERR(PF_CHECKOUT_PARAM(in_data,
				MATH_COLOR_TEN,
				in_data->current_time,
				in_data->time_step,
				in_data->time_scale,
				&color_param[9]));

			AEFX_CLR_STRUCT(rot_param[0]);
			ERR(PF_CHECKOUT_PARAM(in_data,
				MATH_ROT_ONE,
				in_data->current_time,
				in_data->time_step,
				in_data->time_scale,
				&rot_param[0]));

			AEFX_CLR_STRUCT(rot_param[1]);
			ERR(PF_CHECKOUT_PARAM(in_data,
				MATH_ROT_TWO,
				in_data->current_time,
				in_data->time_step,
				in_data->time_scale,
				&rot_param[1]));

			AEFX_CLR_STRUCT(rot_param[2]);
			ERR(PF_CHECKOUT_PARAM(in_data,
				MATH_ROT_THREE,
				in_data->current_time,
				in_data->time_step,
				in_data->time_scale,
				&rot_param[2]));

			AEFX_CLR_STRUCT(rot_param[3]);
			ERR(PF_CHECKOUT_PARAM(in_data,
				MATH_ROT_FOUR,
				in_data->current_time,
				in_data->time_step,
				in_data->time_scale,
				&rot_param[3]));

			AEFX_CLR_STRUCT(rot_param[4]);
			ERR(PF_CHECKOUT_PARAM(in_data,
				MATH_ROT_FIVE,
				in_data->current_time,
				in_data->time_step,
				in_data->time_scale,
				&rot_param[4]));

			AEFX_CLR_STRUCT(rot_param[5]);
			ERR(PF_CHECKOUT_PARAM(in_data,
				MATH_ROT_SIX,
				in_data->current_time,
				in_data->time_step,
				in_data->time_scale,
				&rot_param[5]));

			AEFX_CLR_STRUCT(rot_param[6]);
			ERR(PF_CHECKOUT_PARAM(in_data,
				MATH_ROT_SEVEN,
				in_data->current_time,
				in_data->time_step,
				in_data->time_scale,
				&rot_param[6]));

			AEFX_CLR_STRUCT(rot_param[7]);
			ERR(PF_CHECKOUT_PARAM(in_data,
				MATH_ROT_HEIGHT,
				in_data->current_time,
				in_data->time_step,
				in_data->time_scale,
				&rot_param[7]));

			AEFX_CLR_STRUCT(rot_param[8]);
			ERR(PF_CHECKOUT_PARAM(in_data,
				MATH_ROT_NINE,
				in_data->current_time,
				in_data->time_step,
				in_data->time_scale,
				&rot_param[8]));

			AEFX_CLR_STRUCT(rot_param[9]);
			ERR(PF_CHECKOUT_PARAM(in_data,
				MATH_ROT_TEN,
				in_data->current_time,
				in_data->time_step,
				in_data->time_scale,
				&rot_param[9]));


			AEFX_CLR_STRUCT(cb_getarb_param);
			ERR(PF_CHECKOUT_PARAM(in_data,
				MATH_CEP_GET_ARB_DATA,
				in_data->current_time,
				in_data->time_step,
				in_data->time_scale,
				&cb_getarb_param));


			//layer size
			miP->scale_x = in_data->downsample_x.num*in_data->pixel_aspect_ratio.num / (float)in_data->downsample_x.den;
			miP->scale_y = in_data->downsample_y.num*in_data->pixel_aspect_ratio.den / (float)in_data->downsample_y.den;
			miP->layerSizeF[0]= PF_FpShort(in_data->width*miP->scale_x);
			miP->layerSizeF[1] = PF_FpShort(in_data->height* miP->scale_y);
			//time params
			miP->layerTime_Sec = PF_FpShort(in_data->current_time) / PF_FpShort(in_data->time_scale);
			miP->layerTime_Frame = PF_FpShort(in_data->current_time / (float)in_data->time_step);
			miP->layerDuration = PF_FpShort(in_data->total_time / in_data->time_scale);
            for (int i =0; i<10; i++){
                miP->inSliderF[i] = slider_param[i].u.fs_d.value;
                }
            //user param points
            for (int i =0; i<10; i++){
                copyPointsParam(point_param[i], &miP->inPoints[i]);
                }
             for (int i =0; i<10; i++){
                 miP->inCboxF[i] = PF_FpShort (cb_param[i].u.bd.value);
                }
			 for (int i = 0; i < 10; i++) {
				 miP->inRotF[i] = PF_FpShort(rot_param[i].u.ad.value);
			 }
			//CONVERT COLOR PARAMS TO FLOAT BYSMART WAY
             PF_PixelFloat tempFloat;
             for (int i =0; i<10; i++){
                 AEFX_CLR_STRUCT(tempFloat)
                 ERR(suites.ColorParamSuite1()->PF_GetFloatingPointColorFromColorDef(in_data->effect_ref, &color_param[i], &tempFloat));
                 //user param color
                 miP->inColors[i].color[0] =tempFloat.red;
                 miP->inColors[i].color[1] =tempFloat.green;
                 miP->inColors[i].color[2] =tempFloat.blue;
                }
            miP->pixF[0] = 0;
            miP->pixF[1] = 0;
			//CALL SEQP
			std::string redExprStr, greenExprStr, blueExprStr, alphaExprStr, frag1Str, vertStr, rgbstr;
			if (seqP && !err) {
				redExprStr = seqP->redExAc;
				greenExprStr = seqP->greenExAc;
				blueExprStr = seqP->blueExAc;
				alphaExprStr = seqP->alphaExAc;
                rgbstr =  seqP->rgbExprExAc;
				frag1Str = seqP->Glsl33_FragmentShAc;
				vertStr = seqP->Glsl33_VertexShAc;


				flagsP.PixelsCallExternalInputB = seqP->pixelsCallExternalInputB;
				flagsP.PresetHasWideInput = seqP->presetHasWideInputB;
				flagsP.NeedsPixelAroundB = seqP->needsPixelAroundB;
				flagsP.NeedsLumaB = seqP->needsLumaB;
				flagsP.parserModeB = seqP->exprModeB;
                flagsP.exprRGBModeB = seqP->exprRGBModeB;
			}

			ExprP.redstr = &redExprStr;
			ExprP.greenstr = &greenExprStr;
			ExprP.bluestr = &blueExprStr;
			ExprP.alphastr = &alphaExprStr;
			ExprP.frag1str = &frag1Str;
			ExprP.vertexstr = &vertStr;
			ExprP.frag2str = &glfrag2str;
            ExprP.rgbstr = &rgbstr;
			//CALL EXTERNAL LAYER AND TRANSFORM WORLD IF NEEDED

			if (flagsP.PixelsCallExternalInputB) {
				oiP.x_offFi = miP->x_offFi;
				oiP.y_offFi = miP->y_offFi;

				ERR(tlmath::ExtLayerInput((void*)&oiP,
					in_data,
					inputP,
					extLP,
					&extLW,
					suites,
					format));
			}

			//CALL PARSER MODE
			if (!err && flagsP.parserModeB) {
				ERR(tlmath::ExprRender(out_data, format, inputP, outputP, &extLW, suites,
					(void*)miP,
					(void*)&flagsP,
					(void*)&ExprP));

			}
			// CALL GLSL
			else if (!err) {
                ERR(tlmath::Render_GLSL(in_data,
					out_data,
					inputP,
					outputP,
					&extLW,
					format,
					suites,
					(void*)miP,
					seqP->resetShaderB,
					*ExprP.vertexstr,
					*ExprP.frag1str,
					*ExprP.frag2str));
			}

			//CHECKIN PARAMS
			PF_UNLOCK_HANDLE(arbH);
			ERR2(PF_CHECKIN_PARAM(in_data, &setup_param));
            ERR2(PF_CHECKIN_PARAM(in_data, &description_param));
			ERR2(PF_CHECKIN_PARAM(in_data, &arb_param));
			for (int i = 0; i<10; i++) {
				ERR2(PF_CHECKIN_PARAM(in_data, &slider_param[i]));
				ERR2(PF_CHECKIN_PARAM(in_data, &point_param[i]));
				ERR2(PF_CHECKIN_PARAM(in_data, &cb_param[i]));
				ERR2(PF_CHECKIN_PARAM(in_data, &color_param[i]));
				ERR2(PF_CHECKIN_PARAM(in_data, &rot_param[i]));
			}

			ERR2(PF_CHECKIN_PARAM(in_data, &cb_getarb_param));

			if (extLW.data) {
				ERR2(wsP->PF_DisposeWorld(in_data->effect_ref, &extLW));
			}
			ERR2(extraP->cb->checkin_layer_pixels(in_data->effect_ref, MATH_INPUT));
			ERR2(extraP->cb->checkin_layer_pixels(in_data->effect_ref, MATH_INP_LAYER_ONE));
		}

		suites.HandleSuite1()->host_unlock_handle(reinterpret_cast<PF_Handle>(extraP->input->pre_render_data));
	}

	return err;
}
