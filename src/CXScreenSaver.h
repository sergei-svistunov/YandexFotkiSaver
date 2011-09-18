/*
 * CXScreenSaver.h
 *
 *  Created on: 25.09.2009
 *      Author: svistunov
 */

#ifndef CXSCREENSAVER_H_
#define CXSCREENSAVER_H_

#include "global.h"

#include "X11/Xlib.h"

class CXScreenSaver {
public:
	CXScreenSaver();
	virtual ~CXScreenSaver();
	bool Init(int ArgC, char * ArgV[]);
	void Run(void);
protected:
	virtual void GLInit(void);
	virtual bool OnInit(int ArgC, char * ArgV[]);
	virtual void OnResize(int Width, int Height);
	virtual void OnDraw(void);
	Display * _Dpy;
	Window _Window;
	XVisualInfo * _XVI;
	GLXContext _Context;
private:
	bool _InitWindow(void);
	bool _CreateWindow(void);
	void _Draw(void);
	bool _UseRootWindow;
	bool _UseDRI;
};

#endif /* CXSCREENSAVER_H_ */
