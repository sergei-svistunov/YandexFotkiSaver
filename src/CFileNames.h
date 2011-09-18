/*
 * CFileNames.h
 *
 *  Created on: 18.09.2009
 *      Author: svistunov
 */

#ifndef CFILENAMES_H_
#define CFILENAMES_H_

#include "global.h"

#include <vector>
#include <pthread.h>

class CFileNames {
public:
	CFileNames(char * ImageFolder);
	virtual ~CFileNames();
	void UpdateFileNames();
	char * GetNextFileName(void);
private:
	static void * _ThrUpdateFileNames(void * pthis);
	char * _ImageFolder;
	pthread_t _pThrUpdateFileNames;
	pthread_mutex_t _LockUpdateFileNames, _LockFileNames;
	std::vector <char *> _FileNames;
	std::vector <char *>::iterator  _NextFileNameIter;
};

#endif /* CFILENAMES_H_ */
