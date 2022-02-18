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

#ifndef STORAGE_H
#define STORAGE_H

#include <nds/ndstypes.h>
#include <stdio.h>

#define BYTES_PER_BLOCK (1024*128)

//printing
void printBytes(unsigned long long bytes);

//progress bar
void printProgressBar(float percent);
void clearProgressBar();

//Files
bool fileExists(char const* path);
int copyFile(char const* src, char const* dst);
int copyFilePart(char const* src, u32 offset, u32 size, char const* dst);
unsigned long long getFileSize(FILE* f);
unsigned long long getFileSizePath(char const* path);
bool padFile(char const* path, int size);

//Directories
bool dirExists(char const* path);
bool deleteDir(char const* path);
unsigned long long getDirSize(char const* path);

//home menu
int getMenuSlots();
int getMenuSlotsFree();
#define getMenuSlotsUsed() (getMenuSlots() - getMenuSlotsFree())

//SD card
bool sdIsInserted();
unsigned long long getSDCardSize();
unsigned long long getSDCardFree();
#define getSDCardUsedSpace() (getSDCardSize() - getSDCardFree())

//internal storage
unsigned long long getDsiSize();
unsigned long long getDsiFree();
#define getDsiUsed() (getDSIStorageSize() - getDSIStorageFree())

#endif