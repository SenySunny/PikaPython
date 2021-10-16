#ifndef __PIKA__PARSER__H
#define __PIKA__PARSER__H
#include "dataQueueObj.h"

typedef QueueObj AST;
AST* pikaParseLine(char* line, Args* blockStack);
char* pikaParseToAsm(Args* buffs, char* line);
int32_t AST_deinit(AST* ast);
char* AST_toPikaAsm(AST* ast, Args* buffs);

#endif