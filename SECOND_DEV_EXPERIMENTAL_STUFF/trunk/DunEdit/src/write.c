#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <allegro.h>
#include "global_allegro.h"
#include "write.h"

int write_file(char *filename, unsigned char *data, unsigned long int size) {
   FILE *fich;
   int cc;
   char tmp[300];

   if (data == NULL)
      return 1;
   if (exists(filename)) {
      cc = 0;
      sprintf(tmp, "%s.%03d", filename, cc);
      while (exists(tmp)) {
         ++cc;
         sprintf(tmp, "%s.%03d", filename, cc);
      }
      if (rename(filename, tmp)) {
         sprintf(tmp, "Couldn't rename %s to %s", filename, tmp);
         return 1;
      }
   }
   fich = fopen(filename, "wb");
   if (fich == NULL) {
      fprintf(stderr, "can't open %s\n", filename);
      return 1;
   }
   fwrite(data, size, 1, fich);
   fclose(fich);
   if (config.debug) fprintf(stdout, "%s saved\n", filename);
   return 0;
}

int write_cel_file(CEL_FILE *tc, char *filename) {
   unsigned long int size, i, offset;
   unsigned char *data;
   int res;

   if (tc == NULL)
      return 1;
   if (tc->cel == NULL) {
      fprintf(stderr, "Empty INDEX_FILE\n");
      return 1;
   }
   size = 8 + tc->nb * 4;
   for (i = 0; i < tc->nb; ++i) {
      if (tc->cel[i] != NULL) {
         if (tc->cel[i]->data != NULL) {
            size += tc->cel[i]->size;
         }
      }
   }
   data = (unsigned char *)malloc(size);
   if (data == NULL) {
      fprintf(stderr, "Can't alloc %lu bytes for file\n", size);
      return 1;
   }
   offset = 8 + 4 * tc->nb;
   ((unsigned long int *)data)[0] = tc->nb;
   for (i = 0; i < tc->nb; ++i) {
      ((unsigned long int *)data)[i + 1] = offset;
      if (tc->cel[i] != NULL) {
         if (tc->cel[i]->data != NULL) {
            memcpy(data + offset, tc->cel[i]->data, tc->cel[i]->size);
            offset += tc->cel[i]->size;
         }
      }
   }
   ((unsigned long int *)data)[i + 1] = offset;
   res = write_file(filename, data, size);
   free(data);
   if (config.debug) fprintf(stdout, "%s saved\n", filename);
   return res;
}
