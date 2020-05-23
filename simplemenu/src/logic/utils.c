#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

FILE * fp;

void openLogFile() {
	   char pathToLogFilePlusFileName[300];
	   snprintf(pathToLogFilePlusFileName,sizeof(pathToLogFilePlusFileName),"%s/.simplemenu/log.txt",getenv("HOME"));
	   fp = fopen(pathToLogFilePlusFileName, "w");

}

void logMessage(const char* tag, const char* message) {
   time_t now;
   time(&now);
   char *time = ctime(&now);
   time[strlen(time)-1]='\0';
   fprintf(fp,"%s | %-5s | %s\n", time, tag, message);
}

void closeLogFile() {
	if (fp!=NULL) {
		fclose(fp);
	}
}
