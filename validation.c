#include "basic.h"

/*************************************
 check whether the aperand is register
**************************************/
int isregister(char op[]) {
	if(!strcmp(op, "r0") || !strcmp(op, "r1") || !strcmp(op, "r2") 
		|| !strcmp(op, "r3") || !strcmp(op, "r4") || !strcmp(op, "r5") 
		|| !strcmp(op, "r6") || !strcmp(op, "r7"))
		return TRUE;
	return FALSE;
}

/***********************************************
 check whether the amount of operands is illegal
************************************************/
int is_the_amount_of_operands_illegal(char action[], int num, boolean possiblemore) {
	if(!strcmp(action, "mov") || !strcmp(action, "cmp") || !strcmp(action, "add") 
			|| !strcmp(action, "sub") || !strcmp(action, "lea")) {
		if(num < 2 && !possiblemore)
			return Too_few_operands;
		if(num > 2)
			return Too_much_operands;
	}

	if(!strcmp(action, "clr") || !strcmp(action, "not") || !strcmp(action, "inc")
			|| !strcmp(action, "dec") || !strcmp(action, "jmp")
			|| !strcmp(action, "bne") || !strcmp(action, "jsr") 
			|| !strcmp(action, "red") || !strcmp(action, "prn")
			|| !strcmp(action, "string") || !strcmp(action, "entry")
			|| !strcmp(action, "extern")) {
		if(num < 1 && !possiblemore)
			return Too_few_operands;
		if(num > 1)
			return Too_much_operands;
		
	}

	if(!strcmp(action, "data")) {
		if(num < 1 && !possiblemore)
			return Too_few_operands;
	}

	if(!strcmp(action, "rts") || !strcmp(action, "stop")) {
		if(num > 0)
			return Too_much_operands;;
	}
	
	return 0;
}

/*****************************************
 check whether the label's name is illegal
******************************************/
int legal_label_name(char name[]) {
	int index = 0;

	if(!isalpha(name[index])) 
		return Label_cannot_start_with_non_alpha_char;

	index++;
	while(name[index] != '\0'){
		if(!(isalpha(name[index]) || isdigit(name[index])))
			return Invalid_label_name;
		index++;
	}
	
	if(!strcmp(name, "r0") || !strcmp(name, "r1") || !strcmp(name, "r2") || !strcmp(name, "r3") 
		|| !strcmp(name, "r4") || !strcmp(name, "r5") || !strcmp(name, "r6") || !strcmp(name, "r7") 
		|| !strcmp(name, "mov") || !strcmp(name, "cmp") || !strcmp(name, "add") 
		|| !strcmp(name, "sub") || !strcmp(name, "lea") || !strcmp(name, "clr") 
		|| !strcmp(name, "not") || !strcmp(name, "inc") || !strcmp(name, "dec") 
		|| !strcmp(name, "jmp") || !strcmp(name, "bne") || !strcmp(name, "jsr") 
		|| !strcmp(name, "red") || !strcmp(name, "prn") || !strcmp(name, "rts") 
		|| !strcmp(name, "stop") || !strcmp(name, ".data") || !strcmp(name, ".string") 
		|| !strcmp(name, ".entry") || !strcmp(name, ".extern"))
		return Label_name_cannot_be_a_reserved_word;

	return 0;
}

/****************************
 check validation of operands
*****************************/
int op_validation(char op[], char action[], int num) {
	int index = 0;

	if(!strcmp(action, "string")) {
		if(op[0] != '"')
			return Illegal_name_of_operand;
		index++;
		while(op[index] != '\0') {
			if(!isprint(op[index]))
				return String_cannot_contains_unprintable_chars;
			index++;
		}
		if(op[--index] != '"')
			return Illegal_name_of_operand;
		return 0;
	}
	
	if(!strcmp(action, "data")) {
		if(op[0] == '-' || op[0] == '+')
			index++;
		while(op[index] != '\0') {
			if(!isdigit(op[index]))
				return Illegal_name_of_operand;
			index++;
		}
		return 0;
	}

	if(!strcmp(action, "entry") || !strcmp(action, "extern")) {
		return legal_label_name(op);
	}

	if(((!strcmp(action, "mov") || !strcmp(action, "add") || !strcmp(action, "sub")) 
		&& num == 1) || !strcmp(action, "cmp") || !strcmp(action, "prn")) {
		if(op[0] == '#') {
			index = 1;
			if(op[1] == '-' || op[1] == '+')
				index = 2;

			while(op[index] != '\0')
				if(!isdigit(op[index++]))
					return Illegal_number;
			return 0;
		}
	}

	if(!strcmp(action, "mov") || !strcmp(action, "cmp") || !strcmp(action, "add") 
		|| !strcmp(action, "sub") || !strcmp(action, "clr") || !strcmp(action, "not")
		|| !strcmp(action, "inc") || !strcmp(action, "dec") || !strcmp(action, "red")
		|| !strcmp(action, "prn") || (!strcmp(action, "lea") && num == 2))
		if(!strcmp(op, "r0") || !strcmp(op, "r1") || !strcmp(op, "r2") 
			|| !strcmp(op, "r3") || !strcmp(op, "r4") || !strcmp(op, "r5") 
			|| !strcmp(op, "r6") || !strcmp(op, "r7"))
			return 0;

	if(!strcmp(action, "mov") || !strcmp(action, "cmp") || !strcmp(action, "add") 
		|| !strcmp(action, "sub") || !strcmp(action, "lea") || !strcmp(action, "clr")
		|| !strcmp(action, "not") || !strcmp(action, "inc") || !strcmp(action, "dec")
		|| !strcmp(action, "jmp") || !strcmp(action, "bne") || !strcmp(action, "jsr")
		|| !strcmp(action, "red") || !strcmp(action, "prn"))
		if(isalpha(op[0]))
			return legal_label_name(op);

	if(!strcmp(action, "jmp") || !strcmp(action, "bne") || !strcmp(action, "jsr"))
		if(op[0] == '&')
			return legal_label_name(strcpy(op, op+1));

	return Wrong_addressing_method;
}
