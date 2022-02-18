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
#include <malloc.h>

#include <nds.h>

#include "rom.h"
#include "main.h"
#include "storage.h"

tDSiHeader* getRomHeader(char const* fpath)
{
	if (!fpath) return NULL;

	tDSiHeader* h = NULL;
	FILE* f = fopen(fpath, "rb");

	if (f)
	{
		h = (tDSiHeader*)malloc(sizeof(tDSiHeader));

		if (h)
		{
			fseek(f, 0, SEEK_SET);
			fread(h, sizeof(tDSiHeader), 1, f);
		}

		fclose(f);
	}

	return h;
}

tNDSHeader* getRomHeaderNDS(char const* fpath)
{
	if (!fpath) return NULL;

	tNDSHeader* h = NULL;
	FILE* f = fopen(fpath, "rb");

	if (f)
	{
		h = (tNDSHeader*)malloc(sizeof(tNDSHeader));

		if (h)
		{
			fseek(f, 0, SEEK_SET);
			fread(h, sizeof(tNDSHeader), 1, f);
		}

		fclose(f);
	}

	return h;
}

sNDSBannerExt* getRomBanner(char const* fpath)
{
	if (!fpath) return NULL;

	tNDSHeader* h = getRomHeaderNDS(fpath);
	sNDSBannerExt* b = NULL;

	if (h)
	{
		FILE* f = fopen(fpath, "rb");

		if (f)
		{
			b = (sNDSBannerExt*)malloc(sizeof(sNDSBannerExt));

			if (b)
			{
				fseek(f, 0, SEEK_SET);
				fseek(f, h->bannerOffset, SEEK_CUR);
				fread(b, sizeof(sNDSBannerExt), 1, f);
			}
		}

		free(h);
		fclose(f);		
	}

	return b;
}

bool getGameTitle(sNDSBannerExt* b, char* out, bool full)
{
	if (!b) return false;
	if (!out) return false;

	//get system language
	int lang = PersonalData->language;

	//not japanese or chinese
	if (lang == 0 || lang == 6)
		lang = 1;

	//read title
	u16 c;
	for (int i = 0; i < 128; i++)
	{
		c = b->titles[lang][i];

		//remove accents
		if (c == 0x00F3)
			c = 'o';

		if (c == 0x00E1)
			c = 'a';

		out[i] = (char)c;

		if (!full && out[i] == '\n')
		{
			out[i] = '\0';
			break;
		}
	}
	out[128] = '\0';

	return true;
}

void printRomInfo(char const* fpath)
{
	clearScreen(&topScreen);

	if (!fpath) return;

	tNDSHeader* h = getRomHeaderNDS(fpath);
	sNDSBannerExt* b = getRomBanner(fpath);

		if (!b)
		{
			iprintf("Could not read banner.\n");
		}
		else
		{
			//proper title
			{
				char gameTitle[128+1];
				getGameTitle(b, gameTitle, true);

				iprintf("%s\n\n", gameTitle);
			}

			//file size
			{
				iprintf("Size: ");
				printBytes(getRomSize(fpath));
				iprintf("\n");
			}

			iprintf("Label: %.12s\n", h->gameTitle);
			iprintf("Game Code: %.4s\n", h->gameCode);

			//print full file path
			iprintf("\n%s\n", fpath);
		}
	
	free(b);
	free(h);
}

unsigned long long getRomSize(char const* fpath)
{
	if (!fpath) return 0;

	unsigned long long size = 0;
	FILE* f = fopen(fpath, "rb");

	if (f)
	{
		fseek(f, 0, SEEK_END);
		size = ftell(f);
	}

	fclose(f);
	return size;
}

