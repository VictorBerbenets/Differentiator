#ifndef DSL_H
#define DSL_H

#define PrintWarningForDivisor() fprintf(stderr, "" Purple "" White"Warning:" Grey "" White "%s:%d:" Grey "\n\t|Trying to divide by 0\n",\
                                __PRETTY_FUNCTION__, __LINE__);

#define PrintWarningInvalidOper() fprintf(stderr, "" Purple "" White"Warning:" Grey "" White "%s:%d:" Grey "\n\t|Invalid operator: %c\n",\
                                __PRETTY_FUNCTION__, __LINE__, node_ptr->value.oper); 

#endif