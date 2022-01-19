#ifndef ROM_H
#define ROM_H

#include <nds/ndstypes.h>
#include <nds/memory.h>

tDSiHeader* getRomHeader(char const* fpath);
tNDSHeader* getRomHeaderNDS(char const* fpath);
tNDSBanner* getRomBanner(char const* fpath);
tNDSBanner* getRomBannerNDS(char const* fpath);

bool getGameTitle(tNDSBanner* b, char* out, bool full);

void printRomInfo(char const* fpath);

unsigned long long getRomSize(char const* fpath);

#endif