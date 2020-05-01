#ifndef CONFIG
#define CONFIG
void saveLastState();
void loadLastState();
void saveFavorites();
void loadFavorites();
int loadSections();
int loadConfig();
int readInputConfig();
void loadAliasList(int sectionNumber);
void createConfigFilesInHomeIfTheyDontExist();
void checkIfDefault();
void loadTheme(char *theme);
void checkThemes();
#endif
