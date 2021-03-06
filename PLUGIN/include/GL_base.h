

/*
	GL_base.h
	Based on Adobe After Efffect SDK
*/

#pragma once

#ifndef GL_BASE_H
#define GL_BASE_H
#include "AEGP_SuiteHandler.h"
#include "AEConfig.h"


//OS specific includes
#ifdef AE_OS_WIN
	// mambo jambo with stdint
	#define _STDINT
	#define WIN32_LEAN_AND_MEAN 
	#define DVACORE_MATCH_MAC_STDINT_ON_WINDOWS
	#include <stdint.h>
	#ifndef INT64_MAX
		#define INT64_MAX       LLONG_MAX
	#endif
	#ifndef INTMAX_MAX
		#define INTMAX_MAX       INT64_MAX
	#endif

	#include <windows.h>
	#include <stdlib.h>

	#if (_MSC_VER >= 1400)
		#define THREAD_LOCAL __declspec(thread)
	#endif
#else
	#define THREAD_LOCAL __thread
#endif

// OpenGL 3.3 bindings (see https://github.com/hpicgs/glbinding)
#include "glbinding/gl33core/gl.h"

#ifdef AE_OS_MAC
	#import <Cocoa/Cocoa.h>
#endif

//general includes
#include <string>
#include <fstream>
#include <memory>
#include <set>

//typedefs
typedef unsigned char		u_char;
typedef unsigned short		u_short;
typedef unsigned short		u_int16;
typedef unsigned long		u_long;
typedef short int			int16;

namespace AESDK_OpenGL
{

/*
// Global (to the effect) supporting OpenGL contexts
*/

struct AESDK_OpenGL_EffectCommonData
{
	AESDK_OpenGL_EffectCommonData();
	virtual ~AESDK_OpenGL_EffectCommonData();

	// must surround plug-in OpenGL calls with these functions so that AE
	// doesn't know we're borrowing the OpenGL renderer
	void SetPluginContext();

	bool mInitialized;
	std::set<gl::GLextension> mExtensions;

	//OS specific handles
#ifdef AE_OS_WIN
	HWND	mHWnd;
	HDC		mHDC;
	HGLRC	mHRC;
	std::string mClassName;
#endif
#ifdef AE_OS_MAC
	CGLContextObj		mRC;
	NSOpenGLContext*    mNSOpenGLContext;
#endif
};

typedef std::shared_ptr<AESDK_OpenGL_EffectCommonData> AESDK_OpenGL_EffectCommonDataPtr;

/*
// Per render/thread supporting OpenGL variables
*/

struct AESDK_OpenGL_EffectRenderData : public AESDK_OpenGL_EffectCommonData
{
	AESDK_OpenGL_EffectRenderData();
	virtual ~AESDK_OpenGL_EffectRenderData();

	gl::GLuint mFrameBufferSu;
	gl::GLuint mColorRenderBufferSu;

	u_int16 mRenderBufferWidthSu;
	u_int16 mRenderBufferHeightSu;

	gl::GLuint mProgramObjSu;
	gl::GLuint mProgramObj2Su;

	gl::GLuint mOutputFrameTexture; //pbo texture

	gl::GLuint vao;
	gl::GLuint quad;
};

typedef std::shared_ptr<AESDK_OpenGL_EffectRenderData> AESDK_OpenGL_EffectRenderDataPtr;

enum AESDK_OpenGL_Err
{
	AESDK_OpenGL_OK = 0,
	AESDK_OpenGL_OS_Load_Err,
	AESDK_OpenGL_OS_Unload_Err,
	AESDK_OpenGL_Res_Load_Err,
	AESDK_OpenGL_Res_Unload_Err,
	AESDK_OpenGL_Extensions_Err,
	AESDK_OpenGL_ShaderInit_Err,
	AESDK_OpenGL_Unknown_Err
};

enum { PositionSlot, UVSlot };

/*
// Core functions
*/
void AESDK_OpenGL_Startup(AESDK_OpenGL_EffectCommonData& inData, const AESDK_OpenGL_EffectCommonData* inRootContext = nullptr);
void AESDK_OpenGL_Shutdown(AESDK_OpenGL_EffectCommonData& inData);

void AESDK_OpenGL_InitResources(AESDK_OpenGL_EffectRenderData& inData, PF_OutData *out_data, AEGP_SuiteHandler &suites, u_short inBufferWidth, u_short inBufferHeight,  const std::string& vert1Str, const std::string& frag1Str, const std::string& frag2Str);
void AESDK_OpenGL_MakeReadyToRender(AESDK_OpenGL_EffectRenderData& inData, gl::GLuint textureHandle);
void AESDK_OpenGL_InitShader(gl::GLuint *ObjSu, PF_OutData *out_data, AEGP_SuiteHandler &suites, std::string inVertexShaderStr, std::string inFragmentShaderStr);
void AESDK_OpenGL_BindTexture0ToTarget(gl::GLuint program, gl::GLint inTexture, std::string inTargetName);
void AESDK_OpenGL_BindTexture1ToTarget(gl::GLuint program, gl::GLint inTexture, std::string inTargetName);
void AESDK_OpenGL_BindTexture2ToTarget(gl::GLuint program, gl::GLint inTexture, std::string inTargetName);
void AESDK_OpenGL_BindTexture3ToTarget(gl::GLuint program, gl::GLint inTexture, std::string inTargetName);
void AESDK_OpenGL_BindTexture4ToTarget(gl::GLuint program, gl::GLint inTexture, std::string inTargetName);

/*
// Independent macros and helper functions
*/

//GetProcAddress
#ifdef AE_OS_WIN
	#define GetProcAddress(N) wglGetProcAddress((LPCSTR)N)
#elif defined(AE_OS_MAC)
	#define GetProcAddress(N) NSGLGetProcAddress(N)
#endif

//helper function - error reporting util
std::string ReportError(AESDK_OpenGL_Err inError);
//helper function - check frame buffer status before final render call
std::string CheckFramebufferStatus();



/*
//	Error class and macros used to trap errors
*/
class AESDK_OpenGL_Fault 
{
public:
	AESDK_OpenGL_Fault(AESDK_OpenGL_Err inError)
	{
		mError = inError;
	}
	operator AESDK_OpenGL_Err()
	{
		return mError;
	}

protected:
	AESDK_OpenGL_Err mError;
};

#define GL_CHECK(err) {AESDK_OpenGL_Err err1 = err; if (err1 != AESDK_OpenGL_OK ){ throw AESDK_OpenGL_Fault(err1);}}

class SaveRestoreOGLContext
{
public:
	SaveRestoreOGLContext();
	~SaveRestoreOGLContext();

private:
#ifdef AE_OS_MAC
	CGLContextObj    o_RC;
	NSOpenGLContext* pNSOpenGLContext_;
#endif
#ifdef AE_OS_WIN
	HDC   h_DC; /// Device context handle
	HGLRC h_RC; /// Handle to an OpenGL rendering context
#endif

	SaveRestoreOGLContext(const SaveRestoreOGLContext &);
	SaveRestoreOGLContext &operator=(const SaveRestoreOGLContext &);
};
};
#endif // GL_BASE_H
