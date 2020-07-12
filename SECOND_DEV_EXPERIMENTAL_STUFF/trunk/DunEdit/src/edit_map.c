/* Diablo/Hellfire *.DUN editor
havily inspired by the DS1_EDIT of Paul Siramy
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <allegro.h>
#include "mpq_lib.h"
#include "global_allegro.h"
#include "map.h"
#include "dun.h"
#include "write.h"
#include "monster.h"

#define SPACE_MAX 32
#define TXT_HEIGHT 40
#define TILE_PER_LINE 16
#define MONSTER_PER_LINE 8

BITMAP *tiles, *monsters;

void help() {
clear(screen);
textout(screen, font, "No mouse at the moment...", 10, 20 , 255);
textout(screen, font, "Move the window with arrows, or Shift-Arrow to go faster", 10, 40 , 255);
textout(screen, font, "Move cursor with 1/3/7/9 of the numeric pad", 10, 60 , 255);
textout(screen, font, "Select area with Shift-1..Shift-9 of the numeric pad", 10, 80 , 255);
textout(screen, font, "Change palette with 1/2/3/4/5/6/7/8", 10, 100, 255);
textout(screen, font, "Change display flags with F1..F5", 10, 120, 255);
textout(screen, font, "Change gamma with F12/Shift-F12", 10, 140, 255);
textout(screen, font, "Zomm in/out with =/- keys (qwerty keybord)", 10, 160, 255);
textout(screen, font, "Change spacing between tiles with a/z (qwerty keybord)", 10, 180, 255);
textout(screen, font, "Change mode of editing with tab (edit the tile, or one of the extra layers,)", 10, 200, 255);
textout(screen, font, "including monsters)", 50, 210, 255);
textout(screen, font, "Change what is under the cursor with Enter, then choose with arrow,)", 10, 230, 255);
textout(screen, font, "valid/quit with Enter/Esc", 50, 240, 255);
textout(screen, font, "Copy/cut/paste with CTRL-C/CTRL-X/CTRL-V (works in map mode)", 10, 260, 255);
textout(screen, font, "and in monster mode)", 50, 270, 255);
textout(screen, font, "Quit with Esc", 10, 290, 255);
textout(screen, font, "Snapshot with P", 10, 310, 255);
textout(screen, font, "Center map with Space", 10, 330, 255);
textout(screen, font, "Save with CTRL-S", 10, 350, 255);
textout(screen, font, "H for this help ;)", 10, 370, 255);
textout(screen, font, "All code is heavily inspired by Paul Siramy's ones", 10, 410, 255);
textout(screen, font, "Visit his Diablo 2 page at http://paul.siramy.free.fr/", 10, 420, 255);
while (!key[KEY_SPACE]);
while (key[KEY_SPACE]);
}

int choose_value(unsigned short int val) {
   unsigned short int old = val;
   int done, w, redraw;
   char *msg = "Change value :", tmp[10];

   w = text_length(font, msg);
   done = 0;
   redraw = 1;
   while (!done) {
      if (redraw) {
         vsync();
         rectfill(screen, config.screen_width / 2 - w / 2 - 10, config.screen_height / 2 - 25, config.screen_width / 2 + w / 2 + 10, config.screen_height / 2 + 30, 0);
         rect(screen, config.screen_width / 2 - w / 2 - 10, config.screen_height / 2 - 25, config.screen_width / 2 + w / 2 + 10, config.screen_height / 2 + 30, 255);
         textout(screen, font, msg, config.screen_width / 2 - text_length(font, msg) / 2, config.screen_height / 2 - 15, 255);
         sprintf(tmp, "%04X", val);
         textout(screen, font, tmp, config.screen_width / 2 - text_length(font, tmp) / 2, config.screen_height / 2 + 15, 255);
         redraw = 0;
      }
      if (key[KEY_RIGHT] || key [KEY_UP]) {
         while (key[KEY_RIGHT] || key [KEY_UP]);
         if (val < 0xFFFF)
            ++val;
         if (key[KEY_LSHIFT] || key[KEY_RSHIFT]) {
            if (val <= 0xFFF0)
               val += 0xF;
            else
               val = 0xFFFF;
         }
         redraw = 1;
      }
      if (key[KEY_LEFT] || key [KEY_DOWN]) {
         while (key[KEY_LEFT] || key [KEY_DOWN]);
         if (val > 0)
            --val;
         if (key[KEY_LSHIFT] || key[KEY_RSHIFT]) {
            if (val >= 0xF)
               val -= 0xF;
            else
               val = 0;
         }
         redraw = 1;
      }
      if (key[KEY_PGUP]) {
         while (key[KEY_PGUP]);
         if (val < 0xFF00)
            val += 0x100;
         if (key[KEY_LSHIFT] || key[KEY_RSHIFT]) {
            if (val <= 0xF0FF)
               val += 0xF00;
            else
               val = 0xFFFF;
         }
      }
      if (key[KEY_PGDN]) {
         while (key[KEY_PGDN]);
         if (val > 0x00FF)
            val -= 0x100;
         if (key[KEY_LSHIFT] || key[KEY_RSHIFT]) {
            if (val >= 0x0F00)
               val -= 0xF00;
            else
               val = 0;
         }
         redraw = 1;
      }
      if (key[KEY_ESC]) {
         while (key[KEY_ESC]);
         val = old;
         done = 1;
      }
      if (key[KEY_ENTER]) {
         while (key[KEY_ENTER]);
         done = 1;
      }
   }
   return val;
}

int choose_monster(int mon_idx) {
   int done, redraw, hsize, old_mon;
   BITMAP *subbmp;

   hsize = 192;
   subbmp = create_sub_bitmap(config.my_screen, config.screen_width - 3 * 225, 40, 3 * 224, config.my_screen->h);
   if (subbmp == NULL) {
      fprintf(stderr, "Cant create subbitmap...\n");
      return mon_idx;
   }
   --mon_idx;
   if (mon_idx < 0)
      mon_idx = glb_monster.nb_monster_index;
   redraw = 1;
   done = 0;
   old_mon = mon_idx;
   while (!done) {
      if (redraw) {
         clear(subbmp);
         blit(monsters, subbmp, ((mon_idx % MONSTER_PER_LINE) - 1) * 224 + 16, 32 + (mon_idx / MONSTER_PER_LINE) * hsize - (config.my_screen->h - hsize) / 2, 0, 0, monsters->w, monsters->h);
         vline(subbmp, 224, (config.my_screen->h - hsize) / 2, 192 + (config.my_screen->h - hsize) / 2, 255);
         vline(subbmp, 224 + 0xBF, (config.my_screen->h - hsize) / 2, 192 + (config.my_screen->h - hsize) / 2, 255);
         hline(subbmp, 224, (config.my_screen->h - hsize) / 2, 224 + 0xBF, 255);
         hline(subbmp, 224, (config.my_screen->h - hsize) / 2 + 192, 224 + 0xBF, 255);
         vsync();
         blit(config.my_screen, screen, 0, 0, 0, 0, config.screen_width, config.screen_height);
         redraw = 0;
      }
      if (key[KEY_UP]) {
         while(key[KEY_UP]);
         if (mon_idx >= MONSTER_PER_LINE) {
            mon_idx -= MONSTER_PER_LINE;
            redraw = 1;
         }
      }
      if (key[KEY_LEFT]) {
         while(key[KEY_LEFT]);
         if (mon_idx > 0) {
            --mon_idx;
            redraw = 1;
         }
      }
      if (key[KEY_RIGHT]) {
         while(key[KEY_RIGHT]);
         if (mon_idx < glb_monster.nb_monster_index) {
            ++mon_idx;
            redraw = 1;
         }
      }
      if (key[KEY_DOWN]) {
         while(key[KEY_DOWN]);
         if (mon_idx + MONSTER_PER_LINE <= glb_monster.nb_monster_index) {
            mon_idx += MONSTER_PER_LINE;
            redraw = 1;
         }
      }
      if (key[KEY_ENTER]) {
         while(key[KEY_ENTER]);
         done = 1;
      }
      if (key[KEY_ESC]) {
         while(key[KEY_ESC]);
         mon_idx = old_mon;
         done = 1;
      }
   }
   destroy_bitmap(subbmp);
   if (mon_idx < 0)
      mon_idx = -1;
   if (mon_idx >= glb_monster.nb_monster_index)
      mon_idx = -1;
   ++mon_idx;
   return mon_idx;
}

int choose_til(int til_index) {
   int done, redraw, hsize, old_til;
   BITMAP *subbmp;

   hsize = (glb_map.min_size / 2) * 32 + 64;
   subbmp = create_sub_bitmap(config.my_screen, config.screen_width - 3 * 161, 40, 3 * 160, config.my_screen->h);
   if (subbmp == NULL) {
      fprintf(stderr, "Cant create subbitmap...\n");
      return til_index;
   }
   --til_index;
   if (til_index < 0)
      til_index = glb_map.til->nb;
   old_til = til_index;
   redraw = 1;
   done = 0;
   while (!done) {
      if (redraw) {
         clear(subbmp);
         blit(tiles, subbmp, ((til_index % TILE_PER_LINE) - 1) * 160, (til_index / TILE_PER_LINE) * hsize - (config.my_screen->h - hsize) / 2, 0, 0, tiles->w, tiles->h);
         vline(subbmp, 192, 32 + (config.my_screen->h - hsize) / 2, (glb_map.min_size / 2) * 32 + 32 + (config.my_screen->h - hsize) / 2, 255);
         vline(subbmp, 192 + 127, 32 + (config.my_screen->h - hsize) / 2, (glb_map.min_size / 2) * 32 + 32 + (config.my_screen->h - hsize) / 2, 255);
         hline(subbmp, 192, (config.my_screen->h - hsize) / 2 + 32, 192 + 127, 255);
         hline(subbmp, 192, (config.my_screen->h - hsize) / 2 + (glb_map.min_size / 2) * 32 + 32, 192 + 127, 255);
         vsync();
         blit(config.my_screen, screen, 0, 0, 0, 0, config.screen_width, config.screen_height);
         redraw = 0;
      }
      if (key[KEY_UP]) {
         while(key[KEY_UP]);
         if (til_index >= TILE_PER_LINE) {
            til_index -= TILE_PER_LINE;
            redraw = 1;
         }
      }
      if (key[KEY_LEFT]) {
         while(key[KEY_LEFT]);
         if (til_index > 0) {
            --til_index;
            redraw = 1;
         }
      }
      if (key[KEY_RIGHT]) {
         while(key[KEY_RIGHT]);
         if (til_index < glb_map.til->nb) {
            ++til_index;
            redraw = 1;
         }
      }
      if (key[KEY_DOWN]) {
         while(key[KEY_DOWN]);
         if (til_index + TILE_PER_LINE <= glb_map.til->nb) {
            til_index += TILE_PER_LINE;
            redraw = 1;
         }
      }
      if (key[KEY_ENTER]) {
         while(key[KEY_ENTER]);
         done = 1;
      }
      if (key[KEY_ESC]) {
         while(key[KEY_ESC]);
         til_index = old_til;
         done = 1;
      }
   }
   destroy_bitmap(subbmp);
   if (til_index < 0)
      til_index = -1;
   if (til_index >= glb_map.til->nb)
      til_index = -1;
   ++til_index;
   return til_index;
}

int put_tile_sketch(BITMAP *level, int space, int xx, int yy, int j, int i, int zoom) {
   int x0, y0;

   x0 = (64 * j + space * j) - (64 * i + space * i);
   x0 = xx + x0 * 2 / zoom;
   y0 = (32 * j + space * j) + (32 * i + space * i);
   y0 = yy + y0 * 2 / zoom;
   line(level, x0 - 64 / zoom, y0, x0 + 62 / zoom, y0 + 62 / zoom, 255);
   line(level, x0 - 64 / zoom, y0, x0 + 62 / zoom, y0 - 62 / zoom, 255);
   line(level, x0 + 190 / zoom, y0, x0 + 64 / zoom, y0 + 62 / zoom, 255);
   line(level, x0 + 190 / zoom, y0, x0 + 64 / zoom, y0 - 62 / zoom, 255);
   return 0;
}

int put_subtile_sketch(BITMAP *level, int space, int xx, int yy, int j, int i, int zoom) {
   int x0, y0;

   x0 = (32 * j + space * (j / 2)) - (32 * i + space * (i / 2));
   x0 = xx + x0 * 2 / zoom;
   y0 = (16 * j + space * (j / 2)) + (16 * i + space * (i / 2));
   y0 = yy + y0 * 2 / zoom;
   line(level, x0 , y0 - 32 / zoom, x0 + 62 / zoom, y0 - 2 / zoom, 255);
   line(level, x0 , y0 - 32 / zoom, x0 + 62 / zoom, y0 - 62 / zoom, 255);
   line(level, x0 + 126 / zoom, y0 - 32 / zoom, x0 + 64 / zoom, y0 - 2 / zoom, 255);
   line(level, x0 + 126 / zoom, y0 - 32 / zoom, x0 + 64 / zoom, y0 - 62 / zoom, 255);
   return 0;
}

int ask_yes_no(char *msg) {
   int answer, w, k;
   char *yn = "Yes   No";

   w = text_length(font, msg);
   if (w < text_length(font, yn))
      w = text_length(font, yn);
   rectfill(screen, config.screen_width / 2 - w / 2 - 10, config.screen_height / 2 - 25, config.screen_width / 2 + w / 2 + 10, config.screen_height / 2 + 30, 0);
   rect(screen, config.screen_width / 2 - w / 2 - 10, config.screen_height / 2 - 25, config.screen_width / 2 + w / 2 + 10, config.screen_height / 2 + 30, 255);
   textout(screen, font, msg, config.screen_width / 2 - text_length(font, msg) / 2, config.screen_height / 2 - 15, 255);
   textout(screen, font, "Yes   No", config.screen_width / 2 - text_length(font, yn) / 2, config.screen_height / 2 + 15, 255);
   answer = -1;
   while (answer == -1) {
      k = readkey();
      if ((k >> 8) == KEY_Y)
         answer = 1;
      if ((k >> 8) == KEY_N)
         answer = 0;
   }
   return answer;
}

char *change_curr_palette(int act, int curr_pal) {
   char tmp[300];
   char palname[8][9][80] = {
      { "town.pal", "town.pal", "town.pal", "town.pal", "ltpalg.pal", "ltpalg.pal", "ltpalg.pal", "ltpalg.pal", "ltpalg.pal" },
      { "l1_1.pal", "l1_2.pal", "l1_3.pal", "l1_4.pal", "l1_5.pal", "l1.pal", "l1palg.pal", "l1palg.pal", "l1palg.pal" },
      { "l2_1.pal", "l2_2.pal", "l2_3.pal", "l2_4.pal", "l2_5.pal", "l2.pal", "l2palg.pal", "l2palg.pal", "l2palg.pal" },
      { "l3_1.pal", "l3_2.pal", "l3_3.pal", "l3_4.pal", "l3pfoul.pal", "l3pwater.pal", "l3_w.pal", "l3_i.pal", "l3.pal" },
      { "l4_1.pal", "l4_2.pal", "l4_3.pal", "l4_4.pal", "l4_4.pal", "l4_4.pal", "l4_4.pal", "l4_4.pal", "l4_4.pal" },
      { "l5base.pal", "l5base.pal", "l5base.pal", "l5base.pal", "l5base.pal", "l5base.pal", "l5base.pal", "l5base.pal", "l5base.pal" },
      { "l6base1.pal", "l6base2.pal", "l6base3.pal", "l6base4.pal", "l6base5.pal", "l6base.pal", "l6base.pal", "l6base.pal", "l6base.pal" },
      { "town.pal", "town.pal", "town.pal", "town.pal", "ltpalg.pal", "ltpalg.pal", "ltpalg.pal", "ltpalg.pal", "ltpalg.pal" }
   };

   sprintf(tmp, "%s\\%s", glb_map.path, palname[act][curr_pal]);
   load_palette(tmp);
   pal_d2_2_vga();
   set_palette(config.the_pal);
   create_light_table(&my_color_map, config.the_pal, 63, 63, 63, NULL);
   return palname[act][curr_pal];
}

int main(int argc, char * argv[]) {
   char tmp[80];
   char dun_name[100];
   int done = 0, scr=0, plotx, ploty, act, redraw, redraw_dun, space = 0, zoom = 2, selected = 0, modified = 0,
       display_mask = DISPLAY_MON | DISPLAY_ARCH | DISPLAY_WALL, cursor_x, cursor_y, select_x, select_y;
   enum {MODE_MAP, MODE_MONSTER, MODE_EXTRA, MODE_MAX} mode = MODE_MAP;
   BITMAP *level, *tmpbmp;
   char *pal_name;
   DUN *dun, *buffer;
   int buff_x, buff_y, buff_w, buff_h, i, j;
   int mon_buffer = -1, cur_layer = 0;

   if (argc < 3) {
      fprintf(stderr, "syntax : %s dun_filename area_number\narea = 0 town / 1..6 l1..L6 / 7 hellfire town\n", argv[0]);
      return 1;
   }
   act = argv[2][0] - '0';
   memset(&glb_monster, 0, sizeof(glb_monster));
   memset(&config, 0, sizeof(config));
   if (allegro_init()) {
      fprintf(stderr, "Cant start allegro !\n");
      return 1;
   }
   install_keyboard();
   set_color_depth(8);
   if (file_exists("dun_edit.ini", -1, NULL) == 0) {
      ini_create("dun_edit.ini");
      return 1;
   }
   if (ini_read("dun_edit.ini"))
      return 1;
   set_monster_offsets();
   if (control_create_working_dir(act)) {
      return 1;
   }
   mpq_lib_start(config.debug);
   color_map = &my_color_map;

/*   atexit(dt1debug_exit);*/
   read_gamma();
   set_color_depth(config.screen_depth);
   request_refresh_rate(config.screen_refresh);
   if (config.fullscreen == 1) {
      if (set_gfx_mode(GFX_AUTODETECT_FULLSCREEN, config.screen_width, config.screen_height, 0, 0)) {
         fprintf(stderr, allegro_error);
         return 1;
      }
   }
   else {
      if (set_gfx_mode(GFX_AUTODETECT_WINDOWED, config.screen_width, config.screen_height, 0, 0)) {
         fprintf(stderr, allegro_error);
         return 1;
      }
   }
   set_color_depth(8);   /* get back to a 8bpp color depth, for futur BITMAP creation */
   if (init_map(act)) {
      fprintf(stderr, "cant init map\n");
      mpq_lib_stop(config.debug);
      return 1;
   }
   sprintf(dun_name, "%s\\%s", glb_map.path, argv[1]);
   dun = read_dun(dun_name);
   if (dun == NULL) {
      fprintf(stderr, "cant read dun\n");
      free_map();
      mpq_lib_stop(config.debug);
      return 1;
   }
   if (dun->nb_layer > 1) {
      read_monster();
      if (glb_monster.nb_monster > 0) {
         read_monster_index();
         if (glb_monster.nb_monster_index > 0) {
            monsters = export_monster(MONSTER_PER_LINE);
         }
      }
   }
   else {
      glb_monster.nb_monster = 0;
      glb_monster.nb_monster_index = 0;
   }

   sprintf(dun_name, "%s\\%s\\%s", mod_dir[0], glb_map.path, argv[1]);
   buffer = NULL;
   pal_name = change_curr_palette(act, 0);
   read_gamma();
   pal_d2_2_vga();
   set_palette(config.the_pal);
   config.my_screen = create_bitmap(config.screen_width, config.screen_height);
   if (config.my_screen == NULL) {
      fprintf(stderr, "Cant create bitmap for screen\n");
      free_map();
      free_monster();
      mpq_lib_stop(config.debug);
      return 1;
   }
   tmpbmp = create_sub_bitmap(config.my_screen, 20, 40, config.screen_width - 40, config.screen_height - 80);
   if (tmpbmp == NULL) {
      fprintf(stderr, "Cant create bitmap for display\n");
      destroy_bitmap(config.my_screen);
      free_map();
      free_monster();
      mpq_lib_stop(config.debug);
      return 1;
   }
   level = aff_dun(dun, display_mask, SPACE_MAX);
   if (level == NULL) {
      fprintf(stderr, "Cant create bitmap for dungeon\n");
      destroy_bitmap(tmpbmp);
      destroy_bitmap(config.my_screen);
      free_map();
      free_monster();
      mpq_lib_stop(config.debug);
      return 1;
   }
   plotx = level->w / 2;
   ploty = level->h / 2;
   plotx -= (plotx * SPACE_MAX) / (64 + SPACE_MAX);
   ploty -= (ploty * SPACE_MAX) / (32 + SPACE_MAX);
   cursor_x = dun->w / 2;
   cursor_y = dun->h / 2;
   select_x = dun->w / 2;
   select_y = dun->h / 2;
   redraw = 1;
   redraw_dun = 1;
   tiles = export_til(DISPLAY_WALL | DISPLAY_ARCH, TILE_PER_LINE);
   while ( ! done) {
      if ((redraw) || (redraw_dun)) {
         if (redraw_dun) {
            clear(level);
            if (selected)
               put_dun_with_select(level, dun, display_mask, space, 32 + (64 + space) * (dun->h - 1), (32 + space) * (glb_map.min_size / 2)- 1, select_x, select_y, cursor_x, cursor_y);
            else
               put_dun(level, dun, display_mask, space, 32 + (64 + space) * (dun->h - 1), (32 + space) * (glb_map.min_size / 2)- 1);
            if (mode == MODE_EXTRA)
               put_dun_layer(level, dun, cur_layer, space, 32 + (64 + space) * (dun->h - 1), (32 + space) * (glb_map.min_size / 2)- 1);
            redraw_dun = 0;
         }
         clear(config.my_screen);
         switch (mode) {
            case MODE_MAP :
               sprintf(tmp, "Edit map");
               break;
            case MODE_MONSTER :
               sprintf(tmp, "Edit monsters");
               break;
            case MODE_EXTRA :
               sprintf(tmp, "Edit layer %d info", cur_layer);
               break;
            default :
               sprintf(tmp, "Unknown mode !");
         }
         textout(config.my_screen, font, tmp, 20, config.screen_height - 15, 255);
         sprintf(tmp, "%c %s\\%s", modified ? '*' : ' ', glb_map.path, argv[1]);
         textout(config.my_screen, font, tmp, 10, 0, 255);
         sprintf(tmp, "palette : %s (gamma %s)                ZOOM : 2/%d", pal_name, gamma_str[config.cur_gamma], zoom);
         textout(config.my_screen, font, tmp, 10, 10, 255);
         sprintf(tmp, "space : %d                      %s %s %s %s %s", space, (display_mask & DISPLAY_SOL) ? "SOL" : "   ", (display_mask & DISPLAY_AMP) ? "AMP" : "   ",
                    (display_mask & DISPLAY_MON) ? "MONSTER" : "       ", (display_mask & DISPLAY_ARCH) ? "ARCH" : "    ", (display_mask & DISPLAY_WALL) ? "WALL" : "    ");
         textout(config.my_screen, font, tmp, 10, 20, 255);
         stretch_blit(level, tmpbmp, 0, 0, level->w, level->h, config.screen_width / 2 - 20 - plotx * 2 / zoom, config.screen_height / 2 - 40- ploty * 2 / zoom, level->w * 2 / zoom, level->h * 2 / zoom);
         if (mode == MODE_MAP)
            put_tile_sketch(tmpbmp, space, config.screen_width / 2 - 20 + (32 + (64 + space) * (dun->h - 1) - plotx) * 2 / zoom, config.screen_height / 2 - 40 + ((32 + space) * (glb_map.min_size / 2)- 1- ploty) * 2 / zoom, cursor_x, cursor_y, zoom);
         else
            put_subtile_sketch(tmpbmp, space, config.screen_width / 2 - 20 + (32 + (64 + space) * (dun->h - 1) - plotx) * 2 / zoom, config.screen_height / 2 - 40 + ((32 + space) * (glb_map.min_size / 2)- 1- ploty) * 2 / zoom, cursor_x, cursor_y, zoom);
         vsync();
         blit(config.my_screen, screen, 0, 0, 0, 0, config.screen_width, config.screen_height);
         redraw = 0;
      }
      if (key[KEY_UP]) {
         ploty -= 10;
         if (key[KEY_LSHIFT] || key[KEY_RSHIFT]) {
            ploty -= 20;
         }
         redraw = 1;
      }
      if (key[KEY_DOWN]) {
         ploty += 10;
         if (key[KEY_LSHIFT] || key[KEY_RSHIFT]) {
            ploty += 20;
         }
         redraw = 1;
      }
      if (key[KEY_LEFT]) {
         plotx -= 10;
         if (key[KEY_LSHIFT] || key[KEY_RSHIFT]) {
            plotx -= 20;
         }
         redraw = 1;
      }
      if (key[KEY_RIGHT]) {
         plotx += 10;
         if (key[KEY_LSHIFT] || key[KEY_RSHIFT]) {
            plotx += 20;
         }
         redraw = 1;
      }
      if (key[KEY_TAB]) {
         while(key[KEY_TAB]);
         switch (mode) {
            case MODE_MAP :
               if (dun->nb_layer == 0)
                  break;
               mode = MODE_EXTRA;
               cur_layer = 0;
               cursor_x *= 2;
               cursor_y *= 2;
               display_mask &= ~(DISPLAY_AMP | DISPLAY_SOL);
               selected = 0;
               select_x = cursor_x;
               select_y = cursor_y;
               redraw_dun = 1;
               break;
            case MODE_MONSTER :
               if (dun->nb_layer > 2) {
                  mode = MODE_EXTRA;
                  cur_layer = 2;
                  redraw_dun = 1;
                  break;
               }
               mode = MODE_MAP;
               cursor_x /= 2;
               cursor_y /= 2;
               selected = 0;
               select_x = cursor_x;
               select_y = cursor_y;
               redraw_dun = 1;
               break;
            case MODE_EXTRA :
               ++cur_layer;
               if (cur_layer >= dun->nb_layer) {
                  mode = MODE_MAP;
                  cursor_x /= 2;
                  cursor_y /= 2;
                  selected = 0;
                  select_x = cursor_x;
                  select_y = cursor_y;
                  redraw_dun = 1;
                  break;
               }
               if (cur_layer == 1) {
                  if (glb_monster.nb_monster_index > 0)
                     mode = MODE_MONSTER;
               }
               redraw_dun = 1;
               break;
            default :
               fprintf(stderr, "unknow current mode !\n");
               mode = MODE_MAP;
               cursor_x = 0;
               cursor_y = 0;
               redraw_dun = 1;
               break;
         }
      }
      if (key[KEY_ENTER]) {
         while(key[KEY_ENTER]);
         switch (mode) {
            case MODE_MAP:
               dun->tiles[cursor_y * dun->w + cursor_x] = choose_til(dun->tiles[cursor_y * dun->w + cursor_x]);
               modified = 1;
               redraw_dun = 1;
               break;
            case MODE_MONSTER :
               dun->layer[1][cursor_y * 2 * dun->w + cursor_x] = choose_monster(dun->layer[cur_layer][cursor_y * 2 * dun->w + cursor_x]);
               modified = 1;
               redraw_dun = 1;
               break;
            case MODE_EXTRA :
               dun->layer[cur_layer][cursor_y * 2 * dun->w + cursor_x] = choose_value(dun->layer[cur_layer][cursor_y * 2 * dun->w + cursor_x]);
               modified = 1;
               redraw_dun = 1;
               break;
            default :
               break;
         }
      }
      if (key[KEY_7_PAD]) {
         while(key[KEY_7_PAD]);
         if (cursor_x <= 0)
            continue;
         --cursor_x;
         redraw = 1;
         if (mode != MODE_MAP) {
            continue;
         }
         if (selected == 1)
            redraw_dun = 1;
         if (!(key[KEY_LSHIFT] || key[KEY_RSHIFT])) {
            selected = 0;
            select_x = cursor_x;
            select_y = cursor_y;
         }
         else {
            selected = 1;
            redraw_dun = 1;
         }
      }
      if (key[KEY_9_PAD]) {
         while(key[KEY_9_PAD]);
         if (cursor_y <= 0)
            continue;
         --cursor_y;
         redraw = 1;
         if (mode != MODE_MAP) {
            continue;
         }
         if (selected == 1)
            redraw_dun = 1;
         if (!(key[KEY_LSHIFT] || key[KEY_RSHIFT])) {
            selected = 0;
            select_x = cursor_x;
            select_y = cursor_y;
         }
         else {
            selected = 1;
            redraw_dun = 1;
         }
      }
      if (key[KEY_1_PAD]) {
         while(key[KEY_1_PAD]);
         if (mode != MODE_MAP) {
            if ((cursor_y + 1) < 2 * dun->h) {
               ++cursor_y;
               redraw = 1;
            }
            continue;
         }
         ++cursor_y;
         if (selected == 1)
            redraw_dun = 1;
         if (!(key[KEY_LSHIFT] || key[KEY_RSHIFT])) {
            selected = 0;
            select_x = cursor_x;
            select_y = cursor_y;
         }
         else {
            selected = 1;
            redraw_dun = 1;
         }
         redraw = 1;
      }
      if (key[KEY_3_PAD]) {
         while(key[KEY_3_PAD]);
         if (mode != MODE_MAP) {
            if ((cursor_x + 1) < 2 * dun->w) {
               ++cursor_x;
               redraw = 1;
            }
            continue;
         }
         if (cursor_x + 1 >= dun->w)
            continue;
         ++cursor_x;
         if (selected == 1)
            redraw_dun = 1;
         if (!(key[KEY_LSHIFT] || key[KEY_RSHIFT])) {
            selected = 0;
            select_x = cursor_x;
            select_y = cursor_y;
         }
         else {
            selected = 1;
            redraw_dun = 1;
         }
         redraw = 1;
      }
      if (key[KEY_SPACE]) {
         while(key[KEY_SPACE]);
         plotx = level->w / 2;
         ploty = level->h / 2;
         plotx -= (plotx * SPACE_MAX) / (64 + SPACE_MAX);
         ploty -= (ploty * SPACE_MAX) / (32 + SPACE_MAX);
         zoom = 2;
         space = 0;
         redraw_dun = 1;
      }
      if (key[KEY_A]) {
         if (space <= SPACE_MAX) {
            plotx += plotx / (64 + space);
            ploty += ploty / (32 + space);
            ++space;
            redraw_dun = 1;
         }
      }
      if (key[KEY_Z]) {
         if (space > 0) {
            plotx -= plotx / (64 + space);
            ploty -= ploty / (32 + space);
            --space;
            redraw_dun = 1;
         }
      }
      if (key[KEY_1]) {
         while(key[KEY_1]);
         pal_name = change_curr_palette(act, 0);
         redraw = 1;
      }
      if (key[KEY_2]) {
         while(key[KEY_2]);
         pal_name = change_curr_palette(act, 1);
         redraw = 1;
      }
      if (key[KEY_3]) {
         while(key[KEY_3]);
         pal_name = change_curr_palette(act, 2);
         redraw = 1;
      }
      if (key[KEY_4]) {
         while(key[KEY_4]);
         pal_name = change_curr_palette(act, 3);
         redraw = 1;
      }
      if (key[KEY_5]) {
         while(key[KEY_5]);
         pal_name = change_curr_palette(act, 4);
         redraw = 1;
      }
      if (key[KEY_6]) {
         while(key[KEY_6]);
         pal_name = change_curr_palette(act, 5);
         redraw = 1;
      }
      if (key[KEY_7]) {
         while(key[KEY_7]);
         pal_name = change_curr_palette(act, 6);
         redraw = 1;
      }
      if (key[KEY_8]) {
         while(key[KEY_8]);
         pal_name = change_curr_palette(act, 7);
         redraw = 1;
      }
      if (key[KEY_9]) {
         while(key[KEY_9]);
         pal_name = change_curr_palette(act, 8);
         redraw = 1;
      }
      if (key[KEY_P]) {
         BITMAP *levelbmp;
         while(key[KEY_P]);
         sprintf(tmp, "screenshot-%05i.pcx", scr);
         while (file_exists(tmp, -1, NULL)) {
            scr++;
            sprintf(tmp, "screenshot-%05i.pcx", scr);
         }
         levelbmp = aff_dun(dun, display_mask, 0);
         if (levelbmp != NULL) {
            save_pcx(tmp, levelbmp, config.the_pal);
            destroy_bitmap(levelbmp);
         }
         else
            save_pcx(tmp, level, config.the_pal);
         scr++;
      }
      if (key[KEY_ESC]) {
         while(key[KEY_ESC]);
         if (ask_yes_no("Quit now ?")) {
            if (modified)
               if (ask_yes_no("Do you want to save ?"))
                  write_file(dun_name, dun->data, 4 + 2 * dun->w * dun->h * (1 + 4 * dun->nb_layer));
            done = 1;
         }
         redraw = 1;
      }
      if (key[KEY_C] && (key[KEY_LCONTROL] || key[KEY_RCONTROL])) {
         while(key[KEY_C]);
         switch (mode) {
            case MODE_MAP:
               if (buffer != NULL) {
                  destroy_dun(buffer);
                  buffer = NULL;
               }
               buff_x = select_x;
               buff_y = select_y;
               buff_w = cursor_x - select_x;
               buff_h = cursor_y - select_y;
               if (buff_w < 0) {
                  buff_x = cursor_x;
                  buff_w = -buff_w;
               }
               if (buff_h < 0) {
                  buff_y = cursor_y;
                  buff_h = -buff_h;
               }
               ++buff_w;
               ++buff_h;
               buffer = create_dun(buff_w, buff_h, dun->nb_layer);
               if (buffer == NULL) {
                  fprintf(stderr, "Cant create DUN for copy/paste\n");
                  break;
               }
               copy_dun(dun, buff_x, buff_y, buff_w, buff_h, buffer, 0, 0);
               select_x = cursor_x;
               select_y = cursor_y;
               selected = 0;
               redraw_dun = 1;
               break;
            case MODE_MONSTER :
               mon_buffer = dun->layer[1][cursor_x + cursor_y * 2 * dun->w];
               break;
            default :
               break;
         }
      }
      if (key[KEY_MINUS] ||key[KEY_MINUS_PAD]) {
         while (key[KEY_MINUS] ||key[KEY_MINUS_PAD]);
         ++zoom;
         if (zoom > 8)
            zoom = 8;
         redraw = 1;
      }
      if (key[KEY_EQUALS] ||key[KEY_PLUS_PAD]) {
         while (key[KEY_EQUALS] ||key[KEY_PLUS_PAD]);
         --zoom;
         if (zoom < 1)
            zoom = 1;
         redraw = 1;
      }
      if (key[KEY_X]) {
         while(key[KEY_X]);
         if (key[KEY_LCONTROL] || key[KEY_RCONTROL]) {
            switch (mode) {
               case MODE_MAP:
                  if (buffer != NULL) {
                     destroy_dun(buffer);
                     buffer = NULL;
                  }
                  buff_x = select_x;
                  buff_y = select_y;
                  buff_w = cursor_x - select_x;
                  buff_h = cursor_y - select_y;
                  if (buff_w < 0) {
                     buff_x = cursor_x;
                     buff_w = -buff_w;
                  }
                  if (buff_h < 0) {
                     buff_y = cursor_y;
                     buff_h = -buff_h;
                  }
                  ++buff_w;
                  ++buff_h;
                  buffer = create_dun(buff_w, buff_h, dun->nb_layer);
                  if (buffer == NULL) {
                     fprintf(stderr, "Cant create DUN for copy/paste\n");
                     break;
                  }
                  copy_dun(dun, buff_x, buff_y, buff_w, buff_h, buffer, 0, 0);
                  for (i = buff_y; i < buff_y + buff_h; ++i) {
                     memset(dun->tiles + i * dun->w + buff_x, 0, 2 * buff_w);
                     for (j = 0; j < dun->nb_layer; ++j) {
                        memset(dun->layer[j] + (i * 2 * 2 * dun->w + 2 * buff_x), 0, 4 * buff_w);
                        memset(dun->layer[j] + ((i * 2 + 1 ) * 2 * dun->w + 2 * buff_x), 0, 4 * buff_w);
                     }
                  }
                  select_x = cursor_x;
                  select_y = cursor_y;
                  selected = 0;
                  redraw_dun = 1;
                  modified = 1;
                  break;
               case MODE_MONSTER :
                  mon_buffer = dun->layer[1][cursor_x + cursor_y * 2 * dun->w];
                  dun->layer[1][cursor_x + cursor_y * 2 * dun->w] = 0;
                  redraw_dun = 1;
                  break;
               default :
                  break;
            }
         }
      }
      if (key[KEY_V] && (key[KEY_LCONTROL] || key[KEY_RCONTROL])) {
         while(key[KEY_V]);
         switch (mode) {
            case MODE_MAP:
               if (buffer == NULL) {
                  break;
               }
               copy_dun(buffer, 0, 0, buffer->w, buffer->h, dun, cursor_x, cursor_y);
               modified = 1;
               redraw_dun = 1;
               break;
            case MODE_MONSTER :
               if (mon_buffer == -1)
                  break;
               dun->layer[1][cursor_x + cursor_y * 2 * dun->w] = mon_buffer;
               redraw_dun = 1;
               break;
            default :
               break;
         }
      }
      if (key[KEY_F1]) {
         while(key[KEY_F1]);
         display_mask ^= DISPLAY_SOL;
         redraw_dun = 1;
      }
      if (key[KEY_F2]) {
         while(key[KEY_F2]);
         display_mask ^= DISPLAY_AMP;
         redraw_dun = 1;
      }
      if (key[KEY_F3]) {
         while(key[KEY_F3]);
         display_mask ^= DISPLAY_MON;
         redraw_dun = 1;
      }
      if (key[KEY_F4]) {
         while(key[KEY_F4]);
         display_mask ^= DISPLAY_ARCH;
         redraw_dun = 1;
      }
      if (key[KEY_F5]) {
         while(key[KEY_F5]);
         display_mask ^= DISPLAY_WALL;
         redraw_dun = 1;
      }
      if (key[KEY_S]) {
         while(key[KEY_S]);
         if (key[KEY_LCONTROL] || key[KEY_RCONTROL]) {
            if (modified) {
               write_file(dun_name, dun->data, 4 + 2 * dun->w * dun->h * (1 + 4 * dun->nb_layer));
               modified = 0;
            }
         }
      }
      if (key[KEY_F12]) {
         while(key[KEY_F12]);
         if (key[KEY_LSHIFT] || key[KEY_RSHIFT]) {
            if (config.cur_gamma > GC_060)
               config.cur_gamma--;
         }
         else {
            if (config.cur_gamma < GC_300)
               config.cur_gamma++;
         }
         pal_d2_2_vga();
         set_palette(config.the_pal);
      }
      if (key[KEY_H]) {
         while (key[KEY_H]);
         help();
         redraw = 1;
      }
   }
   destroy_bitmap(level);
   destroy_bitmap(tmpbmp);
   destroy_bitmap(config.my_screen);
   free_monster();
   free_map();
   mpq_lib_stop(config.debug);
   return 0;
}
END_OF_MAIN()
