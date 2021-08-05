#include "advanced.h"

typedef struct {
	unsigned int E     :1;
	unsigned int R     :1;
	unsigned int A     :1;
	unsigned int funct :5;
	unsigned int desReg:3;
	unsigned int desAdd:2;
	unsigned int srcReg:3;
	unsigned int srcAdd:2;
	unsigned int opcode:6;
	unsigned int fill  :sizeof(int)*8 - 24;
} binarycmd;

typedef struct {
	unsigned int E      :1;
	unsigned int R      :1;
	unsigned int A      :1;
	unsigned int content:21;
	unsigned int fill   :sizeof(int)*8 - 24;
} binaryword; 

/*********************************************************************************************************
 Goes through all the rows in a given file and convert them as needed (according to the assembler's logic)
 *********************************************************************************************************/
int secondPass(char filename[], FILE *srcFile, FILE *object, FILE *entries, FILE *externals, int ICF, int DCF) {

	int IC = ADDRESS_FOR_LOADING; /* count the number of command lines */
	int DC = 0; /* count the number of data lines */
	int error;
	int linenum = 0;
	int i;
	int firstrowsize;

	char line[MAX_ROW_SIZE];
	char *action;

	node *nodeptr;
	boolean valid = TRUE;
	const cmd emptycmd = {"\0", "\0", NULL};
	cmd currentcmd = emptycmd;
	binarycmd binarycurrentcmd;
	binaryword binarycurrentword;

	binarycurrentcmd.fill = 0;
	binarycurrentword.fill = 0;

	fprintf(object, "%7d %d\n", ICF - ADDRESS_FOR_LOADING, DCF);
	firstrowsize = ftell(object);

	initobjfile(&object, ICF + DCF - ADDRESS_FOR_LOADING, firstrowsize);
	
	while(fgets(line, MAX_ROW_SIZE-1, srcFile)) {

		/*printf("-----------------------------\n");
		printf("line: %s\n",line);*/

		linenum++;
		freelist(&(currentcmd.oprv));
		currentcmd = emptycmd;

		if((error = simple_extractor(line, &currentcmd))) {
			valid = FALSE;
			printf("%s.as:%d: ", filename, linenum);
			print_error(error);
			continue;
		}
		action = currentcmd.action;

		if(isentry(currentcmd.label))
			fprintf(entries, "%s %07d\n", currentcmd.label, get_address(currentcmd.label));

		if(!strcmp(currentcmd.action, "\0"))
			continue;

		if(!strcmp(action, "data")) {
			nodeptr = currentcmd.oprv;
			while (nodeptr != NULL) {
				add2objfile(&object, DC + ICF, atoi(nodeptr -> data), firstrowsize);
				DC++;
				nodeptr = nodeptr -> next;
			}
			continue;
		}

		i = 1;
		if(!strcmp(action, "string")) {
			while (currentcmd.oprv -> data[i] != '\"') {
				add2objfile(&object, DC + ICF, currentcmd.oprv -> data[i], firstrowsize);
				DC++;
				i++;
			}
			add2objfile(&object, DC + ICF, 0, firstrowsize);
			DC++;
			continue;
		}

		if(!strcmp(action, "extern"))
			continue;

		if(!strcmp(action, "entry"))
			continue;
		
		binarycurrentcmd.E = 0;
		binarycurrentcmd.R = 0;
		binarycurrentcmd.A = 1;
		binarycurrentcmd.funct = get_funct(action);
		binarycurrentcmd.opcode = get_opcode(action);

		if(!strcmp(action, "stop") || !strcmp(action, "rts")) {
			binarycurrentcmd.desReg = 0;
			binarycurrentcmd.desAdd = 0;
			binarycurrentcmd.srcReg = 0;
			binarycurrentcmd.srcAdd = 0;
		} else if(!strcmp(action, "mov") || !strcmp(action, "cmp") || !strcmp(action, "add") 
				|| !strcmp(action, "sub") || !strcmp(action, "lea")) {
			binarycurrentcmd.desReg = get_register(currentcmd.oprv -> next -> data);
			binarycurrentcmd.desAdd = get_method(currentcmd.oprv -> next -> data);
			binarycurrentcmd.srcReg = get_register(currentcmd.oprv -> data);
			binarycurrentcmd.srcAdd = get_method(currentcmd.oprv -> data);
		} else if(!strcmp(action, "clr") || !strcmp(action, "not") || !strcmp(action, "inc")
				|| !strcmp(action, "dec") || !strcmp(action, "jmp") || !strcmp(action, "bne")
				|| !strcmp(action, "jsr") || !strcmp(action, "red") || !strcmp(action, "prn")) {
			binarycurrentcmd.desReg = get_register(currentcmd.oprv -> data);
			binarycurrentcmd.desAdd = get_method(currentcmd.oprv -> data);
			binarycurrentcmd.srcReg = 0;
			binarycurrentcmd.srcAdd = 0;
		}

		add2objfile(&object, IC, *((unsigned int *) &binarycurrentcmd), firstrowsize);
		IC++;
	
		nodeptr = currentcmd.oprv;
		if(!nodeptr)
			continue;

		while (nodeptr) {
			binarycurrentword.E = 0;
			binarycurrentword.R = 0;
			binarycurrentword.A = 0;
			binarycurrentword.content = 0;

			switch(get_method(nodeptr -> data)) {
				case 0:
					binarycurrentword.A = 1;
					strcpy(nodeptr -> data, nodeptr -> data + 1);
					binarycurrentword.content = atoi(nodeptr -> data);
					break;
				case 1:
					if(symbol_exist(nodeptr -> data) == external) {
						fprintf(externals, "%s %07d\n", nodeptr -> data, IC);
						binarycurrentword.E = 1;
					} else
						binarycurrentword.R = 1;
					binarycurrentword.content = get_address(nodeptr -> data);
					break;
				case 2:
					binarycurrentword.A = 1;
					strcpy(nodeptr -> data, nodeptr -> data + 1);
					binarycurrentword.content = get_distance(nodeptr -> data, IC);
					break;
				default:
					nodeptr = nodeptr -> next;
					continue;
			}

			add2objfile(&object, IC, *((unsigned int *) &binarycurrentword), firstrowsize);
			IC++;
			nodeptr = nodeptr -> next;
		}
		/*print_test(currentcmd);*/
	}
	freelist(&(currentcmd.oprv));

	if(valid)
		return 0;	
	return 1;

}
