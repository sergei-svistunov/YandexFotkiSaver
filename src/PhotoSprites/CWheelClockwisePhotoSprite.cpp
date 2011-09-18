/*
 * CWheelClockwisePhotoSprite.cpp
 *
 *  Created on: 11.10.2009
 *      Author: svistunov
 */

#include "CWheelClockwisePhotoSprite.h"
#include "math.h"

CWheelClockwisePhotoSprite::CWheelClockwisePhotoSprite() {

}

CWheelClockwisePhotoSprite::~CWheelClockwisePhotoSprite() {

}

void CWheelClockwisePhotoSprite::OnDraw(float EffectState, float ShowState) {
	GLfloat dx, dy;
	GetTextureOffset(&dx, &dy);

	glBegin(GL_TRIANGLE_FAN);
	glTexCoord2f(0.5f, 0.5f);
	glColor4f(0.5f, 0.5f, 0.5f, EffectState);
	glVertex3f(0.0f, 0.0f, 0.0f);

	for (int a = 0; a <= 360; a += 5) {
		float RA = (float)a * M_PI / 180.0f;
		glTexCoord2f((0.5f + sinf(RA) * sqrtf(0.5f + dx)), (0.5f + cosf(RA) * sqrtf(0.5f + dy)));
		glColor4f(1.0f, 1.0f, 1.0f, (float)a / 360.0f > EffectState ? 0.0f : 1.0f);
		glVertex3f(sinf(RA) * M_SQRT2, cos(RA) * M_SQRT2, 0.0f);
	};
	glEnd();
}
