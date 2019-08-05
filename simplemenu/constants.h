#ifndef CONSTANTS_DEFINED
#define CONSTANTS_DEFINED
static const int ITEMS_PER_PAGE=10;
static const char *consoles[] = {"9999 IN 1",
		                         "GAME BOY",
						         "GAME BOY COLOR",
						         "GAME BOY ADVANCE",
						         "NES",
								 "SNES",
								 "GAME GEAR",
								 "MASTER SYSTEM",
								 "SEGA GENESIS",
                                 "Playstation",
								 "NEO-GEO",
								 "PC ENGINE",
                                 "ATARI"};

static char *emulatorExecutables[] = {"",
		                              "/mnt/emus/gambatte/gambatte",
									  "/mnt/emus/gambatte/gambatte",
									  "/mnt/emus/gpsp/gpsp",
									  "/mnt/emus/fceux/fceux",
									  "/mnt/emus/snes9x4d/snes9x4d",
									  "/mnt/emus/sms/sms",
									  "/mnt/emus/sms/sms",
									  "/mnt/emus/picodrive/picodrive",
									  "/mnt/emus/pcsx_rearmed/pcsx",
									  "/mnt/emus/gngeo/gngeo",
									  "/mnt/emus/gngeo/temper",
									  "/mnt/emus/stella/stella",
									  NULL};

static char *romsDirectories[] = {"/mnt/roms/FAVORITES/",
		                          "/mnt/roms/GB/",
								  "/mnt/roms/GBC/",
								  "/mnt/roms/GBA/",
								  "/mnt/roms/NES/",
								  "/mnt/roms/SNES/",
								  "/mnt/roms/GG/",
								  "/mnt/roms/SMS/",
								  "/mnt/roms/SMD/",
								  "/mnt/roms/PS1/",
								  "/mnt/roms/NEOGEO/",
								  "/mnt/roms/PCE/",
								  "/mnt/roms/2600/"};
#endif
