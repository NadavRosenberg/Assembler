#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define MAX_ROW_SIZE 80
#define MAX_LABEL_SIZE 32
#define MAX_ACTION_NAME_SIZE 7
#define MIN_ACTION_NAME_SIZE 3
#define ADDRESS_FOR_LOADING 100

/* Universal errors for functions in prints.c */
enum {
	Invalid_label_name = 2,
	Missing_arguments,
	Illegal_structure_of_a_sentence,
	Label_cannot_start_with_non_alpha_char,
	Illegal_name_of_operand,
	Illegal_instruction_name,
	Too_few_operands,
	Too_much_operands,
	Illegal_structure_of_a_comment,
	Illegal_actions_name,
	Label_name_already_exist,
	Illegal_number,
	Label_name_cannot_be_a_reserved_word,
	Wrong_addressing_method,
	Symbol_not_exist,
	Symbol_cannot_be_from_external_source,
	Empty_operand_is_not_allowed,
	Missing_comma,
	Too_long_label_name,
	Too_long_operand_name,
	Too_long_row,
	String_cannot_contains_unprintable_chars
};
typedef enum { code = 1, data , external } characterization;
typedef enum { FALSE = 0, TRUE } boolean;

typedef struct record{
	char symbol[MAX_LABEL_SIZE];
	int value;
	characterization crt;
	boolean entry;
	struct record *next;
} symbolrecord;

typedef struct item {
	char data[MAX_LABEL_SIZE + 1]; /* +1 for '&' sign */
	struct item *next;
} node;

typedef struct entry {
	char label[MAX_LABEL_SIZE + 1];
	int row;
	struct entry *next;
} entrynode;

typedef struct oprecord *oprptr;
typedef struct {
	char opname[5];
	int funct;
	int opcode;
} oprecord;

typedef struct {
	char label[MAX_LABEL_SIZE];
	char action[MAX_ACTION_NAME_SIZE];
	node *oprv;	
} cmd;

/* functions from utilities.c */
int get_opcode(char []);
int get_funct(char []);
int get_method(char []);
int get_register(char []);
int get_address(char []);

void add2list(node **, char [], int);
void printlist(node *);
int freelist(node **);

int addsymbol2list(char [],int, characterization, boolean);
int symbol_exist(char []);
void addIC2data(int);
int get_distance(char [], int);
void print_symbol_table();
int isentry(char []);
void free_symbols_table();

void addentry2list(char [], int);
int addentries2symbols();
void printentries();
void free_entries();

/* functions from validation.c */
int isregister(char []);
int is_the_amount_of_operands_illegal(char [], int, boolean);
int legal_label_name(char []);
int op_validation(char op[], char [], int);
