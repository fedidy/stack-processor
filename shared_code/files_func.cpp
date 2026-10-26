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

    file_inf->length = MeasureFile(filename);
    file_inf->code = (char*) calloc((size_t) file_inf->length + 1, sizeof(char));

    FILE* fp = fopen(filename, "r");
    if (!fp) {OUTPUT_ERROR("Can't open file %s\n", filename); return -1;}
    fread(file_inf->code, sizeof(char), (size_t) file_inf->length, fp);
    fclose(fp);

    return 0;
}
