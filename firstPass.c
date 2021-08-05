#include "advanced.h"

/**********************************************************************************************************
 Goes through all the rows in a given file and checks for syntax error (according to the assembler's logic)
 **********************************************************************************************************/
int firstPass(char filename[], FILE *srcFile, int *ICF, int *DCF) {

	int IC = ADDRESS_FOR_LOADING; /* count the number of command lines */
	int DC = 0; /* count the number of data lines */
	int error;
	int linenum = 0;

	char line[MAX_ROW_SIZE];
	char ch;

	node *nodeptr;
	boolean valid = TRUE;
	const cmd emptycmd = {"\0", "\0", NULL};
	cmd currentcmd = emptycmd;

	while(fgets(line, MAX_ROW_SIZE-1, srcFile)) {

		/*printf("-----------------------------\n");
		printf("line: %s\n",line);*/

		linenum++;
		freelist(&(currentcmd.oprv));
		currentcmd = emptycmd;

		if((error = extractor(line, &currentcmd))) { /* if there is any error */ 
			valid = FALSE;
			printf("%s.as:%d: ", filename, linenum);
			print_error(error);
			if(error == Too_long_row)
				while((ch = fgetc(srcFile)) != '\n' && ch != EOF);
			continue;
		}

		if(!strcmp(currentcmd.action, "\0"))
			continue;

		if(!strcmp(currentcmd.action, "string")) {
			if(currentcmd.label[0])
				addsymbol2list(currentcmd.label, DC, data, FALSE);
			DC += strlen(currentcmd.oprv -> data) - 1;
			continue;
		}
		
		if(!strcmp(currentcmd.action, "data")) {
			if(currentcmd.label[0])
				addsymbol2list(currentcmd.label, DC, data, FALSE);

			nodeptr = currentcmd.oprv;
			while(nodeptr != NULL && nodeptr -> data[0] != '\0') {
				DC += get_op_size(nodeptr -> data);
				nodeptr = nodeptr -> next;
			}
			continue;
		}

		if(!strcmp(currentcmd.action, "entry")) {
			addentry2list(currentcmd.oprv -> data, linenum);
			continue;
		}

		if(!strcmp(currentcmd.action, "extern")) {
			addsymbol2list(currentcmd.oprv -> data, 0, external, FALSE);
			continue;
		}

		if(currentcmd.label[0] != '\0')
			addsymbol2list(currentcmd.label, IC, code, FALSE);

		IC++;
		nodeptr = currentcmd.oprv;

		while(nodeptr) {
			IC += get_op_size(nodeptr -> data);
			nodeptr = nodeptr -> next;
		}

		/*print_test(currentcmd);*/
	}
	freelist(&(currentcmd.oprv));

	/*printf("-----------------------------\n");
	print_symbol_table();
	printentries();*/

	addIC2data(IC);
	if((error = addentries2symbols())) {
		valid = FALSE;
		printf("%d: ", error);
		print_error(Symbol_not_exist);
	}

	*ICF = IC;
	*DCF = DC;

	if(valid)
		return 0;	
	return 1;
}
