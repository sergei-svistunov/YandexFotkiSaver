/*
 * CMosaicPhotoSprite.h
 *
 *  Created on: 15.10.2009
 *      Author: svistunov
 */

#ifndef CMOSAICPHOTOSPRITE_H_
#define CMOSAICPHOTOSPRITE_H_

#include "CBasePhotoSprite.h"

#define MOSAIC_WIDTH 50
#define MOSAIC_HEIGHT 50

class CMosaicPhotoSprite: public CBasePhotoSprite {
public:
	CMosaicPhotoSprite();
	virtual ~CMosaicPhotoSprite();
private:
	void OnDraw(float EffectState, float ShowState);
	unsigned char _ShowTimes[MOSAIC_WIDTH][MOSAIC_HEIGHT];
};

#endif /* CMOSAICPHOTOSPRITE_H_ */
