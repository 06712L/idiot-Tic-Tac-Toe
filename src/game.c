#include "game.h"
#include "coco.h"
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#ifdef _WIN32
#include <windows.h>
#include <mmsystem.h>
#endif
#ifndef _DEFAULT_SOURCE
#define _DEFAULT_SOURCE
#endif

void tic_tac_toe_game(int mod)
{
    #define O 0
    #define X 1
    #define NOBODY 2
    #define TWO_PEOPLE_MODE 0
    #define AI_MODE 1

    #ifdef _WIN32
    char *input = malloc(20 * sizeof(char));
    #endif
    char *ai_input = NULL;
    char *ai_mode_text;

    uint8_t who_player = NOBODY;
    if(mod == AI_MODE)
    {
        ai_input = calloc(4, sizeof(char));

        /*
        * ai mode  number
        *  idiot  =  0
        *  ordinary= 1
        *  expert =  2
        */
        switch (ai_mode)
        {
            case 0:
                ai_mode_text = "Mr.HotDog";
                break;
            case 1:
                ai_mode_text = "Mr.Dog";
                break;
            case 2:
                ai_mode_text = "Mr.Egg";
                break;
        }

        who_player = rands(100, 0);

        if(who_player < 50) {who_player = O;}
        else {who_player = X;}
    }

    uint8_t who_round = X;
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
        setbuf(stdin, NULL);
        #ifdef _WIN32
        int siz_text = 20;
        #else 
        char *input = NULL;
        size_t siz_text = 0;
        #endif
        clear;
        printf("round %d\nTurn:%c\n", (round + 1), player_round[who_round]);
        if(mod == 1) {printf("you're: %c\n\n", player_round[who_player]);}
        else {putchar('\n');}

        for(int i = 0; i < 5; i++)
        {
            for(int j = 0; j < 5; j++)
            {
                printf("%c", player_ui[i][j]);
            }
            putchar('\n');
        }
        if(who_round == who_player || mod == TWO_PEOPLE_MODE)
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
        else if(mod == AI_MODE)
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
            int ai_check = 0;
            printf("\ninput:");
            fflush(stdout);
            sleep(rands(3, 1));
            //Zheng-De-AI Pro


            //ai will Egg!
            if(want_egg >= 95 && ai_mode >= 2)
            {
                strcpy(ai_input, "Egg");
                ai_input[strlen(ai_input)] = '\0';
                ai_check = 1;
                which_mod = 5;
            }

            //先手佔領中心
            else if(tic[1][1] == NOBODY && round == 0 && ai_check == 0 && ai_mode >= 0)
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
        who_round = !who_round;
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
    else if(mod == TWO_PEOPLE_MODE)
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

    #undef O 
    #undef X
    #undef NOBODY
    #undef TWO_PEOPLE_MODE
    #undef AI_MODE

    return;
}


//Inspired by Gravity Falls
void what()
{
    uint8_t rooms[20][20];
    char input[30];
    for(uint8_t i = 0; i < 20; i++) {for(uint8_t j = 0; j < 20; j++){rooms[i][j] = 0;}}

    //X,Y
    int where_player[2];
    for(uint8_t i = 0; i < 2; i++) {where_player[i] = rands(19, 0);}
    int where_exit[2];
    for(uint8_t i = 0; i < 2; i++) {where_exit[i] = rands(19, 0);}
    while(where_player[0] == where_exit[0] || where_player[1] == where_exit[1] || rooms[where_player[1]][where_player[0]] == 1) {for(int i = 0; i < 2; i++){where_player[i] = rands(19, 0);}}
    rooms[where_exit[1]][where_exit[0]] = 1;

    uint8_t walk = 1;

    /*
     * 0 = empty room
     * 1 = exit
     * 2 = X
    */

    uint16_t time = rands(275, 145);

    clear;
    puts("you open a door");
    sleep(1);
    puts("A room is inside the door");
    usleep(950000);
    puts("you walk into the room");
    usleep(1250000);
    puts("Suddenly the door behind you closed");
    usleep(700000);
    puts("There are four doors in the room");
    sleep(1);
    printf("\nYou must escape from here before it's too late\n");
    sleep(3);

    while(rooms[where_player[1]][where_player[0]] == 0 && time > 0)
    {
        re_what:
        clear;
        setbuf(stdin, NULL);
        uint8_t your_ahead = 0;
        uint8_t your_rear = 0;
        uint8_t your_left = 0;
        uint8_t your_right = 0;

        //前
        if((where_player[1] - 1) >= 0 && rooms[(where_player[1] - 1)][where_player[0]] != 2)
        {
            your_ahead = 1;
            printf("\t\t[1]The door leading ahead\n");
        }
        else if((where_player[1] - 1) >= 0 && rooms[(where_player[1] - 1)][where_player[0]] == 2) {printf("\t\tX\n");}
        else {putchar('\n');}
        //左
        if((where_player [0] - 1) >= 0  && rooms[where_player[1]][(where_player[0] - 1)] != 2)
        {
            your_left = 1;
            printf("[2]Go to the door on the left");
        }
        else if((where_player [0] - 1) >= 0  && rooms[where_player[1]][(where_player[0] - 1)] == 2) {putchar('X');}

        double s = hypot(abs(where_exit[0] - where_player[0]), abs(where_exit[1] - where_player[1]));
        if(s < (sqrt(2) + 0.01)) {printf("\tI sensed something nearby\t");}
        else {printf("\t\t\t");}

        //右
        if((where_player[0] + 1) < 20 && rooms[where_player[1]][(where_player[0] + 1)] != 2)
        {
            your_right = 1;
            printf("[3]Go to the door on the right\n");
        }
        else if((where_player[0] + 1) < 20 && rooms[where_player[1]][(where_player[0] + 1)] == 2) {puts("X");}
        else {putchar('\n');}
        //後
        if((where_player[1] + 1) < 20 && rooms[(where_player[1] + 1)][where_player[0]] != 2)
        {
            your_rear = 1;
            printf("\t\t[4]To the back door\n");
        }
        else if((where_player[1] + 1) < 20 && rooms[(where_player[1] + 1)][where_player[0]] == 2) {printf("\t\t\tX\n");}
        else {putchar('\n');}
        fflush(stdout);
        /*
         *       ahead
         *   left  ^  right
         *       rear
        */

        int error = 0;
        fgets(input, 30, stdin);
        input[strcspn(input, "\n")] = '\0';
        if(strlen(input) != 1) {error = 1;}
        if(atoi(input) < 1 || atoi(input) > 4) {error = 1;}

        int splayer[2];
        for(int i = 0; i < 2; i++) {splayer[i] = where_player[i];}

        if(input[0] == '1' && your_ahead == 1) {where_player[1] -= 1;}
        if(input[0] == '2' && your_left == 1) {where_player[0] -= 1;}
        if(input[0] == '3' && your_right == 1) {where_player[0] += 1;}
        if(input[0] == '4' && your_rear == 1) {where_player[1] += 1;}
        if(splayer[0] == where_player[0] && splayer[1] == where_player[1]) {error = 1;}

        if(error == 1) {goto re_what;}

        int something_i_can_turn_to = 0;
        while(something_i_can_turn_to == 0)
        {
            int x = rands(19, 0);
            int y = rands(19, 0);

            if(rooms[y][x] == 0 && x != where_player[0] && y != where_player[1] && (walk % 3) == 0)
            {
                rooms[y][x] = 2;
                something_i_can_turn_to = 1;
            }
            else
            {
                something_i_can_turn_to = 1;
            }
            walk++;
        }
        time--;
        play_click
        clear;
        puts("you are moveing...");
        usleep(350000);
    }
    if(rooms[where_player[1]][where_player[0]] == 1)
    {
        clear;
        play_win
        puts("You found a strange door");
        sleep(1);
        puts("You open that door");
        usleep(1500000);
        puts("Wait, why is there an egg inside?");
        usleep(800000);
        puts("My God! That egg knocked you out!");
        sleep(2);
        puts("To be continued");
        sleep(5);
        clear;
        printf("Overview\n\n%d second remaining\nYou took %d steps\n\n", time, walk);

        discovered_what = 1;
        wait_some_time(10);
    }
    else
    {
        uint8_t s = rands(670, 67);
        clear;
        play_lose
        printf("oh,oh...\n\n");
        sleep(1);
        puts("Some noise has started...");
        sleep(rands(5, 3));
        for(uint8_t i = 0; i < s; i++)
        {
            uint8_t ss = rands(5, 0);
            for(int j = 0; j < ss; j++) {putchar('\t');}
            puts("Egg");
            usleep(10000);
        }
        sleep(1);
        printf("Overview\n\n%d second remaining\nYou took %d steps\n\n", time, walk);
        wait_some_time(10);
    }
    setbuf(stdin, NULL);
    return;
}



static void what_chapter_two_print_UI(uint8_t stamina, uint8_t ammo, uint8_t block_distance, uint8_t egg_king_distance, char map[], uint8_t distance_exit) //輸出UI
{
    //體力條
    printf("stamina[");
    for(uint8_t i = 0; i < 10; i++)
    {
        if(i < stamina) {putchar('#');}
        else {putchar('-');}
    }
    printf("] %d%%\n", (stamina * 10));

    //剩餘雞蛋
    printf("Eggs[");
    for(uint8_t i = 0; i < 15; i++)
    {
        if(i < ammo) {putchar('O');}
        else {putchar('X');}
    }
    printf("] %d\n\n\t\t\t-\n", ammo);

    //玩家可視角
    for(uint8_t i = 0; i < 11; i++)
    {
        char sc;
        if(block_distance < 6 && i < 5 && (5 - block_distance) == i) {sc = 'X';}
        else if(distance_exit < 6 && i < 5 && (5 - distance_exit) == i) {sc = '#';}
        else if(egg_king_distance < 6 && i > 5 && (5 + egg_king_distance) == i) {sc = 'O';}
        else {sc = map[i];}
        printf("\t\t\t%c\n", sc);
    }
    printf("\t\t\t-\n\n");

    //跑!
    if(block_distance > 2 && (stamina - 2) >= 0) {puts("[1]Run(-20 stamina)");}
    else {puts("X");}

    //走
    if(block_distance > 1) {puts("[2]Walk(+10 stamina)");}
    else {puts("X");}

    //休息
    puts("[3]take a rest(+30 stamina)");

    //丟雞蛋!
    if(ammo > 0) {puts("[4]Throw an egg(-1 Egg, Knockback 5 tiles, +10 stamina)");}
    else {puts("X");}

    //踢開障礙
    if(block_distance <= 1) {puts("[5]Kick Obstacle");}
    else {puts("X");}

    //回頭看一眼
    if(egg_king_distance > 6)
    {
        puts("[6]take a look back");
    }
}

//Inspired by Baldi's Basic
void what_chapter_two()
{
    char map[11]; //{' ', ' ', ' ', ' ', ' ', '^', ' ', ' ', ' ', ' ', ' '};
    for(uint8_t i = 0; i < 11; i++)
    {
        if(i == 5) {map[i] = '^';}
        else {map[i] = ' ';}
    }
    uint8_t egg_king_distance = rands(10, 4); //Egg king初始距離玩家
    uint8_t stamina = 10; //體力
    uint8_t ammo = 15; //雞蛋初始10顆
    uint8_t distance_exit = rands(125, 100); //初始距離
    uint8_t block_distance = rands(45, 30);

    char input;

    clear;
    puts("You wake up in a somewhat dark corridor.");
    sleep(1);
    puts("You see a window");
    usleep(850000);
    puts("You go over and take a look");
    usleep(670000);
    puts("You saw all sorts of eggs as tall as a person on the street.");
    sleep(2);
    puts("Some sounds appeared...");
    usleep(1500000);
    printf("An egg wearing a crown has found you.\n\n");
    sleep(1);
    puts("You have to run fast!");
    sleep(5);


    while(egg_king_distance > 0 && distance_exit > 0)
    {
        #define NO_ERROR error = 0
        uint8_t look_bake = 0;
        uint8_t error = 1;
        while(error)
        {
            setbuf(stdin, NULL);

            clear;
            what_chapter_two_print_UI(stamina, ammo, block_distance, egg_king_distance, map, distance_exit);

            /*
            * 1 = Run -2 stamina
            * 2 = Walk +1 stamina
            * 3 = rest +3 stamina
            * 4 = throw Egg -1 ammo +4 egg_king_distance +1 stamina
            * 5 = kick block_distance block_distance = rands(max: 30, min: 15)
            * 6 = look bake
            */
            //輸入專區
            input = getchar();
            while(getchar() != '\n');

            if(input == '1' && block_distance > 2 && (stamina - 2) >= 0)
            {
                stamina -= 2;
                distance_exit -= 2;
                egg_king_distance += 2;
                block_distance -= 2;
                NO_ERROR;
            }

            else if(input == '2' && block_distance > 1)
            {
                stamina += 1;
                distance_exit -= 1;
                egg_king_distance += 1;
                block_distance -= 1;
                NO_ERROR;
            }

            else if(input == '3')
            {
                stamina += 3;
                NO_ERROR;
            }

            else if(input == '4' && ammo > 0)
            {
                ammo -= 1;
                egg_king_distance += 6;
                stamina += 1;
                NO_ERROR;
            }

            else if(input == '5' && block_distance <= 1)
            {
                play_kick_door;
                block_distance = rands(17, 5);
                NO_ERROR;
            }

            else if(input == '6' && egg_king_distance > 6)
            {
                look_bake = 1;
                NO_ERROR;
            }
        }
        #undef NO_ERROR

        if(look_bake)
        {
            clear;
            printf("Egg King is about %d meters away\n", (egg_king_distance + rands(10, (-5))));
            wait_some_time(3);
        }

        if(stamina > 10) {stamina = 10;}

        uint8_t egg_king_move = rands(100, 0);
        if(egg_king_move > 95) {egg_king_distance -= 6;} //5% 衝刺!
        else if(egg_king_move < 10 && egg_king_move > 3) {egg_king_distance -= 2;} //6% 走路
        else if(egg_king_distance > 25 && egg_king_distance < 95) {egg_king_distance -= 4;} //86% 跑過來
        //3% 走不動了!
        play_click;
    }

    if(distance_exit <= 0)
    {
        clear;
        play_kick_door
        puts("You kicked open the exit door.");
        usleep(2900000);
        puts("What the heck? There's a dimensional portal behind the door!");
        sleep(1);
        puts("Egg king: Get back here!");
        sleep(2);
        puts("Egg king: I'm going to feed you to the Chicken God!");
        sleep(3);
        puts("Egg king: We'll meet again!");
        sleep(1);
    }
    else if(egg_king_distance <= 0)
    {
        clear;
        for (uint8_t i = 0; i < 255; i++)
        {
            for (uint8_t j = 0; j < 255; j++)
            {
                printf("Egg ");
                fflush(stdout);
                usleep(100000);
            }
            putchar('\n');
        }
    }
    return;
}