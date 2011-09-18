/*
 * CXScreenSaver.cpp
 *
 *  Created on: 25.09.2009
 *      Author: svistunov
 */

#include "CXScreenSaver.h"
#include <time.h>

CXScreenSaver::CXScreenSaver() {
	_UseRootWindow = false;
	_UseDRI = false;
	_Dpy = NULL;
	_Window = 0;
	_XVI = NULL;
	_Context = NULL;
}

CXScreenSaver::~CXScreenSaver() {
	if (_Dpy != NULL) {
		if (_UseRootWindow && _Window != 0)
			XDestroyWindow(_Dpy, _Window);

		if (_XVI != NULL)
			XFree(_XVI);

		if (_Context != NULL)
			glXDestroyContext(_Dpy, _Context);

		XCloseDisplay(_Dpy);
	};
}

bool CXScreenSaver::Init(int ArgC, char * ArgV[]) {
	openlog(ArgV[0], LOG_PID, LOG_USER);

	srand(time(0));

	for (int i=0; i<ArgC; i++) {
		if (strcmp(ArgV[i], "-root") == 0)
			_UseRootWindow = true;
		if (strcmp(ArgV[i], "-dri") == 0)
			_UseDRI = true;
	};

	_Dpy = XOpenDisplay(NULL);
	if (_Dpy == NULL) {
		syslog(LOG_INFO, "Cannot open display");
		return false;
	};

	if (!glXQueryExtension(_Dpy, NULL, NULL)) {
		syslog(LOG_INFO, "X server haven't GLX");
		return false;
	};

	if (!(_UseRootWindow ? _InitWindow() : _CreateWindow()))
		return false;

	if (!OnInit(ArgC, ArgV))
		return false;

	_Context = glXCreateContext(_Dpy, _XVI, NULL, _UseDRI);
	if (_Context == NULL) {
		syslog(LOG_INFO, "Cannot create GL context");
		return false;
	};

	if (!glXMakeCurrent(_Dpy, _Window, _Context)) {
		syslog(LOG_INFO, "Cannot make current context");
		return false;
	};

	if (glewInit() != GLEW_OK) {
		syslog(LOG_INFO, "Cannot init glew");
		return false;
	};

	GLInit();

	XMapWindow(_Dpy, _Window);

	return true;
}

void CXScreenSaver::Run() {
	XWindowAttributes WA;
	XGetWindowAttributes(_Dpy, _Window, &WA);
	OnResize(WA.width, WA.height);

	XSelectInput(_Dpy, _Window, ExposureMask | StructureNotifyMask );

	XEvent Event;
	if (GLXEW_SGI_swap_control)
		glXSwapIntervalSGI(2);

	while (1) {
		_Draw();

		while (XPending(_Dpy)) {
			XNextEvent(_Dpy, &Event);

			switch (Event.type) {
				case Expose:
					break;
				case ConfigureNotify:
					OnResize(Event.xconfigure.width, Event.xconfigure.height);
					break;
				case DestroyNotify:
					return;
			};
		};
	};
}

// Protected methods
void CXScreenSaver::GLInit(void) {
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glEnable(GL_DEPTH_TEST);
}

bool CXScreenSaver::OnInit(int ArgC, char * ArgV[]) {
	return true;
}

void CXScreenSaver::OnResize(int Width, int Height) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	glViewport(0, 0, Width, Height);
	glOrtho(-1, 1, -1, 1, -5, 5);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

};

void CXScreenSaver::OnDraw(void) {

};

// Private methods

bool CXScreenSaver::_InitWindow(void) {
	const char * XSS_Window = getenv("XSCREENSAVER_WINDOW");
	if (XSS_Window == NULL) {
		syslog(LOG_INFO, "XSCREENSAVER_WINDOW not set");
		return false;
	};

	if (sscanf(XSS_Window, "0x%lx", (long unsigned int*)&_Window) != 1) {
		syslog(LOG_INFO, "Cannot read XSCREENSAVER_WINDOW from env. Value: %s", XSS_Window);
		return false;
	};

	XWindowAttributes WA;
	Visual * V;
	XVisualInfo TempXVI;
	int Count;

	XGetWindowAttributes(_Dpy, _Window, &WA);
	V = WA.visual;

	TempXVI.screen = XDefaultScreen(_Dpy);
	TempXVI.visualid = XVisualIDFromVisual(V);

	_XVI = XGetVisualInfo(_Dpy, VisualScreenMask | VisualIDMask, &TempXVI, &Count);
	if (_XVI == NULL) {
		syslog(LOG_INFO, "Cannot get GL visual for window 0x%lx.", _Window);
	};

	return true;
}

bool CXScreenSaver::_CreateWindow(void) {
	int Attr[] = {	GLX_RGBA,
					GLX_RED_SIZE, 1,
					GLX_GREEN_SIZE, 1,
					GLX_BLUE_SIZE, 1,
					GLX_DEPTH_SIZE, 1,
					GLX_DOUBLEBUFFER,
					NULL
	};

	_XVI = glXChooseVisual(_Dpy, DefaultScreen(_Dpy), Attr);
	if (_XVI == NULL) {
		syslog(LOG_INFO, "Cannot find visual");
		return false;
	};

	XSetWindowAttributes WinAttr;
	WinAttr.event_mask = ExposureMask | StructureNotifyMask | KeyPressMask;
	WinAttr.colormap = XCreateColormap(_Dpy, RootWindow(_Dpy, _XVI->screen), _XVI->visual, AllocNone);
	WinAttr.border_pixel = 0;
	WinAttr.background_pixel = 0;

	_Window = XCreateWindow(_Dpy, RootWindow(_Dpy, DefaultScreen(_Dpy)),
			0, 0,
			640, 480,
			0,
			_XVI->depth,
			InputOutput,
			_XVI->visual,
			CWBorderPixel | CWColormap | CWEventMask,
			&WinAttr);

	return true;
}

void CXScreenSaver::_Draw(void) {
	OnDraw();
	glXSwapBuffers(_Dpy, _Window);
}
