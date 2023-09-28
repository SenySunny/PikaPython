#include "PikaVM.h"
#include "dataStrs.h"

extern volatile PikaObj* __pikaMain;
static enum shellCTRL __obj_shellLineHandler_debug(PikaObj* self,
                                                   char* input_line,
                                                   struct ShellConfig* config) {
    /* continue */
    if (strEqu("c", input_line)) {
        return SHELL_CTRL_EXIT;
    }
    /* next */
    if (strEqu("n", input_line)) {
        return SHELL_CTRL_EXIT;
    }
    /* launch shell */
    if (strEqu("sh", input_line)) {
        /* exit pika shell */
        pikaScriptShell((PikaObj*)__pikaMain);
        return SHELL_CTRL_CONTINUE;
    }
    /* quit */
    if (strEqu("q", input_line)) {
        obj_setInt(self, "enable", 0);
        return SHELL_CTRL_EXIT;
    }
    /* print */
    if (strIsStartWith(input_line, "p ")) {
        char* path = input_line + 2;
        Arg* asm_buff = arg_newStr("print(");
        asm_buff = arg_strAppend(asm_buff, path);
        asm_buff = arg_strAppend(asm_buff, ")\n");
        _pikaVM_runPyLines(self->vmFrame->locals, self->vmFrame->globals,
                           arg_getStr(asm_buff), pika_true);
        arg_deinit(asm_buff);
        return SHELL_CTRL_CONTINUE;
    }
    _pikaVM_runPyLines(self->vmFrame->locals, self->vmFrame->globals,
                       input_line, pika_true);
    return SHELL_CTRL_CONTINUE;
}

void PikaDebug_Debuger___init__(PikaObj* self) {
    /* global enable contral */
    obj_setInt(self, "enable", 1);
}

void PikaDebug_Debuger_set_trace(PikaObj* self) {
    if (!obj_getInt(self, "enable")) {
        return;
    }
    struct ShellConfig cfg = {
        .prefix = "(pika-db) ",
        .handler = __obj_shellLineHandler_debug,
        .fn_getchar = __platform_getchar,
    };
    _do_pikaScriptShell(self, &cfg);
}
