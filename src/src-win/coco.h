#ifndef COCO_H
#define COCO_H

#define clear system("cls")
#define play_click PlaySound(TEXT(".\\sound\\click.wav"), NULL, SND_FILENAME | SND_ASYNC);
#define play_tied PlaySound(TEXT(".\\sound\\tied.wav"), NULL, SND_FILENAME | SND_ASYNC);
#define play_win PlaySound(TEXT(".\\sound\\win.wav"), NULL, SND_FILENAME | SND_ASYNC);

extern char *version;

#endif
