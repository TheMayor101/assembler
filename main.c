#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "constants.h"
#include "first_pass_functions.h"
#include "symbol_table.h"
#include "macro_file_word.h"
#include "second_pass.h"



void createObjectFile()
    {
	int i;
	FILE *object_file;
        object_file = fopen("object_file.ob", "w");
    	for(i=0; i < IC; i++){
    		fprintf(object_file,"%s\t%s\t\n",convert_base_10_to_32(i+IC_REAL_VALUE),convert_base_10_to_32(instructionImage[i]));
    	}
    	for(i = 100; i<DC ; i++){
    		fprintf(object_file,"%s\t%s\t\n",convert_base_10_to_32(i+IC),convert_base_10_to_32(dataImage[i]));
    	}
    	fclose(object_file);
    }

void remove_entries(char * file_name){

    entries_file = fopen(file_name, "r");
	fseek(entries_file, 0, SEEK_SET);
	fgetc(entries_file);

 fgetc(entries_file);

if (feof(entries_file))
    {
    remove(file_name);
    }
}

void remove_externals(char * file_name){

    externals_file = fopen(file_name, "r");
	fseek(externals_file, 0, SEEK_SET);
	fgetc(externals_file);

	if (feof(externals_file))
    	{
    	remove(file_name);
    	}
}

/*the main function of the program*/
int main(int argc, char *argv[]){
    int i;
    FILE * first_pass_file;
    FILE * second_pass_file;
    lptr labelPtr; 
    FILE * macros_file;
    FILE * updated_macros_file;
    char file_name[FILENAME_MAX];
    
    for (i=1 ; i < argc; i++) 
    {
        DC = 100;IC = 0;/*the starting index of the data instructions is 100*/
        labelPtr = NULL;
        
        strcpy(file_name, argv[i]);
		strcat(file_name, ".as");
		
		macros_file = fopen(file_name, "r");
		if(!macros_file){
			printf("CANNOT FIND FILE\n");
			break;
		}
		printf("macros now\n");
		updated_macros_file = fopen("updated_macros_file.am", "w");
        replace_macros(macros_file, updated_macros_file);/*calling the function that handles the file with macros*/
        fclose(macros_file);
        fclose(updated_macros_file);
        
        externals_file = fopen("externals_file.ext", "w");
        entries_file = fopen("entries_file.ent", "w");
        
        
        first_pass_file = fopen("updated_macros_file.am", "r");
        	printf("now first pass\n");
        if(first_pass(first_pass_file, &labelPtr)){
			printf("First pass failed\n");
			fclose(first_pass_file);
			freeLabelList(labelPtr);
			break;
		}
		printf("now second pass\n");
		IC = 0;
		second_pass_file = fopen("updated_macros_file.am", "r");
		if(secondPass(second_pass_file,labelPtr)){/*starting second pass*/
	        printf("Second pass failed, aborting assembly for current file\n");
	        fclose(second_pass_file);
	        freeLabelList(labelPtr);
	        break;
		}
		fclose(second_pass_file);
		
		printf("creating files\n");
		printEntryLabel(labelPtr);
		createObjectFile();
	    fclose(entries_file); 
	    fclose(externals_file); 
	    
	    freeLabelList(labelPtr);
        printf("end of project");
	}
	return 0;
}
