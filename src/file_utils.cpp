#include "..//include//differentiator.h"
#include <sys/stat.h>
#include <sys/types.h>

size_t GetStringSize (FILE* TreeFile) { 
    
    Validator(!TreeFile, "invalid file name", exit(INVALID_DATA););
    size_t char_counter = 0;
    char ch = 0;
    while((ch = fgetc(TreeFile)) != '\n' && ch != EOF) {
        printf("CH = <%c>\n", ch);
        char_counter++;
    }
    rewind(TreeFile);
    printf("size = %zd\n", char_counter);
    return char_counter;
}