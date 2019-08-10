#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <globals.h>

void saveLastState() {
	FILE * fp;
	fp = fopen("./config/last_state.cfg", "w");
	fprintf(fp, "%d;%d", currentSection, currentGame);
	fclose(fp);
}

void loadLastState() {
	FILE * fp;
	char * line = NULL;
	size_t len = 0;
	ssize_t read;
	fp = fopen("./config/last_state.cfg", "r");
	char *configurations[2];
	while ((read = getline(&line, &len, fp)) != -1) {
		char *ptr = strtok(line, ";");
		int i=0;
		while(ptr != NULL)
		{
			configurations[i]=ptr;
			ptr = strtok(NULL, ";");
			i++;
		}
	}
	currentSection=atoi(configurations[0]);
	currentGame=atoi(configurations[1]);
	fclose(fp);
	if (line) {
		free(line);
	}
}

void loadConfig() {
	FILE * fp;
	char * line = NULL;
	size_t len = 0;
	ssize_t read;
	struct MenuSection aMenuSection;
	fp = fopen("./config/sections.cfg", "r");
	int menuSectionCounter = 0;
	while ((read = getline(&line, &len, fp)) != -1) {
		char *ptr = strtok(line, ";");
		char *configurations[22];
		int i=0;
		while(ptr != NULL)
		{
			configurations[i]=ptr;
			ptr = strtok(NULL, ";");
			i++;
		}
		strcpy(aMenuSection.sectionName,configurations[0]);
		strcpy(aMenuSection.emulatorFolder,configurations[1]);
		strcpy(aMenuSection.executable,configurations[2]);
		strcpy(aMenuSection.filesDirectory,configurations[3]);
		strcpy(aMenuSection.fileExtension,configurations[4]);
		aMenuSection.headerAndFooterTextBackgroundColor.r=atoi(configurations[5]);
		aMenuSection.headerAndFooterTextBackgroundColor.g=atoi(configurations[6]);
		aMenuSection.headerAndFooterTextBackgroundColor.b=atoi(configurations[7]);
		aMenuSection.headerAndFooterTextForegroundColor.r=atoi(configurations[8]);
		aMenuSection.headerAndFooterTextForegroundColor.g=atoi(configurations[9]);
		aMenuSection.headerAndFooterTextForegroundColor.b=atoi(configurations[10]);
		aMenuSection.bodyBackgroundColor.r=atoi(configurations[11]);
		aMenuSection.bodyBackgroundColor.g=atoi(configurations[12]);
		aMenuSection.bodyBackgroundColor.b=atoi(configurations[13]);
		aMenuSection.bodyTextForegroundColor.r=atoi(configurations[14]);
		aMenuSection.bodyTextForegroundColor.g=atoi(configurations[15]);
		aMenuSection.bodyTextForegroundColor.b=atoi(configurations[16]);
		aMenuSection.bodySelectedTextBackgroundColor.r=atoi(configurations[17]);
		aMenuSection.bodySelectedTextBackgroundColor.g=atoi(configurations[18]);
		aMenuSection.bodySelectedTextBackgroundColor.b=atoi(configurations[19]);
		aMenuSection.bodySelectedTextForegroundColor.r=atoi(configurations[20]);
		aMenuSection.bodySelectedTextForegroundColor.g=atoi(configurations[21]);
		aMenuSection.bodySelectedTextForegroundColor.b=atoi(configurations[22]);
		menuSections[menuSectionCounter]=aMenuSection;
		menuSectionCounter++;
	}
	strcpy(menuSections[menuSectionCounter].sectionName,"END");
	fclose(fp);
	if (line) {
		free(line);
	}
}
