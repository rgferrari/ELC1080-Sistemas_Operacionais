#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include<sys/wait.h> 
#include <sys/types.h>
#include <dirent.h>

#define TIP_SIZE 1024
#define PATH_SIZE 1024

FILE *getFile(char *filePath, char *mode);
int getNumberOfTips(FILE *file);
char **allocateMatrix(int numberOfTips);
void storeTipsInMatrix(char **tips, FILE *file);
char *allocatePath();
void createTip(char **tips, int numberOfTips, char *path);
void printPath(char *path);
void travelThroughFolders(char **tips, int numberOfTips, char *path);