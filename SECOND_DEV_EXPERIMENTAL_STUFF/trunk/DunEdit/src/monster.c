#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "monster.h"
#include "mpq_lib.h"
#include "global_allegro.h"

MONSTER_S glb_monster;

int read_monster() {
   FILE *game;
   char *str;
   int i, c;

   game = fopen(config.game_exe, "rb");
   if (game == NULL) {
      fprintf(stderr, "Cant open %s\n", config.game_exe);
      glb_monster.nb_monster = 0;
      return 1;
   }
   fseek(game, glb_monster.monster_table_offset, SEEK_SET);
   for (i = 0; i < glb_monster.nb_monster; ++i)
      fread(glb_monster.monster_table[i].data, 128, 1, game);
   for (i = 0; i < glb_monster.nb_monster; ++i) {
      glb_monster.monster_table[i].width = ((unsigned long int *)(glb_monster.monster_table[i].data))[0];
      str = glb_monster.monster_table[i].name;
      fseek(game, ((long int *)glb_monster.monster_table[i].data)[20] - config.code_offset, SEEK_SET);
      while ((c = fgetc(game)) != 0) {
         *(str++) = c;
      }
      *str = 0;
      str = glb_monster.monster_table[i].file;
      fseek(game, ((long int *)glb_monster.monster_table[i].data)[2] - config.code_offset, SEEK_SET);
      while ((c = fgetc(game)) != 0) {
         *(str++) = c;
      }
      *str = 0;
      if (((long int *)glb_monster.monster_table[i].data)[6]) {
         str = glb_monster.monster_table[i].trn_name;
         fseek(game, ((long int *)glb_monster.monster_table[i].data)[7] - config.code_offset, SEEK_SET);
         while ((c = fgetc(game)) != 0) {
            *(str++) = c;
         }
         *str = 0;
      }
   }
   fclose(game);
   return 0;
}

int read_monster_index() {
   FILE *game;
   int i;

   game = fopen(config.game_exe, "rb");
   if (game == NULL) {
      fprintf(stderr, "Cant open %s\n", config.game_exe);
      glb_monster.nb_monster_index = 0;
      return 1;
   }
   fseek(game, glb_monster.monster_index_offset, SEEK_SET);
   for (i = 0; i < glb_monster.nb_monster_index; ++i) {
      fread(glb_monster.monster_index + i, glb_monster.monster_index_size, 1, game);
      if (glb_monster.monster_index[i] >= glb_monster.nb_monster)
         glb_monster.monster_index[i] = -1;
   }
   fclose(game);
   return 0;
}

void free_monster() {
   int i;
   for (i = 0; i < glb_monster.nb_monster; ++i) {
      if (glb_monster.monster_table[i].cel != NULL)
         destroy_multi_anim(glb_monster.monster_table[i].cel);
      if (glb_monster.monster_table[i].trn != NULL)
         free(glb_monster.monster_table[i].trn);
   }
}

int put_monster_by_index(BITMAP *bmp, int idx, int x, int y, int selected, int centered) {
   long int mon_num, size;
   char tmp[300];

   if (idx <= 0)
      return 0;
   if (idx > glb_monster.nb_monster_index)
      return 0;
   mon_num = glb_monster.monster_index[idx - 1];
   if ((mon_num < 0) || (mon_num >= glb_monster.nb_monster))
      return 1;
   if (glb_monster.monster_table[mon_num].cel == NULL) {
      sprintf(tmp, glb_monster.monster_table[mon_num].file, 'n');
      glb_monster.monster_table[mon_num].cel = load_multi_anim(tmp, 1);
   }
   if (glb_monster.monster_table[mon_num].cel == NULL) {
      fprintf(stdout, "no cel for monster\n");
      return 1;
   }
   if (glb_monster.monster_table[mon_num].cel->frame[0][0].spr == NULL) {
      if (glb_monster.monster_table[mon_num].trn == NULL) {
         if (strlen(glb_monster.monster_table[mon_num].trn_name) != 0) {
            misc_load_mpq_file(glb_monster.monster_table[mon_num].trn_name, &(glb_monster.monster_table[mon_num].trn), &size, config.debug);
            if (size == 0x100) {
               for (size = 0; size < 0x100; ++size)
                  if (glb_monster.monster_table[mon_num].trn[size] == 0xFF)
                     glb_monster.monster_table[mon_num].trn[size] = 0;
            }
            else {
               free(glb_monster.monster_table[mon_num].trn);
               glb_monster.monster_table[mon_num].trn = NULL;
            }
         }
      }
      if (glb_monster.monster_table[mon_num].cel->cl2) {
         decode_header_cl2(&(glb_monster.monster_table[mon_num].cel->frame[0][0]), glb_monster.monster_table[mon_num].trn);
      }
      else {
         decode_header_cel(&(glb_monster.monster_table[mon_num].cel->frame[0][0]), glb_monster.monster_table[mon_num].trn);
      }
   }
   if (centered)
      x += 32 - glb_monster.monster_table[mon_num].width / 2;
   return put_image_multi_anim(bmp, glb_monster.monster_table[mon_num].cel, 0, 0, x, y, selected);
}

BITMAP *export_monster(int nb_per_line) {
   int i, dx, dy, x, y, mon_num;
   BITMAP *res = NULL;

   dx = 224;
   dy = 192;
   if (glb_monster.nb_monster_index < nb_per_line) {
      x = 32 + dx * glb_monster.nb_monster_index;
      y = 32 + dy;
   }
   else {
      x = nb_per_line * dx + 32;
      y = 32 + dy * (glb_monster.nb_monster_index / nb_per_line + 1);
   }
   res = create_bitmap(x, y);
   if (res == NULL) {
      fprintf(stderr, "%d*%d bitmap not created for monsters\n", x, y);
      return res;
   }
   clear(res);
   for (i = 0; i < glb_monster.nb_monster_index; ++i) {
      put_monster_by_index(res, i + 1, 32 + (i % nb_per_line) * dx, (1 + i / nb_per_line) * dy, 0, 0);
      if ((i >= 0) && (i < glb_monster.nb_monster_index)) {
         mon_num = glb_monster.monster_index[i];
            if ((mon_num >= 0) && (mon_num < glb_monster.nb_monster))
               textout(res, font, glb_monster.monster_table[mon_num].name, 32 + (i % nb_per_line) * dx, (1 + i / nb_per_line) * dy, 255);
      }
   }
   return res;
}
