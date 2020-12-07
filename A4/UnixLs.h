
#ifndef UNIXLS_H
#define UNIXLS_H

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <sys/queue.h>
#include <sys/stat.h>
#include <stdint.h>
#include <dirent.h>
#include <errno.h>
/*

get args, set flags
-i: add inode to parser and format string
-l: add more info to parser and format string
-R: call directory parser recursively

iterate over remaining args with parser: 
 * if file, call file parser, add to file queue
 * if dir, create dir struct, parse each file in it, add dir to dir queue
    * if recursive, recursively call for each dir in dir

print each entry in file queue, then each dir in dir queue

file parser:
 * parse file data, return it
*/

/* 
    specify which flags among i, l, and R are set
*/
typedef struct flags {
    bool i;
    bool l;
    bool R;
} flags;

/*
    structure for directory queue. directory processing is deferred until 
    after loose files, and parent directories in the case of recursive ls
*/
typedef struct dirElem {
    char path[256];
    STAILQ_ENTRY(dirElem) dirElems;
} dirElem;

STAILQ_HEAD(stailhead, dirElem);
struct stailhead dir_head;
/* 
    sets appropriate flags given argument string
    - assumes arg is a valid flag string
*/
void setFlags(flags* f_args, char* const arg);

/* 
    print a file entry according to parameters
    - flags arg specifies format
    - path is printed unmodified as file name
*/
void printEntry(const flags* f_args, char* path, const struct stat* statbuf);

void processDir(const flags* f_args, char* path);


#endif