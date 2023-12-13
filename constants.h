/*This file holds all the constatns of the code*/
#ifndef constants
#define constants

#define MAX_LINE_SIZE 80
#define MAX_LINE_LEN 80
#define MAX_LABEL_LEN 30
#define MAX_MACRO_NAME 80
#define IC_REAL_VALUE 100
#define MACRO "macro"
#define ENDMACRO "endmacro"
#define MAX_PROGRAM_SIZE 256
extern int IC , DC;/* the instruction counter and data counter*/
extern int dataImage[MAX_PROGRAM_SIZE];
extern int instructionImage[MAX_PROGRAM_SIZE];
extern int same_instruction_flag;
extern FILE * externals_file;
extern FILE * entries_file;
extern FILE * object_file;
extern char *base32symbols[]; 


extern int dataImage[];
extern int instructionImage[];


typedef struct line_info {/*This struct holds the line number*/
	int line_number;
} line_info;

typedef enum ARE {A = 0 ,R = 1,E = 2} ARE;/*the types or ARE*/

typedef enum type_of_label {REGULAR,ENTRY,EXTERNAL} type_of_label;/*enum for label types(see 11 in second pass*/

typedef struct label /*the strutct that reprsents a label*/
{
	/*name,type,*/
	char label_name[MAX_LABEL_LEN+2];
	type_of_label type_of_label;
	int counter_value;/*see data type*/
} label;
 
/*we'll define labels with linked list*/
typedef struct label_node * lptr; /*pointer to labelNode object*/

typedef struct label_node{		 /*labelNode data type. includes label object and a pointer to the next labelNode object in list*/
 label label;
 lptr next;
} label_node;


#endif
