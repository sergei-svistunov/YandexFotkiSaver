/*
 * CFadeRectPhotoSprite.cpp
 *
 *  Created on: 19.09.2009
 *      Author: svistunov
 */

#include "CFadeRectPhotoSprite.h"

CFadeRectPhotoSprite::CFadeRectPhotoSprite() {

}

CFadeRectPhotoSprite::~CFadeRectPhotoSprite() {

}

void CFadeRectPhotoSprite::SetProperties(float EffectState, float ShowState) {
	Alpha_LT = Alpha_RT = Alpha_RB = Alpha_LB = EffectState;
}
