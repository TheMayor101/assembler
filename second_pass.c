/*this will be the file of the second pass*/

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "second_pass.h"
#include "constants.h"
#include "symbol_table.h"
#include "first_pass_functions.h"


/*bytes 6,7,8,9 = operation_code(first word);
bytes 4,5 = addressing_methos(third_word);
bytes 2,3 = addressing_methos(seond_word);
bytes 0,1 = ARE 
*/

int encodeOperand(lptr labelPtr, char *oper1, char *oper2, line_info curr_line_info)
{

	int count_errors = 0;
	IC++;
	same_instruction_flag = 0;
	
	/*move 4 bits to get the number*/
	if(oper1)
	    count_errors |= encodeOperator(labelPtr, oper1, Addressing_method(oper1), 1,curr_line_info);
	if(oper2){
	    if(Addressing_method(oper1) == 3 && Addressing_method(oper2) == 3)
	        same_instruction_flag = 1;
	    count_errors |= encodeOperator(labelPtr, oper2, Addressing_method(oper2), 0,curr_line_info);
	}
	   
	
	return count_errors;
}


int encodeOperator(lptr labelPtr, char *oper, int operType, int source_op_flag, line_info curr_line_info)
{
    char* tmp_oper;
	label *tmp_label;
	int tmp_addresing0_number;
	char *end_char; /*for saving*/
	int tmp_field;


	/*see page 18*/
	switch(operType)
	{
		case 0:/*first addresing method*/
			tmp_addresing0_number = (int) strtol(oper+1, &end_char, 10);
			instructionImage[IC] = tmp_addresing0_number << 2;
			IC++;
			break;
		case 1:/*second addresing method*/
			if(findLabel(labelPtr, oper)==0){
				printf("Error in line %d: can't find label %s\n", curr_line_info.line_number, oper);
				IC++;
				return 1;
			}
			
			tmp_label = findLabel(labelPtr, oper);
			if(tmp_label->type_of_label == EXTERNAL){
			    fprintf(externals_file,"%s\t%s\n", tmp_label->label_name,convert_base_10_to_32(IC+IC_REAL_VALUE));/*printing into the externals file*/
				instructionImage[IC] = R;
			}else if(tmp_label->type_of_label == ENTRY){
                fprintf(entries_file,"%s\t%s\n", tmp_label->label_name,convert_base_10_to_32(IC+IC_REAL_VALUE));/*printing into the entries file*/
				instructionImage[IC] = R;
			}else{
				instructionImage[IC] = (tmp_label->counter_value << 2 | E);
			}
			IC++;
			break;
		case 2:/*third addresing method*/
		
		    tmp_oper = strtok(oper,".");
			if(findLabel(labelPtr, tmp_oper) == 0){
				printf("Error in line %d: can't find label %s\n", curr_line_info.line_number, oper);
				IC++;
				return 1;
			}
			tmp_label = findLabel(labelPtr, tmp_oper);
			if(tmp_label->type_of_label == EXTERNAL){
                fprintf(externals_file,"%s\t%s\n", tmp_label->label_name,convert_base_10_to_32(IC+IC_REAL_VALUE));/*printing into the externals file*/
				instructionImage[IC] = R;
			}else if(tmp_label->type_of_label == ENTRY){
                fprintf(entries_file,"%s\t%s\n", tmp_label->label_name,convert_base_10_to_32(IC+IC_REAL_VALUE));/*printing into the entries file*/
				instructionImage[IC] = R;
			}else{
				instructionImage[IC] = (tmp_label->counter_value << 2 | E);
				
			}
			tmp_oper = strtok(NULL,",");
			tmp_field = atoi(tmp_oper);
			switch(tmp_field)
			{
				case 1:
					IC++;
					instructionImage[IC] = 1 << 2;
					break;
				case 2:
					IC++;
					instructionImage[IC] = 2 << 2;
					break;
			}
			IC++;
			break;
		case 3:/*forth addresing method*/
			if(same_instruction_flag == 1)
			{
					instructionImage[IC-1] |=  atoi(oper+1) << 2;
			}else{
			    if(source_op_flag)
					instructionImage[IC] |=  atoi(oper+1) << 6;
			    else
			        instructionImage[IC] |=  atoi(oper+1) << 2;
			    IC++;
			}
			break;
	}
	return 0;
}




int secondPass(FILE *dest_file, lptr labelPtr)
{
	char *token;
	char tmp_line[MAX_LINE_LEN+2];
	int type_of_guide = -1;/*we give each guidence word a number*/
	line_info curr_line_info;
	char *oper1 ,*oper2 ;
    	int error_in_line_flag;
    	error_in_line_flag = 0;
	for(curr_line_info.line_number = 1; fgets(tmp_line, MAX_LINE_SIZE + 2 , dest_file) != NULL; curr_line_info.line_number++)
	{
		if (iswhiteline(tmp_line))
		{
			continue;
		}
		if(tmp_line[0] == ';')/*note line*/
		{
			continue;
		}
		token = strtok(tmp_line," ");
		/*if there is label in line save label in token*/
		if(!token)/*if token is empty*/
			continue;
        
		if(isLabel(token))
			token = strtok(NULL," ");
		type_of_guide = isGuidance(token);
		 if(type_of_guide < 4 && type_of_guide != -1)
		      continue;
		 else if(type_of_guide == 4)/*entry type*/
		  {
		       continue;
		  }
		  else/*if token is label see 3*/
		  {
	        	token = strtok(NULL,"\n");
	        	if(token)
	        	    remove_spaces(token);
			    oper1 = strtok(token,",");
			    
			     if(!oper1){/*for hlt and rts. other 14-15*/
				    IC++;
				    continue;
			     }

			     oper2 = strtok(NULL,",");
			     
			     if(oper2)
			     {
			         
    			     if(encodeOperand(labelPtr, oper1, oper2, curr_line_info))
    			     {
    				    error_in_line_flag = 1;
    			     }  
			     }
			     else
			     {
			         
			         if(encodeOperand(labelPtr, oper1,0,curr_line_info))
			            error_in_line_flag = 1;
			     }
		       }
    }
    return error_in_line_flag;
}




