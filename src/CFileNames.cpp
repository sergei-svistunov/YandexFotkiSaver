/*
 * CFileNames.cpp
 *
 *  Created on: 18.09.2009
 *      Author: svistunov
 */

#include "CFileNames.h"

#include <dirent.h>
#include <fcntl.h>
#include <unistd.h>
#include <algorithm>
#include <sys/stat.h>
#include <linux/stat.h>

CFileNames::CFileNames(char * ImageFolder) {
	_ImageFolder = ImageFolder;
	pthread_mutex_init(&_LockUpdateFileNames, NULL);
	pthread_mutex_init(&_LockFileNames, NULL);
	_NextFileNameIter = _FileNames.begin();
}

CFileNames::~CFileNames() {
	pthread_mutex_lock(&_LockFileNames);
	std::vector <char *>::iterator Iter;
	for (Iter = _FileNames.begin(); Iter != _FileNames.end(); Iter++) {
		delete((char *)*Iter);
	};
	_FileNames.clear();
	pthread_mutex_unlock(&_LockFileNames);

	pthread_mutex_destroy(&_LockUpdateFileNames);
	pthread_mutex_destroy(&_LockFileNames);
}

void CFileNames::UpdateFileNames() {
	pthread_create(&_pThrUpdateFileNames, NULL, CFileNames::_ThrUpdateFileNames, this);
	pthread_detach(_pThrUpdateFileNames);
}

void * CFileNames::_ThrUpdateFileNames(void * pthis) {
	char * ImageFolder = ((CFileNames*)pthis)->_ImageFolder;
	std::vector <char *> FileNames;

	pthread_mutex_lock(&((CFileNames*)pthis)->_LockUpdateFileNames);
	DIR * dir = opendir(ImageFolder);
	struct dirent * dent;
	struct stat buf;
	size_t szImageFolder = strlen(ImageFolder);
	if (dir != NULL) {
		while ((dent = readdir(dir)) != NULL) {
			size_t szFileName = szImageFolder + strlen(dent->d_name) + 2;
			char * FileName = new char[szFileName];
			sprintf(FileName, "%s/%s", ImageFolder, dent->d_name);
			stat(FileName, &buf);
			if (S_ISREG(buf.st_mode))
				FileNames.push_back(FileName);
			else
				delete[] FileName;
		};
		closedir(dir);
	};

	std::random_shuffle(FileNames.begin(), FileNames.end());

	// Подменяем список
	pthread_mutex_lock(&((CFileNames*)pthis)->_LockFileNames);
	((CFileNames*)pthis)->_FileNames.swap(FileNames);
	((CFileNames*)pthis)->_NextFileNameIter = ((CFileNames*)pthis)->_FileNames.begin();
	pthread_mutex_unlock(&((CFileNames*)pthis)->_LockFileNames);

	// Удаляем старый
	std::vector <char *>::iterator Iter;
	for (Iter = FileNames.begin(); Iter != FileNames.end(); Iter++) {
		delete((char *)*Iter);
	};
	FileNames.clear();

	pthread_mutex_unlock(&((CFileNames*)pthis)->_LockUpdateFileNames);
	return NULL;
}

char * CFileNames::GetNextFileName(void) {
	pthread_mutex_lock(&_LockFileNames);

	if (!_NextFileNameIter.base()) {
		pthread_mutex_unlock(&_LockFileNames);
		return NULL;
	};

	char * FileName = (char*)*_NextFileNameIter;
	_NextFileNameIter++;
	if (_FileNames.end() == _NextFileNameIter)
		_NextFileNameIter = _FileNames.begin();
	pthread_mutex_unlock(&_LockFileNames);
	return FileName;
}
