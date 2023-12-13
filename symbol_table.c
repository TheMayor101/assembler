
/*CHANGE THE NAMES OF THE VAIRABLES AND FUNCTIONS*/
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>


#include "constants.h"
#include "instructions.h"
#include "first_pass_functions.h"
#include "symbol_table.h"



int addLabelToList(lptr *labelPtr, char *name_of_label, type_of_label type_of_label,int if_declaration_flag)
{
    lptr node1 , node2;
	lptr t;
	int counter_value = IC+IC_REAL_VALUE;
	if(if_declaration_flag){
	   counter_value = 0;
	}
	t = (lptr) malloc(sizeof(label_node));
	/*checking*/
	if(!t){
		printf("\n Space allocation Error");
		free(t);
	return 1;
	}
	
	node1 = *labelPtr;
	strcpy(t->label.label_name, name_of_label);
	t->label.type_of_label = type_of_label;
	t->label.counter_value = counter_value;
	t->next = NULL;
		
	if(!node1)
		*labelPtr = t;
	else{
		while (node1){
		    if(!strcmp(node1->label.label_name, t->label.label_name)){
			free(t);
			return 1;
		}
			node2 = node1;
			node1 = node1->next;
		}
		node2->next = t;
	}
	return 0;
}

label *findLabel(lptr h, char *name_of_label){
	while(h){
		if(strcmp(h->label.label_name, name_of_label)==0){
		    
			return &(h->label);
		}
		h = h->next;
	}
	return 0;
}

type_of_label findLabelType(lptr h, char *name_of_label){
	while(h){
		if(strcmp(h->label.label_name, name_of_label)==0){
		    
			return (h->label.type_of_label);
		}
		h = h->next;
	}
	return -1;
}

void printEntryLabel(lptr h){
	while(h){
		if(h->label.type_of_label == ENTRY){
			fprintf(entries_file,"%s\t%s\n", h->label.label_name,convert_base_10_to_32(h->label.counter_value));/*printing into the entries_file*/
		}
		h = h->next;
	}
} 


void freeLabelList(lptr h){
	lptr t;
	while(h){
		t = h->next;
		free(h);
		h = t;
	}
}

