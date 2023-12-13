#include "constants.h"

/*encodes the different types of operands*/
int encodeOperand(lptr labelPtr, char *oper1, char *oper2,line_info curr_line_info);

/*encodes the different types of operatores*/
int encodeOperator(lptr labelPtr, char *oper, int operType, int source_op_flag, line_info curr_line_info);

/*A function that calls the functions that handle the second pass*/
int secondPass(FILE *fptr, lptr labelPtr);

