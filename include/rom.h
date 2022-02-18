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

#ifndef ROM_H
#define ROM_H

#include <nds/ndstypes.h>
#include <nds/memory.h>

// sNDSBannerExt from TWiLight Menu++
// https://github.com/DS-Homebrew/TWiLightMenu/blob/587a3ab0bd74d5f74f6eb1dd34ad9673b18c4606/romsel_dsimenutheme/arm9/source/ndsheaderbanner.h
typedef struct {
	u16 version;		//!< version of the banner.
	u16 crc[4];		//!< CRC-16s of the banner.
	u8 reserved[22];
	u8 icon[512];		//!< 32*32 icon of the game with 4 bit per pixel.
	u16 palette[16];	//!< the palette of the icon.
	u16 titles[8][128];	//!< title of the game in 8 different languages.

	// [0xA40] Reserved space, possibly for other titles.
	u8 reserved2[0x800];

	// DSi-specific.
	u8 dsi_icon[8][512];	//!< DSi animated icon frame data.
	u16 dsi_palette[8][16];	//!< Palette for each DSi icon frame.
	u16 dsi_seq[64];	//!< DSi animated icon sequence.
	u8 reserved3[64];
} sNDSBannerExt;

// sNDSBanner version.
typedef enum {
	NDS_BANNER_VER_ORIGINAL	= 0x0001,
	NDS_BANNER_VER_ZH		= 0x0002,
	NDS_BANNER_VER_ZH_KO	= 0x0003,
	NDS_BANNER_VER_DSi		= 0x0103,
} sNDSBannerVersion;

// sNDSBanner sizes.
typedef enum {
	NDS_BANNER_SIZE_ORIGINAL	= 0x0840,
	NDS_BANNER_SIZE_ZH			= 0x0940,
	NDS_BANNER_SIZE_ZH_KO		= 0x0A40,
	NDS_BANNER_SIZE_DSi			= 0x23C0,
} sNDSBannerSize;

tDSiHeader* getRomHeader(char const* fpath);
tNDSHeader* getRomHeaderNDS(char const* fpath);
sNDSBannerExt* getRomBanner(char const* fpath);

bool getGameTitle(sNDSBannerExt* b, char* out, bool full);

void printRomInfo(char const* fpath);

unsigned long long getRomSize(char const* fpath);

#endif