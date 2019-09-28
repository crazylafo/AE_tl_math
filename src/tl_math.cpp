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
		PF_InData    *in_data,
		PF_OutData    *out_data,
		A_long widthL,
		A_long heightL,
		gl::GLuint		inputFrameTexture,
		gl::GLuint	inputExtFrameTexture,
		void			*refcon,
		float			multiplier16bit)
	{
		MathInfo           *miP = reinterpret_cast<MathInfo*>(refcon);

		seqDataP seqP = reinterpret_cast<seqDataP>(DH(out_data->sequence_data));
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
		location = glGetUniformLocation(renderContext->mProgramObjSu, seqP->paramSlider01NameAc);
		glUniform1f(location, miP->inSliderF[0]);
		location = glGetUniformLocation(renderContext->mProgramObjSu, seqP->paramSlider02NameAc);
		glUniform1f(location, miP->inSliderF[1]);
		location = glGetUniformLocation(renderContext->mProgramObjSu, seqP->paramSlider03NameAc);
		glUniform1f(location, miP->inSliderF[2]);
		location = glGetUniformLocation(renderContext->mProgramObjSu, seqP->paramSlider04NameAc);
		glUniform1f(location, miP->inSliderF[3]);
		location = glGetUniformLocation(renderContext->mProgramObjSu, seqP->paramSlider05NameAc);
		glUniform1f(location, miP->inSliderF[4]);
		location = glGetUniformLocation(renderContext->mProgramObjSu, seqP->paramSlider06NameAc);
		glUniform1f(location, miP->inSliderF[5]);
		location = glGetUniformLocation(renderContext->mProgramObjSu, seqP->paramSlider07NameAc);
		glUniform1f(location, miP->inSliderF[6]);
		location = glGetUniformLocation(renderContext->mProgramObjSu, seqP->paramSlider08NameAc);
		glUniform1f(location, miP->inSliderF[7]);
		location = glGetUniformLocation(renderContext->mProgramObjSu, seqP->paramSlider09NameAc);
		glUniform1f(location, miP->inSliderF[8]);
		location = glGetUniformLocation(renderContext->mProgramObjSu, seqP->paramSlider10NameAc);
		glUniform1f(location, miP->inSliderF[9]);

		location = glGetUniformLocation(renderContext->mProgramObjSu, seqP->paramPoint01NameAc);
		glUniform3f(location, miP->inPoints[0].point[0], convertYCoordAEToGL(miP->inPoints[0].point[1], heightL), miP->inPoints[0].point[2]);

		location = glGetUniformLocation(renderContext->mProgramObjSu, seqP->paramPoint02NameAc);
		glUniform3f(location, miP->inPoints[1].point[0], convertYCoordAEToGL(miP->inPoints[1].point[1], heightL), miP->inPoints[1].point[2]);

		location = glGetUniformLocation(renderContext->mProgramObjSu, seqP->paramPoint03NameAc);
		glUniform3f(location, miP->inPoints[2].point[0], convertYCoordAEToGL(miP->inPoints[2].point[1], heightL), miP->inPoints[2].point[2]);

		location = glGetUniformLocation(renderContext->mProgramObjSu, seqP->paramPoint04NameAc);
		glUniform3f(location, miP->inPoints[3].point[0], convertYCoordAEToGL(miP->inPoints[3].point[1], heightL), miP->inPoints[3].point[2]);

		location = glGetUniformLocation(renderContext->mProgramObjSu, seqP->paramPoint05NameAc);
		glUniform3f(location, miP->inPoints[4].point[0], convertYCoordAEToGL(miP->inPoints[4].point[1], heightL), miP->inPoints[4].point[2]);

		location = glGetUniformLocation(renderContext->mProgramObjSu, seqP->paramPoint06NameAc);
		glUniform3f(location, miP->inPoints[5].point[0], convertYCoordAEToGL(miP->inPoints[5].point[1], heightL), miP->inPoints[5].point[2]);

		location = glGetUniformLocation(renderContext->mProgramObjSu, seqP->paramPoint07NameAc);
		glUniform3f(location, miP->inPoints[6].point[0], convertYCoordAEToGL(miP->inPoints[6].point[1], heightL), miP->inPoints[6].point[2]);

		location = glGetUniformLocation(renderContext->mProgramObjSu, seqP->paramPoint08NameAc);
		glUniform3f(location, miP->inPoints[7].point[0], convertYCoordAEToGL(miP->inPoints[7].point[1], heightL), miP->inPoints[7].point[2]);

		location = glGetUniformLocation(renderContext->mProgramObjSu, seqP->paramPoint09NameAc);
		glUniform3f(location, miP->inPoints[8].point[0], convertYCoordAEToGL(miP->inPoints[8].point[1], heightL), miP->inPoints[8].point[2]);

		location = glGetUniformLocation(renderContext->mProgramObjSu, seqP->paramPoint10NameAc);
		glUniform3f(location, miP->inPoints[9].point[0], convertYCoordAEToGL(miP->inPoints[9].point[1], heightL), miP->inPoints[9].point[2]);


		location = glGetUniformLocation(renderContext->mProgramObjSu, seqP->paramCb01NameAc);
		glUniform1f(location, miP->inCboxF[0]);
		location = glGetUniformLocation(renderContext->mProgramObjSu, seqP->paramCb02NameAc);
		glUniform1f(location, miP->inCboxF[1]);
		location = glGetUniformLocation(renderContext->mProgramObjSu, seqP->paramCb03NameAc);
		glUniform1f(location, miP->inCboxF[2]);
		location = glGetUniformLocation(renderContext->mProgramObjSu, seqP->paramCb04NameAc);
		glUniform1f(location, miP->inCboxF[3]);
		location = glGetUniformLocation(renderContext->mProgramObjSu, seqP->paramCb05NameAc);
		glUniform1f(location, miP->inCboxF[4]);
		location = glGetUniformLocation(renderContext->mProgramObjSu, seqP->paramCb06NameAc);
		glUniform1f(location, miP->inCboxF[5]);
		location = glGetUniformLocation(renderContext->mProgramObjSu, seqP->paramCb07NameAc);
		glUniform1f(location, miP->inCboxF[6]);
		location = glGetUniformLocation(renderContext->mProgramObjSu, seqP->paramCb08NameAc);
		glUniform1f(location, miP->inCboxF[7]);
		location = glGetUniformLocation(renderContext->mProgramObjSu, seqP->paramCb09NameAc);
		glUniform1f(location, miP->inCboxF[8]);
		location = glGetUniformLocation(renderContext->mProgramObjSu, seqP->paramCb10NameAc);
		glUniform1f(location, miP->inCboxF[9]);

		location = glGetUniformLocation(renderContext->mProgramObjSu, seqP->paramColor01NameAc);
		glUniform3f(location, miP->inColors[0].color[0], miP->inColors[0].color[1], miP->inColors[0].color[2]);

		location = glGetUniformLocation(renderContext->mProgramObjSu, seqP->paramColor02NameAc);
		glUniform3f(location, miP->inColors[1].color[0], miP->inColors[1].color[1], miP->inColors[1].color[2]);

		location = glGetUniformLocation(renderContext->mProgramObjSu, seqP->paramColor03NameAc);
		glUniform3f(location, miP->inColors[2].color[0], miP->inColors[2].color[1], miP->inColors[2].color[2]);

		location = glGetUniformLocation(renderContext->mProgramObjSu, seqP->paramColor04NameAc);
		glUniform3f(location, miP->inColors[3].color[0], miP->inColors[3].color[1], miP->inColors[3].color[2]);

		location = glGetUniformLocation(renderContext->mProgramObjSu, seqP->paramColor05NameAc);
		glUniform3f(location, miP->inColors[4].color[0], miP->inColors[4].color[1], miP->inColors[4].color[2]);

		location = glGetUniformLocation(renderContext->mProgramObjSu, seqP->paramColor06NameAc);
		glUniform3f(location, miP->inColors[5].color[0], miP->inColors[5].color[1], miP->inColors[5].color[2]);

		location = glGetUniformLocation(renderContext->mProgramObjSu, seqP->paramColor07NameAc);
		glUniform3f(location, miP->inColors[6].color[0], miP->inColors[6].color[1], miP->inColors[6].color[2]);

		location = glGetUniformLocation(renderContext->mProgramObjSu, seqP->paramColor08NameAc);
		glUniform3f(location, miP->inColors[7].color[0], miP->inColors[7].color[1], miP->inColors[7].color[2]);

		location = glGetUniformLocation(renderContext->mProgramObjSu, seqP->paramColor09NameAc);
		glUniform3f(location, miP->inColors[8].color[0], miP->inColors[8].color[1], miP->inColors[8].color[2]);

		location = glGetUniformLocation(renderContext->mProgramObjSu, seqP->paramColor10NameAc);
		glUniform3f(location, miP->inColors[9].color[0], miP->inColors[9].color[1], miP->inColors[9].color[2]);

		location = glGetUniformLocation(renderContext->mProgramObjSu, seqP->paramRot01NameAc);
		glUniform1f(location, miP->inRotF[0]);
		location = glGetUniformLocation(renderContext->mProgramObjSu, seqP->paramRot02NameAc);
		glUniform1f(location, miP->inRotF[1]);
		location = glGetUniformLocation(renderContext->mProgramObjSu, seqP->paramRot03NameAc);
		glUniform1f(location, miP->inRotF[2]);
		location = glGetUniformLocation(renderContext->mProgramObjSu, seqP->paramRot04NameAc);
		glUniform1f(location, miP->inRotF[3]);
		location = glGetUniformLocation(renderContext->mProgramObjSu, seqP->paramRot05NameAc);
		glUniform1f(location, miP->inRotF[4]);
		location = glGetUniformLocation(renderContext->mProgramObjSu, seqP->paramRot06NameAc);
		glUniform1f(location, miP->inRotF[5]);
		location = glGetUniformLocation(renderContext->mProgramObjSu, seqP->paramRot07NameAc);
		glUniform1f(location, miP->inRotF[6]);
		location = glGetUniformLocation(renderContext->mProgramObjSu, seqP->paramRot08NameAc);
		glUniform1f(location, miP->inRotF[7]);
		location = glGetUniformLocation(renderContext->mProgramObjSu, seqP->paramRot09NameAc);
		glUniform1f(location, miP->inRotF[8]);
		location = glGetUniformLocation(renderContext->mProgramObjSu, seqP->paramRot10NameAc);
		glUniform1f(location, miP->inRotF[9]);

		location = glGetUniformLocation(renderContext->mProgramObjSu, seqP->time_secNameAc);
		glUniform1f(location, miP->layerTime_Sec);

		location = glGetUniformLocation(renderContext->mProgramObjSu, seqP->time_frameNameAc);
		glUniform1f(location, miP->layerTime_Frame);
		location = glGetUniformLocation(renderContext->mProgramObjSu, seqP->resolutionNameAc);
		glUniform2f(location, miP->layerSizeF[0], miP->layerSizeF[1]);
		location = glGetUniformLocation(renderContext->mProgramObjSu, seqP->frame_rateNameAc);
		glUniform1f(location, miP->compFpsF);
		location = glGetUniformLocation(renderContext->mProgramObjSu, seqP->layerPositionNameAc);
		glUniform3f(location, miP->layerPos.point[0], convertYCoordAEToGL(miP->layerPos.point[1], heightL), miP->layerPos.point[2]);
		location = glGetUniformLocation(renderContext->mProgramObjSu, seqP->layerScaleNameAc);
		glUniform3f(location, miP->layerScale.point[0], convertYCoordAEToGL( miP->layerScale.point[1], heightL), miP->layerScale.point[2]);
		location = glGetUniformLocation(renderContext->mProgramObjSu, seqP->compResolutionNameAc);
		glUniform2f(location, miP->compSizeF[0], miP->compSizeF[1]);
		location = glGetUniformLocation(renderContext->mProgramObjSu, seqP->cameraPosNameAc);
		glUniform3f(location, miP->cameraPos.point[0], convertYCoordAEToGL(miP->cameraPos.point[1], heightL), miP->cameraPos.point[2]);

		location = glGetUniformLocation(renderContext->mProgramObjSu, seqP->cameraTargetNameAc);
		glUniform3f(location, miP->cameraTarget.point[0], convertYCoordAEToGL(miP->cameraTarget.point[1], heightL), miP->cameraTarget.point[2]);

		location = glGetUniformLocation(renderContext->mProgramObjSu, seqP->cameraRotationNameAc);
		glUniform3f(location, miP->cameraRotation.point[0], convertYCoordAEToGL(miP->cameraRotation.point[1], heightL), miP->cameraRotation.point[2]);

		location = glGetUniformLocation(renderContext->mProgramObjSu, seqP->cameraZoomNameAc);
		glUniform1f(location, miP->cameraZoom);

		location = glGetUniformLocation(renderContext->mProgramObjSu, "multiplier16bit");
		glUniform1f(location, multiplier16bit);
		// Identify the texture to use and bind it to texture unit 0
		AESDK_OpenGL_BindTexture0ToTarget(renderContext->mProgramObjSu, inputFrameTexture, seqP->paramLayer00NameAc);
		AESDK_OpenGL_BindTexture1ToTarget(renderContext->mProgramObjSu, inputExtFrameTexture, seqP->paramLayer01NameAc);
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
                            PF_OutFlag2_I_USE_3D_CAMERA   |
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


std::string
evalMathExprStr (std::string expr,
                 seqDataP   *seqP)
{
    PF_Boolean returnExprErrB = false;
    std::string errReturn = "Error \n";
    MathInfoP miP;
    funcTransfertInfoP fiP;
    fiP.evalExpr = parseExpr<PF_FpShort>((void*)&miP, &fiP, expr, *seqP);
    if (fiP.hasErrorB)
    {
        fiP.channelErrorstr = " expression error";
        returnExprErrB = true;
        errReturn.append(fiP.channelErrorstr).append(": ").append(fiP.errorstr).append("\n");
    }
    else {
        errReturn = compile_success;
        
    }
    return errReturn;
}

void
evalVertShader(std::string inVertShaderStr, std::string& errReturn)
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

    GLint vertCompiledB =0;
    const char *vertShaderStringsP = (const GLchar *) inVertShaderStr.c_str();
    GLuint vertShaderSu = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertShaderSu, 1, &vertShaderStringsP, NULL);
    glCompileShader(vertShaderSu);
    glGetShaderiv(vertShaderSu, GL_COMPILE_STATUS, &vertCompiledB);
    if (!vertCompiledB) {
        char str[4096];
        glGetShaderInfoLog(vertShaderSu, sizeof(str), NULL, str);
        errReturn = str;
    }
    else {
        errReturn = compile_success;
    }
    glDeleteShader(vertShaderSu);
    glFlush();
}

void
evalFragShader(std::string inFragmentShaderStr, std::string& errReturn)
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
    glFlush();
}
PF_Err
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

		A_long	widthL = miP->layerSizeF[0]; // inputP->width;
		A_long	heightL = miP->layerSizeF[1]; //inputP->height;


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
		RenderGL(renderContext, in_data, out_data, widthL, heightL, inputFrameTexture, inputExtFrameTexture,(void*)miP, multiplier16bit);

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
    PF_Err 	err  = PF_Err_NONE;
           //err2  = PF_Err_NONE;

	if (seqP && !err) {
        if (seqP->cameraB ){
            out_data->out_flags2 &= ~PF_OutFlag2_I_USE_3D_CAMERA;
        }else{
            out_data->out_flags2 &= PF_OutFlag2_I_USE_3D_CAMERA;
        }
		if (seqP->presetHasWideInputB) {
			out_data->out_flags &= ~PF_OutFlag_WIDE_TIME_INPUT;
            out_data->out_flags2  &= ~PF_OutFlag2_AUTOMATIC_WIDE_TIME_INPUT;
		}
		else {
			out_data->out_flags &= PF_OutFlag_WIDE_TIME_INPUT;
			out_data->out_flags2 &= PF_OutFlag2_AUTOMATIC_WIDE_TIME_INPUT;
		}
        if (seqP->needsPixelAroundB){
			out_data->out_flags &= ~PF_OutFlag_PIX_INDEPENDENT;
		}
		else {
			out_data->out_flags |= PF_OutFlag_PIX_INDEPENDENT;
			
		}
		

	}



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
			seqData  	*seqP = reinterpret_cast<seqData*>(suites.HandleSuite1()->host_lock_handle(seq_dataH));
				seqP->initializedB = false;
				copyFromArbToSeqData(in_data, out_data, defaultArb, seqP);
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
                
                err =tlmath_ParamsSetup (	in_data,
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
				err = tlmath_UserChangedParam(in_data,
					out_data,
					params,
					output,
					reinterpret_cast<const PF_UserChangedParamExtra *>(extra));
				break;

            case PF_Cmd_SMART_RENDER:
                err = tl_math_SmartRender(    in_data,
                                  out_data,
                                  reinterpret_cast<PF_SmartRenderExtra*>(extra));
                break;

            case PF_Cmd_SMART_PRE_RENDER:
                err = tl_math_PreRender(    in_data,
                                out_data,
                                reinterpret_cast<PF_PreRenderExtra*>(extra));
                break;
				
            case PF_Cmd_UPDATE_PARAMS_UI:
                err = tlmath_UpdateParameterUI(	in_data,
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

