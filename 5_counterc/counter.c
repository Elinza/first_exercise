#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdint.h>

#define DIREC_STR "/"
#define PATH_SIZE 256



static int ProcessDirectory(const char *dir, int64_t *total, int *big)
{
    struct stat buf;
    struct dirent *dirp;
    DIR *dp;
    int n = 0;
    int i = 0;
    char *temp;
    char *path[PATH_SIZE];

    if ((dp = opendir(dir)) == NULL) {
        printf("can't open %s\n", dir);
        return 0;
    }

    while ((dirp = readdir(dp)) != NULL) {
        temp = (char *)malloc(strlen(dirp->d_name) + strlen(dir) + 4);
        strcpy(temp, dir);
        strcat(temp, DIREC_STR);
        strcat(temp, dirp->d_name);
        stat(temp, &buf);
        if (S_ISREG(buf.st_mode)) {
            (*total) += buf.st_size;
            if(buf.st_size > 1024*1024){
                (*big)++;
            }
        }
        if (S_ISDIR(buf.st_mode)) {
            if(strcmp(dirp->d_name, "..") == 0 || strcmp(dirp->d_name, ".")
                    == 0){
                free(temp);
                continue;
            }
            path[n] = (char *)malloc(strlen(temp) + 1);
            strcpy(path[n], temp);
            n++;
        }
        free(temp);
    }

    closedir(dp);

    for (i = 0; i < n; i++) {
        ProcessDirectory(path[i], total, big);
        free(path[i]);
    }

    return 0;
}



int main(int argc, char *argv[])
{
    int64_t total = 0;
    int big = 0;

    if(argc!= 2)
        printf("wrong\n");

    ProcessDirectory(argv[1], &total, &big);

    printf("The total is %ld,There are %d big file\n", total, big);

    return 0;
}
