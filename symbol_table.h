
/*adds a label to the label list*/
int addLabelToList(lptr *labelPtr, char *name_of_label, type_of_label type_of_label,int if_declaration_flag);

/*finds a label in the label list*/
label *findLabel(lptr h, char *name_of_label);

/*chekcs if a label is type entry and if yes returns a pointer to it */
void printEntryLabel(lptr h);

/*frees all the memory allocated to the label list */
void freeLabelList(lptr h);

/*finds a label type in the label list*/
type_of_label findLabelType(lptr h, char *name_of_label);
