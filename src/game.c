#include "game.h"
#include "coco.h"
#include <stddef.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#ifdef _WIN32
#include <windows.h>
#include <mmsystem.h>
#endif
#ifndef _DEFAULT_SOURCE
#define _DEFAULT_SOURCE
#endif

static void greedy(uint8_t who_round, uint8_t who_player, uint8_t tic[3][3], char **ai_input)
{
    enum
    {
        CAN_WIN = 25000,
        CAN_RESIST = 7500,
        CAN_ATTACK = 1000,
        NOBODY = 2
    };

    typedef struct po
    {
        uint16_t point;
        uint8_t x;
        uint8_t y;
        struct po *next;
    }points;

    //初始化點數計載點
    points *head = malloc(sizeof(points));
    head -> next = NULL;
    head -> point = 0;
    head -> x = UNKNOWN;
    head -> y = UNKNOWN;
    points *tail = head;

    bool first = true;
    for(uint8_t y = 0; y < 3; y++)
    {
        for(uint8_t x = 0; x < 3; x++)
        {
            //檢查是否為空格idiot-Tic-Tac-Toe
            if(tic[y][x] == NOBODY)
            {
                //如果不是第一項則建立新點數計載點
                if(!first)
                {
                    points *new = calloc(1, sizeof(points));
                    new -> next = NULL;

                    tail -> next = new;
                    tail = new;
                }
                //紀錄位置
                tail -> x = x;
                tail -> y = y;

                //建立下棋後的棋盤
                uint8_t ai_tic[3][3];
                memcpy(ai_tic, tic, (sizeof(*tic) * 3));
                ai_tic[y][x] = who_round;

                uint16_t *point = &(tail -> point);
                //橫縱
                for(uint8_t h = 0; h < 2; h++)
                {
                    uint8_t s = 0 /*己方*/, xs = 0/*敵方*/;
                    for(uint8_t i = 0; i < 3;i++)
                    {
                        if(h)
                        {
                            if(ai_tic[y][i] == who_round) {s++;}
                            else if(ai_tic[y][i] == who_player) {xs++;}
                        }
                        else
                        {
                            if(ai_tic[i][x] == who_round && ai_tic[i][x] != NOBODY) {s++;}
                            else if(ai_tic[i][x] == who_player && ai_tic[i][x] != NOBODY) {xs++;}
                        }
                    }
                    if(s == 3) {*point += CAN_WIN;}
                    else if(xs == 2) {*point += CAN_RESIST;}
                    else if(s > 1 && xs < 1) {*point += CAN_ATTACK;}
                }

                //斜線
                if(abs(x - y) != 1)
                {
                    for(uint8_t h = 0; h < 2; h++)
                    {
                        uint8_t s = 0 /*己方*/, xs = 0/*敵方*/;
                        uint8_t sx = 2;
                        for(uint8_t i = 0; i < 3; i++)
                        {
                            if(h)
                            {
                                if(ai_tic[i][i] == who_round && ai_tic[i][i] != NOBODY) {s++;}
                                else if(ai_tic[i][i] == who_player && ai_tic[i][i] != NOBODY) {xs++;}
                            }
                            else
                            {
                                if(ai_tic[i][sx] == who_round && ai_tic[i][sx] != NOBODY) {s++;}
                                else if(ai_tic[i][sx] == who_player && ai_tic[i][sx] != NOBODY) {xs++;}
                            }
                            sx--;
                        }
                        if(s == 3) {*point += CAN_WIN;}
                        else if(xs == 2) {*point += CAN_RESIST;}
                        else if(s > 1 && xs < 1) {*point += CAN_ATTACK;}
                    }
                }
                first = false;
            }
        }
    }

    if(!first && tail -> x != UNKNOWN && tail -> y != UNKNOWN)
    {
        uint16_t max_point = 0;
        points *best = head;
        points *p = head;
        while(p != NULL)
        {
            if(p -> point > max_point)
            {
                best = p;
                max_point = p -> point;
            }
            p = p -> next;
        }
        sprintf(*ai_input, "%d%d", ((best -> x) + 1), ((best -> y) + 1));
    }

    points *p = head;
    while(p != NULL)
    {
        points *next_p = p -> next;
        free(p);
        p = next_p;
    }
    return;
}

void tic_tac_toe_game(int mod)
{
    enum
    {
        O = 0,
        X = 1,
        NOBODY = 2
    };

    #ifdef _WIN32
    char *input = malloc(20 * sizeof(char));
    #endif
    char *ai_input = NULL;
    char *ai_mode_text;

    uint8_t who_player = NOBODY;
    if(mod == AI_MODE || mod == AI_VS_AI_MODE)
    {
        ai_input = calloc(8, sizeof(char));

        ai_mode_name(&ai_mode_text);

        who_player = rands(100, 0);

        if(who_player < 50) {who_player = O;}
        else {who_player = X;}
    }

    bool who_round = X;
    /*
    *number  player
    * 0    =  O
    * 1    =  X
    * 2    =  space
    */
    const char player_round[2] = {'O', 'X'};

    char player_ui[5][5]; 
    for(uint8_t i = 0; i < 5; i++)
    {
        if(i & 1)
        {
            for(uint8_t j = 0; j < 5; j++)
            {
                if(j & 1) {player_ui[i][j] = '+';}
                else {player_ui[i][j] = '-';}
            }
        }

        else
        {
            for(uint8_t j = 0; j < 5; j++)
            {
                if(j & 1) {player_ui[i][j] = '|';}
                else {player_ui[i][j] = ' ';}
            }
        }
    }

    uint8_t tic[3][3];
    for(uint8_t i = 0; i < 3; i++) {for(uint8_t j = 0; j < 3; j++) {tic[i][j] = NOBODY;}}

    uint8_t round = 0;
    uint8_t who_win = NOBODY;

    if(mod == AI_MODE)
    {
        puts("you are...");
        sleep(1);
        printf("%c player\n", player_round[who_player]);
        usleep(500000);
        puts("Your opponent is...");
        sleep(1);
        printf("%s\n", ai_mode_text);
        sleep(1);
        puts("Be careful not to get defeated...");
        sleep(3);
    }


    while(round < 9 && who_win == NOBODY)
    {
        re:
        if(mod == TWO_PEOPLE_MODE) {who_player = who_round;}
        else if(mod == AI_VS_AI_MODE) {who_player = !who_player;}
        setbuf(stdin, NULL);
        #ifdef _WIN32
        int siz_text = 20;
        #else 
        char *input = NULL;
        size_t siz_text = 0;
        #endif
        clear;
        printf("round %d\nTurn:%c\n", (round + 1), player_round[who_round]);
        if(mod == AI_MODE) {printf("you're: %c\n\n", player_round[who_player]);}
        else {putchar('\n');}

        for(int i = 0; i < 5; i++)
        {
            for(int j = 0; j < 5; j++)
            {
                printf("%c", player_ui[i][j]);
            }
            putchar('\n');
        }
        if(who_round == who_player && mod <= AI_MODE)
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
            int error = FALSE;
            if(strlen(input) != 2) {error = TRUE;}
            char px[2] = {input[0], '\0'};
            char py[2] = {input[1], '\0'};
            int8_t x = atoi(px);
            int8_t y = atoi(py);
            x--;
            y--;
            if(x < 0 || y < 0 || x > 2 || y > 2) {error = TRUE;}

            if(tic[y][x] == NOBODY && error == FALSE && x >= 0 && y >= 0 && x < 3 && y < 3)
            {
                tic[y][x] = who_round;
                x *= 2;
                y *= 2;
                player_ui[y][x] = player_round[who_round];
            }
            else {error = TRUE;}

            if(strstr(input, "Egg") != NULL)
            {
                printf("\nplayer %c threw some Eggs!\n", player_round[who_round]);
                sleep(1);
                for(int i = 0; i < 30; i++)
                {
                    int s = rands(5, 0);
                    for(int j = 0; j < s; j++) {putchar('\t');}
                    puts("Egg");
                    usleep(20000);
                }
                sleep(1);
                who_win = who_round;
            }
            else if(error == TRUE)
            {
                #ifdef __linux__
                free(input);
                #endif
                goto re;
            }
        }
/*      AI
        |
        v
*/
        else if(mod == AI_MODE || mod == AI_VS_AI_MODE)
        {
            int which_mod = 0; //debug
            int which_q = 0; //debug
            //mix debug
            (void)which_mod;
            (void)which_q;
            /*
            * mod    number
            * Egg  =   5 (ai_mode 2)
            * 先手占中=1 (ai_mode 0)
            * 攻   =   2 (ai_mode 1)
            * 守   =   3 (ai_mode 2)
            * 建立攻 = 6 (ai_mode 2)
            * 隨機 =   4 (ai_mode 0)
            */

            int want_egg = rands(100, 0);
            int ai_check = FALSE;
            printf("\ninput:");
            fflush(stdout);
            sleep(rands(3, 1));
            //Zheng-De-AI Pro

            if(ai_mode < 3)
            {
                //ai will Egg!
                if(want_egg >= 95 && ai_mode >= 2)
                {
                    strcpy(ai_input, "Egg");
                    ai_input[strlen(ai_input)] = '\0';
                    ai_check = 1;
                    which_mod = 5;
                }

                //先手佔領中心
                if(tic[1][1] == NOBODY && round == 0 && ai_check == 0 && ai_mode >= 0)
                {
                    strcpy(ai_input, "22");
                    ai_input[strlen(ai_input)] = '\0';
                    ai_check = 1;
                    which_mod = 1;
                }

                //第二代攻擊方案
                if(ai_check == 0 && ai_mode >= 1)
                {
                    //橫
                    for(int i = 0; i < 3; i++)
                    {
                        int ss = 0;
                        for(int j = 0; j < 3; j++) {if(tic[i][j] == who_round && tic[i][j] != who_player) {ss++;}}
                        
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
                            for(int j = 0; j < 3; j++) {if(tic[j][i] == who_round && tic[j][i] != who_player) {ss++;}}
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
                            if(tic[i][i] == who_round && tic[i][i] != who_player) {ss++;}
                            if(ss == 2) {ai_check = 1;}
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
                    if (ai_check == 0)
                    {
                        int sx = 2;
                        int ss = 0;

                        for(int i = 0; i < 3; i++)
                        {
                            if(tic[i][sx] == who_round && tic[i][sx] != who_player) {ss++;}
                            if(ss == 2) {ai_check = 1;}
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
                //抵禦玩家攻擊
                if(ai_check == 0 && ai_mode >= 2)
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
                    which_mod = 3;
                }

                //第三代攻擊方案
                if(ai_check == 0 && ai_mode >= 2)
                {
                    //橫
                    for(int i = 0; i < 3; i++)
                    {
                        int ss = 0;
                        int xs = 0;
                        for(int j = 0; j < 3; j++)
                        {
                            if(tic[i][j] == who_round && tic[i][j] != who_player) {ss++;}
                            else if(tic[i][j] == who_player) {xs++;}
                        }
                        
                        if(ss > 0 && xs < 1)
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
                            int xs = 0;
                            for(int j = 0; j < 3; j++)
                            {
                                if(tic[j][i] == who_round && tic[j][i] != who_player) {ss++;}
                                else if(tic[j][i] == who_player) {xs++;}
                            }
                            if(ss > 0 && xs < 1)
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
                        int xs = 0;

                        for(int i = 0; i < 3; i++)
                        {
                            if(tic[i][i] == who_round && tic[i][i] != who_player) {ss++;}
                            else if(tic[i][i] == who_player) {xs++;}
                        }
                        if(ss > 0 && xs < 1)
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
                    if (ai_check == 0)
                    {
                        int sx = 2;
                        int ss = 0;
                        int xs = 0;

                        for(int i = 0; i < 3; i++)
                        {
                            if(tic[i][sx] == who_round && tic[i][sx] != who_player) {ss++;}
                            else if(tic[i][sx] == who_player) {xs++;}
                            sx--;
                        }
                        if(ss > 0 && xs < 1)
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

                    which_mod = 6;
                }

                //亂下
                if(ai_check == 0 && ai_mode >= 0)
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
                            which_mod = 4;
                        }
                    }
                }
            }

            else if(ai_mode == 3)
            {
                enum
                {
                    WILL_WIN = 25000,
                    WILL_LOSE = (-20000),
                    WILL_TIDE = 10000
                };

                typedef struct po
                {
                    int16_t point;
                    uint8_t x;
                    uint8_t y;
                    struct po *next;
                }points;

                //初始化點數計載點
                points *head = malloc(sizeof(points));
                head -> next = NULL;
                head -> point = 0;
                points *tail = head;

                bool first = true;
                for(uint8_t y = 0; y < 3; y++)
                {
                    for(uint8_t x = 0; x < 3; x++)
                    {
                    //檢查是否為空格
                        if(tic[y][x] == NOBODY)
                        {
                            //如果不是第一項則建立新點數計載點
                            if(!first)
                            {
                                points *new = calloc(1, sizeof(points));
                                new -> next = NULL;

                                tail -> next = new;
                                tail = new;
                            }
                            //紀錄位置
                            tail -> x = x;
                            tail -> y = y;

                            //建立下棋後的棋盤
                            uint8_t ai_tic[3][3];
                            memcpy(ai_tic, tic, (sizeof(*tic) * 3));
                            ai_tic[y][x] = who_round;
                            //建立虛擬誰的回合
                            bool ai_who_round = who_round;
                            //簡寫point
                            int16_t *point = &(tail -> point);


                            //模擬回合
                            for(uint8_t ai_round = round; ai_round < 9 && tail -> point == 0; ai_round++)
                            {
                                char *s_ai_input = calloc(8, sizeof(char));
                                greedy(ai_who_round, who_player, ai_tic, &s_ai_input);

                                //處理輸入
                                char px[2] = {s_ai_input[0], '\0'};
                                char py[2] = {s_ai_input[1], '\0'};
                                int x = atoi(px);
                                int y = atoi(py);
                                x--;
                                y--;
                                ai_tic[y][x] = ai_who_round;

                                //橫縱
                                for(uint8_t h = 0; h < 2; h++)
                                {
                                    uint8_t s = 0 /*己方*/, xs = 0/*敵方*/;
                                    for(uint8_t i = 0; i < 3;i++)
                                    {
                                        if(h)
                                        {
                                            if(ai_tic[y][i] == who_round) {s++;}
                                            else if(ai_tic[y][i] == who_player) {xs++;}
                                        }
                                        else
                                        {
                                            if(ai_tic[i][x] == who_round && ai_tic[i][x] != NOBODY) {s++;}
                                            else if(ai_tic[i][x] == who_player && ai_tic[i][x] != NOBODY) {xs++;}
                                        }
                                    }
                                    if(s == 3) {*point = WILL_WIN;}
                                    else if(xs == 3) {*point = WILL_LOSE;}
                                }

                                //斜線
                                for(uint8_t h = 0; h < 2; h++)
                                {
                                    uint8_t s = 0 /*己方*/, xs = 0/*敵方*/;
                                    uint8_t sx = 2;
                                    for(uint8_t i = 0; i < 3; i++)
                                    {
                                        if(h)
                                        {
                                            if(ai_tic[i][i] == who_round && ai_tic[i][i] != NOBODY) {s++;}
                                            else if(ai_tic[i][i] == who_player && ai_tic[i][i] != NOBODY) {xs++;}
                                        }
                                        else
                                        {
                                            if(ai_tic[i][sx] == who_round && ai_tic[i][sx] != NOBODY) {s++;}
                                            else if(ai_tic[i][sx] == who_player && ai_tic[i][sx] != NOBODY) {xs++;}
                                        }
                                        sx--;
                                    }
                                    if(s == 3) {*point = WILL_WIN;}
                                    else if(xs == 3) {*point = WILL_LOSE;}
                                }

                                //回合結束
                                ai_who_round = !ai_who_round;
                            }
                            if(*point == 0) {*point = WILL_TIDE;}
                            first = false;
                        }
                    }
                }

                points *best = NULL;
                if(!first && tail -> x != UNKNOWN && tail -> y != UNKNOWN)
                {
                    int16_t max_point = 0;

                    points *p = head;
                    while(p != NULL)
                    {
                        if(p -> point > max_point)
                        {
                            best = p;
                            max_point = p -> point;
                        }
                        p = p -> next;
                    }
                    sprintf(ai_input, "%d%d", ((best -> x) + 1), ((best -> y) + 1));
                }


                points *p = head;
                while(p != NULL)
                {
                    points *next_p = p -> next;
                    free(p);
                    p = next_p;
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
            if(strstr(ai_input, "Egg") != NULL)
            {
                printf("\nAI threw some Eggs!\n");
                sleep(1);
                for(int i = 0; i < 30; i++)
                {
                    int s = rands(5, 0);
                    for(int j = 0; j < s; j++) {putchar('\t');}
                    puts("Egg");
                    usleep(20000);
                }
                sleep(1);
                who_win = who_round;
            }
            else
            {
                tic[y][x] = who_round;
                x *= 2;
                y *= 2;
                player_ui[y][x] = player_round[who_round];
            }
        }

        //橫縱
        for(uint8_t h = 0; h < 2; h++)
        {
            for(uint8_t i = 0; i < 3; i++)
            {
                uint8_t s = 0;
                for(uint8_t j = 0; j < 3; j++)
                {
                    if(h) {if(tic[i][j] == who_round) {s++;}}
                    else {if(tic[j][i] == who_round) {s++;}}
                }
                if(s == 3) {who_win = who_round;}
            }
        }

        //斜線
        for(uint8_t h = 0; h < 2; h++)
        {
            uint8_t s = 0;
            uint8_t sx = 2;
            for(uint8_t i = 0; i < 3; i++)
            {
                if(h) {if(tic[i][i] == who_round) {s++;}}
                else {if(tic[i][sx] == who_round) {s++;}}
                sx--;
            }
            if(s == 3) {who_win = who_round;}
        }


        //回合結束
        who_round ^= 1;
        round++;
        setbuf(stdin, NULL);
        play_click
        #ifdef __linux__
        free(input);
        #endif
    }

    clear;
        usleep(180000);

    //勝利結算
    if(who_win == NOBODY)
    {
        printf("Oh no...\n\nyou two are tied.\n\n");
        play_tied
        wait_some_time(5);
    }
    else if(mod == TWO_PEOPLE_MODE || mod == AI_VS_AI_MODE)
    {
        printf("Congratulations!\n\n%c win!!!\n\n", player_round[who_win]);
        play_win
        wait_some_time(5);
    }
    else if(mod == AI_MODE)
    {
        if(who_win == who_player)
        {
            printf("Congratulations!\n\nyou win!!!\n\n");
            play_win
        }
        else
        {
            printf("oh no...\n\nyou lose.by %s\n\n", ai_mode_text);
            play_lose
        }
        wait_some_time(5);
    }

    #ifdef _WIN32
    free(input);
    #endif
    if(mod == 1) {free(ai_input);}
    setbuf(stdin, NULL);

    return;
}
