prog: assembler.o firstPass.o secondPass.o linefunc.o utilities.o validation.o prints.o files.o
	gcc -g -Wall -ansi -pedantic assembler.o firstPass.o secondPass.o linefunc.o utilities.o validation.o prints.o files.o -o prog
assembler.o: assembler.c advanced.h
	gcc -c -Wall -ansi -pedantic assembler.c -o assembler.o
firstPass.o: firstPass.c advanced.h
	gcc -c -Wall -ansi -pedantic firstPass.c -o firstPass.o
secondPass.o: secondPass.c advanced.h
	gcc -c -Wall -ansi -pedantic secondPass.c -o secondPass.o
prints.o: prints.c advanced.h
	gcc -c -Wall -ansi -pedantic prints.c -o prints.o
linefunc.o: linefunc.c basic.h
	gcc -c -Wall -ansi -pedantic linefunc.c -o linefunc.o
utilities.o: utilities.c basic.h
	gcc -c -Wall -ansi -pedantic utilities.c -o utilities.o
validation.o: validation.c basic.h
	gcc -c -Wall -ansi -pedantic validation.c -o validation.o
files.o: files.c basic.h
	gcc -c -Wall -ansi -pedantic files.c -o files.o
