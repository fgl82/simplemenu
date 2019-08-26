#include <stdlib.h>
#include <string.h>

char *getExtension (char *stringWithExtension) {
	return strrchr(stringWithExtension, '.');
}

char *removeExtension(char *fileName) {
	char *retstr;
	char *lastdot;
	retstr = malloc (strlen (fileName) + 1);
	strcpy (retstr, fileName);
	lastdot = strrchr (retstr, '.');
	if (lastdot != NULL) {
		*lastdot = '\0';
	}
	return retstr;
}
