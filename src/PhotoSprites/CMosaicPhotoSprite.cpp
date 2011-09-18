/*
 * CMosaicPhotoSprite.cpp
 *
 *  Created on: 15.10.2009
 *      Author: svistunov
 */

#include "CMosaicPhotoSprite.h"

CMosaicPhotoSprite::CMosaicPhotoSprite() {
	for (int x = 0; x < MOSAIC_WIDTH; x++)
		for (int y = 0; y < MOSAIC_HEIGHT; y++)
			_ShowTimes[x][y] = rand();
}

CMosaicPhotoSprite::~CMosaicPhotoSprite() {
}

void CMosaicPhotoSprite::OnDraw(float EffectState, float ShowState) {
	GLfloat dx, dy;
	GetTextureOffset(&dx, &dy);

	GLfloat PartWidth = 2.0f / MOSAIC_WIDTH;
	GLfloat PartHeight = 2.0f / MOSAIC_HEIGHT;
	GLfloat TexPartWidth = (1.0f + dx * 2) / MOSAIC_WIDTH;
	GLfloat TexPartHeight = (1.0f + dy * 2) / MOSAIC_HEIGHT;

	for (int x = 0; x < MOSAIC_WIDTH ; x++)
		for (int y = 0; y < MOSAIC_HEIGHT; y++) {
			glBegin(GL_QUADS);
				glColor4f(1.0f, 1.0f, 1.0f, EffectState / ((GLfloat)_ShowTimes[x][y] / 255.0f));
				
				glTexCoord2f(-dx + x * TexPartWidth, -dy + y * TexPartHeight);
				glVertex2f(-1.0f + x * PartWidth, -1.0f + y * PartHeight);

				glTexCoord2f(-dx + x * TexPartWidth, -dy + (y + 1) * TexPartHeight);
				glVertex2f(-1.0f + x * PartWidth, -1.0f + (y + 1) * PartHeight);

				glTexCoord2f(-dx + (x + 1) * TexPartWidth, -dy + (y + 1) * TexPartHeight);
				glVertex2f(-1.0f + (x + 1) * PartWidth, -1.0f + (y + 1) * PartHeight);

				glTexCoord2f(-dx + (x + 1) * TexPartWidth, -dy + y * TexPartHeight);
				glVertex2f(-1.0f + (x + 1) * PartWidth, -1.0f + y * PartHeight);
			glEnd();
		};
}
