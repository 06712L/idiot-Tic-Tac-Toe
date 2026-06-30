#include "coco.h"
#include "game.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <string.h>

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


#define RUN_distance 3
#define RUN_stamina 2
#define WALK_distance 1
#define WALK_stamina 1
#define REST_stamina 3
#define EGG_KING_run 2
#define EGG_KING_walk 1
#define EGG_KING_sprint 5
#define BLOCK_HP 2

static void what_chapter_two_print_UI(uint8_t stamina, uint8_t ammo, uint8_t block_distance, uint8_t egg_king_distance, char map[], uint8_t distance_exit, uint8_t block_hp) //輸出UI
{
    //體力條
    printf("stamina[");
    for(uint8_t i = 0; i < 10; i++)
    {
        if(i < stamina) {putchar('#');}
        else {putchar('-');}
    }
    printf("] %d0%%\n", stamina);

    //剩餘雞蛋
    printf("Eggs[");
    for(uint8_t i = 0; i < 7; i++)
    {
        if(i < ammo) {putchar('O');}
        else {putchar(' ');}
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
    if(block_distance > RUN_distance && (stamina - RUN_stamina) >= 0) {printf("[1]Run(-%d0 stamina)\n", (RUN_stamina));}
    else {puts("X");}

    //走
    if(block_distance > WALK_distance && stamina > 4) {printf("[2]Walk(+%d0 stamina)\n", (WALK_stamina));}
    else if(block_distance > WALK_distance && stamina > 0) {puts("[2]Walk");}
    else {puts("X");}

    //休息
    if(stamina > 6) {printf("[3]take a rest(+%d0 stamina)\n",(REST_stamina));}
    else {printf("[3]take a rest(+%d0 stamina)\n",(REST_stamina - 2));}

    //丟雞蛋!
    if(ammo > 0) {puts("[4]Throw an egg(-1 Egg, That egg needs to be cleaned!)");}
    else {puts("X");}

    //踢開障礙
    if(block_distance <= 1) {printf("[5]Kick Obstacle(you need kicks %d)\n", (block_hp + 1));}
    else {puts("X");}

    //回頭看一眼
    if(egg_king_distance > 5)
    {
        puts("[6]take a look back");
    }
    else {puts("X");}
}

void look_bake(int egg_king_distance)
{
    clear;
    printf("Egg King is about %d meters away\n", (egg_king_distance + rands(3, (-2))));
    wait_some_time(3);
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
    int16_t egg_king_distance = rands(9, 5); //Egg king初始距離玩家
    int8_t stamina = 10; //體力
    uint8_t ammo = 7; //雞蛋初始7顆
    int16_t distance_exit = rands(155, 145); //初始距離
    int8_t block_distance = rands(35, 15);
    int8_t block_hp = BLOCK_HP;
    int8_t throw_egg = 0;

    char input;

    clear;
    puts("You wake up in a somewhat dark corridor.");
    sleep(1);
    puts("You see a window");
    usleep(850000);
    puts("You go over and take a look");
    usleep(770000);
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
        setbuf(stdin, NULL);
        #define __NO_ERROR error = 0
        uint8_t error = 1;
        while(error)
        {
            setbuf(stdin, NULL);

            clear;
            what_chapter_two_print_UI(stamina, ammo, block_distance, egg_king_distance, map, distance_exit, block_hp);

            /*
            * 1 = Run -3 stamina
            * 2 = Walk +1 stamina
            * 3 = rest +3 stamina
            * 4 = throw Egg -1 ammo That egg needs to be cleaned!
            * 5 = kick block_distance block_distance = rands(best: 15, min: 6)
            * 6 = look bake
            */
            //輸入專區
            input = getchar();
            while(getchar() != '\n');

            if(input == '1' && block_distance > RUN_distance && (stamina - RUN_stamina) >= 0)
            {
                stamina -= RUN_stamina;
                distance_exit -= RUN_distance;
                egg_king_distance += RUN_distance;
                block_distance -= RUN_distance;
                __NO_ERROR;
            }

            else if(input == '2' && block_distance > WALK_distance)
            {
                if(stamina > 4) {stamina += WALK_stamina;}
                distance_exit -= WALK_distance;
                egg_king_distance += WALK_distance;
                block_distance -= WALK_distance;
                __NO_ERROR;
            }

            else if(input == '3')
            {
                if(stamina > 6) {stamina += REST_stamina;}
                else {stamina += (REST_stamina - 2);}
                __NO_ERROR;
            }

            else if(input == '4' && ammo > 0)
            {
                ammo -= 1;
                throw_egg = 2;
                __NO_ERROR;
            }

            else if(input == '5' && block_distance <= 1)
            {
                play_kick;
                if(block_hp <= 0)
                {
                    block_distance = rands(15, 6);
                    block_hp = BLOCK_HP;
                }
                else {block_hp -= 1;}
                __NO_ERROR;
            }

            else if(input == '6' && egg_king_distance > 5)
            {
                look_bake(egg_king_distance);
                __NO_ERROR;
            }
        }
        #undef __NO_ERROR

        if(stamina > 10) {stamina = 10;}
        if(block_distance >= distance_exit) {rands((block_distance - distance_exit - 1), 5);}

        if(throw_egg == 0)
        {
            uint8_t egg_king_move = rands(100, 0);
            if(egg_king_move < 1) {egg_king_distance -= EGG_KING_sprint;} //1% 衝刺!
            else if(egg_king_move < 51) {egg_king_distance -= EGG_KING_walk;} //50% 走路
            else if(egg_king_move < 80) {egg_king_distance -= EGG_KING_run;} //29% 跑過來
            //20% 走不動了!
        }
        else
        {
            throw_egg -= 1;
            if(throw_egg < 0) {throw_egg = 0;}
        }
        play_click;
    }

    if(distance_exit <= 0)
    {
        clear;
        play_kick;
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
        for (uint8_t i = 0; i < 12; i++)
        {
            for (uint8_t j = 0; j < 33; j++)
            {
                printf("Egg ");
                fflush(stdout);
                usleep(10000);
            }
            putchar('\n');
        }
    }
    return;
}
#undef RUN_distance
#undef RUN_stamina
#undef WALK_distance
#undef WALK_stamina
#undef REST_stamina
#undef EGG_KING_run
#undef EGG_KING_walk
#undef EGG_KING_sprint
#undef BLOCK_HP
