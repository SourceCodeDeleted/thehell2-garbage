#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "write.h"
#include "dun.h"
#include "global_allegro.h"
#include "mpq_lib.h"

int help() {
fprintf(stdout, "name\tx\ty\twidth\theight\nexemple of *.ini :\nlevels\\towndata\ntown.dun\nsector1s.dun\t23\t23\nsector2s.dun\t23\t0\nsector3s.dun\t0\t23\nsector4s.dun\t0\t0\n");
return 0;
}

int main(int argc, char *argv[]) {
   char path[200];
   char input[50];
   int x, y, W, H, NB, i, c;
   unsigned long int size;
   char output[50];
   char tmp[300];
   FILE *fich;
   DUN *big, *small, *tmp_dun;

   if (argc < 2) {
      fprintf(stderr, "syntax : %s *.ini\n", argv[0]);
      return 1;
   }
   allegro_init();
   memset(&config, 0, sizeof(config));
   if (file_exists("dun_edit.ini", -1, NULL) == 0) {
      ini_create("dun_edit.ini");
      return 1;
   }
   if (ini_read("dun_edit.ini"))
      return 1;
   mpq_lib_start(config.debug);
   fich = fopen(argv[1], "rt");
   if (fich == NULL) {
      fprintf(stderr, "Cant open %s\n", argv[1]);
      mpq_lib_stop(config.debug);
      return 1;
   }
   i = 0;
   while (((c = fgetc(fich))!=EOF) && i < 199) {
      path[i] = c;
      if ((c == '\n') || (c == '\0') || (c == 0x0A) || (c == 0x0D)) {
         if (i > 0) {
            break;
         }
      }
      else
         ++i;
   }
   path[i] = '\0';
   if (c == EOF) {
      help();
      fclose(fich);
      mpq_lib_stop(config.debug);
      return 1;
   }
   sprintf(tmp, "%s\\%s\\.", mod_dir[0], path);
   if (file_exists(tmp, -1, NULL) == 0) {
      fprintf(stderr, "%s doesn't exists. Can't save anything\n", tmp);
      fclose(fich);
      mpq_lib_stop(config.debug);
      return 1;
   }
   i = 0;
   while (((c = fgetc(fich))!=EOF) && i < 49) {
      output[i] = c;
      if ((c == '\n') || (c == '\0') || (c == 0x0A) || (c == 0x0D)) {
         if (i > 0) {
            break;
         }
      }
      else
         ++i;
   }
   output[i] = '\0';
   if (c == EOF) {
      help();
      fclose(fich);
      mpq_lib_stop(config.debug);
      return 1;
   }
   sprintf(tmp, "%s\\%s", path, output);
   big = create_dun(1, 1, 0);
   if (big == NULL) {
      fprintf(stderr, "Can create dun\n");
      fclose(fich);
      mpq_lib_stop(config.debug);
      return 1;
   }
   while ((i = fscanf(fich, "%s%d%d", input, &x, &y)) == 3) {
      sprintf(tmp, "%s\\%s", path, input);
      small = read_dun(tmp);
      if (small == NULL) {
         fprintf(stderr, "Cant read %s\n", tmp);
         destroy_dun(big);
         fclose(fich);
         mpq_lib_stop(config.debug);
         return 1;
      }
      W = big->w;
      if (x + small->w > W)
         W = x + small->w;
      H = big->h;
      if (y + small->h > H)
         H = y + small->h;
      NB = big->nb_layer;
      if (small->nb_layer > NB)
         NB = small->nb_layer;
      if ((H > big->h) || (W > big->w) || (NB > big->nb_layer)) {
         tmp_dun = create_dun(W, H, NB);
         if (tmp_dun == NULL) {
            fprintf(stderr, "Out of memory, abording...\n");
            destroy_dun(big);
            destroy_dun(small);
            fclose(fich);
            mpq_lib_stop(config.debug);
            return 1;
         }
         copy_dun(big, 0, 0, big->w, big->h, tmp_dun, 0, 0);
         destroy_dun(big);
         big = tmp_dun;
      }
      copy_dun(small, 0, 0, small->w, small->h, big, x, y);
      destroy_dun(small);
   }
   sprintf(tmp, "%s\\%s\\%s", mod_dir[0], path, output);
   size = 4 + 2 * big->w * big->h * (1 + 4 * big->nb_layer);
   write_file(tmp, big->data, size);
   if ((i != 3) && (i != EOF)) {
      fprintf(stderr, "incomplete file\n");
      return 1;
   }
   fclose(fich);
   destroy_dun(big);
   mpq_lib_stop(config.debug);
   fprintf(stdout, "Done\n");
   return 0;
}
END_OF_MAIN()
