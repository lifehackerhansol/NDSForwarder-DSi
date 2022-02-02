#include "main.h"
#include "rom.h"
#include "install.h"
#include "menu.h"
#include "storage.h"
#include "message.h"
#include <dirent.h>

enum {
	INSTALL_MENU_INSTALL,
	INSTALL_MENU_RANDOMIZE,
	INSTALL_MENU_BACK
};

static char currentDir[512] = "";

static void generateList(Menu* m);
static void printItem(Menu* m);
static int subMenu();

static void _setHeader(Menu* m)
{
	if (!m) return;
	if (currentDir[0] == '\0')
		setMenuHeader(m, "/");
	else
		setMenuHeader(m, currentDir);
}

void installMenu()
{
	Menu* m = newMenu();
	_setHeader(m);
	generateList(m);

	//no files found
/*	if (m->itemCount <= 0)
	{
		clearScreen(&bottomScreen);

		iprintf("\x1B[31m");	//red
		iprintf("No files found.\n");
		iprintf("\x1B[47m");	//white
		iprintf("\nBack - [B]\n");

		keyWait(KEY_B | KEY_A | KEY_START);
	}
	else*/
	{
		while (1)
		{
			swiWaitForVBlank();
			scanKeys();

			if (moveCursor(m))
			{
				if (m->changePage != 0)
					generateList(m);

				printMenu(m);
				printItem(m);
			}

			//back
			if (keysDown() & KEY_B)
			{
				char* ptr = strrchr(currentDir, '/');

				if (ptr)
				{
					*ptr = '\0';
					_setHeader(m);
					resetMenu(m);
					generateList(m);
					printMenu(m);
				}
				else
				{
					break;
				}
			}

			else if (keysDown() & KEY_X)
				break;

			//selection
			else if (keysDown() & KEY_A)
			{
				if (m->itemCount > 0)
				{
					if (m->items[m->cursor].directory == false)
					{
						//nds file
						switch (subMenu())
						{
							case INSTALL_MENU_INSTALL:
								install(m->items[m->cursor].value, false);
								break;

							case INSTALL_MENU_RANDOMIZE:
								install(m->items[m->cursor].value, true);
								break;

							case INSTALL_MENU_BACK:					
								break;
						}
					}
					else
					{
						//directory
						sprintf(currentDir, "%s", m->items[m->cursor].value);
						_setHeader(m);
						resetMenu(m);
						generateList(m);
					}

					printMenu(m);
				}
			}
		}
	}

	freeMenu(m);
}

static void generateList(Menu* m)
{
	if (!m) return;

	//reset menu
	clearMenu(m);

	m->page += sign(m->changePage);
	m->changePage = 0;

	bool done = false;

	struct dirent* ent;
	DIR* dir = NULL;

	if (currentDir[0] == '\0')
		dir = opendir("/");
	else
		dir = opendir(currentDir);	

	if (dir)
	{
		int count = 0;

		//scan /dsi/
		while ( (ent = readdir(dir)) && !done)
		{
			if (strcmp(".", ent->d_name) == 0 || strcmp("..", ent->d_name) == 0)
				continue;

			if (ent->d_type == DT_DIR)
			{
				if (count < m->page * ITEMS_PER_PAGE)
						count += 1;
			
				else
				{
					if (m->itemCount >= ITEMS_PER_PAGE)
						done = true;
					
					else
					{
						char* fpath = (char*)malloc(strlen(currentDir) + strlen(ent->d_name) + 8);
						sprintf(fpath, "%s/%s", currentDir, ent->d_name);

						addMenuItem(m, ent->d_name, fpath, 1);
					}
				}
			}
			else
			{
				if (strstr(ent->d_name, ".nds") != NULL ||
					strstr(ent->d_name, ".ids") != NULL ||
					strstr(ent->d_name, ".app") != NULL ||
					strstr(ent->d_name, ".dsi") != NULL ||
					strstr(ent->d_name, ".NDS") != NULL ||
					strstr(ent->d_name, ".APP") != NULL ||
					strstr(ent->d_name, ".DSI") != NULL ||
					strstr(ent->d_name, ".IDS") != NULL)
				{
					if (count < m->page * ITEMS_PER_PAGE)
						count += 1;
					
					else
					{
						if (m->itemCount >= ITEMS_PER_PAGE)
							done = true;
						
						else
						{
							char* fpath = (char*)malloc(strlen(currentDir) + strlen(ent->d_name) + 8);
							sprintf(fpath, "%s/%s", currentDir, ent->d_name);

							addMenuItem(m, ent->d_name, fpath, 0);

							free(fpath);
						}
					}
				}
			}
		}
	}

	closedir(dir);

	m->nextPage = done;

	if (m->cursor >= m->itemCount)
		m->cursor = m->itemCount - 1;

	printItem(m);
	printMenu(m);
}

static void printItem(Menu* m)
{
	if (!m) return;
	if (m->itemCount <= 0) return;

	if (m->items[m->cursor].directory)
		clearScreen(&topScreen);
	else
		printRomInfo(m->items[m->cursor].value);
}

static int subMenu()
{
	int result = -1;

	Menu* m = newMenu();

	addMenuItem(m, "Install", NULL, 0);
	addMenuItem(m, "Randomize TID and install", NULL, 0);
	addMenuItem(m, "Back - [B]", NULL, 0);

	printMenu(m);

	while (1)
	{
		swiWaitForVBlank();
		scanKeys();

		if (moveCursor(m))
			printMenu(m);

		if (keysDown() & KEY_B)
		{
			result = -1;
			break;
		}

		else if (keysDown() & KEY_A)
		{
			result = m->cursor;
			break;
		}
	}

	freeMenu(m);
	return result;
}
