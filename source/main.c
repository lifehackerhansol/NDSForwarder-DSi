#include "main.h"
#include "menu.h"
#include "message.h"
#include "nitrofs.h"
#include <time.h>

#define VERSION "0.1.3"

PrintConsole topScreen;
PrintConsole bottomScreen;

enum {
	MAIN_MENU_INSTALL,
	MAIN_MENU_TEST,
	MAIN_MENU_EXIT
};

static void _setupScreens()
{
	REG_DISPCNT = MODE_FB0;
	VRAM_A_CR = VRAM_ENABLE;

	videoSetMode(MODE_0_2D);
	videoSetModeSub(MODE_0_2D);

	vramSetBankA(VRAM_A_MAIN_BG);
	vramSetBankC(VRAM_C_SUB_BG);

	consoleInit(&topScreen,    3, BgType_Text4bpp, BgSize_T_256x256, 31, 0, true,  true);
	consoleInit(&bottomScreen, 3, BgType_Text4bpp, BgSize_T_256x256, 31, 0, false, true);

	clearScreen(&bottomScreen);

	VRAM_A[100] = 0xFFFF;
}

static int _mainMenu(int cursor)
{
	//top screen
	clearScreen(&topScreen);

	iprintf("\tNDSForwarder for HiyaCFW\n");
	iprintf("\nversion %s\n", VERSION);
	iprintf("\x1b[22;0HJeff - 2018-2019");
	iprintf("\x1b[23;0Hlifehackerhansol - 2022");

	//menu
	Menu* m = newMenu();
	setMenuHeader(m, "MAIN MENU");

	addMenuItem(m, "Install", NULL, 0);
	addMenuItem(m, "Test", NULL, 0);
	addMenuItem(m, "Shut Down", NULL, 0);

	m->cursor = cursor;

	//bottom screen
	printMenu(m);

	while (1)
	{
		swiWaitForVBlank();
		scanKeys();

		if (moveCursor(m))
			printMenu(m);

		if (keysDown() & KEY_A)
			break;
	}

	int result = m->cursor;
	freeMenu(m);

	return result;
}

bool isRetailDSi(void) {
	if (REG_SCFG_EXT != 0) {
		*(vu32*)(0x0DFFFE0C) = 0x53524C41;		// Check for 32MB of RAM
		bool isDevConsole = (*(vu32*)(0x0DFFFE0C) == 0x53524C41);
		if (!isDevConsole) return true;
	}
	return false;
}

int main(int argc, char **argv)
{
	srand(time(0));
	_setupScreens();

	//DSi check
	if (!isDSiMode() || !isRetailDSi())
	{
		messageBox("\x1B[31mError:\x1B[33m This app is only for retail DSi.");
		return 0;
	}

	//setup sd card access
	if (!fatInitDefault())
	{
		messageBox("fatInitDefault()...\x1B[31mFailed\n\x1B[47m");
		return 0;
	}

	// setup NitroFS
	if(!nitroFSInit(NULL)) if(!nitroFSInit(argv[0])) if(!nitroFSInit("/NDSForwarder.dsi")) {
		messageBox("nitroFSInit()...\x1B[31mFailed\n\x1B[47m");
		return 0;
	}

	//main menu
	bool programEnd = false;
	int cursor = 0;

	while (!programEnd)
	{
		cursor = _mainMenu(cursor);

		switch (cursor)
		{
			case MAIN_MENU_INSTALL:
				installMenu();
				break;

			case MAIN_MENU_TEST:
				testMenu();
				break;

			case MAIN_MENU_EXIT:
				programEnd = true;
				break;
		}
	}

	return 0;
}

void clearScreen(PrintConsole* screen)
{
	consoleSelect(screen);
	consoleClear();
}