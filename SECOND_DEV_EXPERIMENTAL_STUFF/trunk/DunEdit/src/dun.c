#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "global_allegro.h"
#include "dun.h"
#include "mpq_lib.h"
#include "map.h"

DUN *read_dun(char *filename) {
   DUN *res;
   long int size;
   int i;

   res = (DUN *)malloc(sizeof(DUN));
   if (res == NULL) {
      fprintf(stderr, "Cant alloc %u bytes\n", sizeof(DUN));
      return NULL;
   }
   misc_load_mpq_file(filename, &res->data, &size, config.debug);
   if (res->data == NULL) {
      free(res);
      return NULL;
   }
   res->w = ((unsigned short int *)res->data)[0];
   res->h = ((unsigned short int *)res->data)[1];
   if ((size - 4) % (0x2 * res->w * res->h) != 0) {
      fprintf(stderr, "Unusual %s size, should contain (2*Width*Height)=0x%X sized entries\n", filename, 2 * res->w * res->h);
      free(res->data);
      free(res);
      return NULL;
   }
   res->tiles = ((unsigned short int *)res->data) + 2;
   if (((((size - 4) / (0x2 * res->w * res->h)) - 1) % 4) != 0) {
      fprintf(stderr, "Unusual %s size, Cant fill extra layers\n", filename);
      free(res->data);
      free(res);
      return NULL;
   }
   res->nb_layer = (((size - 4) / (0x2 * res->w * res->h)) - 1) / 4;
   if (res->nb_layer > 4)
      fprintf(stderr, "%s has too many layers ? (%d)\n", filename, res->nb_layer);
   for (i = 0; (i < 4) && (i < res->nb_layer); ++i)
      res->layer[i] = res->tiles + res->w * res->h * (1 + 4 * i);
   return res;
}

int destroy_dun(DUN *dun) {
   int res = 0;

   if (dun == NULL)
      return res;
   if (dun->data != NULL) {
      free(dun->data);
      res += 2 + 0x2 * (1 + 4 * dun->nb_layer) * dun->w * dun->h;
   }
   free(dun);
   res += sizeof(DUN);
   return res;
}

BITMAP *aff_dun(DUN *dun, int display_mask, int space) {
   BITMAP *bmp;

   if (dun == NULL) {
      fprintf(stderr, "dun == NULL\n");
      return NULL;
   }
   if (dun->data == NULL) {
      fprintf(stderr, "dun->data == NULL\n");
      return NULL;
   }
   bmp = create_bitmap((64 + space) * (dun->w + dun->h), (32 + space) * (dun->w + dun->h + glb_map.min_size / 2 - 1));
   if (bmp == NULL) {
      fprintf(stderr, "Cant create bitmap\n");
      return NULL;
   }
   clear_bitmap(bmp);
   put_dun(bmp, dun, display_mask, space, 32 + (64 + space) * (dun->h - 1), (32 + space) * (glb_map.min_size / 2)- 1);
   return bmp;
}

int put_dun_layer(BITMAP *bmp, DUN *dun, int layer, int space, int xx, int yy) {
   unsigned short int val;
   int nb_digit, x0, y0, i, j;

   if (dun == NULL)
   if (layer >= dun->nb_layer) {
      fprintf(stderr, "This layer (%d) doesn't exist\n", layer);
      return 1;
   }
   for (i = 0; i < 2 * dun->h; ++i) {
      for (j = 0; j < 2 * dun->w; ++j) {
         val = dun->layer[layer][i * 2 * dun->w + j];
         nb_digit = 0;
         while (val > 0) {
            val >>= 4;
            ++nb_digit;
         }
         if (nb_digit > 0) {
            x0 = xx + (32 * j + space * (j / 2)) - (32 * i + space * (i / 2)) + 4 * (16 + nb_digit) - 40;
            y0 = yy + (16 * j + space * (j / 2)) + (16 * i + space * (i / 2)) - 12;
            val = dun->layer[layer][i * 2 * dun->w + j];
            while (val > 0) {
               put_digit(bmp, val & 0xF, x0, y0);
               val >>= 4;
               x0 -= 8;
            }
         }
      }
   }
   return 0;
}

int put_dun(BITMAP *bmp, DUN *dun, int display_mask, int space, int xx, int yy) {
   int i, j, til;
   unsigned short int mon[4];

   if (dun == NULL)
      return 1;
   memset(mon, 0, sizeof(mon));
   if (display_mask & DISPLAY_MON) {
      if (dun->nb_layer < 2) {
         display_mask ^= DISPLAY_MON;
         fprintf(stderr, "No layer for monsters\n");
      }
   }
   for (i = 0; i < dun->h; ++i) {
      for (j = 0; j < dun->w; ++j) {
         til = dun->tiles[i * dun->w + j] - 1;
         if (display_mask & DISPLAY_MON) {
            mon[0] = dun->layer[1][dun->w * (4 * i    ) + 2 * j    ];
            mon[1] = dun->layer[1][dun->w * (4 * i    ) + 2 * j + 1];
            mon[2] = dun->layer[1][dun->w * (4 * i + 2) + 2 * j    ];
            mon[3] = dun->layer[1][dun->w * (4 * i + 2) + 2 * j + 1];
         }
         put_til(bmp, til, xx + (64 + space) * (j - i), yy + (32 + space) * (i + j), display_mask, mon, 0);
      }
   }
   return 0;
}

int blit_npc(BITMAP *bmp, NPC_data *dat, int xx, int yy) {
   ANIM *anim;
   MULTI_ANIM *multi_anim;

   if (dat == NULL)
      return 1;
   if (dat->dir == -1) {
      anim = load_anim(dat->filename);
      put_image_anim(bmp, anim, 0, xx + 32 * (dat->y - dat->x), yy + 16 * (dat->x + dat->y), NULL, 0);
      destroy_anim(anim);
   }
   else {
      multi_anim = load_multi_anim(dat->filename, 0);
      put_image_multi_anim(bmp, multi_anim, dat->dir, 0, xx + 32 * (dat->y - dat->x), yy + 16 * (dat->x + dat->y), NULL, 0);
      destroy_multi_anim(multi_anim);
   }
   return 0;
}

DUN *get_town_dun(int exp) {
   char *dungeon[4] = { "levels\\towndata\\sector1s.dun", "levels\\towndata\\sector2s.dun", "levels\\towndata\\sector3s.dun", "levels\\towndata\\sector4s.dun" };
   DUN *s[4], *res;
   int i, j, w, h;
   int xx[4];
   int yy[4];

   for (i = 0; i < 4; ++i) {
      s[i] = read_dun(dungeon[i]);
      if (s[i] == NULL) {
         fprintf(stderr, "Cant read %s\n", dungeon[i]);
         for (j = 0; j < i; ++j)
            destroy_dun(s[j]);
         return NULL;
      }
   }
   i = 1;
   if (s[0]->w != s[1]->w)
      i = 0;
   if (s[2]->w != s[3]->w)
      i = 0;
   if (s[0]->h != s[2]->h)
      i = 0;
   if (s[1]->h != s[3]->h)
      i = 0;
   if (i == 0) {
      fprintf(stderr, "differents sizes\n");
      for (j = 0; j < 4; ++j)
         destroy_dun(s[j]);
      return NULL;
   }
   w = s[0]->w + s[3]->w;
   h = s[0]->h + s[3]->h;
   res = (DUN *)malloc(sizeof(DUN));
   if (res == NULL) {
      fprintf(stderr, "Cant alloc %u bytes\n", sizeof(DUN));
      for (j = 0; j < 4; ++j)
         destroy_dun(s[j]);
      return NULL;
   }
   res->data = (unsigned char *)malloc(4 + 2 * w * h * 17);
   if (res->data == NULL) {
      fprintf(stderr, "Cant alloc\n");
      free(res);
      for (j = 0; j < 4; ++j)
         destroy_dun(s[j]);
      return NULL;
   }
   memset(res->data, 0, 4 + w * h * 17);
   res->w = w;
   res->h = h;
   res->tiles = ((unsigned short int *)res->data) + 2;
   res->nb_layer = 4;
   for (i = 0; i < 4; ++i)
      res->layer[i] = res->tiles + res->w * res->h * (1 + 4 * i);
   xx[0] = s[3]->w;
   yy[0] = s[3]->h;
   xx[1] = s[3]->w;
   yy[1] = 0;
   xx[2] = 0;
   yy[2] = s[3]->h;
   xx[3] = 0;
   yy[3] = 0;
   for (i = 0; i < 4; ++i)
      for (j = 0; j < s[i]->h; ++j) {
         memcpy(res->tiles + (j + yy[i]) * res->w + xx[i], s[i]->tiles + j * s[i]->w, 2 * s[i]->w);
         if (s[i]->nb_layer > 0)
            memcpy(res->layer[0] + ((j + yy[i]) * res->w + xx[i]) * 4, s[i]->layer[0] + (j * s[i]->w) * 4, 8 * s[i]->w);
         if (s[i]->nb_layer > 1)
            memcpy(res->layer[1] + ((j + yy[i]) * res->w + xx[i]) * 4, s[i]->layer[1] + (j * s[i]->w) * 4, 8 * s[i]->w);
         if (s[i]->nb_layer > 2)
            memcpy(res->layer[2] + ((j + yy[i]) * res->w + xx[i]) * 4, s[i]->layer[2] + (j * s[i]->w) * 4, 8 * s[i]->w);
         if (s[i]->nb_layer > 3)
            memcpy(res->layer[3] + ((j + yy[i]) * res->w + xx[i]) * 4, s[i]->layer[3] + (j * s[i]->w) * 4, 8 * s[i]->w);
      }
   return res;
}

int get_town_map(int exp) {
   unsigned long int i;
   PALETTE palette;
   NPC_data npc_classic[12] = {
       { "towners\\townboy\\pegkid1.cel",	0x35, 0x0B, -1 },
       { "towners\\butch\\deadguy.cel", 	0x20, 0x18, -1 },
       { "towners\\twnf\\twnfn.cel",		0x3E, 0x37, -1 },
       { "towners\\healer\\healer.cel",		0x4F, 0x37, -1 },
       { "towners\\drunk\\twndrunk.cel",	0x54, 0x47, -1 },
       { "towners\\townwmn1\\witch.cel",	0x14, 0x50, -1 },
       { "towners\\smith\\smithn.cel",		0x3F, 0x3E, -1 },
       { "towners\\strytell\\strytell.cel",	0x47, 0x3E, -1 },
       { "towners\\townwmn1\\wmnn.cel",		0x42, 0x2B, -1 },
       { "Towners\\animals\\cow.cel",		0x10, 0x3A, 01 },
       { "Towners\\animals\\cow.cel",		0x0E, 0x38, 03 },
       { "Towners\\animals\\cow.cel",		0x14, 0x3B, 04 } };
   NPC_data npc_exp[5] = {
       { "Towners\\Farmer\\Farmrn2.CEL",	0x10,	0x3E, -1 },
       { "Towners\\Farmer\\mfrmrn2.CEL",	0x3E,	0x3D, -1 },
       { "Towners\\Farmer\\cfrmrn2.CEL",	0x3E,	0x3D, -1 },
       { "Towners\\Girl\\Girls1.CEL",		0x2B,	0x4D, -1 },
       { "Towners\\Girl\\Girlw1.CEL",		0x2B,	0x4D, -1 } };
   BITMAP *town_map;
   DUN *dun;

   if (exp) {
      if (init_map(7))
         return 1;
   }
   else {
      if (init_map(0))
         return 1;
   }
   dun = get_town_dun(exp);
   if (dun == NULL) {
      fprintf(stderr, "Cant create bitmap\n");
      free_map();
      return 1;
   }
   town_map = aff_dun(dun, DISPLAY_WALL, 0);
   for (i = 0; i < 12; ++i) {
//      blit_npc(town_map, npc_classic + i, 128 * 0x19, 0);
   }
   if (exp) {
//      blit_npc(town_map, npc_exp, 64 * (dun->h - 1), 32 * (glb_map.min_size / 2) - 1);
   }
   if (read_palette("levels\\towndata\\town.pal", palette)) {
      if (exp) {
         save_bitmap("town_hell_map.bmp", town_map, palette);
         fprintf(stderr, "town_hell_map.bmp saved\n");
      }
      else {
         save_bitmap("town_map.bmp", town_map, palette);
         fprintf(stderr, "town_map.bmp saved\n");
      }
   }
   if (read_palette("levels\\towndata\\ltpalg.pal", palette)) {
      if (exp) {
         save_bitmap("town_hell_map_nb.bmp", town_map, palette);
         fprintf(stderr, "town_hell_map_nb.bmp saved\n");
      }
      else {
         save_bitmap("town_map_nb.bmp", town_map, palette);
         fprintf(stderr, "town_map_nb.bmp saved\n");
      }
   }
   destroy_bitmap(town_map);
   free_map();
   return 0;
}

int export_dun_files(char *names[], int act, int display_mask) {
   int i;
   char tmp[300];
   PALETTE palette;
   DUN *dun;
   BITMAP *bmp;

   if (init_map(act))
      return 1;
   read_palette(glb_map.pal_default, palette);
   for (i = 0; names[i][0] != '\0'; ++i) {
      sprintf(tmp, "%s\\%s.dun", glb_map.path, names[i]);
      dun = read_dun(tmp);
      if (dun == NULL)
         break;
      bmp = aff_dun(dun, display_mask, 0);
      sprintf(tmp, "%s\\%s\\%s.bmp", mod_dir[0], glb_map.path, names[i]);
      if (strcmp(names[i], "foulwatr") == 0)
         read_palette("levels\\l3data\\l3pfoul.pal", palette);
      save_bitmap(tmp, bmp, palette);
      if (strcmp(names[i], "foulwatr") == 0)
         read_palette(glb_map.pal_default, palette);
      fprintf(stderr, "%s saved\n", tmp);
      destroy_bitmap(bmp);
   }
   free_map();
   return 0;
}

DUN *create_dun(int w, int h, int nbl) {
   int i;
   DUN *res;

   if ((w <= 0) || (h <= 0) || (nbl < 0)) {
      fprintf(stderr, "Negative argument for create_dun(%d, %d, %d)\n", w, h, nbl);
      return NULL;
   }
   res = (DUN *)malloc(sizeof(DUN));
   if (res == NULL) {
      fprintf(stderr, "Cant create DUN struct\n");
      return NULL;
   }
   memset(res, 0, sizeof(DUN));
   res->data = (unsigned char *)malloc(4 + 2 * w * h * (1 + 4 * nbl));
   if (res->data == NULL) {
      fprintf(stderr, "Cant alloc memory for DUN data\n");
      free(res);
      return NULL;
   }
   memset(res->data, 0, 4 + 2 * w * h * (1 + 4 * nbl));
   *((unsigned short int *)res->data) = w;
   *((unsigned short int *)(res->data + 2)) = h;
   res->w = w;
   res->h = h;
   res->nb_layer = nbl;
   res->tiles = (unsigned short int *)(res->data + 4);
   for (i = 0; i < nbl; ++i)
      res->layer[i] = res->tiles + res->w * res->h * (1 + 4 * i);
   return res;
}

int copy_dun(DUN *src, int sx, int sy, int w, int h, DUN *dest, int dx, int dy) {
   int i, j;

   if (src == NULL)
      return 1;
   if (src->data == NULL)
      return 1;
   if (dest == NULL)
      return 1;
   if (dest->data == NULL)
      return 1;
   if ((w < 0) || (h < 0))
      return 1;
   if (sx < 0) {
      w += sx;
      dx -= sx;
      sx = 0;
   }
   if (dx < 0) {
      w += dx;
      sx -= dx;
      dx = 0;
   }
   if (sy < 0) {
      h += sy;
      dy -= sy;
      sy = 0;
   }
   if (dy < 0) {
      h += dy;
      sy -= dy;
      dy = 0;
   }
   if (sx + w > src->w)
      w = src->w - sx;
   if (sy + h > src->h)
      h = src->h - sy;
   if (dx + w > dest->w)
      w = dest->w - dx;
   if (dy + h > dest->h)
      h = dest->h - dy;
   for (i = 0; i < h; ++i) {
      memcpy(dest->tiles + (i + dy) * dest->w + dx, src->tiles + (i + sy) * src->w + sx, 2 * w);
      for (j = 0; (j < src->nb_layer) && (j < dest->nb_layer); ++j) {
         memcpy(dest->layer[j] + ((i + dy) * 2 * 2 * dest->w + 2 * dx), src->layer[j] + ((i + sy) * 2 * 2 * src->w + 2 * sx), 4 * w);
         memcpy(dest->layer[j] + (((i + dy) * 2 + 1 ) * 2 * dest->w + 2 * dx), src->layer[j] + (((i + sy) * 2 + 1 ) * 2 * src->w + 2 * sx), 4 * w);
      }
   }
   return 0;
}

int put_dun_with_select(BITMAP *bmp, DUN *dun, int display_mask, int space, int xx, int yy, int x0, int y0, int x1, int y1) {
   int i, j, til, selected;
   unsigned short int mon[4];

   memset(mon, 0, sizeof(mon));
   if (display_mask & DISPLAY_MON) {
      if (dun->nb_layer < 2) {
         display_mask ^= DISPLAY_MON;
      }
   }
   for (i = 0; i < dun->h; ++i) {
      for (j = 0; j < dun->w; ++j) {
         selected = (((x0 <= j) && (j <= x1)) || ((x1 <= j) && (j <= x0))) && (((y0 <= i) && (i <= y1)) || ((y1 <= i) && (i <= y0)));
         til = dun->tiles[i * dun->w + j] - 1;
         if (display_mask & DISPLAY_MON) {
            mon[0] = dun->layer[1][dun->w * (4 * i    ) + 2 * j    ];
            mon[1] = dun->layer[1][dun->w * (4 * i    ) + 2 * j + 1];
            mon[2] = dun->layer[1][dun->w * (4 * i + 2) + 2 * j    ];
            mon[3] = dun->layer[1][dun->w * (4 * i + 2) + 2 * j + 1];
         }
         put_til(bmp, til, xx + (64 + space) * (j - i), yy + (32 + space) * (i + j), display_mask, mon, selected);
      }
   }
   return 0;
}
