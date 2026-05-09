#include "menu.h"
#include "coco.h"
#include <stdlib.h>
#include <time.h>

char *version = "V0.2 Early release version.3";

int main()
{
    srand(time(NULL));
    clear;
    main_menu();

    return 0;
}
