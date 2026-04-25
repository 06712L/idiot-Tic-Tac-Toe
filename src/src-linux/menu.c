#include "menu.h"
#include "coco.h"
#include "game.h"
#include <stdio.h>
#include <stdlib.h>

char input;

//遊戲模式選擇
void gamemod_menu()
{
    while(1)
    {
        clear;
        printf("[1]Two player mode\n[0]return\n");
        input = getchar();
        while(getchar() != '\n');
        play_click;
        if(input == '1')
        {
            clear;
            two_people_game();
        }
        else if(input == '0')
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
        play_click;
        if(input == '0')
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
        play_click;
        if(input == '0')
        {
            return;
        }
    }
}

void how_to_play_text()
{
    printf("How to play?\n");
    printf("y x:1 2 3\n");
    printf(":\n");
    printf("1    | | \n");
    printf("    -+-+-\n");
    printf("2    | | \n");
    printf("    -+-+-\n");
    printf("3    | | \n\n");
    printf("coordinate format:xy\n\n");
    printf("examplez:\n");
    printf("input:12\n");
    printf("x = 1\ty = 2\n");
    printf(" | | \n-+-+-\n | | \n-+-+-\n | | \n  |\n  v\n");
    printf(" |O| \n-+-+-\n | | \n-+-+-\n | | \n\n");
    printf("[0]return\n");
}

//如何遊玩
void how_to_play()
{
    while(1)
    {
        clear;
        how_to_play_text();
        input = getchar();
        while(getchar() != '\n');
        play_click;
        if(input == '0')
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
        clear;
        //標題
        puts("Idiot Tic Tac Toe");
        //選項
        printf("\n[1]play\n[2]settings\n[3]about this game\n[4]how to play\n[5]quit\n");

        input = getchar();
        while(getchar() != '\n');
        play_click;

        if(input == '1') {gamemod_menu();}
        else if(input == '2') {set_menu();}
        else if(input == '3') {about_menu();}
        else if(input == '4') {how_to_play();}
        else if(input == '5')
        {
            clear;
            return;
        }
        else{clear;}
    }
}
