/*
 * CBaseRectPhotoSprite.h
 *
 *  Created on: 19.09.2009
 *      Author: svistunov
 */

#ifndef CBASERECTPHOTOSPRITE_H_
#define CBASERECTPHOTOSPRITE_H_

#include "CBasePhotoSprite.h"

class CBaseRectPhotoSprite: public CBasePhotoSprite {
public:
	CBaseRectPhotoSprite();
	virtual ~CBaseRectPhotoSprite();
protected:
	GLfloat Alpha_LT, Alpha_RT,	Alpha_RB, Alpha_LB;
	GLfloat X_LT, Y_LT, Z_LT,
			X_RT, Y_RT, Z_RT,
			X_RB, Y_RB, Z_RB,
			X_LB, Y_LB, Z_LB;
private:
	void OnDraw(float EffectState, float ShowState);
	virtual void SetProperties(float EffectState, float ShowState);
};

#endif /* CBASERECTPHOTOSPRITE_H_ */
