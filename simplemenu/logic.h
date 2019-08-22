#ifndef LOGIC
#define LOGIC
void quit();
void executeCommand (char *emulatorFolder, char *executable, char fileToBeExecutedWithFullPath[]);
void loadGameList();
void loadFavoritesList();
int countFiles (char* directoryName);
int countGamesInPage();
int doesFavoriteExist(char *name);
void determineStartingScreen(int sectionCount);
#endif
