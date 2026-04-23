#include "menu.h"
#include "var.h"
#include "coco.h"
#include "game.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void gamemod_menu();
void set_menu();
void about_menu();

//主菜單
void main_menu()
{
    input = (char *)calloc(4, sizeof(char));
    while(1)
    {
        //標題
        puts("Idiot Tic Tac Toe");
        //選項
        printf("[1]play\n[2]settings\n[3]about this game\n[4]quit");


        *input = getchar();
        input[strcspn(input, "\n")] = '\0';
        play_click;

        if(strcmp(input, "1"))
        {
            gamemod_menu();
        }
        else if(strcmp(input, "2"))
        {
            set_menu();
        }
        else if(strcmp(input, "3"))
        {
            about_menu();
        }
        else if(strcmp(input, "4"))
        {
            free(input);
            input = NULL;
            return;
        }
        else
        {
            clear;
        }
    }
}

//遊戲模式選擇
void gamemod_menu()
{
    while(1)
    {
        clear;
        printf("[1]Two player mode\n[0]return\n");
        *input = getchar();
        if(strcmp(input, "1"))
        {
            clear;
            two_people_game();
        }
        else if(strcmp(input, "0")) 
        {
            return;
        }
    }
}

//設定
void set_menu()
{
    while(1)
    {
        clear;
        printf("Nothing in there\n[0]return\n");
        *input = getchar();
        if(strcmp(input, "0")) 
        {
            return;
        }
    }
}

//關於
void about_menu()
{
    while(1)
    {
        clear;
        printf("Version:\tV0.1-alpha.1\nDevelopers:\t06712L (Blue Cat)\ntranslator:Google Translate\n[0]return\n");
        *input = getchar();
        if(strcmp(input, "0")) 
        {
            return;
        }
    }
}