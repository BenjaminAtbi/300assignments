#include "UnixLs.h"

//############### Test funcs

void dumpstat(const struct stat* statbuf)
{
    printf("ino:  %8ju ",(uintmax_t)statbuf->st_ino);
    printf("mode: %4o ",statbuf->st_mode);
    printf("size: %8i ", (int)statbuf->st_size);
}
//#############################


void setFlags(flags* f_args, char* const arg)
{
    for(char* c = arg; *c != '\0'; c++)
    {
        switch(*c)
        {
            case 'i': f_args->i = true; break;
            case 'l': f_args->l = true; break;
            case 'R': f_args->R = true; break;
        }
    }
}

void buildPath(char* dest, const char* parent_path, const char* tail_path)
{
    strcpy(dest,parent_path);
    strcat(dest,"/");
    strcat(dest,tail_path);
}

void getSymlinkVal(char* buf, const char* parent_path, const char* tail_path)
{
    char* path_buf = malloc(MAX_PATH);
    buildPath(path_buf, parent_path, tail_path);
    int ret = readlink(path_buf ,buf, MAX_PATH);
    buf[ret] = '\0';
    free(path_buf);
}

char getTypeChar(const struct stat* statbuf)
{
    if(S_ISDIR(statbuf->st_mode))
    {
        return 'd';
    } else if (S_ISLNK(statbuf->st_mode))
    {
        return 'l';
    } else
    {
        return '-';
    }
}

void printEntry(const flags* f_args, const char* pre_path, const char* path, const struct stat* statbuf)
{
    if(f_args->i)
    {
        printf("%ju ",(uintmax_t)statbuf->st_ino);
    }
    if(f_args->l)
    {
        //permissions 
        char *perms = malloc(10);
        strcpy(perms, "rwxrwxrwx");
        mode_t temp_mode = statbuf->st_mode;
        for(int i = 8; i >= 0; i--)
        {
            if(!(1 & temp_mode))
            {
                perms[i] = '-';
            }
            temp_mode = temp_mode >> 1;
        }
        printf("%c%9s %3lu ",getTypeChar(statbuf), perms, statbuf->st_nlink);
        free(perms);

        //userid
        struct passwd *pw = NULL;
        pw = getpwuid(statbuf->st_uid);
        if(pw)
        {
            printf("%8s ",pw->pw_name);
        } else {
            printf("  ----  ");
        }

        //groupid
        struct group *grp = NULL;
        grp = getgrgid(statbuf->st_gid); 
        if(grp)
        {
            printf("%8s ", grp->gr_name);
        } else {
            printf("  ----  ");
        }
        printf("%8li ",statbuf->st_size);

        struct tm *mod_date = localtime(&statbuf->st_mtim.tv_sec);
        char date_s[14];
        int ret = strftime(date_s, 14, "%b %e %H:%M ", mod_date);
        printf("%s ",date_s);

    }
    printf("%s",path); 

    //target of symlink
    if(f_args->l && S_ISLNK(statbuf->st_mode))
    {
        char* buf = malloc(MAX_PATH);
        getSymlinkVal(buf,pre_path,path);
        printf(" -> %s",buf);
        free(buf);
    }
    printf("\n");
}

void processDir(const flags* f_args, const char* path)
{
    //open directory stream
    DIR* dir_s = opendir(path);
    if(dir_s == NULL)
    {
        int err = errno;
        fprintf(stderr, "error opening directory: %s\n",strerror(err));
        return;
    }
    
    //iterate through directory elements
    struct dirent* dir_e;
    int readdir_err;
    
    while(1)
    {
        errno = 0;
        dir_e = readdir(dir_s);
        int readdir_err = errno;
        
        if(dir_e == NULL)
        {
            //break when return null without error
            if(readdir_err == 0)
            {
                break;
            }
            else
            {
                int err = errno;
                fprintf(stderr, "error reading directory entry: %s\n",strerror(err));
            }
        }
        //file name does not start with "." (we do not support either parent/current directory listing, or hidden files)
        else if(dir_e->d_name[0] != '.')
        {
            struct stat stat_buf; 
            char path_buf[MAX_PATH];
            buildPath(path_buf, path, dir_e->d_name);
            int ret = lstat(path_buf,&stat_buf);
            if(ret != 0)
            {
                printf("cannot access \'%s\': No such file or directory (%i)\n",path_buf,ret);
            } else {
                printEntry(f_args, path, dir_e->d_name,&stat_buf);
            
                //prepend subdirecties for processing if -R flag is set
                if(f_args->R && S_ISDIR(stat_buf.st_mode))
                {
                    dirElem* elem = malloc(sizeof(dirElem));
                    strcpy(elem->path, path_buf);
                    STAILQ_INSERT_HEAD(&dir_head, elem, dirElems);
                }
            }
        }
    }
}

int main(int argc, char *argv[]){

    flags f_args = {false,false,false};
    STAILQ_INIT(&dir_head);
    //get all flags before parsing
    int i = 1;
    for(; i < argc && *argv[i] == '-'; i++)
    {
        setFlags(&f_args,argv[i]);
    }

    //process pathnames
    for(int k = i; k < argc; k++)
    {
        struct stat buf; 
        int ret = lstat(argv[k],&buf);
        
        if(ret != 0)
        {
            printf("cannot access \'%s\': No such file or directory\n",argv[k]);
        }
        //print regular files
        else if(S_ISREG(buf.st_mode))
        {
            printEntry(&f_args, "./", argv[k], &buf);
        }
        //defer directories
        else if(S_ISDIR(buf.st_mode))
        {
            dirElem* elem = malloc(sizeof(dirElem));
            strcpy(elem->path, argv[k]);
            STAILQ_INSERT_TAIL(&dir_head, elem, dirElems);
        }
        //if symlink to directory and not -l, explore directory. otherwise, print entry
        else if(S_ISLNK(buf.st_mode))
        {
            char* sym_val = malloc(MAX_PATH);
            getSymlinkVal(sym_val, "./",argv[k]);
            struct stat sym_stat; 
            int ret = lstat(sym_val,&sym_stat);
            if(!f_args.l && S_ISDIR(sym_stat.st_mode))
            {
                dirElem* elem = malloc(sizeof(dirElem));
                strcpy(elem->path, sym_val);
                STAILQ_INSERT_TAIL(&dir_head, elem, dirElems);
            } else {
                printEntry(&f_args, "./", argv[k], &buf);
            }

        }

        //file type outside of project scope
        else
        {
            printf("cannot access \'&s\': Unrecognized file type\n");
        }
    }
    //if no path arguments given, process current directory
    if(i == argc)
    {
        char path[MAX_PATH];
        strcpy(path, ".");
        processDir(&f_args, path);
    } 

    //print directories
    while(!STAILQ_EMPTY(&dir_head))
    {
        dirElem* elem = STAILQ_FIRST(&dir_head);
        STAILQ_REMOVE_HEAD(&dir_head, dirElems);
        printf("%s:\n",elem->path);
        processDir(&f_args, elem->path);
        printf("\n");
        free(elem);
    }

} 