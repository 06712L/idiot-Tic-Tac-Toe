#include "menu.h"
#include "coco.h"
#include "game.h"
#include <stdio.h>
#include <stdlib.h>

int input;

//遊戲模式選擇
void gamemod_menu()
{
    while(1)
    {
        clear;
        printf("[1]Two player mode\n[0]return\n");
        input = getchar();
        while(getchar() != '\n');
        if(input == 1)
        {
            clear;
            two_people_game();
        }
        else if(input == 0) 
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
        input = getchar();
        while(getchar() != '\n');
        if(input == 0) 
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
        printf("Version:\tV0.1-alpha.1\nDevelopers:\t06712L (Blue Cat)\ntranslator:\tGoogle Translate\n[0]return\n");
        input = getchar();
        while(getchar() != '\n');
        if(input == 0) 
        {
            return;
        }
    }
}

//主菜單
void main_menu()
{
    while(1)
    {
        //標題
        puts("Idiot Tic Tac Toe");
        //選項
        printf("[1]play\n[2]settings\n[3]about this game\n[4]\n[5]quit");


        input = getchar();
        while(getchar() != '\n');
        play_click;

        if(input == 1) {gamemod_menu();}
        else if(input == 2) {set_menu();}
        else if(input == 3) {about_menu();}
        else if(input == 4) {}
        else if(input == 5) {return;}
        else
        {
            clear;
        }
    }
}