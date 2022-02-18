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

#ifndef SAV_H
#define SAV_H

#include <nds/ndstypes.h>
#include <stdio.h>
//http://elm-chan.org/docs/fat_e.html

#pragma pack(push, 1)
typedef struct
{
	u8 BS_JmpBoot[3];	//0x0000
	u8 BS_OEMName[8];	//0x0003
	u16 BPB_BytesPerSec;	//0x000B
	u8 BPB_SecPerClus;	//0x000D
	u16 BPB_RsvdSecCnt;	//0x000E
	u8 BPB_NumFATs;
	u16 BPB_RootEntCnt;
	u16 BPB_TotSec16;
	u8 BPB_Media;
	u16 BPB_FATSz16;
	u16 BPB_SecPerTrk;
	u16 BPB_NumHeads;
	u32 BPB_HiddSec;
	u32 BPB_TotSec32;
	u8 BS_DrvNum;
	u8 BS_Reserved1;
	u8 BS_BootSig;
	u32 BS_VolID;
	u8 BS_VolLab[11];
	u8 BS_FilSysType[8];
	u8 BS_BootCode[448];
	u16 BS_BootSign;
} FATHeader;
#pragma pack(push, 0)

bool initFatHeader(FILE* f);

#endif