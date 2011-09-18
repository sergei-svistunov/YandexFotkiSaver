/*
 * CPanoramPhotoSprite.cpp
 *
 *  Created on: 29.09.2009
 *      Author: svistunov
 */

#include "CPanoramPhotoSprite.h"

CPanoramPhotoSprite::CPanoramPhotoSprite() {
	_Type = rand() % 3;
	_Fade = rand() % 2;
}

CPanoramPhotoSprite::~CPanoramPhotoSprite() {

}

void CPanoramPhotoSprite::SetProperties(float EffectState, float ShowState) {
	switch (_Type) {
		case 0:
			glScalef(EffectState, 1.0f, 1.0f);
			break;
		case 1:
			glScalef(1.0f, EffectState, 1.0f);
			break;
		case 2:
			glScalef(EffectState, EffectState, 1.0f);
			break;
	};

	if (_Fade)
		Alpha_LT = Alpha_RT = Alpha_RB = Alpha_LB = EffectState;
}
