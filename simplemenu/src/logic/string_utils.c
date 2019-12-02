#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "../headers/globals.h"

char *replaceWord(const char *s, const char *oldW, const char *newW) {
    char *result;
    int i, cnt = 0;
    int newWlen = strlen(newW);
    int oldWlen = strlen(oldW);
    for (i = 0; s[i] != '\0'; i++) {
        if (strstr(&s[i], oldW) == &s[i]) {
            cnt++;
            i += oldWlen - 1;
        }
    }
    result = (char *)malloc(i + cnt * (newWlen - oldWlen) + 1);
    i = 0;
    while (*s) {
        if (strstr(s, oldW) == s) {
            strcpy(&result[i], newW);
            i += newWlen;
            s += oldWlen;
        } else {
            result[i++] = *s++;
        }
    }
    result[i] = '\0';
    return result;
}

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
	char *result;
	e = strrchr(fileName, '/');
	if (e==NULL) {
		result=malloc(strlen(fileName)+1);
		strcpy(result,fileName);
		return result;
	} else {
		result=malloc(strlen(e+1)+1);
		strcpy(result,e+1);
		return result;
	}
}

char *getGameName(char *gameName) {
	char *tempGameName=malloc(strlen(gameName)+1);
	char *nameWithoutPath;
	char *nameWithoutExtension;
	nameWithoutExtension=getNameWithoutExtension(gameName);
	nameWithoutPath=getNameWithoutPath(nameWithoutExtension);
	nameWithoutPath=getNameWithoutPath(gameName);
//	strcpy(tempGameName,nameWithoutExtension);
	strcpy(tempGameName,nameWithoutPath);
//	free(nameWithoutExtension);
//	free(nameWithoutPath);
	return tempGameName;
}

void stripGameName(char *gameName) {
	char *nameWithoutExtension=getNameWithoutExtension(gameName);
	char *nameWithoutPath=getNameWithoutPath(nameWithoutExtension);
//	strcpy(gameName,nameWithoutExtension);
	strcpy(gameName,nameWithoutPath);
	int charNumber = 0;
	while (gameName[charNumber]) {
		if (gameName[charNumber]=='(') {
			gameName[charNumber-1]='\0';
			break;
		}
		charNumber++;
	}
	free(nameWithoutExtension);
	free(nameWithoutPath);
}

void stripGameNameLeaveExtension(char *gameName) {
	char *nameWithoutPath=getNameWithoutPath(gameName);
	strcpy(gameName,nameWithoutPath);
	int charNumber = 0;
	while (gameName[charNumber]) {
		if (gameName[charNumber]=='(') {
			gameName[charNumber-1]='\0';
			break;
		}
		charNumber++;
	}
	free(nameWithoutPath);
}

int positionWhereGameNameStartsInFullPath (char* string) {
	int counter=strlen(string);
	for (int i=strlen(string);i>0;i--) {
		if(string[i-1]=='/') {
			break;
		}
		counter--;
	}
	return(counter);
}
