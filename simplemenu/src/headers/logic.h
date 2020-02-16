#include "../headers/globals.h"
#ifndef LOGIC
#define LOGIC
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
void deleteGame();
int compareFavorites(const void *s1, const void *s2);
FILE *getCurrentSectionAliasFile();
char *getRomRealName(char *nameWithoutExtension);
char *getAlias(char *romName);
char *getFileNameOrAlias(struct Rom *rom);
int theCurrentSectionHasGames();
#endif
