/*
 * CBaseRectPhotoSprite.cpp
 *
 *  Created on: 19.09.2009
 *      Author: svistunov
 */

#include "CBaseRectPhotoSprite.h"

CBaseRectPhotoSprite::CBaseRectPhotoSprite() {
	Alpha_LT = Alpha_RT = Alpha_RB = Alpha_LB = 1.0f;

	X_LT = -1.0f;
	Y_LT = 1.0f;
	X_RT = 1.0f;
	Y_RT = 1.0f;
	X_RB = 1.0f;
	Y_RB = -1.0f;
	X_LB = -1.0f;
	Y_LB = -1.0f;

	Z_LT = Z_RT = Z_RB = Z_LB = 0.0f;
}

CBaseRectPhotoSprite::~CBaseRectPhotoSprite() {

}

void CBaseRectPhotoSprite::OnDraw(float EffectState, float ShowState) {
	glPushMatrix();

	GLfloat dx, dy;
	GetTextureOffset(&dx, &dy);

	SetProperties(EffectState, ShowState);

	glBegin(GL_QUADS);
		glTexCoord2f(0.0f - dx, 1.0f + dy);
		glColor4f(1.0f, 1.0f, 1.0f, Alpha_LT);
		glVertex3f(X_LT, Y_LT, Z_LT);

		glTexCoord2f(1.0f + dx, 1.0f + dy);
		glColor4f(1.0f, 1.0f, 1.0f, Alpha_RT);
		glVertex3f(X_RT, Y_RT, Z_RT);

		glTexCoord2f(1.0f + dx, 0.0f - dy);
		glColor4f(1.0f, 1.0f, 1.0f, Alpha_RB);
		glVertex3f(X_RB, Y_RB, Z_RB);

		glTexCoord2f(0.0f - dx, 0.0f - dy);
		glColor4f(1.0f, 1.0f, 1.0f, Alpha_LB);
		glVertex3f(X_LB, Y_LB, Z_LB);
	glEnd();

	glPopMatrix();
}

void CBaseRectPhotoSprite::SetProperties(float EffectState, float ShowState) {

}
