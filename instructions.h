/*A function that checks the commas placment*/
int checkComma(char *p,line_info curr_line_info);

/*A function that handles data instructions*/
int storeData(char *p,line_info curr_line_info);

/*A function that handles string instructions*/
int storeString(char *p);

/*A function that handles struct instructions*/
int storeStruct(char *p,line_info curr_line_info);

/*A fucntion that stores the instructions*/
int store_instruction(int type_of_opcode, int source, int dest);

/*A function that process the different types of instructions*/
int process_instruction(int type_of_opcode, char *p,line_info curr_line_info);
