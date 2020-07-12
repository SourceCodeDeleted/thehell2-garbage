#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cel.h"
#include "global_allegro.h"
#include "mpq_lib.h"

unsigned char trn_dummy[0x100] = {
   0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
   0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F,
   0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F,
   0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
   0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F,
   0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5A, 0x5B, 0x5C, 0x5D, 0x5E, 0x5F,
   0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F,
   0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7A, 0x7B, 0x7C, 0x7D, 0x7E, 0x7F,
   0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8A, 0x8B, 0x8C, 0x8D, 0x8E, 0x8F,
   0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 0x9A, 0x9B, 0x9C, 0x9D, 0x9E, 0x9F,
   0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7, 0xA8, 0xA9, 0xAA, 0xAB, 0xAC, 0xAD, 0xAE, 0xAF,
   0xB0, 0xB1, 0xB2, 0xB3, 0xB4, 0xB5, 0xB6, 0xB7, 0xB8, 0xB9, 0xBA, 0xBB, 0xBC, 0xBD, 0xBE, 0xBF,
   0xC0, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7, 0xC8, 0xC9, 0xCA, 0xCB, 0xCC, 0xCD, 0xCE, 0xCF,
   0xD0, 0xD1, 0xD2, 0xD3, 0xD4, 0xD5, 0xD6, 0xD7, 0xD8, 0xD9, 0xDA, 0xDB, 0xDC, 0xDD, 0xDE, 0xDF,
   0xE0, 0xE1, 0xE2, 0xE3, 0xE4, 0xE5, 0xE6, 0xE7, 0xE8, 0xE9, 0xEA, 0xEB, 0xEC, 0xED, 0xEE, 0xEF,
   0xF0, 0xF1, 0xF2, 0xF3, 0xF4, 0xF5, 0xF6, 0xF7, 0xF8, 0xF9, 0xFA, 0xFB, 0xFC, 0xFD, 0xFE, 0xFF
};

CEL_FILE *read_cel_file(char *filename) {
   long int size, nb, i;
   unsigned char *data, *start, *end;
   CEL_FILE *result;

   misc_load_mpq_file(filename, &data, &size, config.debug);
   if (data == NULL) {
      return NULL;
   }
   if (size < 4) {
      fprintf(stderr, "File too short !\n");
      free(data);
      return NULL;
   }
   nb = ((unsigned long int *)data)[0];
   if (size < 4 + 4 * nb) {
      fprintf(stderr, "File too short !\n");
      free(data);
      return NULL;
   }
   if (size != ((unsigned long int *)data)[nb + 1]) {
      fprintf(stderr, "Unexpected file size\n");
      free(data);
      return NULL;
   }
   result = (CEL_FILE *)malloc(sizeof(CEL_FILE));
   if (result == NULL) {
      fprintf(stderr, "Can't alloc %u bytes for file struct\n", sizeof(CEL_FILE));
      free(data);
      return NULL;
   }
   result->nb = nb;
   result->cel = (CEL **)malloc(nb * sizeof(CEL *));
   if (result->cel == NULL) {
      fprintf(stderr, "Can't alloc %u bytes for entry array\n", sizeof(nb * sizeof(CEL)));
      free(data);
      destroy_cel_file(result);
      return NULL;
   }
   for (i = 0; i < nb; ++i) {
      result->cel[i] = (CEL *)malloc(sizeof(CEL));
      if (result->cel[i] == NULL) {
         fprintf(stderr, "Can't alloc %u bytes for entry struct\n", sizeof(CEL));
      }
      else {
         result->cel[i]->size = 0;
         result->cel[i]->data = NULL;
         result->cel[i]->spr = NULL;
      }
   }
   for (i = 0; i < nb; ++i) {
      if (result->cel[i] != NULL) {
         start = data + ((unsigned long int *)data)[i+1];
         end = data + ((unsigned long int *)data)[i+2];
         size = end - start;
         result->cel[i]->data = (unsigned char *)malloc(size);;
         if (result->cel[i]->data == NULL) {
            fprintf(stderr, "Can't alloc %lu bytes for entry data\n", size);
            fprintf(stderr, "Skipping entry %lu (0x%lx)\n", i, i);
            break;
         }
         result->cel[i]->size = size;
         memcpy(result->cel[i]->data, start, size);
      }
   }
   free(data);
   return result;
}

int destroy_cel_file(CEL_FILE *tc) {
   int result, i;

   result = 0;
   if (tc == NULL)
      return result;
   if (tc->cel) {
      for (i = 0; i < tc->nb; ++i) {
         if (tc->cel[i] != NULL) {
            if (tc->cel[i]->data != NULL) {
               free(tc->cel[i]->data);
               tc->cel[i]->data = NULL;
            }
            if (tc->cel[i]->spr != NULL) {
				free( tc->cel[i]->spr );
				//destroy_rle_sprite(tc->cel[i]->spr);
            }
            result += tc->cel[i]->size;
            tc->cel[i]->size = 0;
         }
      }
      free(tc->cel);
      result += tc->nb * sizeof(CEL *);
      tc->nb = 0;
   }
   free(tc);
   result += sizeof(CEL_FILE);
   return result;
}

ANIM *load_anim(char *filename) {
   long int size, i;
   ANIM *res;

   res = (ANIM *)malloc(sizeof(ANIM));
   if (res == NULL) {
      fprintf(stderr, "Cant alloc %u bytes for ANIM struct\n", sizeof(ANIM));
      return NULL;
   }
   misc_load_mpq_file(filename, &(res->data), &size, config.debug);
   if (res->data == NULL) {
      free(res);
      return NULL;
   }
   if (size < 4) {
      fprintf(stderr, "File too short\n");
      free(res->data);
      free(res);
      return NULL;
   }
   res->nb = *((unsigned long int *)res->data);
   res->frame = (CEL *)malloc(res->nb * sizeof(CEL));
   if (res->frame == NULL) {
      fprintf(stderr, "Cant alloc %lu bytes for ANIM\n", res->nb * sizeof(CEL));
      free(res->data);
      free(res);
      return NULL;
   }
   for (i = 0; i < res->nb; ++i) {
      res->frame[i].spr = NULL;
      res->frame[i].data = res->data + ((unsigned long int *)res->data)[i + 1];
      res->frame[i].size = ((unsigned long int *)res->data)[i + 2] - ((unsigned long int *)res->data)[i + 1];
   }
   return res;
}

int destroy_anim(ANIM *anim) {
   int res = 0, i;

   if (anim == NULL)
      return res;
   for (i = 0; i < anim->nb; ++i) {
      if (anim->frame[i].spr != NULL) {
		free( anim->frame[i].spr );
		//destroy_rle_sprite( anim->frame[i].spr );
      }
   }
   free(anim->frame);
   res += anim->nb * sizeof(CEL);
   free(anim->data);
   free(anim);
   res += sizeof(ANIM);
   return res;
}

MULTI_ANIM *load_multi_anim(char *filename, int type) {
   unsigned long int i, j, offset;
   long int size;
   MULTI_ANIM *res;

   res = (MULTI_ANIM *)malloc(sizeof(MULTI_ANIM));
   if (res == NULL) {
      fprintf(stderr, "Cant alloc %u bytes for MULTI_ANIM struct\n", sizeof(MULTI_ANIM));
      return NULL;
   }
   res->cl2 = type;
   misc_load_mpq_file(filename, &(res->data), &size, config.debug);
   if (res->data == NULL) {
      free(res);
      return NULL;
   }
   if (size < 4) {
      fprintf(stderr, "File too short\n");
      free(res->data);
      free(res);
      return NULL;
   }
   res->nb = 8;
   res->sub_nb = (unsigned long int *)malloc(res->nb * sizeof(unsigned long int));
   if (res->sub_nb == NULL) {
      fprintf(stderr, "Cant alloc %lu bytes for MULTI_ANIM\n", res->nb * sizeof(unsigned long int));
      free(res->data);
      free(res);
      return NULL;
   }
   res->frame = (CEL **)malloc(res->nb * sizeof(CEL *));
   if (res->frame == NULL) {
      fprintf(stderr, "Cant alloc %lu bytes for MULTI_ANIM\n", res->nb * sizeof(CEL *));
      free(res->sub_nb);
      free(res->data);
      free(res);
      return NULL;
   }
   memset(res->frame, 0, res->nb * sizeof(CEL *));
   for (i = 0; i < res->nb; ++i) {
      offset = ((unsigned long int *)res->data)[i];
      res->sub_nb[i] = ((unsigned long int *)(res->data + offset))[0];
      res->frame[i] = (CEL *)malloc(res->sub_nb[i] * sizeof(CEL));
      if (res->frame[i] == NULL) {
         fprintf(stderr, "Cant alloc %lu bytes for Frame\n", res->sub_nb[i] * sizeof(CEL));
         for (j = 0; j < i; ++j)
            free(res->frame[j]);
         free(res->frame);
         free(res->sub_nb);
         free(res);
         return NULL;
      }
      for (j = 0; j < res->sub_nb[i]; ++j) {
         res->frame[i][j].data = res->data + offset + ((unsigned long int *)(res->data + offset))[j + 1];
         res->frame[i][j].size = ((unsigned long int *)(res->data + offset))[j + 2] - ((unsigned long int *)(res->data + offset))[j + 1];
         res->frame[i][j].spr = NULL;
      }
   }
   return res;
}

int destroy_multi_anim(MULTI_ANIM *multi_anim) {
   int res;

   res = 0;
   if (multi_anim == NULL)
      return res;
/*bug here   if (multi_anim->frame != NULL) {
      for (i = 0; i < multi_anim->nb; ++i) {
         free(multi_anim->frame[i]);
         res += multi_anim->sub_nb[i] * sizeof(CEL);
      }
   }
   for (i = 0; i < multi_anim->nb; ++i) {
      for (j = 0; j < multi_anim->sub_nb[i]; ++j) {
         if (multi_anim->frame[i][j]->spr) {
            destroy_bitmap(multi_anim->frame[i][j]->spr);
         }
      }
   }
   */
   free(multi_anim->frame);
   res += multi_anim->nb * sizeof(CEL *);
   free(multi_anim->data);
   free(multi_anim);
   res += sizeof(MULTI_ANIM);
   return res;
}

int put_digit(BITMAP *bmp, int digit, int x, int y) {
   int i, j;
   unsigned char font[16][8] = {
      { 0x0E, 0x11, 0x11, 0x11, 0x11, 0x11, 0x0E, 0x00 },
      { 0x04, 0x0C, 0x04, 0x04, 0x04, 0x04, 0x0E, 0x00 },
      { 0x0E, 0x11, 0x01, 0x02, 0x04, 0x08, 0x1F, 0x00 },
      { 0x0E, 0x11, 0x01, 0x06, 0x01, 0x11, 0x0E, 0x00 },
      { 0x02, 0x06, 0x0A, 0x1F, 0x02, 0x02, 0x0E, 0x00 },
      { 0x1E, 0x10, 0x10, 0x1E, 0x01, 0x11, 0x0E, 0x00 },
      { 0x07, 0x08, 0x10, 0x17, 0x19, 0x11, 0x0E, 0x00 },
      { 0x1F, 0x11, 0x02, 0x04, 0x04, 0x08, 0x08, 0x00 },
      { 0x0E, 0x11, 0x11, 0x0E, 0x11, 0x11, 0x0E, 0x00 },
      { 0x0E, 0x11, 0x13, 0x0F, 0x01, 0x02, 0x1E, 0x00 },
      { 0x04, 0x0A, 0x0A, 0x0E, 0x11, 0x11, 0x11, 0x00 },
      { 0x1E, 0x11, 0x11, 0x1E, 0x11, 0x11, 0x1E, 0x00 },
      { 0x06, 0x09, 0x10, 0x10, 0x10, 0x09, 0x06, 0x00 },
      { 0x1C, 0x12, 0x11, 0x11, 0x11, 0x12, 0x1C, 0x00 },
      { 0x1F, 0x11, 0x10, 0x1C, 0x10, 0x11, 0x1F, 0x00 },
      { 0x1F, 0x11, 0x10, 0x1C, 0x10, 0x10, 0x18, 0x00 } };

   if (bmp == NULL) {
      fprintf(stderr, "bmp == NULL\n");
      return 1;
   }
   for (j = 0; j < 8; ++j)
      for (i = 0; i < 8; ++i)
         if (font[digit][j] & (0x80 >> i))
            putpixel(bmp, x + i, y + j - 8, 255);
   return 0;
}

int decode_tile_0(CEL *cel) {
   int x, y, i, size;

   if (cel->size != 0x400) {
      fprintf(stderr, "Size not handled 0 : %lu\n", cel->size);
      return 1;
   }
   size = 0x400 + 0x40;
   cel->spr = (RLE_SPRITE *)malloc(sizeof(RLE_SPRITE) + size);
   if (cel->spr == NULL) {
      fprintf(stderr, "Can't alloc memory\n");
      return 1;
   }
   cel->spr->w = 0x20;
   cel->spr->h = 0x20;
   cel->spr->color_depth = 8;
   i = 0;
   for (y = 0x1F; y >= 0; --y) {
      cel->spr->dat[y * 0x22] = 0x20;
      cel->spr->dat[y * 0x22 + 0x21] = 0x00;
      for (x = 1; x <= 0x20; ++x) {
         cel->spr->dat[y * 0x22 + x] = cel->data[i];
         ++i;
      }
   }
   cel->spr->size = i;
   return 0;
}

int decode_tile_1(CEL *cel) {
   int x, y, nb_pix, i, j, size, c;
   int line_offset[0x20];

   i = 0;
   x = 0;
   size = 0;
   y = 31;
   while (y >= 0) {
      if (x == 0) {
         line_offset[y] = i;
      }
      if (i >= cel->size) {
         fprintf(stderr, "too few data for cel (type 1) a !\n");
         return 1;
      }
      if (cel->data[i] >= 0x80) {
         nb_pix = 0x100 - cel->data[i];
         x += nb_pix;
         ++size;
         ++i;
      }
      else {
         nb_pix = cel->data[i];
         ++size;
         ++i;
         for (j = 0; j < nb_pix; ++j) {
            if (x > 31) {
               fprintf(stderr, "Shouldn't come here\n");
               ++size;
               x = 0;
               --y;
            }
            if (i >= cel->size) {
               fprintf(stderr, "too few data for cel (type 1) c !\n");
               return 1;
            }
            ++size;
            ++x;
            ++i;
         }
      }
      if (x > 31) {
         ++size;
         x = 0;
         --y;
      }
   }
   cel->spr = (RLE_SPRITE *)malloc(sizeof(RLE_SPRITE) + size);
   if (cel->spr == NULL) {
      fprintf(stderr, "Can't alloc memory\n");
      return 1;
   }
   cel->spr->w = 0x20;
   cel->spr->h = 0x20;
   cel->spr->color_depth = 8;
   c = 0;
   for (y = 0; y < 0x20; ++y) {
      x = 0;
      i = line_offset[y];
      while (x < 0x20) {
         if (cel->data[i] >= 0x80) {
            nb_pix = 0x100 - cel->data[i];
            x += nb_pix;
            cel->spr->dat[c] = cel->data[i];
            ++c;
            ++i;
         }
         else {
            nb_pix = cel->data[i];
            cel->spr->dat[c] = cel->data[i];
            ++c;
            ++i;
            for (j = 0; j < nb_pix; ++j) {
               if (x >= 0x20) {
                  fprintf(stderr, "Shouldn't come here\n");
                  break;
               }
               cel->spr->dat[c] = cel->data[i];
               ++c;
               ++x;
               ++i;
            }
         }
      }
      cel->spr->dat[c] = 0x00;
      ++c;
   }
   cel->spr->size = c;
   return 0;
}

int decode_tile_2(CEL *cel) {
   int i, x, y, size;
   int line_size[32]  = { 0, 2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30, 32, 30, 28, 26, 24, 22, 20, 18, 16, 14, 12, 10, 8, 6, 4, 2 };
   int cel_offset[32] = { 0x220, 0x21E, 0x218, 0x212, 0x208, 0x1FE, 0x1F0, 0x1E2, 0x1D0, 0x1BE, 0x1A8, 0x192, 0x178, 0x15E, 0x140, 0x122, 0x100, 0x0E2, 0x0C4, 0x0AA, 0x090, 0x07A, 0x064, 0x052, 0x040, 0x032, 0x024, 0x01A, 0x010, 0x00A, 0x004, 0x002 };
/*   int cel_offset[32] = { 0x220, 0x21C, 0x218, 0x210, 0x208, 0x1FC, 0x1F0, 0x1E0, 0x1D0, 0x1BC, 0x1A8, 0x190, 0x178, 0x15C, 0x140, 0x120, 0x100, 0x0E0, 0x0C4, 0x0A8, 0x090, 0x078, 0x064, 0x050, 0x040, 0x030, 0x024, 0x018, 0x010, 0x008, 0x004, 0x000 };*/

   if (cel->size != 0x220) {
      fprintf(stderr, "Size not handled 2 : %lu\n", cel->size);
      return 1;
   }
   size = 606;
   cel->spr = (RLE_SPRITE *)malloc(sizeof(RLE_SPRITE) + size);
   if (cel->spr == NULL) {
      fprintf(stderr, "Can't alloc memory\n");
      return 1;
   }
   cel->spr->w = 0x20;
   cel->spr->h = 0x20;
   cel->spr->color_depth = 8;
   i = 0;
   cel->spr->dat[i] = -0x20;
   ++i;
   cel->spr->dat[i] = 0x00;
   ++i;
   for (y = 1; y < 0x20; ++y) {
      if (line_size[y] < 0x20) {
         cel->spr->dat[i] = line_size[y] - 0x20;
         ++i;
      }
      cel->spr->dat[i] = line_size[y];
      ++i;
      for (x = 0; x < line_size[y]; ++x) {
         cel->spr->dat[i] = cel->data[cel_offset[y] + x];
         ++i;
      }
      cel->spr->dat[i] = 0;
      ++i;
   }
   cel->spr->size = i;
   return 0;
}

int decode_tile_3(CEL *cel) {
   int i, x, y, size;
   int line_size[32]  = { 0, 2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30, 32, 30, 28, 26, 24, 22, 20, 18, 16, 14, 12, 10, 8, 6, 4, 2 };
   int cel_offset[32] = { 0x220, 0x21C, 0x218, 0x210, 0x208, 0x1FC, 0x1F0, 0x1E0, 0x1D0, 0x1BC, 0x1A8, 0x190, 0x178, 0x15C, 0x140, 0x120, 0x100, 0x0E0, 0x0C4, 0x0A8, 0x090, 0x078, 0x064, 0x050, 0x040, 0x030, 0x024, 0x018, 0x010, 0x008, 0x004, 0x000 };

   size = 576;
   if (cel->size != 0x220) {
      fprintf(stderr, "Size not handled 3 : %lu\n", cel->size);
      return 1;
   }
   cel->spr = (RLE_SPRITE *)malloc(sizeof(RLE_SPRITE) + size);
   if (cel->spr == NULL) {
      fprintf(stderr, "Can't alloc memory\n");
      return 1;
   }
   cel->spr->w = 0x20;
   cel->spr->h = 0x20;
   cel->spr->color_depth = 8;
   i = 0;
   cel->spr->dat[i] = -0x20;
   ++i;
   cel->spr->dat[i] = 0x00;
   ++i;
   for (y = 1; y < 0x20; ++y) {
      cel->spr->dat[i] = line_size[y];
      ++i;
      for (x = 0; x < line_size[y]; ++x) {
         cel->spr->dat[i] = cel->data[cel_offset[y] + x];
         ++i;
      }
      cel->spr->dat[i] = 0;
      ++i;
   }
   cel->spr->size = i;
   return 0;
}

int decode_tile_4(CEL *cel) {
   int i, x, y, size;
   int line_size[32] = { 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 30, 28, 26, 24, 22, 20, 18, 16, 14, 12, 10, 8, 6, 4, 2 };
   int cel_offset[32] = { 0x300, 0x2E0, 0x2C0, 0x2A0, 0x280, 0x260, 0x240, 0x220, 0x200, 0x1E0, 0x1C0, 0x1A0, 0x180, 0x160, 0x140, 0x120, 0x100, 0x0E2, 0x0C4, 0x0AA, 0x090, 0x07A, 0x064, 0x052, 0x040, 0x032, 0x024, 0x01A, 0x010, 0x00A, 0x004, 0x002 };

   if (cel->size != 0x320) {
      fprintf(stderr, "Size not handled 4 : %lu\n", cel->size);
      return 1;
   }
   size = 0x320 + 0x60;
   cel->spr = (RLE_SPRITE *)malloc(sizeof(RLE_SPRITE) + size);
   if (cel->spr == NULL) {
      fprintf(stderr, "Can't alloc memory\n");
      return 1;
   }
   cel->spr->w = 0x20;
   cel->spr->h = 0x20;
   cel->spr->color_depth = 8;
   i = 0;
   for (y = 0; y < 0x20; ++y) {
      if (line_size[y] < 0x20) {
         cel->spr->dat[i] = 0xE0 + line_size[y];
         ++i;
      }
      cel->spr->dat[i] = line_size[y];
      ++i;
      for (x = 0; x < line_size[y]; ++x) {
         cel->spr->dat[i] = cel->data[cel_offset[y] + x];
         ++i;
      }
      cel->spr->dat[i] = 0x00;
      ++i;
   }
   cel->spr->size = i;
   return 0;
}

int decode_tile_5(CEL *cel) {
   int i, x, y, size;
   int line_size[32] = { 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 30, 28, 26, 24, 22, 20, 18, 16, 14, 12, 10, 8, 6, 4, 2 };
   int cel_offset[32] = { 0x300, 0x2E0, 0x2C0, 0x2A0, 0x280, 0x260, 0x240, 0x220, 0x200, 0x1E0, 0x1C0, 0x1A0, 0x180, 0x160, 0x140, 0x120, 0x100, 0x0E0, 0x0C4, 0x0A8, 0x090, 0x078, 0x064, 0x050, 0x040, 0x030, 0x024, 0x018, 0x010, 0x008, 0x004, 0x000 };

   if (cel->size != 0x320) {
      fprintf(stderr, "Size not handled 5 : %lu\n", cel->size);
      return 1;
   }
   size = 0x320 + 0x60;
   cel->spr = (RLE_SPRITE *)malloc(sizeof(RLE_SPRITE) + size);
   if (cel->spr == NULL) {
      fprintf(stderr, "Can't alloc memory\n");
      return 1;
   }
   cel->spr->w = 0x20;
   cel->spr->h = 0x20;
   cel->spr->color_depth = 8;
   i = 0;
   for (y = 0; y < 0x20; ++y) {
      cel->spr->dat[i] = line_size[y];
      ++i;
      for (x = 0; x < line_size[y]; ++x) {
         cel->spr->dat[i] = cel->data[cel_offset[y] + x];
         ++i;
      }
      if (line_size[y] < 0x20) {
         cel->spr->dat[i] = 0xE0 + line_size[y];
         ++i;
      }
      cel->spr->dat[i] = 0;
      ++i;
   }
   cel->spr->size = i;
   return 0;
}

int decode_header_cel(CEL *cel, unsigned char *trn) {
   int width, height;
   unsigned short int offset[5];
   int x, y, nb_pix, i, j, size, c;
   int line_offset[160];

   if (cel == NULL) {
      fprintf(stderr, "cel == NULL\n");
      return 1;
   }
   if (cel->size == 0) {
      fprintf(stderr, "cel->size == 0\n");
      return 1;
   }
   if (cel->size < 10) {
      fprintf(stderr, "cel->size < 10\n");
      return 1;
   }
   if (cel->data == NULL) {
      fprintf(stderr, "cel->data == NULL\n");
      return 1;
   }
   cel->spr = NULL;
   if (trn == NULL)
      trn = trn_dummy;
   for (i = 0; i < 5; ++i)
      offset[i] = *((unsigned short int *)(cel->data + 2 * i));
   width = 0;
   for (i = offset[0]; i < offset[1]; ++i) {
      if (cel->data[i] >= 0x80)
         width += 0x100 - cel->data[i];
      else {
         width += cel->data[i];
         i += cel->data[i];
      }
   }
   if ((i != offset[1]) && (offset[1] != 0)) {
      fprintf(stderr, "offset 1 non valable\n");
      return 1;
   }
   if (width & 31) {
      fprintf(stderr, "bad number of pixels for 0x20 lines\n");
      return 1;
   }
   width >>= 5;
   height = 0;
   for (i = 0; i < 5; ++i)
      if (offset[i] != 0)
         height += 32;
   i = offset[0];
   x = 0;
   size = 0;
   y = height - 1;
   while (y >= 0) {
      if (x == 0) {
         line_offset[y] = i;
      }
      if (i >= cel->size) {
         fprintf(stderr, "too few data for cel\n");
         return 1;
      }
      if (cel->data[i] >= 0x80) {
         nb_pix = 0x100 - cel->data[i];
         x += nb_pix;
         ++size;
         ++i;
      }
      else {
         nb_pix = cel->data[i];
         ++size;
         ++i;
         for (j = 0; j < nb_pix; ++j) {
            if (i >= cel->size) {
               fprintf(stderr, "too few data for cel /\n");
               return 1;
            }
            ++size;
            ++i;
            ++x;
            if (x >= width) {
               ++size;
               x = 0;
               --y;
            }
         }
      }
      if (x >= width) {
         ++size;
         x = 0;
         --y;
      }
   }
   cel->spr = malloc(sizeof(RLE_SPRITE) + size);
   if (cel->spr == NULL) {
      fprintf(stderr, "Can't alloc memory\n");
      return 1;
   }
   cel->spr->w = width;
   cel->spr->h = height;
   cel->spr->color_depth = 8;
   c = 0;
   for (y = 0; y < height; ++y) {
      x = 0;
      i = line_offset[y];
      while (x < width) {
         if (cel->data[i] >= 0x80) {
            nb_pix = 0x100 - cel->data[i];
            x += nb_pix;
            cel->spr->dat[c] = cel->data[i];
            ++c;
            ++i;
         }
         else {
            nb_pix = cel->data[i];
            cel->spr->dat[c] = cel->data[i];
            ++c;
            ++i;
            for (j = 0; j < nb_pix; ++j) {
               cel->spr->dat[c] = trn[cel->data[i]];
               ++c;
               ++x;
               ++i;
               if (x >= width) {
                  break;
               }
            }
         }
      }
      cel->spr->dat[c] = 0x00;
      ++c;
   }
   cel->spr->size = c;
   return 0;
}

int put_image_anim(BITMAP *bmp, ANIM *anim, int frame, int x, int y, int selected) {
   if (anim == NULL) {
      fprintf(stderr, "anim == NULL\n");
      return 1;
   }
   if (anim->data == NULL) {
      fprintf(stderr, "anim->data == NULL\n");
      return 1;
   }
   if (anim->frame == NULL) {
      fprintf(stderr, "anim->frame == NULL\n");
      return 1;
   }
   if ((frame < 0) || (frame >= anim->nb)) {
      fprintf(stderr, "invalid frame number : %d\n", frame);
      return 1;
   }
   if (anim->frame[frame].spr == NULL)
      return 1;
   if (selected)
      draw_lit_rle_sprite(bmp, anim->frame[frame].spr, x, y - anim->frame[frame].spr->h, 127);
   else
      draw_rle_sprite(bmp, anim->frame[frame].spr, x, y - anim->frame[frame].spr->h);
   return 0;
}

int put_image_multi_anim(BITMAP *bmp, MULTI_ANIM *anim, int dir, int frame, int x, int y, int selected) {
   if (anim == NULL) {
      fprintf(stderr, "anim == NULL\n");
      return 1;
   }
   if (anim->data == NULL) {
      fprintf(stderr, "anim->data == NULL\n");
      return 1;
   }
   if (anim->frame == NULL) {
      fprintf(stderr, "anim->frame == NULL\n");
      return 1;
   }
   if ((dir < 0) || (dir >= anim->nb)) {
      fprintf(stderr, "invalid dir number : %d\n", dir);
      return 1;
   }
   if (anim->frame[dir] == NULL) {
      fprintf(stderr, "anim->frame[dir] == NULL\n");
      return 1;
   }
   if ((frame < 0) || (frame >= anim->sub_nb[dir])) {
      fprintf(stderr, "invalid frame number : %d\n", frame);
      return 1;
   }
   if (anim->frame[dir][frame].spr == NULL) {
      return 1;
   }
   if (selected)
      draw_lit_rle_sprite(bmp, anim->frame[dir][frame].spr, x, y - anim->frame[dir][frame].spr->h, 127);
   else
      draw_rle_sprite(bmp, anim->frame[dir][frame].spr, x, y - anim->frame[dir][frame].spr->h);
   return 0;
}

int decode_header_cl2(CEL *cel, unsigned char *trn) {
   unsigned char val;
   int i, j, count;
   int width, height, x, y;
   unsigned short int offset[5];
   int size, c;
   short int *tmpdata;

   if (cel == NULL) {
      fprintf(stdout, "cel == NULL\n");
      return 1;
   }
   if (cel->size == 0) {
      fprintf(stdout, "cel->size == 0\n");
      return 1;
   }
   if (cel->size < 10) {
      fprintf(stdout, "cel->size < 10\n");
      return 1;
   }
   if (cel->data == NULL) {
      fprintf(stdout, "cel->data == NULL\n");
      return 1;
   }
   if (trn == NULL)
      trn = trn_dummy;
   for (i = 0; i < 5; ++i)
      offset[i] = *((unsigned short int *)(cel->data + 2 * i));
   width = 0;
   i = offset[0];
   while (i < offset[1]) {
      val = cel->data[i];
      if ((val > 0) && (val < 0x80)) {
         width += val;
         ++i;
      }
      else if ((val > 0x7F) && (val < 0xC0)) {
         width += 0xBF - val;
         i += 2;
      }
      else {
         width += 0x100 - val;
         i += 0x100 - val + 1;
      }
   }
   if ((i != offset[1]) && (offset[1] != 0)) {
      fprintf(stdout, "nb pixels : %d\n", width);
      fprintf(stderr, "offset 1 non valable\n");
      return 1;
   }
   if (width & 31) {
      fprintf(stderr, "bad number of pixels for 0x20 lines : %d\n", width);
      return 1;
   }
   width >>= 5;
   height = 0;
   for (i = 0; i < 5; ++i)
      if (offset[i] != 0)
         height += 32;
   tmpdata = (short int *)malloc(2 * height * width);
   if (tmpdata == NULL) {
      fprintf(stderr, "out of memory\n");
      return 1;
   }
   i = 10;
   x = 0;
   y = height - 1;
   while (i < cel->size) {
      if ((cel->data[i] < 0x80) && (cel->data[i] > 0)) {
         for (j = 0; j < cel->data[i]; ++j) {
            tmpdata[y * width + x] = -1;
            ++x;
            if (x >= width) {
               x = 0;
               --y;
            }
         }
         ++i;
      }
      else if ((cel->data[i] > 0x7F) && (cel->data[i] < 0xC0)) {
         count = 0xBF - cel->data[i];
         ++i;
         for (j = 0; j < count; ++j) {
            tmpdata[y * width + x] = cel->data[i];
            ++x;
            if (x >= width) {
               x = 0;
               --y;
            }
         }
         ++i;
      }
      else {
         count = 0x100 - cel->data[i];
         ++i;
         for (j = 0; j < count; ++j) {
            tmpdata[y * width + x] = cel->data[i++];
            ++x;
            if (x >= width) {
               x = 0;
               --y;
            }
         }
      }
   }
   size = 0;
   for (i = 0; i < height; ++i) {
      count = 0;
      for (j = 0; j < width; ++j) {
         if (tmpdata[i * width + j] == -1) {
            if (count > 0) {
               size += 1 + count;
               count = -1;
            }
            else {
               count -= 1;
               if (count == -128) {
                  ++size;
                  count = 0;
               }
            }
         }
         else {
            if (count < 0) {
               ++size;
               count = 1;
            }
            else {
               count += 1;
               if (count == 127) {
                  size += 128;
                  count = 0;
               }
            }
         }
      }
      if (count > 0) {
         size += 1 + count;
      }
      else {
         ++size;
      }
      ++size;
   }
   cel->spr = (RLE_SPRITE *)malloc(sizeof(RLE_SPRITE) + size);
   if (cel->spr == NULL) {
      fprintf(stderr, "Can't alloc memory\n");
      return 1;
   }
   cel->spr->w = width;
   cel->spr->h = height;
   cel->spr->color_depth = 8;
   c = 0;
   for (i = 0; i < height; ++i) {
      count = 0;
      for (j = 0; j < width; ++j) {
         if (tmpdata[i * width + j] == -1) {
            if (count > 0) {
               cel->spr->dat[c] = count;
               ++c;
               for (x = j - count; x < j; ++x) {
                  cel->spr->dat[c] = trn[tmpdata[i * width + x]];
                  ++c;
               }
               count = -1;
            }
            else {
               count -= 1;
               if (count == -128) {
                  cel->spr->dat[c] = count;
                  ++c;
                  count = 0;
               }
            }
         }
         else {
            if (count < 0) {
               cel->spr->dat[c] = count;
               ++c;
               count = 1;
            }
            else {
               count += 1;
               if (count == 127) {
                  cel->spr->dat[c] = count;
                  ++c;
                  for (x = j - count; x < j; ++x) {
                     cel->spr->dat[c] = trn[tmpdata[i * width + x]];
                     ++c;
                  }
                  count = 0;
               }
            }
         }
      }
      if (count > 0) {
         cel->spr->dat[c] = count;
         ++c;
         for (x = j - count; x < j; ++x) {
            cel->spr->dat[c] = trn[tmpdata[i * width + x]];
            ++c;
         }
         count = 0;
      }
      else if (count < 0) {
         cel->spr->dat[c] = count;
         ++c;
         count = 0;
      }
      cel->spr->dat[c] = 0x00;
      ++c;
   }
   cel->spr->size = c;
   free(tmpdata);
   return 0;
}

/* void decode_usual_cel(CEL *cel, unsigned char *trn) {
int decode_usual_cel(CEL *cel) {
   unsigned long int i, j, nb_pix;
   int not_done;
   long int width, height, x, y;

   if (cel == NULL)
      return NULL;
   if (cel->size == 0)
      return NULL;
   if (cel->data == NULL)
      return NULL;
   i = 0;
   width = 0;
   not_done = 1;
   while (not_done) {
      if (i >= cel->size) {
         fprintf(stderr, "Reading outside of data\n");
         return 1;
      }
      if (cel->data[i] <= 0x10) {
         fprintf(stderr, "Can't read command : %u\n", cel->data[i]);
         return 1;
      }
      else
      if (cel->data[i] == 0x7F) {
         width += cel->data[0];
         i += 0x7F;
      }
      else {
         if (cel->data[i] >= 0x80)
            width += 0x100 - cel->data[i];
         else
            width += cel->data[i];
         not_done = 0;
      }
   }
   height = 0;
   i = 0;
   x = 0;
   while (i < cel->size) {
      if (cel->data[i] <= 0x10) {
         fprintf(stderr, "Can't read command : %u\n", cel->data[i]);
         return 1;
      }
      else
      if (cel->data[i] >= 0x80) {
         x += 0x100 - cel->data[i];
         ++i;
      }
      else {
         x += cel->data[i];
         i += cel->data[i] + 1;
      }
      if (x > width) {
         fprintf(stderr, "Pixel outside of line\n");
         return 1;
      }
      if (x >= width) {
         x = 0;
         ++height;
      }
   }
   if (x > 0) {
      fprintf(stderr, "Line not finished\n");
      return 1;
   }
   if (i != cel->size) {
      fprintf(stderr, "Badly formated data\n");
      return 1;
   }
   cel->bmp = create_image(width, height);
   if (cel->bmp == NULL) {
      return 1;
   }
   i = 0;
   x = 0;
   y = height - 1;
   while (i < cel->size) {
      if (cel->data[i] <= 0x10) {
         fprintf(stderr, "Can't read command : %u\n", cel->data[i]);
         destroy_bitmap(bmp);
         return 1;
      }
      else
      if (cel->data[i] >= 0x80) {
         nb_pix = 0x100 - cel->data[i];
         x += nb_pix;
         ++i;
      }
      else {
         nb_pix = cel->data[i++];
         for (j = 0; j < nb_pix; ++j) {
            putpixel(cel->bmp, x++, y, cel->data[i++]);
         }
      }
      if (x >= width) {
         x = 0;
         --y;
      }
   }
   return 0;
}
*/

int read_palette(char *filename, PALETTE pal) {
   unsigned char *buff;
   long int size;
   int i, j;

   if (pal == NULL)
      return 0;
   memset(pal, 0, sizeof(PALETTE));
   misc_load_mpq_file(filename, &buff, &size, config.debug);
   if (buff == NULL) {
      return 0;
   }
   if (size != 0x300) {
      fprintf(stderr, "Unusal palette size\n");
      free(buff);
      return 0;
   }
   j = 0;
   for (i = 0; i < 0x100; ++i) {
      pal[i].r = buff[j++] >> 2;
      pal[i].g = buff[j++] >> 2;
      pal[i].b = buff[j++] >> 2;
   }
   free(buff);
   fprintf(stderr, "%s loaded\n", filename);
   return 1;
}
