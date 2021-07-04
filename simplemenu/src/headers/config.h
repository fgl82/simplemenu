#ifndef CONFIG
#define CONFIG
void saveLastState();
void loadLastState();
void saveFavorites();
void loadFavorites();
int loadSections();
int loadConfig();
void loadAliasList(int sectionNumber);
void createConfigFilesInHomeIfTheyDontExist();
void checkIfDefault();
void loadTheme(char *theme);
void loadSectionGroups();
void checkThemes();
struct AutostartRom *getLaunchAtBoot();
#endif
