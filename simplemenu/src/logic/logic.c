#include <ctype.h>
#include <dirent.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <time.h>

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
	if(CURRENT_GAME->alias!=NULL&&strlen(CURRENT_GAME->alias)>2) {
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

char *getCurrentGameName() {
	char * name = malloc(strlen(CURRENT_GAME_NAME)+1);
	strcpy(name, CURRENT_GAME_NAME);
	stripGameName(name);
	return name;
}

void quit() {
	drawShutDownScreen();
	refreshScreen();
	saveLastState();
	saveFavorites();
	clearTimer();
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
	//	if (checkIfEmulatorExists(emulatorFolder,executable)||strcmp(emulatorFolder,"/usr/bin/")==0) {
	#ifndef TARGET_PC
	saveLastState();
	saveFavorites();
	clearTimer();
	freeResources();
//	printf("%s\n",fileToBeExecutedWithFullPath);
//	sleep(1);
	execlp("./invoker.dge","invoker.dge", emulatorFolder, executable, fileToBeExecutedWithFullPath, states, pSectionNumber, pReturnTo, pPictureMode, NULL);
	#else
	printf("%s\n",fileToBeExecutedWithFullPath);
	#endif
	//	} else {
	//		generateError("CONFIGURED EMULATOR NOT FOUND- CHECK SECTIONS.CFG ",0);
	//	}
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
	for (int i=0;i<=CURRENT_SECTION.totalPages;i++) {
		for (int j=0;j<ITEMS_PER_PAGE;j++) {
			if (CURRENT_SECTION.romList[i][j]!=NULL&&strlen(CURRENT_SECTION.romList[i][j]->name)>0) {
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
			FAVORITES_SECTION.romList[i][j]=NULL;
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
		if (FAVORITES_SECTION.romList[page][game]==NULL) {
			FAVORITES_SECTION.romList[page][game]= malloc(sizeof(struct Rom));
			FAVORITES_SECTION.romList[page][game]->name=malloc(300);
			FAVORITES_SECTION.romList[page][game]->alias=malloc(300);
		}
		strcpy(FAVORITES_SECTION.romList[page][game]->alias, favorites[i].alias);
		strcpy(FAVORITES_SECTION.romList[page][game]->name, favorites[i].name);
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
	char temp1[300];
	strcpy(temp1,game1);
	char temp2[300];
	strcpy(temp2,game2);
	for(int i=0;temp1[i]; i++) {
		temp1[i] = tolower(temp1[i]);
	}
	for(int i=0;temp2[i]; i++) {
		temp2[i] = tolower(temp2[i]);
	}
	return(strcmp(temp1, temp2));
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

int compareGamesFromGameList (const void *game1, const void *game2) {
	char* s1 = (*(struct Rom**)game1)->name;
	char* s2 = (*(struct Rom**)game2)->name;
	return genericGameCompare(s1, s2);
}

int compareGamesFromGameListBasedOnAlias (const void *game1, const void *game2) {
	char s1Alias[300];
	if((char *)(*(struct Rom **)game1)->alias!=NULL&&strlen((char *)(*(struct Rom **)game1)->alias)>2) {
		strcpy(s1Alias,(char *)(*(struct Rom **)game1)->alias);
	} else {
		strcpy(s1Alias,(char *)(*(struct Rom **)game1)->name);
	}
	char s2Alias[300];
	if((char *)(*(struct Rom **)game2)->alias!=NULL&&strlen((char *)(*(struct Rom **)game2)->alias)>2) {
		strcpy(s2Alias,(char *)(*(struct Rom **)game2)->alias);
	} else {
		strcpy(s2Alias,(char *)(*(struct Rom **)game2)->name);
	}
	return genericGameCompareWithAlias(getNameWithoutPath(s1Alias), getNameWithoutPath(s2Alias));
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

void loadGameList(int refresh) {
	int loadedFiles=0;
	if (CURRENT_SECTION.initialized==0||refresh) {
		CURRENT_SECTION.initialized=1;
		if (!refresh) {
			loadAliasList(currentSectionNumber);
			for (int i=0;i<1000;i++) {
				for (int j=0;j<10;j++) {
					CURRENT_SECTION.romList[i][j]=NULL;
				}
			}
		} else {
			for (int i=0;i<1000;i++) {
				for (int j=0;j<10;j++) {
					if (CURRENT_SECTION.romList[i][j]!=NULL) {
						free(CURRENT_SECTION.romList[i][j]->name);
						free(CURRENT_SECTION.romList[i][j]->alias);
						free(CURRENT_SECTION.romList[i][j]->directory);
						free(CURRENT_SECTION.romList[i][j]);
						CURRENT_SECTION.romList[i][j]=NULL;
					}
				}
			}
		}
		CURRENT_SECTION.totalPages=0;
		char *files[10000];
		int game = 0;
		int page = 0;
		int dirCounter;
		char *dirs[10];
		char* ptr;
		char dirsCopy[1000];
		strcpy(dirsCopy,CURRENT_SECTION.filesDirectories);
		ptr = strtok(dirsCopy, ",");
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
								realItemCount++;
								int size = strlen(desktopFiles[desktopCounter].parentOPK)+strlen("-m|")+strlen(desktopFiles[desktopCounter].name)+2;// " -m "
								int aliasSize = strlen(desktopFiles[desktopCounter].displayName)+1;
								CURRENT_SECTION.romList[page][game]=malloc(sizeof(struct Rom));
								CURRENT_SECTION.romList[page][game]->name=malloc(size);
								CURRENT_SECTION.romList[page][game]->alias=malloc(aliasSize);
								int directorySize = strlen(dirs[k])+1;
								CURRENT_SECTION.romList[page][game]->directory=malloc(directorySize);
								strcpy(CURRENT_SECTION.romList[page][game]->directory,dirs[k]);
								if (game==ITEMS_PER_PAGE) {
									if(desktopCounter!=realItemCount) {
										page++;
										CURRENT_SECTION.totalPages++;
										game = 0;
									}
								}
//								if (strcmp("default.gcw0.desktop",desktopFiles[desktopCounter].name)!=0) {
//									strcpy(CURRENT_SECTION.romList[page][game]->name,"-m|");
//									strcat(CURRENT_SECTION.romList[page][game]->name,desktopFiles[desktopCounter].name);
//									strcat(CURRENT_SECTION.romList[page][game]->name,"|");
//									strcat(CURRENT_SECTION.romList[page][game]->name,desktopFiles[desktopCounter].parentOPK);
//									//								strcat(CURRENT_SECTION.romList[page][game]->name,"\0");
//								} else {
									strcpy(CURRENT_SECTION.romList[page][game]->name,"-m|");
									strcat(CURRENT_SECTION.romList[page][game]->name,desktopFiles[desktopCounter].name);
//									printf("%s\n",desktopFiles[desktopCounter].name);
									strcat(CURRENT_SECTION.romList[page][game]->name,"|");
									strcat(CURRENT_SECTION.romList[page][game]->name,desktopFiles[desktopCounter].parentOPK);
//									strcpy(CURRENT_SECTION.romList[page][game]->name,desktopFiles[desktopCounter].parentOPK);
									//								strcat(CURRENT_SECTION.romList[page][game]->name,"\0");
//								}
								strcpy(CURRENT_SECTION.romList[page][game]->alias,desktopFiles[desktopCounter].displayName);
								//							strcat(CURRENT_SECTION.romList[page][game]->alias,"\0");
								game++;
							}
							desktopCounter++;
						}
						loadedFiles++;
					}
					//it's not an opk
					else {
						int size = strlen(files[i])+1;
						CURRENT_SECTION.romList[page][game]=malloc(sizeof(struct Rom));

						CURRENT_SECTION.romList[page][game]->name=malloc(size);

						int directorySize = strlen(dirs[k])+1;
						CURRENT_SECTION.romList[page][game]->directory=malloc(directorySize);
						strcpy(CURRENT_SECTION.romList[page][game]->directory,dirs[k]);

						//it's a custom link
						if(strcmp(ext,".fgl")==0) {
							struct StolenGMenuFile stolenFile;
							fillUpStolenGMenuFile(&stolenFile, files[i]);
							strcpy(CURRENT_SECTION.romList[page][game]->name,stolenFile.exec);
							//						strcat(CURRENT_SECTION.romList[page][game]->name,"\0");
							CURRENT_SECTION.romList[page][game]->alias=malloc(strlen(stolenFile.title)+1);
							strcpy(CURRENT_SECTION.romList[page][game]->alias, stolenFile.title);
							//						strcat(CURRENT_SECTION.romList[page][game]->alias,"\0");
						} else {
							//it's a rom
							strcpy(CURRENT_SECTION.romList[page][game]->name,files[i]);
							//						strcat(CURRENT_SECTION.romList[page][game]->name,"\0");
							if(strlen(CURRENT_SECTION.aliasFileName)>2) {
								char *temp=getAlias(CURRENT_SECTION.romList[page][game]->name);
								//							char *temp="PEPE";
								CURRENT_SECTION.romList[page][game]->alias=malloc(strlen(temp)+1);
								strcpy(CURRENT_SECTION.romList[page][game]->alias, temp);
								//							strcat(CURRENT_SECTION.romList[page][game]->alias,"\0");
								free(temp);
							} else {
								CURRENT_SECTION.romList[page][game]->alias=NULL;
							}
						}
						if (game==ITEMS_PER_PAGE) {
							if(i!=realItemCount) {
								page++;
								CURRENT_SECTION.totalPages++;
								game = 0;
							}
						}
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
				return;
			}
		}
		for (int i=0;i<dirCounter;i++){
			free (dirs[i]);
		}
		qsort(menuSections[currentSectionNumber].romList, countGamesInSection(), sizeof(char *), compareGamesFromGameListBasedOnAlias);
	}
}

int countGamesInPage() {
	int gamesCounter=0;
	for (int i=0;i<ITEMS_PER_PAGE;i++) {
		if (CURRENT_SECTION.romList[menuSections[currentSectionNumber].currentPage][i]!=NULL
				&&CURRENT_SECTION.romList[menuSections[currentSectionNumber].currentPage][i]->name!=NULL
				&&strlen(CURRENT_SECTION.romList[menuSections[currentSectionNumber].currentPage][i]->name)>1) {
			gamesCounter++;
		}
	}
	return gamesCounter;
}

int countGamesInSpecificPage(int page) {
	int gamesCounter=0;
	for (int i=0;i<ITEMS_PER_PAGE;i++) {
		if (CURRENT_SECTION.romList[page][i]->name!=NULL) {
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
	int ret = system(command);
	if (ret==-1) {
		generateError("FATAL ERROR", 1);
	}
	char *pictureWithFullPath=malloc(600);
	char *tempGameName;
	char *tempGameName1;
	strcpy(pictureWithFullPath, CURRENT_GAME->directory);
	tempGameName=getGameName(CURRENT_GAME_NAME);
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
