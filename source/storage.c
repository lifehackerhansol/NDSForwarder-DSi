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
#include <errno.h>

#include <nds.h>
#include <sys/dir.h>
#include <sys/statvfs.h>

#include "storage.h"
#include "main.h"
#include "message.h"

#define TITLE_LIMIT 39

//printing
void printBytes(unsigned long long bytes)
{
	if (bytes < 1024)
		printf("%dB", (unsigned int)bytes);

	else if (bytes < 1024 * 1024)
		printf("%.2fKB", (float)bytes / 1024.f);

	else if (bytes < 1024 * 1024 * 1024)
		printf("%.2fMB", (float)bytes / 1024.f / 1024.f);

	else
		printf("%.2fGB", (float)bytes / 1024.f / 1024.f / 1024.f);
}

//progress bar
static int lastBars = 0;

void printProgressBar(float percent)
{
	if (percent < 0.f) percent = 0.f;
	if (percent > 1.f) percent = 1.f;

	int bars = (int)(30.f * percent);

	//skip redundant prints
	if (bars != lastBars)
	{
		consoleSelect(&topScreen);

		printf("\x1B[42m");	//green

		//Print frame
		if (lastBars <= 0)
		{
			printf("\x1b[23;0H[");
			printf("\x1b[23;31H]");
		}

		//Print bars
		if (bars > 0)
		{			
			for (int i = 0; i < bars; i++)
				printf("\x1b[23;%dH|", 1 + i);			
		}

		lastBars = bars;

		printf("\x1B[47m");	//white
	}	
}

void clearProgressBar()
{
	lastBars = 0;
	consoleSelect(&topScreen);
	printf("\x1b[23;0H                                ");
}

//files
bool fileExists(char const* path)
{
	if (!path) return false;

	FILE* f = fopen(path, "rb");
	if (!f)
		return false;

	fclose(f);
	return true;
}

int copyFile(char const* src, char const* dst)
{
	if (!src) return 1;

	unsigned long long size = getFileSizePath(src);
	return copyFilePart(src, 0, size, dst);
}

int copyFilePart(char const* src, u32 offset, u32 size, char const* dst)
{
	if (!src) return 1;
	if (!dst) return 2;

	FILE* fin = fopen(src, "rb");

	if (!fin)
	{
		fclose(fin);
		return 3;
	}
	else
	{
		if (fileExists(dst))
			remove(dst);

		FILE* fout = fopen(dst, "wb");

		if (!fout)
		{
			fclose(fin);
			fclose(fout);
			return 4;
		}
		else
		{
			fseek(fin, offset, SEEK_SET);

			consoleSelect(&topScreen);

			int bytesRead;
			unsigned long long totalBytesRead = 0;

			#define BUFF_SIZE 128 //Arbitrary. A value too large freezes the ds.
			char* buffer = (char*)malloc(BUFF_SIZE);

			while (1)
			{
				unsigned int toRead = BUFF_SIZE;
				if (size - totalBytesRead < BUFF_SIZE)
					toRead = size - totalBytesRead;

				bytesRead = fread(buffer, 1, toRead, fin);
				fwrite(buffer, bytesRead, 1, fout);

				totalBytesRead += bytesRead;
				printProgressBar( ((float)totalBytesRead / (float)size) );

				if (bytesRead != BUFF_SIZE)
					break;
			}

			clearProgressBar();
			consoleSelect(&bottomScreen);

			free(buffer);
		}

		fclose(fout);
	}

	fclose(fin);
	return 0;	
}

unsigned long long getFileSize(FILE* f)
{
	if (!f) return 0;

	fseek(f, 0, SEEK_END);
	unsigned long long size = ftell(f);
	fseek(f, 0, SEEK_SET);

	return size;
}

unsigned long long getFileSizePath(char const* path)
{
	if (!path) return 0;

	FILE* f = fopen(path, "rb");
	unsigned long long size = getFileSize(f);
	fclose(f);

	return size;
}

bool padFile(char const* path, int size)
{
	if (!path) return false;

	FILE* f = fopen(path, "ab");
	if (!f)
	{
		return false;
	}
	else
	{
		char zero = 0;
		fwrite(&zero, size, 1, f);
	}

	fclose(f);
	return true;
}

//directories
bool dirExists(char const* path)
{
	if (!path) return false;

	DIR* dir = opendir(path);

	if (!dir)
		return false;

	closedir(dir);
	return true;
}

bool deleteDir(char const* path)
{
	if (!path) return false;

	if (strcmp("/", path) == 0)
	{
		//oh fuck no
		return false;
	}

	bool result = true;

	DIR* dir = opendir(path);
	struct dirent* ent;

	if (!dir)
	{
		result = false;
	}
	else
	{
		while ( (ent = readdir(dir)) )
		{
			if (strcmp(".", ent->d_name) == 0 || strcmp("..", ent->d_name) == 0)
				continue;

			if (ent->d_type == DT_DIR)
			{
				//Delete directory
				char subpath[512];
				sprintf(subpath, "%s/%s", path, ent->d_name);

				if (!deleteDir(subpath))
					result = false;
			}
			else
			{
				//Delete file
				char fpath[512];
				sprintf(fpath, "%s/%s", path, ent->d_name);

				printf("%s...", fpath);
				if (remove(fpath) != 0)
				{
					printf("\x1B[31m");
					printf("Fail\n");
					printf("\x1B[47m");
					result = false;
				}
				else
				{
					printf("\x1B[42m");
					printf("Done\n");
					printf("\x1B[47m");
				}
			}
		}
	}

	closedir(dir);

	printf("%s...", path);
	if (remove(path) != 0)
	{
		printf("\x1B[31m");
		printf("Fail\n");
		printf("\x1B[47m");
		result = false;
	}
	else
	{
		printf("\x1B[42m");
		printf("Done\n");
		printf("\x1B[47m");
	}

	return result;
}

unsigned long long getDirSize(const char* path)
{
	if (!path) return 0;

	unsigned long long size = 0;
	DIR* dir = opendir(path);
	struct dirent* ent;

	if (dir)
	{
		while ((ent = readdir(dir)))
		{
			if(strcmp(".", ent->d_name) == 0 || strcmp("..", ent->d_name) == 0)
				continue;

			if (ent->d_type == DT_DIR)
			{
				char fullpath[512];
				sprintf(fullpath, "%s/%s", path, ent->d_name);

				size += getDirSize(fullpath);
			}
			else
			{				
				char fullpath[260];
				sprintf(fullpath, "%s/%s", path, ent->d_name);				

				size += getFileSizePath(fullpath);
			}
		}
	}

	closedir(dir);
	return size;
}

//home menu
int getMenuSlots()
{
	//Assume the home menu has a hard limit on slots
	//Find a better way to do this
	return TITLE_LIMIT;
}

int getMenuSlotsFree()
{
	//Get number of open menu slots by subtracting the number of directories in the title folders
	//Find a better way to do this
	const int NUM_OF_DIRS = 3;
	const char* dirs[] = {
		"00030004",
		"00030005",
		"00030015"
	};

	int freeSlots = getMenuSlots();
	
	DIR* dir;
	struct dirent* ent;	
	
	for (int i = 0; i < NUM_OF_DIRS; i++)
	{
		char path[256];
		sprintf(path, "/title/%s", dirs[i]);
		
		dir = opendir(path);
		
		if (dir)
		{
			while ( (ent = readdir(dir)) != NULL )
			{
				if(strcmp(".", ent->d_name) == 0 || strcmp("..", ent->d_name) == 0)
					continue;
				
				if (ent->d_type == DT_DIR)
					freeSlots -= 1;
			}			
		}

		closedir(dir);
	}
	
	return freeSlots;
}

//SD card
bool sdIsInserted()
{
	//Find a better way to do this.
	return true;
}

unsigned long long getSDCardSize()
{
	if (sdIsInserted())
	{
		struct statvfs st;
		if (statvfs("/", &st) == 0)
			return st.f_bsize * st.f_blocks;
	}

	return 0;
}

unsigned long long getSDCardFree()
{
	if (sdIsInserted())
	{
		struct statvfs st;
		if (statvfs("/", &st) == 0)
			return st.f_bsize * st.f_bavail;
	}

	return 0;
}

//internal storage
unsigned long long getDsiSize()
{
	//The DSi has 256MB of internal storage. Some is unavailable and used by other things.
	//An empty DSi reads 1024 open blocks
	return 1024 * BYTES_PER_BLOCK;
}

unsigned long long getDsiFree()
{
	//Get free space by subtracting file sizes in emulated nand folders
	unsigned long long size = getDsiSize();
	unsigned long long appSize = getDirSize("/title/00030004");

	//round up to a full block
	if (appSize % BYTES_PER_BLOCK != 0)
		appSize = ((int)(appSize / BYTES_PER_BLOCK) * BYTES_PER_BLOCK) + BYTES_PER_BLOCK;

	//subtract, but don't go under 0
	if (appSize > size)
	{
		size = 0;
	}
	else
	{
		size -= appSize;
	}

	return size;
}