#include "menu.h"
#include "coco.h"
#include <stdlib.h>
#include <time.h>

char *version = "V0.3-alpha.1";
int sound = 1;

int main()
{
    srand(time(NULL));
    main_menu();

    return 0;
}
