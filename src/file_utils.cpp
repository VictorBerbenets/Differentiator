#include "..//include//differentiator.h"
#include <sys/stat.h>
#include <sys/types.h>


// size_t GetFileSize (const char *file) { 
    
//     Validator(!file, "invalid file name", exit(ERROR_IN_READING_FILE););

//     struct stat buf = {};
//     if (stat(file, &buf)) {

//         fprintf(stderr, "\n" Red "error" Gray "!!!\nFile <%s>: in function <%s>,  in line '%d' - error in function \"stat\"\n\n", __FILE__, __PRETTY_FUNCTION__, __LINE__);
//         return ERROR_IN_READING_FILE;
//     }

//     return buf.st_size;
// }


size_t GetStringSize (FILE* TreeFile) { 
    
    Validator(!TreeFile, "invalid file name", exit(INVALID_DATA););
    int char_counter = 0;
    char ch = 0;
    while((ch = fgetc(TreeFile)) != '\n' && ch != EOF && ch != '\0') {
        char_counter++;
        printf("ch = <%c>\n", ch);
    }
    printf("AAAAAAAAAAAAAAAAAAAAAAAAAAAAA\n");
    rewind(TreeFile);
    return char_counter;
}