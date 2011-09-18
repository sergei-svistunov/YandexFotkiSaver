/*
 * CYandexFotkiScreensaver.h
 *
 *  Created on: 25.09.2009
 *      Author: svistunov
 */

#ifndef CYANDEXFOTKISCREENSAVER_H_
#define CYANDEXFOTKISCREENSAVER_H_

#include "global.h"

#include <pthread.h>
#include <queue>

#include "CXScreenSaver.h"
#include "CConfig.h"
#include "CPhotoSpriteLoader.h"
#include "CBasePhotoSprite.h"
#include "CLogo.h"

class CYandexFotkiScreensaver: public CXScreenSaver {
public:
	CYandexFotkiScreensaver();
	virtual ~CYandexFotkiScreensaver();
private:
	void GLInit(void);
	void OnDraw(void);
	void _DrawDownload(void);
	void _DrawPhotos(void);
	char * _Folder;
	char * _PhotoFolder;
	CConfig * _Config;
	GLuint _FontList;
	CPhotoSpriteLoader * _PhotoSpriteLoader;
	CLogo * _Logo;
	pthread_mutex_t _LockDraw;
	std::queue <CBasePhotoSprite *> _PhotoSpriteQueue;
};

#endif /* CYANDEXFOTKISCREENSAVER_H_ */
