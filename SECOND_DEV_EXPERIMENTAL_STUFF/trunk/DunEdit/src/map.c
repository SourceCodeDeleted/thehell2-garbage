#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "global_allegro.h"
#include "map.h"
#include "monster.h"
#include "mpq_lib.h"

MAP glb_map;
BITMAP *empty_tile;

int init_map(int act) {
   const int line_size[32] = { 0, 2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30, 32, 30, 28, 26, 24, 22, 20, 18, 16, 14, 12, 10, 8, 6, 4, 2 };
   int i, j;
   char *pal_default[] = {
      "levels\\towndata\\town.pal",
      "levels\\l1data\\l1_1.pal",
      "levels\\l2data\\l2_1.pal",
      "levels\\l3data\\l3_1.pal",
      "levels\\l4data\\l4_1.pal",
      "nlevels\\l5data\\l5base.pal",
      "nlevels\\l6data\\l6base.pal",
      "levels\\towndata\\town.pal"
   };
   char *path[] = {
      "levels\\towndata",
      "levels\\l1data",
      "levels\\l2data",
      "levels\\l3data",
      "levels\\l4data",
      "nlevels\\l5data",
      "nlevels\\l6data",
      "nlevels\\towndata"
   };
   char *name[] = {
      "town",
      "l1",
      "l2",
      "l3",
      "l4",
      "l5",
      "l6",
      "town"
   };
   int min_size[] = { 16, 10, 10, 10, 16, 10, 10, 16 };
   char tmp[300];

   memset(&glb_map, 0, sizeof(glb_map));
   glb_map.act = -1;
   if ((act < 0) || (act > 7)) {
      fprintf(stderr, "Bad act asked %d\n", act);
      return 1;
   }
   sprintf(tmp, "%s\\%s.cel", path[act], name[act]);
   glb_map.cel = read_cel_file(tmp);
   if (glb_map.cel == NULL) {
      return 1;
   }
/*
   if (act == 7) {
      CEL *cel;
      int i;

      cel = glb_map.cel->cel[glb_map.cel->nb - 1];
      for (i = glb_map.cel->nb-1; i > 0; --i)
         glb_map.cel->cel[glb_map.cel->nb]=glb_map.cel->cel[glb_map.cel->nb - 1];
      glb_map.cel->cel[0]=cel;
   }
*/
   glb_map.min_size = min_size[act];
   sprintf(tmp, "%s\\%s.min", path[act], name[act]);
   glb_map.min = read_min(tmp, min_size[act]);
   if (glb_map.min == NULL) {
      fprintf(stderr, "Cant load %s\n", tmp);
      free_map();
      return 1;
   }
   sprintf(tmp, "%s\\%s.til", path[act], name[act]);
   glb_map.til = read_til(tmp);
   if (glb_map.til == NULL) {
      fprintf(stderr, "Cant load %s\n", tmp);
      free_map();
      return 1;
   }
   sprintf(tmp, "%s\\%s.sol", path[act], name[act]);
   glb_map.sol = read_sol(tmp);
   if (glb_map.sol == NULL) {
      fprintf(stderr, "Cant load %s\n", tmp);
      free_map();
      return 1;
   }
   glb_map.act = act;
   glb_map.pal_default = pal_default[act];
   glb_map.path = path[act];
   if ((act != 0) && (act != 7)) {
      sprintf(tmp, "%s\\%s.amp", path[act], name[act]);
      glb_map.amp = read_amp(tmp);
      if (glb_map.sol == NULL) {
         fprintf(stderr, "Cant load %s\n", tmp);
         free_map();
         return 1;
      }
   }
   if ((act == 1) || (act == 2) || (act == 5)) {
      int i;
      sprintf(tmp, "%s\\%ss.cel", path[act], name[act]);
      glb_map.scel = load_anim(tmp);
      if (glb_map.scel == NULL) {
         free_map();
         fprintf(stderr, "Cant load %s\n", tmp);
         return 1;
      }
      for (i = 0; i < glb_map.scel->nb; ++i) {
         decode_header_cel(glb_map.scel->frame + i, NULL);
      }
   }
   empty_tile = create_bitmap(64, 32);
   if (empty_tile == NULL) {
      free_map();
      fprintf(stderr, "Cant create empty tile bitmap\n");
      return 1;
   }
   clear(empty_tile);
   for (j = 31; j >= 0; --j)
      for (i = 32 - line_size[j]; i < 32 + line_size[j]; ++i)
         putpixel(empty_tile, i, j, 5);
   return 0;
}

int free_map() {
   int res = 0;

   destroy_bitmap(empty_tile);
   res += destroy_til(glb_map.til);
   glb_map.til = NULL;
   res += destroy_sol(glb_map.sol);
   glb_map.sol = NULL;
   res += destroy_min(glb_map.min);
   glb_map.min = NULL;
   res += destroy_cel_file(glb_map.cel);
   glb_map.cel = NULL;
   res += destroy_anim(glb_map.scel);
   glb_map.scel = NULL;
   res += destroy_amp(glb_map.amp);
   glb_map.amp = NULL;
   glb_map.act = -1;
   return res;
}

MIN *read_min(char *filename, int min_size) {
   MIN *res;
   unsigned char *buff;
   long int size;

   res = (MIN *)malloc(sizeof(MIN));
   if (res == NULL) {
      fprintf(stderr, "Cant alloc %u bytes\n", sizeof(MIN));
      return NULL;
   }
   misc_load_mpq_file(filename, &buff, &size, config.debug);
   res->data = (unsigned short int *)buff;
   if (res->data == NULL) {
      free(res);
      return NULL;
   }
   if (size % (min_size * 2) != 0) {
      fprintf(stderr, "Unusual %s size, should contain 0x%x sized entries\n", filename, min_size);
      free(res->data);
      free(res);
      return NULL;
   }
   res->nb = size / (min_size * 2);
   return res;
}

int destroy_min(MIN *min) {
   int res = 0;

   if (min == NULL)
      return res;
   if (min->data != NULL) {
      free(min->data);
      res += (glb_map.min_size * 2) * min->nb;
   }
   free(min);
   res += sizeof(MIN);
   return res;
}

SOL *read_sol(char *filename) {
   SOL *res;
   long int size;

   res = (SOL *)malloc(sizeof(SOL));
   if (res == NULL) {
      fprintf(stderr, "Cant alloc %u bytes\n", sizeof(SOL));
      return NULL;
   }
   misc_load_mpq_file(filename, &(res->data), &size, config.debug);
   if (res->data == NULL) {
      free(res);
      return NULL;
   }
   res->nb = size;
   return res;
}

int destroy_sol(SOL *sol) {
   int res = 0;

   if (sol == NULL)
      return res;
   if (sol->data != NULL) {
      free(sol->data);
      res += sol->nb;
   }
   free(sol);
   res += sizeof(SOL);
   return res;
}

TIL *read_til(char *filename) {
   TIL *res;
   long int size;
   unsigned char *buff;

   res = (TIL *)malloc(sizeof(TIL));
   if (res == NULL) {
      fprintf(stderr, "Cant alloc %u bytes\n", sizeof(TIL));
      return NULL;
   }
   misc_load_mpq_file(filename, &buff, &size, config.debug);
   res->data = (unsigned short int (*)[4])buff;
   if (res->data == NULL) {
      free(res);
      return NULL;
   }
   if (size % 0x8 != 0) {
      fprintf(stderr, "Unusual %s size, should contain 0x8 sized entries\n", filename);
      free(res->data);
      free(res);
      return NULL;
   }
   res->nb = size / 8;
   return res;
}

int destroy_til(TIL *til) {
   int res = 0;

   if (til == NULL)
      return res;
   if (til->data != NULL) {
      free(til->data);
      res += 0x8 * til->nb;
   }
   free(til);
   res += sizeof(TIL);
   return res;
}

AMP *read_amp(char *filename) {
   AMP *res;
   unsigned char *buff;
   long int size;

   res = (AMP *)malloc(sizeof(AMP));
   if (res == NULL) {
      fprintf(stderr, "Cant alloc %u bytes\n", sizeof(AMP));
      return NULL;
   }
   misc_load_mpq_file(filename, &buff, &size, config.debug);
   res->data = (unsigned short int *)buff;
   if (res->data == NULL) {
      free(res);
      return NULL;
   }
   res->nb = size / 2;
   return res;
}

int destroy_amp(AMP *amp) {
   int res = 0;

   if (amp == NULL)
      return res;
   if (amp->data != NULL) {
      free(amp->data);
      res += 2 * amp->nb;
   }
   free(amp);
   res += sizeof(AMP);
   return res;
}

int put_til(BITMAP *bmp, int til, int x, int y, int display_mask, unsigned short int mon[4], int selected) {
   int i, min_index;
   int dx[] = { 0, 32, -32, 0 };
   int dy[] = { 0, 16, 16, 32 };

   if (til >= glb_map.til->nb) {
      fprintf(stderr, "invalid til index : %d\n", til);
      return 1;
   }
   min_index = -1;
   for (i = 0; i < 4; ++i) {
      if (til >= 0)
         min_index = glb_map.til->data[til][i];
      put_min(bmp, min_index, x + dx[i], y + dy[i], mon[i], display_mask, selected);
      if (til >= 0)
         if (display_mask & DISPLAY_SOL)
            put_sol(bmp, min_index, x + dx[i], y + dy[i] - 12);
   }
   if (display_mask & DISPLAY_ARCH) {
      switch (glb_map.act) {
         case 1:
            switch (til) {
               case -1 :
                  break;
               case 119 :
                  draw_rle_sprite(bmp, glb_map.scel->frame[2].spr, x + 32, y + 16 - glb_map.scel->frame[2].spr->h);
                  break;
               case 120 :
                  draw_rle_sprite(bmp, glb_map.scel->frame[3].spr, x - 32, y + 16 - glb_map.scel->frame[3].spr->h);
                  break;
               case 122 :
                  draw_rle_sprite(bmp, glb_map.scel->frame[4].spr, x - 32, y + 16 - glb_map.scel->frame[4].spr->h);
                  break;
               case 125 :
                  draw_rle_sprite(bmp, glb_map.scel->frame[5].spr, x - 32, y + 16 - glb_map.scel->frame[5].spr->h);
                  break;
               default :
                  if (glb_map.amp->data[til] & 0x0100)
                     draw_rle_sprite(bmp, glb_map.scel->frame[6].spr, x - 32, y + 16 - glb_map.scel->frame[6].spr->h);
                  if (glb_map.amp->data[til] & 0x0200)
                     draw_rle_sprite(bmp, glb_map.scel->frame[7].spr, x + 32, y + 16 - glb_map.scel->frame[7].spr->h);
                  if (glb_map.amp->data[til] & 0x0400)
                     draw_rle_sprite(bmp, glb_map.scel->frame[0].spr, x - 32, y + 16 - glb_map.scel->frame[0].spr->h);
                  if (glb_map.amp->data[til] & 0x0800)
                     draw_rle_sprite(bmp, glb_map.scel->frame[1].spr, x + 32, y + 16 - glb_map.scel->frame[1].spr->h);
            }
            break;
         case 2:
            switch (til) {
               case -1 :
                  break;
               default :
                  if (glb_map.amp->data[til] & 0x0100)
                     draw_rle_sprite(bmp, glb_map.scel->frame[4].spr, x, y - glb_map.scel->frame[4].spr->h);
                  if (glb_map.amp->data[til] & 0x0200)
                     draw_rle_sprite(bmp, glb_map.scel->frame[5].spr, x, y - glb_map.scel->frame[5].spr->h);
                  if (glb_map.amp->data[til] & 0x0400) {
                     draw_rle_sprite(bmp, glb_map.scel->frame[1].spr, x - 64, y + 32 - glb_map.scel->frame[1].spr->h);
                     draw_rle_sprite(bmp, glb_map.scel->frame[0].spr, x - 96, y + 48 - glb_map.scel->frame[0].spr->h);
                  }
                  if (glb_map.amp->data[til] & 0x0800) {
                     draw_rle_sprite(bmp, glb_map.scel->frame[2].spr, x + 64, y + 32 - glb_map.scel->frame[2].spr->h);
                     draw_rle_sprite(bmp, glb_map.scel->frame[3].spr, x + 96, y + 48 - glb_map.scel->frame[3].spr->h);
                  }
            }
            break;
         case 5:
            switch (til) {
               case 0x65 :
               case 0x66 :
               case -1 :
                  break;
/*               default :
                  if (glb_map.amp->data[til] & 0x0100)
                     draw_rle_sprite(bmp, glb_map.scel->frame[0].bmp, x - 32, y + 16 - glb_map.scel->frame[0].bmp->h);
                  if (glb_map.amp->data[til] & 0x0200)
                     draw_rle_sprite(bmp, glb_map.scel->frame[1].bmp, x + 32, y + 16 - glb_map.scel->frame[1].bmp->h);*/
            }
            break;
         default :
         	break;
      }
   }
   if (til >= 0) {
      if (display_mask & DISPLAY_AMP) {
         put_amp(bmp, til, x, y - 8);
      }
   }
   return 0;
}

int put_min(BITMAP *bmp, int min, int x, int y, unsigned short int mon, int display_mask, int selected) {
   int i, cel_index, cel_type;

   if (min >= glb_map.min->nb) {
      fprintf(stderr, "Invalid min index : %d\n", min);
      return 1;
   }
   i = glb_map.min_size - 2;
   if (display_mask & DISPLAY_WALL)
      i = 0;
   if (min >= 0) {
      while (i < glb_map.min_size) {
         cel_index = glb_map.min->data[min * glb_map.min_size + i];
         if (cel_index != 0) {
            cel_type = cel_index >> 12;
            cel_index = (cel_index & 0x0FFF) - 1;
            put_cel(bmp, cel_index, cel_type, (i & 1) ? x + 32 : x, y - 32 * ((glb_map.min_size - i - 1) >> 1), selected);
         }
         ++i;
      }
   }
   else {
      if (selected)
      draw_lit_sprite(bmp, empty_tile, x, y - 32, 127);
   }
   if (display_mask & DISPLAY_MON) {
      put_monster_by_index(bmp, mon, x, y, selected, 1);
   }
   return 0;
}

int put_sol(BITMAP *bmp, int sol, int x, int y) {

   if ((sol < 0) || (sol >= glb_map.sol->nb)) {
      fprintf(stderr, "Invalid sol index : %d\n", sol);
      return 1;
   }
   sol = glb_map.sol->data[sol];
   put_digit(bmp, sol >> 4, x + 22, y);
   put_digit(bmp, sol & 0xF, x + 32, y);
   return 0;
}

int put_amp(BITMAP *bmp, int amp, int x, int y) {

   if (glb_map.amp == NULL) {
      fprintf(stderr, "glb_map.amp == NULL\n");
      return 1;
   }
   if ((amp < 0) || (amp >= glb_map.amp->nb)) {
      fprintf(stderr, "Invalid amp index : %d\n", amp);
      return 1;
   }
   amp = glb_map.amp->data[amp];
   put_digit(bmp, (amp >> 12) & 0xF, x + 12, y + 11);
   put_digit(bmp, (amp >>  8) & 0xF, x + 22, y + 11);
   put_digit(bmp, (amp >>  4) & 0xF, x + 32, y + 11);
   put_digit(bmp, amp & 0xF, x + 42, y + 11);
   return 0;
}

BITMAP *export_min(int display_mask, int nb_per_line) {
   int i, dx, dy, x, y;
   BITMAP *res = NULL;

   dx = 64 + 16;
   dy = (glb_map.min_size / 2) * 32 + 16;
   if (glb_map.min->nb < nb_per_line) {
      x = 16 + dx * glb_map.min->nb;
      y = 16 + dy;
   }
   else {
      x = dx * nb_per_line + 16;
      y = 16 + dy * (glb_map.min->nb / nb_per_line + 1);
   }
   res = create_bitmap(x, y);
   if (res == NULL) {
      fprintf(stderr, "%d*%d bitmap not created for min\n", x, y);
      return res;
   }
   clear(res);
   for (i = 0; i < glb_map.min->nb; ++i) {
      put_min(res, i, 16 + (i % nb_per_line) * dx, dy + (i / nb_per_line) * dy, 0, display_mask, 0);
      if (display_mask & DISPLAY_SOL)
         put_sol(res, i, 16 + (i % nb_per_line) * dx, 18 + dy + (i / nb_per_line) * dy);
   }
   return res;
}

BITMAP *export_til(int display_mask, int nb_per_line) {
   int i, dx, dy, x, y;
   BITMAP *res = NULL;
   unsigned short int mon[4];

   dx = 128 + 32;
   dy = (glb_map.min_size / 2) * 32 + 64;
   if (glb_map.til->nb < nb_per_line) {
      x = 32 + dx * glb_map.til->nb;
      y = 32 + dy;
   }
   else {
      x = nb_per_line * dx + 32;
      y = 32 + dy * (glb_map.til->nb / nb_per_line + 1);
   }
   res = create_bitmap(x, y);
   if (res == NULL) {
      fprintf(stderr, "%d*%d bitmap not created for til\n", x, y);
      return res;
   }
   clear(res);
   memset(mon, 0, sizeof(mon));
   for (i = 0; i < glb_map.til->nb; ++i) {
      put_til(res, i, 64 + (i % nb_per_line) * dx, dy - 64 + (i / nb_per_line) * dy, display_mask, mon, 0);
   }
   return res;
}

int get_til_min(char *filenames, int act, int display_mask) {
   PALETTE palette;
   char tmp[300];
   BITMAP *bmp_til, *bmp_min;

   if (init_map(act))
      return 1;
   bmp_til = export_til(display_mask, 16);
   if (read_palette(glb_map.pal_default, palette)) {
      sprintf(tmp, "%s\\%s\\%s_til.bmp", mod_dir[0], glb_map.path, filenames);
      save_bitmap(tmp, bmp_til, palette);
      fprintf(stderr, "%s saved\n", tmp);
   }
   destroy_bitmap(bmp_til);
   bmp_min = export_min(display_mask, 32);
   if (read_palette(glb_map.pal_default, palette)) {
      sprintf(tmp, "%s\\%s\\%s_min.bmp", mod_dir[0], glb_map.path, filenames);
      save_bitmap(tmp, bmp_min, palette);
      fprintf(stderr, "%s saved\n", tmp);
   }
   destroy_bitmap(bmp_min);
   free_map();
   return 0;
}

int put_cel(BITMAP *bmp, int cel, int type, int x, int y, int selected) {

   if (glb_map.cel == NULL) {
      fprintf(stderr, "glb_map.cel == NULL\n");
      return 1;
   }
   if ((cel < 0) ||(cel >= glb_map.cel->nb)) {
      fprintf(stderr, "bad cel index : 0x%X\n", cel);
      return 1;
   }
   if (glb_map.cel->cel[cel] == NULL) {
      fprintf(stderr, "glb_map.cel->cel[cel] == NULL\n");
      return 1;
   }
   if (glb_map.cel->cel[cel]->spr == NULL) {
      switch (type) {
         case 0 :
            if (decode_tile_0(glb_map.cel->cel[cel]))
               return 1;
            break;
         case 1 :
            if (decode_tile_1(glb_map.cel->cel[cel]))
               return 1;
            break;
         case 2 :
            if (decode_tile_2(glb_map.cel->cel[cel]))
               return 1;
            break;
         case 3 :
            if (decode_tile_3(glb_map.cel->cel[cel]))
               return 1;
            break;
         case 4 :
            if (decode_tile_4(glb_map.cel->cel[cel]))
               return 1;
            break;
         case 5 :
            if (decode_tile_5(glb_map.cel->cel[cel]))
               return 1;
            break;
         default :
            fprintf(stderr, "unknown tile type : %d\n", type);
            return 1;
      }
      if (glb_map.cel->cel[cel]->spr == NULL) {
         return 1;
      }
   }
   if (selected)
      draw_lit_rle_sprite(bmp, glb_map.cel->cel[cel]->spr, x, y - glb_map.cel->cel[cel]->spr->h, 127);
   else
      draw_rle_sprite(bmp, glb_map.cel->cel[cel]->spr, x, y - glb_map.cel->cel[cel]->spr->h);
   return 0;
}
