#include "coco.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void wait_some_time(int time)
{
    for(int i = time; i >= 0; i--)
    {
        printf("waiting %d second to leif\r", i);
        fflush(stdout);
        sleep(1);
    }
    return;
}

int check_file(FILE **file)
{
    int s = 0;
    if(*file == NULL) {s = 1;}
    return s;
}

inline int rands(int max, int min) {return rand() % (max - min + 1) + min;}