#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "write.h"
#include "dun.h"
#include "global_allegro.h"
#include "mpq_lib.h"

int help() {
fprintf(stdout, "name\tx\ty\twidth\theight\tnb_layers\nexemple of *.ini :\nlevels\\towndata\ntown.dun\nsector1s.dun\t23\t23\t25\t25\t4\nsector2s.dun\t23\t0\t25\t23\t4\nsector3s.dun\t0\t23\t23t25\t4\nsector4s.dun\t0\t0\t23\t23\t3\n");
return 0;
}

int main(int argc, char *argv[]) {
   char path[200];
   char input[50];
   int x, y, w, h, nbl, i, c;
   unsigned long int size;
   char output[50];
   char tmp[300];
   FILE *fich;
   DUN *big, *small;

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
      input[i] = c;
      if ((c == '\n') || (c == '\0') || (c == 0x0A) || (c == 0x0D)) {
         if (i > 0) {
            break;
         }
      }
      else
         ++i;
   }
   input[i] = '\0';
   if (c == EOF) {
      help();
      fclose(fich);
      mpq_lib_stop(config.debug);
      return 1;
   }
   sprintf(tmp, "%s\\%s", path, input);
   big = read_dun(tmp);
   if (big == NULL) {
      fprintf(stderr, "Can open %s\n", tmp);
      fclose(fich);
      mpq_lib_stop(config.debug);
      return 1;
   }
   while ((i = fscanf(fich, "%s%d%d%d%d%d", output, &x, &y, &w, &h, &nbl)) == 6) {
      small = create_dun(w, h, nbl);
      if (small == NULL) {
         fprintf(stderr, "Out of memory, abording...\n");
         destroy_dun(big);
         fclose(fich);
         mpq_lib_stop(config.debug);
         return 1;
      }
      copy_dun(big, x, y, w, h, small, 0, 0);
      sprintf(tmp, "%s\\%s\\%s", mod_dir[0], path, output);
      size = 4 + 2 * w * h * (1 + 4 * nbl);
      write_file(tmp, small->data, size);
      destroy_dun(small);
   }
   if ((i != 6) && (i != EOF)) {
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
