#include "PikaStdData_FILEIO.h"
#include <stdio.h>

void PikaStdData_FILEIO_init(PikaObj* self, char* mode, char* path) {
    if (obj_isArgExist(self, "_f")) {
        /* already initialized */
        return;
    }
    FILE* f = __platform_fopen(path, mode);
    if (f == NULL) {
        printf("Error: can't open file %s\n", path);
        return;
    }
    obj_setPtr(self, "_f", f);
    obj_setStr(self, "_mode", mode);
}

void PikaStdData_FILEIO_close(PikaObj* self) {
    FILE* f = obj_getPtr(self, "_f");
    if (f == NULL) {
        return;
    }
    __platform_fclose(f);
    obj_setPtr(self, "_f", NULL);
}

Arg* PikaStdData_FILEIO_read(PikaObj* self, int size) {
    if (size <= 0) {
        /* read all */
        size = PIKA_READ_FILE_BUFF_SIZE;
    }
    FILE* f = obj_getPtr(self, "_f");
    if (f == NULL) {
        return NULL;
    }
    Arg* buf_arg = arg_setBytes(NULL, "", NULL, size);
    uint8_t* buf = arg_getBytes(buf_arg);
    /* read */
    int n = __platform_fread(buf, 1, size, f);
    if (n < size) {
        /* EOF */
        buf[n] = '\0';
    }
    char* mode = obj_getStr(self, "_mode");
    if (strIsContain(mode, 'b')) {
        /* binary */
        Arg* res = arg_setBytes(NULL, "", buf, n);
        arg_deinit(buf_arg);
        return res;
    } else {
        /* text */
        Arg* res = arg_setStr(NULL, "", (char*)buf);
        arg_deinit(buf_arg);
        return res;
    }
}

void PikaStdData_FILEIO_write(PikaObj* self, Arg* s) {
    FILE* f = obj_getPtr(self, "_f");
    if (f == NULL) {
        obj_setErrorCode(self, PIKA_RES_ERR_IO);
        __platform_printf("Error: can't write to file\n");
        return;
    }
    char* mode = obj_getStr(self, "_mode");
    if (strIsContain(mode, 'b')) {
        /* binary */
        __platform_fwrite(arg_getBytes(s), 1, arg_getSize(s), f);
    } else {
        /* text */
        char* str = arg_getStr(s);
        __platform_fwrite(str, 1, strlen(str), f);
    }
}
