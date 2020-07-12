#ifndef _IMAGE_H_
#define _IMAGE_H_

#include <allegro.h>

typedef struct {
   unsigned long int size;
   unsigned char *data;
   RLE_SPRITE *spr;
} CEL;

typedef struct {
   unsigned char *data;
   unsigned long int nb;
   CEL *frame;
} ANIM;

typedef struct {
   unsigned char *data;
   unsigned long int nb;
   unsigned long int *sub_nb;
   CEL **frame;
   int cl2;
} MULTI_ANIM;

typedef struct {
   unsigned long int nb;
   CEL **cel;
} CEL_FILE;

CEL_FILE *read_cel_file(char *);
ANIM *load_anim(char *);
MULTI_ANIM *load_multi_anim(char *, int);
int destroy_cel_file(CEL_FILE *);
int destroy_anim(ANIM *);
int destroy_multi_anim(MULTI_ANIM *);

int read_palette(char *, PALETTE);
int decode_tile_0(CEL *);
int decode_tile_1(CEL *);
int decode_tile_2(CEL *);
int decode_tile_3(CEL *);
int decode_tile_4(CEL *);
int decode_tile_5(CEL *);
int decode_header_cel(CEL *, unsigned char *);
int decode_header_cl2(CEL *, unsigned char *);
int put_image_anim(BITMAP *, ANIM *, int, int, int, int);
int put_image_multi_anim(BITMAP *, MULTI_ANIM *, int, int, int, int, int);
/* int decode_usual_cel(CEL *, unsigned char *);*/
int put_digit(BITMAP *, int, int, int);

#endif /* _IMAGE_H_ */
