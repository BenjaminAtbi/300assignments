#include "UnixLs.h"

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

void printEntry(const flags* f_args, char* path, const struct stat* statbuf)
{
    if(f_args->i)
    {
        printf("%ju ",(uintmax_t)statbuf->st_ino);
    }
    printf("%s\n",path);
}

void processDir(const flags* f_args, char* path)
{
    //open directory stream
    DIR* dir_s = opendir(path);
    if(dir_s == NULL)
    {
        int err = errno;
        fprintf(stderr, "error opening directory: %s\n",strerror(err));
        return;
    }
    
    //iterate through directory items
    struct dirent* dir_e;
    do 
    {
        errno = 0;
        struct dirent* dir_e = readdir(dir_s);
        if(dir_e == NULL && errno != 0)
        {
            int err = errno;
            fprintf(stderr, "error reading directory entry: %s\n",strerror(err));
        } else if(dir_e != NULL)
        {
            struct stat buf; 
            int ret = stat(dir_e->d_name,&buf);
            printEntry(f_args, dir_e->d_name,&buf);
            
            //prepend subdirecties for processing if -R flag is set
            if(f_args->R && S_ISDIR(buf.st_mode))
            {
                dirElem* elem = malloc(sizeof(dirElem));
                strncpy(elem->path, dir_e->d_name, 256);
                STAILQ_INSERT_HEAD(&dir_head, elem, dirElems);
            }

        }
        printf("shit");
    } while(dir_e != NULL || (dir_e == NULL && errno == 0));

    
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
        int ret = stat(argv[k],&buf);
        
        if(ret != 0)
        {
            printf("cannot access \'%s\': No such file or directory\n",argv[k]);
        }
        //print regular files
        else if(S_ISREG(buf.st_mode))
        {
            printEntry(&f_args, argv[k], &buf);
        }
        //defer directories
        else if(S_ISDIR(buf.st_mode))
        {
            dirElem* elem = malloc(sizeof(dirElem));
            strncpy(elem->path, argv[k], 256);
            STAILQ_INSERT_TAIL(&dir_head, elem, dirElems);
        }
        //handle simlinks

        //file type outside of project scope
        else
        {
            printf("cannot access \'&s\': Unrecognized file type\n");
        }
        
    }
    //if no path arguments given, process current directory
    if(i == argc)
    {
        processDir(&f_args, ".");
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