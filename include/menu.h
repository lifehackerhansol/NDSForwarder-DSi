/*
    NDSForwarder for DSi
    Copyright (C) 2018-2020 JeffRuLz
    Copyright (C) 2022-present lifehackerhansol

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef MENU_H
#define MENU_H

#include <nds/ndstypes.h>

#define ITEMS_PER_PAGE 20

typedef struct {
	bool directory;
	char* label;
	char* value;
} Item;

typedef struct {
	int cursor;
	int page;
	int itemCount;
	bool nextPage;
	int changePage;
	char header[32];
	Item items[ITEMS_PER_PAGE];
} Menu;

Menu* newMenu();
void freeMenu(Menu* m);

void addMenuItem(Menu* m, char const* label, char const* value, bool directory);
void setMenuHeader(Menu* m, char* str);

void resetMenu(Menu* m);
void clearMenu(Menu* m);
void printMenu(Menu* m);

bool moveCursor(Menu* m);

#endif