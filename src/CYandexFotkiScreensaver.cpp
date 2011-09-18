/*
 * CYandexFotkiScreensaver.cpp
 *
 *  Created on: 25.09.2009
 *      Author: svistunov
 */

#include "CYandexFotkiScreensaver.h"

#include <sys/stat.h>

#include "StringsCP1251.h"

CYandexFotkiScreensaver::CYandexFotkiScreensaver() {
	char * HomeFolder = getenv("HOME");
	if (!HomeFolder)
		HomeFolder = (char*)"/tmp";

	_Folder = new char[strlen(HomeFolder) + 20];
	sprintf(_Folder, "%s/.YandexFotkiSaver", HomeFolder);
	mkdir(_Folder, S_IRWXU | S_IRWXG | S_IREAD);

	_PhotoFolder = new char[strlen(_Folder) + 11];
	sprintf(_PhotoFolder, "%s/.download", _Folder);
	mkdir(_PhotoFolder, S_IRWXU | S_IRWXG | S_IREAD);


	char * ConfigFName = new char[strlen(_Folder) + 11];
	sprintf(ConfigFName, "%s/saver.cfg", _Folder);
	_Config = new CConfig();
	_Config->Load(ConfigFName);
	delete[] ConfigFName;

	pthread_mutex_init(&_LockDraw, NULL);
}

CYandexFotkiScreensaver::~CYandexFotkiScreensaver() {
	delete[] _Folder;
	delete[] _PhotoFolder;

	delete _Config;

	delete _Logo;
	delete _PhotoSpriteLoader;

	pthread_mutex_destroy(&_LockDraw);
}

void CYandexFotkiScreensaver::GLInit(void) {
	CXScreenSaver::GLInit();

	glClearColor(_Config->GetFloat("background-r", 0.5f), _Config->GetFloat("background-g", 0.5f), _Config->GetFloat("background-b", 0.5f), 1.0f);

	XFontStruct *Font = NULL;
	_FontList = glGenLists(256);
	Font = XLoadQueryFont(_Dpy, "-*-arial-bold-r-normal-*-17-*-*-*-*-*-*-cp1251");
	if (Font == NULL)
		Font = XLoadQueryFont(_Dpy, "-*-*-*-*-*-*-*-*-*-*-*-*-*-cp1251");
	if (Font == NULL)
		Font = XLoadQueryFont(_Dpy, "fixed");
	if (Font == NULL) {
		syslog(LOG_INFO, "Cannot load font");
		exit(0);
	};

	glXUseXFont(Font->fid, 0, 255, _FontList);
	XFreeFont(_Dpy, Font);

	_Logo = new CLogo();
	_PhotoSpriteLoader = new CPhotoSpriteLoader(_Folder, _PhotoFolder, &_LockDraw, _Config);
}

void CYandexFotkiScreensaver::OnDraw(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_ACCUM_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	glPushMatrix();

	switch (_PhotoSpriteLoader->GetStatus()) {
	case 1:
	case 3:
		_DrawDownload();
		break;
	case 2:
		_DrawPhotos();
		break;
	};

	glPopMatrix();
}

void CYandexFotkiScreensaver::_DrawDownload(void) {
	pthread_mutex_lock(&_LockDraw);

	glPushAttrib(GL_ALL_ATTRIB_BITS);
	GLfloat R = (GLfloat)rand() / RAND_MAX * 0.9f;
	glColor3f(1.0f, 1.0f - 1.0f * R, 1.0f - 1.0f * R);
	glListBase(_FontList);
	glRasterPos2f(-0.1f, 0.0f);
	if (_PhotoSpriteLoader->GetStatus() == 1)
		glCallLists(strlen(STR_LOAD_PHOTOS), GL_UNSIGNED_BYTE, STR_LOAD_PHOTOS);
	else
		glCallLists(strlen(STR_LOAD_PHOTOS_ERR), GL_UNSIGNED_BYTE, STR_LOAD_PHOTOS_ERR);
	glPopAttrib();

	_Logo->Draw();

	pthread_mutex_unlock(&_LockDraw);
}

void CYandexFotkiScreensaver::_DrawPhotos(void) {
	if (_PhotoSpriteQueue.size() == 0)
		_PhotoSpriteQueue.push(_PhotoSpriteLoader->GetPhotoSprite());

	if (_PhotoSpriteQueue.back()->IsCompleted()) {
		_PhotoSpriteQueue.push(_PhotoSpriteLoader->GetPhotoSprite());
		if (_PhotoSpriteQueue.size() > 2) {
			delete _PhotoSpriteQueue.front();
			_PhotoSpriteQueue.pop();
		};
	};

	pthread_mutex_lock(&_LockDraw);
	_PhotoSpriteQueue.front()->Draw();

	glTranslatef(0.0f, 0.0f, 0.5f);
	if (_PhotoSpriteQueue.size() > 1)
		_PhotoSpriteQueue.back()->Draw();

	glTranslatef(0.0f, 0.0f, 0.5f);
	_Logo->Draw();

	pthread_mutex_unlock(&_LockDraw);
}
