#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFF_HEIGHT 256
#define BUFF_WIDTH 256



static int cmp(const void *a, const void *b)
{
    return (strcmp(*(char**)b, *(char**)a));
    //return strcmp((const char *)b, (const char *)a);
}



int main()
{
    FILE *fp = fopen("./input.txt", "r");
    char *buff[BUFF_HEIGHT];
    char temp[BUFF_WIDTH];
    int n = 0;
    int i = 0;

    while (fgets(temp, BUFF_WIDTH, fp)) {
        buff[n] = (char*)malloc(strlen(temp)+1);
        strcpy(buff[n], temp);
        n++;
    }

    fclose(fp);

    qsort(buff, n, sizeof(char*), cmp);

    for (i = 0; i < n; i++) {
        printf("%s", buff[i]);
    }

    for (i = 0; i < n; i++) {
        free(buff[i]);
    }

    return 0;
}
