#ifndef _MAP_H_
#define _MAP_H_

#include <allegro.h>
#include "cel.h"

#define DISPLAY_SOL 0x01
#define DISPLAY_AMP 0x02
#define DISPLAY_MON 0x04
#define DISPLAY_ARCH 0x08
#define DISPLAY_WALL 0x10

typedef struct {
   int nb;
   unsigned short int *data;
} AMP;

typedef struct {
   int nb;
   unsigned short int *data;
} MIN;

typedef struct {
   int nb;
   unsigned char *data;
} SOL;

typedef struct {
   int nb;
   unsigned short int (*data)[4];
} TIL;

typedef struct {
   int act;
   int min_size;
   CEL_FILE *cel;
   ANIM *scel;
   AMP *amp;
   MIN *min;
   SOL *sol;
   TIL *til;
   char *pal_default;
   char *path;
} MAP;

extern MAP glb_map;

AMP *read_amp(char *);
MIN *read_min(char *, int);
SOL *read_sol(char *);
TIL *read_til(char *);
int destroy_amp(AMP *);
int destroy_min(MIN *);
int destroy_sol(SOL *);
int destroy_til(TIL *);
int init_map(int);
int free_map();
int put_cel(BITMAP *, int, int, int, int, int);
int put_min(BITMAP *, int, int, int, unsigned short int, int, int);
int put_til(BITMAP *, int, int, int, int, unsigned short int [4], int);
int put_sol(BITMAP *, int, int, int);
int put_amp(BITMAP *, int, int, int);
BITMAP *export_min(int, int);
BITMAP *export_til(int, int);
int get_til_min(char *, int, int);

#endif /* _MAP_H_ */
