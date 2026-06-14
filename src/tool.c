#include "coco.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>

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

int check_file(FILE *file)
{
    if(file == NULL) {return 1;}
    else {return 0;}
}

inline int32_t rands(int32_t max, int32_t min) {return rand() % (max - min + 1) + min;}

void ai_mode_name(char **ai_mode_text)
{
    /*
    * ai mode  number
    *  idiot  =  0
    *  ordinary= 1
    *  expert =  2
    *  win!   =  3
    */
    switch (ai_mode)
    {
        case 0:
            *ai_mode_text = "Mr.HotDog";
            break;
        case 1:
            *ai_mode_text = "Mr.Dog";
            break;
        case 2:
            *ai_mode_text = "Mr.Egg";
        break;
        case 3:
            *ai_mode_text = "Mr.ColdDog";
            break;
    }
}