#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "write.h"
#include "dun.h"
#include "global_allegro.h"
#include "mpq_lib.h"

#define NB_DUN 2

int main(int argc, char *argv[]) {
   int w, h, i, j;
   unsigned long int off1, off2;
   char dunname[NB_DUN][50] = { "nlevels\\l5data\\nakrull.dun", "nlevels\\l5data\\cornerst.dun" };
   unsigned long int reference[NB_DUN][2] = {
      { 0xDD45, 0xDDA6 },
      { 0xDDEA, 0xDE31 }
   };
   char tmp[400];
   FILE *fich;
   DUN *res;

   w = 0;
   h = 0;
   allegro_init();
   memset(&config, 0, sizeof(config));
   if (file_exists("dun_edit.ini", -1, NULL) == 0) {
      ini_create("dun_edit.ini");
      return 1;
   }
   if (ini_read("dun_edit.ini"))
      return 1;
   if (control_create_working_dir(5)) {
      return 1;
   }
   fich = fopen(config.game_exe, "rb");
   if (fich == NULL) {
      fprintf(stderr, "Cant open %s\n", argv[1]);
      return 1;
   }
   fprintf(stdout, "%s opened\n", config.game_exe);
   for (i = 0; i < NB_DUN; ++i) {
      fseek(fich, reference[i][0], SEEK_SET);
      fread(&off1, 4, 1, fich);
      fprintf(stdout, "first reference indicates 0x%lX\n", off1 - config.code_offset);
      fseek(fich, reference[i][1], SEEK_SET);
      fread(&off2, 4, 1, fich);
      fprintf(stdout, "second reference indicates 0x%lX\n", off2 - config.code_offset);
      if (off1 != off2) {
         fprintf(stderr, "Offsets are different, something is not right. Abording\n");
         continue;
      }
      fseek(fich, off1 - config.code_offset, SEEK_SET);
      fread(&w, 1, 1, fich);
      fread(&h, 1, 1, fich);
      fprintf(stdout, "Width : %d\theight : %d\n", w, h);
      res = create_dun(w, h, 0);
      if (res == NULL) {
         fprintf(stderr, "Cant create dun (size %d * %d)\n", w, h);
         continue;
      }
      for (j = 0; j < w * h; ++j) {
         fread(res->tiles + j, 1, 1, fich);
      }
      sprintf(tmp, "%s\\%s", mod_dir[0], dunname[i]);
      write_file(tmp, res->data, 4 + 2 * w * h);
      fprintf(stdout, "%s done\n", dunname[i]);
      destroy_dun(res);
   }
   fclose(fich);
   fprintf(stdout, "Done\n");
   return 0;
}
END_OF_MAIN()
