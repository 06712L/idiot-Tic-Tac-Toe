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

enum
{
    O = 0,
    X = 1,
    NOBODY = 2
};

//by Qwen AI
//0 = O Win, 1 = X Win, 2 = NOBODY, 3 = tied
static uint8_t check_winner(uint8_t board[3][3]) {
    // 橫縱
    for(uint8_t h = 0; h < 2; h++) {
        for(uint8_t i = 0; i < 3; i++) {
            uint8_t s_o = 0, s_x = 0;
            for(uint8_t j = 0; j < 3; j++) {
                uint8_t val = h ? board[i][j] : board[j][i];
                if(val == 0) s_o++;
                else if(val == 1) s_x++;
            }
            if(s_o == 3) return 0;
            if(s_x == 3) return 1;
        }
    }

    // 斜線
    for(uint8_t h = 0; h < 2; h++) {
        uint8_t s_o = 0, s_x = 0;
        uint8_t sx = 2;
        for(uint8_t i = 0; i < 3; i++) {
            uint8_t val = h ? board[i][i] : board[i][sx];
            if(val == 0) s_o++;
            else if(val == 1) s_x++;
            sx--;
        }
        if(s_o == 3) return 0;
        if(s_x == 3) return 1;
    }

    // 檢查是否還有空格 (還沒結束)
    for(uint8_t i = 0; i < 3; i++) {
        for(uint8_t j = 0; j < 3; j++) {
            if(board[i][j] == 2) return 2;
        }
    }

    return 3;
}

//by Qwen AI
// is_maximizing: true = AI round, false = Opponent
static int minimax(uint8_t board[3][3], bool is_maximizing, uint8_t ai_player, uint8_t human_player) {
    uint8_t winner = check_winner(board);

    if (winner == ai_player) return 10;      // AI Win
    if (winner == human_player) return -10;  // Opponent Win
    if (winner == 3) return 0;               // tied

    if (is_maximizing) {
        int best_score = -1000;
        for (uint8_t y = 0; y < 3; y++) {
            for (uint8_t x = 0; x < 3; x++) {
                if (board[y][x] == 2) {
                    board[y][x] = ai_player;
                    int score = minimax(board, false, ai_player, human_player);
                    board[y][x] = 2;

                    if (score > best_score) best_score = score;
                }
            }
        }
        return best_score;
    } else {
        int best_score = 1000;
        for (uint8_t y = 0; y < 3; y++) {
            for (uint8_t x = 0; x < 3; x++) {
                if (board[y][x] == 2) {
                    board[y][x] = human_player;
                    int score = minimax(board, true, ai_player, human_player);
                    board[y][x] = 2;

                    if (score < best_score) best_score = score;
                }
            }
        }
        return best_score;
    }
}

void tic_tac_toe_game(int mod)
{

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

            if(x >= 0 && y >= 0 && x < 3 && y < 3 && tic[y][x] == NOBODY && error == FALSE)
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
                if(mod != AI_VS_AI_MODE) {free(input);}
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

            //by Qwen AI
            else if(ai_mode == 3)
            {
                int best_score = -1000;
                int best_x = -1, best_y = -1;

                for (uint8_t y = 0; y < 3; y++) {
                    for (uint8_t x = 0; x < 3; x++) {
                        if (tic[y][x] == NOBODY) {
                            tic[y][x] = who_round;

                            int score = minimax(tic, false, who_round, who_player);

                            tic[y][x] = NOBODY;

                            if (score > best_score) {
                                best_score = score;
                                best_x = x;
                                best_y = y;
                            }
                        }
                    }
                }

                sprintf(ai_input, "%d%d", best_x + 1, best_y + 1);
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
        if(mod != AI_VS_AI_MODE) {free(input);}
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
    if(mod >= 1) {free(ai_input);}
    setbuf(stdin, NULL);

    return;
}
