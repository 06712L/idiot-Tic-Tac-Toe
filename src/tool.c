#include "coco.h"
#include <stdlib.h>

int pow_int(int x, int y)
{
    int s = x;
    for(int i = 1; i < y; i++) {x *= s;}
    return x;
}

int rands(int max, int min) {return rand() % (max - min + 1) + min;}