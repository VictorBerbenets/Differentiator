#ifndef Validator_h
#define Validator_h

#define Validator(expression, text, ...)                                                                                      \
        if (expression) {                                                                                                      \
        fprintf(stderr, "" White "%s:%d:" Gray " In function " White " '%s': " Gray " \n", __FILE__, __LINE__, __FUNCTION__);   \
        fprintf(stderr, "" White "%s:%d:" Red " error:" Gray "%s\n\t| " Blue " \t%s " Gray " "                                   \
        "\n\t|\n", __FILE__, __LINE__, text, #expression);                                                                       \
        __VA_ARGS__;                                                                                                               \
    }  
#endif