#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <GLES3/gl3.h>
#include <EGL/egl.h>
#include "esUtil_win.h"


// CreateEGLContext()
//Creates an EGL rendering context and all associated elements

EGLBoolean CreateEGLContext ( EGLNativeWindowType hWnd, EGLDisplay* eglDisplay,
                              EGLContext* eglContext, EGLSurface* eglSurface,
                              EGLint attribList[])
{
   EGLint numConfigs;
   EGLint majorVersion;
   EGLint minorVersion;
   EGLDisplay display;
   EGLContext context;
   EGLSurface surface;
   EGLConfig config;
   EGLint contextAttribs[] = { EGL_CONTEXT_CLIENT_VERSION, 3, EGL_NONE, EGL_NONE };

   // Get Display
   display = eglGetDisplay(GetDC(hWnd));
   if ( display == EGL_NO_DISPLAY )
   {
      return EGL_FALSE;
   }

   // Initialize EGL
   if ( !eglInitialize(display, &majorVersion, &minorVersion) )
   {
      return EGL_FALSE;
   }

   // Get configs
   if ( !eglGetConfigs(display, NULL, 0, &numConfigs) )
   {
      return EGL_FALSE;
   }

   // Choose config
   if ( !eglChooseConfig(display, attribList, &config, 1, &numConfigs) )
   {
      return EGL_FALSE;
   }

   // Create a surface
   surface = eglCreateWindowSurface(display, config, (EGLNativeWindowType)hWnd, NULL);
   if ( surface == EGL_NO_SURFACE )
   {
      return EGL_FALSE;
   }

   // Create a GL context
   context = eglCreateContext(display, config, EGL_NO_CONTEXT, contextAttribs );
   if ( context == EGL_NO_CONTEXT )
   {
      return EGL_FALSE;
   }   
   
   // Make the context current
   if ( !eglMakeCurrent(display, surface, surface, context) )
   {
      return EGL_FALSE;
   }
   
   *eglDisplay = display;
   *eglSurface = surface;
   *eglContext = context;
   return EGL_TRUE;
} 

//  esInitContext()
//
//      Initialize ES utility context.  This must be called before calling any other
//      functions.

void ESUTIL_API esInitContext ( ESContext *esContext )
{
   if ( esContext != NULL )
   {
      memset( esContext, 0, sizeof( ESContext) );
   }
}


//  esCreateWindow()
//
//      title - name for title bar of window
//      width - width of window to create
//      height - height of window to create
//      flags  - bitwise or of window creation flags 
//          ES_WINDOW_ALPHA       - specifies that the framebuffer should have alpha
//          ES_WINDOW_DEPTH       - specifies that a depth buffer should be created
//          ES_WINDOW_STENCIL     - specifies that a stencil buffer should be created
//          ES_WINDOW_MULTISAMPLE - specifies that a multi-sample buffer should be created

GLboolean ESUTIL_API esCreateWindow ( ESContext *esContext, const char* title, GLint width, GLint height, GLuint flags )
{
   EGLint attribList[] =
   {
       EGL_RED_SIZE,       5,
       EGL_GREEN_SIZE,     6,
       EGL_BLUE_SIZE,      5,
       EGL_ALPHA_SIZE,     (flags & ES_WINDOW_ALPHA) ? 8 : EGL_DONT_CARE,
       EGL_DEPTH_SIZE,     (flags & ES_WINDOW_DEPTH) ? 8 : EGL_DONT_CARE,
       EGL_STENCIL_SIZE,   (flags & ES_WINDOW_STENCIL) ? 8 : EGL_DONT_CARE,
       EGL_SAMPLE_BUFFERS, (flags & ES_WINDOW_MULTISAMPLE) ? 1 : 0,
       EGL_NONE
   };
   
   if ( esContext == NULL )
   {
      return GL_FALSE;
   }

   esContext->width = width;
   esContext->height = height;

   if ( !WinCreate ( esContext, title) )
   {
      return GL_FALSE;
   }

   if ( !CreateEGLContext ( esContext->hWnd,
                            &esContext->eglDisplay,
                            &esContext->eglContext,
                            &esContext->eglSurface,
                            attribList) )
   {
      return GL_FALSE;
   }
   
   return GL_TRUE;
}


//  esMainLoop()
//
//    Start the main loop for the OpenGL ES application

void ESUTIL_API esMainLoop ( ESContext *esContext )
{
   WinLoop ( esContext );
}



//  esRegisterDrawFunc()

void ESUTIL_API esRegisterDrawFunc ( ESContext *esContext, void (ESCALLBACK *drawFunc) (ESContext* ) )
{
   esContext->drawFunc = drawFunc;
}

//  esRegisterUpdateFunc()

void ESUTIL_API esRegisterUpdateFunc ( ESContext *esContext, void (ESCALLBACK *updateFunc) ( ESContext*, float ) )
{
   esContext->updateFunc = updateFunc;
}


//  esRegisterKeyFunc()

void ESUTIL_API esRegisterKeyFunc ( ESContext *esContext,
                                    void (ESCALLBACK *keyFunc) (ESContext*, unsigned char, bool ) )
{
   esContext->keyFunc = keyFunc;
}
//MouseClick
void ESUTIL_API esRegisterMouseFunc(ESContext* esContext,
    void (ESCALLBACK* mouseFunc)(ESContext*, int, int, bool)) 
{
    esContext->mouseFunc = mouseFunc;
}
//MouseMove
void ESUTIL_API esRegisterMouseMoveFunc(ESContext* esContext,
    void (ESCALLBACK* mouseMoveFunc)(ESContext*, int, int))
{
    esContext->mouseMoveFunc = mouseMoveFunc;
}

// esLogMessage()
//    Log an error message to the debug output for the platform

void ESUTIL_API esLogMessage ( const char *formatStr, ... )
{
    va_list params;
    char buf[2048];

    va_start ( params, formatStr );
    vsprintf_s ( buf, sizeof(buf),  formatStr, params );
    
    printf ( "%s", buf );
	OutputDebugString(buf);
    
    va_end ( params );
}

