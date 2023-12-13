FLAGS = -Wall -ansi -pedantic

all: Assembler

Assembler: main.o macro_file_word.o first_pass_functions.o instructions.o symbol_table.o constants.o second_pass.o
	gcc $(FLAGS) main.o macro_file_word.o first_pass_functions.o instructions.o symbol_table.o constants.o second_pass.o -o Assembler

main.o: constants.h first_pass_functions.h symbol_table.h macro_file_word.h second_pass.h instructions.h
	gcc -c $(FLAGS) main.c

macro_file_word.o: macro_file_word.h
	gcc -c $(FLAGS) macro_file_word.c

first_pass_functions.o: constants.h instructions.h first_pass_functions.h symbol_table.h
	gcc -c $(FLAGS) first_pass_functions.c

instructions.o: constants.h instructions.h first_pass_functions.h symbol_table.h
	gcc -c $(FLAGS) instructions.c

symbol_table.o: constants.h instructions.h first_pass_functions.h symbol_table.h
	gcc -c $(FLAGS) symbol_table.c

constants.o: constants.h
	gcc -c $(FLAGS) constants.c

second_pass.o: second_pass.h constants.h symbol_table.h first_pass_functions.h
	gcc -c $(FLAGS) second_pass.c

.PHONY: all clean

clean: 
	rm -f macro_file_word.o first_pass_functions.o instructions.o symbol_table.o constants.o main.o second_pass.o Assembler
