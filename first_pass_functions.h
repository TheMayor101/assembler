
/*function to check if a string is a label*/
int isLabel(char *p);

/*function to check is register*/
int CheckisRegister(char *p);

/*function to check is a string is a guidance*/
int isGuidance(char *p);

/*get p(char) only word*/
int check_label(char *p, line_info curr_line_info);

/*returns if a line is a string*/
int iswhiteline(char *linep);

/*returns the number of the op code*/
int operations_code(char * str);

/*here will be all the first pass code*/
int first_pass(FILE * first_pass_file , lptr *labelPtr);

/*this function checks the Addressing_method of each param of the opcode*/
int Addressing_method(char *word);

/*searches the first point in a string*/
int search_for_point(char * word);

/*removes spaces for a string*/
void remove_spaces(char* s);

/*converts a base10 number to a base32 string*/
char* convert_base_10_to_32(int num);

/*reverses a given string*/
void reverse_string(char* str);


