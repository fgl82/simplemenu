#ifndef STRING_UTILS
#define STRING_UTILS
char *toLower (char* string);
int positionWhereGameNameStartsInFullPath (char* string);
char *getNameWithoutExtension(char *fileName);
char *getNameWithoutPath(char *fileName);
char *getRomPath (char *fileName);
char *getExtension (char *stringWithExtension);
void stripGameName(char *gameName);
char *getGameName(char *gameName);
#endif
