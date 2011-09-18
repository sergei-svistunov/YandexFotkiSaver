/*
 * CMacEffectPhotoSprite.cpp
 *
 *  Created on: 29.09.2009
 *      Author: svistunov
 */

#include "CMacEffectPhotoSprite.h"
#include "math.h"

CMacEffectPhotoSprite::CMacEffectPhotoSprite() {
	switch (rand() % 4) {
		case 0:
			_Angle = M_PI / 4.0f;
			break;
		case 1:
			_Angle = 3.0f * M_PI / 4.0f;
			break;
		case 2:
			_Angle = 5.0f * M_PI / 4.0f;
			break;
		case 3:
			_Angle = 7.0f * M_PI / 4.0f;
			break;
		};
}

CMacEffectPhotoSprite::~CMacEffectPhotoSprite() {

}

void CMacEffectPhotoSprite::SetProperties(float EffectState, float ShowState) {
	Alpha_LT = Alpha_RT = Alpha_RB = Alpha_LB = EffectState;

	GLfloat Scale = 1.0f + 0.5f * ShowState;
	glScalef(Scale, Scale, 1.0f);
	glTranslatef(0.3f * ShowState * sin(_Angle), 0.3f * ShowState * cos(_Angle), 0);
}
