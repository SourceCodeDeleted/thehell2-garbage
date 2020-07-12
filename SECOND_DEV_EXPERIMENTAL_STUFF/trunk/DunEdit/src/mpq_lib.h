/*
Library for access to MPQ files
Original code if from Tom Amigo
Some additions by Paul Siramy, then by me
*/
#ifndef _MPQ_LIB_H_
#define _MPQ_LIB_H_

#define MAX_MPQ_FILE           4
#define MAX_MOD_DIR            1
#define PATH_MAXI              256

typedef struct {
   int   is_open;			/* false / true */
   unsigned long offset_mpq;		/* Offset to MPQ file data */
   unsigned long offset_htbl;		/* Offset to hash_table of MPQ */
   unsigned long offset_btbl;		/* Offset to block_table of MPQ */
   unsigned long lenght_mpq_part;	/* Lenght of MPQ file data */
   unsigned long lenght_htbl;		/* Lenght of hash table */
   unsigned long lenght_btbl;		/* Lenght of block table */
   unsigned long *hash_table;		/* Hash table */
   unsigned long *block_table;		/* Block table */
   unsigned long count_files;		/* Number of files in MPQ (calculated from size of block_table) */
   unsigned long massive_base[0x500];	/* This massive is used to calculate crc and decode files */
   char	*filename_table;	/* Array of MPQ filenames */
   char	*identify_table;	/* Bitmap table of MPQ filenames 1 - if file name for this entry is known, 0 - if is not */
   char	file_name[257];		/* Name of archive */
   char	work_dir[PATH_MAXI];	/* Work directory */
   char	prnbuf[PATH_MAXI+100];	/* Buffer */
   char	default_list[PATH_MAXI];	/* Path to list file */
   FILE	*fpMpq;
   /* This is used to decompress DCL-compressed and WAVE files */
   unsigned long avail_metods[4];
   unsigned long lenght_write;
   unsigned char * global_buffer, * read_buffer_start, * write_buffer_start, * explode_buffer;
   unsigned long * file_header;
} GLB_MPQ_S;

#ifndef _MPQ_LIB_C_
extern char mpq_file[MAX_MPQ_FILE][PATH_MAXI];
extern char mod_dir[MAX_MOD_DIR][PATH_MAXI];
extern GLB_MPQ_S glb_mpq_struct[MAX_MPQ_FILE];
extern GLB_MPQ_S * glb_mpq;
#endif /* _MPQ_LIB_C_ */

#ifndef false
#define false 0
#endif
#ifndef true
#define true 1
#endif

int misc_load_mpq_file(const char *filename, unsigned char **buffer, long *buf_len, int output);		/* load a file from mpq. First try in the mod directory, if not found in the 1st mpq it can */
unsigned char *txt_read_in_mem(const char *txtname);								/* load a .txt from a mpq (or mod dir) into mem */
void open_all_mpq(int output);

void mpq_lib_start(int output);
void mpq_lib_stop(int output);
int mod_load_in_mem(const char * moddir, const char * filename, unsigned char ** buffer, long * buf_len);	/* read from mod directory, NOT from a mpq */
/*int mpq_load_in_mem(const char * mpqname, const char * filename, unsigned char ** buffer, long * buf_len, int output);*/
void mpq_batch_open(const char * mpqname);
int mpq_batch_load_in_mem(const char * filename, unsigned char ** buffer, long * buf_len);
void mpq_batch_close();

#ifdef _MPQ_LIB_C_

#define CMP_BUFFER_SIZE 36312L	/* Work buffer size for imploding */
#define EXP_BUFFER_SIZE 12596L	/* Work buffer size for exploding */
#define CMP_BINARY          0
#define CMP_ASCII           1
#define DICT_SIZE_1      1024
#define DICT_SIZE_2      2048
#define DICT_SIZE_4      4096

#define DCL_NO_ERROR 0L
#define DCL_ERROR_1  1L
#define DCL_ERROR_2  2L
#define DCL_ERROR_3  3L
#define DCL_ERROR_4  4L

int test_prepare_archive();
long int test_tell_entry(const char * filename);

void BuildBaseMassive();
int InitializeLocals();
void FreeLocals();
unsigned long Crc(const char * string, unsigned long * massive_base, unsigned long massive_base_offset);
void Decode(unsigned long * data_in, unsigned long * massive_base, unsigned long crc, unsigned long lenght);
unsigned short int read_data(unsigned char * buffer, unsigned short int size, void  * crap);
void write_data(unsigned char * buffer, unsigned short int size, void  * crap);
int ExtractTo(FILE * fp_new, unsigned long entry);
int ExtractToMem(unsigned char * mp_new, unsigned long entry);

unsigned long ExtWavUnp1(unsigned long,unsigned long,unsigned long,unsigned long);
unsigned long ExtWavUnp2(unsigned long,unsigned long,unsigned long,unsigned long);
unsigned long ExtWavUnp3(unsigned long,unsigned long,unsigned long,unsigned long);
void Sub_WavUnp1(unsigned long,unsigned long);
unsigned long Sub_WavUnp2(unsigned long,unsigned long,unsigned long);
void Sub_WavUnp3(unsigned long);
void Sub_WavUnp4(unsigned long,unsigned long,unsigned long,unsigned long);
unsigned long Sub_WavUnp5(unsigned long);
void Sub_WavUnp6(unsigned long);
unsigned long Sub_WavUnp7(unsigned long);
unsigned long Sub_WavUnp8(unsigned long);
void Sub_WavUnp9(unsigned long,unsigned long);
void Sub_WavUnp10(unsigned long,unsigned long);
unsigned long Sub_WavUnp11(unsigned long,unsigned long,unsigned long,unsigned long);
void Sub_WavUnp12(unsigned long);
unsigned long Sub_WavUnp13(unsigned long,unsigned long,unsigned long,unsigned long,unsigned long);

typedef struct {
	unsigned char *buf_in;
	unsigned char *buf_out;
} params;

typedef unsigned short int read_data_proc(unsigned char *buffer, unsigned short int size, void *param);
typedef void write_data_proc(unsigned char *buffer, unsigned short int size, void *param);
extern const unsigned char dcl_table[];
extern unsigned long implode(read_data_proc read_data, write_data_proc write_data, unsigned char *work_buff, void *param, unsigned short int type, unsigned short int size);
extern  unsigned long explode(read_data_proc read_data, write_data_proc write_data, void *param);
extern  unsigned long crc32(unsigned char *buffer, unsigned long size, unsigned long old_crc);

static unsigned short int __explode_1(unsigned char *buf);
static unsigned short int __explode_2(unsigned char *buf);
static unsigned short int *__explode_3(unsigned char *buf, unsigned short int result);
static unsigned short int __explode_4(unsigned char *buf, unsigned long flag);
static void __explode_5(unsigned short int count, unsigned char *buf_1, const unsigned char *table, unsigned char *buf_2);
static void __explode_6(unsigned char *buf, const unsigned char *table);
void __explode_7(unsigned char *buf, const unsigned char *table, unsigned long count);

#endif /* _MPQ_LIB_C_ */
#endif /* _MPQ_LIB_H_ */
