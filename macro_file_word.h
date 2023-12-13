typedef struct macro_info{/*a struct that will hold the data of each macro*/
		char name[MAX_LINE_SIZE];/*the name of the macro*/
		char data[MAX_LINE_SIZE];/*the data of the macro*/
} macro_info;

/*this funciton takes a file with macros and creates a new file after handleing the macros*/
void replace_macros(FILE *original_file, FILE *updated_file);
