#ifndef _GLOBAL_ALLEGRO_H_
#define _GLOBAL_ALLEGRO_H_

#include <allegro.h>

typedef enum {
   GC_060, GC_062, GC_064, GC_066, GC_068, GC_070, GC_072,
   GC_074, GC_076, GC_078, GC_080, GC_082, GC_084, GC_086,
   GC_088, GC_090, GC_092, GC_094, GC_096, GC_098, GC_100,
   GC_110, GC_120, GC_130, GC_140, GC_150, GC_160, GC_170,
   GC_180, GC_190, GC_200, GC_210, GC_220, GC_230, GC_240,
   GC_250, GC_260, GC_270, GC_280, GC_290, GC_300, GC_MAX
} GC_ENUM;

typedef enum {
   GV_NONE,
   GV_D100, GV_D101, GV_D102, GV_D103, GV_D104, GV_D105, GV_D106, GV_D107, GV_D108, GV_D109,
   GV_H100, GV_H101,
   GV_CUSTOM,
   GV_MAX
} GV_ENUM;

typedef struct {
   int screen_width;
   int screen_height;
   int screen_depth;
   int screen_refresh;
   int fullscreen;
   int debug;
   int bg_red;
   int bg_green;
   int bg_blue;
   unsigned char pal[0x100][3];
   PALETTE the_pal;
   unsigned char gamma_table[GC_MAX][256];
   int cur_gamma;
   BITMAP *my_screen;
   char game_exe[300];
   unsigned long int code_offset;
   GV_ENUM game_version;
} CONFIG_S;

extern COLOR_MAP my_color_map;

unsigned char pal[256][3];

extern char gamma_str[GC_MAX][5];
extern CONFIG_S config;

int test_version();
int set_monster_offsets();
int ini_read(char *);
int ini_create(char *);
int read_gamma();
void pal_d2_2_vga();
int load_palette(char *);
int global_init();
int control_create_working_dir(int);

#endif /* _GLOBAL_ALLEGRO_H_ */
