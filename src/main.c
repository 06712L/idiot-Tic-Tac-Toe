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

const char *version = "V0.4-alpha.1";
int32_t sound = UKNOW;
int32_t ai_mode = UKNOW;
int32_t discovered_what = UKNOW;



int main()
{
    clear;
    //載入/創建 設定檔 V2.5
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
        discovered_what = 0;
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
            discovered_what = 0;
            goto just_start;
        }
        fputs("1\n2\n0\n", fi); //sound, ai_mode, discovered_what
        fclose(fi);
        fi = fopen("set.txt", "r");
        rewind(fi);
    }
    //EOF = -1

    re_set:
    //寫入配置至變數
    //沒用，專門消不能在goto後宣告變數的
    rewind(fi);

    uint8_t si = 0;
    int32_t s = 0;
    while(fscanf(fi, "%d", &s) > 0 && si < 3)
    {
        if(si == 0 && s < 2 && s >= 0) {sound = s;}
        else if(si == 1 && s < 3 && s >= 0) {ai_mode = s;}
        else if(si == 2 && s < 2 && s >= 0) {discovered_what = s;}
        si++;
    }
    si = 0;
    fclose(fi);

    //3.檢查是否寫入或寫入異常，若是則採用預設值
    if(sound == UKNOW || ai_mode == UKNOW || discovered_what == UKNOW)
    {
        //建立修復清單
        uint8_t need_fix[3];
        for(uint8_t i = 0; i < 3; i++) {need_fix[i] = 0;}
        if(sound == UKNOW) {need_fix[0] = 1;}
        if(ai_mode == UKNOW) {need_fix[1] = 1;}
        if(discovered_what == UKNOW) {need_fix[2] = 1;}
        //轉覆寫模式
        fi = fopen("set.txt", "w");
        if(check_file(&fi))
        {
            if(need_fix[0]) {sound = 1;}
            if(need_fix[1]) {ai_mode = 2;}
            if(need_fix[2]) {discovered_what = 0;}
            loading[2] = ERROR;
            goto just_start;
        }

        while(si < 3)
        {
            //是在修復清單裡的
            if(need_fix[si])
            {
                if(si == 0) {fputs("1\n", fi);}
                else if(si == 1) {fputs("2\n", fi);}
                else if(si == 2) {fputs("0\n", fi);}
            }
            //不在修復清單的，則寫回保存的值
            else
            {
                if(si == 0) {fprintf(fi, "%d\n", sound);}
                else if(si == 1) {fprintf(fi, "%d\n", ai_mode);}
                else if(si == 2) {fprintf(fi, "%d\n", discovered_what);}
            }
            si++;
        }
        fclose(fi);
        fi = fopen("set.txt", "r");
        if(check_file(&fi))
        {
            if(need_fix[0]) {sound = 1;}
            if(need_fix[1]) {ai_mode = 2;}
            if(need_fix[2]) {discovered_what = 0;}
            loading[2] = ERROR;
            goto just_start;
        }
        goto re_set;
    }


    just_start:
    //加載報告
    puts("Configuration file check system V2.5");
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
    else {wait_some_time(1);}

    //播種
    srand(time(NULL));

    //進入遊戲
    main_menu();

    //保存
    fi = fopen("set.txt", "w");
    if(check_file(&fi))
    {
        puts("> Save failed");
        sleep(1);
        return 1;
    }
    fprintf(fi, "%d\n%d\n%d\n", sound, ai_mode, discovered_what);
    fclose(fi);

    return 0;
}
