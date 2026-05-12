#include "menu.h"
#include "coco.h"
#include <stdlib.h>
#include <time.h>

char *version = "V0.2-alpha.1";

int main()
{
    srand(time(NULL));
    clear;
    main_menu();

    return 0;
}
