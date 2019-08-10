#include <constants.h>
#include <dirent.h>
#include <fcntl.h>
#include <globals.h>
#include <definitions.h>
#include <screen.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string_utils.h>
#include <SDL/SDL.h>
#include <sys/mman.h>
#include <unistd.h>

volatile uint32_t *memregs;

uint32_t oc_table[] = {
		0x00c81802,
		0x00cc1013,
		0x00cc1001,
		0x00d01902,
		0x00d00c12,
		0x00d80b23,
		0x00d81101,
		0x00d80833,
		0x00d81a02,
		0x00d80811,
		0x00d81113,
		0x00d80220,
		0x00d80521,
		0x00d80822,
		0x00d80800,
		0x00e01b02,
		0x00e00d12,
		0x00e00632,
		0x00e41201,
		0x00e41213,
		0x00e81c02,
		0x00ea0c23,
		0x00f00922,
		0x00f00900,
		0x00f01301,
		0x00f00933,
		0x00f00911,
		0x00f01313,
		0x00f01d02,
		0x00f00431,
		0x00f00e12,
		0x00f00410,
		0x00f81e02,
		0x00fc0d23,
		0x00fc0621,
		0x00fc1413,
		0x00fc1401,
		0x01000732,
		0x01001f02,
		0x01000f12,
		0x01081501,
		0x01080a11,
		0x01080a22,
		0x01081513,
		0x01080a00,
		0x01080a33,
		0x010e0e23,
		0x01101012,
		0x01141601,
		0x01141613,
		0x01200531,
		0x01200f23,
		0x01200832,
		0x01200b11,
		0x01200230,
		0x01200721,
		0x01200b22,
		0x01200320,
		0x01201701,
		0x01200b33,
		0x01200b00,
		0x01201713,
		0x01201112,
		0x01200510,
		0x012c1801,
		0x012c1813,
		0x01301212,
		0x01321023,
		0x01380c11,
		0x01380c22,
		0x01381901,
		0x01380c00,
		0x01380c33,
		0x01381913,
		0x01401312,
		0x01400932,
		0x01441123,
		0x01441a13,
		0x01441a01,
		0x01440821,
		0x01501b13,
		0x01501412,
		0x01500631,
		0x01500610,
		0x01500d22,
		0x01500d00,
		0x01501b01,
		0x01500d33,
		0x01500d11,
		0x01561223,
		0x015c1c01,
		0x015c1c13,
		0x01601512,
		0x01600a32,
		0x01680921,
		0x01681d01,
		0x01680420,
		0x01680e22,
		0x01681d13,
		0x01680e00,
		0x01681323,
		0x01680e33,
		0x01680e11,
		0x01701612,
		0x01741e01,
		0x01741e13,
		0x017a1423,
		0x01800f33,
		0x01800710,
		0x01800731,
		0x01800f11,
		0x01801712,
		0x01800330,
		0x01800f00,
		0x01801f01,
		0x01800f22,
		0x01800b32,
		0x01801f13,
		0x018c0a21,
		0x018c1523,
		0x01901812,
		0x01981022,
		0x01981000,
		0x01981033,
		0x01981011,
		0x019e1623,
		0x01a01912,
		0x01a00c32,
		0x01b00520,
		0x01b00810,
		0x01b01111,
		0x01b01723,
		0x01b00831,
		0x01b01100,
		0x01b01122,
		0x01b00b21,
		0x01b01a12,
		0x01b01133,
		0x01c01b12,
		0x01c00d32,
		0x01c21823,
		0x01c81233,
		0x01c81211,
		0x01c81222,
		0x01c81200,
		0x01d01c12,
		0x01d41923,
		0x01d40c21,
		0x01e00931,
		0x01e01333,
		0x01e00430,
		0x01e00e32,
		0x01e01311,
		0x01e00910,
		0x01e01300,
		0x01e01322,
		0x01e01d12,
		0x01e61a23,
		0x01f01e12,
		0x01f81400,
		0x01f81b23,
		0x01f81433,
		0x01f81411,
		0x01f80d21,
		0x01f80620,
		0x01f81422,
		0x02001f12,
		0x02000f32,
		0x020a1c23,
		0x02101522,
		0x02101500,
		0x02101533,
		0x02101511,
		0x02100a10,
		0x02100a31,
		0x021c0e21,
		0x021c1d23,
		0x02201032,
		0x02281611,
		0x02281622,
		0x02281600,
		0x02281633,
		0x022e1e23,
		0x02401722,
		0x02400b10,
		0x02401733,
		0x02401700,
		0x02400b31,
		0x02400530,
		0x02401132,
		0x02401711,
		0x02400f21,
		0x02400720,
		0x02401f23,
		0x02581800,
		0x02581833,
		0x02581811,
		0x02581822,
		0x02601232,
		0x02641021,
		0x02701911,
		0x02700c10,
		0x02700c31,
		0x02701922,
		0x02701900,
		0x02701933,
		0x02801332,
		0x02881a11,
		0x02880820,
		0x02881121,
		0x02881a22,
		0x02881a00,
		0x02881a33,
		0x02a00d31,
		0x02a01432,
		0x02a01b11,
		0x02a00630,
		0x02a01b00,
		0x02a01b22,
		0x02a00d10,
		0x02a01b33,
		0x02ac1221,
		0x02b81c00,
		0x02b81c33,
		0x02b81c11,
		0x02b81c22,
		0x02c01532,
		0x02d01d00,
		0x02d01d22,
		0x02d00920,
		0x02d01d33,
		0x02d00e10,
		0x02d00e31,
		0x02d01d11,
		0x02d01321,
		0x02e01632,
		0x02e81e00,
		0x02e81e33,
		0x02e81e11,
		0x02e81e22,
		0x02f41421,
		0x03000730,
		0x03000f10,
		0x03001f11,
		0x03000f31,
		0x03001f00,
		0x03001f22,
		0x03001732,
		0x03001f33,
		0x03180a20,
		0x03181521,
		0x03201832,
		0x03301031,
		0x03301010,
		0x033c1621,
		0x03401932,
		0x03600830,
		0x03601721,
		0x03601a32,
		0x03600b20,
		0x03601110,
		0x03601131,
		0x03801b32,
		0x03841821,
};

int32_t memdev = 0;

void setCPU(uint32_t mhz) {
	currentCPU=mhz;
//	uint32_t x, v;
//	uint32_t total=sizeof(oc_table)/sizeof(uint32_t);
//	for(x=0; x<total; x++) {
//		if((oc_table[x] >> 16) >= mhz) {
//			v = memregs[0];
//			v&= 0xffff0000;
//			v|= (oc_table[x] &  0x0000ffff);
//			memregs[0] = v;
//			break;
//		}
//	}
}

void HW_Init() {
	memdev = open("/dev/mem", O_RDWR);
	if (memdev > 0)	{
		memregs = (uint32_t*)mmap(0, 4096, PROT_READ | PROT_WRITE, MAP_SHARED, memdev, 0x01c20000);
		if (memregs == MAP_FAILED) {
			printf("Could not mmap hardware registers!\n");
			close(memdev);
		}
	}
	setCPU(MED_OC);
}

void readConfig() {
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


void quit() {
	freeResources();
	execlp("sh", "sh", "-c", "sync && poweroff", NULL);
}

int determineExecutable (char *fileToBeExecutedWithFullPath) {
	char *ext = getExtension(fileToBeExecutedWithFullPath);
	if (strcmp(ext,".gb")==0) {
		return 1;
	}
	if (strcmp(ext,".gbc")==0) {
		return 2;
	}
	if (strcmp(ext,".gba")==0) {
		return 3;
	}
	if (strcmp(ext,".nes")==0) {
		return 4;
	}
	if (strcmp(ext,".sfc")==0) {
		return 5;
	}
	if (strcmp(ext,".gg")==0) {
		return 6;
	}
	if (strcmp(ext,".sms")==0) {
		return 7;
	}
	if (strcmp(ext,".md")==0) {
		return 8;
	}
	if (strcmp(ext,".iso")==0) {
		return 9;
	}
	if (strcmp(ext,".zip")==0) {
		return 10;
	}
	if (strcmp(ext,".pce")==0) {
		return 11;
	}
	if (strcmp(ext,".bin")==0) {
		return 12;
	}
	return -1;
}

void executeFavorite (char fileToBeExecutedWithFullPath[]) {
	freeResources();
	char command[200];
	int section = determineExecutable(fileToBeExecutedWithFullPath);
	strcpy(command, "cd ");
	strcat(command, menuSections[section].emulatorFolder);
	strcat(command, ";./");
	strcat(command, menuSections[section].executable);
	if(section!=-1) {
		strcat(command," ");
	}
	strcat(command,"\"");
	strcat(command,fileToBeExecutedWithFullPath);
	strcat(command,"\"");
	int returnValue = system(strcat(command,"&>/dev/null"));
	if (returnValue==-1) {
		printf("ERROR");
	}
	setupDisplay();
	setupDecorations();
}

void executeCommand (char executable[], char fileToBeExecutedWithFullPath[]) {
	freeResources();
	char command[200];
	strcpy(command, "cd ");
	strcat(command, menuSections[currentSection].emulatorFolder);
	strcat(command, ";./");
	strcat(command, executable);
	strcat(command," ");
	strcat(command,"\"");
	strcat(command,fileToBeExecutedWithFullPath);
	strcat(command,"\"");
	int returnValue = system(strcat(command,"&>/dev/null"));
	if (returnValue==-1) {
		printf("ERROR");
	}
	setupDisplay();
	setupDecorations();
}

int isExtensionValid(char *extension, struct MenuSection section) {
	if(currentSection>0) {
		return(strcmp(extension,section.fileExtension));
	}
	return 0;
}

void loadGameList() {
	struct dirent **files;
	int n=scandir(menuSections[currentSection].filesDirectory, &files, 0, alphasort);
	int game = 0;
	int page = 0;
	for (int i=0;i<200;i++) {
		for (int j=0;j<10;j++) {
			gameList[i][j]=NULL;
		}
	}
	for (int i=0;i<n;i++){
		if (strcmp((files[i]->d_name),".gitignore")!=0 &&
				strcmp((files[i]->d_name),"..")!=0 &&
				strcmp((files[i]->d_name),".")!=0 &&
				isExtensionValid(getExtension((files[i]->d_name)),menuSections[currentSection])==0){
			gameList[page][game] = files[i]->d_name;
			game++;
			if (game==ITEMS_PER_PAGE) {
				if(i!=n-1) {
					page++;
					totalPages++;
					game = 0;
				}
			}
		}
	}
	free(files);
}
