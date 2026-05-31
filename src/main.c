#include "menu.h"
#include "coco.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

char *version = "V0.3";
int sound = 4;
int ai_mode = 4;


int main()
{
    //載入/創建 設定檔
    int s = 0;
    //檢查設定檔是否存在
    FILE *fi = fopen("set.txt", "a");
    if(check_file(&fi))
    {
        puts("Failed to create archive");
        sleep(1);
        return 1;
    }
    fclose(fi);
    
    fi = fopen("set.txt", "r");
    if(check_file(&fi))
    {
        puts("Read failed");
        sleep(1);
        return 1;
    }

    //檢查是否含內容
    if(fgetc(fi) == EOF)
    {
        fclose(fi);
        fi = fopen("set.txt", "a");
        if(check_file(&fi))
        {
            puts("Loading failed");
            sleep(1);
            return 1;
        }
        fputs("1\n2\n", fi); //sound, ai_mode
        fclose(fi);
        fi = fopen("set.txt", "r");
    }
    //EOF = -1

    //寫入配置至變數
    int i = 0;
    rewind(fi);
    while(fscanf(fi, "%d", &s) > 0 && i < 2)
    {
        if(i == 0 && s < 2 && s >= 0) {sound = s;}
        else if(i == 1 && s < 4 && s >= 0) {ai_mode = s;}
        i++;
    }
    fclose(fi);

    if(sound == 4 || ai_mode == 4)
    {
        sound = 1;
        ai_mode = 2;

        fi = fopen("set.txt", "w");
        if(check_file(&fi))
        {
            printf("Repair failed\nPlease manually copy the following content to set.txt :\n1\n2\n");
            sleep(1);
            return 1;
        }
        fputs("1\n2\n", fi); //sound, ai_mode
        fclose(fi);
    }

    //播種
    srand(time(NULL));

    main_menu();

    fi = fopen("set.txt", "w");
    if(check_file(&fi))
    {
        puts("Save failed");
        sleep(1);
        return 1;
    }
    fprintf(fi, "%d\n%d\n", sound, ai_mode);
    fclose(fi);

    return 0;
}
