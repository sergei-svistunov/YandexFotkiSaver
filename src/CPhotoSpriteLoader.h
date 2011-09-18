/*
 * CPhotoSpriteLoader.h
 *
 *  Created on: 19.09.2009
 *      Author: svistunov
 */

#ifndef CPHOTOSPRITELOADER_H_
#define CPHOTOSPRITELOADER_H_

#include "global.h"
#include <pthread.h>
#include <queue>

#include "CConfig.h"
#include "CFileNames.h"
#include "CHTTP.h"

#include "CBasePhotoSprite.h"

class CPhotoSpriteLoader {
public:
	CPhotoSpriteLoader(char * Folder, char * PhotoFolder, pthread_mutex_t * LockDraw, CConfig * Config);
	virtual ~CPhotoSpriteLoader();
	CBasePhotoSprite * GetPhotoSprite(void);
	int GetStatus(void);
private:
	static void * _ThrFillQueue(void * pthis);
	bool DownloadPhotos(void);
	void SetStatus(int aStatus);
	int _Status;
	char * _XMLFileName;
	char * _Folder;
	char * _PhotoFolder;
	char * _DownloadFolder;
	pthread_mutex_t * _LockDraw, _LockStatus;
	CConfig * _Config;
	CFileNames * _FileNames;
	CHTTP * _HTTP;
	pthread_t _pThrFillQueue;
	pthread_mutex_t _LockPhotoSpriteQueue;
	std::queue <CBasePhotoSprite *> _PhotoSpriteQueue;
	time_t _LastUpdate;
};

#endif /* CPHOTOSPRITELOADER_H_ */
