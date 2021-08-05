#include "advanced.h"

int main(int argc, char *argv[]) {

	FILE *srcFile, *object, *entries, *externals;

	int ICF, DCF;
	int i;

	char *filename = NULL;

	if(argc == 1) {
		printf("You should enter at least one file !\n");
		return 1;
	}

	/* goes through all the files */
	for(i=1; i<argc; i++, free(filename), free_symbols_table(), free_entries()) {

		filename = malloc(strlen(argv[i]) + 4);
		strcpy(filename, argv[i]);

		if(!access_files(filename, &srcFile, &object, &entries, &externals)) 
			continue;

		if(firstPass(argv[i], srcFile, &ICF, &DCF)) { /* if there is any error */
			delete_all_files(filename);
			continue;
		}

		rewind(srcFile);

		if(secondPass(argv[i], srcFile, object, entries, externals, ICF, DCF)) { /* if there is any error */
			delete_all_files(filename);
			continue;
		}

		delete_unused_files_and_close_the_rest(filename, &object, &entries, &externals);
	}

	return 0;
}
