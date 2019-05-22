#include "tl_math.h"
#include "glbinding/gl33ext/gl.h"
#include <glbinding/gl/extension.h>



namespace {
	THREAD_LOCAL int t_thread = -1;

	std::atomic_int S_cnt;
	std::map<int, std::shared_ptr<AESDK_OpenGL::AESDK_OpenGL_EffectRenderData> > S_render_contexts;
	std::recursive_mutex S_mutex;

	AESDK_OpenGL::AESDK_OpenGL_EffectCommonDataPtr S_GLator_EffectCommonData; //global context


																			  // - OpenGL resources are restricted per thread, mimicking the OGL driver
																			  // - The filter will eliminate all TLS (Thread Local Storage) at PF_Cmd_GLOBAL_SETDOWN
	AESDK_OpenGL::AESDK_OpenGL_EffectRenderDataPtr GetCurrentRenderContext()
	{
		S_mutex.lock();
		AESDK_OpenGL::AESDK_OpenGL_EffectRenderDataPtr result;

		if (t_thread == -1) {
			t_thread = S_cnt++;

			result.reset(new AESDK_OpenGL::AESDK_OpenGL_EffectRenderData());
			S_render_contexts[t_thread] = result;
		}
		else {
			result = S_render_contexts[t_thread];
		}
		S_mutex.unlock();
		return result;
	}

#ifdef AE_OS_WIN
	std::string get_string_from_wcs(const wchar_t* pcs)
	{
		int res = WideCharToMultiByte(CP_ACP, 0, pcs, -1, NULL, 0, NULL, NULL);

		std::auto_ptr<char> shared_pbuf(new char[res]);

		char *pbuf = shared_pbuf.get();

		res = WideCharToMultiByte(CP_ACP, 0, pcs, -1, pbuf, res, NULL, NULL);

		return std::string(pbuf);
	}
#endif

	void RenderQuad(GLuint vbo)
	{
		glEnableVertexAttribArray(PositionSlot);
		glEnableVertexAttribArray(UVSlot);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glVertexAttribPointer(PositionSlot, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0);
		glVertexAttribPointer(UVSlot, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glDisableVertexAttribArray(PositionSlot);
		glDisableVertexAttribArray(UVSlot);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}



	struct CopyPixelFloat_t {
		PF_PixelFloat	*floatBufferP;
		PF_EffectWorld	*input_worldP;
	};

	PF_Err
		CopyPixelFloatIn(
			void			*refcon,
			A_long			x,
			A_long			y,
			PF_PixelFloat	*inP,
			PF_PixelFloat	*)
	{
		CopyPixelFloat_t	*thiS = reinterpret_cast<CopyPixelFloat_t*>(refcon);
		PF_PixelFloat		*outP = thiS->floatBufferP + y * thiS->input_worldP->width + x;

		outP->red = inP->red;
		outP->green = inP->green;
		outP->blue = inP->blue;
		outP->alpha = inP->alpha;

		return PF_Err_NONE;
	}

	PF_Err
		CopyPixelFloatOut(
			void			*refcon,
			A_long			x,
			A_long			y,
			PF_PixelFloat	*,
			PF_PixelFloat	*outP)
	{
		CopyPixelFloat_t		*thiS = reinterpret_cast<CopyPixelFloat_t*>(refcon);
		const PF_PixelFloat		*inP = thiS->floatBufferP + y * thiS->input_worldP->width + x;

		outP->red = inP->red;
		outP->green = inP->green;
		outP->blue = inP->blue;
		outP->alpha = inP->alpha;

		return PF_Err_NONE;
	}


	gl::GLuint UploadTexture(AEGP_SuiteHandler& suites,					// >>
		PF_PixelFormat			format,				// >>
		PF_EffectWorld			*input_worldP,		// >>
		PF_EffectWorld			*output_worldP,		// >>
		PF_InData				*in_data,			// >>
		size_t& pixSizeOut,						// <<
		gl::GLenum& glFmtOut,						// <<
		float& multiplier16bitOut,
	    gl::GLuint  textureNum )					// <<
	{
		// - upload to texture memory
		// - we will convert on-the-fly from ARGB to RGBA, and also to pre-multiplied alpha,
		// using a fragment shader
#ifdef _DEBUG
		GLint nUnpackAlignment;
		::glGetIntegerv(GL_UNPACK_ALIGNMENT, &nUnpackAlignment);
		assert(nUnpackAlignment == 4);
#endif

		gl::GLuint inputFrameTexture;
		glGenTextures(1, &inputFrameTexture);

		glBindTexture(GL_TEXTURE_2D, inputFrameTexture);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (GLint)GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (GLint)GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, (GLint)GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, (GLint)GL_CLAMP_TO_EDGE);

		glTexImage2D(GL_TEXTURE_2D, 0, (GLint)GL_RGBA32F, input_worldP->width, input_worldP->height, 0, GL_RGBA, GL_FLOAT, nullptr);

		multiplier16bitOut = 1.0f;
		switch (format)
		{
		case PF_PixelFormat_ARGB128:
		{
			glFmtOut = GL_FLOAT;
			pixSizeOut = sizeof(PF_PixelFloat);

			std::auto_ptr<PF_PixelFloat> bufferFloat(new PF_PixelFloat[input_worldP->width * input_worldP->height]);
			CopyPixelFloat_t refcon = { bufferFloat.get(), input_worldP };

			CHECK(suites.IterateFloatSuite1()->iterate(in_data,
				0,
				input_worldP->height,
				input_worldP,
				nullptr,
				reinterpret_cast<void*>(&refcon),
				CopyPixelFloatIn,
				output_worldP));

			glPixelStorei(GL_UNPACK_ROW_LENGTH, input_worldP->width);
			glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, input_worldP->width, input_worldP->height, GL_RGBA, GL_FLOAT, bufferFloat.get());
			break;
		}

		case PF_PixelFormat_ARGB64:
		{
			glFmtOut = GL_UNSIGNED_SHORT;
			pixSizeOut = sizeof(PF_Pixel16);
			multiplier16bitOut = 65535.0f / 32768.0f;

			glPixelStorei(GL_UNPACK_ROW_LENGTH, input_worldP->rowbytes / sizeof(PF_Pixel16));
			PF_Pixel16 *pixelDataStart = NULL;
			PF_GET_PIXEL_DATA16(input_worldP, NULL, &pixelDataStart);
			glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, input_worldP->width, input_worldP->height, GL_RGBA, GL_UNSIGNED_SHORT, pixelDataStart);
			break;
		}

		case PF_PixelFormat_ARGB32:
		{
			glFmtOut = GL_UNSIGNED_BYTE;
			pixSizeOut = sizeof(PF_Pixel8);

			glPixelStorei(GL_UNPACK_ROW_LENGTH, input_worldP->rowbytes / sizeof(PF_Pixel8));
			PF_Pixel8 *pixelDataStart = NULL;
			PF_GET_PIXEL_DATA8(input_worldP, NULL, &pixelDataStart);
			glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, input_worldP->width, input_worldP->height, GL_RGBA, GL_UNSIGNED_BYTE, pixelDataStart);
			break;
		}

		default:
			CHECK(PF_Err_BAD_CALLBACK_PARAM);
			break;
		}

		glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);

		//unbind all textures
		glBindTexture(GL_TEXTURE_2D, textureNum);

		return inputFrameTexture;
	}

	void ReportIfErrorFramebuffer(PF_InData *in_data, PF_OutData *out_data)
	{
		// Check for errors...
		std::string error_msg;
		if ((error_msg = CheckFramebufferStatus()) != std::string("OK"))
		{
			out_data->out_flags |= PF_OutFlag_DISPLAY_ERROR_MESSAGE;
			PF_SPRINTF(out_data->return_msg, error_msg.c_str());
			CHECK(PF_Err_OUT_OF_MEMORY);
		}
	}


	void SwizzleGL(const AESDK_OpenGL::AESDK_OpenGL_EffectRenderDataPtr& renderContext,
		A_long widthL, A_long heightL,
		gl::GLuint		inputFrameTexture,
		float			multiplier16bit)
	{
		glBindTexture(GL_TEXTURE_2D, inputFrameTexture);

		glUseProgram(renderContext->mProgramObj2Su);

		// view matrix, mimic windows coordinates
		vmath::Matrix4 ModelviewProjection = vmath::Matrix4::translation(vmath::Vector3(-1.0f, -1.0f, 0.0f)) *
			vmath::Matrix4::scale(vmath::Vector3(2.0 / float(widthL), 2.0 / float(heightL), 1.0f));

		GLint location = glGetUniformLocation(renderContext->mProgramObj2Su, "ModelviewProjection");
		glUniformMatrix4fv(location, 1, GL_FALSE, (GLfloat*)&ModelviewProjection);
		location = glGetUniformLocation(renderContext->mProgramObj2Su, "multiplier16bit");
		glUniform1f(location, multiplier16bit);

		AESDK_OpenGL_BindTexture0ToTarget(renderContext->mProgramObj2Su, inputFrameTexture, std::string("videoTexture"));

		// render
		glBindVertexArray(renderContext->vao);
		RenderQuad(renderContext->quad);
  		glBindVertexArray(0);

		glUseProgram(0);

		glFlush();
	}

	PF_FpShort convertYCoordAEToGL( PF_FpShort ypt, PF_FpShort wHeight)
	{
		return wHeight - ypt; 

	}

	void RenderGL(const AESDK_OpenGL::AESDK_OpenGL_EffectRenderDataPtr& renderContext,
		A_long widthL, A_long heightL,
		gl::GLuint		inputFrameTexture,
		gl::GLuint	inputExtFrameTexture,
		void			*refcon,
		float			multiplier16bit)
	{
		MathInfo           *miP = reinterpret_cast<MathInfo*>(refcon);
		// - make sure we blend correctly inside the framebuffer
		// - even though we just cleared it, another effect may want to first
		// draw some kind of background to blend with
		glEnable(GL_BLEND);
		glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
		glBlendEquation(GL_FUNC_ADD);

		// view matrix, mimic windows coordinates
		vmath::Matrix4 ModelviewProjection = vmath::Matrix4::translation(vmath::Vector3(-1.0f, -1.0f, 0.0f)) *
		vmath::Matrix4::scale(vmath::Vector3(2.0 / float(widthL), 2.0 / float(heightL), 1.0f));

		glBindTexture(GL_TEXTURE_2D, inputFrameTexture);
        glBindTexture(GL_TEXTURE_2D, inputExtFrameTexture);
		glUseProgram(renderContext->mProgramObjSu);

		// program uniforms
		GLint location = glGetUniformLocation(renderContext->mProgramObjSu, "ModelviewProjection");
		glUniformMatrix4fv(location, 1, GL_FALSE, (GLfloat*)&ModelviewProjection);
		location = glGetUniformLocation(renderContext->mProgramObjSu, "var1");
		glUniform1f(location, miP->inOneF);
		location = glGetUniformLocation(renderContext->mProgramObjSu, "var2");
		glUniform1f(location, miP->inTwoF);
		location = glGetUniformLocation(renderContext->mProgramObjSu, "var3");
		glUniform1f(location, miP->inThreeF);
		location = glGetUniformLocation(renderContext->mProgramObjSu, "var4");
		glUniform1f(location, miP->inFourF);
		location = glGetUniformLocation(renderContext->mProgramObjSu, "pt1");
		glUniform2f(location, miP->pointOneX, convertYCoordAEToGL(miP->pointOneY, heightL));
		location = glGetUniformLocation(renderContext->mProgramObjSu, "mouse");
		glUniform2f(location, miP->pointOneX, convertYCoordAEToGL(miP->pointOneY, heightL));
		location = glGetUniformLocation(renderContext->mProgramObjSu, "iMouse");
		glUniform2f(location, miP->pointOneX, convertYCoordAEToGL(miP->pointOneY, heightL));
		location = glGetUniformLocation(renderContext->mProgramObjSu, "pt2");
		glUniform2f(location, miP->pointTwoX, convertYCoordAEToGL(miP->pointOneY, heightL));
		location = glGetUniformLocation(renderContext->mProgramObjSu, "cl1");
		glUniform3f(location, miP->colorOne[0], miP->colorOne[1], miP->colorOne[2]);
		location = glGetUniformLocation(renderContext->mProgramObjSu, "cl2");
		glUniform3f(location, miP->colorTwo[0], miP->colorTwo[1], miP->colorTwo[2]);
		location = glGetUniformLocation(renderContext->mProgramObjSu, "time");
		glUniform1f(location, miP->layerTime_Sec);
		location = glGetUniformLocation(renderContext->mProgramObjSu, "iTime");
		glUniform1f(location, miP->layerTime_Sec);
		location = glGetUniformLocation(renderContext->mProgramObjSu, "layertime_sec");
		glUniform1f(location, miP->layerTime_Sec);
		location = glGetUniformLocation(renderContext->mProgramObjSu, "layerTime_frame");
		glUniform1f(location, miP->layerTime_Frame);
		location = glGetUniformLocation(renderContext->mProgramObjSu, "layerDuration");
		glUniform1f(location, miP->layerDuration);
		location = glGetUniformLocation(renderContext->mProgramObjSu, "resolution");
		glUniform2f(location, miP->layerWidthF, miP->layerHeightF);
		location = glGetUniformLocation(renderContext->mProgramObjSu, "iResolution");
		glUniform2f(location, miP->layerWidthF, miP->layerHeightF);
		location = glGetUniformLocation(renderContext->mProgramObjSu, "layerPosition");
		glUniform3f(location, miP->layerPos_X, convertYCoordAEToGL(miP->pointOneY, heightL), miP->layerPos_Z);
		location = glGetUniformLocation(renderContext->mProgramObjSu, "layerScale");
		glUniform3f(location, miP->layerScale_X, convertYCoordAEToGL(miP->pointOneY, heightL), miP->layerScale_Z);
		location = glGetUniformLocation(renderContext->mProgramObjSu, "compWidth");
		glUniform1f(location, miP->compWidthF);
		location = glGetUniformLocation(renderContext->mProgramObjSu, "compHeight");
		glUniform1f(location, miP->compHeightF);
		location = glGetUniformLocation(renderContext->mProgramObjSu, "compFps");
		glUniform1f(location, miP->compFpsF);
		location = glGetUniformLocation(renderContext->mProgramObjSu, "multiplier16bit");
		glUniform1f(location, multiplier16bit);
		// Identify the texture to use and bind it to texture unit 0
		AESDK_OpenGL_BindTexture0ToTarget(renderContext->mProgramObjSu, inputFrameTexture, std::string("layerTex"));
		AESDK_OpenGL_BindTexture1ToTarget(renderContext->mProgramObjSu, inputExtFrameTexture, std::string("extLayerTex"));
		// render
		glBindVertexArray(renderContext->vao);
		RenderQuad(renderContext->quad);
		glBindVertexArray(0);

		glUseProgram(0);
		glDisable(GL_BLEND);
	}

	void DownloadTexture(const AESDK_OpenGL::AESDK_OpenGL_EffectRenderDataPtr& renderContext,
		AEGP_SuiteHandler&		suites,				// >>
		PF_EffectWorld			*input_worldP,		// >>
		PF_EffectWorld			*output_worldP,		// >>
		PF_InData				*in_data,			// >>
		PF_PixelFormat			format,				// >>
		size_t					pixSize,			// >>
		gl::GLenum				glFmt				// >>
	)
	{
		//download from texture memory onto the same surface
		PF_Handle bufferH = NULL;
		bufferH = suites.HandleSuite1()->host_new_handle(((renderContext->mRenderBufferWidthSu * renderContext->mRenderBufferHeightSu)* pixSize));
		if (!bufferH) {
			CHECK(PF_Err_OUT_OF_MEMORY);
		}
		void *bufferP = suites.HandleSuite1()->host_lock_handle(bufferH);

		glReadBuffer(GL_COLOR_ATTACHMENT0);
		glReadPixels(0, 0, renderContext->mRenderBufferWidthSu, renderContext->mRenderBufferHeightSu, GL_RGBA, glFmt, bufferP);

		switch (format)
		{
		case PF_PixelFormat_ARGB128:
		{
			PF_PixelFloat* bufferFloatP = reinterpret_cast<PF_PixelFloat*>(bufferP);
			CopyPixelFloat_t refcon = { bufferFloatP, input_worldP };

			CHECK(suites.IterateFloatSuite1()->iterate(in_data,
				0,
				input_worldP->height,
				input_worldP,
				nullptr,
				reinterpret_cast<void*>(&refcon),
				CopyPixelFloatOut,
				output_worldP));
			break;
		}

		case PF_PixelFormat_ARGB64:
		{
			PF_Pixel16* buffer16P = reinterpret_cast<PF_Pixel16*>(bufferP);

			//copy to output_worldP
			for (int y = 0; y < output_worldP->height; ++y)
			{
				PF_Pixel16 *pixelDataStart = NULL;
				PF_GET_PIXEL_DATA16(output_worldP, NULL, &pixelDataStart);
				::memcpy(pixelDataStart + (y * output_worldP->rowbytes / sizeof(PF_Pixel16)),
					buffer16P + (y * renderContext->mRenderBufferWidthSu),
					output_worldP->width * sizeof(PF_Pixel16));
			}
			break;
		}

		case PF_PixelFormat_ARGB32:
		{
			PF_Pixel8 *buffer8P = reinterpret_cast<PF_Pixel8*>(bufferP);

			//copy to output_worldP
			for (int y = 0; y < output_worldP->height; ++y)
			{
				PF_Pixel8 *pixelDataStart = NULL;
				PF_GET_PIXEL_DATA8(output_worldP, NULL, &pixelDataStart);
				::memcpy(pixelDataStart + (y * output_worldP->rowbytes / sizeof(PF_Pixel8)),
					buffer8P + (y * renderContext->mRenderBufferWidthSu),
					output_worldP->width * sizeof(PF_Pixel8));
			}
			break;
		}

		default:
			CHECK(PF_Err_BAD_CALLBACK_PARAM);
			break;
		}

		//clean the data after being copied
		suites.HandleSuite1()->host_unlock_handle(bufferH);
		suites.HandleSuite1()->host_dispose_handle(bufferH);
	}
} // anonymous namespace


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

    
	out_data->out_flags = PF_OutFlag_CUSTOM_UI |
		PF_OutFlag_PIX_INDEPENDENT |
		PF_OutFlag_SEND_UPDATE_PARAMS_UI |
		PF_OutFlag_WIDE_TIME_INPUT |
		PF_OutFlag_DEEP_COLOR_AWARE |	// just 16bpc, not 32bpc
		PF_OutFlag_NON_PARAM_VARY ;


	out_data->out_flags2 = PF_OutFlag2_SUPPORTS_QUERY_DYNAMIC_FLAGS |
							PF_OutFlag2_FLOAT_COLOR_AWARE |
							PF_OutFlag2_SUPPORTS_SMART_RENDER | 
							PF_OutFlag2_AUTOMATIC_WIDE_TIME_INPUT;
    

    
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
    }
	else	{
        err = PF_Err_INTERNAL_STRUCT_DAMAGED;
    }
	try
	{
		// always restore back AE's own OGL context
		SaveRestoreOGLContext oSavedContext;
		AEGP_SuiteHandler suites(in_data->pica_basicP);

		//Now comes the OpenGL part - OS specific loading to start with
		S_GLator_EffectCommonData.reset(new AESDK_OpenGL::AESDK_OpenGL_EffectCommonData());
		AESDK_OpenGL_Startup(*S_GLator_EffectCommonData.get());

	}
	catch (PF_Err& thrown_err)
	{
		err = thrown_err;
	}
	catch (...)
	{
		err = PF_Err_OUT_OF_MEMORY;
	}

	return err;
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


	// INVISIBLE PARAM TO EXCHANGE WITH CEP
	PF_ADD_CHECKBOXX(STR(StrID_CEP_SENDARB_Param_Name), FALSE, 0, MATH_CEP_SEND_ARB_DATA_DISK_ID);
	AEFX_CLR_STRUCT(def);
	PF_ADD_CHECKBOXX(STR(StrID_CEP_GETARB_Param_Name), FALSE, 0, MATH_CEP_GET_ARB_DATA_DISK_ID);
	AEFX_CLR_STRUCT(def);
	PF_ADD_CHECKBOXX(STR(StrID_CEP_GETEVAL_Param_Name), FALSE, 0, MATH_CEP_GET_EVAL_DISK_ID);
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
	def.flags= PF_ParamFlag_SUPERVISE;
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
							PF_ParamFlag_SUPERVISE,
							uiSliderOne_VAR_DISK_ID);

	AEFX_CLR_STRUCT(def);
    
    PF_ADD_FLOAT_SLIDERX(	STR(StrID_INPUTTWO_Param_Name),
                         MATH_VAR_MIN,
                         MATH_VAR_MAX,
                         MATH_VAR_MIN,
                         MATH_SLIDER_MAX,
                         MATH_VAR_DFLT,
                         PF_Precision_THOUSANDTHS,
                         0,
						PF_ParamFlag_SUPERVISE,
                         MATH_SLIDER_TWO_VAR_DISK_ID);
    
    AEFX_CLR_STRUCT(def);
    
    PF_ADD_FLOAT_SLIDERX(	STR(StrID_INPUTTHREE_Param_Name),
                         MATH_VAR_MIN,
                         MATH_VAR_MAX,
                         MATH_VAR_MIN,
                         MATH_SLIDER_MAX,
                         MATH_VAR_DFLT,
                         PF_Precision_THOUSANDTHS,
                         0,
		   				PF_ParamFlag_SUPERVISE,
                         MATH_SLIDER_THREE_VAR_DISK_ID);
    
    AEFX_CLR_STRUCT(def);
    
    PF_ADD_FLOAT_SLIDERX(STR( StrID_INPUTFOUR_Param_Name),
                         MATH_VAR_MIN,
                         MATH_VAR_MAX,
                         MATH_VAR_MIN,
                         MATH_SLIDER_MAX,
                         MATH_VAR_DFLT,
                         PF_Precision_THOUSANDTHS,
                         0,
						 PF_ParamFlag_SUPERVISE,
                         MATH_SLIDER_FOUR_VAR_DISK_ID);
    
    AEFX_CLR_STRUCT(def);
    PF_END_TOPIC( MATH_TOPIC_SLIDER_DISK_ID);

    AEFX_CLR_STRUCT(def);
    
	def.flags = PF_ParamFlag_SUPERVISE;
    PF_ADD_TOPIC(STR( StrID_TOPIC_POINTS_Param_Name), MATH_TOPIC_POINTS_DISK_ID);
	AEFX_CLR_STRUCT(def);
	 def.flags = PF_ParamFlag_SUPERVISE;
    PF_ADD_POINT(STR (strID_INPUTPOINTONE_Param_Name), 50, 50, FALSE, MATH_POINT_ONE_DISK_ID);
    AEFX_CLR_STRUCT(def);
	def.flags = PF_ParamFlag_SUPERVISE;
    PF_ADD_POINT(STR(strID_INPUTPOINTTWO_Param_Name), 50, 50, FALSE, MATH_POINT_TWO_DISK_ID);
    AEFX_CLR_STRUCT(def);
    PF_END_TOPIC(MATH_TOPIC_POINTS_DISK_ID);
    AEFX_CLR_STRUCT(def);
	def.flags = PF_ParamFlag_SUPERVISE;
    PF_ADD_TOPIC(STR( StrID_TOPIC_COLORS_Param_Name), MATH_TOPIC_COLORS_DISK_ID);
    AEFX_CLR_STRUCT(def);
	def.flags = PF_ParamFlag_SUPERVISE;
    PF_ADD_COLOR(STR(strID_INPUTCOLORONE_Param_Name), PF_MAX_CHAN8, PF_MAX_CHAN8, PF_MAX_CHAN8, MATH_COLOR_ONE_DISK_ID);
    AEFX_CLR_STRUCT(def);
	def.flags = PF_ParamFlag_SUPERVISE;
    PF_ADD_COLOR(STR(strID_INPUTCOLORTWO_Param_Name), PF_MAX_CHAN8, PF_MAX_CHAN8, PF_MAX_CHAN8,MATH_COLOR_TWO_DISK_ID);
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
                         MATH_VAR_MIN,
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




    out_data->num_params = MATH_NUM_PARAMS;
    


	return err;
}

static PF_Err
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

    
    return PF_Err_NONE;
    
}
static PF_Err
UpdateParameterUI(
                  PF_InData			*in_data,
                  PF_OutData			*out_data,
                  PF_ParamDef			*params[],
                  PF_LayerDef			*outputP)
{
    PF_Err				err = PF_Err_NONE ,err2			= PF_Err_NONE;
    AEGP_SuiteHandler		suites(in_data->pica_basicP);
	AEGP_EffectRefH			meH = NULL;
    PF_ParamDef		param_copy[MATH_NUM_PARAMS];
    ERR(MakeParamCopy(params, param_copy));
	my_global_dataP		globP = reinterpret_cast<my_global_dataP>(DH(out_data->global_data));
    seqDataP            seqP = reinterpret_cast<seqDataP>(DH(out_data->sequence_data));
	AEGP_StreamRefH		MATH_TOPIC_SLIDER_streamH = NULL,
                        MATH_SLIDER_ONE_VAR_streamH = NULL,
                        MATH_SLIDER_TWO_VAR_streamH = NULL,
                        MATH_SLIDER_THREE_VAR_streamH = NULL,
                        MATH_SLIDER_FOUR_VAR_streamH = NULL,
                        MATH_TOPIC_POINTS_streamH = NULL,
                        MATH_POINT_ONE_streamH =NULL,
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

		strcpy(param_copy[MATH_TOPIC_SLIDER].name, seqP->sliderGrpP.grpNameAc);
		ERR(suites.ParamUtilsSuite3()->PF_UpdateParamUI(in_data->effect_ref,
														MATH_TOPIC_SLIDER,
														&param_copy[MATH_TOPIC_SLIDER]));

		strcpy(param_copy[MATH_SLIDER_ONE_VAR].name, seqP->paramSliderVc[0].sliderNameAc);
		ERR(suites.ParamUtilsSuite3()->PF_UpdateParamUI(in_data->effect_ref,
														MATH_SLIDER_ONE_VAR,
														&param_copy[MATH_SLIDER_ONE_VAR]));

		strcpy(param_copy[MATH_SLIDER_TWO_VAR].name, seqP->paramSliderVc[1].sliderNameAc);
		ERR(suites.ParamUtilsSuite3()->PF_UpdateParamUI(in_data->effect_ref,
														MATH_SLIDER_TWO_VAR,
														&param_copy[MATH_SLIDER_TWO_VAR]));

		strcpy(param_copy[MATH_SLIDER_THREE_VAR].name, seqP->paramSliderVc[3].sliderNameAc);
		ERR(suites.ParamUtilsSuite3()->PF_UpdateParamUI(in_data->effect_ref,
														MATH_SLIDER_THREE_VAR,
														&param_copy[MATH_SLIDER_THREE_VAR]));

		strcpy(param_copy[MATH_SLIDER_FOUR_VAR].name, seqP->paramSliderVc[4].sliderNameAc);
		ERR(suites.ParamUtilsSuite3()->PF_UpdateParamUI(in_data->effect_ref,
														MATH_SLIDER_FOUR_VAR,
														&param_copy[MATH_SLIDER_FOUR_VAR]));


		strcpy(param_copy[MATH_TOPIC_POINTS].name, seqP->pointGrpP.grpNameAc);
		ERR(suites.ParamUtilsSuite3()->PF_UpdateParamUI(in_data->effect_ref,
														MATH_TOPIC_POINTS,
														&param_copy[MATH_TOPIC_POINTS]));
		strcpy(param_copy[MATH_POINT_ONE].name, seqP->paramPointVc[0].pointNameAc);
		ERR(suites.ParamUtilsSuite3()->PF_UpdateParamUI(in_data->effect_ref,
														MATH_POINT_ONE,
														&param_copy[MATH_POINT_ONE]));
		strcpy(param_copy[MATH_POINT_TWO].name, seqP->paramPointVc[1].pointNameAc);
		ERR(suites.ParamUtilsSuite3()->PF_UpdateParamUI(in_data->effect_ref,
														MATH_POINT_TWO,
														&param_copy[MATH_POINT_TWO]));


		strcpy(param_copy[MATH_TOPIC_COLORS].name, seqP->colorGrpP.grpNameAc);
		ERR(suites.ParamUtilsSuite3()->PF_UpdateParamUI(in_data->effect_ref,
														MATH_TOPIC_COLORS,
														&param_copy[MATH_TOPIC_COLORS]));
		strcpy(param_copy[MATH_COLOR_ONE].name, seqP->paramColorVc[0].colorNameAc);
		ERR(suites.ParamUtilsSuite3()->PF_UpdateParamUI(in_data->effect_ref,
														MATH_COLOR_ONE,
														&param_copy[MATH_COLOR_ONE]));

		strcpy(param_copy[MATH_COLOR_TWO].name, seqP->paramColorVc[0].colorNameAc);
		ERR(suites.ParamUtilsSuite3()->PF_UpdateParamUI(in_data->effect_ref,
														MATH_COLOR_TWO,
														&param_copy[MATH_COLOR_TWO]));


		strcpy(param_copy[MATH_TOPIC_INPUTS].name, seqP->layerGrpP.grpNameAc);
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

		ERR(suites.DynamicStreamSuite2()->AEGP_SetDynamicStreamFlag(MATH_TOPIC_SLIDER_streamH, AEGP_DynStreamFlag_HIDDEN, FALSE, !seqP->sliderGrpP.grpVisibleB));
        ERR(suites.DynamicStreamSuite2()->AEGP_SetDynamicStreamFlag(MATH_SLIDER_ONE_VAR_streamH, AEGP_DynStreamFlag_HIDDEN, FALSE, !seqP->paramSliderVc[0].sliderVisibleB));
		ERR(suites.DynamicStreamSuite2()->AEGP_SetDynamicStreamFlag(MATH_SLIDER_TWO_VAR_streamH, AEGP_DynStreamFlag_HIDDEN, FALSE, !seqP->paramSliderVc[1].sliderVisibleB));
		ERR(suites.DynamicStreamSuite2()->AEGP_SetDynamicStreamFlag(MATH_SLIDER_THREE_VAR_streamH, AEGP_DynStreamFlag_HIDDEN, FALSE,!seqP->paramSliderVc[2].sliderVisibleB));
		ERR(suites.DynamicStreamSuite2()->AEGP_SetDynamicStreamFlag(MATH_SLIDER_FOUR_VAR_streamH, AEGP_DynStreamFlag_HIDDEN, FALSE, !seqP->paramSliderVc[3].sliderVisibleB));

		ERR(suites.DynamicStreamSuite2()->AEGP_SetDynamicStreamFlag(MATH_TOPIC_POINTS_streamH, AEGP_DynStreamFlag_HIDDEN, FALSE, !seqP->pointGrpP.grpVisibleB));
		ERR(suites.DynamicStreamSuite2()->AEGP_SetDynamicStreamFlag(MATH_POINT_ONE_streamH, AEGP_DynStreamFlag_HIDDEN, FALSE, !seqP->paramPointVc[0].pointVisibleB));
		ERR(suites.DynamicStreamSuite2()->AEGP_SetDynamicStreamFlag(MATH_POINT_TWO_streamH, AEGP_DynStreamFlag_HIDDEN, FALSE, !seqP->paramPointVc[1].pointVisibleB));

		ERR(suites.DynamicStreamSuite2()->AEGP_SetDynamicStreamFlag(MATH_TOPIC_COLORS_streamH, AEGP_DynStreamFlag_HIDDEN, FALSE, !seqP->colorGrpP.grpVisibleB));
		ERR(suites.DynamicStreamSuite2()->AEGP_SetDynamicStreamFlag(MATH_COLOR_ONE_streamH, AEGP_DynStreamFlag_HIDDEN, FALSE, !seqP->paramColorVc[0].colorVisibleB));
		ERR(suites.DynamicStreamSuite2()->AEGP_SetDynamicStreamFlag(MATH_COLOR_TWO_streamH, AEGP_DynStreamFlag_HIDDEN, FALSE, !seqP->paramColorVc[1].colorVisibleB));

		ERR(suites.DynamicStreamSuite2()->AEGP_SetDynamicStreamFlag(MATH_TOPIC_INPUTS_streamH, AEGP_DynStreamFlag_HIDDEN, FALSE, !seqP->layerGrpP.grpVisibleB));

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

static PF_Err
UserChangedParam(
	PF_InData						*in_data,
	PF_OutData						*out_data,
	PF_ParamDef						*params[],
	PF_LayerDef						*outputP,
	const PF_UserChangedParamExtra	*which_hitP)
{
	PF_Err				err = PF_Err_NONE;
    std::string taskId;
    AEGP_SuiteHandler    suites(in_data->pica_basicP);

	if (which_hitP->param_index == MATH_SETUP)
	{
		ERR(CallCepDialog(in_data, out_data, taskId));
        ERR(suites.AdvAppSuite2()->PF_RefreshAllWindows());
		ERR(SetupDialogSend(in_data, out_data, params,taskId, outputP));
        //ERR(CallCepDeleteTask(in_data, out_data, taskId));

	}

	return err;
}

void evalFragShader(std::string inFragmentShaderStr, std::string& errReturn)
{
    // always restore back AE's own OGL context
    SaveRestoreOGLContext oSavedContext;

    // our render specific context (one per thread)
    AESDK_OpenGL::AESDK_OpenGL_EffectRenderDataPtr renderContext = GetCurrentRenderContext();

    if (!renderContext->mInitialized) {
        //Now comes the OpenGL part - OS specific loading to start with
        AESDK_OpenGL_Startup(*renderContext.get(), S_GLator_EffectCommonData.get());
        renderContext->mInitialized = true;
    }

    renderContext->mProgramObjSu = 0;
    renderContext->SetPluginContext();

    GLint fragCompiledB =0;
    const char *fragmentShaderStringsP = (const GLchar *) inFragmentShaderStr.c_str();

    GLuint fragmentShaderSu = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShaderSu, 1, &fragmentShaderStringsP, NULL);
    glCompileShader(fragmentShaderSu);

    glGetShaderiv(fragmentShaderSu, GL_COMPILE_STATUS, &fragCompiledB);
    if (!fragCompiledB) {
        char str[4096];
        glGetShaderInfoLog(fragmentShaderSu, sizeof(str), NULL, str);
        errReturn = str;
    }
    else {
        errReturn = compile_success;
    }
    glDeleteShader(fragmentShaderSu);
}
static PF_Err
Render_GLSL(PF_InData                *in_data,
		    PF_OutData               *out_data,
			PF_EffectWorld           *inputP,
			PF_EffectWorld           *outputP,
			PF_EffectWorld           *extLW,
			PF_PixelFormat           format,
			AEGP_SuiteHandler        &suites,
			void                    *refcon, 
			PF_Boolean              ShaderResetB,
			const std::string&		vertexShstr,
			const std::string&		fragSh1str,
			const std::string&		fragSh2str
)

{
	PF_Err err = PF_Err_NONE;
	MathInfo           *miP = reinterpret_cast<MathInfo*>(refcon);
	try
	{

		// always restore back AE's own OGL context
		SaveRestoreOGLContext oSavedContext;

		// our render specific context (one per thread)
		AESDK_OpenGL::AESDK_OpenGL_EffectRenderDataPtr renderContext = GetCurrentRenderContext();

		if (!renderContext->mInitialized) {
			//Now comes the OpenGL part - OS specific loading to start with
			AESDK_OpenGL_Startup(*renderContext.get(), S_GLator_EffectCommonData.get());
			renderContext->mInitialized = true;
		}

		renderContext->mProgramObjSu = 0;
		renderContext->SetPluginContext();

		// - Gremedy OpenGL debugger
		// - Example of using a OpenGL extension
		//bool hasGremedy = renderContext->mExtensions.find(gl::GLextension::GL_GREMEDY_frame_terminator) != renderContext->mExtensions.end();

		A_long				widthL = miP->layerWidthF; // inputP->width;
		A_long				heightL = miP->layerHeightF;  //inputP->height;


		//loading OpenGL resources
		AESDK_OpenGL_InitResources(*renderContext.get(),
			out_data,
			suites,
			widthL,
			heightL,
			ShaderResetB,
			vertexShstr,
			fragSh1str,
			fragSh2str);
        


		//CHECK(format);
		// upload the input world to a texture
		size_t pixSize;
		gl::GLenum glFmt;
		float multiplier16bit;
		gl::GLuint inputFrameTexture = UploadTexture(suites, format, inputP, outputP, in_data, pixSize, glFmt, multiplier16bit,0);
		gl::GLuint inputExtFrameTexture = UploadTexture(suites, format, extLW, outputP , in_data, pixSize, glFmt, multiplier16bit, 1);
		// Set up the frame-buffer object just like a window.
		AESDK_OpenGL_MakeReadyToRender(*renderContext.get(), renderContext->mOutputFrameTexture);
		ReportIfErrorFramebuffer(in_data, out_data);

		glViewport(0, 0, widthL, heightL);
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// - simply blend the texture inside the frame buffer
		RenderGL(renderContext, widthL, heightL, inputFrameTexture, inputExtFrameTexture,(void*)miP, multiplier16bit);

		// - we toggle PBO textures (we use the PBO we just created as an input)
		AESDK_OpenGL_MakeReadyToRender(*renderContext.get(), inputFrameTexture);
		AESDK_OpenGL_MakeReadyToRender(*renderContext.get(), inputExtFrameTexture);
		ReportIfErrorFramebuffer(in_data, out_data);

		glClear(GL_COLOR_BUFFER_BIT);

		// swizzle using the previous output
		SwizzleGL(renderContext, widthL, heightL, renderContext->mOutputFrameTexture, multiplier16bit);
		/*
		if (hasGremedy) {
			gl::glFrameTerminatorGREMEDY();
		}*/

		// - get back to CPU the result, and inside the output world
		DownloadTexture(renderContext, suites, inputP, outputP, in_data,
			format, pixSize, glFmt);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glBindTexture(GL_TEXTURE_2D, 0);
        glBindTexture(GL_TEXTURE_2D, 1);
		glDeleteTextures(1, &inputFrameTexture);
        glDeleteTextures(2, &inputExtFrameTexture);
	}
	catch (PF_Err& thrown_err)
	{
		err = thrown_err;
	}
	catch (...)
	{
		err = PF_Err_OUT_OF_MEMORY;
	}
	return  err;
}

static PF_Err
ExtLayerInput(void *refcon,
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
	if ( !err && (oiP->x_offFi != 0 || oiP->y_offFi != 0) ) {
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

static PF_Err
ExprRender( PF_OutData     *out_data,
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

	

	WorldTransfertInfo   wtiP;
	AEFX_CLR_STRUCT(wtiP);
	wtiP.inW = *inputP;
	wtiP.outW = *outputP;
	if (&extLW->data && flagsPP.PixelsCallExternalInputB) {
		wtiP.extLW = *extLW;
	}



	std::string exprErrStr = "Error \n";
	PF_Boolean returnExprErrB = false;

	fiP.redExpr = parseExpr<PF_FpShort>((void*)&miPP, &fiP, *exprP->redstr);
	if (fiP.hasErrorB)
	{
		fiP.channelErrorstr = "red channel expression";
		returnExprErrB = true;
		exprErrStr.append(fiP.channelErrorstr).append(": ").append(fiP.errorstr).append("\n");

	}
	fiP.greenExpr = parseExpr<PF_FpShort>((void*)&miPP, &fiP, *exprP->greenstr);
	if (fiP.hasErrorB)
	{
		fiP.channelErrorstr = "green channel expression";
		returnExprErrB = true;
		exprErrStr.append(fiP.channelErrorstr).append(": ").append(fiP.errorstr).append("\n");

	}
	fiP.blueExpr = parseExpr<PF_FpShort>((void*)&miPP, &fiP, *exprP->bluestr);
	if (fiP.hasErrorB)
	{
		fiP.channelErrorstr = "blue channel expression";
		returnExprErrB = true;
		exprErrStr.append(fiP.channelErrorstr).append(": ").append(fiP.errorstr).append("\n");

	}
	fiP.alphaExpr = parseExpr<PF_FpShort>((void*)&miPP, &fiP, *exprP->alphastr);
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
	threaded_render* thRenderPtr = new threaded_render();
	switch (format) {

	case PF_PixelFormat_ARGB128:
		AEFX_CLR_STRUCT(workers_thrds);
		for (A_long thrd_id = 0; thrd_id < num_thrd; ++thrd_id) {
			workers_thrds.emplace_back(std::thread(&threaded_render::render_32,
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
			workers_thrds.emplace_back(std::thread(&threaded_render::render_16,
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
			workers_thrds.emplace_back(std::thread(&threaded_render::render_8,
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


static PF_Err
PreRender(PF_InData                *in_data,
          PF_OutData                *out_data,
          PF_PreRenderExtra        *extraP)
{
    PF_Err    err                = PF_Err_NONE;

    PF_RenderRequest req    = extraP->input->output_request;

    PF_CheckoutResult        in_result, extL_result;
    AEGP_SuiteHandler        suites(in_data->pica_basicP);
    PF_Handle   infoH        =    suites.HandleSuite1()->host_new_handle(sizeof( MathInfoP));
    AEGP_LayerH        layerH;
    AEGP_CompH        compH;
    AEGP_ItemH      itemH;
    MathInfo        * miP = NULL;

    if (infoH){
         miP = reinterpret_cast< MathInfo*>(suites.HandleSuite1()->host_lock_handle(infoH));
        if ( miP){
            extraP->output->pre_render_data = infoH;

            PF_ParamDef  arb_param;
            AEFX_CLR_STRUCT(arb_param);
            ERR(PF_CHECKOUT_PARAM(  in_data,
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
                miP->compWidthF = PF_FpLong(width);
                miP->compHeightF = PF_FpLong(height);
                AEGP_DownsampleFactor dsp;
                compSuite->AEGP_GetCompDownsampleFactor(compH, &dsp);
                miP->compWidthF *= dsp.xS;
                miP->compHeightF *= dsp.yS;
                PF_FpLong fpsF;
                compSuite->AEGP_GetCompFramerate(compH,&fpsF);
                miP->compFpsF = static_cast<float>( fpsF);

                layerSuite->AEGP_GetLayerCurrentTime(layerH, AEGP_LTimeMode_LayerTime, &currTime);
                StreamSuite->AEGP_GetLayerStreamValue(layerH, AEGP_LayerStream_POSITION, AEGP_LTimeMode_LayerTime, &currTime, NULL, &strValP, &strTypeP);
                miP->layerPos_X = strValP.three_d.x;
                miP->layerPos_Y = strValP.three_d.y;
                miP->layerPos_Z = strValP.three_d.z;
                StreamSuite->AEGP_GetLayerStreamValue(layerH, AEGP_LayerStream_SCALE, AEGP_LTimeMode_LayerTime, &currTime, NULL, &strValSP, &strTypeP);
                miP->layerScale_X= strValSP.three_d.x;
				miP->layerScale_Y = strValSP.three_d.z;
                miP->layerScale_Z = strValSP.three_d.z;


            AEFX_CLR_STRUCT(in_result);
            AEFX_CLR_STRUCT(extL_result);

            if (!err){
                req.preserve_rgb_of_zero_alpha = FALSE;
                ERR(extraP->cb->checkout_layer( in_data->effect_ref,
                                                MATH_INPUT,
                                                MATH_INPUT,
                                                &req,
                                                in_data->current_time,
                                                in_data->time_step,
                                                in_data->time_scale,
                                                &in_result));






                if (!err){

                    UnionLRect(&in_result.result_rect,         &extraP->output->result_rect);
                    UnionLRect(&in_result.max_result_rect,     &extraP->output->max_result_rect);


                    ERR(extraP->cb->checkout_layer( in_data->effect_ref,
                                                   MATH_INP_LAYER_ONE,
                                                   MATH_INP_LAYER_ONE,
                                                   &req,
                                                   (in_data->current_time + A_long(extlayer_toff_param.u.fs_d.value) * in_data->time_step),
                                                   in_data->time_step,
                                                   in_data->time_scale,
                                                   &extL_result));


                    PF_Fixed     widthFi    = INT2FIX(ABS(extL_result.max_result_rect.right - extL_result.max_result_rect.left)),
                                 heightFi = INT2FIX(ABS(extL_result.max_result_rect.bottom - extL_result.max_result_rect.top));

                    miP->x_offFi = PF_Fixed ( widthFi/2 -  extlayer_poff_param.u.td.x_value);
                    miP->y_offFi = PF_Fixed ( heightFi/2 - extlayer_poff_param.u.td.y_value);



                }
            }
			ERR(PF_CHECKIN_PARAM(in_data, &arb_param));
            suites.HandleSuite1()->host_unlock_handle(infoH);
        }
    }
    return err;
}

static PF_Err
SmartRender(
            PF_InData                *in_data,
            PF_OutData                *out_data,
            PF_SmartRenderExtra        *extraP)
{

    PF_Err            err            = PF_Err_NONE,
    err2        = PF_Err_NONE;

    PF_EffectWorld  *inputP        = NULL,
                    *outputP    = NULL,
                    *extLP= NULL;
    PF_PixelFormat    format        =    PF_PixelFormat_INVALID;
    AEGP_SuiteHandler suites(in_data->pica_basicP);
    seqDataP            seqP = reinterpret_cast<seqDataP>(DH(out_data->sequence_data));
    AEFX_SuiteScoper<PF_WorldSuite2> wsP  = AEFX_SuiteScoper<PF_WorldSuite2>(in_data,
                                                                               kPFWorldSuite,
                                                                               kPFWorldSuiteVersion2,
                                                                               out_data);

    if (!err){

        MathInfo *miP = reinterpret_cast< MathInfo*>(suites.HandleSuite1()->host_lock_handle(reinterpret_cast<PF_Handle>(extraP->input->pre_render_data)));
       //flagInfo
        if (miP){
            OffInfo         oiP;
            AEFX_CLR_STRUCT(oiP);
            FlagsInfo      flagsP;
            AEFX_CLR_STRUCT(flagsP);
            m_ArbData        *arbP            = NULL;
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
						arb_param,
						var1_param,
						var2_param,
						var3_param,
						var4_param,
						point1_param,
						point2_param,
						color1_param,
                        color2_param;


            AEFX_CLR_STRUCT(setup_param);
            ERR(PF_CHECKOUT_PARAM(  in_data,
                                    MATH_SETUP,
                                    in_data->current_time,
                                    in_data->time_step,
                                    in_data->time_scale,
                                    &setup_param));

            AEFX_CLR_STRUCT(arb_param);
            ERR(PF_CHECKOUT_PARAM(  in_data,
                                  MATH_ARB_DATA,
                                  in_data->current_time,
                                  in_data->time_step,
                                  in_data->time_scale,
                                  &arb_param));

            AEFX_CLR_STRUCT(var1_param);
            ERR(PF_CHECKOUT_PARAM(  in_data,
                                  MATH_SLIDER_ONE_VAR,
                                  in_data->current_time,
                                  in_data->time_step,
                                  in_data->time_scale,
                                  &var1_param));

            AEFX_CLR_STRUCT(var2_param);
            ERR(PF_CHECKOUT_PARAM(in_data,
                                  MATH_SLIDER_TWO_VAR,
                                  in_data->current_time,
                                  in_data->time_step,
                                  in_data->time_scale,
                                  &var2_param));

            AEFX_CLR_STRUCT(var3_param);
            ERR(PF_CHECKOUT_PARAM(  in_data,
                                  MATH_SLIDER_THREE_VAR,
                                  in_data->current_time,
                                  in_data->time_step,
                                  in_data->time_scale,
                                  &var3_param));

            AEFX_CLR_STRUCT(var4_param);
            ERR(PF_CHECKOUT_PARAM(  in_data,
                                  MATH_SLIDER_FOUR_VAR,
                                  in_data->current_time,
                                  in_data->time_step,
                                  in_data->time_scale,
                                  &var4_param));


            AEFX_CLR_STRUCT(point1_param);
            ERR(PF_CHECKOUT_PARAM(  in_data,
								  MATH_POINT_ONE,
                                  in_data->current_time,
                                  in_data->time_step,
                                  in_data->time_scale,
                                  &point1_param));

            AEFX_CLR_STRUCT(point2_param);
            ERR(PF_CHECKOUT_PARAM(in_data,
								  MATH_POINT_TWO,
                                  in_data->current_time,
                                  in_data->time_step,
                                  in_data->time_scale,
                                  &point2_param));




            AEFX_CLR_STRUCT(color1_param);
            ERR(PF_CHECKOUT_PARAM(  in_data,
								  MATH_COLOR_ONE,
                                  in_data->current_time,
                                  in_data->time_step,
                                  in_data->time_scale,
                                  &color1_param));

            AEFX_CLR_STRUCT(color2_param);
            ERR(PF_CHECKOUT_PARAM(in_data,
								  MATH_COLOR_TWO,
                                  in_data->current_time,
                                  in_data->time_step,
                                  in_data->time_scale,
                                  &color2_param));


            //layer size
            miP->scale_x = in_data->downsample_x.num*in_data->pixel_aspect_ratio.num/ (float)in_data->downsample_x.den;
            miP->scale_y = in_data->downsample_y.num*in_data->pixel_aspect_ratio.den/ (float)in_data->downsample_y.den;
            miP->layerWidthF = PF_FpShort (in_data->width*miP->scale_x) ;
            miP->layerHeightF = PF_FpShort(in_data->height* miP->scale_y);
            //time params
            miP->layerTime_Sec = PF_FpShort(in_data->current_time)/PF_FpShort(in_data->time_scale);
            miP->layerTime_Frame = PF_FpShort(in_data->current_time/ (float)in_data->time_step);
            miP->layerDuration =PF_FpShort( in_data->total_time / in_data->time_scale);
            miP->inOneF    = var1_param.u.fs_d.value;
            miP->inTwoF    = var2_param.u.fs_d.value;
            miP->inThreeF= var3_param.u.fs_d.value;
            miP->inFourF    =  var4_param.u.fs_d.value;

            //user param points
            miP->pointOneX = static_cast<PF_FpShort>(round(FIX_2_FLOAT(point1_param.u.td.x_value)));
            miP->pointOneY =static_cast<PF_FpShort>(round(FIX_2_FLOAT(point1_param.u.td.y_value)));
            miP->pointTwoX = static_cast<PF_FpShort>(round(FIX_2_FLOAT( point2_param.u.td.x_value)));
            miP->pointTwoY =static_cast<PF_FpShort>(round(FIX_2_FLOAT( point2_param.u.td.y_value)));


            miP->xLF = 0;
            miP->yLF = 0;

            //CONVERT COLOR PARAMS TO FLOAT BYSMART WAY
            PF_PixelFloat tempFloat1, tempFloat2;
            ERR(suites.ColorParamSuite1()->PF_GetFloatingPointColorFromColorDef(in_data->effect_ref, & color1_param, &tempFloat1));
            ERR(suites.ColorParamSuite1()->PF_GetFloatingPointColorFromColorDef(in_data->effect_ref, & color2_param, &tempFloat2));

            //user param color
            miP->colorOne[0] = tempFloat1.red;
            miP->colorOne[1] = tempFloat1.green;
            miP->colorOne[2] = tempFloat1.blue;

            miP->colorTwo[0] =  tempFloat2.red;
            miP->colorTwo[1] = tempFloat2.green;
            miP->colorTwo[2] = tempFloat2.blue;

            //CALL SEQP
			std::string redExprStr, greenExprStr, blueExprStr, alphaExprStr, funcOneStr, funcTwoStr, funcThreeStr, frag1str;
            if (seqP && !err) {
                redExprStr = seqP->redExAc;
				greenExprStr = seqP->greenExAc;
				blueExprStr = seqP->blueExAc;
				alphaExprStr = seqP->alphaExAc;
				frag1str = seqP->Glsl_FragmentShAc;

				flagsP.PixelsCallExternalInputB = seqP->PixelsCallExternalInputB;
				flagsP.PresetHasWideInput = seqP->PresetHasWideInputB;
				flagsP.NeedsPixelAroundB = seqP->NeedsPixelAroundB;
				flagsP.NeedsLumaB = seqP->NeedsLumaB;
				flagsP.CallsAEGP_CompB = seqP->CallsAEGP_CompB;
				flagsP.CallsAEGP_layerB = seqP->CallsAEGP_layerB;
				flagsP.parserModeB = seqP->parserModeB;
            }

			ExprP.redstr = &redExprStr;
			ExprP.greenstr = &greenExprStr;
			ExprP.bluestr = &blueExprStr;
			ExprP.alphastr = &alphaExprStr;
			ExprP.frag1str  = &frag1str;
			ExprP.vertexstr = &glvertstr;
			ExprP.frag2str  = &glfrag2str;

            //CALL EXTERNAL LAYER AND TRANSFORM WORLD IF NEEDED
		
			if (flagsP.PixelsCallExternalInputB) {
				oiP.x_offFi = miP->x_offFi;
				oiP.y_offFi = miP->y_offFi;
				
				ERR(ExtLayerInput((void*)&oiP,
					in_data,
					inputP,
					extLP,
					&extLW,
					suites,
					format));
			}

            //CALL PARSER MODE
			if (arbP && !err && flagsP.parserModeB == false) {
				ERR(ExprRender(out_data, format, inputP, outputP, &extLW, suites,
					(void*)miP,
					(void*)&flagsP,
					(void*)&ExprP));
				
			}
            // CALL GLSL
			else if (arbP && !err){
				ERR(Render_GLSL(in_data,
					out_data,
					inputP,
					outputP,
					&extLW,
					format,
					suites,
					(void*)miP,
					seqP->ShaderResetB,
					*ExprP.vertexstr,
					*ExprP.frag1str,
					*ExprP.frag2str));
			}

            //CHECKIN PARAMS
			PF_UNLOCK_HANDLE(arbH);
            ERR2(PF_CHECKIN_PARAM(in_data, &setup_param));
            ERR2(PF_CHECKIN_PARAM(in_data, &arb_param));
            ERR2(PF_CHECKIN_PARAM(in_data, &var1_param));
            ERR2(PF_CHECKIN_PARAM(in_data, &var2_param));
            ERR2(PF_CHECKIN_PARAM(in_data, &var3_param));
            ERR2(PF_CHECKIN_PARAM(in_data, &var4_param));
            ERR2(PF_CHECKIN_PARAM(in_data, &point1_param));
            ERR2(PF_CHECKIN_PARAM(in_data, &point2_param));
            ERR2(PF_CHECKIN_PARAM(in_data, &color1_param));
            ERR2(PF_CHECKIN_PARAM(in_data, &color2_param));
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



static PF_Err
GlobalSetdown(
	PF_InData		*in_data,
	PF_OutData		*out_data,
	PF_ParamDef		*params[],
	PF_LayerDef		*output)
{
	PF_Err			err = PF_Err_NONE;
    AEGP_SuiteHandler	suites(in_data->pica_basicP);
    
    if (in_data->global_data) {
        suites.HandleSuite1()->host_dispose_handle(in_data->global_data);
    }
	try
	{
		// always restore back AE's own OGL context
		SaveRestoreOGLContext oSavedContext;

		S_mutex.lock();
		S_render_contexts.clear();
		S_mutex.unlock();

		//OS specific unloading
		AESDK_OpenGL_Shutdown(*S_GLator_EffectCommonData.get());
		S_GLator_EffectCommonData.reset();

		if (in_data->sequence_data) {
			PF_DISPOSE_HANDLE(in_data->sequence_data);
			out_data->sequence_data = NULL;
		}
	}
	catch (PF_Err& thrown_err)
	{
		err = thrown_err;
	}
	catch (...)
	{
		err = PF_Err_OUT_OF_MEMORY;
	}

	return err;
    
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
    seqDataP            seqP = reinterpret_cast<seqDataP>(DH(out_data->sequence_data));
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
    
	

	if (seqP && !err) {
		
		if (seqP->PresetHasWideInputB) {
			out_data->out_flags &= ~PF_OutFlag_WIDE_TIME_INPUT;
            out_data->out_flags2  &= ~PF_OutFlag2_AUTOMATIC_WIDE_TIME_INPUT;
		}
		else {
			out_data->out_flags &= PF_OutFlag_WIDE_TIME_INPUT;
			out_data->out_flags2 &= PF_OutFlag2_AUTOMATIC_WIDE_TIME_INPUT;
		}
        if (seqP->NeedsPixelAroundB){
			out_data->out_flags &= ~PF_OutFlag_PIX_INDEPENDENT;
		}
		else {
			out_data->out_flags |= PF_OutFlag_PIX_INDEPENDENT;
			
		}
		

	}

    ERR2(PF_CHECKIN_PARAM(in_data, &arb_param));

	return err;
}


static PF_Err
SequenceSetdown (
                 PF_InData        *in_data,
                 PF_OutData        *out_data)
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
               PF_InData        *in_data,
               PF_OutData        *out_data)
{
    PF_Err err = PF_Err_NONE;
    AEGP_SuiteHandler suites(in_data->pica_basicP);

    err = SequenceSetdown(in_data, out_data);

    if (!err){
        PF_Handle    seq_dataH =    suites.HandleSuite1()->host_new_handle(sizeof(seqData));

        if (seq_dataH){


                out_data->sequence_data = seq_dataH;
                suites.HandleSuite1()->host_unlock_handle(seq_dataH);
        } else {    // whoa, we couldn't allocate sequence data; bail!
            err = PF_Err_OUT_OF_MEMORY;
        }
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
                err = GlobalSetdown(in_data,
									out_data,
									params,
									output);
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

            case PF_Cmd_SEQUENCE_SETUP:
                err = SequenceSetup(in_data,out_data);
                break;

            case PF_Cmd_SEQUENCE_SETDOWN:
                err = SequenceSetdown(in_data,out_data);
                break;

            case PF_Cmd_SEQUENCE_RESETUP:
                err = SequenceSetup(in_data,out_data);
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

			case PF_Cmd_USER_CHANGED_PARAM:
				err = UserChangedParam(in_data,
					out_data,
					params,
					output,
					reinterpret_cast<const PF_UserChangedParamExtra *>(extra));
				break;

            case PF_Cmd_SMART_RENDER:
                err = SmartRender(    in_data,
                                  out_data,
                                  reinterpret_cast<PF_SmartRenderExtra*>(extra));
                break;

            case PF_Cmd_SMART_PRE_RENDER:
                err = PreRender(    in_data,
                                out_data,
                                reinterpret_cast<PF_PreRenderExtra*>(extra));
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

