#ifndef LOGIC
#define LOGIC
void writeLog (char *line);
void generateError(char *pErrorMessage, int pThereIsACriticalError);
void quit();
void executeCommand (char *emulatorFolder, char *executable, char *fileToBeExecutedWithFullPath);
void loadGameList();
void loadFavoritesSectionGameList();
int countFiles (char* directoryName, char *fileExtensions);
int countGamesInPage();
int countGamesInSection();
int doesFavoriteExist(char *name);
void setSectionsState(char *states);
void determineStartingScreen(int sectionCount);
int getFirstNonHiddenSection(int sectionCount);
struct Favorite findFavorite(char *name);
void selectRandomGame();
void deleteCurrentGame();
int compareFavorites(const void *s1, const void *s2);
FILE *getCurrentSectionAliasFile();
char *getRomRealName(char *nameWithoutExtension);
char *getFileNameOrAlias(char *romName);
char *getOPKName(char *package_path);
char *getOPKCategory(char *package_path);
#endif
