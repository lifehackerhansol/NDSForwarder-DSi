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

#include <stdio.h>

#include <nds.h>
#include "message.h"
#include "main.h"

void keyWait(u32 key)
{
	while (1)
	{
		swiWaitForVBlank();
		scanKeys();

		if (keysDown() & key)
			break;
	}
}

bool choiceBox(char* message)
{	
	const int choiceRow = 10;
	int cursor = 0;

	clearScreen(&bottomScreen);

	printf("\x1B[33m");	//yellow
	printf("%s\n", message);
	printf("\x1B[47m");	//white
	printf("\x1b[%d;0H\tYes\n\tNo\n", choiceRow);

	while (1)
	{
		swiWaitForVBlank();
		scanKeys();

		//Clear cursor
		printf("\x1b[%d;0H ", choiceRow + cursor);

		if (keysDown() & (KEY_UP | KEY_DOWN))
			cursor = !cursor;

		//Print cursor
		printf("\x1b[%d;0H>", choiceRow + cursor);

		if (keysDown() & (KEY_A | KEY_START))
			break;

		if (keysDown() & KEY_B)
		{
			cursor = 1;
			break;
		}
	}

	scanKeys();
	return (cursor == 0)? YES: NO;
}

bool choicePrint(char* message)
{
	bool choice = NO;

	printf("\x1B[33m");	//yellow
	printf("\n%s\n", message);
	printf("\x1B[47m");	//white
	printf("Yes - [A]\nNo  - [B]\n");

	while (1)
	{
		swiWaitForVBlank();
		scanKeys();

		if (keysDown() & KEY_A)
		{
			choice = YES;
			break;
		}

		else if (keysDown() & KEY_B)
		{
			choice = NO;
			break;
		}
	}

	scanKeys();
	return choice;
}

void messageBox(char* message)
{
	clearScreen(&bottomScreen);
	messagePrint(message);
}

void messagePrint(char* message)
{
	printf("%s\n", message);
	printf("\nOkay - [A]\n");

	while (1)
	{
		swiWaitForVBlank();
		scanKeys();

		if (keysDown() & (KEY_A | KEY_B | KEY_START))
			break;
	}

	scanKeys();
}