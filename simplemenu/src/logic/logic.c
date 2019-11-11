#include <ctype.h>
#include <dirent.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>

#include "../headers/config.h"
#include "../headers/constants.h"
#include "../headers/control.h"
#include "../headers/definitions.h"
#include "../headers/globals.h"
#include "../headers/graphics.h"
#include "../headers/screen.h"
#include "../headers/string_utils.h"
#include "../headers/system_logic.h"
#include "../headers/opk.h"

void writeLog (char *line) {
#ifdef LOG_ON
	FILE *f;
	f = fopen("log.txt", "a");
	fprintf(f, "%s\n", line);
	fclose(f);
#endif
}

FILE *getCurrentSectionAliasFile() {
	FILE *aliasFile;
	aliasFile = fopen(CURRENT_SECTION.aliasFileName, "r");
	return aliasFile;
}

struct Favorite findFavorite(char *name) {
	struct Favorite favorite;
	for (int i=0; i<favoritesSize;i++) {
		favorite = favorites[i];
		if (strcmp(favorite.name, name)==0) {
			return favorite;
		}
	}
	return favorite;
}

char *search(char* arr[], int n, char *x)
{

	if (strcmp(arr[n - 1],x)==0)
		return "Found";

	char *backup = arr[n - 1];
	arr[n - 1] = x;

	for (int i = 0;; i++) {
		if (strcmp(arr[i],x)==0) {
			arr[n - 1] = backup;
			if (i < n - 1)
				return "Found";
			return "Not Found";
		}
	}
	return "Not Found";
}

char *getRomRealName(char *nameWithoutExtension) {
	char *nameTakenFromAlias;
	char* strippedNameWithoutExtension = malloc(strlen(nameWithoutExtension)+1);
	strcpy(strippedNameWithoutExtension,nameWithoutExtension);
	stripGameName(strippedNameWithoutExtension);
	nameTakenFromAlias=ht_get(CURRENT_SECTION.aliasHashTable,strippedNameWithoutExtension);
	if (nameTakenFromAlias!=NULL) {
		int charNumber=0;
		while (nameTakenFromAlias[charNumber]) {
			if (nameTakenFromAlias[charNumber]=='(') {
				nameTakenFromAlias[charNumber-1]='\0';
				break;
			}
			charNumber++;
		}
		charNumber=0;
		while (nameTakenFromAlias[charNumber]) {
			if (nameTakenFromAlias[charNumber]=='/') {
				nameTakenFromAlias[charNumber-1]='\0';
				break;
			}
			charNumber++;
		}
		free(strippedNameWithoutExtension);
		return(nameTakenFromAlias);
	}
	if(strippedNameWithoutExtension != NULL) {
		free(strippedNameWithoutExtension);
	}
	nameTakenFromAlias=malloc(strlen(nameWithoutExtension)+1);
	strcpy(nameTakenFromAlias, nameWithoutExtension);
	return(nameTakenFromAlias);
}

char *getOPKName(char *package_path) {
	struct OPK *opk = opk_open(package_path);
	char *name;
	const char *metadata_name;
	opk_open_metadata(opk, &metadata_name);
	const char *key, *val;
	size_t skey, sval;
	while(opk_read_pair(opk, &key, &skey, &val, &sval) && key) {
		if (!strncmp(key, "Name", skey)) {
			name=malloc((int)sval+1);
			strncpy(name,val,(int)sval);
			name[sval]='\0';
		}
	}
	opk_close(opk);
	return name;
}

char *getOPKCategory(char *package_path) {
	struct OPK *opk = opk_open(package_path);
	char *name;
	const char *metadata_name;
	opk_open_metadata(opk, &metadata_name);
	const char *key, *val;
	size_t skey, sval;
	while(opk_read_pair(opk, &key, &skey, &val, &sval) && key) {
		if (!strncmp(key, "Categories", skey)) {
			name=malloc((int)sval+1);
			strncpy(name,val,(int)sval);
			name[sval-1]='\0';
			printf("%s\n",name);
		}
	}
	opk_close(opk);
	return name;
}

char *getFileNameOrAlias(char *romName) {
	char *alias = malloc(500);
	if (strlen(CURRENT_SECTION.aliasFileName)>1) {
		strcpy(alias, getRomRealName(romName));
	} else {
		strcpy(alias, romName);
	}
	if(strcmp(alias, romName)==0) {
		if(currentSectionNumber==favoritesSectionNumber) {
			struct Favorite favorite = findFavorite(alias);
			if (strlen(favorite.alias)<2) {
				stripGameName(alias);
			}
		} else {
			char *ext = getExtension(romName);
			if (strcmp(ext,".opk")==0) {
				const char *opkName = getOPKName(romName);
				strcpy (alias, opkName);
			}
			stripGameName(alias);
		}
	}
	return alias;
}

void generateError(char *pErrorMessage, int pThereIsACriticalError) {
	itsStoppedBecauseOfAnError=1;
	errorMessage=pErrorMessage;
	thereIsACriticalError=pThereIsACriticalError;
}

char *getCurrentGameName() {
	char * name = malloc(strlen(CURRENT_GAME_NAME)+1);
	strcpy(name, CURRENT_GAME_NAME);
	stripGameName(name);
	return name;
}

void quit() {
	drawShutDownScreen();
	refreshScreen();
	//	freeResources();
	saveLastState();
	saveFavorites();
	clearTimer();
	execlp("sh", "sh", "-c", "sync && poweroff", NULL);
}

int doesFavoriteExist(char *name) {
	for(int i=0;i<favoritesSize;i++) {
		if (strcmp(favorites[i].name,name)==0) {
			return 1;
		}
	}
	return 0;
}

void setSectionsState(char *states) {
	char *endSemiColonStr;
	char *semiColonToken = strtok_r(states, ";", &endSemiColonStr);
	int i=0;
	while (semiColonToken != NULL)
	{
		char *endDashToken;
		char *dashToken = strtok_r(semiColonToken, "-", &endDashToken);
		int j=0;
		while (dashToken != NULL) {
			if (j==0) {
				menuSections[i].currentPage=atoi(dashToken);
			} else if (j==1) {
				menuSections[i].currentGame=atoi(dashToken);
			} else {
				menuSections[i].alphabeticalPaging=atoi(dashToken);
			}
			j++;
			dashToken = strtok_r(NULL, "-", &endDashToken);
		}
		semiColonToken = strtok_r(NULL, ";", &endSemiColonStr);
		i++;
	}
}

int checkIfEmulatorExists(char *path, char *executable) {
	char executableWithFullPath[300];
	strcpy(executableWithFullPath,path);
	strcat(executableWithFullPath,executable);
	FILE * fp;
	fp = fopen(executableWithFullPath, "r");
	if (fp==NULL) {
		return 0;
	}
	fclose(fp);
	return 1;
}

void executeCommand (char *emulatorFolder, char *executable, char *fileToBeExecutedWithFullPath) {
	char states[200]="";
	for (int i=0;i<favoritesSectionNumber+1;i++) {
		char tempString[200]="";
		snprintf(tempString,sizeof(tempString),"%d-%d-%d;",menuSections[i].currentPage,menuSections[i].currentGame,menuSections[i].alphabeticalPaging);
		strcat(states,tempString);
	}
	char pReturnTo[3];
	snprintf(pReturnTo,sizeof(pReturnTo),"%d;",returnTo);
	char pSectionNumber[3]="";
	char pPictureMode[2]="";
	snprintf(pSectionNumber,sizeof(pSectionNumber),"%d",currentSectionNumber);
	snprintf(pPictureMode,sizeof(pPictureMode),"%d",pictureMode);
	if (checkIfEmulatorExists(emulatorFolder,executable)||strcmp(emulatorFolder,"/usr/bin/")==0) {
		freeResources();
		execlp("./invoker.dge","invoker.dge", emulatorFolder, executable, fileToBeExecutedWithFullPath, states, pSectionNumber, pReturnTo, pPictureMode, NULL);
	} else {
		generateError("CONFIGURED EMULATOR NOT FOUND- CHECK SECTIONS.CFG ",0);
	}
}

int isExtensionValid(char *extension, char *fileExtensions) {
	char fileExtensionsCopy[200];
	strcpy(fileExtensionsCopy, fileExtensions);
	char *ptr = strtok(fileExtensionsCopy, ",");
	int i=0;
	while(ptr != NULL) {
		int areStringsDifferent = strcmp(extension,ptr);
		if (!areStringsDifferent) {
			return(1);
		}
		ptr = strtok(NULL, ",");
		i++;
	}
	return(0);
}

int countGamesInSection() {
	int gamesCounter=0;
	for (int i=0;i<=CURRENT_SECTION.totalPages;i++) {
		for (int j=0;j<ITEMS_PER_PAGE;j++) {
			if (CURRENT_SECTION.gameList[i][j]!=NULL) {
				gamesCounter++;
			}
		}
	}
	return gamesCounter;
}

void loadFavoritesSectionGameList() {
	int game = 0;
	int page = 0;
	FAVORITES_SECTION.totalPages=0;
	for (int i=0;i<1000;i++) {
		for (int j=0;j<10;j++) {
			FAVORITES_SECTION.gameList[i][j]=NULL;
		}
	}
	for (int i=0;i<favoritesSize;i++){
		if (game==ITEMS_PER_PAGE) {
			if(i!=favoritesSize) {
				page++;
				game = 0;
				FAVORITES_SECTION.totalPages++;
			}
		}
		if (strlen(favorites[i].alias)>1) {
			FAVORITES_SECTION.gameList[page][game] = favorites[i].alias;
		} else {
			FAVORITES_SECTION.gameList[page][game] = favorites[i].name;
		}
		game++;
	}
}

int recursivelyScanDirectory (char *directory, char* files[], int i) {
	DIR * d;
	d = opendir (directory);
	if (d==NULL) {
		return 0;
	}
	while (1) {
		struct dirent *entry;
		char * d_name;
		entry = readdir (d);
		if (!entry) {
			break;
		}
		d_name = entry->d_name;
		if (entry->d_type & DT_DIR) {
			if (strcmp(d_name,"media") != 0 && strcmp (d_name, "..") != 0 && strcmp (d_name, ".") != 0) {
				char path[PATH_MAX];
				char * e = strrchr(d_name, '/');
				if (e==NULL) {
					strcat(d_name, "/");
				}
				snprintf (path, PATH_MAX, "%s%s", directory, d_name);
				i=+recursivelyScanDirectory(path, files, i);
			}
		} else {
			char path[PATH_MAX];
			snprintf (path, PATH_MAX, "%s%s", directory, d_name);
			files[i]=malloc(sizeof(path));
			strcpy(files[i],path);
			i++;
		}
	}
	free(d);
	return i;
}

int genericGameCompare(char *game1, char *game2) {
	char* s1 = game1;
	char* s2 = game2;
	int pos1=positionWhereGameNameStartsInFullPath(s1);
	int pos2=positionWhereGameNameStartsInFullPath(s2);
	s1+=pos1;
	s2+=pos2;
	char *temp1 = malloc(strlen(s1)+1);
	strcpy(temp1,s1);
	char *temp2 = malloc(strlen(s2)+1);
	strcpy(temp2,s2);
	for(int i=0;temp1[i]; i++) {
		temp1[i] = tolower(temp1[i]);
	}
	for(int i=0;temp2[i]; i++) {
		temp2[i] = tolower(temp2[i]);
	}
	return strcmp(temp1, temp2);;
}

int genericGameCompareWithAlias(char *game1, char *game2) {
	char* s1 = game1;
	char* s2 = game2;
	char *temp1 = malloc(strlen(s1)+1);
	strcpy(temp1,s1);
	char *temp2 = malloc(strlen(s2)+1);
	strcpy(temp2,s2);
	for(int i=0;temp1[i]; i++) {
		temp1[i] = tolower(temp1[i]);
	}
	for(int i=0;temp2[i]; i++) {
		temp2[i] = tolower(temp2[i]);
	}
	return strcmp(temp1, temp2);;
}

int compareFavorites(const void *f1, const void *f2)
{
	struct Favorite *e1 = (struct Favorite *)f1;
	struct Favorite *e2 = (struct Favorite *)f2;
	char temp1[300]="";
	char temp2[300]="";
	if (strlen(e1->alias)>1) {
		strcpy(temp1,e1->alias);
	} else {
		strcpy(temp1,e1->name);
		stripGameName(temp1);
	}
	if (strlen(e2->alias)>1) {
		strcpy(temp2,e2->alias);
	} else {
		strcpy(temp2,e2->name);
		stripGameName(temp2);
	}
	for(int i=0;temp1[i]; i++) {
		temp1[i] = tolower(temp1[i]);
	}
	for(int i=0;temp2[i]; i++) {
		temp2[i] = tolower(temp2[i]);
	}
	//	temp1[0]=tolower(temp1[0]);
	//	temp2[0]=tolower(temp2[0]);
	return strcmp(temp1, temp2);
}

int compareGamesFromGameList (const void *game1, const void *game2) {
	char* s1 = (char *)(*(char **)game1);
	char* s2 = (char *)(*(char **)game2);
	return genericGameCompare(s1, s2);
}

int compareGamesFromGameListBasedOnAlias (const void *game1, const void *game2) {
	char s1Alias[300];
	strcpy(s1Alias,(char *)(*(char **)game1));
	char s2Alias[300];
	strcpy(s2Alias,(char *)(*(char **)game2));
	stripGameName(s1Alias);
	stripGameName(s2Alias);
	char *temp = getRomRealName(s1Alias);
	strcpy(s1Alias, temp);
	temp = getRomRealName(s2Alias);
	strcpy(s2Alias, temp);
	return genericGameCompareWithAlias(s1Alias, s2Alias);
}

void loadGameList(int refresh) {
	int loadedFiles=0;
	if (CURRENT_SECTION.gameList[0][0] == NULL||refresh) {
		loadAliasList(currentSectionNumber);
		CURRENT_SECTION.totalPages=0;
		char *files[10000];
		int n = recursivelyScanDirectory(CURRENT_SECTION.filesDirectory, files, 0);
		int game = 0;
		int page = 0;
		for (int i=0;i<n;i++){
			char *ext = getExtension(files[i]);
			if (ext&&strcmp((files[i]),"..")!=0 &&
					strcmp((files[i]),".")!=0 &&
					strcmp(ext,".png")!=0&&
					isExtensionValid(ext,CURRENT_SECTION.fileExtensions)){
				if(strcmp(ext,".opk")==0) {
					char *category = getOPKCategory(files[i]);
					if(strcmp(category,"emulators")==0) {
						continue;
					}
				}
				int size = strlen(files[i])+1;
				CURRENT_SECTION.gameList[page][game]=malloc(size);
				if (game==ITEMS_PER_PAGE) {
					if(i!=n) {
						page++;
						CURRENT_SECTION.totalPages++;
						game = 0;
					}
				}
				strcpy(CURRENT_SECTION.gameList[page][game],files[i]);
				loadedFiles++;
				strcat(CURRENT_SECTION.gameList[page][game],"\0");
				game++;
			}
		}
		for (int i=0;i<n;i++){
			free(files[i]);
		}
		if (loadedFiles==0) {
			CURRENT_SECTION.hidden=1;
			return;
		}
		if (strlen(CURRENT_SECTION.aliasFileName)>1) {
			qsort(menuSections[currentSectionNumber].gameList, countGamesInSection(), sizeof(char *), compareGamesFromGameListBasedOnAlias);
		} else {
			qsort(menuSections[currentSectionNumber].gameList, countGamesInSection(), sizeof(char *), compareGamesFromGameList);
		}
	}
}

int countGamesInPage() {
	int gamesCounter=0;
	for (int i=0;i<ITEMS_PER_PAGE;i++) {
		if (CURRENT_SECTION.gameList[menuSections[currentSectionNumber].currentPage][i]!=NULL) {
			gamesCounter++;
		}
	}
	return gamesCounter;
}

int countGamesInSpecificPage(int page) {
	int gamesCounter=0;
	for (int i=0;i<ITEMS_PER_PAGE;i++) {
		if (CURRENT_SECTION.gameList[page][i]!=NULL) {
			gamesCounter++;
		}
	}
	return gamesCounter;
}

int getFirstNonHiddenSection(int sectionCount) {
	for (int i=0;i<sectionCount;i++) {
		if (menuSections[i].hidden==0) {
			return i;
		}
	}
	return 0;
}

void selectRandomGame() {
	int gamesInSection = countGamesInSection();
	if (gamesInSection%10==0) {
		CURRENT_SECTION.currentPage = rand() % ((int)gamesInSection/10);
	} else {
		CURRENT_SECTION.currentPage = rand() % ((int)(gamesInSection/10) +1);
	}
	CURRENT_SECTION.currentGame = rand() % countGamesInPage();
}

void determineStartingScreen(int sectionCount) {
	if (sectionCount==-1) {
		generateError("SECTIONS FILE NOT FOUND-SHUTTING DOWN",1);
		return;
	}
	if(sectionCount==0||currentSectionNumber==favoritesSectionNumber) {
		favoritesSectionSelected=1;
		loadFavoritesSectionGameList();
		if (countGamesInPage()==0) {
			favoritesSectionSelected=0;
			currentSectionNumber=0;
			determineStartingScreen(sectionCount);
		}
	} else {
		loadGameList(0);
		if(CURRENT_SECTION.hidden) {
			int advanced = advanceSection();
			loadGameList(0);
			if(advanced) {
				while(CURRENT_SECTION.hidden) {
					if(currentSectionNumber==0||currentSectionNumber==favoritesSectionNumber) {
						generateError("NO ROMS WERE FOUND-SHUTTING DOWN",1);
						break;
					}
					advanceSection();
					loadGameList(0);
				}
			}
		}
	}
}

void deleteCurrentGame() {
	char command [300];
	snprintf(command,sizeof(command),"rm \"%s\";",CURRENT_GAME_NAME);
	system(command);
	CURRENT_SECTION.gameList[CURRENT_SECTION.totalPages][countGamesInSpecificPage(CURRENT_SECTION.totalPages)-1]=NULL;
	char *pictureWithFullPath=malloc(600);
	char *tempGameName=malloc(300);
	if (favoritesSectionSelected) {
		if (favoritesSize == 0) {
			return;
		}
		struct Favorite favorite = findFavorite(CURRENT_GAME_NAME);
		strcpy(pictureWithFullPath, favorite.filesDirectory);
		tempGameName=getGameName(favorite.name);
	} else {
		strcpy(pictureWithFullPath, CURRENT_SECTION.filesDirectory);
		tempGameName=getGameName(CURRENT_GAME_NAME);
	}
	strcat(pictureWithFullPath,"media/");
	tempGameName=getNameWithoutExtension(tempGameName);
	strcat(pictureWithFullPath,tempGameName);
	strcat(pictureWithFullPath,".png");
	snprintf(command,sizeof(command),"rm \"%s\";",pictureWithFullPath);
	system(command);
}
