#ifndef COCO_H
#define COCO_H

#include "game.h"
#include <stdio.h>
#include <stdint.h>

#define TRUE 1
#define FALSE 0

#ifdef _WIN32
#define clear system("cls")
#define play_click if(sound) {PlaySound(TEXT(".\\sound\\click.wav"), NULL, SND_FILENAME | SND_ASYNC);}
#define play_tied if(sound) {PlaySound(TEXT(".\\sound\\tied.wav"), NULL, SND_FILENAME | SND_ASYNC);}
#define play_win if(sound) {PlaySound(TEXT(".\\sound\\winner.wav"), NULL, SND_FILENAME | SND_ASYNC);}
#define play_lose if(sound) {PlaySound(TEXT(".\\sound\\lose.wav"), NULL, SND_FILENAME | SND_ASYNC);}
#define play_kick_door if(sound) {PlaySound(TEXT(".\\sound\\kick-door.wav"), NULL, SND_FILENAME | SND_ASYNC);}

#else
#define clear printf("\033[2J\033[H")
#define play_click if(sound) {system("aplay ./sound/click.wav > /dev/null 2>&1 &");}
#define play_tied if(sound) {system("aplay ./sound/tied.wav > /dev/null 2>&1 &");}
#define play_win if(sound) {system("aplay ./sound/winner.wav > /dev/null 2>&1 &");}
#define play_lose if(sound) {system("aplay ./sound/lose.wav > /dev/null 2>&1 &");}
#define play_kick_door if(sound) {system("aplay ./sound/kick-door.wav > /dev/null 2>&1 &");}
#endif

int rands(int32_t max, int32_t min);
int check_file(FILE **file);
void wait_some_time(int time);

extern const char *version;
extern int32_t sound;
extern int32_t ai_mode;
extern int32_t discovered_what;

#endif
