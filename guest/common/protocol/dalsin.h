/* AmigaChrome Team: AmigaChrome's own Zorro manufacturer number, 0xDA15 (55829),
 * from 1 October 2026. Before that the boards used 2011, the number UAE and
 * home-made test boards share (ShowConfig calls it "UAE Amiga Emulator",
 * Linux's zorro.ids "Hacker Test Board"). 0xDA15 is in neither list.
 *
 * Products stay as they were: 1 ACBridge control, 2 ACStorage, 9 ACRTG,
 * 11 AC090's RAM.
 *
 * Drivers look for the AmigaChrome Team first, then 2011, so one build works with either.
 * A boot ROM whose drivers do carries DALSIN_ROM_MARK; the runtime presents
 * AmigaChrome Team to a ROM that has it and 2011 to one built before. */
#ifndef AMIGACHROME_DALSIN_H
#define AMIGACHROME_DALSIN_H

#define DALSIN_MANUFACTURER      0xDA15     /* 55829 */
#define DALSIN_MANUFACTURER_OLD  2011
#define DALSIN_ROM_MARK          "Zorro manufacturer Dalsin $DA15"

#endif
