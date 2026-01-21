#ifndef DIRECTORY_H
#define DIRECTORY_H
#include "dirent.h"
#include <stdio.h>


DIR* open_dir(char *path);


void print_dir(DIR *dir);


#endif // DIRECTORY_H
