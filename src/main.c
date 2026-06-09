#include "menu.h"
#include "coco.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <stdint.h>
//加載專用
#define UKNOW 4
#define ERROR 1
#define PASS 0

const char *version = "V0.3.2";
int sound = UKNOW;
int ai_mode = UKNOW;


int main()
{
    clear;
    //載入/創建 設定檔 V2.0
    uint8_t loading[3];
    for(uint8_t i = 0; i < 3; i++) {loading[i] = PASS;}
    //1.檢查設定檔是否存在
    FILE *fi = fopen("set.txt", "a");
    if(check_file(&fi)) {loading[0] = ERROR;}
    fclose(fi);

    //轉讀取模式
    fi = fopen("set.txt", "r");
    if(check_file(&fi))
    {
        for(uint8_t i = 1; i < 3; i++) {loading[i] = ERROR;}
        sound = 1;
        ai_mode = 2;
        goto just_start;
    }

    //2.檢查是否含內容，如沒有則添加默認值
    if(fgetc(fi) == EOF)
    {
        fclose(fi);
        //轉附加模式
        fi = fopen("set.txt", "a");
        if(check_file(&fi))
        {
            loading[2] = ERROR;
            sound = 1;
            ai_mode = 2;
            goto just_start;
        }
        fputs("1\n2\n", fi); //sound, ai_mode
        fclose(fi);
        fi = fopen("set.txt", "r");
    }
    //EOF = -1

    //寫入配置至變數
    uint8_t i = 0;
    int s = 0;
    rewind(fi);
    while(fscanf(fi, "%d", &s) > 0 && i < 2)
    {
        if(i == 0 && s < 2 && s >= 0) {sound = s;}
        else if(i == 1 && s < 4 && s >= 0) {ai_mode = s;}
        i++;
    }
    fclose(fi);

    //3.檢查是否寫入或寫入異常，若是則採用預設值
    if(sound == UKNOW || ai_mode == UKNOW)
    {
        sound = 1;
        ai_mode = 2;

        //轉覆寫模式
        fi = fopen("set.txt", "w");
        if(check_file(&fi))
        {
            loading[2] = ERROR;
            goto just_start;
        }
        fputs("1\n2\n", fi); //sound, ai_mode
        fclose(fi);
    }


    just_start:
    //加載報告
    putchar('{');
    uint8_t error = 0;
    for(uint8_t i = 0; i < 3; i++) 
    {
        if(loading[i]) {error = 1;}
        printf("%c", loading[i] ? 'X':'V');
    }
    printf("}\n\n");
    printf("[%s]\tFile inspection and creation\n[%s]\tCheck the file contents.\n[%s]\tCheck if copied to variable\n\n", loading[1] ? "FAIL":"PASS", loading[2] ? "FAIL":"PASS", loading[0] ? "FAIL":"PASS");
    
    if(error)
    {
        printf("recommend:\n\n");
        puts("> Please grant the software read and write permissions, otherwise default values will be used.");
    }
    else {puts("Well done!");}
    putchar('\n');

    if(error) {wait_some_time(10);}
    else {wait_some_time(3);}

    //播種
    srand(time(NULL));

    main_menu();

    //保存
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
