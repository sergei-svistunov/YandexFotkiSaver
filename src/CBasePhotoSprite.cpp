/*
 * CBasePhotoSprite.cpp
 *
 *  Created on: 19.09.2009
 *      Author: svistunov
 */

#include "CBasePhotoSprite.h"

CBasePhotoSprite::CBasePhotoSprite(){
	Texture = 0;
	struct timeval CycleStart;
	gettimeofday(&CycleStart, NULL);
	_StartTime = CycleStart.tv_sec;

	_FirstTick = 0;
	_EffectTime = 2000;
	_ShowTime = 6000;
	_ImageData = NULL;
}

CBasePhotoSprite::~CBasePhotoSprite() {
	if (Texture)
		glDeleteTextures(1, &Texture);

	if (_ImageData != NULL)
		delete[] _ImageData;
}

void CBasePhotoSprite::Init(pthread_mutex_t * LockDraw, int EffectTime, int ShowTime, float BackgroundColor[4]) {
	_LockDraw = LockDraw;
	_EffectTime = EffectTime;
	_ShowTime = ShowTime;
	memcpy(_BackgroundColor, BackgroundColor, 4 * sizeof(float));
}

int NextPow2(int Num) {
	int Pow2 = 1;
	if (Num == 0)
		return 1;

	for (; Pow2 < Num; Pow2 <<= 1);

	return Pow2;
};

bool CBasePhotoSprite::Load(char * FileName) {
	Magick::Image * Image = new Magick::Image();
	try {
		Image->read(FileName);
		Image->type(Magick::TrueColorType);

		Width = Image->columns();
		Height = Image->rows();

		_TexWidth = NextPow2(Width);
		_TexHeight = NextPow2(Height);

		Image->flip();
		Image->filterType(Magick::QuadraticFilter);
		Magick::Geometry NewSize(_TexWidth, _TexHeight);
		NewSize.aspect(true);
		Image->scale(NewSize);

		_ImageData = new unsigned char[_TexWidth * _TexHeight * 3];

		Image->getConstPixels(1, 1, _TexWidth, _TexHeight);
		Image->writePixels(Magick::RGBQuantum, _ImageData);
		Image->getConstPixels(0, 0, 0, 0);
	} catch (Magick::Exception &Error) {
		syslog(LOG_INFO, "ImageMagick error: %s", Error.what());
		delete Image;
		return false;
	};

	delete Image;
	return true;
}

void CBasePhotoSprite::Draw(void) {
	float EffectState, ShowState;

	struct timeval CycleStart;
	gettimeofday(&CycleStart, NULL);
	intmax_t CurTick = (CycleStart.tv_sec - _StartTime) * 1000 + CycleStart.tv_usec / 1000;

	glPushAttrib(GL_ALL_ATTRIB_BITS);

	glEnable(GL_TEXTURE_2D);

	if (Texture == 0) {
		glGenTextures(1, &Texture);
		glBindTexture(GL_TEXTURE_2D, Texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _TexWidth, _TexHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, _ImageData);
	};

	if (_FirstTick == 0) {
		EffectState = ShowState = 0;
		_FirstTick = CurTick;
	} else {
		EffectState = (float)(CurTick - _FirstTick) / (float)_EffectTime;
		ShowState = (float)(CurTick - _FirstTick) / (float)_ShowTime;
		EffectState = EffectState > 1.0f ? 1.0f : EffectState;
		ShowState = ShowState > 1.0f ? 1.0f : ShowState;
	};

	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, _BackgroundColor);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, Texture);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	OnDraw(EffectState, ShowState);

	glPopAttrib();
}

bool CBasePhotoSprite::IsCompleted(void) {
	struct timeval CycleStart;
	gettimeofday(&CycleStart, NULL);
	uintmax_t CurTick = (CycleStart.tv_sec - _StartTime) * 1000 + CycleStart.tv_usec / 1000;

	return (float)(CurTick - _FirstTick) / (float)_ShowTime >= 1.0f;
}

void CBasePhotoSprite::GetTextureOffset(GLfloat * dx, GLfloat * dy) {
	GLint Viewport[4];
	glGetIntegerv(GL_VIEWPORT, Viewport);
	GLint ScrWidth = Viewport[2];
	GLint ScrHeight = Viewport[3];
	GLfloat kx = (GLfloat)ScrWidth / (GLfloat)Width;
	GLfloat ky = (GLfloat)ScrHeight / (GLfloat)Height;
	*dx = *dy = 0;
	if (kx > ky) {
		*dx = ((GLfloat)ScrWidth / ky - (GLfloat)Width) / (GLfloat)ScrWidth;
	} else {
		*dy = ((GLfloat)ScrHeight / kx - (GLfloat)Height) / (GLfloat)ScrHeight;
	};
}
