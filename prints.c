#include "advanced.h"

/************************
 prints the correct error
 ************************/
int print_error(int error){
	switch(error){
		case Missing_arguments:
			printf("Missing arguments\n");
			return 1;
		case Invalid_label_name:
			printf("Invalid label name\n");
			return 1;
		case Illegal_structure_of_a_sentence:
			printf("Illegal structure of a sentence\n");
			return 1;
		case Illegal_name_of_operand:
			printf("Illegal name of operand\n");
			return 1;
		case Label_cannot_start_with_non_alpha_char:
			printf("Label cannot start with non alpha char\n");
			return 1;
		case Illegal_instruction_name:
			printf("Illegal instruction name\n");
			return 1;
		case Too_few_operands:
			printf("Too few operands\n");
			return 1;
		case Too_much_operands:
			printf("Too much operands\n");
			return 1;
		case Illegal_structure_of_a_comment:
			printf("Illegal structure of a comment\n");
			return 1;
		case Illegal_actions_name:
			printf("Illegal action's name\n");
			return 1;
		case Label_name_already_exist:
			printf("Label name already exist\n");
			return 1;
		case Illegal_number:
			printf("Illegal number\n");
			return 1;
		case Label_name_cannot_be_a_reserved_word:
			printf("Label name cannot be a reserved word\n");
			return 1;
		case Wrong_addressing_method:
			printf("Wrong addressing method\n");
			return 1;
		case Symbol_not_exist:
			printf("Symbol not exist\n");
			return 1;
		case Symbol_cannot_be_from_external_source:
			printf("Symbol cannot be from external source\n");
			return 1;
		case Empty_operand_is_not_allowed:
			printf("Empty operand is not allowed\n");
			return 1;
		case Missing_comma:
			printf("Missing comma\n");
			return 1;
		case Too_long_label_name:
			printf("Too long label name\n");
			return 1;
		case Too_long_operand_name:
			printf("Too long operand name\n");
			return 1;
		case Too_long_row:
			printf("Too long row\n");
			return 1;
		case String_cannot_contains_unprintable_chars:
			printf("String cannot contains unprintable chars\n");
			return 1;
	}
	return 0;
}

/************************************************
 prints the disassembled cmd for testing purposes
*************************************************/
void print_test(cmd currentcmd) {
	node *nodeptr;

	printf("label: %s\n", currentcmd.label);
	printf("action: %s\n", currentcmd.action);
	nodeptr = currentcmd.oprv;
	while (nodeptr) {
		printf("nodeptr -> data: %s\n", nodeptr->data);
		nodeptr = nodeptr->next;
	}
}
