#ifndef COCO_H
#define COCO_H

#include <stdio.h>

#ifdef _WIN32
#define clear system("cls")
#define play_click PlaySound(TEXT(".\\sound\\click.wav"), NULL, SND_FILENAME | SND_ASYNC);
#define play_tied PlaySound(TEXT(".\\sound\\tied.wav"), NULL, SND_FILENAME | SND_ASYNC);
#define play_win PlaySound(TEXT(".\\sound\\winner.wav"), NULL, SND_FILENAME | SND_ASYNC);
#define play_lose PlaySound(TEXT(".\\sound\\lose.wav"), NULL, SND_FILENAME | SND_ASYNC);

#else
#define clear system("clear")
#define play_click if(sound) {system("aplay ./sound/click.wav > /dev/null 2>&1 &");}
#define play_tied if(sound) {system("aplay ./sound/tied.wav > /dev/null 2>&1 &");}
#define play_win if(sound) {system("aplay ./sound/winner.wav > /dev/null 2>&1 &");}
#define play_lose if(sound) {system("aplay ./sound/lose.wav > /dev/null 2>&1 &");}
#endif

int rands(int max, int min);
int pow_int(int x, int y);
int check_file(FILE **file);

extern char *version;
extern int sound;
extern int ai_mode;

#endif
