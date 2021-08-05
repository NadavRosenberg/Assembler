#include "basic.h"

/* functions from firstPass.c */
int firstPass(char [], FILE *, int *, int *);

/* functions from secondPass.c */
int secondPass(char [], FILE *, FILE *, FILE *, FILE *, int, int);

/* functions from assembler.c */
int print_error(int);

/* functions from linefunc.c */
int extractor(char [], cmd *);
int simple_extractor(char [], cmd *);
int get_op_size(char []);

/* functions from prints.c */
int print_error(int);
void print_test(cmd);

/* functions from files.c */
int access_files(char [], FILE **, FILE **, FILE **, FILE **);
int delete_all_files(char []);
int delete_file(char []);
int delete_unused_files_and_close_the_rest(char [], FILE **, FILE **, FILE **);
int close_files(FILE **, FILE **, FILE **);
void add2objfile(FILE **, int, unsigned int, int);
void initobjfile(FILE **, int, int);
