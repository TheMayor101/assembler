#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_LINE_SIZE 80
#define MAX_MACRO_NAME 80
#define MACRO "macro"
#define ENDMACRO "endmacro"

typedef struct macro_info{
		char name[MAX_LINE_SIZE];/* the name of the macro */
		char data[MAX_LINE_SIZE];/* the data of the macro*/
}macro_info;

void replace_macros(FILE *original_file, FILE *updated_file)
{
	int number_of_macros = 0;
	int macro_flag = 0; /* will reprsent if we found a macro or endmacro*/
	char name[MAX_LINE_SIZE] = {0}; /* the name of the macro */
    	char data[MAX_LINE_SIZE] = {0}; /* the data of the macro*/
    	char * line_copy = NULL;/*this will be a copy of the line we read from the file*/
	int i;
	char * line = NULL;/*this will hold the line we read from the file*/
	macro_info **macros;
	line = malloc(MAX_LINE_SIZE+1);
	macros = malloc((1 + number_of_macros) * sizeof(macro_info*));/*creating the array that will hold the pointers to the structs*/
    while(fgets(line, MAX_LINE_SIZE, original_file))/*here we read the file*/
	{
		line_copy = malloc(MAX_LINE_SIZE);
		strncpy(line_copy, line, MAX_LINE_SIZE);
		
		if (0 == strncmp(strtok(line_copy, " "), MACRO, 5))
		{
			strncpy(name, strtok(NULL, " "), MAX_LINE_SIZE);

			while(fgets(line, MAX_LINE_SIZE, original_file))/*reading the next line in the file*/
			{
				if (0 == strncmp(line, ENDMACRO, 8))
				{

					macros[number_of_macros] = malloc(sizeof(macro_info));/*creating a new struct to put in the array*/
					strncpy(macros[number_of_macros]->name, name,MAX_LINE_SIZE);/*adding the macro name to the macro struct*/
					strncpy(macros[number_of_macros]->data, data,MAX_LINE_SIZE);/*adding the data name to the macro struct*/
					

					fgets(line, MAX_LINE_SIZE, original_file);/*reading the next line*/
					break;
				}
				else{	
		  			strncat(data, line, MAX_LINE_SIZE );
				}
			}
		  
	    
			for (i = 0; i < MAX_LINE_SIZE; i++)
			{
				name[i] = '\0';
			}
			
			for (i = 0; i < MAX_LINE_SIZE; i++)
			{
				data[i] = '\0';
			}
			number_of_macros++;/*increasing the number of macros counter*/
		}  
	
		for(i = 0; i < number_of_macros ; i++)/*we go over the macros array to see if the name of the macro is in the array*/
		{
			if(0 == strncmp(macros[i]->name, line , 5))
			{
			    fputs(macros[i]->data, updated_file);
			    macro_flag = 1;
			}
		}

		if(0 == macro_flag){
			fputs(line, updated_file);
		}
	
		macro_flag = 0;
		free(line_copy);
	}
	
	for(i = 0; i < number_of_macros ; i++)/*freeing the macro struct array*/
	{
	   free(macros[i]);
	}
		
	free(line);
	free(macros);
}
