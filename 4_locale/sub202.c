#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <locale.h>
#define BUFF_WIDTH 256
#define BUFF_HEIGHT 256



int cmp(const void *a, const void *b)
{
    setlocale(LC_COLLATE, "zh_CN.utf8");
    return (strcoll(*(char**)a, *(char**)b));
}



int main(int argc, char *argv[])
{
    FILE *fp = fopen(argv[1], "r");
    char *buff[BUFF_HEIGHT];
    int i = 0;
    int n = 0;
    char temp[BUFF_WIDTH];

    setlocale(LC_ALL, "zh_CN.utf8");

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

    for (i = 0;i < n; i++) {
        free(buff[n]);
    }

    exit(0);
}
