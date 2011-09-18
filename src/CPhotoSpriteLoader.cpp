/*
 * CPhotoSpriteLoader.cpp
 *
 *  Created on: 19.09.2009
 *      Author: svistunov
 */

#include <unistd.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <stdio.h>
#include <dirent.h>
#include <fcntl.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <linux/stat.h>

#include "CPhotoSpriteLoader.h"

#include "CFadeRectPhotoSprite.h"
#include "CMacEffectPhotoSprite.h"
#include "CPanoramPhotoSprite.h"
#include "CWheelClockwisePhotoSprite.h"
#include "CMosaicPhotoSprite.h"

#define UPDATE_TIME 30 * 60

CPhotoSpriteLoader::CPhotoSpriteLoader(char * Folder, char * PhotoFolder, pthread_mutex_t * LockDraw, CConfig * Config) {
	_LockDraw = LockDraw;
	_Status = 0;
	_LastUpdate = 0;

	_XMLFileName = new char[strlen(PhotoFolder) + 11];
	sprintf(_XMLFileName, "%s/fotki.xml", PhotoFolder);

	_Folder = Folder;
	_PhotoFolder = PhotoFolder;

	_DownloadFolder = new char[strlen(PhotoFolder) + 7];
	sprintf(_DownloadFolder, "%s/.temp", PhotoFolder);
	mkdir(_DownloadFolder, S_IRWXU | S_IRWXG | S_IREAD);

	_FileNames = new CFileNames(PhotoFolder);
	_HTTP = new CHTTP;

	pthread_mutex_init(&_LockPhotoSpriteQueue, NULL);
	pthread_mutex_init(&_LockStatus, NULL);

	_Config = Config;

	pthread_create(&_pThrFillQueue, NULL, CPhotoSpriteLoader::_ThrFillQueue, this);
	pthread_detach(_pThrFillQueue);
	pthread_setschedprio(_pThrFillQueue, 10);
}

CPhotoSpriteLoader::~CPhotoSpriteLoader() {
	pthread_cancel(_pThrFillQueue);
	delete _DownloadFolder;
	delete _XMLFileName;
	delete _FileNames;
	delete _HTTP;

	pthread_mutex_lock(&_LockPhotoSpriteQueue);
	while (!_PhotoSpriteQueue.empty()) {
		delete _PhotoSpriteQueue.front();
		_PhotoSpriteQueue.pop();
	};
	pthread_mutex_unlock(&_LockPhotoSpriteQueue);
	pthread_mutex_destroy(&_LockPhotoSpriteQueue);
}

void * CPhotoSpriteLoader::_ThrFillQueue(void * pthis) {
	CPhotoSpriteLoader * tthis = (CPhotoSpriteLoader*)pthis;

	tthis->SetStatus(1);

	struct timeval CycleStart;
	char * UpdateTimeFileName = new char[strlen(tthis->_Folder) + 13];
	sprintf(UpdateTimeFileName, "%s/LastUpdated", tthis->_Folder);
	FILE * Stream = fopen(UpdateTimeFileName, "a+b");
	delete[] UpdateTimeFileName;

	float BackgroundColor[4] = {
			tthis->_Config->GetFloat("background-r", 0.5f),
			tthis->_Config->GetFloat("background-g", 0.5f),
			tthis->_Config->GetFloat("background-b", 0.5f),
			1.0f
	};

	while (1) {
		gettimeofday(&CycleStart, NULL);

		//Обновляем картинки
		if (CycleStart.tv_sec - tthis->_LastUpdate > UPDATE_TIME) {
			tthis->SetStatus(1);

			flock(fileno(Stream), LOCK_EX);
			fseek(Stream, SEEK_SET, 0);

			if (fscanf(Stream, "%li", &tthis->_LastUpdate) < 1)
				tthis->_LastUpdate = 0;

			if (CycleStart.tv_sec - tthis->_LastUpdate > UPDATE_TIME) {
				if (tthis->DownloadPhotos()) {
					tthis->_LastUpdate = CycleStart.tv_sec;
					ftruncate(fileno(Stream), 0);
					fprintf(Stream, "%li", tthis->_LastUpdate);
					fflush(Stream);
				} else {
					tthis->SetStatus(3);
					sleep(5);
					if (tthis->_LastUpdate > 0)
						tthis->_LastUpdate = CycleStart.tv_sec;
					else
						continue;
				};
			};

			flock(fileno(Stream), LOCK_UN);
			tthis->_FileNames->UpdateFileNames();
			tthis->SetStatus(2);
		};

		pthread_mutex_lock(&tthis->_LockPhotoSpriteQueue);
		int szPhotoSpriteQueue = tthis->_PhotoSpriteQueue.size();
		pthread_mutex_unlock(&tthis->_LockPhotoSpriteQueue);

		if (szPhotoSpriteQueue < 3) {
			CBasePhotoSprite * PhotoSprite;
			switch (rand() % 5) {
			//switch (4) {
				case 0:
					PhotoSprite = new CFadeRectPhotoSprite();
					break;
				case 1:
					PhotoSprite = new CMacEffectPhotoSprite();
					break;
				case 2:
					PhotoSprite = new CPanoramPhotoSprite();
					break;
				case 3:
					PhotoSprite = new CWheelClockwisePhotoSprite();
					break;
				case 4:
					PhotoSprite = new CMosaicPhotoSprite();
					break;
			};
			PhotoSprite->Init(tthis->_LockDraw, tthis->_Config->GetInt("effect_time", 2000), tthis->_Config->GetInt("show_time", 6000), BackgroundColor);

			char * FileName = NULL;
			while (FileName == NULL || !PhotoSprite->Load(FileName)) {
				FileName = tthis->_FileNames->GetNextFileName();
				pthread_yield();
			};

			pthread_mutex_lock(&tthis->_LockPhotoSpriteQueue);
			tthis->_PhotoSpriteQueue.push(PhotoSprite);
			pthread_mutex_unlock(&tthis->_LockPhotoSpriteQueue);
		} else {
			sleep(1);
		};
	};

	fclose(Stream);

	return NULL;
}

CBasePhotoSprite * CPhotoSpriteLoader::GetPhotoSprite(void) {
	CBasePhotoSprite * PhotoSprite = NULL;

	while (PhotoSprite == NULL) {
		pthread_mutex_lock(&_LockPhotoSpriteQueue);
		if (_PhotoSpriteQueue.empty()) {
			pthread_mutex_unlock(&_LockPhotoSpriteQueue);
			pthread_yield();
			continue;
		};
		PhotoSprite = _PhotoSpriteQueue.front();
		_PhotoSpriteQueue.pop();
		pthread_mutex_unlock(&_LockPhotoSpriteQueue);
	};

	return PhotoSprite;
}

bool CPhotoSpriteLoader::DownloadPhotos(void) {
	if (!_HTTP->Get("http://fotki.yandex.ru/export/slideshow-best50.xml", _XMLFileName)) {
		syslog(LOG_ERR, "Cannot load slideshow-best50.xml");
		return false;
	};

	xmlDoc * XMLDoc = NULL;
	xmlNode * Elem = NULL;

	LIBXML_TEST_VERSION

	XMLDoc = xmlReadFile(_XMLFileName, NULL, 0);
	if (XMLDoc == NULL)
		return false;

	Elem = xmlDocGetRootElement(XMLDoc);

	Elem = Elem->children;
	do {
		if (strcmp((char*)Elem->name, "image") == 0) {
			 xmlAttr * Attr = Elem->properties;
			 while (Attr && strcmp((char*)Attr->name, "url") != 0)
				 Attr = Attr->next;

			 if (Attr) {
				 char * XMLURL = (char*)Attr->children->content;
				 size_t szXMLURL = strlen(XMLURL);
				 char * URL = new char[szXMLURL + 2];
				 strcpy(URL, XMLURL);
				 URL[szXMLURL - 1] = 0;
				 strcat(URL, "XL");
				 char * FileName = strrchr(URL, '/');
				 char * FullFileName = new char[strlen(_DownloadFolder) + strlen(FileName) + 1];
				 strcpy(FullFileName, _DownloadFolder);
				 strcat(FullFileName, FileName);
				 for (int i=0; i<3; i++)
					 if (_HTTP->Get(URL, FullFileName))
						 break;

				 delete[] URL;
				 delete[] FullFileName;
			 };
		};
		Elem = Elem->next;
	} while (Elem);

	xmlFreeDoc(XMLDoc);
	xmlCleanupParser();

	remove(_XMLFileName);

	//Удаляем старые файлы
	DIR * dir = opendir(_PhotoFolder);
	struct dirent * dent;
	struct stat buf;
	size_t szPhotoFolder = strlen(_PhotoFolder);
	if (dir != NULL) {
		while ((dent = readdir(dir)) != NULL) {
			size_t szFileName = szPhotoFolder + strlen(dent->d_name) + 2;
			char * FileName = new char[szFileName];
			sprintf(FileName, "%s/%s", _PhotoFolder, dent->d_name);
			stat(FileName, &buf);
			if (S_ISREG(buf.st_mode))
				remove(FileName);
			delete[] FileName;
		};
		closedir(dir);
	};

	//Перемещаем скаченные файлы
	dir = opendir(_DownloadFolder);
	size_t szDownloadFolder = strlen(_DownloadFolder);
	if (dir != NULL) {
		while ((dent = readdir(dir)) != NULL) {
			char * OldFileName = new char[szDownloadFolder + strlen(dent->d_name) + 2];
			char * NewFileName = new char[szPhotoFolder + strlen(dent->d_name) + 2];

			sprintf(OldFileName, "%s/%s", _DownloadFolder, dent->d_name);
			sprintf(NewFileName, "%s/%s", _PhotoFolder, dent->d_name);
			stat(OldFileName, &buf);

			if (S_ISREG(buf.st_mode))
				rename(OldFileName, NewFileName);

			delete[] OldFileName;
			delete[] NewFileName;
		};
		closedir(dir);
	};

	return true;
}

void CPhotoSpriteLoader::SetStatus(int aStatus) {
	pthread_mutex_lock(&_LockStatus);
	_Status = aStatus;
	pthread_mutex_unlock(&_LockStatus);
};

int CPhotoSpriteLoader::GetStatus(void) {
	pthread_mutex_lock(&_LockStatus);
	int ResStatus = _Status;
	pthread_mutex_unlock(&_LockStatus);
	return ResStatus;
};
