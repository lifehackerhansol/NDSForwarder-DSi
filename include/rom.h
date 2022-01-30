#ifndef ROM_H
#define ROM_H

#include <nds/ndstypes.h>
#include <nds/memory.h>

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
} sNDSBannerExt;

#define NDS_BANNER_VER_DSi 0x0103
#define NDS_BANNER_SIZE_DSi 0x23C0

tDSiHeader* getRomHeader(char const* fpath);
tNDSHeader* getRomHeaderNDS(char const* fpath);
tNDSBanner* getRomBanner(char const* fpath);
tNDSBanner* getRomBannerNDS(char const* fpath);
sNDSBannerExt* getRomBannerDSi(char const* fpath);

bool getGameTitle(tNDSBanner* b, char* out, bool full);

void printRomInfo(char const* fpath);

unsigned long long getRomSize(char const* fpath);

#endif