#include "../headers/files_func.h"

#include <assert.h>
#include <sys/stat.h>
#include <stdlib.h>


off_t MeasureFile(const char* filename) {
    assert(filename);
    struct stat file_param = {};
    stat(filename, &file_param);

    return file_param.st_size;
}

int FileCopy(file_info *file_inf, const char *filename) {
    assert(file_inf); assert(filename);

    file_inf->file_length = MeasureFile(filename);
    file_inf->file_copy = (char*) calloc((size_t) file_inf->file_length + 1, sizeof(char));

    FILE* fp = fopen(filename, "r");
    if (!fp) {return -fprintf(stderr, "%s:%d: Can't open file %s\n", __FILE__, __LINE__, filename);}
    fread(file_inf->file_copy, sizeof(char), (size_t) file_inf->file_length, fp);
    fclose(fp);

    return 0;
}
