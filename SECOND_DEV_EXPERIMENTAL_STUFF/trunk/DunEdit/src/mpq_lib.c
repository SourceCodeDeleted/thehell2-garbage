/*
Library for access to MPQ files
Original code if from Tom Amigo
Some additions by Paul Siramy, then by me
*/
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define _MPQ_LIB_C_
#include "mpq_lib.h"

char mpq_file[MAX_MPQ_FILE][PATH_MAXI] = {"c:\\program files\\diablo\\patch_rt.mpq", "c:\\program files\\diablo\\hellfire.mpq", "c:\\program files\\diablo\\diabdat.mpq", ""};
char mod_dir[MAX_MOD_DIR][PATH_MAXI] = {"c:\\program files\\diablo"};
/*char mod_dir[MAX_MOD_DIR][PATH_MAXI] = {"c:\\program files\\diablo\\research\\Data"};*/
GLB_MPQ_S glb_mpq_struct[MAX_MPQ_FILE];
GLB_MPQ_S * glb_mpq;

void mpq_lib_start(int output) {
   int i;

   for (i = 0; i < MAX_MPQ_FILE; ++i) {
      memset( &(glb_mpq_struct[i]), 0, sizeof(GLB_MPQ_S));
      glb_mpq_struct[i].is_open = false;
   }
   open_all_mpq(output);
}

void mpq_lib_stop(int output) {
   int i;

   for (i = 0; i < MAX_MPQ_FILE; ++i) {
      if (glb_mpq_struct[i].is_open != false) {
         if (output) fprintf(stdout, "closing %s\n", glb_mpq_struct[i].file_name);
         glb_mpq = & glb_mpq_struct[i];
         mpq_batch_close();
         memset( & glb_mpq_struct[i], 0, sizeof(GLB_MPQ_S));
         glb_mpq_struct[i].is_open = false;
      }
   }
}

int misc_load_mpq_file(const char * filename, unsigned char ** buffer, long * buf_len, int output) {
   int i, entry;

   for (i = 0; i < MAX_MOD_DIR; ++i) {
      entry = mod_load_in_mem(mod_dir[i], filename, buffer, buf_len);
      if (entry != -1) {
         if (output) fprintf(stdout, "%s, found in %s\n", filename, mod_dir[i]);
         return true;
      }
   }
   for (i=0; i<MAX_MPQ_FILE; i++) {
      if (glb_mpq_struct[i].is_open != false) {
         glb_mpq = & glb_mpq_struct[i];
         entry = mpq_batch_load_in_mem(filename, buffer, buf_len);
         if (entry != -1) {
            if (output) fprintf(stdout, "%s, found in %s\n", filename, mpq_file[i]);
            return true;
         }
      }
   }
   return false;
}

void open_all_mpq(int output) {
   int i;
   for (i=0; i<MAX_MPQ_FILE; i++) {
      if ((mpq_file[i] != NULL) && (mpq_file[i][0] != '\0')){
         if (output) fprintf(stdout, "opening %s\n", mpq_file[i]);
         glb_mpq = & glb_mpq_struct[i];
         mpq_batch_open(mpq_file[i]);
      }
   }
}

unsigned char * txt_read_in_mem(const char * txtname) {
   unsigned char * buff = NULL, * new_buff;
   int  entry;
   long len;

/*   fprintf(stderr, "want to read a txt from mpq : %s\n", txtname);*/
   entry = misc_load_mpq_file(txtname, & buff, & len, 0);
   if ((entry == -1) || (buff == NULL)) {
      fprintf(stderr, "read_txt_in_mem() : file %s not found", txtname);
      return NULL;
   }
   len++;
   new_buff = (unsigned char *) realloc(buff, len);
   if (new_buff == NULL) {
      fprintf(stderr, "read_txt_in_mem() : can't reallocate %lu bytes for %s", len, txtname);
      if (buff != NULL)
         free(buff);
      return NULL;
   }
   if (new_buff != buff) {
      memcpy(new_buff, buff, len - 1);
   }
   * (new_buff + len - 1) = 0;
   return new_buff;
}

int mod_load_in_mem(const char * moddir, const char * filename, unsigned char ** buffer, long * buf_len) {
   FILE * in;
   long size;
   char strtmp[PATH_MAXI];
   * buffer  = (unsigned char *) NULL;
   * buf_len = 0;
   sprintf(strtmp, "%s\\%s", moddir, filename);
   in = fopen(strtmp, "rb");
   if (in == NULL)
      return -1;
   fseek(in, 0, SEEK_END);
   size = ftell(in) + 1;
   fseek(in, 0, SEEK_SET);
   * buffer = (unsigned char *) malloc(size);
   if (* buffer == NULL) {
      fclose(in);
      fprintf(stderr, "not enough mem (%li bytes) for %s (from Mod Directory)\n", size, filename);
      return -1;
   }
   size--;
   *buf_len = size;
   fread(*buffer, size, 1, in);
   *( ((unsigned char *) (*buffer)) + size) = 0;
   fclose(in);
   return 0;
}

void mpq_batch_open(const char *mpqname) {
   if (glb_mpq == NULL) {
      fprintf(stderr, "mpq_batch_open_mpq() : pointer glb_mpq == NULL\n");
                return;
   }
   if (glb_mpq->is_open != false) {
      fprintf(stderr, "mpq_batch_open_mpq() : glb_mpq->is_open != false\n");
                return;
   }
   if (mpqname == NULL) {
      fprintf(stderr, "mpq_batch_open_mpq() : mpqname == NULL\n");
                return;
   }
   glb_mpq->avail_metods[0] = 0x08;
   glb_mpq->avail_metods[1] = 0x01;
   glb_mpq->avail_metods[2] = 0x40;
   glb_mpq->avail_metods[3] = 0x80;
   strcpy(glb_mpq->file_name, mpqname);
   glb_mpq->fpMpq = fopen(glb_mpq->file_name, "rb");
   if( ! glb_mpq->fpMpq) {
      fprintf(stderr, "mpq_batch_open_mpq() :  can't open %s\n", glb_mpq->file_name);
      return;
   }
   if (test_prepare_archive()) {
      fprintf(stderr, "test_prepare_archive failed\n");
      return;
   }
   glb_mpq->is_open = true;
}

int mpq_batch_load_in_mem(const char *filename, unsigned char **buffer, long *buf_len) {
   int  num_entry, size_unpack;
   unsigned char *mp_new;

   if (glb_mpq == NULL) {
      fprintf(stderr, "mpq_batch_load_in_mem_mpq() : pointer glb_mpq == NULL\n");
      return -1;
   }
   if (glb_mpq->is_open == false) {
      fprintf(stderr, "mpq_batch_load_in_mem_mpq() : glb_mpq->is_open == false\n");
      return -1;
   }
   if (filename == NULL) {
      fprintf(stderr, "mpq_batch_load_in_mem_mpq() : filename == NULLn");
      return -1;
   }
   if (strlen(filename) == 0) {
      fprintf(stderr, "mpq_batch_load_in_mem_mpq() : strlen(filename) == 0\n");
      return -1;
   }
   num_entry = test_tell_entry(filename);
   if (num_entry != -1) {
      size_unpack = * (glb_mpq->block_table + (num_entry * 4) + 2);
      if((num_entry < 0) || (num_entry > (int) glb_mpq->count_files - 1)) {
         fprintf(stderr, "Error! Invalid entry number %i (Valid entry numbers are 1-%lu)\n",
            num_entry + 1,
            glb_mpq->count_files
         );
         return -1;
      }
      else {
         mp_new = (unsigned char *) malloc(size_unpack);
         if (mp_new == NULL) {
            fprintf(stderr, "mpq_batch_load_in_mem() : not enough memory for %i bytes\n", size_unpack);
            return -1;
         }
         ExtractToMem(mp_new, num_entry);
         *buffer = mp_new;
         *buf_len = size_unpack;
/*         fprintf(output, "ok (%i bytes)", size_unpack);*/
      }
   }
   if (num_entry == 65535) {
      num_entry = -1;
   }
   return num_entry;
}

void mpq_batch_close() {
   fclose(glb_mpq->fpMpq);
   FreeLocals();
}

int test_prepare_archive() {      /* open and prepare the archive */
   unsigned long   mpq_header[2] = {0x1a51504d, 0x00000020};
   unsigned long   detected = 0;
   unsigned long   tmp;
   char  name_htable[] = "(hash table)";
   char  name_btable[] = "(block table)";
    while(fread(&tmp,sizeof(unsigned long),1,glb_mpq->fpMpq)) {
      if(mpq_header[0]==tmp) {
         fread(&tmp,sizeof(unsigned long),1,glb_mpq->fpMpq);
         if(mpq_header[1]==tmp) {
            detected=1;
            break;
         }
      }
   }
   if(!detected) {
      fprintf(stderr, "\nError: File \'%s\' is not a valid MPQ archive\n", glb_mpq->file_name);
      fclose(glb_mpq->fpMpq);
      return -1;
   }
   glb_mpq->offset_mpq=ftell(glb_mpq->fpMpq)-8;
   fread(&glb_mpq->lenght_mpq_part,sizeof(unsigned long),1,glb_mpq->fpMpq);
   fseek(glb_mpq->fpMpq,glb_mpq->offset_mpq+16,SEEK_SET);
   fread(&glb_mpq->offset_htbl,sizeof(unsigned long),1,glb_mpq->fpMpq);
   fread(&glb_mpq->offset_btbl,sizeof(unsigned long),1,glb_mpq->fpMpq);
   fread(&glb_mpq->lenght_htbl,sizeof(unsigned long),1,glb_mpq->fpMpq);
   glb_mpq->lenght_htbl *= 4;
   fread(&glb_mpq->lenght_btbl,sizeof(unsigned long),1,glb_mpq->fpMpq);
   glb_mpq->count_files = glb_mpq->lenght_btbl;
   glb_mpq->lenght_btbl *= 4; /* in fact, length of block table is in # of file */
   BuildBaseMassive();
   if(InitializeLocals()) {
      fclose(glb_mpq->fpMpq);
      return -2;
   }
   fseek(glb_mpq->fpMpq,glb_mpq->offset_mpq+glb_mpq->offset_htbl,SEEK_SET);
   fread(glb_mpq->hash_table,sizeof(unsigned long),glb_mpq->lenght_htbl,glb_mpq->fpMpq);
   fseek(glb_mpq->fpMpq,glb_mpq->offset_mpq+glb_mpq->offset_btbl,SEEK_SET);
   fread(glb_mpq->block_table,sizeof(unsigned long),glb_mpq->lenght_btbl,glb_mpq->fpMpq);
   tmp=Crc(name_htable,glb_mpq->massive_base,0x300);
   Decode(glb_mpq->hash_table,glb_mpq->massive_base,tmp,glb_mpq->lenght_htbl);
   tmp=Crc(name_btable,glb_mpq->massive_base,0x300);
   Decode(glb_mpq->block_table,glb_mpq->massive_base,tmp,glb_mpq->lenght_btbl);
   return 0;
}

long int test_tell_entry(const char * filename) {
   unsigned long scrc1, scrc2, scrc3, pointer_ht;
   scrc1 = Crc(filename, glb_mpq->massive_base, 0);
   scrc2 = Crc(filename, glb_mpq->massive_base, 0x100);
   scrc3 = Crc(filename, glb_mpq->massive_base, 0x200);
   pointer_ht = (scrc1 & (glb_mpq->lenght_htbl/4-1) ) * 4;
   for(; pointer_ht<glb_mpq->lenght_htbl; pointer_ht += 4)
   {
      if( (* (glb_mpq->hash_table+pointer_ht)   == scrc2) &&
          (* (glb_mpq->hash_table+pointer_ht+1) == scrc3)
      )
         return * (glb_mpq->hash_table + pointer_ht + 3);
   }
   return -1;
}

int InitializeLocals() {
   glb_mpq->global_buffer = (unsigned char *) malloc(0x60000);
   if(!glb_mpq->global_buffer) {
      fprintf(stderr, "\nError! Insufficient memory\n");
      return -1;
   }
   glb_mpq->read_buffer_start=glb_mpq->global_buffer;
   glb_mpq->write_buffer_start=glb_mpq->global_buffer+0x1000;
   glb_mpq->explode_buffer=glb_mpq->global_buffer+0x2000;
   glb_mpq->file_header=(unsigned long *)(glb_mpq->global_buffer+0x6000);
   glb_mpq->hash_table=(unsigned long *)malloc(glb_mpq->lenght_htbl*4);
   glb_mpq->block_table=(unsigned long *)malloc(glb_mpq->lenght_btbl*4);
   glb_mpq->filename_table=(char *)calloc(glb_mpq->lenght_btbl/4,PATH_MAXI);
   glb_mpq->identify_table=(char *)calloc(glb_mpq->lenght_btbl/4,sizeof (char));
   if(glb_mpq->hash_table && glb_mpq->block_table && glb_mpq->filename_table && glb_mpq->identify_table)
      return 0;
   else {
      fprintf(stderr, "\nError! Insufficient memory\n");
      return -1;
   }
}

void FreeLocals()
{   
   if(glb_mpq->global_buffer)
      free(glb_mpq->global_buffer);
   if(glb_mpq->hash_table)
      free(glb_mpq->hash_table);
   if(glb_mpq->block_table)
      free(glb_mpq->block_table);
   if(glb_mpq->filename_table)
      free(glb_mpq->filename_table);
   if(glb_mpq->identify_table)
      free(glb_mpq->identify_table);
   return;
}

void BuildBaseMassive()
{
   unsigned long   s1;
   int   i,j;
   ldiv_t divres;
   divres.rem=0x100001;
   for(i=0;i<0x100;i++) {
      for(j=0;j<5;j++) {
         divres=ldiv(divres.rem*125+3,0x002AAAAB);
         s1=(divres.rem&0xFFFFL)<<0x10;
         divres=ldiv(divres.rem*125+3,0x002AAAAB);
         s1=s1|(divres.rem&0xFFFFL);
         glb_mpq->massive_base[i+0x100*j]=s1;
      }
   }
   return;
}

unsigned long Crc(const char *string,unsigned long *massive_base,unsigned long massive_base_offset)
{
 char   byte;
 unsigned long   crc=0x7fed7fed;
 unsigned long   s1=0xEEEEEEEE;
 
 byte=*string;
 while(byte) {
   if(byte>0x60 && byte<0x7B)
      byte-=0x20;
   crc=*(massive_base+massive_base_offset+byte)^(crc+s1);
   s1+=crc+(s1<<5)+byte+3;
   string++;
   byte=*string;
 }
 return crc;
}

unsigned short int read_data(unsigned char *buffer,unsigned short int size,void *crap)
{
   params *param=(params *)crap;
   memcpy(buffer,param->buf_in,size);
   param->buf_in+=size;
   return size;
}

void write_data(unsigned char *buffer,unsigned short int size,void *crap)
{
   params *param=(params *)crap;
   memcpy(param->buf_out,buffer,size);
   param->buf_out+=size;
   glb_mpq->lenght_write+=size;
}


void Decode(unsigned long *data_in, unsigned long *massive_base, unsigned long crc, unsigned long lenght)
{
 unsigned long   i,dec;
 unsigned long   s1=0xEEEEEEEE;
 for(i=0;i<lenght;i++) {
   s1+=*(massive_base+0x400+(crc&0xFFL));
   dec=*(data_in+i)^(s1+crc);
   s1+=dec+(s1<<5)+3;
   *(data_in+i)=dec;
   crc=(crc>>0x0b)|(0x11111111+((crc^0x7FFL)<<0x15));
 }
 return;
}

unsigned long GetUnknowCrc(unsigned long entry)
{   
   unsigned long   tmp,i,j,coded_dword,crc_file;
   unsigned long   flag,size_unpack,num_block,offset_body;
   unsigned long   sign_riff1=0x46464952;
   unsigned long   sign_riff3=0x45564157;
   unsigned long   sign_mpq1=0x1a51504d;
   unsigned long   sign_mpq2=0x00000020;
   ldiv_t   divres;
   offset_body=*(glb_mpq->block_table+entry*4);
   flag=*(glb_mpq->block_table+entry*4+3);
   fseek(glb_mpq->fpMpq,glb_mpq->offset_mpq+offset_body,SEEK_SET);
   fread(&coded_dword,sizeof(unsigned long),1,glb_mpq->fpMpq);
   if(flag&0x200 || flag&0x100) {
      size_unpack=*(glb_mpq->block_table+entry*4+2);
      divres=ldiv(size_unpack-1,0x1000);
      num_block=divres.quot+2;
      for(j=0;j<=0xff;j++) {
         crc_file=((num_block*4)^coded_dword)-0xeeeeeeee - *(glb_mpq->massive_base+0x400+j);
         if((crc_file&0xffL) == j) {
            fseek(glb_mpq->fpMpq,glb_mpq->offset_mpq+offset_body,SEEK_SET);
            fread(glb_mpq->file_header,sizeof(unsigned long),num_block,glb_mpq->fpMpq);
            Decode(glb_mpq->file_header,glb_mpq->massive_base,crc_file,num_block);
            tmp=num_block*4;
            if(tmp == *glb_mpq->file_header) {
               for(i=0;i<num_block-1;i++) {
                    tmp+=*(glb_mpq->file_header+i+1)-*(glb_mpq->file_header+i);
                  if(*(glb_mpq->file_header+i+1)-*(glb_mpq->file_header+i)>0x1000) {
                     tmp=0xffffffff;
                     break;
                  }
               }
               if(tmp!=0xffffffff) {
                  crc_file++;
                  break;
               }
            }
         }
         crc_file=0;
      }
   } else {
      for(j=0;j<=0xff;j++) {
         crc_file=(sign_riff1^coded_dword)-0xeeeeeeee - *(glb_mpq->massive_base+0x400+j);
         if((crc_file&0xff)==j) {
            fseek(glb_mpq->fpMpq,glb_mpq->offset_mpq+offset_body,SEEK_SET);
            fread(glb_mpq->file_header,sizeof(unsigned long),3,glb_mpq->fpMpq);
            Decode(glb_mpq->file_header,glb_mpq->massive_base,crc_file,3);
            if(sign_riff1==*glb_mpq->file_header) {
               if(sign_riff3==*(glb_mpq->file_header+2))
                  break;
            }
         }
         crc_file=0;
      }
      if(!crc_file) {
         for(j=0;j<=0xff;j++) {
            crc_file=(sign_mpq1^coded_dword)-0xeeeeeeee - *(glb_mpq->massive_base+0x400+j);
            if((crc_file&0xffL) == j) {
               fseek(glb_mpq->fpMpq,glb_mpq->offset_mpq+offset_body,SEEK_SET);
               fread(glb_mpq->file_header,sizeof(unsigned long),2,glb_mpq->fpMpq);
               Decode(glb_mpq->file_header,glb_mpq->massive_base,crc_file,2);
               if(sign_mpq1 == *glb_mpq->file_header) {
                  if(sign_mpq2 == *(glb_mpq->file_header+1))
                     break;
               }
            }
            crc_file=0;
         }
      }
   }
   return crc_file;
}

int ExtractTo(FILE *fp_new,unsigned long entry)
{
   unsigned long   size_pack,size_unpack;
   unsigned char   *read_buffer,*write_buffer;
   unsigned long   i,j,offset_body,flag,crc_file;
   unsigned long   num_block,lenght_read,iteration;
   char   *szNameFile;
   unsigned char   metod;
   ldiv_t   divres;
   params   param;
   offset_body=*(glb_mpq->block_table+entry*4);
   size_unpack=*(glb_mpq->block_table+entry*4+2);
   flag=*(glb_mpq->block_table+entry*4+3);
   crc_file = 0;
   if(flag&0x30000) {
      if(*(glb_mpq->identify_table+entry)&0x1) {
         szNameFile=glb_mpq->filename_table+PATH_MAXI*entry;
         if(strrchr(szNameFile,'\\'))
            szNameFile=strrchr(szNameFile,'\\')+1;
         crc_file=Crc(szNameFile,glb_mpq->massive_base,0x300);
         if((flag&0x20200) == 0x20200)
            crc_file=(crc_file+offset_body)^size_unpack;
      }
      else
         crc_file=GetUnknowCrc(entry);
   }
   if(flag&0x200 || flag&0x100) {
      divres=ldiv(size_unpack-1,0x1000);
      num_block=divres.quot+2;
      fseek(glb_mpq->fpMpq,glb_mpq->offset_mpq+offset_body,SEEK_SET);
      fread(glb_mpq->file_header,sizeof(unsigned long),num_block,glb_mpq->fpMpq);
      if(flag&0x30000)
         Decode(glb_mpq->file_header,glb_mpq->massive_base,(crc_file-1),num_block);
      read_buffer=glb_mpq->read_buffer_start;
      for(j=0;j<(num_block-1);j++) {
         lenght_read=*(glb_mpq->file_header+j+1)-*(glb_mpq->file_header+j);
         fread(read_buffer,sizeof(char),lenght_read,glb_mpq->fpMpq);
         if(flag&0x30000)
            Decode((unsigned long *)read_buffer,glb_mpq->massive_base,crc_file,lenght_read/4);
         if(lenght_read==0x1000 || (j==num_block-2 && lenght_read==(size_unpack&0xFFF)))
               fwrite(read_buffer,sizeof(char),lenght_read,fp_new);
         else {
            if(flag&0x200) {
               metod=*read_buffer;
               iteration=0;
               for(i=0;i<4;i++) {
                  if(metod&glb_mpq->avail_metods[i])
                     iteration++;
               }
               read_buffer+=1;
               lenght_read-=1;
            } else {
               iteration=1;
               metod=8;
            }
            write_buffer=glb_mpq->write_buffer_start;
            if(metod&0x08) {
               param.buf_in =read_buffer;
               param.buf_out=write_buffer;
               glb_mpq->lenght_write=0;
               explode(&read_data,&write_data,&param);
               lenght_read=glb_mpq->lenght_write;
               iteration--;
               if(iteration) {
                  read_buffer=write_buffer;
                  write_buffer=glb_mpq->read_buffer_start;
               }
            }
            if(metod&0x01) {
               lenght_read=ExtWavUnp1((unsigned long)read_buffer,(unsigned long)lenght_read,(unsigned long)write_buffer,0x1000);
               iteration--;
               if(iteration) {
                  read_buffer=write_buffer;
                  write_buffer=glb_mpq->read_buffer_start;
               }
            }
            if(metod&0x40)
               lenght_read=ExtWavUnp2((unsigned long)read_buffer,(unsigned long)lenght_read,(unsigned long)write_buffer,0x1000);
            if(metod&0x80)
               lenght_read=ExtWavUnp3((unsigned long)read_buffer,(unsigned long)lenght_read,(unsigned long)write_buffer,0x1000);
            fwrite(write_buffer,1,lenght_read,fp_new);
            read_buffer=glb_mpq->read_buffer_start;
         }
         crc_file++;
      }
   }
   else {
      size_pack=*(glb_mpq->block_table+entry*4+1);
      if(flag&0x30000)
         lenght_read=0x1000;
      else
         lenght_read=0x60000;
      if(size_pack<lenght_read)
         lenght_read=size_pack;
      read_buffer=glb_mpq->read_buffer_start;
      if(lenght_read) {
         divres=ldiv(size_pack,lenght_read);
         num_block=divres.quot;
      } else {
         num_block=0;
         divres.rem=0;
      }
      fseek(glb_mpq->fpMpq,glb_mpq->offset_mpq+offset_body,SEEK_SET);
      for (j=0;j<num_block;j++) {
         fread(read_buffer,1,lenght_read,glb_mpq->fpMpq);
         if(flag&0x30000) {
            Decode((unsigned long *)read_buffer,glb_mpq->massive_base,crc_file,lenght_read/4);
            crc_file++;
         }
         fwrite(read_buffer,1,lenght_read,fp_new);
      }
      if(divres.rem) {
         fread(read_buffer,1,divres.rem,glb_mpq->fpMpq);
         if(flag&0x30000)
            Decode((unsigned long *)read_buffer,glb_mpq->massive_base,crc_file,divres.rem/4);
         fwrite(read_buffer,1,divres.rem,fp_new);
      }
   }
   return 0;
}

int ExtractToMem(unsigned char * mp_new, unsigned long entry)
{
   unsigned long  size_pack,size_unpack;
   unsigned char  * read_buffer,*write_buffer;
   unsigned long i,j,offset_body,flag,crc_file;
   unsigned long num_block,lenght_read,iteration;
   char    * szNameFile;
   unsigned char  metod;
   ldiv_t divres;
   params param;
   unsigned char * buff_ptr = (unsigned char *) mp_new;
   offset_body=*(glb_mpq->block_table+entry*4);
   size_unpack=*(glb_mpq->block_table+entry*4+2);
   flag=*(glb_mpq->block_table+entry*4+3);
   crc_file = 0;
   if(flag&0x30000) {
      if(*(glb_mpq->identify_table+entry)&0x1) {
         szNameFile=glb_mpq->filename_table+PATH_MAXI*entry;
         if(strrchr(szNameFile,'\\'))
            szNameFile=strrchr(szNameFile,'\\')+1;
         crc_file=Crc(szNameFile,glb_mpq->massive_base,0x300);

         if ((flag & 0x20200) == 0x20200)
            crc_file=(crc_file+offset_body)^size_unpack;
      }
      else
         crc_file=GetUnknowCrc(entry);
   }
   if(flag&0x200 || flag&0x100) {
      divres=ldiv(size_unpack-1,0x1000);
      num_block=divres.quot+2;
      fseek(glb_mpq->fpMpq,glb_mpq->offset_mpq+offset_body,SEEK_SET);
      fread(glb_mpq->file_header,sizeof(unsigned long),num_block,glb_mpq->fpMpq);
      if(flag&0x30000)
         Decode(glb_mpq->file_header,glb_mpq->massive_base,(crc_file-1),num_block);
      read_buffer=glb_mpq->read_buffer_start;
      for(j=0;j<(num_block-1);j++) {
         lenght_read=*(glb_mpq->file_header+j+1)-*(glb_mpq->file_header+j);
         fread(read_buffer,sizeof(char),lenght_read,glb_mpq->fpMpq);
         if(flag&0x30000)
            Decode((unsigned long *)read_buffer,glb_mpq->massive_base,crc_file,lenght_read/4);
         if(lenght_read==0x1000 || (j==num_block-2 && lenght_read==(size_unpack&0xFFF)))
         {
               memcpy(buff_ptr, read_buffer, lenght_read);
               buff_ptr += lenght_read;
         }
         else {
            if(flag&0x200) {
               metod=*read_buffer;
               iteration=0;
               for(i=0;i<4;i++) {
                  if(metod&glb_mpq->avail_metods[i])
                     iteration++;
               }
               read_buffer+=1;
               lenght_read-=1;
            } else {
               iteration=1;
               metod=8;
            }
            write_buffer=glb_mpq->write_buffer_start;
            if(metod&0x08) {
               param.buf_in =read_buffer;
               param.buf_out=write_buffer;
               glb_mpq->lenght_write=0;
               explode(&read_data,&write_data,&param);
               lenght_read=glb_mpq->lenght_write;
               iteration--;
               if(iteration) {
                  read_buffer=write_buffer;
                  write_buffer=glb_mpq->read_buffer_start;
               }
            }
            if(metod&0x01) {
               lenght_read=ExtWavUnp1((unsigned long)read_buffer,(unsigned long)lenght_read,(unsigned long)write_buffer,0x1000);
               iteration--;
               if(iteration) {
                  read_buffer=write_buffer;
                  write_buffer=glb_mpq->read_buffer_start;
               }
            }
            if(metod&0x40)
               lenght_read=ExtWavUnp2((unsigned long)read_buffer,(unsigned long)lenght_read,(unsigned long)write_buffer,0x1000);
            if(metod&0x80)
               lenght_read=ExtWavUnp3((unsigned long)read_buffer,(unsigned long)lenght_read,(unsigned long)write_buffer,0x1000);
            memcpy(buff_ptr, write_buffer, lenght_read);
            buff_ptr += lenght_read;
            read_buffer=glb_mpq->read_buffer_start;
         }
         crc_file++;
      }
   }
   else {
      size_pack=*(glb_mpq->block_table+entry*4+1);
      if(flag&0x30000)
         lenght_read=0x1000;
      else
         lenght_read=0x60000;
      if(size_pack<lenght_read)
         lenght_read=size_pack;
      read_buffer=glb_mpq->read_buffer_start;
      if(lenght_read) {
         divres=ldiv(size_pack,lenght_read);
         num_block=divres.quot;
      } else {
         num_block=0;
         divres.rem=0;
      }
      fseek(glb_mpq->fpMpq,glb_mpq->offset_mpq+offset_body,SEEK_SET);
      for (j=0;j<num_block;j++) {
         fread(read_buffer,1,lenght_read,glb_mpq->fpMpq);
         if(flag&0x30000) {
            Decode((unsigned long *)read_buffer,glb_mpq->massive_base,crc_file,lenght_read/4);
            crc_file++;
         }
         memcpy(buff_ptr, read_buffer, lenght_read);
         buff_ptr += lenght_read;
      }
      if(divres.rem) {
         fread(read_buffer,1,divres.rem,glb_mpq->fpMpq);
         if(flag&0x30000)
            Decode((unsigned long *)read_buffer,glb_mpq->massive_base,crc_file,divres.rem/4);
         memcpy(buff_ptr, read_buffer, divres.rem);
         buff_ptr += divres.rem;
      }
   }
   return 0;
}

unsigned long ExtWavUnp3(unsigned long buf_in,unsigned long size_in,unsigned long buf_out,unsigned long size_out)
{
   return Sub_WavUnp13(buf_in,size_in,2,buf_out,size_out);
}

unsigned long ExtWavUnp2(unsigned long buf_in,unsigned long size_in,unsigned long buf_out,unsigned long size_out)
{
   return Sub_WavUnp13(buf_in,size_in,1,buf_out,size_out);
}

unsigned long Sub_WavUnp13(unsigned long buf_in,unsigned long size_in,unsigned long flag,unsigned long buf_out,unsigned long size_out)
{
unsigned long small_tbl1[90] =
{
   0x7,    0x8,    0x9,    0xA,    0xB,    0xC,    0xD,    0xE,    0x10,   0x11,
   0x13,   0x15,   0x17,   0x19,   0x1C,   0x1F,   0x22,   0x25,   0x29,   0x2D,
   0x32,   0x37,   0x3C,   0x42,   0x49,   0x50,   0x58,   0x61,   0x6B,   0x76,
   0x82,   0x8F,   0x9D,   0xAD,   0xBE,   0xD1,   0xE6,   0xFD,   0x117,  0x133,
   0x151,  0x173,  0x198,  0x1C1,  0x1EE,  0x220,  0x256,  0x292,  0x2D4,  0x31C,
   0x36C,  0x3C3,  0x424,  0x48E,  0x502,  0x583,  0x610,  0x6AB,  0x756,  0x812,
   0x8E0,  0x9C3,  0xABD,  0xBD0,  0xCFF,  0xE4C,  0xFBA,  0x114C, 0x1307, 0x14EE,
   0x1706, 0x1954, 0x1BDC, 0x1EA5, 0x21B6, 0x2515, 0x28CA, 0x2CDF, 0x315B, 0x364B,
   0x3BB9, 0x41B2, 0x4844, 0x4F7E, 0x5771, 0x602F, 0x69CE, 0x7462, 0x7FFF, 0x0
};
unsigned long small_tbl2[32] =
{
   0xFFFFFFFF, 0x0, 0xFFFFFFFF, 0x4, 0xFFFFFFFF, 0x2, 0xFFFFFFFF, 0x6,
   0xFFFFFFFF, 0x1, 0xFFFFFFFF, 0x5, 0xFFFFFFFF, 0x3, 0xFFFFFFFF, 0x7,
   0xFFFFFFFF, 0x1, 0xFFFFFFFF, 0x5, 0xFFFFFFFF, 0x3, 0xFFFFFFFF, 0x7,
   0xFFFFFFFF, 0x2, 0xFFFFFFFF, 0x4, 0xFFFFFFFF, 0x6, 0xFFFFFFFF, 0x8
};
   unsigned long   var14,var18,var1c;
   unsigned long   tmp0,tmp1,tmp2,tmp3,tmp4;
   unsigned long   var8[2];
   unsigned long   var10[2];
   var14=buf_in;
   size_in+=buf_in;
   tmp0=var14+2;
   var1c=size_out;
   tmp3=buf_out;
   var18=buf_out;
   var10[0]=0x2C;
   var10[1]=0x2C;
   if((long)flag>0) {
      for(tmp2=0;tmp2<flag;tmp2++) {
         var8[tmp2]=(long)*((short int *)tmp0);
         tmp0+=2;
         if(size_out<2)
            return tmp3-var18;
         *((unsigned short int *)tmp3)=(unsigned short int) var8[tmp2];
         tmp3+=2;
         var1c-=2;
      }
   }
   tmp2=flag-1;
   while(tmp0<size_in) {   
      if(flag==2)   {
         if(tmp2==0)
            tmp2=1;
         else
            tmp2=0;
      }
      tmp1=(unsigned long)*((unsigned char *)tmp0);
      tmp0++;
      buf_in=tmp0;
      if(tmp1&0x80) {
         tmp1&=0x7F;
         tmp4=1;
         if(tmp1==0)   {
            if(var10[tmp2])
               var10[tmp2]--;
            if(var1c<2)
               return tmp3-var18;
            *((unsigned short int *)tmp3)=(unsigned short int)var8[tmp2];
            tmp3+=2;
            var1c-=2;
            if((tmp4==0)&&(flag==2)) {
               if(tmp2==0)
                  tmp2=1;
               else
                  tmp2=0;
            }
         } else {
            tmp1--;
            if(tmp1==0)   {
               var10[tmp2]+=8;
               tmp4=var10[tmp2];
               if(tmp4>0x58)
                  var10[tmp2]=0x58;
               tmp4=0;
               if(tmp4==0 && flag==2) {
                  if(tmp2==0)
                     tmp2=1;
                  else
                     tmp2=0;
               }
            } else {
               tmp1--;
               if(tmp1==0)   {
                  var10[tmp2]+=0xFFFFFFF8;
                  if((long)var10[tmp2]<0)
                     var10[tmp2]=0;
                  tmp4=0;
               }
               if((tmp4==0)&&(flag==2)) {
                  if(tmp2==0)
                     tmp2=1;
                  else
                     tmp2=0;
               }
            }
         }
      } else {
         tmp0=small_tbl1[var10[tmp2]];         
         tmp4=tmp0>>(*((unsigned char *)(var14+1)));
         if(tmp1&1)
            tmp4+=tmp0;
         if(tmp1&2)
            tmp4+=tmp0>>1;
         if(tmp1&4)
            tmp4+=tmp0>>2;
         if(tmp1&8)
            tmp4+=tmp0>>3;
         if(tmp1&0x10)
            tmp4+=tmp0>>4;
         if(tmp1&0x20)
            tmp4+=tmp0>>5;
         tmp0=var8[tmp2];
         if((tmp1&0xFF)&0x40) {
            tmp0-=tmp4;
            if((long)tmp0<=(long)0xFFFF8000)
               tmp0=0xFFFF8000;
         } else {
            tmp0+=tmp4;
            if((long)tmp0>=0x7FFF)
               tmp0=0x7FFF;
         }
         var8[tmp2]=tmp0;
         tmp0=var1c;
         if(tmp0<2)
            return tmp3-var18;
         *((unsigned short int *)tmp3)=(unsigned short int)var8[tmp2];
         tmp1&=0x1F;
         tmp3+=2;
         var1c-=2;
         tmp4=var10[tmp2]+small_tbl2[tmp1];
         var10[tmp2]=tmp4;
         if((long)tmp4<0)
            var10[tmp2]=0;
         else {
            if(tmp4>0x58)
               var10[tmp2]=0x58;
         }
         tmp0=buf_in;
      }
   }
   return tmp3-var18;
}

unsigned long ExtWavUnp1(unsigned long buf_in, unsigned long size_in, unsigned long buf_out, unsigned long size_out)
{
   unsigned long   work_buff,base;
   unsigned long   tmp1,tmp2,tmp3;
   long   i;
   work_buff=(unsigned long)(unsigned char *)malloc(0x3a80);
   *((unsigned long *)work_buff)=buf_in+4;
   *((unsigned long *)(work_buff+4))=*((unsigned long *)buf_in);
   *((unsigned long *)(work_buff+8))=0x20;
   base=work_buff+0xC;
   Sub_WavUnp12(base);
   size_out=Sub_WavUnp11(buf_out,size_out,work_buff,base);
   while(1) {
      tmp2=*((unsigned long *)(work_buff+0x3070));
      if((long)tmp2<=0)
         break;
      tmp3=Sub_WavUnp5(tmp2);
      *((unsigned long *)tmp3)=*((unsigned long *)tmp2);
      tmp3=*((unsigned long *)tmp2);
      *((unsigned long *)(tmp3+4))=*((unsigned long *)(tmp2+4));
      *((unsigned long *)tmp2)=0;
      *((unsigned long *)(tmp2+4))=0;
   }
   if(*((unsigned long *)(work_buff+0x306C))) {
      tmp3=Sub_WavUnp5(*((unsigned long *)(work_buff+0x306C)));
      *((unsigned long *)tmp3)=*((unsigned long *)(work_buff+0x306C));
      tmp3=*((unsigned long *)(work_buff+0x306C));
      *((unsigned long *)(tmp3+4))=*((unsigned long *)(work_buff+0x3070));
      *((unsigned long *)(work_buff+0x306C))=0;
      *((unsigned long *)(work_buff+0x3070))=0;
   }
   while(1) {
      tmp2=*((unsigned long *)(work_buff+0x3064));
      if((long)tmp2<=0)
         break;
      tmp3=Sub_WavUnp5(tmp2);
      *((unsigned long *)tmp3)=*((unsigned long *)tmp2);
      tmp3=*((unsigned long *)tmp2);
      *((unsigned long *)(tmp3+4))=*((unsigned long *)(tmp2+4));
      *((unsigned long *)tmp2)=0;
      *((unsigned long *)(tmp2+4))=0;
   }
   if(*((unsigned long *)(work_buff+0x3060))) {
      tmp3=Sub_WavUnp5(*((unsigned long *)(work_buff+0x3060)));
      *((unsigned long *)tmp3)=*((unsigned long *)(work_buff+0x3060));
      tmp3=*((unsigned long *)(work_buff+0x3060));
      *((unsigned long *)(tmp3+4))=*((unsigned long *)(work_buff+0x3064));
      *((unsigned long *)(work_buff+0x3060))=0;
      *((unsigned long *)(work_buff+0x3064))=0;
   }
   tmp2=work_buff+0x305C;
   for(i=0x203;i!=0;i--) {
      tmp3=*((unsigned long *)(tmp2-0x18));
      tmp2-=0x18;
      if(tmp3) {
         tmp3=Sub_WavUnp5(tmp2);
         *((unsigned long *)tmp3)=*((unsigned long *)tmp2);
         tmp3=*((unsigned long *)tmp2);
         *((unsigned long *)(tmp3+4))=*((unsigned long *)(tmp2+4));
         *((unsigned long *)tmp2)=0;
         *((unsigned long *)(tmp2+4))=0;
      }
      tmp1=*((unsigned long *)tmp2);
      if(tmp1) {
         tmp3=*((unsigned long *)(tmp2+4));
         if((long)tmp3<0)
            tmp3=~tmp3;
         else
            tmp3=tmp2+tmp3-*((unsigned long *)(tmp1+4));
         *((unsigned long *)tmp3)=tmp1;
         tmp3=*((unsigned long *)tmp2);
         *((unsigned long *)(tmp3+4))=*((unsigned long *)(tmp2+4));
         *((unsigned long *)tmp2)=0;
         *((unsigned long *)(tmp2+4))=0;
      }
   }
   free((unsigned char *)work_buff);
   return size_out;
}

void Sub_WavUnp12(unsigned long base)
{
   unsigned long   tmp;
   long   i;
   Sub_WavUnp6(base);
   tmp=base+0x3474;
   for(i=0x80;i!=0;i--) {
      *((unsigned long *) tmp)=0;
      tmp+=0xC;
   }
   return;
}

unsigned long Sub_WavUnp11(unsigned long buf_out,unsigned long size_out,unsigned long work_buff,unsigned long base)
{
   unsigned long   var4,var8,varc,var10,var14;
   unsigned long   flag,tmp0,tmp1,tmp2,tmp3,tmp4,tmp5;
   var4 = 0;
   var14 = 0;
   if(size_out==0)
      return 0;
   tmp0=work_buff;
   if(*((unsigned long *)(tmp0+8))<=8) {
      tmp3=*((unsigned long *)tmp0);
      *((unsigned long *)(tmp0+4))|=((unsigned long)*((unsigned short int *)tmp3)&0xFFFF)<<(*((unsigned long *)(tmp0+8))&0xFF);      
      *((unsigned long *)(tmp0+8))+=0x10;
      *((unsigned long *)tmp0)+=2;                                
   }
   *((unsigned long *)(tmp0+8))+=0xFFFFFFF8;
   varc=*((unsigned long *)(tmp0+4))&0xFF;
   *((unsigned long *)(tmp0+4))=*((unsigned long *)(tmp0+4))>>8;
   Sub_WavUnp1(varc,base);
   if((varc&0xFF)==0)
      *((unsigned long *)base)=1;
   else
      *((unsigned long *)base)=0;
   varc=buf_out;
   while(1) {
      if(*((unsigned long *)(tmp0+8))<=7) {
         tmp3=*((unsigned long *)tmp0);
         *((unsigned long *)(tmp0+4))|=((unsigned long)*((unsigned short int *)tmp3)&0xFFFF)<<(*((unsigned long *)(tmp0+8))&0xFF);      
         *((unsigned long *)(tmp0+8))+=0x10;
         *((unsigned long *)tmp0)+=2;
      }
      tmp3=*((unsigned long *)(tmp0+4));
      tmp4=tmp3&0x7F;               
      tmp2=base+(tmp4+0x45F)*12;
      tmp1=*((unsigned long *)tmp2);
      var8=0;
      flag=1;
      if(tmp1==*((unsigned long *)(base+4))) {
         var8=1;
         tmp5=*((unsigned long *)(tmp2+4));
         if(tmp5>7) {
            *((unsigned long *)(tmp0+4))=*((unsigned long *)(tmp0+4))>>7;
            *((unsigned long *)(tmp0+8))+=0xFFFFFFF9;
            var14=*((unsigned long *)(tmp2+8));
         } else {
            *((unsigned long *)(tmp0+4))=*((unsigned long *)(tmp0+4))>>(*((unsigned long *)(tmp2+4))&0xFF);
            *((unsigned long *)(tmp0+8))-=*((unsigned long *)(tmp2+4));
            tmp5=*((unsigned long *)(tmp2+8));
            flag=0;
         }
      } else {
         tmp5=*((unsigned long *)(base+0x3060));
         tmp3=*((unsigned long *)tmp5);
         var14=*((unsigned long *)(tmp3+4));
         tmp3=var14;
         if((long)var14<=0)
            var14=0;
      }
      if(flag) {
         var10=0;
         do {
            var14=*((unsigned long *)(var14+0x14));
            if(Sub_WavUnp8(tmp0)) {
               tmp3=*((unsigned long *)(var14+4));
               var14=tmp3;
            } else
               tmp3=var14;
            var10++;
            if(var10==7)
               var4=tmp3;
         } while(*((unsigned long *)(tmp3+0x14)));
         if(var8==0) {
            if(var10>7)   {
               *((unsigned long *)tmp2)=*((unsigned long *)(base+4));
               *((unsigned long *)(tmp2+4))=var10;
               *((unsigned long *)(tmp2+8))=var4;
            } else {
               tmp3=(((var10|0xFFFFFFFF)>>((0x20-var10)&0xFF)))&tmp4;
               tmp1=1<<(var10&0xFF);
               tmp5=base+0x347C+tmp3*12;
               tmp2=(tmp1*3)<<2;
               do {
                  tmp3+=tmp1;
                  *((unsigned long *)(tmp5-8))=*((unsigned long *)(base+4));
                  *((unsigned long *)(tmp5-4))=var10;
                  *((unsigned long *)tmp5)=*((unsigned long *)(var14+8));
                  tmp5+=tmp2;
               } while(tmp3<=0x7F);
            }
         }
         var10=*((unsigned long *)(var14+8));   
         tmp5=var10;
      }
      if(tmp5==0x101) {
         tmp5=*((unsigned long *)(tmp0+8));
         if(tmp5<=8)   {
            tmp3=*((unsigned long *)tmp0);
            *((unsigned long *)(tmp0+4))|=((unsigned long)*((unsigned short int *)tmp3)&0xFFFF)<<(*((unsigned long *)(tmp0+8))&0xFF);
            *((unsigned long *)(tmp0+8))+=0x10;
            *((unsigned long *)tmp0)+=2;
         }
         tmp2=*((unsigned long *)(tmp0+4))&0xFF;
         *((unsigned long *)(tmp0+4))=*((unsigned long *)(tmp0+4))>>8;
         *((unsigned long *)(tmp0+8))+=0xFFFFFFF8;         
         var10=tmp2;
         Sub_WavUnp9(tmp2,base);
         if(*((unsigned long *)base)==0) {
            tmp2=*((unsigned long *)(base+tmp2*4+0x306C));
            if(tmp2) {                           
               while(tmp2) {
                  tmp3=*((unsigned long *)(tmp2+0xC));
                  tmp3++;
                  *((unsigned long *)(tmp2+0xC))=tmp3;   
                  tmp0=tmp2;
                  tmp5=tmp3;
                  while(1) {
                     tmp4=*((unsigned long *)(tmp0+4));
                     if((long)tmp4<0)
                        tmp4=0;
                     if(tmp4) {
                        if(*((unsigned long *)(tmp4+0xC))<tmp5)
                           tmp0=tmp4;
                        else
                           break;
                     } else
                        break;
                  }                        
                  if(tmp0!=tmp2) {
                     Sub_WavUnp4(tmp0,2,tmp2,base+0x305C);
                     Sub_WavUnp4(tmp2,2,tmp4,base+0x305C);
                     tmp3=*((unsigned long *)(tmp0+0x10));
                     tmp5=*((unsigned long *)(tmp3+0x14));
                     tmp3=*((unsigned long *)(tmp2+0x10));
                     if(*((unsigned long *)(tmp3+0x14))==tmp2)
                        *((unsigned long *)(tmp3+0x14))=tmp0;
                     if(tmp5==tmp0) {
                        tmp5=*((unsigned long *)(tmp0+0x10));
                        *((unsigned long *)(tmp5+0x14))=tmp2;
                     }
                     tmp3=*((unsigned long *)(tmp2+0x10));
                     *((unsigned long *)(tmp2+0x10))=*((unsigned long *)(tmp0+0x10));
                     *((unsigned long *)(tmp0+0x10))=tmp3;
                     *((unsigned long *)(base+4))+=1;
                  }
                  tmp2=*((unsigned long *)(tmp2+0x10));
               }
               tmp5=var10;
               tmp0=work_buff;
            }
            else
               tmp5=var10;
         }
         else
            tmp5=var10;
      }
      tmp3=varc;
      if(tmp5!=0x100)                  
      {
         size_out--;
         *((unsigned char *)varc)=(unsigned char)tmp5&0xFF;
         tmp3++;
         varc=tmp3;
         if(size_out) {
            if(*((unsigned long *)base)) {
               tmp3=*((unsigned long *)(base+tmp5*4+0x306C));
               Sub_WavUnp10(tmp3,base);
            }
         } else
            return tmp3-buf_out;
      }
      else
         return tmp3-buf_out;
   }
}

void Sub_WavUnp1(unsigned long arg1,unsigned long base)
{
unsigned char wav_table[2512] =
{   0x0A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02,
   0x00, 0x00, 0x54, 0x16, 0x16, 0x0D, 0x0C, 0x08,
   0x06, 0x05, 0x06, 0x05, 0x06, 0x03, 0x04, 0x04,
   0x03, 0x05, 0x0E, 0x0B, 0x14, 0x13, 0x13, 0x09,
   0x0B, 0x06, 0x05, 0x04, 0x03, 0x02, 0x03, 0x02,
   0x02, 0x02, 0x0D, 0x07, 0x09, 0x06, 0x06, 0x04,
   0x03, 0x02, 0x04, 0x03, 0x03, 0x03, 0x03, 0x03,
   0x02, 0x02, 0x09, 0x06, 0x04, 0x04, 0x04, 0x04,
   0x03, 0x02, 0x03, 0x02, 0x02, 0x02, 0x02, 0x03,
   0x02, 0x04, 0x08, 0x03, 0x04, 0x07, 0x09, 0x05,
   0x03, 0x03, 0x03, 0x03, 0x02, 0x02, 0x02, 0x03,
   0x02, 0x02, 0x03, 0x02, 0x02, 0x02, 0x02, 0x02,
   0x02, 0x02, 0x02, 0x01, 0x01, 0x01, 0x02, 0x01,
   0x02, 0x02, 0x06, 0x0A, 0x08, 0x08, 0x06, 0x07,
   0x04, 0x03, 0x04, 0x04, 0x02, 0x02, 0x04, 0x02,
   0x03, 0x03, 0x04, 0x03, 0x07, 0x07, 0x09, 0x06,
   0x04, 0x03, 0x03, 0x02, 0x01, 0x02, 0x02, 0x02,
   0x02, 0x02, 0x0A, 0x02, 0x02, 0x03, 0x02, 0x02,
   0x01, 0x01, 0x02, 0x02, 0x02, 0x06, 0x03, 0x05,
   0x02, 0x03, 0x02, 0x01, 0x01, 0x01, 0x01, 0x01,
   0x01, 0x01, 0x01, 0x01, 0x01, 0x02, 0x03, 0x01,
   0x01, 0x01, 0x02, 0x01, 0x01, 0x01, 0x01, 0x01,
   0x01, 0x02, 0x04, 0x04, 0x04, 0x07, 0x09, 0x08,
   0x0C, 0x02, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
   0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x02, 0x01,
   0x01, 0x03, 0x04, 0x01, 0x02, 0x04, 0x05, 0x01,
   0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x02, 0x01,
   0x01, 0x01, 0x04, 0x01, 0x01, 0x01, 0x01, 0x01,
   0x02, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
   0x01, 0x01, 0x02, 0x01, 0x01, 0x01, 0x01, 0x01,
   0x01, 0x01, 0x03, 0x01, 0x01, 0x01, 0x01, 0x01,
   0x01, 0x01, 0x02, 0x01, 0x01, 0x01, 0x01, 0x01,
   0x01, 0x02, 0x02, 0x01, 0x01, 0x02, 0x02, 0x02,
   0x06, 0x4B, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x27, 0x00,
   0x00, 0x23, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0xFF, 0x01, 0x01, 0x01,
   0x01, 0x01, 0x01, 0x01, 0x02, 0x02, 0x01, 0x01,
   0x06, 0x0E, 0x10, 0x04, 0x06, 0x08, 0x05, 0x04,
   0x04, 0x03, 0x03, 0x02, 0x02, 0x03, 0x03, 0x01,
   0x01, 0x02, 0x01, 0x01, 0x01, 0x04, 0x02, 0x04,
   0x02, 0x02, 0x02, 0x01, 0x01, 0x04, 0x01, 0x01,
   0x02, 0x03, 0x03, 0x02, 0x03, 0x01, 0x03, 0x06,
   0x04, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x02,
   0x01, 0x02, 0x01, 0x01, 0x01, 0x29, 0x07, 0x16,
   0x12, 0x40, 0x0A, 0x0A, 0x11, 0x25, 0x01, 0x03,
   0x17, 0x10, 0x26, 0x2A, 0x10, 0x01, 0x23, 0x23,
   0x2F, 0x10, 0x06, 0x07, 0x02, 0x09, 0x01, 0x01,
   0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x0B,
   0x07, 0x05, 0x0B, 0x02, 0x02, 0x02, 0x06, 0x02,
   0x02, 0x01, 0x04, 0x02, 0x01, 0x03, 0x09, 0x01,
   0x01, 0x01, 0x03, 0x04, 0x01, 0x01, 0x02, 0x01,
   0x01, 0x01, 0x02, 0x01, 0x01, 0x01, 0x05, 0x01,
   0x01, 0x01, 0x0D, 0x01, 0x01, 0x01, 0x01, 0x01,
   0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x02, 0x01,
   0x01, 0x03, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
   0x01, 0x02, 0x01, 0x01, 0x01, 0x01, 0x0A, 0x04,
   0x02, 0x01, 0x06, 0x03, 0x02, 0x01, 0x01, 0x01,
   0x01, 0x01, 0x03, 0x01, 0x01, 0x01, 0x05, 0x02,
   0x03, 0x04, 0x03, 0x03, 0x03, 0x02, 0x01, 0x01,
   0x01, 0x02, 0x01, 0x02, 0x03, 0x03, 0x01, 0x03,
   0x01, 0x01, 0x02, 0x05, 0x01, 0x01, 0x04, 0x03,
   0x05, 0x01, 0x03, 0x01, 0x03, 0x03, 0x02, 0x01,
   0x04, 0x03, 0x0A, 0x06, 0x01, 0x01, 0x01, 0x01,
   0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x02, 0x02,
   0x01, 0x0A, 0x02, 0x05, 0x01, 0x01, 0x02, 0x07,
   0x02, 0x17, 0x01, 0x05, 0x01, 0x01, 0x0E, 0x01,
   0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
   0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
   0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
   0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
   0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
   0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x06, 0x02,
   0x01, 0x04, 0x05, 0x01, 0x01, 0x02, 0x01, 0x01,
   0x01, 0x01, 0x02, 0x01, 0x01, 0x01, 0x01, 0x01,
   0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
   0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
   0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x07, 0x01,
   0x01, 0x02, 0x01, 0x01, 0x01, 0x01, 0x02, 0x01,
   0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x02, 0x01,
   0x01, 0x01, 0x01, 0x01, 0x01, 0x11, 0x00, 0x00,
   0xFF, 0xFB, 0x98, 0x9A, 0x84, 0x85, 0x63, 0x64,
   0x3E, 0x3E, 0x22, 0x22, 0x13, 0x13, 0x18, 0x17,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0xFF, 0xF1, 0x9D, 0x9E, 0x9A, 0x9B,
   0x9A, 0x97, 0x93, 0x93, 0x8C, 0x8E, 0x86, 0x88,
   0x80, 0x82, 0x7C, 0x7C, 0x72, 0x73, 0x69, 0x6B,
   0x5F, 0x60, 0x55, 0x56, 0x4A, 0x4B, 0x40, 0x41,
   0x37, 0x37, 0x2F, 0x2F, 0x27, 0x27, 0x21, 0x21,
   0x1B, 0x1C, 0x17, 0x17, 0x13, 0x13, 0x10, 0x10,
   0x0D, 0x0D, 0x0B, 0x0B, 0x09, 0x09, 0x08, 0x08,
   0x07, 0x07, 0x06, 0x05, 0x05, 0x04, 0x04, 0x04,
   0x19, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0xC3, 0xCB, 0xF5, 0x41,
   0xFF, 0x7B, 0xF7, 0x21, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0xBF, 0xCC, 0xF2, 0x40,
   0xFD, 0x7C, 0xF7, 0x22, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x7A, 0x46, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC3, 0xD9,
   0xEF, 0x3D, 0xF9, 0x7C, 0xE9, 0x1E, 0xFD, 0xAB,
   0xF1, 0x2C, 0xFC, 0x5B, 0xFE, 0x17, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xBD, 0xD9,
   0xEC, 0x3D, 0xF5, 0x7D, 0xE8, 0x1D, 0xFB, 0xAE,
   0xF0, 0x2C, 0xFB, 0x5C, 0xFF, 0x18, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x70, 0x6C,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0xBA, 0xC5, 0xDA, 0x33, 0xE3, 0x6D, 0xD8, 0x18,
   0xE5, 0x94, 0xDA, 0x23, 0xDF, 0x4A, 0xD1, 0x10,
   0xEE, 0xAF, 0xE4, 0x2C, 0xEA, 0x5A, 0xDE, 0x15,
   0xF4, 0x87, 0xE9, 0x21, 0xF6, 0x43, 0xFC, 0x12,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0xB0, 0xC7, 0xD8, 0x33, 0xE3, 0x6B, 0xD6, 0x18,
   0xE7, 0x95, 0xD8, 0x23, 0xDB, 0x49, 0xD0, 0x11,
   0xE9, 0xB2, 0xE2, 0x2B, 0xE8, 0x5C, 0xDD, 0x15,
   0xF1, 0x87, 0xE7, 0x20, 0xF7, 0x44, 0xFF, 0x13,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x5F, 0x9E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x33, 0x00, 0x00, 0x00, 0x66, 0x00, 0x00, 0x00,
   0x3A, 0x00, 0x00, 0x00, 0x3A, 0x00, 0x00, 0x00,
   0x50, 0x00, 0x00, 0x00, 0x70, 0x00, 0x00, 0x00,
   0x3A, 0x00, 0x00, 0x00, 0x3A, 0x00, 0x00, 0x00,
   0x3A, 0x00, 0x00, 0x00, 0x3A, 0x00, 0x00, 0x00,
   0x4D, 0x00, 0x00, 0x00, 0x66, 0x00, 0x00, 0x00,
   0x80, 0x00, 0x00, 0x00, 0x9A, 0x00, 0x00, 0x00,
   0x3A, 0x00, 0x00, 0x00, 0x3A, 0x00, 0x00, 0x00,
   0x3A, 0x00, 0x00, 0x00, 0x3A, 0x00, 0x00, 0x00,
   0x3A, 0x00, 0x00, 0x00, 0x3A, 0x00, 0x00, 0x00,
   0x3A, 0x00, 0x00, 0x00, 0x3A, 0x00, 0x00, 0x00,
   0x3A, 0x00, 0x00, 0x00, 0x3A, 0x00, 0x00, 0x00,
   0x3A, 0x00, 0x00, 0x00, 0x3A, 0x00, 0x00, 0x00,
   0x3A, 0x00, 0x00, 0x00, 0x3A, 0x00, 0x00, 0x00,
   0x3A, 0x00, 0x00, 0x00, 0x3A, 0x00, 0x00, 0x00,
   0x46, 0x00, 0x00, 0x00, 0x53, 0x00, 0x00, 0x00,
   0x60, 0x00, 0x00, 0x00, 0x6D, 0x00, 0x00, 0x00,
   0x7A, 0x00, 0x00, 0x00, 0x86, 0x00, 0x00, 0x00,
   0x93, 0x00, 0x00, 0x00, 0xA0, 0x00, 0x00, 0x00,
   0xAD, 0x00, 0x00, 0x00, 0xBA, 0x00, 0x00, 0x00,
   0xC6, 0x00, 0x00, 0x00, 0xD3, 0x00, 0x00, 0x00,
   0xE0, 0x00, 0x00, 0x00, 0xED, 0x00, 0x00, 0x00,
   0xFA, 0x00, 0x00, 0x00, 0x06, 0x01, 0x00, 0x00
};
   unsigned long   var_4,var_8,var_c;
   unsigned long   tmp0,tmp1,tmp2,tmp3,tmp4;
   long   i;
   while((long)*((unsigned long *)(base+0x3064))>0) {       
      tmp0=Sub_WavUnp2(0,*((unsigned long *)(base+0x3050)),*((unsigned long *)(base+0x3064)));
      if(*((unsigned long *) tmp0))
         Sub_WavUnp3(tmp0);
      tmp2=base+0x3054;
      *((unsigned long *)(tmp0))=tmp2;
      *((unsigned long *)(tmp0+4))=*((unsigned long *)(tmp2+4));
      tmp3=*((unsigned long *)(tmp2+4));
      if((long)tmp3<=0)
         tmp3=~tmp3;
      tmp3=Sub_WavUnp2(0,*((unsigned long *)(base+0x3050)),tmp3);
      *((unsigned long *)tmp3)=(unsigned long)tmp0;
      *((unsigned long *)(tmp2+4))=base;
   }
   tmp0=base+0x306c;
   for(i=0x102;i!=0;i--) {
      *((unsigned long *)tmp0)=0;
      tmp0+=4;
   }
   var_c=0;
   var_8=base+0x306C;
   tmp1=(unsigned long)wav_table+(((arg1&0xff)<<7)+(arg1&0xff))*2;
   var_4=tmp1;
   tmp0=0;
   while(tmp0<0x100) {
      if(*((unsigned char *)(tmp0+tmp1))) {   
         tmp2=*((unsigned long *)(base+0x3058));
         if((long)tmp2<=0) {
            tmp2=*((unsigned long *)(base+0x3068))*24+base+8;
            *((unsigned long *)(base+0x3068))=*((unsigned long *)(base+0x3068))+1;
         }
         Sub_WavUnp4(tmp2,2,0,base+0x305C);
         *((unsigned long *)(tmp2+0x10))=0;
         *((unsigned long *)(tmp2+0x14))=0;
         *((unsigned long *)var_8)=tmp2;
         *((unsigned long *)(tmp2+8))=tmp0;
         tmp3=*((unsigned long *)(tmp0+tmp1))&0xFF;
         *((unsigned long *)(tmp2+0xC))=tmp3;
         if(tmp3>=var_c)
            var_c=tmp3;
         else {
            tmp1=*((unsigned long *)(base+0x3064));
            if((long)tmp1>0) {
               while(tmp1)   {
                  if(*((unsigned long *)(tmp1+0xC))<tmp3)
                     tmp1=*((unsigned long *)(tmp1+4));
                  else
                     break;
               }
            } else
               tmp1=0;
            arg1=Sub_WavUnp2(0,*((unsigned long *)(base+0x305C)),tmp2);
            if(*((unsigned long *)arg1))
               Sub_WavUnp3(arg1);
            if(tmp1)
               tmp3=Sub_WavUnp2(0,*((unsigned long *)(base+0x305C)),tmp1);
            else
               tmp3=base+0x3060;
            tmp4=*((unsigned long *)tmp3);
            *((unsigned long *)arg1)=tmp4;
            *((unsigned long *)(arg1+4))=*((unsigned long *)(tmp4+4));
            tmp1=var_4;
            *((unsigned long *)(tmp4+4))=tmp2;
            *((unsigned long *)tmp3)=arg1;
         }
      }
      var_8+=4;
      tmp0++;
   }
   if(tmp0<0x102) {
      tmp1=base+tmp0*4+0x306C;
      while(tmp0<0x102) {
         tmp2=*((unsigned long *)(base+0x3058));
         if((long)tmp2<=0)   {
            tmp2=base+*((unsigned long *)(base+0x3068))*24+8;
            *((unsigned long *)(base+0x3068))=*((unsigned long *)(base+0x3068))+1;
            
         }
         Sub_WavUnp4(tmp2,1,0,base+0x305C);
         tmp1+=4;
         *((unsigned long *)(tmp2+0x10))=0;
         *((unsigned long *)(tmp2+0x14))=0;
         *((unsigned long *)(tmp1-4))=tmp2;
         *((unsigned long *)(tmp2+8))=tmp0;
         tmp0++;
         *((unsigned long *)(tmp2+0xC))=1;
      }
   }
   tmp0=*((unsigned long *)(base+0x3064));
   if((long)tmp0<=0)
      tmp0=0;
   while(tmp0) {
      tmp1=*((unsigned long *)(tmp0+4));
      if((long)tmp1<=0)
         tmp1=0;
      arg1=tmp1;
      if(tmp1) {
         tmp2=*((unsigned long *)(base+0x3058));
         if((long)tmp2<=0)   {
            tmp2=base+*((unsigned long *)(base+0x3068))*24+8;
            *((unsigned long *)(base+0x3068))=*((unsigned long *)(base+0x3068))+1;
         }
         Sub_WavUnp4(tmp2,2,0,base+0x305C);
         *((unsigned long *)(tmp2+0x10))=0;
         *((unsigned long *)(tmp2+0x14))=0;
         *((unsigned long *)(tmp2+0x14))=tmp0;
         *((unsigned long *)(tmp2+0xC))=*((unsigned long *)(tmp1+0xC))+*((unsigned long *)(tmp0+0xC));
         *((unsigned long *)(tmp0+0x10))=tmp2;
         *((unsigned long *)(tmp1+0x10))=tmp2;
         tmp3=*((unsigned long *)(tmp2+0xC));
         if(tmp3>=var_c)
            var_c=tmp3;
         else {
            tmp1=*((unsigned long *)(tmp1+4));
            tmp0=tmp1;
            if((long)tmp0<=0)
               tmp0=0;
            while(tmp0)   {
               if(*((unsigned long *)(tmp0+0xC))<tmp3) {
                  tmp0=*((unsigned long *)(tmp0+4));
                  if((long)tmp0<=0)
                     tmp0=0;
               }
               else
                  break;
            }
            tmp1=Sub_WavUnp2(0,*((unsigned long *)(base+0x305C)),tmp2);
            if(*((unsigned long *)(tmp1))) {
               tmp3=Sub_WavUnp5(tmp1);
               *((unsigned long *)(tmp3))=*((unsigned long *)(tmp1));
               tmp4=*((unsigned long *)tmp1);
               *((unsigned long *)(tmp4+4))=*((unsigned long *)(tmp1+4));
               *((unsigned long *)tmp1)=0;
               *((unsigned long *)(tmp1+4))=0;
            }
            if(tmp0)
               tmp3=Sub_WavUnp2(0,*((unsigned long *)(base+0x305C)),tmp0);
            else
               tmp3=base+0x3060;
            tmp4=*((unsigned long *)(tmp3));
            *((unsigned long *)(tmp1))=tmp4;
            *((unsigned long *)(tmp1+4))=*((unsigned long *)(tmp4+4));
            *((unsigned long *)(tmp4+4))=tmp2;
            *((unsigned long *)(tmp3))=tmp1;
            tmp1=arg1;
         }
         tmp0=*((unsigned long *)(tmp1+4));
         if((long)tmp0<=0)
            tmp0=0;
      } else
         break;
   }
   *((unsigned long *)(base+4))=1;
   return;      
}

void Sub_WavUnp9(unsigned long arg1,unsigned long arg2)
{
   unsigned long   var4,var8,tmp0,tmp1,tmp2,tmp3,tmp4;
   tmp0=*((unsigned long *)(arg2+0x3064));
   if((long)tmp0<0)
      tmp0=0;
   tmp2=*((unsigned long *)(arg2+0x3058));
   if((long)tmp2<0)
      tmp2=0;
   var4=arg2;
   if(tmp2==0) {
      tmp2=var4+*((unsigned long *)(var4+0x3068))*24+8;
      *((unsigned long *)(var4+0x3068))+=1;
   }
   var8=arg2+0x305C;
   tmp1=Sub_WavUnp2(0,*((unsigned long *)var8),tmp2);
   if(*((unsigned long *)tmp1)) {
      tmp3=Sub_WavUnp5(tmp1);
      *((unsigned long *)tmp3)=*((unsigned long *)tmp1);
      tmp3=*((unsigned long *)tmp1);
      *((unsigned long *)(tmp3+4))=*((unsigned long *)(tmp1+4));
      *((unsigned long *)tmp1)=0;
      *((unsigned long *)(tmp1+4))=0;
   }
   *((unsigned long *)tmp1)=var8+4;
   *((unsigned long *)(tmp1+4))=*((unsigned long *)(var8+8));
   tmp3=Sub_WavUnp2(0,*((unsigned long *)var8),Sub_WavUnp7(var8+4));
   *((unsigned long *)tmp3)=tmp1;
   *((unsigned long *)(var8+8))=tmp2;
   *((unsigned long *)(tmp2+0x10))=0;
   *((unsigned long *)(tmp2+0x14))=0;
   *((unsigned long *)(tmp2+8))=*((unsigned long *)(tmp0+8));
   *((unsigned long *)(tmp2+0xC))=*((unsigned long *)(tmp0+0xC));
   *((unsigned long *)(tmp2+0x10))=tmp0;
   *((unsigned long *)(var4+*((unsigned long *)(tmp0+8))*4+0x306C))=tmp2;
   tmp2=*((unsigned long *)(var4+0x3058));
   if((long)tmp2<0)
      tmp2=0;
   if(tmp2==0)   {
      tmp2=var4+*((unsigned long *)(var4+0x3068))*24+8;
      *((unsigned long *)(var4+0x3068))+=1;
   }
   tmp1=Sub_WavUnp2(0,*((unsigned long *)var8),tmp2);
   if(*((unsigned long *)tmp1)) {
      tmp3=Sub_WavUnp5(tmp1);
      *((unsigned long *)tmp3)=*((unsigned long *)tmp1);
      tmp3=*((unsigned long *)tmp1);
      *((unsigned long *)(tmp3+4))=*((unsigned long *)(tmp1+4));
      *((unsigned long *)tmp1)=0;
      *((unsigned long *)(tmp1+4))=0;
   }
   *((unsigned long *)tmp1)=var8+4;
   *((unsigned long *)(tmp1+4))=*((unsigned long *)(var8+8));
   tmp3=Sub_WavUnp2(0,*((unsigned long *)var8),Sub_WavUnp7(var8+4));
   *((unsigned long *)tmp3)=tmp1;
   tmp3=arg1;
   *((unsigned long *)(var8+8))=tmp2;
   *((unsigned long *)(tmp2+0x14))=0;
   *((unsigned long *)(tmp2+8))=tmp3;
   *((unsigned long *)(tmp2+0xC))=0;
   *((unsigned long *)(tmp2+0x10))=tmp0;
   *((unsigned long *)(var4+tmp3*4+0x306C))=tmp2;
   *((unsigned long *)(tmp0+0x14))=tmp2;
   while(tmp2) {
      tmp4=tmp2;
      *((unsigned long *)(tmp2+0xC))+=1;
      tmp3=*((unsigned long *)(tmp2+0xC));
      while(1) {
         tmp1=*((unsigned long *)(tmp4+4));
         if((long)tmp1<=0)   {
            tmp1=0;
            break;
         }
         if(*((unsigned long *)(tmp1+0xC))>=tmp3)
            break;
         tmp4=tmp1;
      }
      if(tmp4!=tmp2) {
         Sub_WavUnp4(tmp4,2,tmp2,var8);
         Sub_WavUnp4(tmp2,2,tmp1,var8);
         tmp3=*((unsigned long *)(tmp4+0x10));
         tmp1=*((unsigned long *)(tmp2+0x10));
         tmp0=*((unsigned long *)(tmp3+0x14));
         tmp3=*((unsigned long *)(tmp1+0x14));
         if(tmp3==tmp2)
            *((unsigned long *)(tmp1+0x14))=tmp4;
         if(tmp0==tmp4) {
            tmp1=*((unsigned long *)(tmp4+0x10));
            *((unsigned long *)(tmp1+0x14))=tmp2;
         }
         tmp3=*((unsigned long *)(tmp2+0x10));
         *((unsigned long *)(tmp2+0x10))=*((unsigned long *)(tmp4+0x10));
         *((unsigned long *)(tmp4+0x10))=tmp3;
         *((unsigned long *)(var4+4))+=1;
      }
      tmp2=*((unsigned long *)(tmp2+0x10));
   }
   return;
}

void Sub_WavUnp6(unsigned long ptr_base)
{
   unsigned long   ptr1;
   long   i;
   ptr1=ptr_base+8;            
   for(i=0x203;i!=0;i--) {      
      *((unsigned long *)ptr1)=0;
      *((unsigned long *)(ptr1+4))=0;
      ptr1+=0x18;            
   }
   ptr1=ptr_base+0x3054;
   *((unsigned long *)ptr1)=0;
   *((unsigned long *)(ptr1+4))=0;
   *((unsigned long *)ptr1)=ptr1;
   *((unsigned long *)(ptr_base+0x3050))=0;
   *((unsigned long *)(ptr_base+0x3058))=~ptr1;
   ptr1=ptr_base+0x3060;
   *((unsigned long *)ptr1)=0;
   *((unsigned long *)(ptr1+4))=0;
   *((unsigned long *)ptr1)=(unsigned long) ptr1;
   *((unsigned long *)(ptr_base+0x305C))=0;
   *((unsigned long *)(ptr_base+0x3064))=~ptr1;
   *((unsigned long *)(ptr_base+0x3068))=0;
   *((unsigned long *)(ptr_base+4))=1;
   return;
}

unsigned long Sub_WavUnp2(unsigned long arg1,unsigned long arg2,unsigned long arg3)
{
   if(arg1)
      return arg3+arg2;
   return arg3;
}

void Sub_WavUnp3(unsigned long ptr)  
{
   unsigned long ptr0,ptr1;
   ptr0=*((unsigned long *)ptr);
   if(ptr0) {
      ptr1=*((unsigned long *)(ptr+4));
      if((long)ptr1<0)
          ptr1=~ptr1;
      else
         ptr1=ptr1+ptr-*((unsigned long *)(ptr0+4));
      *((unsigned long *)ptr1)=ptr0;
      *((unsigned long *)(ptr0+4))=*((unsigned long *)(ptr+4));
      *((unsigned long *)ptr)=0;
      *((unsigned long *)(ptr+4))=0;
   }
   return;
}

void Sub_WavUnp4(unsigned long arg1,unsigned long arg2,unsigned long arg3,unsigned long arg4)
{
   unsigned long   tmp0,tmp1;
   tmp0=*((unsigned long *)arg1);
   if(tmp0) {
      tmp1=*((unsigned long *)(arg1+4));
      if((long)tmp1<0)
         tmp1=~tmp1;
      else
         tmp1+=arg1-*((unsigned long *)(tmp0+4));
      *((unsigned long *)tmp1)=(unsigned long)tmp0;
      *((unsigned long *)(tmp0+4))=*((unsigned long *)(arg1+4));
      *((unsigned long *)arg1)=0;
      *((unsigned long *)(arg1+4))=0;
   }
   if(arg3==0)
      arg3=arg4+4;
   if(arg2-1) {
      if((arg2-2)==0)   {
         tmp1=*((unsigned long *)arg3);
         *((unsigned long *)arg1)=tmp1;
         *((unsigned long *)(arg1+4))=*((unsigned long *)(tmp1+4));
         *((unsigned long *)(tmp1+4))=arg1;
         *((unsigned long *)arg3)=arg1;
      }
      return;
   }
   *((unsigned long *)arg1)=arg3;
   *((unsigned long *)(arg1+4))=*((unsigned long *)(arg3+4));
   tmp1=*((unsigned long *)(arg3+4));
   if((long)tmp1<=0)
      tmp1=~tmp1;
   *((unsigned long *)tmp1)=arg1;
   *((unsigned long *)(arg3+4))=arg1;
   return;
}

unsigned long Sub_WavUnp5(unsigned long arg1)
{
   unsigned long tmp0,tmp1;
   tmp0=*((unsigned long *)(arg1+4));
   if((long)tmp0<0)
      return ~tmp0;
   tmp1=*((unsigned long *)arg1);
   tmp0+=arg1-*((unsigned long *)(tmp1+4));
   return tmp0;
}

unsigned long Sub_WavUnp7(unsigned long arg1)
{
   unsigned long tmp;
   tmp=*((unsigned long *)(arg1+4));
   if((long)tmp<=0)
      return ~tmp;
   return tmp;
}

unsigned long Sub_WavUnp8(unsigned long arg1)
{
   unsigned long   tmp0,tmp1;
   tmp0=*((unsigned long *)(arg1+4));
   *((unsigned long *)(arg1+4))=tmp0>>1;
   tmp0&=1;
   *((unsigned long *)(arg1+8))-=1;
   if(*((unsigned long *)(arg1+8))==0) {
      tmp1=*((unsigned long *)arg1);
      *((unsigned long *)arg1)+=4;
      *((unsigned long *)(arg1+4))=*((unsigned long *)tmp1);
      *((unsigned long *)(arg1+8))=0x20;
   }
   return tmp0;
}

void Sub_WavUnp10(unsigned long arg1,unsigned long arg2)
{
   unsigned long   tmp0,tmp1,tmp2,tmp3,arg2_old;
   arg2_old=arg2;
   while(arg1) {
      tmp2=arg1;
      *((unsigned long *)(arg1+0xC))+=1;
      tmp3=*((unsigned long *)(arg1+0xC));
      while(1) {
         tmp0=*((unsigned long *)(tmp2+4));
         if((long)tmp0<=0) {
            tmp0=0;
            break;
         }
         if(*((unsigned long *)(tmp0+0xC))>=tmp3)
            break;
         tmp2=tmp0;
      }
      if(tmp2!=arg1) {
         if(*((unsigned long *)tmp2)) {
            tmp3=Sub_WavUnp5(tmp2);
            *((unsigned long *)tmp3)=*((unsigned long *)tmp2);
            tmp1=*((unsigned long *)tmp2);
            *((unsigned long *)(tmp1+4))=*((unsigned long *)(tmp2+4));
            *((unsigned long *)tmp2)=0;
            *((unsigned long *)(tmp2+4))=0;
         }
         tmp1=arg1;
         if(tmp1==0)
            tmp1=arg2+0x3060;
         tmp3=*((unsigned long *)tmp1);
         *((unsigned long *)tmp2)=tmp3;
         *((unsigned long *)(tmp2+4))=*((unsigned long *)(tmp3+4));
         *((unsigned long *)tmp1)=tmp2;
         arg2=Sub_WavUnp2(0,*((unsigned long *)(arg2+0x205C)),arg1);
         if(*((unsigned long *)arg2)) {
            tmp3=Sub_WavUnp5(arg2);
            *((unsigned long *)tmp3)=*((unsigned long *)arg2);
            tmp1=*((unsigned long *)arg2);
            *((unsigned long *)(tmp1+4))=*((unsigned long *)(arg2+4));
            *((unsigned long *)arg2)=0;
            *((unsigned long *)(arg2+4))=0;
         }
         if(tmp0)
            tmp3=Sub_WavUnp2(0,*((unsigned long *)(arg2_old+0x305C)),tmp0);
         else
            tmp3=arg2_old+0x3060;
         tmp1=*((unsigned long *)tmp3);
         *((unsigned long *)arg2)=tmp1;
         tmp0=*((unsigned long *)(tmp1+4));
         *((unsigned long *)(arg2+4))=tmp0;
         *((unsigned long *)(tmp1+4))=arg1;
         *((unsigned long *)tmp3)=arg2;
         tmp3=*((unsigned long *)(tmp2+0x10));
         tmp1=*((unsigned long *)(tmp3+0x14));
         tmp3=*((unsigned long *)(arg1+0x10));
         if(*((unsigned long *)(tmp3+0x14))==arg1)
            *((unsigned long *)(tmp3+0x14))=tmp2;
         if(tmp1==tmp2) {
            tmp1=*((unsigned long *)(tmp2+0x10));
            *((unsigned long *)(tmp1+0x14))=arg1;
         }
         tmp3=*((unsigned long *)(arg1+0x10));
         *((unsigned long *)(arg1+0x10))=*((unsigned long *)(tmp2+0x10));
         *((unsigned long *)(tmp2+0x10))=tmp3;
         arg2=arg2_old;
         *((unsigned long *)(arg2_old+4))+=1;
      }
      arg1=*((unsigned long *)(arg1+0x10));
   }
   return;
}

unsigned long explode(read_data_proc read_data, write_data_proc write_data, void * param)
{   
   unsigned long      result;
   unsigned short int      read_result;
const unsigned char table[] =
{   0x02, 0x04, 0x04, 0x05, 0x05, 0x05, 0x05, 0x06,
   0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06,
   0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x07, 0x07,
   0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07,
   0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07,
   0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07,
   0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08,
   0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08,
   0x03, 0x0D, 0x05, 0x19, 0x09, 0x11, 0x01, 0x3E,
   0x1E, 0x2E, 0x0E, 0x36, 0x16, 0x26, 0x06, 0x3A,
   0x1A, 0x2A, 0x0A, 0x32, 0x12, 0x22, 0x42, 0x02,
   0x7C, 0x3C, 0x5C, 0x1C, 0x6C, 0x2C, 0x4C, 0x0C,
   0x74, 0x34, 0x54, 0x14, 0x64, 0x24, 0x44, 0x04,
   0x78, 0x38, 0x58, 0x18, 0x68, 0x28, 0x48, 0x08,
   0xF0, 0x70, 0xB0, 0x30, 0xD0, 0x50, 0x90, 0x10,
   0xE0, 0x60, 0xA0, 0x20, 0xC0, 0x40, 0x80, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
   0x00, 0x00, 0x01, 0x00, 0x02, 0x00, 0x03, 0x00,
   0x04, 0x00, 0x05, 0x00, 0x06, 0x00, 0x07, 0x00,
   0x08, 0x00, 0x0A, 0x00, 0x0E, 0x00, 0x16, 0x00,
   0x26, 0x00, 0x46, 0x00, 0x86, 0x00, 0x06, 0x01,
   0x03, 0x02, 0x03, 0x03, 0x04, 0x04, 0x04, 0x05,
   0x05, 0x05, 0x05, 0x06, 0x06, 0x06, 0x07, 0x07,
   0x05, 0x03, 0x01, 0x06, 0x0A, 0x02, 0x0C, 0x14,
   0x04, 0x18, 0x08, 0x30, 0x10, 0x20, 0x40, 0x00,
   0x0B, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C,
   0x0C, 0x08, 0x07, 0x0C, 0x0C, 0x07, 0x0C, 0x0C,
   0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C,
   0x0C, 0x0C, 0x0D, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C,
   0x04, 0x0A, 0x08, 0x0C, 0x0A, 0x0C, 0x0A, 0x08,
   0x07, 0x07, 0x08, 0x09, 0x07, 0x06, 0x07, 0x08,
   0x07, 0x06, 0x07, 0x07, 0x07, 0x07, 0x08, 0x07,
   0x07, 0x08, 0x08, 0x0C, 0x0B, 0x07, 0x09, 0x0B,
   0x0C, 0x06, 0x07, 0x06, 0x06, 0x05, 0x07, 0x08,
   0x08, 0x06, 0x0B, 0x09, 0x06, 0x07, 0x06, 0x06,
   0x07, 0x0B, 0x06, 0x06, 0x06, 0x07, 0x09, 0x08,
   0x09, 0x09, 0x0B, 0x08, 0x0B, 0x09, 0x0C, 0x08,
   0x0C, 0x05, 0x06, 0x06, 0x06, 0x05, 0x06, 0x06,
   0x06, 0x05, 0x0B, 0x07, 0x05, 0x06, 0x05, 0x05,
   0x06, 0x0A, 0x05, 0x05, 0x05, 0x05, 0x08, 0x07,
   0x08, 0x08, 0x0A, 0x0B, 0x0B, 0x0C, 0x0C, 0x0C,
   0x0D, 0x0D, 0x0D, 0x0D, 0x0D, 0x0D, 0x0D, 0x0D,
   0x0D, 0x0D, 0x0D, 0x0D, 0x0D, 0x0D, 0x0D, 0x0D,
   0x0D, 0x0D, 0x0D, 0x0D, 0x0D, 0x0D, 0x0D, 0x0D,
   0x0D, 0x0D, 0x0D, 0x0D, 0x0D, 0x0D, 0x0D, 0x0D,
   0x0D, 0x0D, 0x0D, 0x0D, 0x0D, 0x0D, 0x0D, 0x0D,
   0x0D, 0x0D, 0x0D, 0x0D, 0x0D, 0x0D, 0x0D, 0x0D,
   0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C,
   0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C,
   0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C,
   0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C,
   0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C,
   0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C,
   0x0D, 0x0C, 0x0D, 0x0D, 0x0D, 0x0C, 0x0D, 0x0D,
   0x0D, 0x0C, 0x0D, 0x0D, 0x0D, 0x0D, 0x0C, 0x0D,
   0x0D, 0x0D, 0x0C, 0x0C, 0x0C, 0x0D, 0x0D, 0x0D,
   0x0D, 0x0D, 0x0D, 0x0D, 0x0D, 0x0D, 0x0D, 0x0D,
   0x90, 0x04, 0xE0, 0x0F, 0xE0, 0x07, 0xE0, 0x0B,
   0xE0, 0x03, 0xE0, 0x0D, 0xE0, 0x05, 0xE0, 0x09,
   0xE0, 0x01, 0xB8, 0x00, 0x62, 0x00, 0xE0, 0x0E,
   0xE0, 0x06, 0x22, 0x00, 0xE0, 0x0A, 0xE0, 0x02,
   0xE0, 0x0C, 0xE0, 0x04, 0xE0, 0x08, 0xE0, 0x00,
   0x60, 0x0F, 0x60, 0x07, 0x60, 0x0B, 0x60, 0x03,
   0x60, 0x0D, 0x60, 0x05, 0x40, 0x12, 0x60, 0x09,
   0x60, 0x01, 0x60, 0x0E, 0x60, 0x06, 0x60, 0x0A,
   0x0F, 0x00, 0x50, 0x02, 0x38, 0x00, 0x60, 0x02,
   0x50, 0x00, 0x60, 0x0C, 0x90, 0x03, 0xD8, 0x00,
   0x42, 0x00, 0x02, 0x00, 0x58, 0x00, 0xB0, 0x01,
   0x7C, 0x00, 0x29, 0x00, 0x3C, 0x00, 0x98, 0x00,
   0x5C, 0x00, 0x09, 0x00, 0x1C, 0x00, 0x6C, 0x00,
   0x2C, 0x00, 0x4C, 0x00, 0x18, 0x00, 0x0C, 0x00,
   0x74, 0x00, 0xE8, 0x00, 0x68, 0x00, 0x60, 0x04,
   0x90, 0x00, 0x34, 0x00, 0xB0, 0x00, 0x10, 0x07,
   0x60, 0x08, 0x31, 0x00, 0x54, 0x00, 0x11, 0x00,
   0x21, 0x00, 0x17, 0x00, 0x14, 0x00, 0xA8, 0x00,
   0x28, 0x00, 0x01, 0x00, 0x10, 0x03, 0x30, 0x01,
   0x3E, 0x00, 0x64, 0x00, 0x1E, 0x00, 0x2E, 0x00,
   0x24, 0x00, 0x10, 0x05, 0x0E, 0x00, 0x36, 0x00,
   0x16, 0x00, 0x44, 0x00, 0x30, 0x00, 0xC8, 0x00,
   0xD0, 0x01, 0xD0, 0x00, 0x10, 0x01, 0x48, 0x00,
   0x10, 0x06, 0x50, 0x01, 0x60, 0x00, 0x88, 0x00,
   0xA0, 0x0F, 0x07, 0x00, 0x26, 0x00, 0x06, 0x00,
   0x3A, 0x00, 0x1B, 0x00, 0x1A, 0x00, 0x2A, 0x00,
   0x0A, 0x00, 0x0B, 0x00, 0x10, 0x02, 0x04, 0x00,
   0x13, 0x00, 0x32, 0x00, 0x03, 0x00, 0x1D, 0x00,
   0x12, 0x00, 0x90, 0x01, 0x0D, 0x00, 0x15, 0x00,
   0x05, 0x00, 0x19, 0x00, 0x08, 0x00, 0x78, 0x00,
   0xF0, 0x00, 0x70, 0x00, 0x90, 0x02, 0x10, 0x04,
   0x10, 0x00, 0xA0, 0x07, 0xA0, 0x0B, 0xA0, 0x03,
   0x40, 0x02, 0x40, 0x1C, 0x40, 0x0C, 0x40, 0x14,
   0x40, 0x04, 0x40, 0x18, 0x40, 0x08, 0x40, 0x10,
   0x40, 0x00, 0x80, 0x1F, 0x80, 0x0F, 0x80, 0x17,
   0x80, 0x07, 0x80, 0x1B, 0x80, 0x0B, 0x80, 0x13,
   0x80, 0x03, 0x80, 0x1D, 0x80, 0x0D, 0x80, 0x15,
   0x80, 0x05, 0x80, 0x19, 0x80, 0x09, 0x80, 0x11,
   0x80, 0x01, 0x80, 0x1E, 0x80, 0x0E, 0x80, 0x16,
   0x80, 0x06, 0x80, 0x1A, 0x80, 0x0A, 0x80, 0x12,
   0x80, 0x02, 0x80, 0x1C, 0x80, 0x0C, 0x80, 0x14,
   0x80, 0x04, 0x80, 0x18, 0x80, 0x08, 0x80, 0x10,
   0x80, 0x00, 0x00, 0x1F, 0x00, 0x0F, 0x00, 0x17,
   0x00, 0x07, 0x00, 0x1B, 0x00, 0x0B, 0x00, 0x13,
   0xA0, 0x0D, 0xA0, 0x05, 0xA0, 0x09, 0xA0, 0x01,
   0xA0, 0x0E, 0xA0, 0x06, 0xA0, 0x0A, 0xA0, 0x02,
   0xA0, 0x0C, 0xA0, 0x04, 0xA0, 0x08, 0xA0, 0x00,
   0x20, 0x0F, 0x20, 0x07, 0x20, 0x0B, 0x20, 0x03,
   0x20, 0x0D, 0x20, 0x05, 0x20, 0x09, 0x20, 0x01,
   0x20, 0x0E, 0x20, 0x06, 0x20, 0x0A, 0x20, 0x02,
   0x20, 0x0C, 0x20, 0x04, 0x20, 0x08, 0x20, 0x00,
   0xC0, 0x0F, 0xC0, 0x07, 0xC0, 0x0B, 0xC0, 0x03,
   0xC0, 0x0D, 0xC0, 0x05, 0xC0, 0x09, 0xC0, 0x01,
   0xC0, 0x0E, 0xC0, 0x06, 0xC0, 0x0A, 0xC0, 0x02,
   0xC0, 0x0C, 0xC0, 0x04, 0xC0, 0x08, 0xC0, 0x00,
   0x40, 0x0F, 0x40, 0x07, 0x40, 0x0B, 0x40, 0x03,
   0x00, 0x03, 0x40, 0x0D, 0x00, 0x1D, 0x00, 0x0D,
   0x00, 0x15, 0x40, 0x05, 0x00, 0x05, 0x00, 0x19,
   0x00, 0x09, 0x40, 0x09, 0x00, 0x11, 0x00, 0x01,
   0x00, 0x1E, 0x00, 0x0E, 0x40, 0x01, 0x00, 0x16,
   0x00, 0x06, 0x00, 0x1A, 0x40, 0x0E, 0x40, 0x06,
   0x40, 0x0A, 0x00, 0x0A, 0x00, 0x12, 0x00, 0x02,
   0x00, 0x1C, 0x00, 0x0C, 0x00, 0x14, 0x00, 0x04,
   0x00, 0x18, 0x00, 0x08, 0x00, 0x10, 0x00, 0x00
};
   unsigned char       * work_buff;
   work_buff = (unsigned char *) glb_mpq->explode_buffer;
   *((unsigned long *) (work_buff + 0x16)) = (unsigned long) read_data;
   *((unsigned long *) (work_buff + 0x1A)) = (unsigned long) write_data;
   *((void **) (work_buff + 0x12)) = param;
   *((unsigned short int *) (work_buff + 0x0E)) = 0x0800;
   read_result = read_data(work_buff + 0x2222, 0x0800, param);
   if (read_result == DCL_ERROR_4)
   {
      result = DCL_ERROR_3;
   }
   else
   {
      unsigned short int   flag_0 = *(work_buff + 0x2222),
            flag_1 = *(work_buff + 0x2223),
            flag_2 = *(work_buff + 0x2224);
      *((unsigned short int *) (work_buff + 0x02)) = flag_0;
      *((unsigned short int *) (work_buff + 0x06)) = flag_1;
      *((unsigned short int *) (work_buff + 0x0A)) = flag_2;
      *((unsigned short int *) (work_buff + 0x0C)) = 0x00;
      *((unsigned short int *) (work_buff + 0x0E)) = 0x03;
      *((unsigned short int *) (work_buff + 0x10)) = read_result;
      if ((flag_1 < 0x04) || (flag_1 > 0x06))
      {
         result = DCL_ERROR_1;
      }
      else
      {
          *((unsigned short int *) (work_buff + 0x08)) = (unsigned short int) (0x0000FFFFL >> (0x0010 - flag_1));
          if (flag_0 > 0x01)
          {
             result = DCL_ERROR_2;
          }
          else
          {
             if (flag_0)
             {
                __explode_7(work_buff + 0x2FA2, table + 0x00D0, 0x0100);
                __explode_6(work_buff, table + 0x01D0);
             }
             __explode_7(work_buff + 0x30E2, table + 0x00B0, 0x0010);
             __explode_5(0x0010, work_buff + 0x30E2, table + 0x00C0, work_buff + 0x2B22);
             __explode_7(work_buff + 0x30F2, table + 0x0080, 0x0010);
             __explode_7(work_buff + 0x3102, table + 0x0090, 0x0020);
             __explode_7(work_buff + 0x30A2, table, 0x0040);
             __explode_5(0x0040, work_buff + 0x30A2, table + 0x0040, work_buff + 0x2A22);
             if (__explode_1(work_buff) != 0x0306)
             {
                result = DCL_NO_ERROR;
             }
             else
             {
                result = DCL_ERROR_4;
             }
          }
      }
   }
   return result;
}

unsigned short int __explode_1(unsigned char *buf)
{
   unsigned long         result, temp;
   unsigned char         *s, *d;
   write_data_proc   *write_data;
   void         *param;
   *((unsigned short int *) (buf + 0x04)) = 0x1000;
   while (result = __explode_2(buf), (unsigned short int) result < 0x0305)
   {
      if ((unsigned short int) result < 0x0100)
      {
         temp = *((unsigned short int *) (buf + 0x04));
         *((unsigned short int *) (buf + 0x04)) = (unsigned short int) (temp + 0x01);
         *(buf + temp + 0x1E) =(unsigned char) result;
      }
      else
      {
         result -= 0x00FE;
         s = (unsigned char *) __explode_3(buf, (unsigned short int)result);
         if (!s)
         {
            result = 0x0306;
            break;
         }
         else
         {
            temp = *((unsigned short int *) (buf + 0x04));
            d = temp + 0x1E + buf;
            *((unsigned short int *) (buf + 0x04)) =(unsigned short int) (temp + result);
            s = (unsigned char *)((unsigned long)d - (unsigned long)s);
            do
            {
               result--;
               *(d++) = *(s++);
            } while (result); 
         }         
      }
      if (*((unsigned short int *) (buf + 4)) >= 0x2000)
      {
         result = 0x1000;
         write_data = (write_data_proc *) *((unsigned long *) (buf + 0x1A));
         param = (void *) *((unsigned long *) (buf + 0x12));
         write_data(buf + 0x101E, 0x1000, param);
         __explode_7(buf + 0x001E, buf + 0x101E, *((unsigned short int *) (buf + 0x04)) - 0x1000);
         *((unsigned short int *) (buf + 0x04)) -= 0x1000;
      }
   }
   write_data = (write_data_proc *) *((unsigned long *) (buf + 0x1A));
   param = (void *) *((unsigned long *) (buf + 0x12));
   write_data(buf + 0x101E, (unsigned short int)(*((unsigned short int *) (buf + 0x04)) - 0x1000), param);   
   return (unsigned short int) result;
}

unsigned short int __explode_2(unsigned char *buf)
{
   unsigned long   result, flag, flag_1;
   if (*((unsigned short int *) (buf + 0x0A)) & 0x01)
   {
      if (__explode_4(buf, 0x01)) return 0x0306;
      result = *(buf + ((unsigned char) *((unsigned short int *) (buf + 0x0A))) + 0x2B22);
      if (__explode_4(buf, *(buf + ((unsigned short int) result) + 0x30E2))) return 0x0306;
      flag = *(buf + ((unsigned short int) result) + 0x30F2);
      if (flag)
      {
         flag_1 = (*((unsigned short int *) (buf + 0x0A))) & ((0x01 << flag) - 0x01);
         if (__explode_4(buf, flag))
         {
            if ((((unsigned short int) result) + ((unsigned short int) flag_1)) != 0x010E) return 0x0306;
         }
         result = *((unsigned short int *) (buf + (((unsigned short int) result) << 0x01) + 0x3102)) + flag_1;
      }
      result += 0x0100;
   }
   else
   {
      if (__explode_4(buf, 0x01)) return 0x0306;
      if (!*((unsigned short int *) (buf + 0x02)))
      {
         result = (unsigned char) *((unsigned short int *) (buf + 0x0A));
         if (__explode_4(buf, 0x08)) return 0x0306;
      }
      else
      {
         flag = *((unsigned short int *) (buf + 0x0A));
         if ((unsigned char) flag)
         {
            result = *(buf + ((unsigned char) flag) + 0x2C22);
            if (((unsigned short int) result) == 0x00FF)
            {
               if (flag & 0x003F)
               {
                  if (__explode_4(buf, 0x04)) return 0x0306;
                  result = *(buf + ((unsigned char) *((unsigned short int *) (buf + 0x0A))) + 0x2D22);
               }
               else
               {
                  if (__explode_4(buf, 0x06)) return 0x0306;
                  result = *(buf + ((*((unsigned short int *) (buf + 0x0A))) & 0x007F) + 0x2E22);
               }
            }
         }
         else
         {
            if (__explode_4(buf, 0x08)) return 0x0306;
            result = *(buf + ((unsigned char) *((unsigned short int *) (buf + 0x0A))) + 0x2EA2);
         }
         flag = *(buf + ((unsigned short int) result) + 0x2FA2);
         if (__explode_4(buf, flag)) return 0x0306;
      }
   }
   return (unsigned short int)result;
}

unsigned short int *__explode_3(unsigned char *buf, unsigned short int flag)
{
   unsigned long   result, flag_1;
   result = *(buf + ((unsigned char) *((unsigned short int *) (buf + 0x0A))) + 0x2A22);
   if (__explode_4(buf, *(buf + ((unsigned short int) result) + 0x30A2))) return 0x00;
   if (((unsigned short int) flag) == 0x02)
   {
      result <<= 0x02;
      result |= *((unsigned short int *) (buf + 0x0A)) & 0x03;
      if (__explode_4(buf, 0x02)) return 0x00;
   }
   else
   {
      flag_1 = *((unsigned short int *) (buf + 0x06));
      result <<= flag_1;
      result |= *((unsigned short int *) (buf + 0x08)) & *((unsigned short int *) (buf + 0x0A));
      if (__explode_4(buf, flag_1)) return 0x00;
   }
   return (unsigned short int *) (result + 0x01);
}

unsigned short int __explode_4(unsigned char *buf, unsigned long flag)
{
   unsigned long         result;
   unsigned short int         read_result;
   read_data_proc   *read_data = (read_data_proc *) *((unsigned long *) (buf + 0x16));
   void         *param = (void *) *((unsigned long *) (buf + 0x12));
   result = *((unsigned short int *) (buf + 0x0C));
   if ((unsigned short int) flag <= (unsigned short int) result) {
      *((unsigned short int *) (buf + 0x0A)) >>= flag;
      *((unsigned short int *) (buf + 0x0C)) -= (unsigned short int)flag;
      result = 0x00;
   }
   else {
      *((unsigned short int *) (buf + 0x0A)) >>= result;
      result = *((unsigned short int *) (buf + 0x0E));
      if ((unsigned short int) result == *((unsigned short int *) (buf + 0x10))) {
         *((unsigned short int *) (buf + 0x0E)) = 0x0800;
         read_result = read_data(buf + 0x2222, 0x0800, param);
         *((unsigned short int *) (buf + 0x10)) = read_result;
         if (!read_result) return 0x01;
         *((unsigned short int *) (buf + 0x0E)) = 0x00;
      }
      result = *((unsigned short int *) (buf + 0x0E)) + 0x2222;
      *((unsigned short int *) (buf + 0x0A)) |= *(buf + result) << 0x08;
      *((unsigned short int *) (buf + 0x0E)) += 0x01;
      *((unsigned short int *) (buf + 0x0A)) >>= flag - *((unsigned short int *) (buf + 0x0C));
      *((unsigned short int *) (buf + 0x0C)) = (unsigned short int)(0x08 - (flag - *((unsigned short int *) (buf + 0x0C))));
      result = 0x00;
   }
   return (unsigned short int) result;
}

void __explode_5(unsigned short int count, unsigned char *buf_1, const unsigned char *table, unsigned char *buf_2)
{
   short int   i = (short int)(count - 1);
   unsigned long   idx_1, idx_2;
   
   for (; i >= 0; i--)
   {
      idx_1 = *(table + i);
      idx_2 = 0x01 << *(buf_1 + i);
      for (;;)
      {
         *(buf_2 + (unsigned short int) idx_1) = (unsigned char) i;
         idx_1 += idx_2;
         if ((unsigned short int) idx_1 >= 0x0100) break;
      }
   } 
}

void __explode_6(unsigned char *buf, const unsigned char *table)
{
   short int   i;
   unsigned long   idx_1, idx_2;
   
   for (i = 0x00FF; i >= 0; i--)
   {      
      idx_1 = *(buf + i + 0x2FA2);
      if (idx_1 <= 0x08 )
      {
         idx_2 = *((unsigned short int *) (table + (i << 0x01)));
         idx_1 = 0x01 << idx_1;
         do
         {
            *(buf + idx_2 + 0x2C22) =(unsigned char) i;
            idx_2 += idx_1;
         } while ((unsigned short int) idx_2 < 0x0100);
      }
      else
      {
         idx_2 = *((unsigned short int *) (table + (i << 0x01)));
         if ((unsigned char) idx_2)
         {
            
            *(buf + (unsigned char) idx_2 + 0x2C22) = 0xFF;
            if (*((unsigned short int *) (table + (i << 0x01))) & 0x003F)
            {
               *(buf + i + 0x2FA2) -= 0x04;
               idx_1 = 0x01 << *(buf + i + 0x2FA2);
               idx_2 = *((unsigned short int *) (table + (i << 0x01))) >> 0x04;
               do
               {
                  *(buf + idx_2 + 0x2D22) =(unsigned char) i;
                  idx_2 += idx_1;
               } while ((unsigned short int) idx_2 < 0x0100);
            }
            else
            {
               *(buf + i + 0x2FA2) -= 0x06;
               idx_1 = 0x01 << *(buf + i + 0x2FA2);
               idx_2 = *((unsigned short int *) (table + (i << 0x01))) >> 0x06;
               do
               {
                  *(buf + idx_2 + 0x2E22) = (unsigned char)i;
                  idx_2 += idx_1;
               } while ((unsigned short int) idx_2 < 0x0080);
            }
         }
         else
         {
            *(buf + i + 0x2FA2) -= 0x08;
            idx_1 = 0x01 << *(buf + i + 0x2FA2);
            idx_2 = *((unsigned short int *) (table + (i << 0x01))) >> 0x08;
            do
            {
               *(buf + idx_2 + 0x2EA2) = (unsigned char)i;
               idx_2 += idx_1;
            } while ((unsigned short int) idx_2 < 0x0100);
         }
      }
   }
}

void __explode_7(unsigned char *buf, const unsigned char *table, unsigned long count)
{
   unsigned long   half_count = count >> 0x02;
   unsigned char   *buf_end;
   
   if (half_count)
   {
      buf_end = buf + (half_count << 0x02);
      do
      {
         *((unsigned long *) buf) = *((unsigned long *) table);
         buf += 4;
         table += 4;
      } while (buf < buf_end);
   }
   switch (count - (half_count << 0x02))
   {
      case 3:
         *(buf++) = *(table++);
         
      case 2:
         *(buf++) = *(table++);
         
      case 1:
         *buf = *table;
         
      default:
         break;
   }
}

