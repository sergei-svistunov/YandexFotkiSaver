/*
 * CWheelClockwisePhotoSprite.h
 *
 *  Created on: 11.10.2009
 *      Author: svistunov
 */

#ifndef CWHEELCLOCKWISEPHOTOSPRITE_H_
#define CWHEELCLOCKWISEPHOTOSPRITE_H_

#include "CBasePhotoSprite.h"

class CWheelClockwisePhotoSprite: public CBasePhotoSprite {
public:
	CWheelClockwisePhotoSprite();
	virtual ~CWheelClockwisePhotoSprite();
private:
	void OnDraw(float EffectState, float ShowState);
};

#endif /* CWHEELCLOCKWISEPHOTOSPRITE_H_ */
