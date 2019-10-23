#include <ctype.h>
#include <stdlib.h>
#include <string.h>

char *toLower (char* string) {
	char *copy = malloc(2000);
	strcpy(copy, string);
	for(int i=0;copy[i]; i++) {
		copy[i] = tolower(copy[i]);
	}
	return copy;
}

char *getExtension (char *stringWithExtension) {
	return strrchr(stringWithExtension, '.');
}

char *getRomPath(char *fileName) {
	char *retstr;
	char *lastslash;
	retstr = malloc(strlen (fileName) + 1);
	strcpy (retstr, fileName);
	lastslash = strrchr (retstr, '/');
	if (lastslash != NULL) {
		*lastslash = '\0';
	}
	return retstr;
}

char *getNameWithoutExtension(char *fileName) {
	char *retstr;
	char *lastdot;
	retstr = malloc(strlen (fileName) + 1);
	strcpy (retstr, fileName);
	lastdot = strrchr (retstr, '.');
	if (lastdot != NULL) {
		*lastdot = '\0';
	}
	return retstr;
}

char *getNameWithoutPath(char *fileName) {
	char *e;
	e = strrchr(fileName, '/');
	if (e==NULL) {
		return fileName;
	} else {
		return e+1;
	}
}

char *getGameName(char *gameName) {
	char *tempGameName=malloc(strlen(gameName)+1);
	strcpy(tempGameName,getNameWithoutExtension(gameName));
	strcpy(tempGameName,getNameWithoutPath(gameName));
	return tempGameName;
}

void stripGameName(char *gameName) {
	strcpy(gameName,getNameWithoutExtension(gameName));
	strcpy(gameName,getNameWithoutPath(gameName));
	int charNumber = 0;
	while (gameName[charNumber]) {
		if (gameName[charNumber]=='('||charNumber>35) {
			gameName[charNumber-1]='\0';
			break;
		}
		charNumber++;
	}
}
