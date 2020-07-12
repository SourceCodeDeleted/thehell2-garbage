#ifndef _DUN_H_
#define _DUN_H_

#include <allegro.h>

typedef struct {
   int w, h, nb_layer;
   unsigned char *data;
   unsigned short int *tiles;
   unsigned short int *(layer[16]);
} DUN;

typedef struct {
   char *filename;
   int x;
   int y;
   int dir;
} NPC_data;

DUN *read_dun(char *);
int destroy_dun(DUN *);
BITMAP *aff_dun(DUN *, int, int);
int blit_npc(BITMAP *, NPC_data *, int, int);
DUN *get_town_dun(int);
int get_town_map(int);
int export_dun_files(char *[], int, int);
int put_dun(BITMAP *, DUN *, int, int, int, int);
int put_dun_with_select(BITMAP *, DUN *, int, int, int, int, int, int, int, int);
int put_dun_layer(BITMAP *, DUN *, int, int, int, int);
DUN *create_dun(int, int, int);
int copy_dun(DUN *, int, int, int, int, DUN *, int, int);

#endif /*_DUN_H_ */
