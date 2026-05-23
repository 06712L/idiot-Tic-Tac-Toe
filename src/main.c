#include "menu.h"
#include "coco.h"
#include <stdlib.h>
#include <time.h>

char *version = "V0.2.2";
int sound = 1;

int main()
{
    srand(time(NULL));
    main_menu();

    return 0;
}
