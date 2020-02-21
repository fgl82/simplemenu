#define _GNU_SOURCE
#include <ctype.h>
#include <dirent.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <time.h>
#include <stdio.h>

#include "../headers/config.h"
#include "../headers/control.h"
#include "../headers/constants.h"
#include "../headers/definitions.h"
#include "../headers/globals.h"
#include "../headers/graphics.h"
#include "../headers/screen.h"
#include "../headers/string_utils.h"
#include "../headers/system_logic.h"
#include "../headers/opk.h"
#include "../headers/doubly_linked_rom_list.h"


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
			if (nameTakenFromAlias[charNumber]=='('||nameTakenFromAlias[charNumber]=='[') {
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

int getOPK(char *package_path, struct OPKDesktopFile* desktopFiles) {
	struct OPK *opk = opk_open(package_path);
	char *name;
	char *category;
	int i=0;
	while (1) {
		const char *metadata_name;
		if (opk_open_metadata(opk, &metadata_name) <= 0) {
			break;
		}
		strcpy(desktopFiles[i].parentOPK,package_path);
		strcpy(desktopFiles[i].name,metadata_name);
		const char *key=NULL;
		const char *val=NULL;
		size_t skey, sval;
		while(opk_read_pair(opk, &key, &skey, &val, &sval) && key) {
			if (!strncmp(key, "Name", skey)) {
				name=malloc((int)sval+1);
				strncpy(name,val,(int)sval);
				name[sval]='\0';
				strcpy(desktopFiles[i].displayName,name);
				free(name);
			}
			if (!strncmp(key, "Categories", skey)) {
				category=malloc((int)sval+1);
				strncpy(category,val,(int)sval);
				category[sval]='\0';
				strcpy(desktopFiles[i].category,category);
				free(category);
			}
		}
		i++;
	}
	opk_close(opk);
	return i;
}

char *getAlias(char *romName) {
	char *alias = malloc(300);
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
				strcpy (alias, romName);
			}
			stripGameName(alias);
		}
	}
	return alias;
}

char *getFileNameOrAlias(struct Rom *rom) {
	char *displayName = malloc(300);
	if(rom->alias!=NULL&&strlen(rom->alias)>2) {
		//this is a favorite with no alias
		if(strcmp(rom->alias, " ")!=0) {
			strcpy(displayName, rom->alias);
		} else {
			strcpy(displayName, rom->name);
		}
	} else {
		strcpy(displayName, rom->name);
	}
	if(strcmp(displayName, rom->name)==0) {
		//		if(stripGames) {
		stripGameName(displayName);
		//		} else {
		//			char tmp[300];
		//			strcpy(tmp, getNameWithoutPath(displayName));
		//			strcpy(displayName, tmp);
		//			printf("returning %s\n",displayName);
		//		}
	}
	return displayName;
}

void generateError(char *pErrorMessage, int pThereIsACriticalError) {
	itsStoppedBecauseOfAnError=1;
	errorMessage=pErrorMessage;
	thereIsACriticalError=pThereIsACriticalError;
}

//char *getCurrentGameName() {
//	struct Rom* currentGame = GetNthElement(CURRENT_GAME_NUMBER);
//	char * name = malloc(strlen(currentGame->name)+1);
//	strcpy(name, currentGame->name);
//	stripGameName(name);
//	return name;
//}

void quit() {
	drawShutDownScreen();
	refreshScreen();
	saveLastState();
	saveFavorites();
	clearTimer();
	clearPicModeHideLogoTimer();
	clearPicModeHideMenuTimer();
	sleep(1.5);
	freeResources();
	if (shutDownEnabled) {
		execlp("sh", "sh", "-c", "sync && poweroff", NULL);
	} else {
		exit(0);
	}
}

int doesFavoriteExist(char *name) {
	for(int i=0;i<favoritesSize;i++) {
		if (strstr(name,favorites[i].name)) {
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
				menuSections[i].currentGameInPage=atoi(dashToken);
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
		snprintf(tempString,sizeof(tempString),"%d-%d-%d;",menuSections[i].currentPage,menuSections[i].currentGameInPage,menuSections[i].alphabeticalPaging);
		strcat(states,tempString);
	}
	char pReturnTo[3];
	snprintf(pReturnTo,sizeof(pReturnTo),"%d;",returnTo);
	char pSectionNumber[3]="";
	char pPictureMode[2]="";
	snprintf(pSectionNumber,sizeof(pSectionNumber),"%d",currentSectionNumber);
	snprintf(pPictureMode,sizeof(pPictureMode),"%d",pictureMode);
	saveLastState();
#ifndef TARGET_PC
	saveFavorites();
	clearTimer();
	freeResources();
	setCPU(currentCPU);
	execlp("./invoker.dge","invoker.dge", emulatorFolder, executable, fileToBeExecutedWithFullPath, states, pSectionNumber, pReturnTo, pPictureMode, NULL);
#else
	printf("%s\n",fileToBeExecutedWithFullPath);
	loadLastState();
#endif
}

int isExtensionValid(char *extension, char *fileExtensions) {
	char fileExtensionsCopy[200];
	strcpy(fileExtensionsCopy, fileExtensions);
	char *ptr = strtok(fileExtensionsCopy, ",");
	while(ptr != NULL) {
		int areStringsDifferent = strcmp(extension,ptr);
		if (!areStringsDifferent) {
			return(1);
		}
		ptr = strtok(NULL, ",");
	}
	return(0);
}

int countGamesInSection() {
	int gamesCounter=0;
	struct Node* currentGameNode = CURRENT_SECTION.head;
	for (int i=0;i<=10000;i++) {
		if (currentGameNode!=NULL&&strlen(currentGameNode->data->name)>0) {
			gamesCounter++;
			currentGameNode = currentGameNode->next;
		} else {
			break;
		}
	}
	return gamesCounter;
}

struct Node *split(struct Node *head)
{
	struct Node *fast = head,*slow = head;
	while (fast->next && fast->next->next)
	{
		fast = fast->next->next;
		slow = slow->next;
	}
	struct Node *temp = slow->next;
	slow->next = NULL;
	return temp;
}

struct Node *merge(struct Node *first, struct Node *second)
{
	if (!first) {
		return second;
	}

	if (!second) {
		return first;
	}

	char s1Alias[300];
	if(first->data->alias!=NULL&&strlen(first->data->alias)>2) {
		strcpy(s1Alias,first->data->alias);
	} else {
		strcpy(s1Alias,first->data->name);
	}
	char s2Alias[300];
	if(second->data->alias!=NULL&&strlen(second->data->alias)>2) {
		strcpy(s2Alias,second->data->alias);
	} else {
		strcpy(s2Alias,second->data->name);
	}

	char * noPathS1Alias=getNameWithoutPath(s1Alias);
	char * noPathS2Alias=getNameWithoutPath(s2Alias);

	for(int i=0;i<strlen(noPathS1Alias);i++) {
		noPathS1Alias[i]=tolower(noPathS1Alias[i]);
	}

	for(int i=0;i<strlen(noPathS2Alias);i++) {
		noPathS2Alias[i]=tolower(noPathS2Alias[i]);
	}

	if (strcmp(noPathS1Alias, noPathS2Alias)<=0)
	{
		free(noPathS1Alias);
		free(noPathS2Alias);
		first->next = merge(first->next,second);
		first->next->prev = first;
		first->prev = NULL;
		return first;
	}
	else
	{
		free(noPathS1Alias);
		free(noPathS2Alias);
		second->next = merge(first,second->next);
		second->next->prev = second;
		second->prev = NULL;
		return second;
	}
}

struct Node *mergeSort(struct Node *head) {
	if (!head || !head->next) {
		return head;
	}
	struct Node *second = split(head);

	// Recur for left and right halves
	head = mergeSort(head);
	second = mergeSort(second);

	// Merge the two sorted halves
	return merge(head,second);
}

void loadFavoritesSectionGameList() {
	int game = 0;
	int page = 0;
	FAVORITES_SECTION.totalPages=0;
	cleanListForSection(&FAVORITES_SECTION);
	for (int i=0;i<favoritesSize;i++){
		if (game==ITEMS_PER_PAGE) {
			if(i!=favoritesSize) {
				page++;
				game = 0;
				FAVORITES_SECTION.totalPages++;
			}
		}

		int size = strlen(favorites[i].name)+1;
		int aliasSize = strlen(favorites[i].alias)+1;

		struct Rom *rom = malloc(sizeof(struct Rom));
		rom->name=malloc(size);
		rom->alias=malloc(aliasSize);
		rom->directory=malloc(1);
		strcpy(rom->alias, favorites[i].alias);
		strcpy(rom->name, favorites[i].name);
		InsertAtTailInSection(&FAVORITES_SECTION, rom);
		game++;
		FAVORITES_SECTION.gameCount++;
	}
	FAVORITES_SECTION.tail=GetNthNode(FAVORITES_SECTION.gameCount);

	int i=0;
	FAVORITES_SECTION.currentGameInPage=0;
	FAVORITES_SECTION.currentPage=0;
	FAVORITES_SECTION.currentGameNode=FAVORITES_SECTION.head;
	while (i<FAVORITES_SECTION.realCurrentGameNumber) {
		FAVORITES_SECTION.currentGameInPage++;
		if (FAVORITES_SECTION.currentGameInPage%ITEMS_PER_PAGE==0) {
			FAVORITES_SECTION.currentGameInPage=0;
			FAVORITES_SECTION.currentPage++;
		}
		FAVORITES_SECTION.currentGameNode=FAVORITES_SECTION.currentGameNode->next;
		i++;
	}
//	menuSections[favoritesSectionNumber].head = mergeSort(menuSections[favoritesSectionNumber].head);
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
	return strcmp(temp1, temp2);
}

void fillUpStolenGMenuFile(struct StolenGMenuFile* stolenFile, char* fileName) {
	FILE * fp;
	char * line = NULL;
	size_t len = 0;
	ssize_t read;
	fp = fopen(fileName, "r");
	while ((read = getline(&line, &len, fp)) != -1) {
		char *ptr;
		if(strstr(line,"title")!=NULL) {
			ptr = strtok(line, "=");
			ptr = strtok(NULL, "=");
			strcpy(stolenFile->title,ptr);
			stolenFile->title[strlen(stolenFile->title)-1]='\0';
		} else if(strstr(line,"exec")!=NULL) {
			ptr = strtok(line, "=");
			ptr = strtok(NULL, "=");
			strcpy(stolenFile->exec,ptr);
			stolenFile->exec[strlen(stolenFile->exec)-1]='\0';
			break;
		}
	}
	fclose(fp);
	if(line) {
		free(line);
	}
}

int theCurrentSectionHasGames() {
	int dirCounter;
	char *dirs[10];
	char* ptr;
	char dirsCopy[1000];
	strcpy(dirsCopy,CURRENT_SECTION.filesDirectories);
	ptr = strtok(dirsCopy, ",");
	char *files[10000];
	int value = 0;
	while (ptr!=NULL) {
		dirs[dirCounter]=malloc(strlen(ptr)+1);
		strcpy(dirs[dirCounter],ptr);
		ptr = strtok(NULL, ",");
		dirCounter++;
	}
	for(int k=0;k<dirCounter;k++) {
		int n = recursivelyScanDirectory(dirs[k], files, 0);
		for (int i=0;i<n;i++){
			char *ext = getExtension(files[i]);
			if (ext&&strcmp((files[i]),"..")!=0 &&
					strcmp((files[i]),".")!=0 &&
					strcmp(ext,".png")!=0&&
					isExtensionValid(ext,CURRENT_SECTION.fileExtensions)) {
				value = 1;
				break;
			}
		}
		for (int i=0;i<n;i++){
			free(files[i]);
		}
		if (value==1) {
			break;
		}
	}
	for (int i=0;i<dirCounter;i++){
		free (dirs[i]);
	}
	return value;
}

void loadGameList(int refresh) {
	int loadedFiles=0;
	if (CURRENT_SECTION.initialized==0||refresh) {
		CURRENT_SECTION.initialized=1;
		//We don't need to reload the alias file if just refreshing
		if (!refresh) {
			loadAliasList(currentSectionNumber);
		}
		cleanListForSection(&CURRENT_SECTION);
		CURRENT_SECTION.totalPages=0;
		char *files[10000];
		int game = 0;
		int dirCounter;
		char *dirs[10];
		char* ptr;
		char dirsCopy[1000];
		strcpy(dirsCopy,CURRENT_SECTION.filesDirectories);
		ptr = strtok(dirsCopy, ",");
		loading=1;
		while (ptr!=NULL) {
			dirs[dirCounter]=malloc(strlen(ptr)+1);
			strcpy(dirs[dirCounter],ptr);
			ptr = strtok(NULL, ",");
			dirCounter++;
		}
		for(int k=0;k<dirCounter;k++) {
			int n = recursivelyScanDirectory(dirs[k], files, 0);
			int realItemCount = n;
			for (int i=0;i<n;i++){
				char *ext = getExtension(files[i]);
				if (ext&&strcmp((files[i]),"..")!=0 &&
						strcmp((files[i]),".")!=0 &&
						strcmp(ext,".png")!=0&&
						isExtensionValid(ext,CURRENT_SECTION.fileExtensions)){
					//it's an opk
					if(strcmp(ext,".opk")==0) {
						struct OPKDesktopFile desktopFiles[10];
						int desktopFilesCount=getOPK(files[i], desktopFiles);
						int desktopCounter=0;
						while(desktopCounter<desktopFilesCount) {
							if(strstr(desktopFiles[desktopCounter].category,CURRENT_SECTION.category)==NULL&&strcmp(CURRENT_SECTION.category,"all")!=0) {
								break;
							} else {
								#ifdef TARGET_RG300
								while(strstr(desktopFiles[desktopCounter].name,"gcw0")!=NULL) {
									desktopCounter++;
								}
								#endif
								realItemCount++;
								int size = strlen(desktopFiles[desktopCounter].parentOPK)+strlen("-m|")+strlen(desktopFiles[desktopCounter].name)+2;// " -m "
								int aliasSize = strlen(desktopFiles[desktopCounter].displayName)+1;

								struct Rom *rom;
								rom=malloc(sizeof(struct Rom));
								rom->name=malloc(size);
								rom->alias=malloc(aliasSize);
								int directorySize = strlen(dirs[k])+1;
								rom->directory=malloc(directorySize);
								strcpy(rom->directory,dirs[k]);

								if (game==ITEMS_PER_PAGE) {
									if(desktopCounter!=realItemCount) {
										CURRENT_SECTION.totalPages++;
										game = 0;
									}
								}

								strcpy(rom->name,"-m|");
								strcat(rom->name,desktopFiles[desktopCounter].name);
								strcat(rom->name,"|");
								strcat(rom->name,desktopFiles[desktopCounter].parentOPK);
								strcpy(rom->alias,desktopFiles[desktopCounter].displayName);
								InsertAtTail(rom);
								game++;
								CURRENT_SECTION.gameCount++;
							}
							desktopCounter++;
						}
						loadedFiles++;
					}
					//it's not an opk
					else {
						int size = strlen(files[i])+1;

						struct Rom *rom;
						rom=malloc(sizeof(struct Rom));
						rom->name=malloc(size);

						int directorySize = strlen(dirs[k])+1;
						rom->directory=malloc(directorySize);
						strcpy(rom->directory,dirs[k]);

						//it's a custom link
						if(strcmp(ext,".fgl")==0) {
							struct StolenGMenuFile stolenFile;
							fillUpStolenGMenuFile(&stolenFile, files[i]);
							strcpy(rom->name,stolenFile.exec);
							rom->alias=malloc(strlen(stolenFile.title)+1);
							strcpy(rom->alias, stolenFile.title);
						} else {
							//it's a rom
							strcpy(rom->name,files[i]);
							if(strlen(CURRENT_SECTION.aliasFileName)>2) {
								char *temp=getAlias(rom->name);
								rom->alias=malloc(strlen(temp)+1);
								strcpy(rom->alias, temp);
								free(temp);
							} else {
								rom->alias=NULL;
							}
						}
						CURRENT_SECTION.gameCount++;
						if (game==ITEMS_PER_PAGE) {
							if(i!=realItemCount) {
								CURRENT_SECTION.totalPages++;
								game = 0;
							}
						}
						InsertAtTail(rom);
						loadedFiles++;
						game++;
					}
				}
			}
			for (int i=0;i<n;i++){
				free(files[i]);
			}
			if (loadedFiles==0) {
				CURRENT_SECTION.hidden=1;
				loading=0;
				return;
			}
		}
		for (int i=0;i<dirCounter;i++){
			free (dirs[i]);
		}
		CURRENT_SECTION.head = mergeSort(CURRENT_SECTION.head);
		CURRENT_SECTION.tail=GetNthNode(CURRENT_SECTION.gameCount-1);
		printf("%d\n",CURRENT_SECTION.realCurrentGameNumber);
		printf("%d\n",CURRENT_SECTION.currentGameInPage);
		printf("%d\n",CURRENT_SECTION.currentPage);
		CURRENT_SECTION.currentGameNode=GetNthNode(CURRENT_GAME_NUMBER);
	}
	loading=0;
}

int countGamesInPage() {
	int gamesCounter=0;
	struct Node *node;
	int number = CURRENT_GAME_NUMBER;
	while(number%ITEMS_PER_PAGE!=0) {
		number--;
	}
	node = CURRENT_SECTION.currentGameNode;
	for (int i=number;i<number+ITEMS_PER_PAGE;i++) {
		if (node!=NULL) {
			if (node->data->name!=NULL && strlen(node->data->name)>1) {
				gamesCounter++;
			}
			if (node->next==NULL) {
				break;
			}
			node=node->next;
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
	if (gamesInSection%ITEMS_PER_PAGE==0) {
		CURRENT_SECTION.currentPage = rand() % ((int)gamesInSection/ITEMS_PER_PAGE);
	} else {
		CURRENT_SECTION.currentPage = rand() % ((int)(gamesInSection/ITEMS_PER_PAGE) +1);
	}
	CURRENT_SECTION.currentGameInPage = rand() % countGamesInPage();
}

void determineStartingScreen(int sectionCount) {
	if (sectionCount==-1) {
		generateError("SECTIONS FILE NOT FOUND-SHUTTING DOWN",1);
		return;
	}
	if(sectionCount==0||currentSectionNumber==favoritesSectionNumber) {
		favoritesSectionSelected=1;
		loadFavoritesSectionGameList();
		int gamesInSection=CURRENT_SECTION.gameCount;
		int pages = gamesInSection / ITEMS_PER_PAGE;
		if (gamesInSection%ITEMS_PER_PAGE==0) {
			pages--;
		}
		CURRENT_SECTION.totalPages=pages;
		if (favoritesSize==0) {
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
		int gamesInSection=CURRENT_SECTION.gameCount;
		int pages = gamesInSection / ITEMS_PER_PAGE;
		if (gamesInSection%ITEMS_PER_PAGE==0) {
			pages--;
		}
		CURRENT_SECTION.totalPages=pages;
	}
}

void deleteGame(struct Rom *rom) {
	char command [300];
	snprintf(command,sizeof(command),"rm \"%s\";",rom->name);
	int ret = system(command);
	if (ret==-1) {
		generateError("FATAL ERROR", 1);
	}
	char *pictureWithFullPath=malloc(600);
	char *tempGameName;
	char *tempGameName1;
	strcpy(pictureWithFullPath, rom->directory);
	tempGameName=getGameName(rom->name);
	strcat(pictureWithFullPath,"media/");
	tempGameName1=getNameWithoutExtension(tempGameName);
	strcat(pictureWithFullPath,tempGameName);
	strcat(pictureWithFullPath,".png");
	snprintf(command,sizeof(command),"rm \"%s\";",pictureWithFullPath);
	free(tempGameName);
	free(tempGameName1);
	free(pictureWithFullPath);
	ret = system(command);
	if (ret==-1) {
		generateError("FATAL ERROR", 1);
	}
}
