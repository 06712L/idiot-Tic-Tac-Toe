#include "game.h"
#include "coco.h"
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#ifndef _DEFAULT_SOURCE
#define _DEFAULT_SOURCE
#endif

static int rands(int max, int min)
{
    return rand() % (max - min + 1) + min;
}

void two_people_game()
{
    char *input = NULL;

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
        re:
        setbuf(stdin, NULL);
        size_t siz_text = 0;
        clear;
        printf("round %d\nTurn:%c\n\n", round, player_round[who_round]);
        for(int i = 0; i < 5; i++)
        {
            for(int j = 0; j < 5; j++)
            {
                printf("%c", player_ui[i][j]);
            }
            printf("\n");
        }
        printf("\ninput:");
        fflush(stdout);
        //input "XY"
        getline(&input, &siz_text, stdin);
        input[strcspn(input, "\n")] = '\0';
        int error = 0;
        if(strlen(input) != 2) {error = 1;}
        char px[2] = {input[0], '\0'};
        char py[2] = {input[1], '\0'};
        int x = atoi(px);
        int y = atoi(py);
        x--;
        y--;
        if(x < 0 || y < 0) {error = 1;}

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
            for(int i =0; i < 30; i++)
            {
                puts("Egg");
                usleep(20000);
            }
            sleep(1);
            who_win = who_round;
        }
        else if(error == 1) {goto re;}

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
        for(int i = 5; i >= 0; i--)
        {
            printf("waiting... %ds\r", i);
            fflush(stdout);
            sleep(1);
        }
    }
    else
    {
        printf("Congratulations!\n\n%c win!!!\n\n", player_round[who_win]);
        play_win;
        for(int i = 5; i >= 0; i--)
        {
            printf("waiting... %ds\r", i);
            fflush(stdout);
            sleep(1);
        }
    }
    setbuf(stdin, NULL);
    return;
}


void ai_vs_human()
{
    char *input = NULL;
    char *ai_input = calloc(4, sizeof(char));

    int who_round = rands(1, 0);
    /*
     * number  player
     * 0    =  O
     * 1    =  X
     */
    char player_round[2] = {'O', 'X'};
    int who_player = rands(1, 0);

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

    puts("you are...");
    sleep(1);
    printf("%c player\n", player_round[who_player]);
    usleep(500000);
    puts("Be careful not to get defeated...");
    sleep(3);

    while(round < 9 && who_win == 2)
    {
        re:
        setbuf(stdin, NULL);
        size_t siz_text = 0;
        clear;
        printf("round %d\nTurn:%c\n\n", round, player_round[who_round]);
        for(int i = 0; i < 5; i++)
        {
            for(int j = 0; j < 5; j++)
            {
                printf("%c", player_ui[i][j]);
            }
            printf("\n");
        }
        if(who_round == who_player)
        {
            printf("\ninput:");
            fflush(stdout);
            //input "XY"
            getline(&input, &siz_text, stdin);
            input[strcspn(input, "\n")] = '\0';
            int error = 0;
            if(strlen(input) != 2) {error = 1;}
            char px[2] = {input[0], '\0'};
            char py[2] = {input[1], '\0'};
            int x = atoi(px);
            int y = atoi(py);
            x--;
            y--;
            if(x < 0 || y < 0) {error = 1;}

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
                for(int i =0; i < 30; i++)
                {
                    puts("Egg");
                    usleep(20000);
                }
                sleep(1);
                who_win = who_round;
            }
            else if(error == 1) {goto re;}
        }
        else
        {
            ai_input = calloc(4, sizeof(char));
            int ai_check = 0;
            printf("\ninput:");
            fflush(stdout);
            sleep(rands(5, 2));
            ai_check = 0;
            //Zheng-De-AI Pro
            if(tic[1][1] == 2)
            {
                strcpy(ai_input, "22");
                ai_input[strlen(ai_input)] = '\0';
            }
            else if(tic[1][1] != 2)
            {
                //橫
                for(int i = 0; i < 3; i++)
                {
                    int ss = 0;
                    for(int j = 0; j < 3; j++)
                    {
                        if(tic[i][j] == who_player) {ss++;}
                    }
                    if(ss > 1)
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
                    }
                }
                if(ai_check == 0)
                {
                    //縱
                    for(int i = 0; i < 3; i++)
                    {
                        int ss = 0;
                        for(int j = 0; j < 3; j++)
                        {
                            if(tic[j][i] == who_player) {ss++;}
                        }
                        if(ss > 1)
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
                        }
                    }
                }
                if(ai_check == 0)
                {
                    //待施工
                }
                if(ai_check == 0)
                {
                    //待施工
                }
            }
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
    free(ai_input);
    usleep(180000);

    //勝利結算
    if(who_win == 2)
    {
        printf("Oh no...\n\nyou two are tied.\n\n");
        play_tied;
        for(int i = 5; i >= 0; i--)
        {
            printf("waiting... %ds\r", i);
            fflush(stdout);
            sleep(1);
        }
    }
    else
    {
        printf("Congratulations!\n\n%c win!!!\n\n", player_round[who_win]);
        play_win;
        for(int i = 5; i >= 0; i--)
        {
            printf("waiting... %ds\r", i);
            fflush(stdout);
            sleep(1);
        }
    }
    setbuf(stdin, NULL);
    return;
}
