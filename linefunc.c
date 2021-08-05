#include "basic.h"

int isregister(char []);

/************************************
 skip white spaces and place ptr next
*************************************/
void skip_white_spaces(char **ptr){
	while (**ptr == ' ' || **ptr == '\t')
		*ptr += 1;
}

/******************************************
 check whether we get to the end of the row
*******************************************/
int EOR(char ch) {
	return (ch == '\n') || (ch == EOF);
}

/************************************************
 return the size of the operand (not all of them)
*************************************************/
int get_op_size(char op[]) {

	if(!strcmp(op, "r0") || !strcmp(op, "r1") || !strcmp(op, "r2") || !strcmp(op, "r3")
		|| !strcmp(op, "r4") || !strcmp(op, "r5") || !strcmp(op, "r6") || !strcmp(op, "r7"))
		return 0;
	return 1;
}

/*******************************************
 disassembles the sentence and prints errors
********************************************/
int extractor(char line[], cmd *currentcmd) {

	char action[7] = "\0";
	char data[MAX_ROW_SIZE];
	char *ptr = &line[0];
	char *endptr;

	int oprnum = 0;

	if(!strchr(line, '\n') && !strchr(line, EOF))
		return Too_long_row;

	/* handle comments */
	if(*ptr == ';')
		return 0;
	skip_white_spaces(&ptr);
	if(*ptr == ';')
		return Illegal_structure_of_a_comment;
	
	/* handle empty rows */
	if(EOR(*ptr))
		return 0;

	/* handle the label */
	endptr = strchr(line, ':');

	if(endptr) { /* if colon found */
		if(endptr - ptr > MAX_LABEL_SIZE)
			return Too_long_label_name;

		strncpy(currentcmd -> label, ptr, endptr - ptr);
		currentcmd -> label[endptr - ptr] = '\0';

		if(!(isalpha((currentcmd -> label)[0])))
			return Label_cannot_start_with_non_alpha_char;
		while(++ptr != endptr)
			if(!(isalpha(*ptr) || isdigit(*ptr)))
				return Invalid_label_name;

		if(symbol_exist(currentcmd -> label))
			return Label_name_already_exist;

		ptr++;
		skip_white_spaces(&ptr);
	}

	if(EOR(*ptr))
		return Missing_arguments;

	/* handle the action */
	endptr = ptr;

	while(*endptr != '\t' && *endptr != ' ' && !EOR(*endptr)) endptr++;

	if(endptr - ptr > MAX_ACTION_NAME_SIZE + 1 || endptr - ptr < MIN_ACTION_NAME_SIZE)
		return Illegal_actions_name;

	strncpy(action, ptr, endptr - ptr);
	action[endptr - ptr] = '\0';
	strncpy(currentcmd -> action, action[0]=='.'? &action[1]:action, MAX_ACTION_NAME_SIZE + 1);

	if(action[0] != '.') {
		if(strcmp(action, "mov") && strcmp(action, "cmp") && strcmp(action, "add") 
			&& strcmp(action, "sub") && strcmp(action, "lea") && strcmp(action, "clr")
			&& strcmp(action, "not") && strcmp(action, "inc") && strcmp(action, "dec")
			&& strcmp(action, "jmp") && strcmp(action, "bne") && strcmp(action, "jsr")
			&& strcmp(action, "red") && strcmp(action, "prn") && strcmp(action, "rts")
			&& strcmp(action, "stop"))
			return Illegal_name_of_operand;
	} else if(strcmp(action, ".string") && strcmp(action, ".data") 
			&& strcmp(action, ".extern") && strcmp(action, ".entry")) 
			return Illegal_instruction_name;

	/* handle the operends */
	skip_white_spaces(&endptr);
	if(EOR(*endptr)) {
		if(is_the_amount_of_operands_illegal(currentcmd -> action, 0, FALSE))
			return is_the_amount_of_operands_illegal(currentcmd -> action, 0, FALSE);
		return 0;
	}

	while(!EOR(*endptr)) {
		oprnum++;

		skip_white_spaces(&endptr);

		ptr = endptr;

		if(!strcmp(action, ".string")) {
			if(!EOR(*endptr)) {
				endptr++;
				while(*endptr != '"' && !EOR(*endptr)) endptr++;
				if(!EOR(*endptr))
					endptr++;
			}
			
			if(endptr - ptr > MAX_ROW_SIZE) 
				return Too_long_operand_name;
		}
		else {
			while(*endptr != '\t' && *endptr != ' ' && *endptr != ',' && !EOR(*endptr)) endptr++;

			if(endptr - ptr > MAX_LABEL_SIZE + 1)
				return Too_long_operand_name;
		}

		strncpy(data, ptr, endptr - ptr);
		data[endptr - ptr] = '\0';
		add2list(&(currentcmd -> oprv), data, endptr - ptr + 1);

		if(data[0] == '\0')
			return Empty_operand_is_not_allowed;
	
		if(is_the_amount_of_operands_illegal(currentcmd -> action, oprnum, TRUE))
			return is_the_amount_of_operands_illegal(currentcmd -> action, oprnum, TRUE);

		if(op_validation(data, currentcmd -> action, oprnum))
			return op_validation(data, currentcmd -> action, oprnum);

		skip_white_spaces(&endptr);
		if(EOR(*endptr)) {
			if(is_the_amount_of_operands_illegal(currentcmd -> action, oprnum, FALSE))
				return is_the_amount_of_operands_illegal(currentcmd -> action, oprnum, FALSE);
			return 0;
		}

		if(*endptr != ',')
			return Missing_comma;

		endptr++;
	}

	return 0;
}

/*******************************************
 disassembles the sentence and prints errors
********************************************/
int simple_extractor(char line[], cmd *currentcmd) {

	char action[7] = "\0";
	char data[MAX_LABEL_SIZE + 1];
	char *ptr = &line[0];
	char *endptr;

	int temp = 0;

	/* handle comments */
	if(*ptr == ';')
		return 0;
	skip_white_spaces(&ptr);
	
	/* handle empty rows */
	if(EOR(*ptr))
		return 0;

	/* handle the label */
	endptr = strchr(line, ':');

	if(endptr) { /* if colon found */
		strncpy(currentcmd -> label, ptr, endptr - ptr);
		currentcmd -> label[endptr - ptr] = '\0';

		ptr = endptr + 1;
		skip_white_spaces(&ptr);
	}

	/* handle the action */
	endptr = ptr;

	while(*endptr != '\t' && *endptr != ' ' && !EOR(*endptr)) endptr++;

	strncpy(action, ptr, endptr - ptr);
	action[endptr - ptr] = '\0';
	strncpy(currentcmd -> action, action[0]=='.'? &action[1]:action, MAX_ACTION_NAME_SIZE + 1);

	/* handle the operends */
	skip_white_spaces(&endptr);
	if(EOR(*endptr))
		return 0;
	while(!EOR(*endptr)) {
		skip_white_spaces(&endptr);

		ptr = endptr;

		if(!strcmp(action, ".string")) {
			if(!EOR(*endptr)) {
				endptr++;
				while(*endptr != '"' && !EOR(*endptr)) endptr++;
				if(!EOR(*endptr))
					endptr++;
			}
		}
		else
			while(*endptr != '\t' && *endptr != ' ' && *endptr != ',' && !EOR(*endptr)) endptr++;

		strncpy(data, ptr, endptr - ptr);
		data[endptr - ptr] = '\0';
		add2list(&(currentcmd -> oprv), data, endptr - ptr + 1);
	

		if(data[0] == '&') {
			if(!(temp = symbol_exist(strcpy(data, data+1))))
				return Symbol_not_exist;
			else if(temp == external)
				return Symbol_cannot_be_from_external_source;
		}

		if(isalpha(data[0]) && !isregister(data)){
			if(!symbol_exist(data))
				return Symbol_not_exist;}

		skip_white_spaces(&endptr);
		if(EOR(*endptr))
			return 0;

		endptr++;
	}

	return 0;
}
