#include "basic.h"

oprecord optable[16] = {
	{"mov\0", 0, 0},
	{"cmp\0", 0, 1},
	{"add\0", 1, 2},
	{"sub\0", 2, 2},
	{"lea\0", 0, 4},
	{"clr\0", 1, 5},
	{"not\0", 2, 5},
	{"inc\0", 3, 5},
	{"dec\0", 4, 5},
	{"jmp\0", 1, 9},
	{"bne\0", 2, 9},
	{"jsr\0", 3, 9},
	{"red\0", 0, 12},
	{"prn\0", 0, 13},
	{"rts\0", 0, 14},
	{"stop\0", 0, 15}
};

int get_opcode(char action[]) {
	int i;

	for(i=0; i < sizeof(optable)/sizeof(optable[0]); i++)
		if(!strcmp(optable[i].opname, action))
			return optable[i].opcode;
	return 0;
}

int get_funct(char action[]) {
	int i;

	for(i=0; i < sizeof(optable)/sizeof(optable[0]); i++)
		if(!strcmp(optable[i].opname, action))
			return optable[i].funct;
	return 0;
}

int get_method(char opr[]) {
	if(opr[0] == '#')
		return 0;
	if(opr[0] == '&')
		return 2;
	if(!strcmp(opr, "r0") || !strcmp(opr, "r1") || !strcmp(opr, "r2")
		|| !strcmp(opr, "r3") || !strcmp(opr, "r4") || !strcmp(opr, "r5")
		|| !strcmp(opr, "r6") || !strcmp(opr, "r7"))
		return 3;
	return 1;
}

int get_register(char opr[]) {
	if(!strcmp(opr, "r1"))
		return 1;
	if(!strcmp(opr, "r2"))
		return 2;
	if(!strcmp(opr, "r3"))
		return 3;
	if(!strcmp(opr, "r4"))
		return 4;
	if(!strcmp(opr, "r5"))
		return 5;
	if(!strcmp(opr, "r6"))
		return 6;
	if(!strcmp(opr, "r7"))
		return 7;
	return 0;
}

/*********************************
 functions to control linked lists
**********************************/

void add2list(node **head, char data[], int size) {
	node *current, *prev;
	node *temp;

	temp = (node *) malloc(sizeof(node));
	if(!temp)
		exit(0);

	strncpy(temp -> data, data, size);
	temp -> next = NULL;

	current = *head;
	while (current) {
		prev = current;
		current = current -> next;
	}

	if(current == *head)
		*head = temp;
	else 
		prev -> next = temp;
}

void printlist(node *ptr) {

	while(ptr) {
		printf("%s\n", ptr -> data);
		ptr = ptr -> next;
	}
}

int freelist(node **head) {
	node *temp;

	while (*head) {
		temp = *head;
		*head = (*head) -> next;
		free(temp);
	}

	return TRUE;
}

/********************************************
 functions to manipulate the table of symbols
*********************************************/
symbolrecord *symboltable;

int addsymbol2list(char symbol[],int value, characterization crt, boolean entry) {
	symbolrecord *current, *prev;
	symbolrecord *temp;

	temp = (symbolrecord *) malloc(sizeof(symbolrecord));
	if(!temp)
		exit(0);

	strcpy(temp -> symbol, symbol);
	temp -> value = value;
	temp -> crt = crt;
	temp -> entry = entry;
	temp -> next = NULL;

	current = symboltable;
	while (current) {
		prev = current;
		current = current -> next;
	}

	if(current == symboltable)
		symboltable = temp;
	else 
		prev -> next = temp;

	return 0;
}

int symbol_exist(char symbol[]) {
	symbolrecord *ptr = symboltable;

	while(ptr != NULL) {
		if(!strcmp(ptr -> symbol, symbol))
			return ptr -> crt;
		ptr = ptr -> next;
	}
	return FALSE;
}

int get_address(char symbol[]) {
	symbolrecord *ptr = symboltable;

	while(ptr != NULL) {
		if(!strcmp(ptr -> symbol, symbol))
			return ptr -> value;
		ptr = ptr -> next;
	}
	return FALSE;
}

void addIC2data(int IC) {
	symbolrecord *ptr = symboltable;

	while(ptr) {
		if(ptr -> crt == data)
			ptr -> value += IC;
		ptr = ptr -> next;
	}
}

int get_distance(char symbol[], int from) {
	symbolrecord *ptr = symboltable;

	while(ptr != NULL) {
		if(!strcmp(ptr -> symbol, symbol)) 
			return (ptr -> value) - from + 1;
		ptr = ptr -> next;
	}
	return 0;
}

int isentry(char symbol[]) {
	symbolrecord *ptr = symboltable;

	while(ptr) {
		if(!strcmp(ptr -> symbol, symbol)) 
			return ptr -> entry;
		ptr = ptr -> next;
	}
	return FALSE;	
}

void print_symbol_table() {
	symbolrecord *ptr = symboltable;

	while(ptr) {
		printf("%s, %d, %d, %d\n", ptr -> symbol, ptr -> value, ptr-> crt, ptr-> entry);
		ptr = ptr -> next;
	}
}

void free_symbols_table() {
	symbolrecord *temp;

	while (symboltable) {
		temp = symboltable;
		symboltable = symboltable -> next;
		free(temp);
	}
}

/*********************************
 functions to control entries list
**********************************/
entrynode *entries;

void addentry2list(char label[], int row) {
	entrynode *current, *prev;
	entrynode *temp;

	temp = (entrynode *) malloc(sizeof(entrynode));
	if(!temp)
		exit(0);

	strcpy(temp -> label, label);
	temp -> row = row;
	temp -> next = NULL;

	current = entries;
	while (current) {
		prev = current;
		current = current -> next;
	}

	if(current == entries)
		entries = temp;
	else 
		prev -> next = temp;
}

int addentries2symbols() {
	entrynode *entryptr = entries;
	symbolrecord *symbolptr;
	boolean exist;

	while(entryptr) {
		exist = FALSE;
		symbolptr = symboltable;

		while(symbolptr) {
			if(!strcmp(symbolptr -> symbol, entryptr -> label)) {
				symbolptr -> entry = TRUE;
				exist = TRUE;
				break;
			}
			symbolptr = symbolptr -> next;
		}

		if(!exist)
			return entryptr -> row;
		entryptr = entryptr -> next;
	}
	return 0;
}

void printentries() {
	entrynode *ptr = entries;

	while(ptr) {
		printf("label: %s, row: %d\n", ptr -> label, ptr -> row);
		ptr = ptr -> next;
	}
}

void free_entries() {
	entrynode *temp;

	while (entries) {
		temp = entries;
		entries = entries -> next;
		free(temp);
	}
}
