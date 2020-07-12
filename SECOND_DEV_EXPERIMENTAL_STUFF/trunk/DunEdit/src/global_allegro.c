#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "global_allegro.h"
#include "mpq_lib.h"
#include "monster.h"

COLOR_MAP my_color_map;

char game_version_str[GV_MAX][10] = {
   "NONE",
   "D100", "D101", "D102", "D103", "D104", "D105", "D106", "D107", "D108", "D109",
   "H100", "H101",
   "CUSTOM"
};

char gamma_str[GC_MAX][5] = {
   {"0.60"}, {"0.62"}, {"0.64"}, {"0.66"}, {"0.68"}, {"0.70"}, {"0.72"},
   {"0.74"}, {"0.76"}, {"0.78"}, {"0.80"}, {"0.82"}, {"0.84"}, {"0.86"},
   {"0.88"}, {"0.90"}, {"0.92"}, {"0.94"}, {"0.96"}, {"0.98"}, {"1.00"},
   {"1.10"}, {"1.20"}, {"1.30"}, {"1.40"}, {"1.50"}, {"1.60"}, {"1.70"},
   {"1.80"}, {"1.90"}, {"2.00"}, {"2.10"}, {"2.20"}, {"2.30"}, {"2.40"},
   {"2.50"}, {"2.60"}, {"2.70"}, {"2.80"}, {"2.90"}, {"3.00"}
};

CONFIG_S config;

int ini_read(char *ininame) {
   typedef enum {T_NULL, T_MPQ, T_MOD, T_STR, T_INT, T_GAM, T_YES, T_ZOOM, T_ULI, T_GVN} TYPE_E;
   static struct { /* 'static' because we need to keep the default string values */
      char name[30];
      TYPE_E type;
      void *data_ptr;
      void *def;
   } datas[] = {
      {"mod_dir",                 T_MOD,  mod_dir[0],                          ""                       },
      {"MPQ1",                    T_MPQ,  mpq_file[0],                         ""                       },
      {"MPQ2",                    T_MPQ,  mpq_file[1],                         ""                       },
      {"MPQ3",                    T_MPQ,  mpq_file[2],                         ""                       },
      {"MPQ4",                    T_MPQ,  mpq_file[3],                         ""                       },
      {"full_screen",             T_YES,  & config.fullscreen,                 "YES"                    },
      {"screen_width",            T_INT,  & config.screen_width,               (void *) 640             },
      {"screen_height",           T_INT,  & config.screen_height,              (void *) 480             },
      {"screen_depth",            T_INT,  & config.screen_depth,               (void *) 8               },
      {"refresh_rate",            T_INT,  & config.screen_refresh,             (void *) 60              },
      {"gamma_correction",        T_GAM,  & config.cur_gamma,                  "1.00"                   },
      {"backgroung_red",          T_INT,  & config.bg_red,                     "60"                     },
      {"backgroung_green",        T_INT,  & config.bg_green,                   "60"                     },
      {"backgroung_blue",         T_INT,  & config.bg_blue,                    "60"                     },
      {"debug",                   T_YES,  & config.debug,                      "YES"                    },
      {"game_exe",                T_STR,  & config.game_exe,                   ""                       },
      {"game_version",            T_GVN,  & config.game_version,               "NONE"                   },
      {"monster_table_offset",    T_ULI,  & glb_monster.monster_table_offset,  (void *) 0               },
      {"nb_monster",              T_INT,  & glb_monster.nb_monster,            (void *) 0               },
      {"monster_index_offset",    T_ULI,  & glb_monster.monster_index_offset,  (void *) 0               },
      {"nb_monster_index",        T_INT,  & glb_monster.nb_monster_index,      (void *) 0               },
      {"monster_index_size",      T_INT,  & glb_monster.monster_index_size,    (void *) 0               },
      {"code_offset",             T_ULI,  & config.code_offset,                (void *) 0               },
      {"", T_NULL, NULL, NULL} /* do not remove */
   };
   int i, val, is_ok, n;
   const char *str;
/*   int len;
   char *buf, **tmpptr;*/

   set_config_file(ininame);
   i = 0;
   is_ok = 1;
   while (datas[i].type != T_NULL) {
      str = get_config_string(NULL, datas[i].name, (const char *) datas[i].def);
      if (str == NULL) {
         fprintf(stderr, "   error, line not found : <%s>\n", datas[i].name);
         is_ok = 0;
      }
      switch (datas[i].type) {
         /* string */
         case T_STR :
            strcpy(datas[i].data_ptr, str);
/*            if (strlen(str))
               datas[i].data_ptr = (char *) str;*/
            break;
         /* number */
         case T_INT :
            val = get_config_int(NULL, datas[i].name, (int) datas[i].def);
            * ((int *) datas[i].data_ptr) = val;
            break;
         case T_ULI :
            val = get_config_int(NULL, datas[i].name, (unsigned long int) datas[i].def);
            * ((unsigned long int *) datas[i].data_ptr) = val;
            break;
         /* gamma correction */
         case T_GAM :
            n=0;
            while ( (n < GC_MAX) &&
                    (strcmp(str, gamma_str[n]) != 0) )
               n++;
            if (n < GC_MAX)
               config.cur_gamma = n;
            break;
         /* game version */
         case T_GVN :
            n=0;
            while ( (n < GV_MAX) &&
                    (strcmp(str, game_version_str[n]) != 0) )
               n++;
            if (n < GV_MAX)
               config.game_version = n;
            else
               config.game_version = GV_NONE;
            break;
         /* mpq file */
         case T_MPQ :
            strcpy(datas[i].data_ptr, str);
/*            len = strlen(str);
            if (len) {
               buf = (char *) malloc(sizeof(char) * (len+1));
               if (buf == NULL) {
                  fprintf(stderr, "read_ini(), malloc() error on Mpq name");
               }
               else {
                  strcpy(buf, str);
                  tmpptr = datas[i].data_ptr;
                  *tmpptr = buf;
               }
            }*/
            break;
         /* mod directory */
         case T_MOD :
            strcpy(datas[i].data_ptr, str);
/*            len = strlen(str);
            if (len) {
               buf = (char *) malloc(sizeof(char) * (len+1));
               if (buf == NULL) {
                  fprintf(stderr, "read_ini(), malloc() error on Mod name");
               }
               else {
                  strcpy(buf, str);
                  if (buf[strlen(buf)-1] == '\\')
                     buf[strlen(buf)-1] = 0;
                  tmpptr = datas[i].data_ptr;
                  * tmpptr = buf;
               }
            }*/
            break;
         /* read a YES/NO string, but store it as TRUE/FALSE */
         case T_YES :
            if (strlen(str)) {
               if (stricmp(str, "YES") == 0)
                  * ((int *) datas[i].data_ptr) = 1;
               else
                  * ((int *) datas[i].data_ptr) = 0;
            }
            break;
         /* zoom string */
         case T_ZOOM :
/*            if (strlen(str)) {
               * ((int *) datas[i].data_ptr) = -1;
               if (stricmp(str, "1:1") == 0)
                  * ((int *) datas[i].data_ptr) = ZM_11;
               else if (stricmp(str, "1:2") == 0)
                  * ((int *) datas[i].data_ptr) = ZM_12;
               else if (stricmp(str, "1:4") == 0)
                  * ((int *) datas[i].data_ptr) = ZM_14;
               else if (stricmp(str, "1:8") == 0)
                  * ((int *) datas[i].data_ptr) = ZM_18;
               else if (stricmp(str, "1:16") == 0)
                  * ((int *) datas[i].data_ptr) = ZM_116;
            }
               datas[i].data_ptr = (char *) str;
            break;*/
         /* default */
         case T_NULL :
            break;
      }
      i++;
   }
   if (is_ok != 1) {
      fprintf(stderr, "%s is not valid.\n"
            "Delete it, and relaunch this prog to create a new good one,\n"
            "then edit it to make changes where necessary, then relaunch this prog", ininame);
      return 1;
   }
   return 0;
}

int ini_create(char *ininame) {
   FILE * out;

   printf("can't open %s, creating it\n", ininame);
   out = fopen(ininame, "wt");
   if (out == NULL) {
      fprintf(stderr, "can't create %s", ininame);
      return 1;
   }
   fputs("; Paths to the mpqs. When the editor need to read a file from a mpq\n", out);
   fputs("; it scan first in mod_dir. If not found, in MPQ1. If not found,\n", out);
   fputs("; in MPQ2. If again not found, again until MPQ4.\n", out);
   fputs("; =================================================================\n", out);
   fputs("MPQ1     = c:\\Diablo\\Patch_rt.mpq\n", out);
   fputs("MPQ2     = c:\\Sierra\\hellfire\\hellfire.mpq\n", out);
   fputs("MPQ3     = D:\\diabdat.mpq\n", out);
   fputs("MPQ4     =\n", out);
   fputs("mod_dir  = .\n", out);
   fputs("\n", out);
   fputs("; if you don't have a mpq, put a blank\n", out);
   fputs("\n", out);
   fputs("; mod_dir is an optional Mod Directory, like :\n", out);
   fputs("; mod_dir = c:\\Diablo\\my_dir\n", out);
   fputs("\n", out);
   fputs("\n", out);
   fputs("; screen configuration\n", out);
   fputs("; screen_depth can be either 8, 15, 16, 24 or 32 bits per pixel\n", out);
   fputs(";    (if your video card support it)\n", out);
   fputs("; if full_screen is not set to YES, it'll be a windowed screen\n", out);
   fputs(";    (will always fail under MS-DOS)\n", out);
   fputs("; =============================================================\n", out);
   fputs("full_screen   = YES\n", out);
   fputs("screen_width  = 640\n", out);
   fputs("screen_height = 480\n", out);
   fputs("screen_depth  = 8\n", out);
   fputs("\n", out);
   fputs("\n", out);
   fputs("; Try to use this refresh rate, if possible. Not all drivers are able to\n", out);
   fputs("; control this at all, and even when they can, not all rates will be\n", out);
   fputs("; possible on all hardware, so the actual settings may differ from what\n", out);
   fputs("; you requested. Some non-exhaustive values : 60, 70, 72, 75, 85, 100, 120\n", out);
   fputs("; ========================================================================\n", out);
   fputs("refresh_rate = 60\n", out);
   fputs("\n", out);
   fputs("\n", out);
   fputs("; default gamma correction\n", out);
   fputs("; valid value : 0.60, 0.62, [...] 0.98, 1.00, 1.10, 1.20, [...], 3.00\n", out);
   fputs("; -------------------------------------------------------------------\n", out);
   fputs("gamma_correction = 1.00\n", out);
   fputs("\n", out);
   fputs("\n", out);
   fputs("; ========================================================================\n", out);
   fputs("; BackGround color\n", out);
   fputs("backgroung_red   = 60\n", out);
   fputs("backgroung_green = 60\n", out);
   fputs("backgroung_blue  = 60\n", out);
   fputs("; ========================================================================\n", out);
   fputs("; print some debug info (file loaded)\n", out);
   fputs("debug = YES\n", out);
   fputs("\n", out);
   fputs("; ========================================================================\n", out);
   fputs("; *.exe is used to find the monsters tables\n", out);
   fputs("; (and to extract 2 hellfire dungeons)\n", out);
   fputs("; game version is :\n", out);
   fputs(";    D100 to D109 for diablo V1.00 to V1.09\n", out);
   fputs(";    H100 to H101 for Hellfire V1.00 or V1.01\n", out);
   fputs(";    NONE (no monster info are read)\n", out);
   fputs(";    CUSTOM (you must fill monster tables data\n", out);
   fputs(";    D101, D102 and D106 are not handled at the moment\n", out);
   fputs("\n", out);
   fputs("game_exe = C:\\Diablo\\diablo.exe\n", out);
   fputs("game_version = D109\n", out);
   fputs("\n", out);
   fputs(";game_exe = C:\\SIERRA\\Hellfire\\hellfire.exe\n", out);
   fputs(";game_version = H101\n", out);
   fputs("\n", out);
   fputs("; ========================================================================\n", out);
   fputs("; Fill in following field for CUSTOM game version\n", out);
   fputs("; Wrong data here leads to crash ;)\n", out);
   fputs("\n", out);
   fputs("monster_table_offset =     ; 0x95C08 for D109, 0xAA8F0 for H101\n", out);
   fputs("nb_monster =               ; 0x70 in Diablo, 0x8A in Hellfire\n", out);
   fputs("monster_index_offset =     ; 0x99408 for D109, AEDF0 for H101\n", out);
   fputs("nb_monster_index =         ; 0x80 in diablo, 0x9A in Hellfire\n", out);
   fputs("monster_index_size =       ; 1 in diablo, 4 in Hellfire\n", out);
   fputs("code_offset =              ; 0x402200 for D109, 0x401600 for H101\n", out);
   fputs("\n", out);
   fclose(out);
   fprintf(stderr, "new viewtile.ini was created\n");
   return 0;
}

int read_gamma() {
   FILE *in;
   int gt, i;

   in = fopen("gamma.dat", "rb");
   if (in == NULL) {
      printf("can't read gamma.dat\n");
      for (gt = GC_060; gt < GC_MAX; ++gt) {
         for (i = 0; i < 256; ++i) {
            config.gamma_table[gt][i] = i;
         }
      }
      return 1;
   }
   else {
      for (gt = GC_060; gt < GC_MAX; ++gt) {
         for (i = 0; i < 256; ++i) {
            config.gamma_table[gt][i] = fgetc(in);
         }
      }
      fclose(in);
   }
   return 0;
}

int load_palette(char *name) {
   int  i, j;
   unsigned char *buff;
   long int size;

   misc_load_mpq_file(name, &buff, &size, config.debug);
   if (buff == NULL) {
      printf("\ncan't open palette : %s\n", name);
      return 1;
   }
   if (size != 0x300) {
      printf("Wrong palette size : %ld\n", size);
      free(buff);
      return 1;
   }
   j = 0;
   for (i = 0; i < 256; i++) {
      config.pal[i][0] = buff[j++];
      config.pal[i][1] = buff[j++];
      config.pal[i][2] = buff[j++];
   }
   config.pal[0][0] = config.bg_red;
   config.pal[0][1] = config.bg_green;
   config.pal[0][2] = config.bg_blue;
/*   config.pal[255][0] = 0;
   config.pal[255][1] = 0;
   config.pal[255][2] = 0;*/
   free(buff);
   return 0;
}

void pal_d2_2_vga() {
   unsigned char r, g, b;
   int i;

   for (i=0; i<256; i++) {
      r = config.pal[i][0];
      g = config.pal[i][1];
      b = config.pal[i][2];
      r = config.gamma_table[config.cur_gamma][r];
      g = config.gamma_table[config.cur_gamma][g];
      b = config.gamma_table[config.cur_gamma][b];
      config.the_pal[i].r = r >> 2;
      config.the_pal[i].g = g >> 2;
      config.the_pal[i].b = b >> 2;
   }
}

int control_create_working_dir(int act) {
   char tmp[300];

   if ((act < 0) || (act > 7))
      return 1;
   sprintf(tmp, "%s\\.", mod_dir[0]);
   if (file_exists(tmp, -1, NULL) == 0) {
      fprintf(stderr, "mod_dir doesn't exists. Can't save anything\n");
      return 1;
   }
   if (act < 5) {
      sprintf(tmp, "%s\\levels\\.", mod_dir[0]);
      if (!file_exists(tmp, -1, NULL)) {
         tmp[strlen(tmp) - 2] = 0;
         if (mkdir(tmp)) {
            fprintf(stderr, "main() : can't create directory %s", tmp);
            return 1;
         }
      }
      if (act == 0) {
         sprintf(tmp, "%s\\levels\\towndata\\.", mod_dir[0]);
         if (!file_exists(tmp, -1, NULL)) {
            tmp[strlen(tmp) - 2] = 0;
            if (mkdir(tmp)) {
               fprintf(stderr, "main() : can't create directory %s", tmp);
               return 1;
            }
         }
      }
      else {
         sprintf(tmp, "%s\\levels\\l%ddata\\.", mod_dir[0], act);
         if (!file_exists(tmp, -1, NULL)) {
            tmp[strlen(tmp) - 2] = 0;
            if (mkdir(tmp)) {
               fprintf(stderr, "main() : can't create directory %s", tmp);
               return 1;
            }
         }
      }
   }
   else {
      sprintf(tmp, "%s\\nlevels\\.", mod_dir[0]);
      if (!file_exists(tmp, -1, NULL)) {
         tmp[strlen(tmp) - 2] = 0;
         if (mkdir(tmp)) {
            fprintf(stderr, "main() : can't create directory %s", tmp);
            return 1;
         }
      }
      if (act == 7) {
         sprintf(tmp, "%s\\nlevels\\towndata\\.", mod_dir[0]);
         if (!file_exists(tmp, -1, NULL)) {
            tmp[strlen(tmp) - 2] = 0;
            if (mkdir(tmp)) {
               fprintf(stderr, "main() : can't create directory %s", tmp);
               return 1;
            }
         }
      }
      else {
         sprintf(tmp, "%s\\nlevels\\l%ddata\\.", mod_dir[0], act);
         if (!file_exists(tmp, -1, NULL)) {
            tmp[strlen(tmp) - 2] = 0;
            if (mkdir(tmp)) {
               fprintf(stderr, "main() : can't create directory %s", tmp);
               return 1;
            }
         }
      }
   }
   return 0;
}

int set_monster_offsets() {
   unsigned long int codeoffset[GV_CUSTOM] = {
      0x0,
      0x401800,	0x0,		0x0,		0x401C00,	0x401C00,	0x402000,	0x0,		0x402200,	0x402600,	0x402200,
      0x401E00,	0x401600
   };
   unsigned long int montbloffset[GV_CUSTOM] = {
      0x0,
      0x93C20,	0x0,		0x0,		0x977F8,	0xB1AE8,	0xB7E58,	0x0,		0x96C70,	0x977A8,	0x95C08,
      0xA8408,	0xAA8F0
   };
   unsigned long int monidxoffset[GV_CUSTOM] = {
      0x0,
      0x97420,	0x0,		0x0,		0x9AFF8,	0xB52E8,	0xBB658,	0x0,		0x9A470,	0x9AFA8,	0x99408,
      0xA8408,	0xAEDF0
   };
   int nbmon[GV_CUSTOM] = {
      0x0,
      0x70,	0x70,		0x70,		0x70,		0x70,		0x70,		0x70,		0x70,		0x70,		0x70,
      0x8A,	0x8A
   };
   int nbmonidx[GV_CUSTOM] = {
      0x0,
      0x80,	0x80,		0x80,		0x80,		0x80,		0x80,		0x80,		0x80,		0x80,		0x80,
      0x9A,	0x9A
   };
   int monidxsize[GV_CUSTOM] = {
      0,
      1,	1,		1,		1,		1,		1,		1,		1,		1,		1,
      4,	4
   };

   if ((config.game_version < GV_NONE) || (config.game_version > GV_CUSTOM))
      config.game_version = GV_NONE;
   if (config.game_version == GV_CUSTOM)
      return 0;
   if (test_version())
      config.game_version = GV_NONE;
   glb_monster.nb_monster = nbmon[config.game_version];
   if (config.debug)
      fprintf(stdout, "nb monsters : 0x%X\n", glb_monster.nb_monster);
   glb_monster.monster_table_offset = montbloffset[config.game_version];
   if (config.debug)
      fprintf(stdout, "monster table offset : 0x%lX\n", glb_monster.monster_table_offset);
   glb_monster.monster_index_offset = monidxoffset[config.game_version];
   if (config.debug)
      fprintf(stdout, "monster index table offset : 0x%lX\n", glb_monster.monster_index_offset);
   glb_monster.nb_monster_index = nbmonidx[config.game_version];
   if (config.debug)
      fprintf(stdout, "nb monster index : 0x%X\n", glb_monster.nb_monster_index);
   glb_monster.monster_index_size = monidxsize[config.game_version];
   if (config.debug)
      fprintf(stdout, "monster index size : 0x%X\n", glb_monster.monster_index_size);
   config.code_offset = codeoffset[config.game_version];
   if (config.debug)
      fprintf(stdout, "code offset : 0x%lX\n", config.code_offset);
   return 0;
}

int test_version() {
   FILE *game;
   int i;
   unsigned long int version_offset[GV_CUSTOM] = {
      0x0,
      0x98328, 0x0, 0x0, 0x9C420, 0xA1120, 0xAE5B0, 0x0, 0x8D3C8, 0x8DF2C, 0x8C38C,
      0x9CE38, 0x9F2C0
   };
   char version_name[GV_CUSTOM][20] = {
      "",
      "Diablo v1.00", "", "", "Diablo v1.03", "Diablo v1.04", "Diablo v1.05", "", "Diablo v1.07", "Diablo v1.08", "Diablo v1.09",
      "Hellfire v1.00", "Hellfire v1.01"
   };

   if ((config.game_version < GV_NONE) || (config.game_version > GV_CUSTOM))
      return 1;
   if (config.game_version == GV_CUSTOM)
      return 1;
   game = fopen(config.game_exe, "rb");
   if (game == NULL) {
      fprintf(stderr, "Cant open %s\n", config.game_exe);
      glb_monster.nb_monster = 0;
      return 1;
   }
   fseek(game, version_offset[config.game_version], SEEK_SET);
   for (i = 0; version_name[config.game_version][i] != '\0'; ++i) {
      if (version_name[config.game_version][i] != fgetc(game)) {
         fclose(game);
         fprintf(stderr, "Altered game version, won't read monster data\n");
         return 1;
      }
   }
   fclose(game);
   if (config.debug)
      fprintf(stdout, "version is %s\n", version_name[config.game_version]);
   return 0;
}
