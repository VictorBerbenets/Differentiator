#ifndef DSL_H
#define DSL_H

#define PrintWarning()   fprintf(stderr, "" Purple "" White"Warning:" Grey "" White "%s:%d:" Grey "\n\t|Trying to divide by 0\n", \
                        __PRETTY_FUNCTION__, __LINE__);

#endif