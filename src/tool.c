#include "coco.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int pow_int(int x, int y)
{
    int s = x;
    for(int i = 1; i < y; i++) {x *= s;}
    return x;
}

int check_file(FILE **file)
{
    int s = 0;
    if(*file == NULL)
    {
        puts("Loading failed");
        sleep(1);
        s = 1;
    }

    return s;
}

int rands(int max, int min) {return rand() % (max - min + 1) + min;}