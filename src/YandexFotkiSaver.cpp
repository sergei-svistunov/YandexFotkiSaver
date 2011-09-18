#include "global.h"
#include "CYandexFotkiScreensaver.h"

int main (int argc, char *argv[]) {
	CYandexFotkiScreensaver * YandexFotkiScreensaver = new CYandexFotkiScreensaver();
	if (YandexFotkiScreensaver->Init(argc, argv))
		YandexFotkiScreensaver->Run();
	delete YandexFotkiScreensaver;
}
