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

#include "main.h"
#include "message.h"
#include "storage.h"

void testMenu()
{
	//top screen
	clearScreen(&topScreen);
	iprintf("Storage Check Test\n\n");

	//bottom screen
	clearScreen(&bottomScreen);

	unsigned int free = 0;
	unsigned int size = 0;
	const bool sdFound = (access("sd:/", F_OK) == 0);

	//home menu slots
	if (isDSiMode() && sdFound) {
		iprintf("Free Home Menu Slots:\n");
		swiWaitForVBlank();

		free = getMenuSlotsFree();
		iprintf("\t%d / ", free);
		swiWaitForVBlank();

		size = getMenuSlots();
		iprintf("%d\n", size);
		swiWaitForVBlank();
	}

	//SD Card
	{
		iprintf("\nFree SD Space:\n\t");
		swiWaitForVBlank();

		unsigned long long sdfree = getSDCardFree();
		printBytes(sdfree);
		iprintf(" / ");
		swiWaitForVBlank();

		unsigned long long sdsize = getSDCardSize();
		printBytes(sdsize);	
		iprintf("\n");
		swiWaitForVBlank();

		printf("\t%d / %d blocks\n", (unsigned int)(sdfree / BYTES_PER_BLOCK), (unsigned int)(sdsize / BYTES_PER_BLOCK));
	}

	//Emunand
	if (isDSiMode() && sdFound) {
		iprintf("\nFree DSi Space:\n\t");
		swiWaitForVBlank();

		free = getDsiFree();
		printBytes(free);
		iprintf(" / ");
		swiWaitForVBlank();

		size = getDsiSize();
		printBytes(size);
		iprintf("\n");
		swiWaitForVBlank();

		printf("\t%.0f / %.0f blocks\n", (float)free / BYTES_PER_BLOCK, (float)size / BYTES_PER_BLOCK);
	}

	//end
	iprintf("\nBack - [B]\n");
	keyWait(KEY_B);
}