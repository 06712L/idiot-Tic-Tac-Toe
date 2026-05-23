#include "menu.h"
#include "coco.h"
#include "game.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#ifdef _WIN32
#include <windows.h>
#include <mmsystem.h>
#endif

char input;

static void what_menu()
{
    char ui[5];
    char what_c[5] = {'W', 'H', 'A', 'T', '?'};
    int what_what[5];
    for(int i = 0; i < 5; i++) {what_what[i] = 5;}
    for(int i = 0; i < 5;)
    {
        int can = 0;
        int s = rands(4, 0);
        for(int j = 0; j < 5; j++)
        {
            if(what_what[j] != s) {can = 1;}
            else
            {
                can = 0;
                break;
            }
        }

        if(can == 1)
        {
            what_what[i] = s;
            i++;
        }
    }

    int done = 0;

    while(!done)
    {
        re_input:
        for(int i = 0; i < 5; i++) {ui[i] = ' ';}
        clear;
        for(int i = 0; i < 5; i++)
        {
            if(what_what[i] == i) {printf("%c", what_c[i]);}
            else {putchar('*');}
        }
        putchar('\n');

        printf("input(1~5):");
        fflush(stdout);
        input = getchar();
        while(getchar() != '\n');
        char sinput[2] = {input, '\0'};
        int s_num = atoi(sinput);
        s_num--;
        if(s_num < 0 || s_num > 4) {goto re_input;}
        ui[s_num] = '^';
        play_click

        re_in:
        clear;
        for(int i = 0; i < 5; i++)
        {
            if(what_what[i] == i) {printf("%c", what_c[i]);}
            else {putchar('*');}
        }
        putchar('\n');
        for(int i = 0; i < 5; i++) {printf("%c", ui[i]);}
        putchar('\n');

        printf("input(1~5):");
        fflush(stdout);
        input = getchar();
        while(getchar() != '\n');
        char ssinput[2] = {input, '\0'};
        int ss_num = atoi(ssinput);
        ss_num--;
        if(ss_num < 0 || ss_num > 4 || ss_num == s_num) {goto re_in;}
        play_click

        int s = what_what[s_num];
        what_what[s_num] = what_what[ss_num];
        what_what[ss_num] = s;

        for(int i = 0; i < 5; i++)
        {
            if(what_what[i] == i) {done = 1;}
            else
            {
                done = 0;
                break;
            }
        }
    }
    clear;
    puts("Password passed");
    sleep(1);

    if(done == 1)
    {
        clear;
        what();
    }

    return;
}

//遊戲模式選擇
static void gamemod_menu()
{
    while(1)
    {
        /*
         * mod
         * 0 = two-persons-game
         * 1 = ai-vs-human
        */
        clear;
        printf("[1]Two player mode\n[2]AI vs Human\n[0]return\n");
        input = getchar();
        while(getchar() != '\n');
        play_click

        if(input == '1')
        {
            clear;
            tic_tac_toe_game(0);
        }

        else if(input == '2')
        {
            clear;
            tic_tac_toe_game(1);
        }

        else if(input == '8')
        {
            clear;
            what_menu();
        }

        else if(input == '0') {return;}
    }
}

//設定
static void set_menu()
{
    while(1)
    {
        clear;
        printf("[1]silent mode(%c)\n[0]return\n", sound ? 'X':'V');
        input = getchar();
        while(getchar() != '\n');
        play_click
        if(input == '1')
        {
            if(!sound) {sound = 1;}
            else {sound = 0;}
        }
        if(input == '0') {return;}
    }
}

//關於
static void about_menu()
{
    while(1)
    {
        clear;
        printf("Version:\t%s\nDevelopers:\t06712L (Blue Cat)\ntranslator:\tGoogle Translate\n[0]return\n", version);
        input = getchar();
        while(getchar() != '\n');
        play_click

        if(input == '0') {return;}
    }
}

static void how_to_play_text()
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
    printf("example:\n");
    printf("input:21\n");
    printf("x = 2\ty = 1\n");
    printf(" | | \n-+-+-\n | | \n-+-+-\n | | \n  |\n  v\n");
    printf(" |O| \n-+-+-\n | | \n-+-+-\n | | \n\n");
    printf("[0]return\n");
}

//如何遊玩
static void how_to_play()
{
    while(1)
    {
        clear;
        how_to_play_text();
        input = getchar();
        while(getchar() != '\n');
        play_click

        if(input == '0') {return;}
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
        play_click

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
