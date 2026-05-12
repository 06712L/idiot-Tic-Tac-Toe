#include "game.h"
#include "coco.h"
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#ifdef _WIN32
#include <windows.h>
#include <mmsystem.h>
#endif
#ifndef _DEFAULT_SOURCE
#define _DEFAULT_SOURCE
#endif

static int rands(int max, int min)
{
    return rand() % (max - min + 1) + min;
}

static void wait_some_time(int time)
{
    for(int i = time; i >= 0; i--)
    {
        printf("waiting... %ds\r", i);
        fflush(stdout);
        sleep(1);
    }
    return;
}

void tic_tac_toe_game(int mod)
{
    char *input = NULL;
    #ifdef _WIN32
    input = malloc(4 * sizeof(char));
    #endif
    char *ai_input = NULL;
    int who_player;
    if(mod == 1)
    {
        ai_input = calloc(4, sizeof(char));
        who_player = rands(1, 0);
    }

    int who_round = rands(100, 0);
    if(who_round < 50) {who_round = 0;}
    else {who_round = 1;}
    /*
    *number  player
    * 0    =  O
    * 1    =  X
    * 2    =  space
    */
    const char player_round[2] = {'O', 'X'};

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

    if(mod == 1)
    {
        puts("you are...");
        sleep(1);
        printf("%c player\n", player_round[who_player]);
        usleep(500000);
        puts("Be careful not to get defeated...");
        sleep(3);
    }


    while(round < 9 && who_win == 2)
    {
        re:
        setbuf(stdin, NULL);
        #ifdef _WIN32
        int siz_text = 4;
        #else
        size_t siz_text = 0;
        #endif
        clear;
        printf("round %d\nTurn:%c\n", (round + 1), player_round[who_round]);
        if(mod == 1) {printf("you're: %c\n\n", player_round[who_player]);}
        else {printf("\n");}

        for(int i = 0; i < 5; i++)
        {
            for(int j = 0; j < 5; j++)
            {
                printf("%c", player_ui[i][j]);
            }
            printf("\n");
        }
        if(who_round == who_player || mod == 0)
        {
            printf("\ninput:");
            fflush(stdout);
            //input "XY"
            #ifdef _WIN32
            fgets(input, siz_text, stdin);
            #else
            getline(&input, &siz_text, stdin);
            #endif
            input[strcspn(input, "\n")] = '\0';
            int error = 0;
            if(strlen(input) != 2) {error = 1;}
            char px[2] = {input[0], '\0'};
            char py[2] = {input[1], '\0'};
            int x = atoi(px);
            int y = atoi(py);
            x--;
            y--;
            if(x < 0 || y < 0 || x > 2 || y > 2) {error = 1;}

            if(tic[y][x] == 2 && error == 0)
            {
                tic[y][x] = who_round;
                x *= 2;
                y *= 2;
                player_ui[y][x] = player_round[who_round];
            }
            else {error = 1;}

            if(strstr(input, "Egg") != NULL)
            {
                printf("\nplayer %c threw some Eggs!\n", player_round[who_round]);
                sleep(1);
                for(int i = 0; i < 30; i++)
                {
                    int s = rands(5, 0);
                    for(int j = 0; j < s; j++) {printf("\t");}
                    puts("Egg");
                    usleep(20000);
                }
                sleep(1);
                who_win = who_round;
            }
            else if(error == 1) {goto re;}
        }
/*        AI
          |
          v
*/
        else if(mod == 1)
        {
            int which_mod; //debug
            int which_q = 0; //debug
            //mix debug
            (void)which_mod;
            (void)which_q;

            int ai_check = 0;
            printf("\ninput:");
            fflush(stdout);
            sleep(rands(3, 1));
            ai_check = 0;
            //Zheng-De-AI Pro

            //先手佔領中心
            if(tic[1][1] == 2 && round == 0)
            {
                strcpy(ai_input, "22");
                ai_input[strlen(ai_input)] = '\0';
                which_mod = 1;
            }
            //抵禦玩家攻擊
            else
            {
                //橫
                for(int i = 0; i < 3; i++)
                {
                    int ss = 0;
                    for(int j = 0; j < 3; j++) {if(tic[i][j] == who_player) {ss++;}}
                    if(ss == 2)
                    {
                        int x;
                        for(int j = 0; j < 3; j++)
                        {
                            if(tic[i][j] == 2)
                            {
                                x = j;
                                ai_check = 1;
                                sprintf(ai_input, "%d%d", (x + 1), (i + 1));
                                break;
                            }
                        }
                        which_q = 1;
                        break;
                    }
                }
                if(ai_check == 0)
                {
                    //縱
                    for(int i = 0; i < 3; i++)
                    {
                        int ss = 0;
                        for(int j = 0; j < 3; j++) {if(tic[j][i] == who_player) {ss++;}}
                        if(ss == 2)
                        {
                            int y;
                            for(int j = 0; j < 3; j++)
                            {
                                if(tic[j][i] == 2)
                                {
                                    y = j;
                                    ai_check = 1;
                                    sprintf(ai_input, "%d%d",(i + 1), (y + 1));
                                    break;
                                }
                            }
                            which_q = 2;
                            break;
                        }
                    }
                }
                //左斜(\)
                if(ai_check == 0)
                {
                    int ss = 0;

                    for(int i = 0; i < 3; i++)
                    {
                        if(tic[i][i] == who_player) {ss++;}
                        if(ss > 1) {ai_check = 1;}
                    }
                    if(ai_check == 1)
                    {
                        for(int i = 0; i < 3; i++)
                        {
                            if(tic[i][i] == 2)
                            {
                                sprintf(ai_input, "%d%d", (i + 1), (i + 1));
                                break;
                            }
                        }
                        which_q = 3;
                    }
                }
                //右斜(/)
                if(ai_check == 0)
                {
                    int sx = 2;
                    int ss = 0;

                    for(int i = 0; i < 3; i++)
                    {
                        if(tic[i][sx] == who_player) {ss++;}
                        if(ss > 1) {ai_check = 1;}
                        sx--;
                    }
                    if(ai_check == 1)
                    {
                        sx = 2;
                        for(int i = 0; i < 3; i++)
                        {
                            if(tic[i][sx] == 2)
                            {
                                sprintf(ai_input, "%d%d", (sx + 1), (i + 1));
                                break;
                            }
                            sx--;
                        }
                        which_q = 4;
                    }
                }
                which_mod = 2;
            }
            //初代攻擊方案
            if(ai_check == 0 && tic[1][1] != 2)
            {
                int ok = 0;
                while(ok == 0)
                {
                    int x = rands(3, 1);
                    int y = rands(3, 1);

                    if(tic[y-1][x-1] == 2)
                    {
                        sprintf(ai_input, "%d%d",x, y);
                        ok = 1;
                        which_mod = 3;
                    }
                }
            }
            printf("%s", ai_input);
            fflush(stdout);
            sleep(1);
            char px[2] = {ai_input[0], '\0'};
            char py[2] = {ai_input[1], '\0'};
            int x = atoi(px);
            int y = atoi(py);
            x--;
            y--;

            tic[y][x] = who_round;
            x *= 2;
            y *= 2;
            player_ui[y][x] = player_round[who_round];
        }

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
                if(s[0] == 0 && s[1] == 0 && s[2] == 0) {who_win = 0;}
                else if(s[0] == 1 && s[1] == 1 && s[2] == 1) {who_win = 1;}
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
                if(s[0] == 0 && s[1] == 0 && s[2] == 0) {who_win = 0;}
                else if(s[0] == 1 && s[1] == 1 && s[2] == 1) {who_win = 1;}
            }
        }

        //左斜(\)
        s[0] = tic[0][0];
        s[1] = tic[1][1];
        s[2] = tic[2][2];
        if(s[0] != 2 && s[1] != 2 && s[2] != 2)
        {
            if(s[0] == 0 && s[1] == 0 && s[2] == 0) {who_win = 0;}
            else if(s[0] == 1 && s[1] == 1 && s[2] == 1) {who_win = 1;}
        }

        //右斜(/)
        s[0] = tic[0][2];
        s[1] = tic[1][1];
        s[2] = tic[2][0];
        if(s[0] != 2 && s[1] != 2 && s[2] != 2)
        {
            if(s[0] == 0 && s[1] == 0 && s[2] == 0) {who_win = 0;}
            else if(s[0] == 1 && s[1] == 1 && s[2] == 1) {who_win = 1;}
        }

        //回合結束
        if(who_round == 0) {who_round = 1;}
        else {who_round = 0;}
        round++;
        setbuf(stdin, NULL);
        play_click;
    }

    clear;
    free(input);
    usleep(180000);

    //勝利結算
    if(who_win == 2)
    {
        printf("Oh no...\n\nyou two are tied.\n\n");
        play_tied;
        wait_some_time(5);
    }
    else if(mod == 0)
    {
        printf("Congratulations!\n\n%c win!!!\n\n", player_round[who_win]);
        play_win;
        wait_some_time(5);
    }
    else if(mod == 1)
    {
        if(who_win == who_player)
        {
            printf("Congratulations!\n\nyou win!!!\n\n");
            play_win;
        }
        else
        {
            printf("oh no...\n\n you lose.by a idiot AI\n\n");
            play_lose;
        }
        wait_some_time(5);
    }
    setbuf(stdin, NULL);
    return;
}
