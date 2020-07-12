#ifndef _MONSTER_H_
#define _MONSTER_H_

#include <allegro.h>
#include "cel.h"

typedef struct {
   unsigned char data[0x80];
   long int width;
   char file[256];
   char trn_name[256];
   char name[256];
   MULTI_ANIM *cel;
   unsigned char *trn;
} MONSTER;

typedef struct {
   MONSTER monster_table[0x100];
   int nb_monster;
   long int monster_index[0x100];
   unsigned long int monster_table_offset;
   unsigned long int monster_index_offset;
   int nb_monster_index;
   int monster_index_size;
} MONSTER_S;

extern MONSTER_S glb_monster;

int read_monster();
int read_monster_index();
void free_monster();
int put_monster_by_index(BITMAP *, int, int, int, int, int);
BITMAP *export_monster(int);

#endif /* _MONSTER_H_ */
