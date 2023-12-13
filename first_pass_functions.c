#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "constants.h"
#include "instructions.h"
#include "first_pass_functions.h"
#include "symbol_table.h"




void reverse_string(char* str)
{
    int last_char_ind = strlen(str) - 1;
    int half_len = last_char_ind / 2;
    int i = 0;
    char c;
    
    while (i <= half_len) {
        c = str[i];
        str[i] = str[last_char_ind - i];
        str[last_char_ind - i] = c;
        i++;
    }
}


char* convert_base_10_to_32(int num)
{
    char* converted;
    int digit;
    char digit_base_32;
    int i = 0;
    converted = malloc(100);
    if(num==0){
        return base32symbols[0];
    }
    if(num < 0){
        num = 1024 + num;
    }
    while (num > 0) {
        digit = num % 32; 
        digit_base_32 = *base32symbols[digit]; 
        converted[i] = digit_base_32;  
        i++;
        num = num / 32;
    }
    converted[i] = '\0';
    reverse_string(converted);
    return converted;

}


void remove_spaces(char* s) {
    char* d = s;
    do {
        while (*d == ' ' || *d== '\n' ) {
            ++d;
        }
    } while ((*s++ = *d++));
}



int isLabel(char *p){
	while(*p){
		if(*p == ':')
			return 1;
		p++;
	}
	return 0;
}

/*function to check is register*/
int CheckisRegister(char *p)
{
	int reg;
	char *null = NULL;
	if(p[0] == 'r')
	{
		reg = (int) strtol(p+1, &null, 10);
		if(reg>=0 && reg<=7){
			return reg;
		}
	}
	return -1;
}

int isGuidance(char *p){
    char *guidance[5] ={".data",".string",".struct",".extern",".entry"}; /*Char array to hold the guidance types */
	int i=0;
	int flag = -1;
	for(i=0;i<5;i++)
    {
        if(strcmp(guidance[i],p)==0)/*we check if the op_code we got matches the saved words*/
        {
            flag = i;
            return flag;
        }
    }
    return flag;
}

/*get p(char) only word*/
int check_label(char *p, line_info curr_line_info){
	char temp[MAX_LABEL_LEN];
	int i = 0;
	int j = 0;
	if(!isalpha(p[i])){
		printf("Error in line %d: Label name contains a char which is not a digit nor a letter\n", curr_line_info.line_number);
		return 0;
	}
	while(p[j] != ':' && p[j] != '\0'&&p[j] != '\n'){
		if(i>MAX_LABEL_LEN){
			printf("Error in line %d: Label name is too long\n", curr_line_info.line_number);
			return 0;
		}
		if(isalnum(p[j])==0){
			printf("Error in line %d: Label name contains a char which is not a digit nor a letter '%d'\n", curr_line_info.line_number,p[j]);
			return 0;
		}
		temp[i] = p[j];
		j++;i++;
	}
	if(CheckisRegister(temp)!=-1 || operations_code(temp)>=0 || isGuidance(temp)!= -1){
		printf("Error in line %d: Label name cannot be register or instruction name\n", curr_line_info.line_number);
		return 0;
	}
	return i;
}

/* return 0 false or 1 true */
int iswhiteline(char *linep)
{
	while (*linep != '\0')
	{
		if (!isspace(*linep))
			return 0;
		++linep;
	}
	return 1;
}



int operations_code(char * word)
{
    int i = 0;
    int flag = -1;
    char * op_code[16] = {"mov" ,"cmp" , "add" , "sub" , "not" , "clr" , "lea" , "inc" ,  "dec" , "jmp" , "bne", "get", "prn" , "jsr" , "rts" , "hlt"};
    for(i=0;i<=15;i++)
    {
        if(strcmp(op_code[i],word)==0)/*we check if the op_code we got matches the saved words*/
        {
            flag = i;
            return flag;
        }
    }
    
    return flag;
}

int search_for_point(char * word)
{
    int i = 0;
	for(i=0; i<sizeof(word);i++){
	    if(word[i] == '.'){
		    return 0;
	    }
	}
	return -1;
}
/*returns the addressing method of the word 0,1,2 or 3*/
/*write the part to the labels*/
int Addressing_method(char *word){
    char * endptr = NULL;
    if(word[0] == '#'){
	    if((int)strtol(word+1, &endptr, 10)){
		    return 0;
	    }
    }
    
	if(search_for_point(word) == -1)
	{
	    
	    if(isGuidance(word) == -1 && CheckisRegister(word)==-1 && operations_code(word) == -1){
	        return 1; /*direct adress*/
	    }
	}
	if(CheckisRegister(word)!=-1){
		return 3;/*direct register adress*/
	}
	return 2;/*access to a string adress*/
}

/*here will be all the first pass code*/
int first_pass(FILE * first_pass_file , lptr *labelPtr) 
{
	int label_flag = 0;
	int error_flag = 0;
	int error_in_line_flag = 0;
	char temp_line[MAX_LINE_SIZE+2];/*2 for the \n*/
	char temp_lable[MAX_LABEL_LEN+2];
	 
    char *token , *token2;
    int labelSize = 30;/*This is the max label size*/
	int typeFlag = 0;
	line_info curr_line_info;
	char line[MAX_LINE_SIZE+2];/*we use an array to keep the content of each line*/
	
    
    
	for(curr_line_info.line_number = 1; fgets(temp_line, MAX_LINE_SIZE + 2 , first_pass_file) != NULL; curr_line_info.line_number++)
	{
	    if (iswhiteline(temp_line))
		{
			continue;
		}
	    error_in_line_flag = 0;
		if (temp_line[0] == ';')/*This means we have a note line*/
		{
			continue;
		}
		strncpy(line,temp_line,MAX_LINE_SIZE+2);
		line[MAX_LINE_SIZE] = '\0';
		token = strtok(temp_line," ");
		
		if(!token){/*means the token is empty*/
			continue;
		}
		
		label_flag = isLabel(token);
		if(label_flag)
		{
			labelSize = check_label(token,curr_line_info);
			if(!labelSize){
				error_flag = 1;
				continue;
			}
			strncpy(temp_lable, token, labelSize);
			temp_lable[labelSize] = '\0'; /*we make sure it's a string*/
			token = strtok(NULL," ");
		} 
		typeFlag = isGuidance(token);
		if(!(typeFlag == -1)) 
		{
			if(!token){
				printf("Error in line %d: Missing parameters\n", curr_line_info.line_number);
				error_flag = 1;
				continue;
			}
		    if(typeFlag == 3){/*if the type is extern*/
		        token = strtok(NULL,"\n");
			    remove_spaces(token);
			    if(check_label(token,curr_line_info) != 0){
			        if(findLabel(*labelPtr,token) != 0){
			            printf("Error in line %d: label: %s already exist\n", curr_line_info.line_number, temp_lable);
					    continue;
			        }
				    else if(addLabelToList(labelPtr, token, EXTERNAL,1) == 1)
				    { 
					    printf("Error in line %d: Failed to add label %s to list\n", curr_line_info.line_number, temp_lable);
					    error_flag = 1;
					    continue;
				    }
			    }
			    
		    }
			else if (typeFlag == 4)/*if the type is entry*/
			{
			    token = strtok(NULL,"\n");
			    remove_spaces(token);
			    
			    if(check_label(token,curr_line_info) != 0){
			        if(findLabel(*labelPtr,token) != 0){
			            printf("Error in line %d: label: %s already exist\n", curr_line_info.line_number, temp_lable);
					    continue;
			        }
				    else if(addLabelToList(labelPtr, token, ENTRY,1) == 1)
				    { 
					    printf("Error in line %d: Failed to add label %s to list\n", curr_line_info.line_number, temp_lable);
					    error_flag = 1;
					    continue;
				    }
			    }
			}/*now we'll check the data,struct and strint instructions*/
			else{
				if(label_flag){
				    if(findLabelType(*labelPtr,temp_lable) == ENTRY){
				            findLabel(*labelPtr,temp_lable)->counter_value = IC+IC_REAL_VALUE;
				     }else if(findLabel(*labelPtr,temp_lable) != 0 && findLabelType(*labelPtr,temp_lable) != EXTERNAL){
					    continue;
			        }
					else{
					    addLabelToList(labelPtr, temp_lable, REGULAR,0);
					}
				}
				switch(typeFlag){
				    case 0:
				        token2 = strtok(NULL,"\n");
					  	error_in_line_flag |= storeData(token2,curr_line_info);
					    if(!token2){
						    error_flag = 1;
						    printf("Error in line %d: Redundant parameters\n", curr_line_info.line_number);
			    		}
				    	break;
				    case 1:
				        token2 = strtok(NULL,"\n");
					    token = strtok(line, "\"");
					    
					   
					    if(token && token2){
					    	error_in_line_flag |= storeString(token2);/*that way we add only if there is error*/
			    		}else{
			    			error_flag = 1;
				    		printf("Error in line %d: Incorrect parameter\n", curr_line_info.line_number);
					    }
					    break;
				    case 2:
				        token2 = strtok(NULL,"\n");
					    error_in_line_flag |= storeStruct(token2,curr_line_info);
						break;
				}

				if(error_in_line_flag){
					printf("Error while processing line %d: Data image storage action failure\n", curr_line_info.line_number);
					error_flag = 1;
				}
			}
		}else{

		    if(label_flag){
		        if(findLabelType(*labelPtr,temp_lable) == ENTRY){
			            findLabel(*labelPtr,temp_lable)->counter_value = IC+IC_REAL_VALUE;
				}else if(findLabel(*labelPtr,temp_lable) != 0 && findLabelType(*labelPtr,temp_lable) != EXTERNAL){
			    continue;
		        }
			    addLabelToList(labelPtr, temp_lable, REGULAR,0);
			}
			remove_spaces(token);
			typeFlag = operations_code(token);
			
			if(typeFlag == -1){
				printf("Error in line %d: '%s' is not an instruction or guidance\n", curr_line_info.line_number, token);
				error_in_line_flag = 1;
			}else{
				token = strtok(NULL,"\n");
				
				
				if(strtok(NULL,"\n")){
				    
					printf("Error in line %d: Redundant parameters\n", curr_line_info.line_number);
					error_flag = 1;
					continue;
				}
				
				if(process_instruction(typeFlag, token, curr_line_info)){
				    
					error_in_line_flag = 1;
				}
				
			}
			
		}
		error_flag |= error_in_line_flag;
	}	
	return error_flag;
		
	
	
}





    
    

