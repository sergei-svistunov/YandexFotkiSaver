/*
 * CHTTP.h
 *
 *  Created on: 20.09.2009
 *      Author: svistunov
 */

#ifndef CHTTP_H_
#define CHTTP_H_

#include "global.h"
#include "curl/curl.h"
#include "curl/easy.h"

class CHTTP {
public:
	CHTTP();
	virtual ~CHTTP();
	bool Get(const char * URL, const char * FileName);
private:
	CURL* _curl;
};

#endif /* CHTTP_H_ */

