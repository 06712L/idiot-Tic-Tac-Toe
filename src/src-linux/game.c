#include "game.h"
#include "var.h"
#include "coco.h"
#include <string.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

int rands(int max, int min)
{
    return rand() % (max - min + 1) + min;
}

void two_people_game()
{
    srand(time(NULL));

    int who_round = rands(1, 0);
    /*
    *number  player
    * 0    =  O
    * 1    =  X
    */
    char player_round[2] = {'O', 'X'};

    char player_ui[5][5] = 
    {
        {' ', '|', ' ', '|', ' '},
        {'-', '+', '-', '+', '-'},
        {' ', '|', ' ', '|', ' '},
        {'-', '+', '-', '+', '-'},
        {' ', '|', ' ', '|', ' '}
    };

    int tic[3][3] = 
    {
        {2, 2, 2},
        {2, 2, 2},
        {2, 2, 2}
    };

    int round = 0;
    int who_win = 2;

    while(round < 9 && who_win == 2)
    {
        printf("round %d\nTurn:%c\n\n", round, player_round[who_round]);
        for(int i = 0; i < 5; i++)
        {
            for(int j = 0; j < 5; j++)
            {
                printf("%c", player_ui[i][j]);
            }
            printf("\n");
        }
        printf("\n");
        fgets(input, 4, stdin);
        input[strcspn(input, "\n")] = '\0';
        play_click;
        char px[2] = {input[0], '\0'};
        char py[2] = {input[1], '\0'};
        int x = atoi(px);
        int y = atoi(py);

        tic[x][y] = who_round;
        if(x != 0) {x = x + 2;}
        if(y != 0) {y = y + 2;}
        player_ui[x][y] = player_round[who_round];

        int s[3];
        //橫排
        for(int i = 0; i < 3; i++)
        {
            for(int j = 0; j < 3; j++)
            {
                s[j] = tic[i][j];
            }
            if(s[0] != 2 && s[1] != 2 && s[2] != 2)
            {
                if(s[0] == 0 && s[1] == 0 && s[2] == 0) {who_win = who_round;}
                else if(s[0] == 1 && s[1] == 1 && s[2] == 1) {who_win = who_round;}
            }
        }

        //縱排
        for(int i = 0; i < 3; i++)
        {
            for(int j = 0; j < 3; j++)
            {
                s[j] = tic[j][i];
            }
            if(s[0] != 2 && s[1] != 2 && s[2] != 2)
            {
                if(s[0] == 0 && s[1] == 0 && s[2] == 0) {who_win = who_round;}
                else if(s[0] == 1 && s[1] == 1 && s[2] == 1) {who_win = who_round;}
            }
        }

        //左斜(\)
        s[0] = tic[0][0];
        s[1] = tic[1][1];
        s[2] = tic[2][2];
        if(s[0] != 2 && s[1] != 2 && s[2] != 2)
        {
            if(s[0] == 0 && s[1] == 0 && s[2] == 0) {who_win = who_round;}
            else if(s[0] == 1 && s[1] == 1 && s[2] == 1) {who_win = who_round;}
        }

        //右斜(/)
        s[0] = tic[0][2];
        s[1] = tic[1][1];
        s[2] = tic[2][0];
        if(s[0] != 2 && s[1] != 2 && s[2] != 2)
        {
            if(s[0] == 0 && s[1] == 0 && s[2] == 0) {who_win = who_round;}
            else if(s[0] == 1 && s[1] == 1 && s[2] == 1) {who_win = who_round;}
        }

        //回合結束
        if(who_round == 0) {who_round = 1;}
        else {who_round = 0;}
        round++;
    }
}