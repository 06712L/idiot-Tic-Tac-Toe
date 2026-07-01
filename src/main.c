#include "menu.h"
#include "coco.h"
#include "savefile.h"
#include <stdlib.h>
#include <time.h>
#include <stdint.h>
#define ERROR 1
#define PASS 0

const char *const version = "V0.4-beta.1";
int8_t sound = UNKNOWN;
int8_t ai_mode = UNKNOWN;
int8_t discovered_what = UNKNOWN;

int main()
{
    clear;

    boot_loading();

    //播種
    srand(time(NULL));

    //進入遊戲
    main_menu();

    return quit_savefile();
}
