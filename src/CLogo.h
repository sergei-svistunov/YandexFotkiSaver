/*
 * CLogo.h
 *
 *  Created on: 21.09.2009
 *      Author: svistunov
 */

#ifndef CLOGO_H_
#define CLOGO_H_

#include "global.h"

class CLogo {
public:
	CLogo();
	virtual ~CLogo();
	void Draw(void);
private:
	void PrintData(void);
	GLuint _Texture;
};

#endif /* CLOGO_H_ */
