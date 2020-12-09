
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
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <unistd.h>
#define MAX_PATH 256

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
    char path[MAX_PATH];
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
    fill buffer with value of a symlink file
*/
void getSymlinkVal(char* buf, const char* parent_path, const char* tail_path);

/*
    return 'd' if directory, 'l' if link, otherwise '-'
*/
char getTypeChar(const struct stat* statbuf);

/* 
    print a file entry according to parameters
    - flags arg specifies format
    - path is printed unmodified as file name
*/
void printEntry(const flags* f_args, const char* pre_path, const char* path, const struct stat* statbuf);

/*
    iterate through contents of dir
     - print each entry 
     - if -R flag set, add each directory encountered to directory queue
*/
void processDir(const flags* f_args, const char* path);


#endif