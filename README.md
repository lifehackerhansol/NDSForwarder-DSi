# NDSForwarder for hiyaCFW and flashcards
A basic nds-bootstrap forwarder generator for DSi SDNAND and flashcards.

## Features
- Generate forwarders directly on the SD card (games, old NDS homebrew, etc)
- View basic title header info.

## Usage
- **Flashcard:** https://wiki.ds-homebrew.com/ds-index/forwarders.html?tab=flashcard
    - See [this list](https://github.com/DS-Homebrew/TWiLightMenu/blob/master/universal/include/compatibleDSiWareMap.h) for which DSiWare titles work on flashcards.
- **DSi SD Card:** https://wiki.ds-homebrew.com/ds-index/forwarders.html?tab=dsi-sd-card

## Notes
- Backup your SD card! Nothing bad should happen, but this is an early release so who knows.
- This will install *everything* as a forwarder, even DSiWare. If you use DSi SDNAND and want to install DSiWare natively, please use [NAND Title Manager](https://github.com/Epicpkmn11/NTM).
- This is only for DSi systems, as well as flashcards which are usable on any DS model.
- On flashcards, forwarders get installed to the `forwarders` folder on the SD root.
- See [this list](https://github.com/DS-Homebrew/TWiLightMenu/blob/master/universal/include/compatibleDSiWareMap.h) for which DSiWare titles work on flashcards.

## Credits

- [devkitPro](https://devkitpro.org/) for devkitARM toolchain and libnds library
- [Tuxality](https://github.com/Tuxality/maketmd) for MakeTMD
- [Martin Korth (nocash)](https://problemkaputt.de/gbatek.htm) for extensive NDS documentation
- [JeffRulz](https://github.com/JeffRulz/TMFH) for Title Manager for hiyaCFW, which this is a fork of
- [Rocket Robz](https://github.com/RocketRobz/NTR_Forwarder) for NTR Forwarder, the forwarders themselves, and adding flashcard support
- [MechanicalDragon](https://github.com/MechanicalDragon0687/NDSForwarder) for NDSForwarder (the 3DS one) which gave inspiration for this app
