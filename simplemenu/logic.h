#ifndef LOGIC
#define LOGIC
void quit();
char *determineExecutable(char *fileToBeExecutedWithFullPath);
void executeFavorite (char fileToBeExecutedWithFullPath[]);
void executeCommand (char executable[], char fileToBeExecutedWithFullPath[]);
void loadGameList();
#endif
