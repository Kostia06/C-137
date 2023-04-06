#ifndef FILE_H
#define FILE_H

#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>

#include "../utils/include.h"

char* read_file(char* file_name,size_t* return_size);
char** get_error_lines(char* file_name,int index,int size,int* return_size,int* return_index);

#endif
