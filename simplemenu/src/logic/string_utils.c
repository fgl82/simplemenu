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

char *removeExtension(char *fileName) {
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

char *removePath(char *fileName) {
	//	char *retstr;
	//	char *lastdot;
	//	retstr = malloc(strlen (fileName) + 1);
	//	strcpy (retstr, fileName);
	//	lastdot = strrchr (retstr, '//');
	//	if (lastdot != NULL) {
	//		*lastdot = '\0';
	//	}
	//	printf("%s\n",retstr);
	//	return retstr;
	char *e;
	e = strrchr(fileName, '/');
	if (e==NULL) {
		return fileName;
	} else {
//		printf("%s\n",e+1);
		return e+1;
	}
}

void stripGameName(char *gameName) {
	strcpy(gameName,removeExtension(gameName));
	strcpy(gameName,removePath(gameName));
	int charNumber = 0;
	while (gameName[charNumber]) {
		if (gameName[charNumber]=='('||charNumber>35) {
			gameName[charNumber-1]='\0';
			break;
		}
		charNumber++;
	}
}
