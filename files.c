#include "basic.h"

int delete_file(char filename[]) {
	if(remove(filename))
		return FALSE;
	return TRUE;
}

int delete_all_files(char filename[]) {
	int originallen = strlen(filename);

	strcat(filename, ".ob");
	if(!delete_file(filename)) {
		printf("Cannot delete the file: %s\n", filename);
		return 0;
	}

	filename[originallen] = '\0';
	strcat(filename, ".ext");
	if(!delete_file(filename)) {
		printf("Cannot delete the file: %s\n", filename);
		return 0;
	}

	filename[originallen] = '\0';
	strcat(filename, ".ent");
	if(!delete_file(filename)) {
		printf("Cannot delete the file: %s\n", filename);
		return 0;
	}
	
	filename[originallen] = '\0';
	return 1;
}

int access_files(char filename[], FILE **srcFile, FILE **object, FILE **entries, FILE **externals) {
	int originallen = strlen(filename);

	strcat(filename, ".as");
	if(!(*srcFile = fopen(filename, "r"))) {
		printf("Cannot open the file: %s\n", filename);
		return FALSE;
	}

	filename[originallen] = '\0';
	strcat(filename, ".ob");
	if(!(*object = fopen(filename, "w"))) {
		printf("Cannot create the file: %s\n", filename);
		return FALSE;
	}

	filename[originallen] = '\0';
	strcat(filename, ".ent");
	if(!(*entries = fopen(filename, "w"))) {
		printf("Cannot create the file: %s\n", filename);
		return FALSE;
	}

	filename[originallen] = '\0';
	strcat(filename, ".ext");
	if(!(*externals = fopen(filename, "w"))) {
		printf("Cannot create the file: %s\n", filename);
		return FALSE;
	}

	filename[originallen] = '\0';
	return TRUE;
}

int delete_unused_files_and_close_the_rest(char filename[], FILE **object, FILE **entries, FILE **externals) {
	int originallen = strlen(filename);

	fseek(*object, 0, SEEK_END);

	strcat(filename, ".ob");
	switch(ftell(*object)) {
		case -1:
			return FALSE;
		case 0:
			if(!delete_file(filename))
				return FALSE;
		default:
			fclose(*object);
	} 

	fseek(*entries, 0, SEEK_END);

	filename[originallen] = '\0';
	strcat(filename, ".ent");
	switch(ftell(*entries)) {
		case -1:
			return FALSE;
		case 0:
			if(!delete_file(filename))
				return FALSE;
		default:
			fclose(*entries);
	}

	fseek(*externals, 0, SEEK_END);

	filename[originallen] = '\0';
	strcat(filename, ".ext");
	switch(ftell(*externals)) {
		case -1:
			return FALSE;
		case 0:
			if(!delete_file(filename))
				return FALSE;
		default:
			fclose(*externals);
	}

	filename[originallen] = '\0';
	return TRUE;
}

void close_files(FILE **object, FILE **entries, FILE **externals) {

	if(!(*object))
		fclose(*object);
	if(!(*entries))
		fclose(*entries);
	if(!(*externals))
		fclose(*externals);
}

void add2objfile(FILE **object, int address, unsigned int content, int firstrowsize) {
	int current = ftell(*object);

	fseek(*object, (address - ADDRESS_FOR_LOADING) * 15 + firstrowsize, SEEK_SET);
	fprintf(*object, "%07d %06x\n", address, content & 0xffffff);
	if((address - ADDRESS_FOR_LOADING) * 15 + firstrowsize < current)
		current += 15;
	fseek(*object, current, SEEK_SET);
}

void initobjfile(FILE **object, int rows, int firstrowsize) {
	int i;

	for(i=0; i<rows; i++) {
		fprintf(*object, "0000000 000000\n");
	}
	fseek(*object, firstrowsize, SEEK_SET);
}
