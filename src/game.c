#include "game.h"
#include "coco.h"
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#ifdef _WIN32
#include <windows.h>
#include <mmsystem.h>
#endif
#ifndef _DEFAULT_SOURCE
#define _DEFAULT_SOURCE
#endif

static void wait_some_time(int time)
{
    for(int i = time; i >= 0; i--)
    {
        printf("waiting %d second to leif\r", i);
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
    int who_player = 2;
    if(mod == 1)
    {
        ai_input = calloc(4, sizeof(char));
        who_player = rands(100, 0);

        if(who_player < 50) {who_player = 0;}
        else {who_player = 1;}
    }

    int who_round = 1;
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
        else {putchar('\n');}

        for(int i = 0; i < 5; i++)
        {
            for(int j = 0; j < 5; j++)
            {
                printf("%c", player_ui[i][j]);
            }
            putchar('\n');
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
                    for(int j = 0; j < s; j++) {putchar('\t');}
                    puts("Egg");
                    usleep(20000);
                }
                sleep(1);
                who_win = who_round;
            }
            else if(error == 1) {goto re;}
        }
/*      AI
        |
        v
*/
        else if(mod == 1)
        {
            int which_mod = 0; //debug
            int which_q = 0; //debug
            //mix debug
            (void)which_mod;
            (void)which_q;
            /*
            * mod    number
            * Egg  =   5
            * 先手占中=1
            * 攻   =   2
            * 守   =   3
            * 建立攻 = 6
            * 隨機 =   4
            */

            int want_egg = rands(100, 0);
            int ai_check = 0;
            printf("\ninput:");
            fflush(stdout);
            sleep(rands(3, 1));
            ai_check = 0;
            //Zheng-De-AI Pro


            //ai will Egg!
            if(want_egg >= 99)
            {
                strcpy(ai_input, "Egg");
                ai_input[strlen(ai_input)] = '\0';
                ai_check = 1;
                which_mod = 5;
            }

            //先手佔領中心
            else if(tic[1][1] == 2 && round == 0 && ai_check == 0)
            {
                strcpy(ai_input, "22");
                ai_input[strlen(ai_input)] = '\0';
                ai_check = 1;
                which_mod = 1;
            }

            //第二代攻擊方案
            if(ai_check == 0)
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
            if(ai_check == 0)
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
            if(ai_check == 0)
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
                            if(tic[i][j] == who_round && tic[i][j] != who_player) {ss++;}
                            else if(tic[i][j] == who_player) {xs++;}
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
                        else if(tic[i][sx]) {xs++;}
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
            if(ai_check == 0)
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
        if(!who_round) {who_round = 1;}
        else {who_round = 0;}
        round++;
        setbuf(stdin, NULL);
        play_click
    }

    clear;
    free(input);
    if(mod == 1) {free(ai_input);}
        usleep(180000);

    //勝利結算
    if(who_win == 2)
    {
        printf("Oh no...\n\nyou two are tied.\n\n");
        play_tied
        wait_some_time(5);
    }
    else if(mod == 0)
    {
        printf("Congratulations!\n\n%c win!!!\n\n", player_round[who_win]);
        play_win
        wait_some_time(5);
    }
    else if(mod == 1)
    {
        if(who_win == who_player)
        {
            printf("Congratulations!\n\nyou win!!!\n\n");
            play_win
        }
        else
        {
            printf("oh no...\n\nyou lose.by an idiot AI\n\n");
            play_lose
        }
        wait_some_time(5);
    }
    setbuf(stdin, NULL);
    return;
}


//Inspired by Gravity Falls
void what()
{
    int rooms[20][20];
    char input[30];
    for(int i = 0; i < 20; i++) {for(int j = 0; j < 20; j++){rooms[i][j] = 0;}}

    //X,Y
    int where_player[2];
    for(int i = 0; i < 2; i++) {where_player[i] = rands(19, 0);}
    int where_exit[2];
    for(int i = 0; i < 2; i++) {where_exit[i] = rands(19, 0);}
    while(where_player[0] == where_exit[0] || where_player[1] == where_exit[1] || rooms[where_player[1]][where_player[0]] == 1) {for(int i = 0; i < 2; i++){where_player[i] = rands(19, 0);}}
    rooms[where_exit[1]][where_exit[0]] = 1;

    int walk = 1;

    /*
     * 0 = empty room
     * 1 = exit
     * 2 = X
    */

    int time = rands(375, 150);

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
        int your_ahead = 0;
        int your_rear = 0;
        int your_left = 0;
        int your_right = 0;

        if((where_player[1] - 1) >= 0 && rooms[(where_player[1] - 1)][where_player[0]] != 2)
        {
            your_ahead = 1;
            printf("\t\t[1]The door leading ahead\n");
        }
        else if(rooms[(where_player[1] - 1)][where_player[0]] == 2) {puts("X");}
        else {putchar('\n');}
        if((where_player [0] - 1) >= 0  && rooms[where_player[1]][(where_player[0] - 1)] != 2)
        {
            your_left = 1;
            printf("[2]Go to the door on the left");
        }
        else if(rooms[where_player[1]][(where_player[0] - 1)] == 2) {putchar('X');}

        double s = hypot(abs(where_exit[0] - where_player[0]), abs(where_exit[1] - where_player[1]));
        if(s < (sqrt(2) + 0.01)) {printf("\tI sensed something nearby\t");}
        else {printf("\t\t\t");}

        if((where_player[0] + 1) < 20 && rooms[where_player[1]][(where_player[0] + 1)] != 2)
        {
            your_right = 1;
            printf("[3]Go to the door on the right\n");
        }
        else if(rooms[where_player[1]][(where_player[0] + 1)] == 2) {puts("X");}
        else {putchar('\n');}
        if((where_player[1] + 1) < 20 && rooms[(where_player[1] + 1)][where_player[0]] != 2)
        {
            your_rear = 1;
            printf("\t\t[4]To the back door\n");
        }
        else if(rooms[(where_player[1] + 1)][where_player[0]] == 2) {printf("\t\t\tX");}
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
        wait_some_time(10);
    }
    else
    {
        int s = rands(670, 67);
        clear;
        play_lose
        printf("oh,oh...\n\n");
        sleep(1);
        puts("Some noise has started...");
        sleep(rands(5, 3));
        for(int i = 0; i < s; i++)
        {
            int ss = rands(5, 0);
            for(int j = 0; j < ss; j++) {putchar('\t');}
            puts("Egg");
            usleep(10000);
        }
        sleep(1);
        printf("Overview\n\n%d second remaining\nYou took %d steps\n\n", time, walk);
        wait_some_time(10);
    }
    return;
}

/*
//Inspired by Baldi's Basic
static void what_chapter_two()
{
    char map[11] = {'#', '#', '#', '#', '#', '^', '#', '#', '#', '#', '#'};
    int egg_king_distance = rands(30, 17); //Egg king初始距離玩家
    int stamina = 10; //體力
    int ammo = 15; //雞蛋初始10顆
    int distance_exit = rands(105, 95); //初始距離
    int block = rands(30, 25);

    char *input = calloc(2, sizeof(char));

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


    while(egg_king_distance <= 0 && distance_exit > 0)
    {
        int can_hit_block = 0;
        if(block <= 1) {can_hit_block = 1;}

        printf("stamina[");
        for(int i = 0; i < 10; i++)
        {
            if(i < stamina) {putchar('#');}
            else {putchar('-');}
        }
        printf("] %d%%\n", (stamina * 10));

        printf("Eggs[");
        for(int i = 0; i < 15; i++)
        {
            if(i < ammo) {putchar('O');}
            else {putchar('X');}
        }
        printf("] %d\n\n@", ammo);

        for(int i = 0; i < 11; i++)
        {
            char sc;
            if((block - 5) == i && i < 5) {sc = 'X';}
            else if((egg_king_distance + 5) == i && i > 5) {sc = 'O';}
            else {sc = map[i];}
            printf("\t\t\t%c\n", sc);
        }
        printf("@\n\n");

        if((block - 5) > 3) {puts("[1]Run(-20 stamina)");}
        else {puts("X");}

        if((block - 5) > 4) {puts("[2]Walk(+10 stamina)");}
        else {puts("X");}

        puts("[3]take a rest(+30 stamina)");
        
        if(ammo > 0) {puts("[4]Throw an egg(-1 Egg, Knockback 3 tiles)");}
        else {puts("X");}

        if(block == 1) {puts("[5]Kick Obstacle");}
        else {puts("X");}

        //V0.3-alpha.2繼續施工
    }
}
*/