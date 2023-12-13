#include<stdio.h>
#include<stdlib.h>
#define MAX_PROGRAM_SIZE 256	/*max program size line count to be assembled*/
int dataImage[MAX_PROGRAM_SIZE];/* an array that holds all the data */
int instructionImage[MAX_PROGRAM_SIZE];/* an array that hold all the instructions*/
int IC, DC;/*the instruction counter and data counter*/
int same_instruction_flag;
FILE * externals_file;
FILE * entries_file;
/*an array which holds all the 32 base symbols*/
char *base32symbols[] = {"!","@","#","$","%","^","&","*","<",">","a","b","c","d","e","f","g","h","i","j","k","l","m","n","o","p","q","r","s","t","u","v"};
