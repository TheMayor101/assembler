/*in this file there are 3 functions that handle string, data, and struct instructions*/
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "constants.h"
#include "instructions.h"
#include "first_pass_functions.h"
#include "symbol_table.h"

int checkComma (char *p, line_info curr_line_info)
{
  if (*p == ',')
    {
      printf ("Error in line %d: Comma is dispositioned\n",
	      curr_line_info.line_number);
      return 0;
    }
  p++;
  while (*p)
    {
      if (*p == ',')
	{
	  if (*(p + 1) == '\0')
	    {
	      printf ("Error in line %d: Comma is dispositioned\n",
		      curr_line_info.line_number);
	      return 0;
	    }
	  if (*(p + 1) == ',')
	    {
	      printf ("Error in line %d: Multiple consecutive commas\n",
		      curr_line_info.line_number);
	      return 0;
	    }
	}
      p++;
    }
  return 1;
}

int storeData (char *p, line_info curr_line_info)
{
  char *token;
  char *endChar = NULL;
  int tempDC = DC;
  int tempNum;
  if (!checkComma (p, curr_line_info))
  {
      printf("Error in line %d: Data guidance parameter is incorrect, aborting storage action\n",
	 curr_line_info.line_number);
      DC = tempDC;
      return 1;
  }
  token = strtok (p, ",");
  while (token)
    {
      tempNum = (int) strtol (token, &endChar, 10);
      if (tempNum >= 512 || tempNum < -512)
	{
	  printf("Error in line %d: Natural number parameter %s is too large to fit in memory\n",curr_line_info.line_number, token);
	  DC = tempDC;
	  return 1;
	}
      dataImage[DC] = tempNum;
      DC++;IC++;
      token = strtok (NULL, ",");
    }
  return 0;

}

int storeString (char *p)
{
  int i = 0;
  while (p[i] != '\0')
    {
      if (!(p[i] == '"'))
	{
	  dataImage[DC] = p[i];
	  DC++;IC++;
	}
      i++;
    }
  dataImage[DC] = '\0';
  DC++;IC++;
  return 0;
}

int storeStruct (char *p, line_info curr_line_info)
{   
  char *token;
  char *endChar = NULL;
  int tempDC = DC;
  int tempNum;
  int i;
  remove_spaces(p);
  if (!checkComma (p, curr_line_info))
    {
      printf("Error in line %d: Data guidance parameter is incorrect, aborting storage action\n",curr_line_info.line_number);
      DC = tempDC;
      return 1;
    }
  token = strtok (p, ",");

  tempNum = (int) strtol (token, &endChar, 10);
  if (tempNum >= 512 || tempNum < -512)
    {
      printf("Error in line %d: Natural number parameter %s is too large to fit in memory\n",curr_line_info.line_number, token);
      DC = tempDC;
      return 1;
    }
  dataImage[DC] = tempNum;
  DC++;
  token = strtok (NULL, ",");
  i=0;
  while (token[i] != '\0')
    {
      if (!(token[i] == '"'))
	{
	  dataImage[DC] = token[i];
	  DC++;IC++;
	}
      i++;
    }
  dataImage[DC] = '\0';
  DC++;IC++;
  return 0;
}

/*source and dest are 2-5 bytes*/

int store_instruction (int type_of_opcode, int source, int dest)
{
  int tmp_DC = 0;
  int tmp_IC = 0;
  tmp_DC |= (type_of_opcode << 6);	/*tmp_DC will be ????-00-00-00 */
  /*counting lines now and update IC */
  if (dest >= 0)
    {
      if (dest == 2)
	{
	  tmp_IC += 2;
	}
      else
	{
	  tmp_IC += 1;
	}
      tmp_DC |= (dest << 2);	/*tmp_DC will be ????-00-??-00 */
    }
    
  if (source >= 0)
    {
      if (source == 2)
	{
	  tmp_IC += 2;
	  
	}else
	{
	  tmp_IC += 1;
	}
      tmp_DC |= (source << 4);
    }
    
  instructionImage[IC] = tmp_DC;
  if(!(source == 3 && dest == 3))
    IC++;
  IC += tmp_IC;
  return 0;
}

/*check this*/
int process_instruction (int type_of_opcode, char *p,line_info curr_line_info)
{
    
  char *token1, *token2;
  int oper1 = 0, oper2 = 0;

    
  token1 = strtok (p, ",");
  token2 = strtok (NULL, "\0");
  if (p)
    {
      /*bytes 4-5 are 0 */
     if ((type_of_opcode > 3 && type_of_opcode != 6)){
        if(token1 && token2)
	        {
	            printf("Error in line %d: too many operands\n", curr_line_info.line_number);
	        }
	        remove_spaces(token1);
	        oper1 = Addressing_method(token1);
	    return (store_instruction (type_of_opcode, -1, oper1));
    }
    else
	{
	    if(!token1 && !token2)
	        {
	            printf("Error in line %d: too few operands\n", curr_line_info.line_number);
	        }
	        remove_spaces(token1);
	        remove_spaces(token2);
	        oper1 = Addressing_method(token1);
	        oper2 = Addressing_method(token2);
	    return (store_instruction (type_of_opcode, oper1, oper2));
	}
    }
    
      /*bytes 2-5 are 0 */
    if (type_of_opcode == 14 || type_of_opcode == 15)
	    {
	        if(token1 || token2) 
	        {
	            printf("Error in line %d: too many operands\n", curr_line_info.line_number);
	        }
	        return (store_instruction (type_of_opcode, -1, -1));
	    }

  printf ("Error: check line %d",curr_line_info.line_number);
  return 1;
}
