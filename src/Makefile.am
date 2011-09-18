bindir=${libdir}/xscreensaver
bin_PROGRAMS=YandexFotkiSaver

YandexFotkiSaver_SOURCES=YandexFotkiSaver.cpp global.h\
        CBasePhotoSprite.cpp CBasePhotoSprite.h \
        CConfig.cpp CConfig.h \
        CFileNames.cpp CFileNames.h \
        CHTTP.cpp CHTTP.h \
        CLogo.cpp CLogo.h \
        CPhotoSpriteLoader.cpp CPhotoSpriteLoader.h \
        CXScreenSaver.cpp CXScreenSaver.h \
        CYandexFotkiScreensaver.cpp CYandexFotkiScreensaver.h \
        LogoImageData.h StringsCP1251.h \
        PhotoSprites/CBaseRectPhotoSprite.cpp PhotoSprites/CBaseRectPhotoSprite.h \
        PhotoSprites/CFadeRectPhotoSprite.cpp PhotoSprites/CFadeRectPhotoSprite.h \
        PhotoSprites/CMacEffectPhotoSprite.cpp PhotoSprites/CMacEffectPhotoSprite.h \
        PhotoSprites/CMosaicPhotoSprite.cpp PhotoSprites/CMosaicPhotoSprite.h \
        PhotoSprites/CPanoramPhotoSprite.cpp PhotoSprites/CPanoramPhotoSprite.h \
        PhotoSprites/CWheelClockwisePhotoSprite.cpp PhotoSprites/CWheelClockwisePhotoSprite.h

YandexFotkiSaver_CPPFLAGS=-IPhotoSprites ${MAGICKPP_CFLAGS} ${XML_CFLAGS}
YandexFotkiSaver_LDADD = ${MAGICKPP_LIBS} ${XML_LIBS} ${GL_LIBS} ${GLEW_LIBS} ${CURL_LIBS}
