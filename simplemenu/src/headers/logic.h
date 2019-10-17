#ifndef LOGIC
#define LOGIC
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
#endif
